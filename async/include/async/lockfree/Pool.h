/* -------------------------------------------------------------------------
// CERL: C++ Erlang Server Model
// 
// Module: async/lockfree/Pool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Pool.h 2804 2010-05-10 04:01:03Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_LOCKFREE_POOL_H
#define ASYNC_LOCKFREE_POOL_H

#ifndef ASYNC_LOCKFREE_LIST_H
#include "List.h"
#endif

#ifndef ASYNC_MEMORY_H
#include "../Memory.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class PoolMT

class PoolMT
{
private:
	ListNode* m_freelist;
	Pool* m_pool;
	MWSRList m_mwsr;
	
public:
	PoolMT(Pools& pools, size_t cbElem)
	{
		m_pool = &pools.get_pool(cbElem + sizeof(ListNode*));
		m_freelist = NULL;
	}
	
	void* cerl_call allocate()
	{
		if (m_freelist || (m_freelist = m_mwsr.clear()) != NULL)
		{
			ListNode* p = m_freelist;
			m_freelist = p->prev;
			return p + 1;
		}
		ListNode* p = (ListNode*)m_pool->allocate();
		return p + 1;
	}
	
	void cerl_call deallocate(void* p)
	{
		m_mwsr.push((ListNode*)p - 1);
	}
};

// =========================================================================

NS_CERL_END

#endif /* ASYNC_LOCKFREE_POOL_H */
