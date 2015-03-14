/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/connection/active.h
// Creator: Li Jie
// Email: cpunion@gmail.com
// Date: 2010-5-10 13:55:00
// 
// $Id: active.h 2754 2010-05-05 10:25:27Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_CONN_ACTIVE_H
#define VENUS_CONN_ACTIVE_H

#ifndef VENUS_BASIC_H
#include "../Basic.h"
#endif

#ifndef VENUS_IO_H
#include "../Io.h"
#endif

#ifndef VENUS_MAIL_H
#include "../Mail.h"
#endif

#ifndef STDEXT_HASHMAP_H
#include "../../../../stdext/include/stdext/HashMap.h"
#endif

#include <map>

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class ConnectionImpl

class ConnectionImpl
{
private:
	ConnectionImpl(const ConnectionImpl&);
	ConnectionImpl& operator=(const ConnectionImpl&);

protected:
	typedef MailReader<SocketFile> Reader;
	typedef BufferedWriter<SocketFile> Writer;
	
	enum { InvalidMailNo = -1 };
	enum { MailNoMask = 0x7FFFFFFF };

	typedef bool (cerl_callback *GetProc)(Reader& ar, void* param);

	struct RecvInfo
	{
		ScopedAlloc* alloc;
		GetProc proc;
		void* param;
		bool result;
		Fiber fiber;
	};

	typedef NS_STDEXT::map<cerl::MailNo, RecvInfo> RecvMap;
	// callback information
	RecvMap receivers;

	NInformation const host;
	SocketFileObject file;
	SocketFile fileR;
	SocketFile fileW;
	Pool& msgBufPool;

	// wait for receiver fiber
	FiberMutex mutexR;
	// make writers queuing
	FiberMutex mutexW;

