/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/timer/fast_timer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: fast_timer.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TIMER_FAST_TIMER_H
#define ASYNC_TIMER_FAST_TIMER_H

#ifndef ASYNC_FASTTIMER_H
#include "../FastTimer.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class Timer, TimerQueue

typedef FastTimer Timer;
typedef FastTimerQueue TimerQueue;

// -------------------------------------------------------------------------

namespace detail {

class TimeoutTimer : public Timer
{
public:
	IoService::ScheduleMessage wakeup;
	bool fOk;

	void cerl_call onTimer(void* caller)
	{
		fOk = false;
		wakeup.postMT();
	}
};

} // namespace detail

// -------------------------------------------------------------------------
// class TimerService

template <int nInst>
class TimerServiceImpl
{
public:
	static TimerQueue* g_timerQueue;
	
public:
	static void cerl_callback initIoService(void* param)
	{
		IoService* service = (IoService*)param;
		
		Pools& pools = service->pools;
		PoolMT* pool = new(pools.allocate(sizeof(PoolMT))) PoolMT(pools, sizeof(detail::TimeoutTimer));
		
		service->timerR1 = pool;
	}

	static void cerl_call init(size_t timerPrecision = 500, size_t timerSlots = 31)
	{
		static IoService::InitChain::Node init(initIoService);
		static TimerQueue timerQueue(timerPrecision, timerSlots);
		
		g_timerQueue = &timerQueue;
		HTHREAD hThread = CreateThread(ThreadMain, &timerQueue);
		CloseThread(hThread);
	}

	static void cerl_call term()
	{
	}

private:
	static THREADRET WINAPI ThreadMain(LPVOID lpThreadParameter)
	{
		FastTimerQueue* timerQueue = (FastTimerQueue*)lpThreadParameter;
		FastTimerQueue::duration_type const precision = timerQueue->getPrecision();
		for (;;)
		{
			timerQueue->tick();
			Sleep(precision);
		}
		return 0;
	}
};

template <int nInst>
TimerQueue* TimerServiceImpl<nInst>::g_timerQueue;

typedef TimerServiceImpl<0> TimerService;

#define CerlTimerQueue() TimerService::g_timerQueue

// -------------------------------------------------------------------------

inline bool cerl_call timed_yield(Fiber self, Timeout timeout)
{
	CERL_FIBER_ASSERT_SELF(self);

	IoService* service = CerlIoService(self);
	
	detail::TimeoutTimer* timer;
	Timer::create(&timer, (PoolMT*)service->timerR1);

	timer->wakeup = service->messageOfSchedule(self);
	timer->fOk = true;

	CerlTimerQueue()->start(timer, timeout);
	service->yield(self);
	
	bool fOk = timer->fOk;
	timer->stop();
	return fOk;
}

// -------------------------------------------------------------------------
// class TimerInit

class TimerInit
{
public:
	explicit TimerInit(size_t timerPrecision = 500, size_t timerSlots = 31)
	{
		TimerService::init(timerPrecision, timerSlots);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_TIMER_WINDOWS_H */
