/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/task/FiberTaskPool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: FiberTaskPool.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TASK_FIBERTASKPOOL_H
#define ASYNC_TASK_FIBERTASKPOOL_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#ifndef ASYNC_MESSAGEQUEUE_H
#include "../MessageQueue.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class FiberTaskPool

class FiberTaskPool
{
private:
	FiberMessageQueue m_tasks;
	CloseObjectOnQuit<FiberMessageQueue> m_quit;

public:
	struct Task
	{
		void* reserved;
		void* proc;
	};

	typedef void (cerl_callback *TaskProc)(Fiber self, Task* lpParam);

public:
	FiberTaskPool()
		: m_quit(m_tasks)
	{
	}

	void cerl_call start(Fiber self, size_t fiberCount)
	{
		IoService* const service = CerlIoService(self);
		m_quit.sink(service);

		while (fiberCount--)
			service->startFiber(self, FiberMain, &m_tasks);
	}

	void cerl_call addTask(Fiber self, Task* task, TaskProc proc)
	{
		task->proc = (void*)proc;
		m_tasks.push(self, (FiberMessageQueue::Node*)task);
	}

private:
	static void cerl_callback FiberMain(LPVOID lpParam)
	{
		FiberParam p(lpParam);

		FiberMessageQueue& tasks = *(FiberMessageQueue*)p.val;
		for (;;)
		{
			Task* task = (Task*)tasks.pop(p.self);
			if (task == NULL) // quit
				break;
			((TaskProc)task->proc)(p.self, task);
		}
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_TASK_FIBERTASKPOOL_H */
