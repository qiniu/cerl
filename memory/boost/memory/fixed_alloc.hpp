//
//  boost/memory/fixed_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_FIXED_ALLOC_HPP
#define BOOST_MEMORY_FIXED_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#ifndef BOOST_MEMORY_CONTAINER_DCL_LIST_HPP
#include "container/dcl_list.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class fixed_alloc

#pragma pack(1)

#ifndef ROUND
#define ROUND(x, y)		(((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#define MIN(x, y)		((x) < (y) ? (x) : (y))
#endif

template <class PolicyT>
class fixed_alloc
{
private:
	fixed_alloc(const fixed_alloc&);
	void operator=(const fixed_alloc&);

	void swap(fixed_alloc& o); // NO SWAP: dcl_list

public:
	typedef typename PolicyT::alloc_type alloc_type;
	typedef size_t size_type;

	enum { MemBlockSize = PolicyT::MemBlockBytes - alloc_type::Padding };

protected:
	struct MemBlock;
	friend struct MemBlock;
	
	struct Header : public dcl_list_node<MemBlock>
	{
		size_t nUsed;
	};

	enum { HeaderSize = sizeof(Header) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock : Header
	{
		char buffer[BlockSize];
	};

	struct ChunkHeader
	{
		MemBlock* pBlock;
	};

	struct FreeChunk : public dcl_list_node<FreeChunk>
	{
	};

	enum { ChunkHeaderSize = sizeof(ChunkHeader) };
	enum { MinElemBytes = sizeof(FreeChunk) };

	dcl_list<FreeChunk> m_freelist;
	dcl_list<MemBlock> m_blks;
	size_type m_cbChunk;
	size_type m_nChunkPerBlock;
	MemBlock* m_lastBlock;
	alloc_type m_alloc;

private:
	void BOOST_MEMORY_CALL init_(size_type cbElem)
	{
		cbElem = ROUND(cbElem, sizeof(void*));
		m_cbChunk = MAX(cbElem, MinElemBytes) + ChunkHeaderSize;
		m_lastBlock = NULL;
		m_nChunkPerBlock = BlockSize/m_cbChunk;

		BOOST_MEMORY_ASSERT(m_nChunkPerBlock > 0);
	}
	
public:
	explicit fixed_alloc(size_type cbElem)
	{
		init_(cbElem);
	}

	fixed_alloc(alloc_type alloc, size_type cbElem) : m_alloc(alloc)
	{
		init_(cbElem);
	}

	~fixed_alloc()
	{
		clear();
	}

	size_type BOOST_MEMORY_CALL alloc_size() const
	{
		return m_cbChunk - ChunkHeaderSize;
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return m_alloc;
	}

	void BOOST_MEMORY_CALL clear()
	{
		MemBlock* nextBlk;
		for (MemBlock* blk = this->m_blks.first(); !this->m_blks.done(blk); blk = nextBlk)
		{
			nextBlk = blk->next();
			m_alloc.deallocate(blk);
		}
		this->m_blks.clear();
		this->m_freelist.clear();
	}

private:
	__forceinline MemBlock*& BOOST_MEMORY_CALL chunkHeader_(void* const p)
	{
		return ((ChunkHeader*)p - 1)->pBlock;
	}

	void BOOST_MEMORY_CALL do_allocate_block_()
	{
		MemBlock* const blk = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
		m_blks.push_front(blk);
		m_lastBlock = blk;

		blk->nUsed = 0;

		char* p = blk->buffer + ChunkHeaderSize;
		for (size_type i = m_nChunkPerBlock; i--; p += m_cbChunk)
		{
			chunkHeader_(p) = blk;
			m_freelist.push_front((FreeChunk*)p);
		}

		BOOST_MEMORY_ASSERT(!m_freelist.empty());
	}

	void BOOST_MEMORY_CALL do_deallocate_block_(MemBlock* const blk)
	{
		char* p = blk->buffer + ChunkHeaderSize;
		for (size_type i = m_nChunkPerBlock; i--; p += m_cbChunk)
		{
			((FreeChunk*)p)->erase();
		}

		blk->erase();

		m_alloc.deallocate(blk);
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate()
	{
		if (m_freelist.empty())
			do_allocate_block_();

		void* p = &m_freelist.front();
		++chunkHeader_(p)->nUsed;
		m_freelist.pop_front();
		
		BOOST_MEMORY_DBG_FILL(p, alloc_size());
		return p;
	}

	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		BOOST_MEMORY_ASSERT(cb <= alloc_size());
		return allocate();
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* const p)
	{
		MemBlock* const blk = chunkHeader_(p);

		BOOST_MEMORY_ASSERT(blk->nUsed > 0 && blk->nUsed <= m_nChunkPerBlock);
		BOOST_MEMORY_DBG_FILL(p, alloc_size());

		m_freelist.push_back((FreeChunk*)p); // NOTE: we don't use push_front! why?
		if (--blk->nUsed == 0 && blk != m_lastBlock)
			do_deallocate_block_(blk);
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* const p, size_t cb)
	{
		BOOST_MEMORY_ASSERT(cb == alloc_size());
		deallocate(p);
	}
};

#pragma pack()

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_FIXED_ALLOC_HPP */
