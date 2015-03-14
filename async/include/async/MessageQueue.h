/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/MessageQueue.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: MessageQueue.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_MESSAGEQUEUE_H
#define ASYNC_MESSAGEQUEUE_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// See http://en.wikipedia.org/wiki/Producer-consumer_problem

#if (0)
#include "msgq/semaphore.h"
#elif defined(X_OS_LINUX)
#include "msgq/condition.h"
#elif defined(X_OS_WINDOWS)
#include "msgq/iocp.h"
#else
#error "error: Unsupport OS platform!!!"
#endif

// -------------------------------------------------------------------------
// class FiberMessageQueue

#ifndef ASYNC_LIST_H
#include "List.h"
#endif

NS_CERL_BEGIN

class FiberMessageQueue
{
public:
	typedef ListNode Node;

private:
	struct WaitingNode
	{
		WaitingNode* prev;
		Fiber fiber;
	};

	Queue m_queue;
	WaitingNode* m_waitingList;
	BOOL m_fWorking;

public:
	FiberMessageQueue()
		: m_fWorking(TRUE)
	{
	}

	// push val;
	// if waiting list is not empty: wakeup a fiber;
	//
	void cerl_call push(Fiber self, Node* val)
	{
		m_queue.push(val);
		if (m_waitingList)
		{
			CerlIoService(self)->scheduleFiber(m_waitingList->fiber);
			m_waitingList = m_waitingList->prev;
		}
	}

	// while message queue is empty && m_fWorking: push self into waiting list;
	// return the top element if has, or return NULL;
	//
	Node* cerl_call pop(Fiber self)
	{
		while (m_queue.empty() && m_fWorking)
		{
			WaitingNode node = { m_waitingList, self };
			m_waitingList = &node;
			yield(self);
		}
		return m_queue.pop();
	}

	// if message queue is empty: return NULL;
	// else: pop & return the top element;
	//
	Node* cerl_call try_pop()
	{
		return m_queue.pop();
	}

	// set m_fWorking = FALSE & wake up all fibers in waiting list.
	//
	void cerl_call close()
	{
		m_fWorking = FALSE;
		if (m_waitingList)
		{
			WaitingNode* node = m_waitingList;
			IoService* service = CerlIoService(node->fiber);
			m_waitingList = NULL;
			do
			{
				service->scheduleFiber(node->fiber);
				node = node->prev;
			}
			while (node);
		}
	}
};

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_MESSAGEQUEUE_H */
