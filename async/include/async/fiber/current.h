/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fiber/current.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: current.h 2705 2010-04-28 09:18:07Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_CURRENT_H
#define ASYNC_FIBER_CURRENT_H

#ifndef ASYNC_BASIC_H
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------
// CurrentFiber

#if defined(X_OS_WINDOWS) && !defined(ASYNC_FIBER_WINCORO_H)

NS_CERL_BEGIN

class CurrentFiberImpl
{
public:
	static void cerl_call put(Fiber fiber) {}
	static Fiber cerl_call get() {
		return (Fiber)GetCurrentFiber();
	}
};

class CurrentFiberInit
{
};

inline Fiber cerl_call getCurrentFiber()
{
	return (Fiber)GetCurrentFiber();
}

inline PVOID cerl_call getFiberData()
{
	return GetFiberData();
}

NS_CERL_END

#else

#ifndef ASYNC_FIBER_CURRENT_DETAIL_H
#include "current/detail.h"
#endif

#endif

// -------------------------------------------------------------------------
// macro CERL_FIBER_ASSERT_SELF

#if defined(_DEBUG)

inline bool cerl_call IsCurrentFiber(cerl::Fiber fiber)
{
	return cerl::getCurrentFiber() == fiber;
}

#define CERL_FIBER_ASSERT_SELF(self)	\
do { \
	if (!IsCurrentFiber(self)) \
		CERL_ERROR("Fiber", ("WARN: ASSERT_SELF failed - Current: %p, Required: %p\n", cerl::getCurrentFiber(), self)); \
} while (0)

#else

#define CERL_FIBER_ASSERT_SELF(self)	(void)0

#endif

// -------------------------------------------------------------------------

#endif /* ASYNC_FIBER_CURRENT_H */
