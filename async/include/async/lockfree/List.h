/* -------------------------------------------------------------------------
// CERL: C++ Erlang Server Model
// 
// Module: async/lockfree/List.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: List.h 2804 2010-05-10 04:01:03Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_LOCKFREE_LIST_H
#define ASYNC_LOCKFREE_LIST_H

#ifndef ASYNC_LIST_H
#include "../List.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class MWSRList - Multi-Writer-Single-Reader List

class MWSRList
{
public:
	typedef ListNode Node;

protected:
	Node* m_head;

public:
	MWSRList()
		: m_head(NULL)
	{
	}

	//
	// return the old value of list head
	//
	Node* cerl_call push(Node* val)
	{
		for (;;)
		{
			Node* head = m_head;
			CERL_ASSERT(val != head);
			val->prev = head;
			if (InterlockedCompareExchangePointer((PVOID*)&m_head, val, head) == head)
				return head;
		}
	}

	Node* cerl_call clear()
	{
		return (Node*)InterlockedExchangePointer((PVOID*)&m_head, NULL);
	}

	bool cerl_call empty() const
	{
		return m_head == NULL;
	}
};

typedef MWSRList SWSRList;

// =========================================================================

NS_CERL_END

#endif /* ASYNC_LOCKFREE_LIST_H */
