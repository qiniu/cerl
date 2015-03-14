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
// Module: stdext/HashSet.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: HashSet.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_HASHSET_H
#define STDEXT_HASHSET_H

#ifndef STDEXT_P_HASHSET_H
#include "p/HashSet.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class HashSet

template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashSet : public PHashSet<
	ValT, HashCompT, WINX_HASH_TYPENAME_ HashtableAllocTraits<AllocT>::alloc_type>
{
public:
	typedef ValT key_type;
	typedef ValT value_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;

private:
	typedef HashtableAllocTraits<AllocT> Traits;
	typedef typename Traits::alloc_type BaseAllocT;
	typedef PHashSet<ValT, HashCompT, BaseAllocT> Base;

public:
	typedef typename Base::size_type size_type;

	explicit HashSet(AllocT& alloc, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), n) {}
	
	template <class Iterator>
	HashSet(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), first, last, n) {}
};

// -------------------------------------------------------------------------
// class HashMultiSet

template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashMultiSet : public PHashMultiSet<
	ValT, HashCompT, WINX_HASH_TYPENAME_ HashtableAllocTraits<AllocT>::alloc_type>
{
public:
	typedef ValT key_type;
	typedef ValT value_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;
	
private:
	typedef HashtableAllocTraits<AllocT> Traits;
	typedef typename Traits::alloc_type BaseAllocT;
	typedef PHashMultiSet<ValT, HashCompT, BaseAllocT> Base;
	
public:
	typedef typename Base::size_type size_type;
	
	explicit HashMultiSet(AllocT& alloc, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), n) {}
	
	template <class Iterator>
	HashMultiSet(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), first, last, n) {}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestHashSet

#if defined(STD_UNITTEST)

template <class LogT>
class TestHashSet : public TestCase
{
	WINX_TEST_SUITE(TestHashSet);
		WINX_TEST(testSet);
		WINX_TEST(testMultiSet);
	WINX_TEST_SUITE_END();

public:
	void testSet(LogT& log)
	{
		typedef NS_STDEXT::HashSet<int> SetType;

		NS_STDEXT::DefaultAlloc alloc;
		SetType simp(alloc);

		simp.insert(1);
		simp.insert(1);
		simp.insert(192);
		simp.insert(194);
		for (SetType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print(*it).newline();
		}
	}

	void testMultiSet(LogT& log)
	{
		typedef NS_STDEXT::HashMultiSet<int> SetType;

		NS_STDEXT::DefaultAlloc alloc;
		SetType simp(alloc);

		simp.insert(1);
		simp.insert(1);
		simp.insert(192);
		simp.insert(194);
		for (SetType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print(*it).newline();
		}
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_HASHSET_H */
