/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Server.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Server.h 2743 2010-05-05 08:21:38Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_SERVER_H
#define ASYNC_SERVER_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef ASYNC_IOSERVICEPOOL_H
#include "IoServicePool.h"
#endif

#ifndef ASYNC_TIMER_H
#include "Timer.h"
#endif

#ifndef ASYNC_IO_H
#include "Io.h"
#endif

#ifndef ASYNC_BUFFERED_H
#include "Buffered.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// CERL_ISVALID_HANDLER / CERL_NULL_HANDLER

namespace detail {

typedef LONG (cerl_call* NullHandler)(LPCVOID self, ...);

class No	{ int unused; };
class Yes	{ No no[2]; };

No valid(int, NullHandler);
Yes valid(int, ...);

} // namespace detail

#define CERL_ISVALID_HANDLER(Impl, handler)									\
	( sizeof(cerl::detail::valid(0, &Impl::handler)) == sizeof(cerl::detail::Yes) )

#define CERL_NULL_HANDLER(handler)											\
public:																		\
	static LONG cerl_call handler(LPCVOID self, ...) {						\
		CERL_ASSERT(false && "Don't call me!!!\n");							\
		return 0;															\
	}

// -------------------------------------------------------------------------
// class ServerOptions

struct ServerOptions
{
	enum { IoBufferSize = 4000 };
	enum { MultiThreads = FALSE };
	enum { SessionStackSize = 0 };
	enum { StartupStackSize = 0 };
    unsigned int DefaultReadTimeout;
    unsigned int DefaultWriteTimeout;
    ServerOptions():DefaultReadTimeout(CERL_INFINITE), DefaultWriteTimeout(CERL_DEFAULT_TIMEOUT){}
	//enum { DefaultReadTimeout = CERL_INFINITE };
	//enum { DefaultWriteTimeout = CERL_DEFAULT_TIMEOUT };
};

// -----------------------------------------------------------------------
// class ServerSession

class ServerSession
{
public:
	template <class ServerT>
	static void cerl_call main(Fiber self, ServerT* const server, SOCKET sdAccept)
	{
		typedef typename ServerT::Caller Caller;

		SocketFileObject fileObj;
		fileObj.open_handle(self, sdAccept);

		fileObj.set_read_timeout(server->DefaultReadTimeout);
        fileObj.set_write_timeout(server->DefaultWriteTimeout);
        
		SocketFile file(self, fileObj);

		IoService* const service = CerlIoService(self);
		Pool& pool = service->pools.get_pool(server->IoBufferSize);
		ScopedAlloc alloc(CerlBlockPool(self));

		Caller caller(file, pool, alloc);

		try
		{
			server->onAccept(&caller);

			CloseObjectOnQuit<SocketFileObject> cooq(service, fileObj);
			while (caller.processRequest(server))
			{
				alloc.clear();
			}

			server->onDisaccept(&caller);
		}
		catch(...)
		{
			CERL_ERROR("Server", ("ERROR: ServerSession::main - encounter unexcepted exception!\n"));
		}
	}
};

// -----------------------------------------------------------------------
// class ServerRunner

template <class ServerT, class SessionT = ServerSession>
class ServerRunner
{
public:
	typedef ServerT server_type;

private:
	struct Session
	{
		server_type* m_server;
		SOCKET m_sdAccept;

		void cerl_call run(Fiber self) const
		{
			SessionT::main(self, m_server, m_sdAccept);
		}
	};

	static void cerl_callback StartupMain(LPVOID lpParam)
	{
		FiberParam p(lpParam);
		((server_type*)p.val)->onStartup(p.self);
	}

public:
	static HRESULT cerl_call run(Fiber self, server_type* server, SOCKET socketListen)
	{
		ListenSocket s;
		if (s.open_handle(self, socketListen) != S_OK)
			return E_FAIL;

		IoService* const service = CerlIoService(self);
		if (CERL_ISVALID_HANDLER(server_type, onStartup))
		{
			service->spawnFiber(StartupMain, server, server->StartupStackSize);
		}

		CloseObjectOnQuit<ListenSocket> cooq(service, s);
		while (s.good())
		{
			const SOCKET sdAccept = s.accept(self);
			if (sdAccept == INVALID_SOCKET)
				continue;

			CERL_VLOG("Async Server", ("ACCEPT CONNECTION: %d\n", sdAccept));

			const Session session = { server, sdAccept };
			if (server->MultiThreads)
				IoServicePool::startSession(self, session, server->SessionStackSize);
			else
				service->startSession(self, session, server->SessionStackSize);
		}

		CERL_VLOG("Server", ("INFO: ServerRunner::run terminated.\n"));
		return S_OK;
	}
};

// -----------------------------------------------------------------------
// class ServerImpl

template <class ServerT, class CallerT>
class ServerImpl : public ServerOptions
{
	CERL_NULL_HANDLER(onStartup)
	// void cerl_call onStartup(cerl::Fiber self);

public:
	typedef CallerT Caller;

	void cerl_call onAccept(Caller* caller) // on accept a client connection
	{
	}

	void cerl_call onDisaccept(Caller* caller) // on disconnect a client connection
	{
	}

public:
	HRESULT cerl_call run(Fiber self, SOCKET socketListen)
	{
		typedef ServerRunner<ServerT> Runner;
		return Runner::run(self, static_cast<ServerT*>(this), socketListen);
	}

	HRESULT cerl_call _run(Fiber self, SOCKET socketListen)
	{
		typedef ServerRunner<ServerT> Runner;
		return Runner::run(self, static_cast<ServerT*>(this), socketListen);
	}
};

// -------------------------------------------------------------------------
// class CallerBase

#define CERL_CALLER_PARAMS	cerl::SocketFile& socketFile, cerl::Pool& bufferPool, cerl::ScopedAlloc& tempAlloc
#define CERL_CALLER_ARGS	socketFile, bufferPool, tempAlloc

class CallerBase
{
public:
	typedef BufferedReader<SocketFile> Reader;
	typedef BufferedWriter<SocketFile> Writer;

protected:
	char* m_readBuffer;
	char* m_writeBuffer;
	Reader m_reader;
	Writer m_writer;
	Pool& m_pool;
	ScopedAlloc& m_alloc;

public:
	template <class ServerT>
	bool cerl_call processRequest(ServerT* server)
	{
		return false;
	}

public:
	CallerBase(SocketFile& file, Pool& pool, ScopedAlloc& alloc)
		: m_readBuffer((char*)pool.allocate()),
		  m_writeBuffer((char*)pool.allocate()),
		  m_reader(file, m_readBuffer, pool.alloc_size()),
		  m_writer(file, m_writeBuffer, pool.alloc_size()),
		  m_pool(pool), m_alloc(alloc)
	{
	}

	~CallerBase()
	{
		m_pool.deallocate(m_readBuffer);
		m_pool.deallocate(m_writeBuffer);
	}

	Fiber cerl_call getSelf() const
	{
		return m_writer.get_file().getFiber();
	}

	ScopedAlloc& cerl_call getAlloc()
	{
		return m_alloc;
	}

	SocketFile& cerl_call getSocket()
	{
		return m_writer.get_file();
	}

	Writer& cerl_call getWriter()
	{
		return m_writer;
	}

	Reader& cerl_call getReader()
	{
		return m_reader;
	}
};

// -----------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_SERVER_H */
