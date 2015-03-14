/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/Memory.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 15:43:17
// 
// $Id: Memory.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MEMORY_H
#define STDEXT_MEMORY_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_BOOST_MEMORY_H
#include "boost/Memory.h"
#endif

#ifndef STDEXT_MEMORY_TLSPOOLS_H
#include "memory/TlsPools.h"
#endif

// -------------------------------------------------------------------------

NS_STDEXT_BEGIN

typedef system_alloc SystemAlloc;
typedef stdlib_alloc DefaultStaticAlloc;
typedef default_alloc DefaultAlloc;

typedef block_pool BlockPool;

typedef auto_alloc AutoAlloc;
typedef scoped_alloc ScopedAlloc;
typedef scoped_pool ScopedPool;

typedef stdlib Stdlib;
typedef scoped Scoped;

typedef pool Pool;
typedef pools Pools;
typedef tls_pools TlsPools;

#define RegionAllocT region_alloc
#define ConstructorTraits constructor_traits
#define DestructorTraits destructor_traits
#define StlAlloc stl_allocator
#define Defragment defragment

#define DclListNode dcl_list_node
#define DclList dcl_list

NS_STDEXT_END

#define STD_NO_CONSTRUCTOR(Type)				BOOST_MEMORY_NO_CONSTRUCTOR(Type)
#define STD_NO_DESTRUCTOR(Type)					BOOST_MEMORY_NO_DESTRUCTOR(Type)

#define STD_NEW(alloc, Type)					BOOST_MEMORY_NEW(alloc, Type)
#define STD_NEW_ARRAY(alloc, Type, count)		BOOST_MEMORY_NEW_ARRAY(alloc, Type, count)

#define STD_UNMANAGED_NEW(alloc, Type)			new((alloc).allocate(sizeof(Type))) Type

#define STD_ALLOC(alloc, Type)					BOOST_MEMORY_ALLOC(alloc, Type)
#define STD_ALLOC_ARRAY(alloc, Type, count)		BOOST_MEMORY_ALLOC_ARRAY(alloc, Type, count)

// -------------------------------------------------------------------------
// --> Memory leak checker - count-checker

#ifndef STDEXT_WINAPI_WINBASE_H
#include "winapi/winbase.h"
#endif

NS_STDEXT_BEGIN

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

inline void reportCountLeak_(unsigned nRef, const char* szClass, const char* szFile, int nLine)
{
	char szBuf[1024];
	size_t cch = 0;

	sprintf(szBuf, "%s(%d):", szFile, nLine);
	for (cch = strlen(szBuf); cch < 70; ++cch)
	{
		szBuf[cch] = ' ';
	}
	szBuf[cch] = '\0';
	OutputDebugStringA(szBuf);

	sprintf(szBuf, "%s (%d) is leaked\n", szClass, nRef);
	OutputDebugStringA(szBuf);
}

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

class CountChecker_
{
public:
	CountChecker_(const char* szClass, const char* szFile, int nLine)
		: m_nRef(0), m_szClass(szClass), m_szFile(szFile), m_nLine(nLine) {}
	~CountChecker_()
		{ if (m_nRef > 0)
			reportCountLeak_(m_nRef, m_szClass, m_szFile, m_nLine); }
	void __stdcall operator++()	{ ++m_nRef; }
	void __stdcall operator--()	{ --m_nRef; }
 
private:
	unsigned m_nRef;
	const char* m_szClass;
	const char* m_szFile;
	int m_nLine;
};

#if !defined(_DEBUG)
#	define WINX_DECLARE_COUNT(Class)
#else
#	define WINX_DECLARE_COUNT(Class)										\
	class XCountChecker_													\
	{																		\
	public:																	\
		XCountChecker_()							{ ++counter(); }		\
		XCountChecker_(const XCountChecker_& rhs)	{ ++counter(); }		\
		~XCountChecker_()							{ --counter(); }		\
	private:																\
		CountChecker_& __stdcall counter()									\
			{ static std::CountChecker_ r(#Class, __FILE__, __LINE__);		\
			  return r; }													\
	} winx_cntchecker_;
#endif

NS_STDEXT_END

// -------------------------------------------------------------------------
// backward compatibility

NS_STDEXT_BEGIN

typedef auto_alloc AutoFreeAlloc; // for backward compatibility
typedef scoped_alloc ScopeAlloc; // for backward compatibility

#define FixedAllocT fixed_alloc
#define ObjectPool object_pool
#define ObjectPoolTraits object_pool_traits
#define ScopedObjectPool scoped_object_pool

NS_STDEXT_END

// -------------------------------------------------------------------------

#endif /* STDEXT_MEMORY_H */
