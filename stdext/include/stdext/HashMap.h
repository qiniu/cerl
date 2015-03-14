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
// Module: stdext/HashMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: HashMap.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_HASHMAP_H
#define STDEXT_HASHMAP_H

#ifndef STDEXT_P_HASHMAP_H
#include "p/HashMap.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class HashMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = DefaultAlloc
	>
class HashMap : public PHashMap<
	KeyT, DataT, HashCompT, WINX_HASH_TYPENAME_ HashtableAllocTraits<AllocT>::alloc_type>
{
public:
	typedef KeyT key_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;

private:
	typedef HashtableAllocTraits<AllocT> Traits;
	typedef typename Traits::alloc_type BaseAllocT;
	typedef PHashMap<KeyT, DataT, HashCompT, BaseAllocT> Base;
	
public:
	typedef typename Base::size_type size_type;

	explicit HashMap(AllocT& alloc, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), n) {}

	template <class Iterator>
	HashMap(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), first, last, n) {}
};

// -------------------------------------------------------------------------
// class HashMultiMap

template <
	class KeyT, class DataT,
	class HashCompT = HashCompare<KeyT>,
	class AllocT = DefaultAlloc
	>
class HashMultiMap : public PHashMultiMap<
	KeyT, DataT, HashCompT, WINX_HASH_TYPENAME_ HashtableAllocTraits<AllocT>::alloc_type>
{
public:
	typedef KeyT key_type;
	typedef typename HashCompT::hasher hasher;
	typedef typename HashCompT::key_equal key_equal;
	typedef typename HashCompT::key_pred key_pred;

private:
	typedef HashtableAllocTraits<AllocT> Traits;
	typedef typename Traits::alloc_type BaseAllocT;
	typedef PHashMultiMap<KeyT, DataT, HashCompT, BaseAllocT> Base;

public:
	typedef typename Base::size_type size_type;

	explicit HashMultiMap(AllocT& alloc, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), n) {}
	
	template <class Iterator>
	HashMultiMap(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(Traits::getAlloc((Base*)0, alloc), first, last, n) {}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestHashMap

#if defined(STD_UNITTEST)

template <class LogT>
class TestHashMap : public TestCase
{
	WINX_TEST_SUITE(TestHashMap);
		WINX_TEST(testMap);
		WINX_TEST(testMultiMap);
	WINX_TEST_SUITE_END();

public:
	void testMap(LogT& log)
	{
		typedef NS_STDEXT::HashMap<int, int> MapType;

		NS_STDEXT::DefaultAlloc alloc;
		MapType simp(alloc);

		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(192, 4));
		simp.insert(MapType::value_type(194, 8));
		for (MapType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print((*it).first)
				.print(", ")
				.print((*it).second)
				.newline();
		}
	}

	void testMultiMap(LogT& log)
	{
		typedef NS_STDEXT::HashMultiMap<int, int> MapType;

		NS_STDEXT::DefaultAlloc alloc;
		MapType simp(alloc);

		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(1, 2));
		simp.insert(MapType::value_type(192, 4));
		simp.insert(MapType::value_type(194, 8));
		for (MapType::const_iterator it = simp.begin(); it != simp.end(); ++it)
		{
			log.print((*it).first)
				.print(", ")
				.print((*it).second)
				.newline();
		}
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_HASHMAP_H */
