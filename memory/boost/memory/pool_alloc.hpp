//
//  boost/memory/pool_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_POOL_ALLOC_HPP
#define BOOST_MEMORY_POOL_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class pool_alloc

#pragma pack(1)

#ifndef ROUND
#define ROUND(x, y)		(((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#define MIN(x, y)		((x) < (y) ? (x) : (y))
#endif

template <class PolicyT>
class pool_alloc
{
private:
	pool_alloc(const pool_alloc&);
	void operator=(const pool_alloc&);

public:
	typedef typename PolicyT::alloc_type alloc_type;
	typedef size_t size_type;

	enum { MemBlockSize = PolicyT::MemBlockBytes - alloc_type::Padding };

protected:
	struct MemBlock;
	friend struct MemBlock;

	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock
	{
		MemBlock* pPrev;
		char buffer[BlockSize];
	};

	struct FreeChunk
	{
		FreeChunk* pPrev;
	};

	enum { MinElemBytes = sizeof(FreeChunk) };

	FreeChunk* m_freelist;
	MemBlock* m_blks;
	size_type m_cbChunk;
	size_type m_nChunkPerBlock;
	alloc_type m_alloc;
	MemoryState m_mstate;
	BOOST_MEMORY_THREAD_CHECKER;

private:
	void BOOST_MEMORY_CALL init_(size_type cbElem)
	{
		cbElem = ROUND(cbElem, sizeof(void*));
		m_cbChunk = MAX(cbElem, (size_type)MinElemBytes);
		m_nChunkPerBlock = BlockSize/m_cbChunk;
		m_freelist = NULL;
		m_blks = NULL;

		BOOST_MEMORY_ASSERT(m_nChunkPerBlock > 0);
	}
	
public:
	explicit pool_alloc(size_type cbElem)
	{
		init_(cbElem);
	}

	pool_alloc(alloc_type alloc, size_type cbElem) : m_alloc(alloc)
	{
		init_(cbElem);
	}

	~pool_alloc()
	{
		clear();
	}

	size_type BOOST_MEMORY_CALL alloc_size() const
	{
		return m_cbChunk;
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return m_alloc;
	}

	void BOOST_MEMORY_CALL clear()
	{
		BOOST_MEMORY_CHECK_THREAD();

		MemBlock* nextBlk;
		for (MemBlock* blk = m_blks; blk; blk = nextBlk)
		{
			nextBlk = blk->pPrev;
			m_alloc.deallocate(blk);
		}
		m_blks = NULL;
		m_freelist = NULL;
		m_mstate.clear();
	}

private:
	void* BOOST_MEMORY_CALL do_allocate_()
	{
		MemBlock* const blk = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
		blk->pPrev = m_blks;
		m_blks = blk;

		FreeChunk* p = (FreeChunk*)blk->buffer;
		p->pPrev = m_freelist;
		for (size_type i = m_nChunkPerBlock; --i;)
		{
			FreeChunk* const n = (FreeChunk*)((char*)p + m_cbChunk);
			n->pPrev = p;
			p = n;
		}
		m_freelist = p->pPrev;

		BOOST_MEMORY_DBG_FILL(p, alloc_size());
		m_mstate.allocate(p);
		return p;
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate()
	{
		BOOST_MEMORY_CHECK_THREAD();
		if (m_freelist)
		{
			FreeChunk* p = m_freelist;
			m_freelist = p->pPrev;
			BOOST_MEMORY_DBG_FILL(p, alloc_size());
			m_mstate.allocate(p);
			return p;
		}
		return do_allocate_();
	}

	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		BOOST_MEMORY_ASSERT(cb <= alloc_size());

		return allocate();
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* const p)
	{
		BOOST_MEMORY_DBG_FILL(p, alloc_size());
		BOOST_MEMORY_CHECK_THREAD();
		m_mstate.deallocate(p);

		((FreeChunk*)p)->pPrev = m_freelist;
		m_freelist = (FreeChunk*)p;
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* const p, size_t cb)
	{
		BOOST_MEMORY_ASSERT(cb <= alloc_size());
		BOOST_MEMORY_DBG_FILL(p, alloc_size());
		BOOST_MEMORY_CHECK_THREAD();
		m_mstate.deallocate(p);

		((FreeChunk*)p)->pPrev = m_freelist;
		m_freelist = (FreeChunk*)p;
	}
};

#pragma pack()

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_POOL_ALLOC_HPP */
