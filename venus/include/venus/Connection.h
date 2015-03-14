/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Connection.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Connection.h 2795 2010-05-07 05:24:39Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_CONNECTION_H
#define VENUS_CONNECTION_H

#if (0)
#define VENUS_CONNECTION_IMPL_DATAQUE
#else
#define VENUS_CONNECTION_IMPL_VENUSONLY
#endif

#ifndef VENUS_CONNECTION_BUFSIZE
#define VENUS_CONNECTION_BUFSIZE	16000
#endif

#ifndef VENUS_CONNECTION_STACKSIZE
#define VENUS_CONNECTION_STACKSIZE	0
#endif

#if defined(VENUS_CONNECTION_IMPL_ACTIVE)
#include "conn/active.h"
#elif defined(VENUS_CONNECTION_IMPL_DATAQUE)
#include "conn/dataque.h"
#elif defined(VENUS_CONNECTION_IMPL_VENUSONLY)
#include "conn/venusonly.h"
#endif

#include "conn/sync.h"

#ifndef ASYNC_EVENT_H
#include "../../../async/include/async/Event.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class Connections

class SharedConnectionImpl;

typedef NS_STDEXT::HashMap<NInformation, SharedConnectionImpl*> Connections;

// -------------------------------------------------------------------------
// class SharedConnectionImpl

class SharedConnectionImpl : public ConnectionImpl
{
public:
	Pool& connPool;
	Connections* const connections;
	UINT32 nRef;

public:
	SharedConnectionImpl(
		SOCKET socket, Pool& msgBufPoolArg, Pool& connPoolArg,
		const NInformation& hostArg, Connections* connsArg)
		: ConnectionImpl(socket, hostArg, msgBufPoolArg),
		  connPool(connPoolArg), connections(connsArg), nRef(1)
	{
	}

	~SharedConnectionImpl()
	{
		if (connections)
		{
			const NInformation& host = get_host();
#if defined(_DEBUG)
			Connections::iterator it = connections->find(host);
			CERL_ASSERT(it != connections->end());
			CERL_ASSERT((*it).second == this);
#endif
			connections->erase(host);
		}
	}

	void cerl_call acquire()
	{
		++nRef;
	}

	void cerl_call release()
	{
		if (--nRef == 0)
		{
			this->~SharedConnectionImpl();
			connPool.deallocate(this);
		}
	}

	void cerl_call set_auto_reconnect(bool b)
	{
		if (!connections)
		{
			ConnectionImpl::set_auto_reconnect(b);
		}
	}
};


// -------------------------------------------------------------------------
// class Connection

class Connection
{
private:
	SharedConnectionImpl* impl;
	
public:
	Connection()
		: impl(NULL)
	{
	}
	
	Connection(SOCKET socket)
	{
		Fiber self = cerl::getCurrentFiber();

		init(self, socket);
	}

	Connection(const NInformation& host)
	{
		Fiber self = cerl::getCurrentFiber();

		init(self, connectSocket(self, host), host);
	}

	Connection(const char* hostname, Port port)
	{
		Fiber self = cerl::getCurrentFiber();

		cerl::NInformation host(hostname, port);

		init(self, connectSocket(self, host), host);
	}

	Connection(Connections& conns, const NInformation& host)
	{
		Fiber self = cerl::getCurrentFiber();

		if (conns[host])
		{
			impl = conns[host];
			impl->acquire();
		}
		else
		{
			init(self, INVALID_SOCKET, host, &conns);
			conns[host] = impl;
		}
	}

	Connection(const Connection& conn)
		: impl(conn.impl)
	{
		if (conn.impl)
			conn.impl->acquire();
	}

	~Connection()
	{
		if (impl)
		{
			impl->release();
			impl = NULL;
		}
	}

private:
	void init(Fiber self, SOCKET socket, const NInformation& host = NInformation(), Connections* conns = NULL)
	{
		Pools& pools = CerlIoService(self)->pools;
		Pool& connPool = pools.get_pool(sizeof(SharedConnectionImpl));
		Pool& msgBufPool = pools.get_pool(VENUS_CONNECTION_BUFSIZE);
		impl = new(connPool.allocate()) SharedConnectionImpl(socket, msgBufPool, connPool, host, conns);
	}

public:
	Connection& cerl_call operator=(const Connection& conn)
	{
		if (conn.impl)
			conn.impl->acquire();
		if (impl)
			impl->release();
		impl = conn.impl;
		return *this;
	}

	void cerl_call set_auto_reconnect(bool b)
	{ 
		CERL_ASSERT(impl);
		
		impl->set_auto_reconnect(b);
	}

	const NInformation& cerl_call get_host() const
	{
		CERL_ASSERT(impl);

		return impl->get_host();
	}

	Connections* cerl_call get_connections() const
	{
		CERL_ASSERT(impl);

		return impl->connections;
	}

	void cerl_call release()
	{
		if (impl != NULL)
		{
			impl->release();
			impl = NULL;
		}
	}

public:
	void cerl_call set_timeout(Timeout to)
	{
		impl->set_timeout(to);
	}

	Timeout cerl_call get_timeout() const
	{
		return impl->get_timeout();
	}

	void cerl_call set_conn_timeout(Timeout to)
	{
		impl->set_conn_timeout(to);
	}

	Timeout cerl_call get_conn_timeout() const
	{
		return impl->get_conn_timeout();
	}

public:
	bool cerl_call good() const
	{
		return impl != NULL && impl->good();
	}

