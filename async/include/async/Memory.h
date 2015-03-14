/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Memory.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Memory.h 2554 2010-04-20 01:40:11Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_MEMORY_H
#define ASYNC_MEMORY_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../../../stdext/include/stdext/Memory.h"
#endif

#if (0)
#define CERL_MEMORY_API
#endif

// -------------------------------------------------------------------------
// Memory Management API

#if defined(CERL_MEMORY_API)

STDAPI_(void*) cerl_allocate(size_t cb);
STDAPI_(void) cerl_deallocate(void* p);
STDAPI_(size_t) cerl_alloc_size(void* p);

#elif !defined(CERL_NO_MEMORY_API)

#define CERL_NO_MEMORY_API

#endif

#ifndef CERL_MEMORY_PREALLOC
#define CERL_MEMORY_PREALLOC 0
#endif

// -------------------------------------------------------------------------

NS_CERL_BEGIN

#if !defined(CERL_NO_MEMORY_API)

class SystemAlloc
{
public:
	enum { Padding = NS_BOOST_MEMORY::system_alloc::Padding };

	static void* cerl_call allocate(size_t cb) {
		return cerl_allocate(cb);
	}

	static void cerl_call deallocate(void* p) {
		cerl_deallocate(p);
	}

	static size_t cerl_call alloc_size(void* p) {
		return cerl_alloc_size(p);
	}

	static void cerl_call swap(SystemAlloc& o)	{
	}
};

class Sys
{
public:
	enum { MemBlockBytes = NS_BOOST_MEMORY_POLICY::sys::MemBlockBytes };

public:
	typedef SystemAlloc alloc_type;
	typedef SystemAlloc system_alloc_type;
};

typedef NS_BOOST_MEMORY::block_alloc<Sys> BlockPool;

class Scoped : public Sys
{
public:
	typedef NS_BOOST_MEMORY::proxy_alloc<BlockPool> alloc_type;
};

typedef NS_BOOST_MEMORY::region_alloc<Scoped, CERL_MEMORY_PREALLOC> ScopedAlloc;

typedef NS_BOOST_MEMORY::pools_alloc<Sys> Pools;
typedef NS_BOOST_MEMORY::pool_alloc<Sys> Pool;

typedef NS_BOOST_MEMORY::tls_pools_alloc<Sys> TlsPools;

typedef NS_STDEXT::BasicCString<char, TlsPools> CString;
typedef NS_STDEXT::BasicCString<wchar_t, TlsPools> CWString;

#else

typedef NS_BOOST_MEMORY::stdlib_alloc SystemAlloc;
typedef NS_BOOST_MEMORY::block_pool BlockPool;

typedef NS_BOOST_MEMORY_POLICY::sys Sys;
typedef NS_BOOST_MEMORY_POLICY::scoped Scoped;

typedef NS_BOOST_MEMORY::region_alloc<Scoped, CERL_MEMORY_PREALLOC> ScopedAlloc;

typedef NS_STDEXT::Pools Pools;
typedef NS_STDEXT::Pool Pool;

typedef NS_STDEXT::TlsPools TlsPools;

typedef NS_STDEXT::CString CString;
typedef NS_STDEXT::WCString WCString;

#endif // defined(CERL_NO_MEMORY_API)

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_MEMORY_H */
