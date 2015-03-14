/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Fiber.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Fiber.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_H
#define ASYNC_FIBER_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#if (1)
#define CERL_DISABLE_FIBER_API
#endif

// -------------------------------------------------------------------------
// macro CERL_FIBER_STKSIZE

#ifndef CERL_FIBER_STKSIZE
#define CERL_FIBER_STKSIZE		(1024 * 16 - 16)
#endif

// -------------------------------------------------------------------------
// LPFIBER_START_ROUTINE

#if !defined(X_OS_WINDOWS)

typedef VOID (* PFIBER_START_ROUTINE)(LPVOID lpFiberParameter);
typedef PFIBER_START_ROUTINE LPFIBER_START_ROUTINE;

#endif // !defined(X_OS_WINDOWS)

// -------------------------------------------------------------------------
// class Fiber

NS_CERL_BEGIN

typedef struct FiberInstance{}* Fiber;

typedef void (cerl_callback *FiberCallback)(LPVOID lpParam);

inline void* cerl_call getFiberData(Fiber fiber)
{
	return *(void**)fiber;
}

NS_CERL_END

// -------------------------------------------------------------------------

#if 0 //defined(X_OS_WINDOWS)
#include "fiber/wincoro.h"
#elif defined(X_OS_LINUX)
#include "fiber/coro.h"
#elif defined(X_OS_WINDOWS)
#include "fiber/windows.h"
#elif defined(X_OS_LINUX)
#include "fiber/ucontext.h"
#else
#error "Unsupport OS platform!!!"
#endif

// -------------------------------------------------------------------------
// disable GetCurrentFiber, etc.

#if defined(CERL_DISABLE_FIBER_API)

#undef GetCurrentFiber
#undef GetFiberData
#undef SwitchToFiber

#define GetCurrentFiber		__unsupport_function__
#define GetFiberData		__unsupport_function__
#define SwitchToFiber		__unsupport_function__

#endif

// -------------------------------------------------------------------------

#endif /* ASYNC_FIBER_H */
