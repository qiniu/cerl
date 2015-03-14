/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/MultiIoService.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: IoServicePool.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IOSERVICEPOOL_H
#define ASYNC_IOSERVICEPOOL_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef STDEXT_THREAD_THREAD_H
#include "../../../stdext/include/stdext/thread/Thread.h"
#endif

NS_CERL_BEGIN

// -----------------------------------------------------------------------
// class MultiIoService

class MultiIoService
{
private:
	struct ThreadState
	{
		IoService* service;
		size_t conns;
		HTHREAD hThread;
	};

	struct Startup : public IoService::Task
	{
	private:
		FiberCallback m_lpStartAddress;
		void* m_startParam;

	public:
		Startup(FiberCallback lpStartAddress, void* startParam)
			: m_lpStartAddress(lpStartAddress),
			  m_startParam(startParam)
		{
		}

		void cerl_call run(IoService* service, DWORD dwStackSize)
		{
			service->createFiberAndSwith(m_lpStartAddress, m_startParam, dwStackSize);
		}
	};

	template <class SessionT>
	class SessionImpl : public IoService::Task
	{
	private:
		ThreadState* m_state;
		const SessionT m_session;

	public:
		SessionImpl(ThreadState* state, const SessionT& session)
			: m_state(state), m_session(session)
		{
		}

		void cerl_call run(IoService* service, DWORD dwStackSize)
		{
			service->createFiberAndSwith(FiberMain, this, dwStackSize);
		}

	private:
		static void cerl_callback FiberMain(LPVOID lpParam)
		{
			FiberParam p(lpParam);

			SessionImpl* s = (SessionImpl*)p.val;
			ThreadState* state = s->m_state;

			++state->conns;
			CERL_LOG("IoService", ("NEW-SESSION: service - %p (COUNT: %u)\n", state->service, state->conns));

			s->m_session.run(p.self);

			--state->conns;
			CERL_LOG("IoService", ("CLOSE-SESSION: service - %p (COUNT: %u)\n", state->service, state->conns));

			delete s;
		}
	};

	static THREADRET WINAPI ThreadMain(LPVOID lpThreadParameter)
	{
		IoService service;
		*(void**)lpThreadParameter = &service;
		service.run();
		return 0;
	}

private:
	ThreadState* m_threads;
	size_t m_nThread;
	size_t m_curr;

public:
	MultiIoService()
		: m_threads(NULL), m_nThread(0)
	{
	}

	template <class SessionT>
	void cerl_call startSession(Fiber self, const SessionT& session, size_t dwStackSize = 0)
	{
		CERL_ASSERT(m_nThread > 0);

		typedef SessionImpl<SessionT> Session;

		size_t conns = m_threads[0].conns;
		for (size_t i = 1; i < m_nThread; ++i)
			conns += m_threads[i].conns;

		while (m_threads[m_curr].conns * m_nThread > conns)
		{
			m_curr = (m_curr + 1) % m_nThread;
		}
		
		while (m_threads[m_curr].service == NULL)
		{
			SwitchToThread();
		}

		m_threads[m_curr].service->postTask(new Session(&m_threads[m_curr], session), dwStackSize);
	}

	void cerl_call start(size_t nThread = 2)
	{
		CERL_ASSERT(m_nThread == 0);

		m_curr = 0;
		m_nThread = nThread;
		m_threads = new ThreadState[nThread];
		for (size_t i = 0; i < nThread; ++i)
		{
			m_threads[i].conns = 0;
			m_threads[i].service = NULL;
			m_threads[i].hThread = CreateThread(ThreadMain, &m_threads[i].service);
		}
	}

	void cerl_call quit()
	{
		for (size_t i = 0; i < m_nThread; ++i)
		{
			m_threads[i].service->quit();
		}
	}

	void cerl_call run()
	{
		for (size_t i = 0; i < m_nThread; ++i)
		{
			WaitThread(m_threads[i].hThread);
			CloseThread(m_threads[i].hThread);
		}
		delete[] m_threads;
		m_threads = NULL;
		m_nThread = 0;
	}

	void cerl_call run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		while (m_threads[m_curr].service == NULL)
		{
			m_curr = (m_curr + 1) % m_nThread;
			SwitchToThread();
		}

		Startup startup(lpStartAddress, startParam);
		m_threads[m_curr].service->postTask(&startup, dwStackSize);

		run();
	}
};

// -------------------------------------------------------------------------
// class SingleIoService

class SingleIoService
{
private:
	IoService* m_service;

public:
	template <class SessionT>
	void cerl_call startSession(Fiber self, const SessionT& session, size_t dwStackSize = 0)
	{
		m_service->startSession(self, session, dwStackSize);
	}

	void cerl_call run()
	{
		m_service->run();
	}

	void cerl_call run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		m_service->run(lpStartAddress, startParam, dwStackSize);
	}

	void cerl_call start(size_t threadCount = 1)
	{
		static IoService g_service;
		m_service = &g_service;
	}

	void cerl_call quit()
	{
		m_service->quit();
	}
};

// -------------------------------------------------------------------------
// class IoServicePoolImpl

namespace detail {

template <class IoServicesT>
class IoServicePoolImpl
{
public:
	typedef IoServicesT services;
	static IoServicesT g_services;

	template <class SessionT>
	static void cerl_call startSession(Fiber self, const SessionT& session, size_t dwStackSize = 0)
	{
		g_services.startSession(self, session, dwStackSize);
	}

	static void cerl_call run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		g_services.run(lpStartAddress, startParam, dwStackSize);
	}

	static void cerl_call init(size_t threadCount = 2)
	{
		g_services.start(threadCount);
	}

	static void cerl_call term()
	{
	}

	static void cerl_call quit()
	{
		g_services.quit();
	}
};

template <class IoServicesT>
IoServicesT IoServicePoolImpl<IoServicesT>::g_services;

} // namespace detail

// -------------------------------------------------------------------------
// class IoServicePool

#if (0)
#define CERL_MULTI_IOSERVICE
#endif

#if defined(CERL_MULTI_IOSERVICE)
typedef detail::IoServicePoolImpl<MultiIoService> IoServicePool;
#else
typedef detail::IoServicePoolImpl<SingleIoService> IoServicePool;
#endif

#define CerlIoServicePool()	(&IoServicePool::g_services)

// -------------------------------------------------------------------------
// class IoServicePoolInit

class IoServicePoolInit
{
public:
	IoServicePoolInit(size_t threadCount = 2) {
		IoServicePool::g_services.start(threadCount);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_IOSERVICEPOOL_H */
