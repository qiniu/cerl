//
//  boost/memory/system_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_SYSTEM_ALLOC_HPP
#define BOOST_MEMORY_SYSTEM_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#if !defined(_GLIBCXX_CSTDLIB) && !defined(_CSTDLIB)
#include <cstdlib> // malloc, free
#endif

NS_BOOST_MEMORY_BEGIN

// =========================================================================
// class stdlib_alloc

#pragma pack(1)

#if defined(__GNUG__)
#define _msize	malloc_usable_size
#endif

class stdlib_alloc
{
public:
	enum { Padding = 16 /* must >= sizeof(AllocateHeader) */ };
	
	typedef size_t size_type;

	static void* BOOST_MEMORY_CALL allocate(size_t cb) {
		return malloc(cb);
	}

	static void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize) {
		return realloc(p, newSize);
	}

	static void BOOST_MEMORY_CALL deallocate(void* p)			{ free(p); }
	static void BOOST_MEMORY_CALL deallocate(void* p, size_t)	{ free(p); }
	
	static void BOOST_MEMORY_CALL swap(stdlib_alloc& o)	{}

	static size_t BOOST_MEMORY_CALL alloc_size(void* p)
	{
		return _msize(p);
	}
};

#pragma pack()

// -------------------------------------------------------------------------
// class system_alloc

typedef stdlib_alloc system_alloc;

// =========================================================================
// Configurations

#ifndef NS_BOOST_MEMORY_POLICY_BEGIN
#define NS_BOOST_MEMORY_POLICY_BEGIN	namespace policy {
#define NS_BOOST_MEMORY_POLICY_END		}
#define NS_BOOST_MEMORY_POLICY			boost::memory::policy
#endif

NS_BOOST_MEMORY_POLICY_BEGIN

class sys
{
public:
	enum { MemBlockBytes = 65536 /* 64k */ };

public:
	typedef system_alloc alloc_type;
	typedef system_alloc system_alloc_type;
};

typedef sys stdlib;

NS_BOOST_MEMORY_POLICY_END

// =========================================================================

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_SYSTEM_ALLOC_HPP */
