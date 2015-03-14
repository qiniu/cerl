/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Server.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Server.h 2732 2010-04-29 09:01:30Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_SERVER_H
#define VENUS_SERVER_H

#ifndef VENUS_MAIL_H
#include "Mail.h"
#endif

#ifndef VENUS_DEBUG_H
#include "Debug.h"
#endif

#ifndef VENUS_IO_H
#include "Io.h"
#endif

#ifndef ASYNC_SERVER_H
#include "../../../async/include/async/Server.h"
#endif

NS_CERL_BEGIN

// -----------------------------------------------------------------------
// class Caller

class Caller
{
public:
	typedef MailReader<SocketFile> Reader;
	typedef BufferedWriter<SocketFile> Writer;

private:
	char* m_readBuffer;
	char* m_writeBuffer;
	Reader m_reader;
	Writer m_writer;
	Pool& m_pool;
	MailHeader m_mailHeader;

public:
	template <class ServerT>
	bool cerl_call processRequest(ServerT* server)
	{
		if (!NS_STDEXT_IO_BINARY::get_struct(m_reader, m_mailHeader))
			return false;

		if (
			VENUS_MAIL_MAGIC_CODE != m_mailHeader.magic ||
			sizeof(MailHeader) != m_mailHeader.cbHeader
			)
		{
			CERL_WARN("Server", ("WARN: Caller::readMailHeader - Invalid MailHeader! magic: %x, header sie: %d\n", m_mailHeader.magic, m_mailHeader.cbHeader));
			return false;
		}

		server->_process(this, m_reader);

		return true;
	}

public:
	Caller(SocketFile& file, Pool& pool, ScopedAlloc& alloc)
		: m_readBuffer((char*)pool.allocate()),
		  m_writeBuffer((char*)pool.allocate()),
		  m_reader(file, m_readBuffer, pool.alloc_size()),
		  m_writer(file, m_writeBuffer, pool.alloc_size()),
		  m_pool(pool)
	{
		m_reader.setAlloc(&alloc);
	}
	
	~Caller()
	{
		m_pool.deallocate(m_readBuffer);
		m_pool.deallocate(m_writeBuffer);
	}

	FID cerl_call getFid() const
	{
		return (cerl::FID)m_mailHeader.fid;
	}

	Fiber cerl_call getSelf() const
	{
		return m_reader.get_file().getFiber();
	}

	ScopedAlloc& cerl_call getAlloc()
	{
		return m_reader.getAlloc();
	}

	SocketFile& cerl_call getSocket()
	{
		return m_reader.get_file();
	}

	template <class ResultT>
	bool cerl_call reply(const ResultT& result)
	{
		if (!(m_mailHeader.fid & CERL_FID_ASYNC_MASK))
		{
			m_mailHeader.fid |= (CERL_FID_RET_MASK | CERL_FID_ASYNC_MASK);
			NS_STDEXT_IO_BINARY::put_struct(m_writer, m_mailHeader);
			return NS_CERL::putMailBody(m_writer, result) && m_writer.flush();
		}
		return true;
	}

	bool cerl_call reply_error(Code code)
	{
		return reply(code);
	}

#if defined(CERL_DEBUG)
	template <class ServerT, class ResultT>
	bool cerl_call dbg_reply(ServerT* server, const ResultT& result)
	{
		if (!(m_mailHeader.fid & CERL_FID_ASYNC_MASK))
		{
			CERL_DUMP_SEND_REPLY(getSelf(), server, m_mailHeader.fid, result);
			return reply(result);
		}
		return true;
	}
	
	template <class ServerT>
	bool cerl_call dbg_reply_error(ServerT* server, Code code)
	{
		return dbg_reply(server, code);
	}
#else
#define dbg_reply(server, result)			reply(result)
#define dbg_reply_error(server, result)		reply_error(result)
#endif
};

// -------------------------------------------------------------------------
// class Server

class Server : public ServerOptions
{
	CERL_NULL_HANDLER(onStartup)
	// void cerl_call onStartup(cerl::Fiber self);

public:
	typedef cerl::Caller Caller;

	cerl::String cerl_call _dump() const // dump State
	{
		return cerl::String();
	}

	void cerl_call onAccept(Caller* caller) // on accept a client connection
	{
	}

	void cerl_call onDisaccept(Caller* caller) // on disconnect a client connection
	{
	}
};

// -------------------------------------------------------------------------
// handle_call

template <class CallerT, class ServerT, class ArchiveT>
inline void cerl_call handle_call(ServerT* server, CallerT* caller, FID fid, ArchiveT& ar)
{
	CERL_ASSERT(is_sync_call(fid));

#if defined(CERL_DEBUG)
	if (CERL_FID_DEBUG == fid)
	{	
		_DebugResult result;

		Code code = debug_invalid_op;
		const bool fOk = getMailBody(ar, code);
		CERL_ASSERT(fOk && "cerl::handle_call - fid:debug");
		if (fOk)
		{
			CERL_DUMP_RECV_REQUEST(caller->getSelf(), server, fid, code);
			if (code == debug_dump_state)
			{
				result = code_ok;
				result.value = server->_dump();
			}
			else
				result = code_unknown_fid;
		}
		else
		{
		   result = code_format_error;
		}
		caller->dbg_reply(server, result);
	}
	else
	{
#else
	{
#endif
		skipMailBody(ar);
		CERL_TRACE("cerl::handle_call - Unknown FID: %.8X\n", fid);
		caller->dbg_reply_error(server, code_unknown_fid);
	}
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /*VENUS_SERVER_H*/
