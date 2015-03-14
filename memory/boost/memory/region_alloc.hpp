//
//  boost/memory/region_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_REGION_ALLOC_HPP
#define BOOST_MEMORY_REGION_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class region_alloc

#ifndef BOOST_MEMORY_PREALLOC
#define BOOST_MEMORY_PREALLOC	0
#endif

#pragma pack(1)

template <class PolicyT, int m_fPreAlloc = BOOST_MEMORY_PREALLOC>
class region_alloc
{
private:
	typedef typename PolicyT::alloc_type AllocT;

	region_alloc(const region_alloc&);
	const region_alloc& operator=(const region_alloc&);

public:
	enum { MemBlockSize = PolicyT::MemBlockBytes - AllocT::Padding };

	typedef size_t size_type;
	typedef AllocT alloc_type;

private:
	struct MemBlock;
	friend struct MemBlock;

	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock
	{
		MemBlock* pPrev;
		char buffer[BlockSize];
	};
	struct DestroyNode
	{
		DestroyNode* pPrev;
		destructor_t fnDestroy;
	};
	
	char* m_begin;
	char* m_end;
	AllocT m_alloc;
	DestroyNode* m_destroyChain;
	BOOST_MEMORY_THREAD_CHECKER;

private:
	__forceinline MemBlock* BOOST_MEMORY_CALL chainHeader_() const
	{
		return (MemBlock*)(m_begin - HeaderSize);
	}

	void BOOST_MEMORY_CALL init_()
	{
		if (m_fPreAlloc)
		{
			MemBlock* pNew = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
			pNew->pPrev = NULL;
			m_begin = pNew->buffer;
			m_end = (char*)pNew + m_alloc.alloc_size(pNew);
		}
		else
		{
			m_begin = m_end = (char*)HeaderSize;
		}
	}

public:
	region_alloc() : m_destroyChain(NULL)
	{
		init_();
	}
	explicit region_alloc(alloc_type alloc) : m_alloc(alloc), m_destroyChain(NULL)
	{
		init_();
	}

	~region_alloc()
	{
		clear();
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return m_alloc;
	}

	void BOOST_MEMORY_CALL swap(region_alloc& o)
	{
		swap_object(this, &o);
	}

	void BOOST_MEMORY_CALL clear()
	{
		BOOST_MEMORY_CHECK_THREAD();
		while (m_destroyChain)
		{
			DestroyNode* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->fnDestroy(curr + 1);
		}
		MemBlock* pHeader = chainHeader_();
		while (pHeader)
		{
			MemBlock* curr = pHeader;
			pHeader = pHeader->pPrev;
			m_alloc.deallocate(curr);
		}
		m_begin = m_end = (char*)HeaderSize;
	}

	void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize)
	{
		BOOST_MEMORY_CHECK_THREAD();

		if (oldSize >= newSize)
			return p;
		void* p2 = allocate(newSize);
		memcpy(p2, p, oldSize);
		return p2;
	}

private:
	void* BOOST_MEMORY_CALL do_allocate_(size_t cb)
	{
		if (cb >= BlockSize)
		{
			MemBlock* pHeader = chainHeader_();
			MemBlock* pNew = (MemBlock*)m_alloc.allocate(HeaderSize + cb);
			if (pHeader)
			{
				pNew->pPrev = pHeader->pPrev;
				pHeader->pPrev = pNew;
			}
			else
			{
				m_end = m_begin = pNew->buffer;
				pNew->pPrev = NULL;
			}
			return pNew->buffer;
		}
		else
		{
			MemBlock* pNew = (MemBlock*)m_alloc.allocate(sizeof(MemBlock));
			pNew->pPrev = chainHeader_();
			m_begin = pNew->buffer;
			m_end = (char*)pNew + m_alloc.alloc_size(pNew);
			return m_end -= cb;
		}
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		BOOST_MEMORY_CHECK_THREAD();
		if ((size_t)(m_end - m_begin) >= cb)
		{
			return m_end -= cb;
		}
		return do_allocate_(cb);
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)allocate(sizeof(DestroyNode) + cb);
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
		return pNode + 1;
	}
#endif
	
	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)allocate(sizeof(DestroyNode) + cb);
		pNode->fnDestroy = fn;
		return pNode + 1;
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, destructor_t fn)
	{
		DestroyNode* pNode = (DestroyNode*)p - 1;
		BOOST_MEMORY_ASSERT(pNode->fnDestroy == fn);
		BOOST_MEMORY_CHECK_THREAD();

		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
	}

	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	__forceinline static void BOOST_MEMORY_CALL manage(void* p, int fnZero)
	{
		// no action
	}

	__forceinline static void BOOST_MEMORY_CALL deallocate(void* p)
	{
		// no action
	}

	__forceinline static void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		// no action
	}

	template <class Type>
	__forceinline static void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		// no action
	}

	template <class Type>
	__forceinline static void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		// no action
	}
};

#pragma pack()

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_REGION_ALLOC_HPP */
