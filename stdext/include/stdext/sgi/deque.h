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
// Module: sgi/deque.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: deque.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_DEQUE_H__
#define __SGI_DEQUE_H__

// -------------------------------------------------------------------------

#ifndef __SGI_CONFIG_H__
#include "config.h"
#endif

#ifndef __SGI_STRING_H__
#include "string.h"
#endif

#ifndef __SGI_STL_DEQUE_H
#include "../../../../stl/deque.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_CC_VC6)

namespace std {

template <class _Tp, class _Ref, class _Ptr> __forceinline
random_access_iterator_tag _Iter_cat(const stdext::_Deque_iterator<_Tp, _Ref, _Ptr>&)
	{ return random_access_iterator_tag(); }

template <class _Tp, class _Ref, class _Ptr> __forceinline
ptrdiff_t* _Dist_type(const stdext::_Deque_iterator<_Tp, _Ref, _Ptr>&)
	{return (ptrdiff_t*)0; }

} // namespace std

#endif

// -------------------------------------------------------------------------

#if defined(SGI_DEQUE_UNITTEST)

#ifndef STD_DEQUE_H
#include "../../std/deque.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef STDEXT_COUNTER_H
#include "../Counter.h"
#endif

template <class LogT>
class TestSgiDeque : public TestCase
{
	WINX_TEST_SUITE(TestSgiDeque);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();	

public:
	typedef std::StlAlloc<char, NS_STDEXT::ScopedAlloc> _DequeAlloc;
	typedef stdext::deque<char, _DequeAlloc> _SgiDeque;
	typedef std::_Deque<char, _DequeAlloc> _StdDeque;
	typedef std::_Deque<char> _StdDeque0;
	typedef std::vector<char> _StdVector;

	enum { N = 160000 };

	void doSgiDeque(LogT& log)
	{
		const char szHello[] = "Hello, world!";

		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);

		log.print("===== SgiDeque =====\n");
	
		_SgiDeque pool(alloc);
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int i = 0; i < N; ++i)
				pool.insert(pool.end(), szHello, szHello + countof(szHello));
		}
		counter.trace(log);
		log.print(ROUND(pool.size(), 512), "Capacity: %d\n");
	}
	
	void doStdDeque(LogT& log)
	{
		const char szHello[] = "Hello, world!";

		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);

		log.print("===== StdDeque =====\n");
	
		_StdDeque pool(alloc);
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int i = 0; i < N; ++i)
				pool.insert(pool.end(), szHello, szHello + countof(szHello));
		}
		counter.trace(log);
		log.print(ROUND(pool.size(), 4096), "Capacity: %d\n");
	}

	void doStdDeque0(LogT& log)
	{
		const char szHello[] = "Hello, world!";

		log.print("===== StdDeque0 =====\n");

		_StdDeque0 pool;
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int i = 0; i < N; ++i)
				pool.insert(pool.end(), szHello, szHello + countof(szHello));
		}
		counter.trace(log);
		log.print(ROUND(pool.size(), 4096), "Capacity: %d\n");
	}

	void doStdVector(LogT& log)
	{
		const char szHello[] = "Hello, world!";

		log.print("===== StdVector =====\n");
	
		_StdVector pool;
		NS_STDEXT::PerformanceCounter counter;
		{
			for (int i = 0; i < N; ++i)
				pool.insert(pool.end(), szHello, szHello + countof(szHello));
		}
		counter.trace(log);
		log.print(pool.capacity(), "Capacity: %d\n");
	}

	void test(LogT& log)
	{
		for (int i = 0; i < 4; ++i)
		{
			doSgiDeque(log);
			doStdDeque(log);
			doStdDeque0(log);
			doStdVector(log);
			log.newline();
		}
	}
};

#endif // defined(SGI_DEQUE_UNITTEST)

// -------------------------------------------------------------------------
// $Log: deque.h,v $

#endif /* __SGI_DEQUE_H__ */
