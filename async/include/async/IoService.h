/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/IoService.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: IoService.h 2743 2010-05-05 08:21:38Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IOSERVICE_H
#define ASYNC_IOSERVICE_H

#ifndef ASYNC_FIBER_H
#include "Fiber.h"
#endif

#ifndef ASYNC_IOCOMPLETIONPORT_H
#include "IoCompletionPort.h"
#endif

#ifndef ASYNC_QUITEVENT_H
#include "QuitEvent.h"
#endif

#ifndef ASYNC_INITCHAIN_H
#include "InitChain.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// type FiberData

class IoService;

typedef DWORD FiberSerialNo;

#pragma pack(1)

namespace detail {

struct FiberData
{
	IoService* service;
	void* startParam;
	Fiber self;
	FiberSerialNo sn;
	int errNo;
};

} // namespace detail

#pragma pack()

#define CerlBlockPool(fiber)			(CerlIoService(fiber)->recycle)
#define CerlFiberData(fiber)			(*(cerl::detail::FiberData**)(fiber))
#define CerlFiberSerialNo(fiber)		(CerlFiberData(fiber)->sn)
#define CerlIoService(fiber)			(CerlFiberData(fiber)->service)
#define CerlIoTimeout(fiber)			(CERL_DEFAULT_TIMEOUT)
#define CerlMemPools(fiber)				(CerlIoService(fiber)->pools)
#define CerlMemPool(fiber, cbElem)		(CerlIoService(fiber)->pools.get_pool(cbElem))
#define CerlDefaultLog(fiber)			(CerlIoService(fiber)->log)
#define CerlFiberErrno(fiber)			(CerlFiberData(fiber)->errNo)

// -------------------------------------------------------------------------
// class DefaultLog

typedef NS_STDEXT::OutputLog DefaultLog;

// -------------------------------------------------------------------------
// class IoService

class IoService
{
public:
	BlockPool recycle;
	Pools pools;
	IoCompletionPort iocpMT;
	IoCompletionPortST iocpST;
	Fiber const self; // 主Fiber句柄。
	Fibers fibers; // Fiber管理者。负责生成、销毁Fiber对象。
	Pool& fiberDataPool; // 用来 new FiberData。
	ThreadToFiber threadToFiber;
	QuitEvent quitEvent;
	DWORD quitLockRef;
	DWORD const threadId;
	DefaultLog log;
	void* timerR1; // 为timer服务保留

#if defined(X_OS_WINDOWS)
	struct OVERLAPPED_IO
	{
		OVERLAPPED Overlapped; // must be first member!
		Fiber fiber;
		size_t* result;
		FiberSerialNo sn;
	};
	struct OVERLAPPED_ACCEPT
	{
		OVERLAPPED Overlapped;
		Fiber fiber;
		FiberSerialNo sn;

		void cerl_call initSerialNo()
			{ sn = CerlFiberSerialNo(fiber); }
	};
	Pool& overlappedPool; // 用来 new Overlapped，仅在 Windows 平台下需要。
#endif

	typedef cerl::InitChain<IoService> InitChain;

public:
	enum COMPLETION_KEY
	{
		ClientIoTask		= -6,	// dw = uToken, lp = task;
		ClientIoDeleteFiber	= -5,	// lp = fiberToDelete;
		ClientIoSchedule	= -4,	// dw = sn, lp = fiberToWakeup;
		ClientIoAccept		= -3,	// lp = overlapped;
		ClientIoReadWrite	= -2,	// dw = bytes, lp = overlapped;
		ClientIoQuit		= -1,
		ClientIoNoop		= 0,
	};

	struct ScheduleMessage
	{
		IoService* service;
		Fiber fiber;
		DWORD sn;

		void cerl_mtcall postST() const {
			service->iocpST.post(sn, (ULONG_PTR)ClientIoSchedule, (LPOVERLAPPED)fiber);
		}

		void cerl_mtcall postMT() const {
			service->iocpMT.post(sn, (ULONG_PTR)ClientIoSchedule, (LPOVERLAPPED)fiber);
		}
	};

	struct Task
	{
		virtual void cerl_call run(IoService* service, DWORD uToken) = 0;
	};

public:
	explicit IoService()
		: self(threadToFiber.convert(&recycle)),
		  fibers(pools),
		  fiberDataPool(pools.get_pool(sizeof(detail::FiberData))),
#if defined(X_OS_WINDOWS)
		  overlappedPool(pools.get_pool(sizeof(OVERLAPPED_IO))),
#endif
		  quitLockRef(0),
		  threadId(GetCurrentThreadId()),
		  timerR1(NULL)
	{
		CERL_ASSERT(this == (void*)&recycle);
		CERL_LOG("IoService", ("START: IoService - %p, MainFiber - %p\n", this, self));
		TlsPools::put(pools);
		InitChain::init(this);
	}

	ScheduleMessage cerl_call messageOfSchedule(Fiber fiber)
	{
		ScheduleMessage msg = { this, fiber, CerlFiberSerialNo(fiber) };
		return msg;
	}

	void cerl_call scheduleFiber(Fiber fiber)
	{
		CERL_ASSERT(threadId == GetCurrentThreadId());

		iocpST.post(CerlFiberSerialNo(fiber), (ULONG_PTR)ClientIoSchedule, (LPOVERLAPPED)fiber);
	}

	void cerl_call postTaskST(Task* task, DWORD uToken = 0)
	{
		CERL_ASSERT(threadId == GetCurrentThreadId());

		iocpST.post(uToken, (ULONG_PTR)ClientIoTask, (LPOVERLAPPED)task);
	}

