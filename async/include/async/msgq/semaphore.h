/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/msgq/semaphore.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: semaphore.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_MSGQ_SEMAPHORE_H
#define ASYNC_MSGQ_SEMAPHORE_H

#ifndef ASYNC_LIST_H
#include "../List.h"
#endif

#ifndef STDEXT_QUEUE_H
#include "../../../../stdext/include/stdext/Queue.h"
#endif

#ifndef STDEXT_THREAD_MUTEX_H
#include "../../../../stdext/include/stdext/thread/Mutex.h"
#endif

#ifndef STDEXT_THREAD_SEMAPHORE_H
#include "../../../../stdext/include/stdext/thread/Semaphore.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class LimitedMessageQueue

template <class ValT>
class LimitedMessageQueue
{
protected:
	typedef NS_STDEXT::Semaphore Semaphore;
	typedef NS_STDEXT::LimitedQueue<ValT> QueueT;
	typedef NS_STDEXT::Mutex CS;

	QueueT m_msgs;
	CS m_cs;
	Semaphore m_sem;
	Semaphore m_sem_full;

public:
	LimitedMessageQueue(ValT data[], size_t n)
		: m_msgs(data, n), m_sem_full(n)
	{
	}

	void cerl_call push(const ValT& val)
	{
		m_sem_full.acquire();
		{
			CS::scoped_lock lock(m_cs);
			CERL_TRACE_IF(m_msgs.full(), ("ERROR: try to put message to a full message-queue!\n"));
			m_msgs.push(val);
		}
		m_sem.release();
	}

	void cerl_call pop(ValT& val)
	{
		m_sem.acquire();
		{
			CS::scoped_lock lock(m_cs);
			CERL_TRACE_IF(m_msgs.empty(), ("ERROR: try to fetch message from an empty message-queue!\n"));
			val = m_msgs.front();
			m_msgs.pop();
		}
		m_sem_full.release();
	}
};

// -------------------------------------------------------------------------
// class MessageQueue

class MessageQueue
{
public:
	typedef ListNode Node;

protected:
	typedef NS_STDEXT::Semaphore Semaphore;
	typedef NS_STDEXT::Mutex CS;

	Queue m_msgs;
	CS m_cs;
	Semaphore m_sem;

public:
	void cerl_call push(Node* val)
	{
		{
			CS::scoped_lock lock(m_cs);
			m_msgs.push(val);
		}
		m_sem.release();
	}

	Node* cerl_call pop()
	{
		m_sem.acquire();
		{
			CS::scoped_lock lock(m_cs);
			CERL_TRACE_IF(m_msgs.empty(), ("ERROR: try to fetch message from an empty message queue!\n"));
			return m_msgs.pop();
		}
	}

	Node* cerl_call unsafe_clear()
	{
		return m_msgs.clear();
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_MSGQ_SEMAPHORE_H */
