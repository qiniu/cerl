/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Mutex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Mutex.h 2716 2010-04-28 10:11:54Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_MUTEX_H
#define ASYNC_MUTEX_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef ASYNC_TIMER_H
#include "Timer.h"
#endif


#ifndef STDEXT_THREAD_MUTEX_H
#include "../../../stdext/include/stdext/thread/Mutex.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class ScopedLock

template <class ResourceT>
class ScopedLock
{
private:
	ResourceT& m_cs;

public:
	ScopedLock(Fiber self, ResourceT& cs) : m_cs(cs) {
		m_cs.acquire(self);
	}
	~ScopedLock() {
		m_cs.release();
	}
};

// -------------------------------------------------------------------------
// class FiberOnce

class FiberOnce
{
private:
	struct WaitingNode
	{
		WaitingNode* prev;
		Fiber fiber;
	};

	Fiber m_fiberLock;
	WaitingNode* m_waitingList;

public:
	FiberOnce()
		: m_fiberLock(NULL),
		  m_waitingList(NULL)
	{
	}

	Fiber cerl_call getCurrentFiber() const
	{
		return m_fiberLock;
	}

	bool cerl_call acquire(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock != self);

		if (m_fiberLock == NULL) {
			m_fiberLock = self;
			return true;
		}

		WaitingNode node = { m_waitingList, self };
		m_waitingList = &node;
		yield(self);
		return false;
	}

	void cerl_call release()
	{
		CERL_FIBER_ASSERT_SELF(m_fiberLock);
	
		IoService* service = CerlIoService(m_fiberLock);
		for (WaitingNode* node = m_waitingList; node; node = node->prev)
			service->scheduleFiber(node->fiber);
		m_waitingList = NULL;
		m_fiberLock = NULL;
	}
};

// -------------------------------------------------------------------------
// class NullMutex

class NullMutex
{
	Fiber m_fiberLock;

public:
	NullMutex()
		: m_fiberLock(NULL)
	{
	}
	
	Fiber cerl_call getCurrentFiber() const
	{
		return m_fiberLock;
	}

	bool cerl_call timed_acquire(Fiber self, Timeout timeout)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock != self && "recursive lock not allowed!");

		if (m_fiberLock == NULL) {
			m_fiberLock = self;
			return true;
		}

		CERL_ASSERT(!"lock failed!");

		return false;
	}

	void cerl_call acquire(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock != self && "recursive lock not allowed!");

		if (m_fiberLock == NULL) {
			m_fiberLock = self;
			return;
		}

		CERL_ASSERT(!"lock failed!");
	}

	void cerl_call release()
	{
		CERL_FIBER_ASSERT_SELF(m_fiberLock);
		m_fiberLock = NULL;
	}
};

// -------------------------------------------------------------------------
// class FiberMutex

class FiberMutex
{
private:
	struct WaitingNode : public DListNode
	{
		Fiber fiber;
	};

	Fiber m_fiberLock;
	DList m_waitingList;

public:
	FiberMutex()
		: m_fiberLock(NULL)
	{
	}
	
	Fiber cerl_call getCurrentFiber() const
	{
		return m_fiberLock;
	}

	bool cerl_call timed_acquire(Fiber self, Timeout timeout)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock != self && "recursive lock not allowed!");

		if (m_fiberLock == NULL) {
			m_fiberLock = self;
			return true;
		}

		WaitingNode node;
		node.fiber = self;
		m_waitingList.push_back(&node);
		if (timed_yield(self, timeout))
		{
			m_fiberLock = self;
			return true;
		}
		else
		{
			node.erase();
			if (m_fiberLock == self)
			{
				if (m_waitingList.empty())
				{
					m_fiberLock = NULL;
				}
				else
				{
					m_fiberLock = ((WaitingNode&)m_waitingList.front()).fiber;
					m_waitingList.pop_front();
					CerlIoService(m_fiberLock)->scheduleFiber(m_fiberLock);
				}
			}
			return false;
		}
	}

	void cerl_call acquire(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock != self && "recursive lock not allowed!");

		if (m_fiberLock == NULL) {
			m_fiberLock = self;
			return;
		}

		WaitingNode node;
		node.fiber = self;
		m_waitingList.push_back(&node);
		yield(self);
		
		CERL_ASSERT(m_fiberLock == self);
	}

	void cerl_call release()
	{
		CERL_FIBER_ASSERT_SELF(m_fiberLock);

		if (m_waitingList.empty())
		{
			m_fiberLock = NULL;
			return;
		}

		WaitingNode& node = (WaitingNode&)m_waitingList.front();
		m_waitingList.pop_front();
		
		m_fiberLock = node.fiber;
		CerlIoService(m_fiberLock)->scheduleFiber(m_fiberLock);
	}

	typedef ScopedLock<FiberMutex> scoped_lock;
};

// -------------------------------------------------------------------------
// class FiberNestableMutex

class FiberNestableMutex
{
private:
	FiberMutex m_mutex;
	ptrdiff_t m_nLock;

public:
	Fiber cerl_call getCurrentFiber() const
	{
		return m_mutex.getCurrentFiber();
	}

	void cerl_call acquire(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);

		if (m_mutex.getCurrentFiber() == self)
		{
			++m_nLock;
		}
		else
		{
			m_mutex.acquire(self);
			m_nLock = 1;
		}
	}

	void cerl_call release()
	{
		if (--m_nLock == 0)
			m_mutex.release();
	}

	typedef ScopedLock<FiberNestableMutex> scoped_lock;
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_MUTEX_H */
