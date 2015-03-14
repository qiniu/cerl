/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Event.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Event.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_EVENT_H
#define ASYNC_EVENT_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef ASYNC_LIST_H
#include "List.h"
#endif

#ifndef ASYNC_TIMER_H
#include "Timer.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class FiberEvent

class FiberEvent
{
private:
	Fiber m_fiberLock;
	DList m_notifierList;
	BOOL m_fEventFired;

public:
	FiberEvent()
		: m_fiberLock(NULL),
		  m_fEventFired(FALSE)
	{
	}
	~FiberEvent()
	{
		do_clear_();
	}

	Fiber cerl_call getCurrentFiber() const
	{
		return m_fiberLock;
	}

	void cerl_call reset()
	{
		m_fEventFired = FALSE;
	}

	void cerl_call wait(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock == NULL);

		if (!m_fEventFired)
		{
			m_fiberLock = self;
			yield(self);
			m_fiberLock = NULL;
		}
		m_fEventFired = FALSE;
	}

	bool cerl_call timed_wait(Fiber self, Timeout timeout)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_fiberLock == NULL);

		if (!m_fEventFired)
		{
			m_fiberLock = self;
			if (!timed_yield(self, timeout))
			{
				do_clear_();
				m_fiberLock = NULL;
				return false;
			}
			m_fiberLock = NULL;
		}
		m_fEventFired = FALSE;
		return true;
	}

public:
	class Notifier : private DListNode
	{
	private:
		FiberEvent* m_event;
		friend class FiberEvent;

	public:
		Notifier(FiberEvent* ev)
		{
			if (!ev->m_fEventFired)
			{
				m_event = ev;
				m_event->m_notifierList.push_back(this);
			}
			else
			{
				m_event = NULL;
			}
		}

		~Notifier()
		{
			erase();
		}

		bool cerl_call good() const
		{
			return m_event != NULL;
		}

		void cerl_call notify(Fiber self)
		{
			CERL_FIBER_ASSERT_SELF(self);
			if (m_event)
			{
				m_event->do_notify_(self);
				m_event = NULL;
			}
		}
	};

	friend class Notifier;

private:
	void cerl_call do_clear_()
	{
		for (Notifier* p = (Notifier*)m_notifierList.first(); !m_notifierList.done(p); )
		{
			p->m_event = NULL;
			p = (Notifier*)p->next();
		}
		m_notifierList.erase();
	}

	void cerl_call do_notify_(Fiber self)
	{
		do_clear_();
		if (m_fiberLock)
		{
			CerlIoService(m_fiberLock)->scheduleFiber(self);
			switchToFiber(self, m_fiberLock);
		}
		else
		{
			m_fEventFired = TRUE;
		}
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_EVENT_H */