	Writer writer;
	Reader reader;
	UINT32 currMailNo;
	bool autoReconnect;

public:
	ConnectionImpl(
		SOCKET socket, const NInformation& hostArg, Pool& msgBufPoolArg)
		: host(hostArg), fileR(file), fileW(file), msgBufPool(msgBufPoolArg), 
		  writer(fileW, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  reader(fileR, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  currMailNo(0), autoReconnect(true)
	{
		Fiber self = getCurrentFiber();
		open_handle(self, socket);
	}

	~ConnectionImpl()
	{
		msgBufPool.deallocate((void*)writer.get_buffer(), msgBufPool.alloc_size());
		msgBufPool.deallocate((void*)reader.get_buffer(), msgBufPool.alloc_size());

		if (file.good() && owner())
			file.close();

		// wait for receiving fiber to quit
		CERL_VLOG("Connection", ("wait for receiving fiber\n"));
		Fiber self = getCurrentFiber();
		ScopedLock<FiberMutex> guard(self, mutexR);
	}

	void set_auto_reconnect(bool b)
	{
		autoReconnect = b;
	}

	const NInformation& get_host() const
	{
		return host;
	}

	bool cerl_call good() const
	{
		return file.good();
	}

public:
	void cerl_call set_timeout(Timeout to)
	{
		file.set_read_timeout(to);
		file.set_write_timeout(to);
	}

	Timeout cerl_call get_timeout() const
	{
		return file.get_write_timeout();
	}

private:

	bool owner() const
	{
		return !(NInformation() == host);
	}

	HRESULT cerl_call reopen(Fiber self, const NInformation& host)
	{
		CERL_ASSERT(!file.good());

		if (!owner())
			return E_ACCESSDENIED;

		// wait for receiving fiber to quit
		CERL_VLOG("Connection", ("wait for receiving fiber\n"));
		ScopedLock<FiberMutex> guard(self, mutexR);

		reader.clear();
		writer.clear();
		SOCKET socket = connectSocket(self, host);
		// todo: error process on connect failed.
		return open_handle(self, socket);
	}

	HRESULT cerl_call open_handle(Fiber self, SOCKET socket)
	{
		HRESULT result = file.open_handle(socket);

		if (file.good())
		{
			file.set_read_timeout(CERL_INFINITE);
			file.set_write_timeout(CERL_INFINITE);
			CerlIoService(self)->startFiber(self, ReceiverMain, this, VENUS_CONNECTION_STACKSIZE);
		}

		return result;
	}

	template <class ArgsT>
	bool cerl_call send(Fiber self, MailNo mailno, FID fid, const ArgsT& args)
	{
		MailHeader header = { sizeof(MailHeader), 0, VENUS_MAIL_MAGIC_CODE, mailno, fid };

		fileW.setFiber(self);

		NS_STDEXT_IO_BINARY::put_struct(writer, header);
		
		return NS_CERL::putMailBody(writer, args) && writer.flush();
	}

	template <class ResultT>
	struct Traits
	{
		static bool cerl_callback getMailBody(Reader& ar, void* param)
		{
			return cerl::getMailBody(ar, *(ResultT*)param);
		}
	};

public:
	template <class ArgsT>
	bool cerl_call cast(FID fid, const ArgsT& args)
	{
		CERL_ASSERT(this);
		Fiber self = cerl::getCurrentFiber();

		MailNo const mailno = (++currMailNo & MailNoMask);
		Timeout timeout = get_timeout();
		if (!mutexW.timed_acquire(self, timeout))
		{
			CERL_WARN("Connection", ("WARN: wait to write timeout\n"));
			return false;
		}

		if (!good() && autoReconnect && reopen(self, host) != S_OK)
		{
			mutexW.release();
			CERL_WARN("Connection", ("WARN: connection lost!\n"));
			return false;
		}

		const bool b = send(self, mailno, fid, args);
		mutexW.release();
		return b;
	}

	template <class ResultT, class ArgsT>
	bool cerl_call call(ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(this);
		Fiber self = cerl::getCurrentFiber();

		MailNo const mailno = (++currMailNo & MailNoMask);
		CERL_VLOG("Connection", ("INFO: SEND MailNo: %d\n", (int)mailno));

		Timeout timeout = get_timeout();
		if (!mutexW.timed_acquire(self, timeout))
		{
			CERL_WARN("Connection", ("WARN: wait to write timeout\n"));
			return false;
		}

		if (!good() && autoReconnect && reopen(self, host) != S_OK)
		{
			mutexW.release();
			CERL_WARN("Connection", ("WARN: connection lost!\n"));
			return false;
		}

		RecvInfo info;
		info.alloc = &alloc;
		info.proc = Traits<ResultT>::getMailBody;
		info.param = &result;
		info.result = false;
		info.fiber = self;
		receivers.insert(std::make_pair(mailno, info));

		bool const fOk = send(self, mailno, fid, args);

		mutexW.release();

		if (!fOk)
		{
			CERL_WARN("Connection", ("send failed\n"));
			return false;
		}

		bool succ = timed_yield(self, get_timeout());

		receivers.erase(mailno);

		if (!succ)
			return false;

		return info.result;
	}

private:
	void cerl_callback receiver_(Fiber self)
	{
		// protect receive fiber
		ScopedLock<FiberMutex> guard(self, mutexR);

		SocketFile pfile(file);

		Reader reader(pfile, (char*)msgBufPool.allocate(), msgBufPool.alloc_size());
	
		MailHeader header;

		CERL_VLOG("Connection", ("INFO: Connection::receiver - receiver begin loop.\n"));
		for(;;)
		{
			CERL_VLOG("Connection", ("INFO: Connection::receiver - get header.\n"));
			if (!NS_STDEXT_IO_BINARY::get_struct(reader, header)
				|| VENUS_MAIL_MAGIC_CODE != header.magic 
				|| sizeof(MailHeader) != header.cbHeader)
			{
				CERL_WARN("Connection", ("WARN: Connection::receiver - read mail failed: mailno: %d.\n", header.mailno));
				file.close();
				break;
			}

			RecvMap::iterator iterReceiver = receivers.find(header.mailno & MailNoMask);
			if (receivers.end() == iterReceiver)
			{
				CERL_WARN("Connection", ("DROP: mail\n"));
				skipMailBody(reader);
				continue;
			}


			RecvInfo& receiver = iterReceiver->second;
			reader.setAlloc(receiver.alloc);

			CERL_VLOG("Connection", ("INFO: Connection::receiver - do unserialization. \n"));
			if (!(receiver.result = receiver.proc(reader, receiver.param)))
			{
				CERL_WARN("Connection", ("WARN: Connection::receiver - read mail failed!\n"));
				file.close();
				break;
			}

			receivers.erase(iterReceiver);

			CERL_VLOG("Connection", ("INFO: Connection::receiver - schedule to curr fiber. \n"));
			CerlIoService(self)->scheduleFiber(self);
			switchToFiber(self, receiver.fiber);
		}

		CERL_VLOG("Connection", ("INFO: Connection::receiver cleanup ...\n"));

		RecvMap::iterator iter = receivers.begin();
		for (; iter != receivers.end(); ++ iter)
		{
			RecvInfo& receiver = iter->second;
			receiver.result = false;
			*(Code*)receiver.param = code_socket_error;
			CerlIoService(self)->scheduleFiber(self);
			switchToFiber(self, receiver.fiber);
		}

		CERL_VLOG("Connection", ("receiving fiber exit\n"));
	}
	
	static void cerl_callback ReceiverMain(LPVOID lpParam)
	{
		FiberParam p(lpParam);
		((ConnectionImpl*)p.val)->receiver_(p.self);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif
