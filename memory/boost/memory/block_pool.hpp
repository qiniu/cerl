//
//  boost/memory/block_pool.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_BLOCKPOOL_HPP
#define BOOST_MEMORY_BLOCKPOOL_HPP

#ifndef BOOST_MEMORY_SYSTEM_ALLOC_HPP
#include "system_alloc.hpp"
#endif

#if !defined(_CLIMITS_) && !defined(_CLIMITS)
#include <climits> // INT_MAX
#endif

#if !defined(_ALGORITHM_) && !defined(_ALGORITHM)
#include <algorithm> // std::swap
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class proxy_alloc

#pragma pack(1)

template <class AllocT>
class proxy_alloc
{
private:
	AllocT* m_alloc;

public:
	proxy_alloc(AllocT& alloc) : m_alloc(&alloc) {}

public:
	typedef size_t size_type;
	
	enum { Padding = AllocT::Padding };

public:
	__forceinline void* BOOST_MEMORY_CALL allocate(size_type cb)	{ return m_alloc->allocate(cb); }
	__forceinline void BOOST_MEMORY_CALL deallocate(void* p) 		{ m_alloc->deallocate(p); }
	__forceinline void BOOST_MEMORY_CALL swap(proxy_alloc& o)		{ std::swap(m_alloc, o.m_alloc); }
	__forceinline size_type BOOST_MEMORY_CALL alloc_size(void* p) const { return m_alloc->alloc_size(p); }
	__forceinline AllocT& BOOST_MEMORY_CALL instance() const { return *m_alloc; }
	__forceinline AllocT* BOOST_MEMORY_CALL operator&() const { return m_alloc; }
	__forceinline operator AllocT&() const { return *m_alloc; }
};

#pragma pack()

// -------------------------------------------------------------------------
// class block_pool

#pragma pack(1)

template <class PolicyT>
class block_alloc
{
private:
	typedef typename PolicyT::alloc_type AllocT;
	enum { m_cbBlock = PolicyT::MemBlockBytes - AllocT::Padding };

	struct Block {
		Block* next;
	};

	Block* m_freeList;

	int m_nFree;
	const int m_nFreeLimit;
	size_t m_cbAllocSize;

private:
	block_alloc(const block_alloc&);
	void operator=(const block_alloc&);

public:
	block_alloc(int cbFreeLimit = INT_MAX)
		: m_nFree(1), m_nFreeLimit(cbFreeLimit / m_cbBlock)
	{
		m_freeList = (Block*)AllocT::allocate(m_cbBlock);
		m_freeList->next = NULL;
		m_cbAllocSize = AllocT::alloc_size(m_freeList);
	}
	~block_alloc()
	{
		clear();
	}

public:
	enum { Padding = AllocT::Padding };
	enum { BlockSize = m_cbBlock };

public:
	void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		BOOST_MEMORY_ASSERT(cb >= (size_t)m_cbBlock);

		if (cb > (size_t)m_cbBlock)
			return AllocT::allocate(cb);
		else
		{
			if (m_freeList)
			{
				BOOST_MEMORY_ASSERT(AllocT::alloc_size(m_freeList) >= cb);
				Block* blk = m_freeList;
				m_freeList = blk->next;
				--m_nFree;
				return blk;
			}
			return AllocT::allocate(m_cbBlock);
		}
	}

	void BOOST_MEMORY_CALL deallocate(void* p)
	{
		if (AllocT::alloc_size(p) > m_cbAllocSize || m_nFree >= m_nFreeLimit) {
			AllocT::deallocate(p);
		}
		else {
			Block* blk = (Block*)p;
			blk->next = m_freeList;
			m_freeList = blk;
			++m_nFree;
		}
	}

	static size_t BOOST_MEMORY_CALL alloc_size(void* p)
	{
		return AllocT::alloc_size(p);
	}

	void BOOST_MEMORY_CALL clear()
	{
		while (m_freeList)
		{
			Block* blk = m_freeList;
			m_freeList = blk->next;
			AllocT::deallocate(blk);
		}
		m_nFree = 0;
	}
};

typedef block_alloc<NS_BOOST_MEMORY_POLICY::sys> block_pool;

#pragma pack()

// -------------------------------------------------------------------------
// class pool

typedef proxy_alloc<block_pool> proxy_block_pool;

NS_BOOST_MEMORY_POLICY_BEGIN

class scoped : public sys
{
public:
	typedef proxy_block_pool alloc_type;
};

NS_BOOST_MEMORY_POLICY_END

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_BLOCKPOOL_HPP */
