/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Timer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Timer.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TIMER_H
#define ASYNC_TIMER_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef STDEXT_THREAD_THREAD_H
#include "../../../stdext/include/stdext/thread/Thread.h"
#endif

// -------------------------------------------------------------------------
// type Timeout

NS_CERL_BEGIN

typedef UINT32 Timeout; // in ms(millisecond) ºÁÃë

NS_CERL_END

#ifndef CERL_INFINITE
#define CERL_INFINITE	0xFFFFFFFFUL
#endif

#ifndef CERL_DEFAULT_TIMEOUT
#define CERL_DEFAULT_TIMEOUT	3000
#endif

// -------------------------------------------------------------------------

#if (1)
#include "timer/fast_timer.h"
#elif defined(X_OS_LINUX)
#include "timer/posix.h"
#elif defined(X_OS_WINDOWS)
#include "timer/windows.h"
#else
#error "error: Unsupport OS platform!!!"
#endif

// -------------------------------------------------------------------------
// function sleep

NS_CERL_BEGIN

__forceinline void cerl_call sleep(Fiber self, Timeout waitInMs)
{
	timed_yield(self, waitInMs);
}

inline void cerl_call sleep(Fiber self /*, Timeout = 0 */)
{
	IoService* const service = CerlIoService(self);
	service->scheduleFiber(self);
	switchToFiber(self, service->self);
}

NS_CERL_END

// -------------------------------------------------------------------------
// class Heartbeat

NS_CERL_BEGIN

template <class ServerT>
class Heartbeat
{
private:
	struct Param
	{
		ServerT* server;
		Timeout timeout;
	};

	static void cerl_callback FiberMain(void* lpParam)
	{
		cerl::FiberParam p(lpParam);

		Param para = *(Param*)p.val;
		
		bool fQuit = false;
		SetFlagOnQuit sfoq(p.service, fQuit);
		for (;;)
		{
			timed_yield(p.self, para.timeout);
			if (fQuit)
				break;
			para.server->onHeartbeat(p.self);
		}
	}

public:
	static void cerl_call start(Fiber self, ServerT* server, Timeout timeout)
	{
		Param para = { server, timeout };
		startFiber(self, FiberMain, &para);
	}
};

template <class ServerT>
inline void cerl_call heartbeat(Fiber self, ServerT* server, Timeout timeout)
{
	return Heartbeat<ServerT>::start(self, server, timeout);
}

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_TIMER_H */
