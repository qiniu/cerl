/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/conn/venusonly.h
// Creator: Mo Huafeng
// Email: longshanksmo@gmail.com
// Date: 2010-1-04 11:25:00
// 
// $Id: venusonly.h 2754 2010-05-05 10:25:27Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_CONN_VENUSONLY_H
#define VENUS_CONN_VENUSONLY_H

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

	NInformation const host;
	SocketFileObject fileObj;
	SocketFile fileR;
	SocketFile fileW;
	Pool& msgBufPool;
	FiberMutex mutexR;
	FiberMutex mutexW;
	Writer writer;
	Reader reader;
	UINT32 currMailNo;
	bool autoReconnect;
	int	connTimeout;

public:
	ConnectionImpl(
		SOCKET socket, const NInformation& hostArg, Pool& msgBufPoolArg)
		: host(hostArg), fileR(fileObj), fileW(fileObj), msgBufPool(msgBufPoolArg), 
		  writer(fileW, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  reader(fileR, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  currMailNo(0), autoReconnect(true), connTimeout(500)
	{
		fileObj.open_handle(socket);
	}

	~ConnectionImpl()
	{
		msgBufPool.deallocate((void*)writer.get_buffer(), msgBufPool.alloc_size());
		msgBufPool.deallocate((void*)reader.get_buffer(), msgBufPool.alloc_size());

		if (fileObj.good() && owner())
			fileObj.close();
	}

	void cerl_call set_auto_reconnect(bool b)
	{
		autoReconnect = b;
	}

	const NInformation& cerl_call get_host() const
	{
		return host;
	}

	bool cerl_call good() const
	{
		return fileObj.good();
	}

public:
	void cerl_call set_timeout(Timeout to)
	{
		fileObj.set_read_timeout(to);
		fileObj.set_write_timeout(to);
	}

	Timeout cerl_call get_timeout() const
	{
		return fileObj.get_write_timeout();
	}

	void cerl_call set_conn_timeout(Timeout to)
	{
		connTimeout = to;
	}

	Timeout cerl_call get_conn_timeout() const
	{
		return connTimeout;
	}

private:

	bool owner() const
	{
		return !(NInformation() == host);
	}

	HRESULT cerl_call reopen(Fiber self, const NInformation& host)
	{
		CERL_ASSERT(!fileObj.good());

		if (!owner())
			return E_ACCESSDENIED;

		reader.clear();
		writer.clear();
		SOCKET socket = connectSocket(self, host, connTimeout);
		// todo: error process on connect failed.
		return fileObj.open_handle(socket);
	}

	template <class ArgsT>
	bool cerl_call send(Fiber self, MailNo mailno, FID fid, const ArgsT& args)
	{
		MailHeader header = { sizeof(MailHeader), 0, VENUS_MAIL_MAGIC_CODE, mailno, fid };

		fileW.setFiber(self);

		NS_STDEXT_IO_BINARY::put_struct(writer, header);
		
		bool isOk = NS_CERL::putMailBody(writer, args) && writer.flush();
		if (!isOk)
		{
			writer.clear();
		}

		return isOk;
	}

	template <class ResultT>
	bool cerl_call recv(Fiber self, ScopedAlloc& alloc, MailNo mailno, ResultT& result)
	{
		MailHeader header;

		fileR.setFiber(self);
		reader.setAlloc(&alloc);

		if (!NS_STDEXT_IO_BINARY::get_struct(reader, header))
		{
			CERL_WARN("Connection", ("WARNING: ConnectionImpl::recv - get header failed.\n"));
			goto lzError;
		}

		if (
			header.magic != VENUS_MAIL_MAGIC_CODE ||
			header.cbHeader != sizeof(MailHeader)
			)
		{
			CERL_WARN("Connection", ("WARN: Connection::recv - recv mail failed! magic: %x, head size: %d\n", header.magic, header.cbHeader));
			goto lzError;
		}

		CERL_VLOG("Connection", ("INFO: RECV MailNo: %d\n", (int)header.mailno & MailNoMask));
		if ((header.mailno & MailNoMask) != mailno)
		{
			CERL_WARN("Connection", ("WARN: Connection::recv - mail no. error! mailno: %d\n", header.mailno));
			goto lzError;
		}
		
		if (!NS_CERL::getMailBody(reader, result))
		{
			CERL_WARN("Connection", ("WARN: Connection::recv - unserialization error!\n"));
			goto lzError;
		}
		return true;

lzError:
		result = code_socket_error;
		fileObj.close();
		reader.clear();
		return false;
	}

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
			//CERL_WARN("Connection", ("WARN: connection lost!\n"));
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
			//CERL_WARN("Connection", ("WARN: connection lost!\n"));
			return false;
		}

		bool const fOk = send(self, mailno, fid, args);

		cerl::FiberSerialNo sn = CerlFiberSerialNo(self);
		(void)sn; // fix warning

		mutexW.release();

		if (!fOk)
		{
			result = code_socket_error;

			CERL_WARN("Connection", ("WARN: Connection::call - send request failed!\n"));
			return false;
		}

		CERL_ASSERT(CerlFiberSerialNo(self) == sn && \
				"this implementation can't work because FiberMutex::release() switched fiber");

		mutexR.acquire(self);
		bool const fOk2 = recv(self, alloc, mailno, result);
		mutexR.release();
		if (!fOk2)
		{
			result = code_socket_error;
			CERL_WARN("Connection", ("WARN: Connection::call - recv response failed!\n"));
		}
		return fOk2;
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif
