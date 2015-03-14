/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/timer/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: windows.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TIMER_WINDOWS_H
#define ASYNC_TIMER_WINDOWS_H

NS_CERL_BEGIN

// -------------------------------------------------------------------------

namespace detail {

struct TimerRoutineParam
{
	IoService::ScheduleMessage wakeup;
	bool fOk;
};

inline VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	TimerRoutineParam&p = *(TimerRoutineParam*)lpParam;
	p.fOk = false;
	p.wakeup.post();
}

} // namespace detail

inline bool cerl_call timed_yield(Fiber self, Timeout timeout)
{
	CERL_FIBER_ASSERT_SELF(self);

	IoService* service = CerlIoService(self);
	detail::TimerRoutineParam p = { service->messageOfSchedule(self), true };

	HANDLE hTimer;
	CreateTimerQueueTimer(&hTimer, NULL, detail::TimerRoutine, &p, timeout, 0, WT_EXECUTEINTIMERTHREAD);
	service->yield(self);
	DeleteTimerQueueTimer(NULL, hTimer, INVALID_HANDLE_VALUE);

	return p.fOk;
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_TIMER_WINDOWS_H */