	template <class ArgsT>
	bool cerl_call cast(FID fid, const ArgsT& args)
	{
		return impl->cast(fid, args);
	}
	
	bool cerl_call cast(FID fid)
	{
		return impl->cast(fid, cerl_noargs);
	}

	template <class ResultT, class ArgsT>
	bool cerl_call call(ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		return impl->call(alloc, result, fid, args);
	}

	template <class ResultT>
	bool cerl_call call(ScopedAlloc& alloc, ResultT& result, FID fid)
	{
		return impl->call(alloc, result, fid, cerl_noargs);
	}

#if defined(CERL_DEBUG)
	template <class ServerT>
	bool cerl_call dbg_cast0(ServerT* server, FID fid)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, cerl_noargs);
		return impl->cast(fid, cerl_noargs);
	}

	template <class ServerT, class ArgsT>
	bool cerl_call dbg_cast(ServerT* server, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, args);
		return impl->cast(fid, args);
	}

	template <class ServerT, class ResultT>
	bool cerl_call dbg_call0(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, cerl_noargs);
		bool b = impl->call(alloc, result, fid, cerl_noargs);
		CERL_DUMP_RECV_REPLY(getCurrentFiber(), server, fid, result);
		return b;
	}

	template <class ServerT, class ResultT, class ArgsT>
	bool cerl_call dbg_call(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(good());
		CERL_DUMP_SEND_REQUEST(getCurrentFiber(), server, fid, args);
		bool b = impl->call(alloc, result, fid, args);
		CERL_DUMP_RECV_REPLY(getCurrentFiber(), server, fid, result);
		return b;
	}

#else
	template <class ServerT>
	bool cerl_call dbg_cast0(ServerT* server, FID fid)
	{
		return impl->cast(fid, cerl_noargs);
	}

	template <class ServerT, class ArgsT>
	bool cerl_call dbg_cast(ServerT* server, FID fid, const ArgsT& args)
	{
		return impl->cast(fid, args);
	}

	template <class ServerT, class ResultT>
	bool cerl_call dbg_call0(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid)
	{
		return impl->call(alloc, result, fid, cerl_noargs);
	}

	template <class ServerT, class ResultT, class ArgsT>
	bool cerl_call dbg_call(ServerT* server, ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		return impl->call(alloc, result, fid, args);
	}
#endif
};

// -------------------------------------------------------------------------
// function connect

inline Connection cerl_call connect(const NInformation& host)
{
	return Connection(host);
}

inline Connection cerl_call connect(const char* hostname, Port port)
{
	return Connection(hostname, port);
}

inline Connection cerl_call connect(Connections& conns, const NInformation& host)
{
	return Connection(conns, host);
}

// -------------------------------------------------------------------------
// function multi_call

namespace detail {

// -------------------------------------------------------------------------
// class Multicall

template <class JobT>
class Multicall
{
private:
	struct SharedParam
	{
		FiberEvent* ev;
		size_t count;
	};

	struct CallParam
	{
		SharedParam* shared;
		const JobT* job;
		size_t i;
	};
	
	static void cerl_callback do_call(LPVOID lpParam)
	{
		FiberParam p(lpParam);

		CallParam const cp = *(CallParam*)p.val;
		
		FiberEvent::Notifier ev(cp.shared->ev);

		bool const fOk = cp.job->run(p.self, cp.i, ev);

		if (fOk && --cp.shared->count == 0)
		{
			CERL_ASSERT(ev.good());
			ev.notify(p.self);
		}
	}
	
public:
	static void cerl_call run(Fiber self, JobT* job, size_t n, size_t m, Timeout timeout, size_t dwStackSize = 0)
	{
		FiberEvent ev;
		
		IoService* const service = CerlIoService(self);
		SharedParam shared = { &ev, m };
		CallParam* params = (CallParam*)_alloca(sizeof(CallParam)*n);

		for (size_t i = 0; i < n; ++i)
		{
			CallParam cp = { &shared, job, i };
			params[i] = cp;
			service->spawnFiber(do_call, &params[i], dwStackSize);
		}
		
		ev.timed_wait(self, timeout);
	}
};

template <class ConnectionT, class ResultT, class ArgsT>
struct BatchJob
{
	ConnectionT* conns;
	ResultT* results;
	FID fid;
	const ArgsT* args;
	
	bool cerl_call run(Fiber self, size_t i, const FiberEvent::Notifier& ev) const
	{
		ResultT result = code_timeout_error;
		ConnectionT conn = conns[i];
		ScopedAlloc alloc(CerlBlockPool(self));
		ArgsT args2;
		NS_CERL_IO::copy(alloc, args2, *args);
		conn.dbg_call(&conn, alloc, result, fid, args2);
		if (ev.good() && result._code <= code_ok)
		{
			NS_CERL_IO::copy(alloc, results[i], result);
			return true;
		}
		return false;
	}
};

} // namespace detail

template <class ConnectionT, class ResultT, class ArgsT>
inline void cerl_call multi_call(
	ConnectionT conns[], ResultT results[], size_t n, size_t m,
	int fid, const ArgsT& args, size_t dwStackSize = 0)
{
#if defined(CERL_DEBUG)
	typedef detail::BatchJob<ConnectionT, ResultT, ArgsT> Job;
#else
	typedef detail::BatchJob<Connection, ResultT, ArgsT> Job;
#endif
	
	Fiber self = cerl::getCurrentFiber();

	Job job = { conns, results, (FID)fid, &args };
	detail::Multicall<Job>::run(self, &job, n, m, conns->get_timeout(), dwStackSize);
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif
