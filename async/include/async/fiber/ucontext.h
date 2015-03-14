/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fiber/ucontext.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: ucontext.h 2705 2010-04-28 09:18:07Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_UCONTEXT_H
#define ASYNC_FIBER_UCONTEXT_H

#ifndef ASYNC_MEMORY_H
#include "../Memory.h"
#endif

#ifndef ASYNC_FIBER_CURRENT_H
#include "current.h"
#endif

#ifndef UCONTEXT_H
#include <ucontext.h>
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// type FiberContext

namespace detail {

struct FiberContext
{
	enum { DefStackSize = CERL_FIBER_STKSIZE };
	LPVOID param; // must be first member!
	ucontext_t context;
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

		typedef void (*p_func)();

		FiberContext* fiber = (FiberContext*)pool.allocate();
		const size_t stackSize = dwStackSize ? dwStackSize : FiberContext::DefStackSize;

		getcontext(&fiber->context);

		fiber->context.uc_stack.ss_sp = pools.allocate(stackSize);
		fiber->context.uc_stack.ss_size = pools.alloc_size(fiber->context.uc_stack.ss_sp, stackSize);
		fiber->param = lpParameter;

		makecontext(&fiber->context, (p_func)lpStartAddress, 1, lpParameter);

		return (Fiber)fiber;
	}

	void cerl_call destroy(Fiber lpFiber)
	{
		WINX_CHECK_THREAD();

		ucontext_t& context = ((detail::FiberContext*)lpFiber)->context;
		pools.deallocate(context.uc_stack.ss_sp, context.uc_stack.ss_size);
		pool.deallocate(lpFiber);
	}
};

// -------------------------------------------------------------------------
// function switchToFiber

inline void cerl_call switchToFiber(Fiber self, Fiber switchTo)
{
	CERL_FIBER_ASSERT_SELF(self);
	CERL_VLOG("Fiber", ("SWITCH: %p - Thread: %.8X\n", switchTo, (UINT)GetCurrentThreadId()));

	CurrentFiberImpl::put(switchTo);
	swapcontext(
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
		getcontext(&fiber.context);
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

#endif /* ASYNC_FIBER_UCONTEXT_H */
