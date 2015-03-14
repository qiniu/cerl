/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/task/ShareFiberTaskPool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: ShareFiberTaskPool.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TASK_SHAREFIBERTASKPOOL_H
#define ASYNC_TASK_SHAREFIBERTASKPOOL_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#ifndef ASYNC_MESSAGEQUEUE_H
#include "../MessageQueue.h"
#endif

#ifndef STDEXT_THREAD_THREAD_H
#include "../../../../stdext/include/stdext/thread/Thread.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class ShareFiberTaskPool

namespace detail {

class ShareFiberTaskPool
{
private:
	MessageQueue m_tasks;

public:
	typedef void* TaskProc;

	struct TaskStub
	{
		void* reserved;
		Fiber fiberMain;
		IoService::ScheduleMessage wakeup;
	};

public:
	ShareFiberTaskPool(size_t threadCount)
	{
		start(threadCount);
	}

	void cerl_call start(size_t threadCount = 2)
	{
		while (threadCount--)
		{
			HTHREAD hThread = CreateThread(ThreadMain, this);
			CloseThread(hThread);
		}
	}

	void cerl_call addTask(TaskStub* task)
	{
		m_tasks.push((MessageQueue::Node*)task);
	}

private:
	void cerl_call main() // 单个thread的代码
	{
		ThreadToFiber threadToFiber;
		const Fiber fiberMain = threadToFiber.convert();
		for (;;)
		{
			TaskStub* task = (TaskStub*)m_tasks.pop();
			task->fiberMain = fiberMain;
			switchToFiber(fiberMain, task->wakeup.fiber);
			task->wakeup.postMT();
		}
	}

	static THREADRET WINAPI ThreadMain(LPVOID lpThreadParameter)
	{
		((ShareFiberTaskPool*)lpThreadParameter)->main();
		return 0;
	}
};

template <int nInst>
struct ShareFiberTaskPoolParams
{
	static size_t g_threadCount;
};

template <int nInst>
size_t ShareFiberTaskPoolParams<nInst>::g_threadCount = 2;

} // namespace detail

// -------------------------------------------------------------------------
// class TaskRunnerInit

class TaskRunnerInit
{
public:
	typedef detail::ShareFiberTaskPoolParams<0> Params;

	TaskRunnerInit(size_t threadCount = 2)
	{
		Params::g_threadCount = threadCount;
	}
};

// -------------------------------------------------------------------------
// class TaskRunner

class TaskRunner :
	private detail::ShareFiberTaskPool::TaskStub,
	private IoService::Task
{
private:

	void cerl_call run(IoService* service, DWORD uToken)
	{
		static detail::ShareFiberTaskPool g_taskPool(TaskRunnerInit::Params::g_threadCount);
		g_taskPool.addTask(this);
	}

public:
	TaskRunner(Fiber self = NULL)
	{
		if (!self)
			self = getCurrentFiber();

		CERL_FIBER_ASSERT_SELF(self);
		IoService* const service = CerlIoService(self);
		wakeup = service->messageOfSchedule(self);
		service->postTaskST(this);
		switchToFiber(self, service->self);
	}

	~TaskRunner()
	{
		switchToFiber(wakeup.fiber, fiberMain);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_TASK_SHAREFIBERTASKPOOL_H */
