/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fiber/coro.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: coro.h 2705 2010-04-28 09:18:07Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_CORO_H
#define ASYNC_FIBER_CORO_H

#ifndef ASYNC_MEMORY_H
#include "../Memory.h"
#endif

#ifndef ASYNC_FIBER_CURRENT_H
#include "current.h"
#endif

// -------------------------------------------------------------------------
// import libcoro

#if defined(X_OS_LINUX) && !defined(__linux)
#define __linux
#endif

#if defined(X_CPU_X86) && !defined(__x86)
#define __x86
#endif 

extern "C"
{
	#include "libcoro/coro.h"
};

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// type FiberContext

namespace detail {

struct FiberContext
{
	enum { DefStackSize = CERL_FIBER_STKSIZE };
	LPVOID param; // must be first member!
	coro_context context;
	LPVOID stkbase;
	size_t stksize;
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
		const size_t stackSize = dwStackSize ? dwStackSize : (size_t)FiberContext::DefStackSize;

		fiber->stkbase = pools.allocate(stackSize);
		fiber->stksize = pools.alloc_size(fiber->stkbase, stackSize);
		fiber->param = lpParameter;

		void* stktop = (void*)(((size_t)fiber->stkbase + fiber->stksize) & ~0xf);
		void* stkbase = (void*)(((size_t)fiber->stkbase + 0xf) & ~0xf);
		size_t stksize = (char*)stktop - (char*)stkbase;
		coro_create(&fiber->context, lpStartAddress, lpParameter, stkbase, stksize);
		return (Fiber)fiber;
	}

	void cerl_call destroy(Fiber lpFiber)
	{
		WINX_CHECK_THREAD();

		detail::FiberContext* const fiber = (detail::FiberContext*)lpFiber;
		pools.deallocate(fiber->stkbase, fiber->stksize);
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
	coro_transfer(
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

class FiberInit : public CurrentFiberInit
{
public:
	FiberInit() {
		coro_initialize();
	}
	~FiberInit() {
		coro_terminate();
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_FIBER_CORO_H */
