/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/IoService.inl
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: IoService.inl 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IOSERVICE_INL
#define ASYNC_IOSERVICE_INL

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// function IoService::run

NS_CERL_BEGIN

inline bool cerl_call IoService::processMessage_(DWORD bytes, ULONG_PTR key, LPOVERLAPPED overlapped)
{
	switch (key)
	{
#if defined(X_OS_WINDOWS)
	case ClientIoReadWrite:
		{
			OVERLAPPED_IO* o = (OVERLAPPED_IO*)overlapped;
			if (o->sn == CerlFiberSerialNo(o->fiber))
			{
				*o->result = bytes;
				switchToFiber(self, o->fiber);
			}
			overlappedPool.deallocate(o);
		}
		break;
#endif
	case ClientIoSchedule:
		resumeFiber(bytes, (Fiber)overlapped);
		break;
	case ClientIoDeleteFiber:
		deleteFiber((Fiber)overlapped);
		break;
#if defined(X_OS_WINDOWS)
	case ClientIoAccept:
		{
			OVERLAPPED_ACCEPT* o = (OVERLAPPED_ACCEPT*)overlapped;
			resumeFiber(o->sn, o->fiber);
		}
		break;
#endif
	case ClientIoTask:
		((Task*)overlapped)->run(this, bytes);
		break;
	case ClientIoQuit:
		quitEvent.fire();
		quitLockRef |= QUIT_MASK;
		CERL_LOG("IoService", ("QUIT: IoService - %p, MainFiber - %p\n", this, self));
		break;
	default:
		return false;
	}
	return true;
}

inline void cerl_call IoService::processMessageQueueST_()
{
	CERL_ASSERT(threadId == GetCurrentThreadId());

	enum { BATCH_COUNT = 1024 };

	size_t n = BATCH_COUNT;

	while (n-- && !iocpST.empty())
	{
		const IoCompletionPort::Message& msg = iocpST.front();
		processMessage_(msg.bytes, msg.key, msg.overlapped);
		iocpST.pop_front();
	}
}

NS_CERL_END

// -------------------------------------------------------------------------

#if defined(X_OS_LINUX)
#include "disp/epoll.h"
#elif defined(X_OS_WINDOWS)
#include "disp/iocp.h"
#else
#error "error: Unsupport OS platform!!!"
#endif

// -------------------------------------------------------------------------
// function IoService::run

NS_CERL_BEGIN

inline void cerl_call IoService::run(FiberCallback lpStartAddress, void* startParam, size_t dwStackSize)
{
	spawnFiber(lpStartAddress, startParam, dwStackSize);
	run();
}

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_IOSERVICE_INL */