	void cerl_mtcall postTask(Task* task, DWORD uToken = 0)
	{
		if (threadId == GetCurrentThreadId())
			iocpST.post(uToken, (ULONG_PTR)ClientIoTask, (LPOVERLAPPED)task);
		else
			iocpMT.post(uToken, (ULONG_PTR)ClientIoTask, (LPOVERLAPPED)task);
	}

	void cerl_mtcall quit()
	{
		iocpMT.post(0, (ULONG_PTR)ClientIoQuit, NULL);
	}

private:
	enum { QUIT_MASK = 0x40000000 };

	Fiber cerl_call createFiber(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		detail::FiberData* p = (detail::FiberData*)fiberDataPool.allocate();
		p->service = this;
		p->sn = 0;
		p->startParam = startParam;
		p->self = fibers.create(lpStartAddress, p, dwStackSize);
		++quitLockRef;
		CERL_VLOG("Fiber", ("SWPAN: %p, stack size: %zu\n", p->self, dwStackSize));
		return p->self;
	}

	void cerl_call deleteFiber(Fiber fiberToKill)
	{
		CERL_ASSERT(fiberToKill);
		CERL_ASSERT(!IsCurrentFiber(fiberToKill));

		detail::FiberData* p = CerlFiberData(fiberToKill);
		fiberDataPool.deallocate(p);
		fibers.destroy(fiberToKill);
		--quitLockRef;

		CERL_VLOG("Fiber", ("DELETE FIBER: %p (COUNT: %u)\n", fiberToKill, quitLockRef & ~QUIT_MASK));
	}

public:
	Fiber cerl_call createFiberAndSwith(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		CERL_FIBER_ASSERT_SELF(self);

		Fiber fiber = createFiber(lpStartAddress, startParam, dwStackSize);
		switchToFiber(self, fiber);
		return fiber;
	}

	void cerl_call resumeFiber(FiberSerialNo sn, Fiber fiberToWakeup)
	{
		CERL_FIBER_ASSERT_SELF(self);

		if (sn == CerlFiberSerialNo(fiberToWakeup))
		{
			switchToFiber(self, fiberToWakeup);
		}
		else
		{
			CERL_WARN("IoService", ("WARN: IoService::resumeFiber %p canceled - sn: %d, current: %d\n",
				fiberToWakeup, (int)sn, (int)CerlFiberSerialNo(fiberToWakeup)));
		}
	}

public:
	class FiberParam
	{
	public:
		Fiber const self;
		void* const val;
		IoService* const service;

#define CerlFiberParamData_ ((detail::FiberData*)(lpParam))

	public:
		FiberParam(LPVOID lpParam)
			: self(CerlFiberParamData_->self),
			  val(CerlFiberParamData_->startParam),
			  service(CerlFiberParamData_->service)
		{
		}

		~FiberParam()
		{
			service->exitFiber(self);
		}
	};

public:
	Fiber cerl_call spawnFiber(
		FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		Fiber const fiber = createFiber(lpStartAddress, startParam, dwStackSize);
		scheduleFiber(fiber);
		return fiber;
	}

	Fiber cerl_call startFiber(
		Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
	{
		CERL_FIBER_ASSERT_SELF(self);

		Fiber const fiber = createFiber(lpStartAddress, startParam, dwStackSize);
		scheduleFiber(self);
		switchToFiber(self, fiber);
		return fiber;
	}

	void cerl_call exitFiberAndSwitch(Fiber self, Fiber fiberSwitchTo)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(threadId == GetCurrentThreadId());

		CERL_VLOG("Fiber", ("EXIT FIBER: %p\n", self));
		iocpST.post(0, (ULONG_PTR)ClientIoDeleteFiber, (LPOVERLAPPED)self);

		switchToFiber(self, fiberSwitchTo);
	}

	void cerl_call exitFiber(Fiber self)
	{
		exitFiberAndSwitch(self, this->self);
	}

	void cerl_call yield(Fiber self)
	{
		switchToFiber(self, this->self);
		++CerlFiberSerialNo(self);
	}

public:
	void cerl_call run();
	void cerl_call run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0);

private:
	template <class SessionT>
	class SessionImpl
	{
	public:
		static void cerl_callback FiberMain(LPVOID lpParam)
		{
			FiberParam p(lpParam);
			((const SessionT*)p.val)->run(p.self);
		}
	};

public:
	struct Session
	{
		void cerl_call run(Fiber self) const {
		}
	};

	template <class SessionT>
	void cerl_call startSession(Fiber self, const SessionT& session, size_t dwStackSize = 0)
	{
		CERL_ASSERT( CerlIoService(self) == this );

		typedef SessionImpl<SessionT> Session;
		startFiber(self, Session::FiberMain, (void*)&session, dwStackSize);
	}

private:
	bool cerl_call processMessage_(DWORD bytes, ULONG_PTR key, LPOVERLAPPED overlapped);
	void cerl_call processMessageQueueST_();
};

// -------------------------------------------------------------------------
// free functions for fiber controlling

inline Fiber cerl_call startFiber(Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
{
	return CerlIoService(self)->startFiber(self, lpStartAddress, startParam, dwStackSize);
}

inline Fiber cerl_call spawnFiber(Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
{
	return CerlIoService(self)->spawnFiber(lpStartAddress, startParam, dwStackSize);
}

inline void cerl_call exitFiber(Fiber self)
{
	IoService* const service = CerlIoService(self);
	service->exitFiberAndSwitch(self, service->self);
}

inline void cerl_call yield(Fiber self)
{
	detail::FiberData* d = CerlFiberData(self);
	switchToFiber(self, d->service->self);
	++d->sn;
}

// -------------------------------------------------------------------------
// class FiberParam

typedef IoService::FiberParam FiberParam;

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_IOSERVICE_H */
