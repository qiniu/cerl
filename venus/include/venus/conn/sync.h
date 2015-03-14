/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/connection/sync.h
// Creator: Li Jie
// Email: cpunion@gmail.com
// Date: 2010-1-04 11:25:00
// 
// $Id: sync.h 2795 2010-05-07 05:24:39Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_CONN_SYNC_H
#define VENUS_CONN_SYNC_H

#ifndef VENUS_BASIC_H
#include "../Basic.h"
#endif

#ifndef VENUS_IO_H
#include "../Io.h"
#endif

#ifndef VENUS_MAIL_H
#include "../Mail.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class SyncConnection

class SyncConnection
{
private:
	SyncConnection(const SyncConnection&);
	SyncConnection& operator=(const SyncConnection&);

protected:
	typedef MailReader<SyncSocketFile> Reader;
	typedef BufferedWriter<SyncSocketFile> Writer;
	
	enum { InvalidMailNo = -1 };
	enum { MailNoMask = 0x7FFFFFFF };

	NInformation const host;
	SyncSocketFile file;
	Pool& msgBufPool;
	Writer writer;
	Reader reader;
	UINT32 currMailNo;
	bool autoReconnect;

public:
	SyncConnection(const NInformation& hostArg, Pool& msgBufPoolArg)
		: host(hostArg), msgBufPool(msgBufPoolArg),
		  writer(file, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  reader(file, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  currMailNo(0), autoReconnect(true)
	{
		file.open_handle(connectSocket(hostArg));
	}

	SyncConnection(SOCKET socket, Pool& msgBufPoolArg)
		: msgBufPool(msgBufPoolArg),
		  writer(file, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  reader(file, (char*)msgBufPool.allocate(), msgBufPool.alloc_size()),
		  currMailNo(0)
	{
		file.open_handle(socket);
	}

	~SyncConnection()
	{
		msgBufPool.deallocate((void*)reader.get_buffer(), msgBufPool.alloc_size());
		msgBufPool.deallocate((void*)writer.get_buffer(), msgBufPool.alloc_size());

		if (file.good() && owner())
			file.close();
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
		return file.good();
	}

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

	HRESULT cerl_call reopen()
	{
		CERL_ASSERT(!file.good());

		if (!owner())
			return E_ACCESSDENIED;

		reader.clear();
		writer.clear();
		SOCKET socket = connectSocket(host);
		// todo: error process on connect failed.
		return file.open_handle(socket);
	}

	template <class ArgsT>
	bool cerl_call send(MailNo mailno, FID fid, const ArgsT& args)
	{
		MailHeader header = { sizeof(MailHeader), 0, VENUS_MAIL_MAGIC_CODE, mailno, fid };

		NS_STDEXT_IO_BINARY::put_struct(writer, header);
		
		return NS_CERL::putMailBody(writer, args) && writer.flush();
	}

	template <class ResultT>
	bool cerl_call recv(MailNo mailno, ResultT& result, ScopedAlloc& alloc)
	{
		MailHeader header;

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
		file.close();
		return false;
	}

public:
	template <class ArgsT>
	bool cerl_call cast(FID fid, const ArgsT& args)
	{
		CERL_ASSERT(this);

		MailNo const mailno = (++currMailNo & MailNoMask);

		if (!good())
		{
			CERL_WARN("Connection", ("WARN: connection lost!\n"));
			return false;
		}

		return send(mailno, fid, args);
	}

	bool cerl_call cast(FID fid)
	{
		return cast(fid, cerl_noargs);
	}

	template <class ResultT, class ArgsT>
	bool cerl_call call(ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(this);

		MailNo const mailno = (++currMailNo & MailNoMask);
		CERL_VLOG("Connection", ("INFO: SEND MailNo: %d\n", (int)mailno));

		if (!good())
		{
			CERL_WARN("Connection", ("WARN: connection lost!\n"));
			return false;
		}

		bool const fOk = send(mailno, fid, args);

		bool const fOk2 = fOk && recv(mailno, result, alloc);
		if (!fOk2)
		{
			result = code_socket_error;
			if (!fOk)
				CERL_WARN("Connection", ("WARN: Connection::call - send request failed!\n"));
			else
				CERL_WARN("Connection", ("WARN: Connection::call - recv response failed!\n"));
		}
		return fOk2;
	}

	template <class ResultT>
	bool cerl_call call(ScopedAlloc& alloc, ResultT& result, FID fid)
	{
		return call(alloc, result, fid, cerl_noargs);
	}

#if defined(CERL_DEBUG)
	template <class ServerT, class ArgsT>
	bool cerl_call dbg_cast(ServerT* server, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, args);
		return cast(fid, args);
	}

	template <class ServerT>
	bool cerl_call dbg_cast0(ServerT* server, FID fid)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, cerl_noargs);
		return cast(fid, cerl_noargs);
	}

	template <class ServerT, class ResultT, class ArgsT>
	bool cerl_call dbg_call(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, args);
		bool b = call(alloc, result, fid, args);
		CERL_DUMP_RECV_REPLY(getCurrentFiber(), server, fid, result);
		return b;
	}

	template <class ServerT, class ResultT>
	bool cerl_call dbg_call0(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, cerl_noargs);
		bool b = call(alloc, result, fid, cerl_noargs);
		CERL_DUMP_RECV_REPLY(getCurrentFiber(), server, fid, result);
		return b;
	}

#else

	template <class ServerT, class ArgsT>
	bool cerl_call dbg_cast(ServerT* server, FID fid, const ArgsT& args)
	{
		return cast(fid, args);
	}

	template <class ServerT>
	bool cerl_call dbg_cast0(ServerT* server, FID fid)
	{
		return cast(fid, cerl_noargs);
	}

	template <class ServerT, class ResultT, class ArgsT>
	bool cerl_call dbg_call(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		return call(alloc, result, fid, args);
	}

	template <class ServerT, class ResultT>
	bool cerl_call dbg_call0(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid)
	{
		return call(alloc, result, fid, cerl_noargs);
	}

#endif
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif
