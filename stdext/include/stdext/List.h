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
// Module: stdext/List.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: List.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_LIST_H
#define STDEXT_LIST_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

#ifndef STD_LIST_H
#include "../std/list.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class List

template <class ValT, class AllocT = DefaultAlloc>
class List : public std::list< ValT, StlAlloc<ValT, AllocT> >
{
private:
	typedef StlAlloc<ValT, AllocT> StlAllocT;
	typedef std::list<ValT, StlAllocT> Base;

	List(const List&);
	void operator=(const List&);

public:
    typedef typename Base::size_type size_type;
    
	explicit List(AllocT& alloc)
		: Base(alloc)
	{
	}

	List(AllocT& alloc, size_type count, const ValT& val = ValT())
		: Base(count, val, alloc)
	{
	}

	template <class Iterator>
	List(AllocT& alloc, Iterator aFirst, Iterator aLast)
		: Base(aFirst, aLast, alloc)
	{
	}

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class Slist

#ifndef _WINX_NO_SLIST

template <class ValT, class AllocT = DefaultAlloc>
class Slist : public stdext::slist< ValT, StlAlloc<ValT, AllocT> >
{
private:
	typedef StlAlloc<ValT, AllocT> StlAllocT;
	typedef stdext::slist<ValT, StlAllocT> Base;

	Slist(const Slist&);
	void operator=(const Slist&);

public:
    typedef typename Base::size_type size_type;
    
	explicit Slist(AllocT& alloc)
		: Base(alloc)
	{
	}

	Slist(AllocT& alloc, size_type count, const ValT& val = ValT())
		: Base(count, val, alloc)
	{
	}

	template <class Iterator>
	Slist(AllocT& alloc, Iterator aFirst, Iterator aLast)
		: Base(aFirst, aLast, alloc)
	{
	}

	void winx_call copy(const Base& from) {
		Base::operator=(from);
	}
};

#endif // _WINX_NO_SLIST

// -------------------------------------------------------------------------
// class TestList

#if defined(STD_UNITTEST)

template <class LogT>
class TestList : public TestCase
{
	WINX_TEST_SUITE(TestList);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	class Obj
	{
	private:
		int m_val;
	public:
		Obj(int arg = 0) {
			m_val = arg;
		}
	};

	void testBasic(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;
		NS_STDEXT::List<Obj> coll(alloc);
		coll.push_back(1);
		coll.push_back(2);
		coll.push_back(4);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// class TestSlist

#if defined(STD_UNITTEST)

#ifndef _WINX_NO_SLIST

template <class LogT>
class TestSlist : public TestCase
{
	WINX_TEST_SUITE(TestSlist);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;
		NS_STDEXT::Slist<int> coll(alloc);
		coll.push_front(1);
		coll.push_front(2);
		coll.push_front(4);
		for (NS_STDEXT::Slist<int>::const_iterator it = coll.begin(); it != coll.end(); ++it)
		{
			log.print(*it).newline();
		}
	}
};

#endif // _WINX_NO_SLIST

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// class TestDclList

#if defined(STD_UNITTEST)

template <class LogT>
class TestDclList : public TestCase
{
	WINX_TEST_SUITE(TestDclList);
		WINX_TEST(testBasic);
		WINX_TEST(testInsertFront);
	WINX_TEST_SUITE_END();

public:
	class Obj : public DclListNode<Obj>
	{
	public:
		int m_val;
	public:
		Obj(int arg)
		{
			m_val = arg;
		}
		Obj(int arg, DclList<Obj>& lst, bool fInsertBack)
			: DclListNode<Obj>(lst, Obj::insertAtBack)
		{
			m_val = arg;
		}
		Obj(int arg, DclList<Obj>& lst)
			: DclListNode<Obj>(lst, Obj::insertAtFront)
		{
			m_val = arg;
		}
	};

	template <class ListType>
	void print(LogT& log, ListType& coll)
	{
		for (typename ListType::const_iterator it = coll.begin(); it != coll.end(); ++it)
		{
			const Obj& val = *it;
			log.print(val.m_val).newline();
		}
	}

	template <class ListType>
	void print2(LogT& log, const ListType& coll)
	{
		for (const Obj* o = coll.first(); !coll.done(o); o = o->next())
		{
			log.print(o->m_val).newline();
		}
	}

	void testBasic(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		
		NS_STDEXT::DclList<Obj> coll;

		coll.push_back(STD_NEW(alloc, Obj)(1));
		STD_NEW(alloc, Obj)(2, coll, true);
		STD_NEW(alloc, Obj)(2, coll, true);
		coll.push_back(STD_NEW(alloc, Obj)(4));

		print(log, coll);
	}

	void testInsertFront(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);
		
		NS_STDEXT::DclList<Obj> coll;

		STD_NEW(alloc, Obj)(1, coll);
		coll.push_front(STD_NEW(alloc, Obj)(2));
		coll.push_front(STD_NEW(alloc, Obj)(2));
		STD_NEW(alloc, Obj)(4, coll);
		
		print2(log, coll);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_LIST_H */
