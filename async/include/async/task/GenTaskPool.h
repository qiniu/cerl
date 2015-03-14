/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/task/GenTaskPool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: GenTaskPool.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TASK_GENTASKPOOL_H
#define ASYNC_TASK_GENTASKPOOL_H

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
// class GenTaskPool

namespace detail {

class GenTaskPool
{
private:
	MessageQueue m_tasks;

public:
	typedef void (cerl_callback *TaskProc)(LPVOID lpParam);

	struct TaskStub
	{
		void* reserved;
		void* param;
		TaskProc proc;
		IoService::ScheduleMessage wakeup;
	};

public:
	GenTaskPool(size_t threadCount)
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
		for (;;)
		{
			TaskStub* task = (TaskStub*)m_tasks.pop();
			task->proc(task->param);
			task->wakeup.postMT();
		}
	}

	static THREADRET WINAPI ThreadMain(LPVOID lpThreadParameter)
	{
		Pools pools;
		NS_STDEXT::TlsPools::put(pools);
		((GenTaskPool*)lpThreadParameter)->main();
		return 0;
	}
};

template <int nInst>
struct GenTaskPoolParams
{
	static size_t g_threadCount;
};

template <int nInst>
size_t GenTaskPoolParams<nInst>::g_threadCount = 2;

} // namespace detail

// -------------------------------------------------------------------------
// class TaskPool, TaskPoolInit

typedef detail::GenTaskPool TaskPool;

class TaskPoolInit
{
public:
	typedef detail::GenTaskPoolParams<0> Params;

	TaskPoolInit(size_t threadCount = 2)
	{
		Params::g_threadCount = threadCount;
	}
};

// -------------------------------------------------------------------------
// function runTask

inline void cerl_call runTask(Fiber self, TaskPool::TaskProc proc, void* param)
{
	CERL_FIBER_ASSERT_SELF(self);

	static TaskPool g_taskPool(TaskPoolInit::Params::g_threadCount);

	IoService* const service = CerlIoService(self);
	TaskPool::TaskStub task = { NULL, param, proc, service->messageOfSchedule(self) };
	g_taskPool.addTask(&task);
	service->yield(self);
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_TASK_GENTASKPOOL_H */
