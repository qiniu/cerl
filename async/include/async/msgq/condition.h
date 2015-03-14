/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/msgq/condition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: condition.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_MSGQ_CONDITION_H
#define ASYNC_MSGQ_CONDITION_H

#ifndef ASYNC_LIST_H
#include "../List.h"
#endif

#ifndef STDEXT_QUEUE_H
#include "../../../../stdext/include/stdext/Queue.h"
#endif

#ifndef STDEXT_THREAD_MUTEX_H
#include "../../../../stdext/include/stdext/thread/Mutex.h"
#endif

#ifndef STDEXT_THREAD_CONDITION_H
#include "../../../../stdext/include/stdext/thread/Condition.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class LimitedMessageQueue

template <class ValT>
class LimitedMessageQueue
{
protected:
	typedef NS_STDEXT::Condition Condition;
	typedef NS_STDEXT::LimitedQueue<ValT> QueueT;
	typedef Condition::mutex_type CS;

	QueueT m_msgs;
	CS m_monitor;
	Condition m_cond_empty;
	Condition m_cond_full;

public:
	LimitedMessageQueue(ValT data[], size_t n)
		: m_msgs(data, n)
	{
	}

	bool cerl_call try_push(const ValT& val)
	{
		CS::scoped_lock lock(m_monitor);
		if (m_msgs.full())
			return false;
		const bool fempty = m_msgs.empty();
		m_msgs.push(val);
		if (fempty)
			m_cond_empty.notify_one();
		return true;
	}

	void cerl_call push(const ValT& val)
	{
		CS::scoped_lock lock(m_monitor);
		while (m_msgs.full())
			m_cond_full.wait(m_monitor);
		const bool fempty = m_msgs.empty();
		m_msgs.push(val);
		if (fempty)
			m_cond_empty.notify_one();
	}

	void cerl_call pop(ValT& val)
	{
		CS::scoped_lock lock(m_monitor);
		while (m_msgs.empty())
			m_cond_empty.wait(m_monitor);
		const bool ffull = m_msgs.full();
		val = m_msgs.front();
		m_msgs.pop();
		if (ffull)
			m_cond_full.notify_one();
	}

	bool cerl_call try_pop(ValT& val)
	{
		CS::scoped_lock lock(m_monitor);
		if (m_msgs.empty())
			return false;
		const bool ffull = m_msgs.full();
		val = m_msgs.front();
		m_msgs.pop();
		if (ffull)
			m_cond_full.notify_one();
		return true;
	}

	void cerl_call clear()
	{
		CS::scoped_lock lock(m_monitor);
		m_msgs.clear();
	}
};

// -------------------------------------------------------------------------
// class MessageQueue

class MessageQueue
{
public:
	typedef ListNode Node;

protected:
	typedef NS_STDEXT::Condition Condition;
	typedef Condition::mutex_type CS;

	Queue m_msgs;
	CS m_monitor;
	Condition m_cond_empty;

public:
    MessageQueue()
    {
    }
    
public:
	void cerl_call push(Node* val)
	{
		CS::scoped_lock lock(m_monitor);
		const bool fempty = m_msgs.empty();
		m_msgs.push(val);
		if (fempty)
			m_cond_empty.notify_all();
	}

	Node* cerl_call pop()
	{
		CS::scoped_lock lock(m_monitor);
		while (m_msgs.empty())
			m_cond_empty.wait(m_monitor);
		return m_msgs.pop();
	}
	
	Node* cerl_call clear()
	{
		CS::scoped_lock lock(m_monitor);
		return m_msgs.clear();
	}

	Node* cerl_call unsafe_clear()
	{
		return m_msgs.clear();
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_MSGQ_CONDITION_H */
