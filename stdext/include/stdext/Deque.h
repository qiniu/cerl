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
// Module: stdext/Deque.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-9-6 10:08:57
// 
// $Id: Deque.h 2009-9-6 10:08:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DEQUE_H
#define STDEXT_DEQUE_H

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

#if defined(STD_UNITTEST)
	#ifndef STDEXT_COUNTER_H
	#include "Counter.h"
	#endif
#endif

// -------------------------------------------------------------------------
// class StlDeque

#if defined(X_CC_VC6)

#ifndef _DEQUE_
#include <deque>
#endif

#ifndef AllocTLGORITHM_
#include <algorithm>
#endif

#define WINX_DEQUE_USING_BASE_												\
public:																		\
	using Base::begin;														\
	using Base::end;														\
	using Base::assign;														\
	using Base::insert;														\
	using Base::resize

template<class DataT, class AllocT = allocator<DataT> >
class StlDeque : public std::deque<DataT, AllocT>
{
private:
	typedef std::deque<DataT, AllocT> Base;
	WINX_DEQUE_USING_BASE_;

public:
	typedef typename Base::iterator iterator;

	explicit StlDeque(const AllocT& alloc = AllocT())
		: Base(alloc) {}
	
	explicit StlDeque(size_type count, const DataT& val = DataT(), const AllocT& alloc = AllocT())
		: Base(count, val, alloc) {}

	template <class Iterator>
	StlDeque(Iterator first, Iterator last, const AllocT& alloc = AllocT())
		: Base(alloc)
	{
		std::copy(first, last, std::back_insert_iterator<Base>(*this));
	}

public:
	template <class Iterator>
	void assign(Iterator first, Iterator last)
	{
		Base::resize(std::distance(first, last));
		std::copy(first, last, begin());
	}

	template <class Iterator>
	void insert(iterator it, Iterator first, Iterator last)
	{
		std::copy(first, last, std::insert_iterator<Base>(*this, it));
	}
};

#else

#if !defined(_DEQUE_) && !defined(_GLIBCXX_DEQUE) && !defined(_DEQUE)
#include <deque>
#endif

#define StlDeque	std::deque

#endif

// -------------------------------------------------------------------------
// class Deque

NS_STDEXT_BEGIN

template <class DataT, class AllocT = DefaultAlloc>
class Deque : public StlDeque<DataT, StlAlloc<DataT, AllocT> >
{
private:
	typedef StlDeque<DataT, StlAlloc<DataT, AllocT> > Base;

	Deque(const Deque&);
	void operator=(const Deque&);

public:
	typedef typename Base::size_type size_type;

	explicit Deque(AllocT& a)
		: Base(a) {}
	
	Deque(AllocT& a, size_type n, const DataT& v = DataT())
		: Base(n, v, a) {}

	template <class Iterator>
	Deque(AllocT& a, Iterator first, Iterator last)
		: Base(first, last, a) {}

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestDeque

#if defined(STD_UNITTEST)

template <class LogT>
class TestDeque : public TestCase
{
	WINX_TEST_SUITE(TestDeque);
		WINX_TEST(testDeque);
		WINX_TEST(testCompare);
	WINX_TEST_SUITE_END();

public:
	void testDeque(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;
		NS_STDEXT::Deque<int> coll(alloc);
		coll.push_back(1);
		coll.push_back(2);
		coll.push_back(3);
		AssertExp(coll.size() == 3);
	}

public:
	enum { N = 40000 };

	void doStlDeque(LogT& log)
	{
		typedef std::deque<int> DequeT;
		log.print("===== std::deque =====\n");
		NS_STDEXT::PerformanceCounter counter;
		{
			DequeT coll;
			for (int i = 0; i < N; ++i)
				coll.push_back(i);
		}
		counter.trace(log);
	}

	void doDeque(LogT& log)
	{
		typedef NS_STDEXT::Deque<int, NS_STDEXT::ScopedAlloc> DequeT;
		log.print("===== NS_STDEXT::Deque (ScopedAlloc) =====\n");
		NS_STDEXT::PerformanceCounter counter;
		{
			NS_STDEXT::BlockPool recycle;
			NS_STDEXT::ScopedAlloc alloc(recycle);
			DequeT coll(alloc);
			for (int i = 0; i < N; ++i)
				coll.push_back(i);
		}
		counter.trace(log);
	}

	void doShareAllocDeque(LogT& log)
	{
		typedef NS_STDEXT::Deque<int, NS_STDEXT::ScopedAlloc> DequeT;
		NS_STDEXT::BlockPool recycle;
		log.newline();
		for (int i = 0; i < 5; ++i)
		{
			log.print("===== doShareAllocDeque =====\n");
			NS_STDEXT::PerformanceCounter counter;
			{
				NS_STDEXT::ScopedAlloc alloc(recycle);
				DequeT coll(alloc);
				for (int i = 0; i < N; ++i)
					coll.push_back(i);
			}
			counter.trace(log);
		}
	}

	void testCompare(LogT& log)
	{
		for (int i = 0; i < 5; ++i)
		{
			log.newline();
			doStlDeque(log);
			doDeque(log);
		}
		doShareAllocDeque(log);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_DEQUE_H */

