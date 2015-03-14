/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fiber/wincoro.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: wincoro.h 2705 2010-04-28 09:18:07Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_WINCORO_H
#define ASYNC_FIBER_WINCORO_H

#ifndef ASYNC_MEMORY_H
#include "../Memory.h"
#endif

#ifndef ASYNC_FIBER_CURRENT_H
#include "current.h"
#endif

#ifndef ASYNC_FIBER_WINCORO_CORO_H
#include "wincoro/coro.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// type FiberContext

namespace detail {

struct FiberContext
{
	enum { DefStackSize = CERL_FIBER_STKSIZE };
	LPVOID param; // must be first member!
	CoroContext context;
};

} // namespace detail

// -------------------------------------------------------------------------
// class Fibers

class Fibers
{
private:
	Pools& pools;
	Pool& pool;

	WINX_THREAD_CALLER_CHECK();

public:
	explicit Fibers(Pools& poolsArg)
		: pools(poolsArg),
		  pool(poolsArg.get_pool(sizeof(detail::FiberContext)))
	{
	}

	Fiber cerl_call create(
		LPFIBER_START_ROUTINE lpStartAddress,
		LPVOID lpParameter = NULL, size_t dwStackSize = 0)
	{
		using namespace detail;

		WINX_CHECK_THREAD();

		FiberContext* fiber = (FiberContext*)pool.allocate();
		const size_t stackSize = dwStackSize ? dwStackSize : FiberContext::DefStackSize;
		void* stk = pools.allocate(stackSize);

		fiber->param = lpParameter;

		coro_newcontext(
			&fiber->context, lpStartAddress, lpParameter,
			stk, pools.alloc_size(stk, stackSize));
		return (Fiber)fiber;
	}

	void cerl_call destroy(Fiber lpFiber)
	{
		WINX_CHECK_THREAD();

		detail::FiberContext* const fiber = (detail::FiberContext*)lpFiber;
		pools.deallocate(fiber->context.StackBase, (char*)fiber->context.Top - (char*)fiber->context.StackBase);
		pool.deallocate(lpFiber);
	}
};

// -------------------------------------------------------------------------
// function switchToFiber

inline void WINAPI switchToFiber(Fiber self, Fiber switchTo)
{
	CERL_FIBER_ASSERT_SELF(self);
	CERL_VLOG("Fiber", ("SWITCH: %p - Thread: %.8X\n", switchTo, (UINT)GetCurrentThreadId()));

	CurrentFiberImpl::put(switchTo);
	coro_swapcontext(
		&((detail::FiberContext*)self)->context,
		&((detail::FiberContext*)switchTo)->context);
}

inline void cerl_call switchToFiber(Fiber switchTo)
{
	switchToFiber(CurrentFiberImpl::get(), switchTo);
}

// -------------------------------------------------------------------------
// class ThreadToFiber

class ThreadToFiber
{
private:
	detail::FiberContext fiber;

public:
	Fiber cerl_call convert(LPVOID lpParameter = NULL)
	{
		fiber.param = lpParameter;
		CurrentFiberImpl::put((Fiber)&fiber);
		return (Fiber)&fiber;
	}

	void cerl_call unconvert()
	{
		CurrentFiberImpl::put(NULL);
	}
};

// -------------------------------------------------------------------------
// class FiberInit

typedef CurrentFiberInit FiberInit;

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_FIBER_WINCORO_H */
