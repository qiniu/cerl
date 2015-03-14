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
// Module: stdext/Relation.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Relation.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_RELATION_H
#define STDEXT_RELATION_H

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

#ifndef STDEXT_TUPLE_H
#include "Tuple.h"
#endif

#ifndef STDEXT_DEQUE_H
#include "Deque.h"
#endif

#ifndef STDEXT_MAP_H
#include "Map.h"
#endif

#ifndef STDEXT_SET_H
#include "Set.h"
#endif

#ifndef STDEXT_HASHMAP_H
#include "HashMap.h"
#endif

#ifndef STDEXT_HASHSET_H
#include "HashSet.h"
#endif

#ifndef STDEXT_STATICALGO_H
#include "StaticAlgo.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// MapIndexing

template <int Field, class KeyT, class AllocT>
struct MapIndexing
{
	typedef MultiMap<KeyT, void*, std::less<KeyT>, AllocT> type;
	typedef Set<KeyT, std::less<KeyT>, AllocT> prime_type;
};

// -------------------------------------------------------------------------
// HashMapIndexing

template <int Field, class KeyT, class AllocT>
struct HashMapIndexing
{
	typedef HashMultiMap<KeyT, void*, HashCompare<KeyT>, AllocT> type;
	typedef HashSet<KeyT, HashCompare<KeyT>, AllocT> prime_type;
};

// -------------------------------------------------------------------------
// IndexingAct_

template <
	int IndexedFieldMasks,
	class TupleT,
	template <int Field, class KeyT, class AllocT> class IndexingT,
	class AllocT
	>
struct IndexingAct_
{
	enum { CurrFieldIndex = StaticLog2<MasksTraits<IndexedFieldMasks>::Head>::value };
	
	typedef typename TupleItemTraits<CurrFieldIndex, TupleT>::value_type CurrKeyT;
	typedef typename IndexingT<CurrFieldIndex, CurrKeyT, AllocT>::type CurrIndexingT;
	typedef IndexingAct_<MasksTraits<IndexedFieldMasks>::Tail, TupleT, IndexingT, AllocT> NextAct;
	
	template <int IndexExcluded>
	static void winx_call eraseIndexing(void* const dest[], const TupleT& t)
	{
		WINX_ASSERT(dest[0]);
		if (CurrFieldIndex != IndexExcluded)
		{
			typedef typename CurrIndexingT::iterator It;
			typedef std::pair<It, It> RangeT;

			It it;
			CurrIndexingT& idxDest = *(CurrIndexingT*)dest[0];
			const CurrKeyT& key = TupleItemTraits<CurrFieldIndex, TupleT>::get(t);
			const RangeT rg = idxDest.equal_range(key);
			for (it = rg.first; ; ++it)
			{
				WINX_ASSERT(it != rg.second);
				if ((*it).second == (void*)&t)
				{
					idxDest.erase(it);
					break;
				}
			}
		}
		NextAct::template eraseIndexing<IndexExcluded>(dest+1, t);
	}

	static void winx_call makeIndexing(void* const dest[], const TupleT& t)
	{
		WINX_ASSERT(dest[0]);
		{
			CurrIndexingT& idxDest = *(CurrIndexingT*)dest[0];
			const CurrKeyT& key = TupleItemTraits<CurrFieldIndex, TupleT>::get(t);
			idxDest.insert(
				typename CurrIndexingT::value_type(key, (void*)&t)
				);
		}
		NextAct::makeIndexing(dest+1, t);
	}

	static void winx_call copy(void* const dest[], void* const src[])
	{
		WINX_ASSERT(dest[0] && src[0]);
		{
			CurrIndexingT& idxDest = *(CurrIndexingT*)dest[0];
			const CurrIndexingT& idxSrc = *(const CurrIndexingT*)src[0];
			idxDest.copy(idxSrc);
		}
		NextAct::copy(dest+1, src+1);
	}

	static void winx_call clear(void* const dest[])
	{
		WINX_ASSERT(dest[0]);
		{
			CurrIndexingT& idxDest = *(CurrIndexingT*)dest[0];
			idxDest.clear();
		}
		NextAct::clear(dest+1);
	}
	
	static void winx_call init(AllocT& alloc, void* dest[])
	{
		if (DestructorTraits<CurrKeyT>::HasDestructor)
			dest[0] = STD_NEW(alloc, CurrIndexingT)(alloc);
		else
			dest[0] = STD_UNMANAGED_NEW(alloc, CurrIndexingT)(alloc);
		NextAct::init(alloc, dest+1);
	}
};

template <
	class TupleT,
	template <int Field, class KeyT, class AllocT> class IndexingT,
	class AllocT
	>
struct IndexingAct_<0, TupleT, IndexingT, AllocT>
{
	template <int IndexExcluded>
	static void winx_call eraseIndexing(void* const dest[], const TupleT& t) {
		// noting to do
	}

	static void winx_call makeIndexing(void* const dest[], const TupleT& t) {
		// noting to do
	}
	
	static void winx_call copy(void* const dest[], void* const src[]) {
		// noting to do
	}

	static void winx_call clear(void* const dest[]) {
		// noting to do
	}
	
	static void winx_call init(AllocT& alloc, void* dest[]) {
		// noting to do
	}
};

// -------------------------------------------------------------------------
// PrimaryKeyUnionFieldsAct_

template <
	int PrimaryKeyUnionFieldMasks,
	class TupleT,
	template <int Field, class KeyT, class AllocT> class IndexingT,
	class AllocT
	>
class PrimaryKeyUnionFieldsAct_
{
private:
	typedef TupleSelectKey<TupleT, PrimaryKeyUnionFieldMasks> PrimaryKeyT;
	typedef IndexingT<0, PrimaryKeyT, AllocT> PrimaryIndexingT;
	typedef typename PrimaryIndexingT::prime_type PrimarySetT;
	
public:
	typedef PrimarySetT primary_set;

	template <class RelationT>
	static const TupleT* winx_call insert(RelationT* rel, const TupleT& val)
	{
		typedef typename RelationT::IndexingActT IndexingActT;
		const std::pair<typename primary_set::iterator, bool> ret = rel->m_data.insert(val);
		if (ret.second)
		{
			const TupleT& t = *ret.first;
			IndexingActT::makeIndexing(rel->m_indexs, t);
			return &t;
		}
		return NULL;
	}
};

template <
	class TupleT,
	template <int Field, class KeyT, class AllocT> class IndexingT,
	class AllocT
	>
class PrimaryKeyUnionFieldsAct_<0, TupleT, IndexingT, AllocT>
{
private:
	class PrimarySetT
	{
	public:
		typedef bool iterator;
		typedef bool const_iterator;

		PrimarySetT(AllocT&) {}
		
		size_t winx_call size() const;

		void winx_call copy(const PrimarySetT&) {}
		void winx_call erase(const TupleT& t) {}
		void winx_call clear() {}
	};
	
public:
	typedef PrimarySetT primary_set;

	template <class RelationT>
	static const TupleT* winx_call insert(RelationT* rel, const TupleT& val)
	{
		typedef typename RelationT::IndexingActT IndexingActT;

		enum { HasDestructor = TupleTraits<TupleT>::HasDestructor };
	
		AllocT& alloc = rel->get_alloc();
		
		const TupleT* t = HasDestructor ?
			STD_NEW(alloc, TupleT)(val) :
			STD_UNMANAGED_NEW(alloc, TupleT)(val);
		
		IndexingActT::makeIndexing(rel->m_indexs, *t);
		
		return t;
	}
};

// -------------------------------------------------------------------------
// class Relation

template <
	class TupleT,
	int IndexedFieldMasks,
	int PrimaryKeyUnionFieldMasks = 0,
	template <int Field, class KeyT, class AllocT> class IndexingT = HashMapIndexing,
	class AllocT = DefaultAlloc>
class Relation
{
public:
	enum { Fields = TupleTraits<TupleT>::Fields };
	enum { IndexedFields = MasksTraits<IndexedFieldMasks>::Count };

	template <int Field>
	class Indexing
	{
	public:
		typedef typename TupleItemTraits<Field, TupleT>::value_type key_type;
		typedef typename IndexingT<Field, key_type, AllocT>::type type;

		typedef typename type::iterator iterator;
		typedef typename type::const_iterator const_iterator;

		typedef std::pair<iterator, iterator> range;
		typedef std::pair<const_iterator, const_iterator> const_range;

	public:
		static const TupleT& winx_call item(const const_iterator& it) {
			return *(const TupleT*)(*it).second;
		}
		
		static TupleT& winx_call item(const iterator& it) {
			return *(TupleT*)(*it).second;
		}
	};

private:
	Relation(const Relation&);
	void operator=(const Relation&);

	typedef IndexingAct_<IndexedFieldMasks, TupleT, IndexingT, AllocT> IndexingActT;
	typedef PrimaryKeyUnionFieldsAct_<PrimaryKeyUnionFieldMasks, TupleT, IndexingT, AllocT> PrimaryKeyActT;
	typedef typename PrimaryKeyActT::primary_set PrimarySetT;
	
	friend class PrimaryKeyUnionFieldsAct_<PrimaryKeyUnionFieldMasks, TupleT, IndexingT, AllocT>;
	
private:
	void* m_indexs[IndexedFields];
	AllocT& m_alloc;
	PrimarySetT m_data;
	
public:
	typedef size_t size_type;
	typedef AllocT alloc_type;

	typedef PrimarySetT primary_set;
	typedef typename PrimarySetT::iterator primary_set_iterator;
	
	explicit Relation(AllocT& alloc)
		: m_alloc(alloc), m_data(alloc)
	{
		IndexingActT::init(alloc, m_indexs);
	}

public:
	alloc_type& winx_call get_alloc() const {
		return m_alloc;
	}
	
	const primary_set& winx_call data() const {
		return m_data;
	}

	size_type winx_call size() const
	{
		if (PrimaryKeyUnionFieldMasks)
			return m_data.size();
		else {
			typedef Indexing<0> Indexing0;
			return ((const typename Indexing0::type*)m_indexs[0])->size();
		}
	}
	
	void winx_call clear() {
		if (size()) {
			IndexingActT::clear(m_indexs);
			m_data.clear();
		}
	}

	void winx_call copy(const Relation& from) {
		IndexingActT::copy(m_indexs, from.m_indexs);
		m_data.copy(from.m_data);
	}
	
	void winx_call swap(Relation& o) {
		swap_object(this, &o);
	}
	
	template <int Field>
	void winx_call erase(
		typename Indexing<Field>::iterator const it)
	{
		enum { FieldIdx = MasksIndexOfBit<Field, IndexedFieldMasks>::value };

		typedef Indexing<Field> IndexingN;
		const TupleT& t = IndexingN::item(it);
		IndexingActT::template eraseIndexing<Field>(m_indexs, t);
		m_data.erase(t);
		((typename IndexingN::type*)m_indexs[FieldIdx])->erase(it);
	}

	template <int Field>
	size_type winx_call erase(
		typename Indexing<Field>::iterator const first,
		typename Indexing<Field>::iterator const last)
	{
		enum { FieldIdx = MasksIndexOfBit<Field, IndexedFieldMasks>::value };

		size_type n = 0;
		typedef Indexing<Field> IndexingN;
		for (typename Indexing<Field>::iterator it = first; it != last; ++it) {
			const TupleT& t = IndexingN::item(it);
			IndexingActT::template eraseIndexing<Field>(m_indexs, t);
			m_data.erase(t);
			++n;
		}
		((typename IndexingN::type*)m_indexs[FieldIdx])->erase(first, last);
		return n;
	}

	template <int Field>
	size_type winx_call erase(const typename Indexing<Field>::key_type& key)
	{
		enum { FieldIdx = MasksIndexOfBit<Field, IndexedFieldMasks>::value };
		WINX_ASSERT(m_indexs[FieldIdx] != NULL);

		typedef typename Indexing<Field>::type MapT;
		typedef typename Indexing<Field>::range RangeT;

		const RangeT rg = ((MapT*)m_indexs[FieldIdx])->equal_range(key);
		return erase<Field>(rg.first, rg.second);
	}

	template <int Field>
	size_type winx_call count(const typename Indexing<Field>::key_type& key) const
	{
		enum { FieldIdx = MasksIndexOfBit<Field, IndexedFieldMasks>::value };
		WINX_ASSERT(m_indexs[FieldIdx] != NULL);

		typedef typename Indexing<Field>::type MapT;
		return ((const MapT*)m_indexs[FieldIdx])->count(key);
	}
	
	template <int Field>
	typename Indexing<Field>::const_range winx_call
		select(const typename Indexing<Field>::key_type& key) const
	{
		enum { FieldIdx = MasksIndexOfBit<Field, IndexedFieldMasks>::value };
		WINX_ASSERT(m_indexs[FieldIdx] != NULL);

		typedef typename Indexing<Field>::type MapT;
		return ((const MapT*)m_indexs[FieldIdx])->equal_range(key);
	}

	template <int Field>
	typename Indexing<Field>::range winx_call
		select(const typename Indexing<Field>::key_type& key)
	{
		enum { FieldIdx = MasksIndexOfBit<Field, IndexedFieldMasks>::value };
		WINX_ASSERT(m_indexs[FieldIdx] != NULL);

		typedef typename Indexing<Field>::type MapT;
		return ((MapT*)m_indexs[FieldIdx])->equal_range(key);
	}

	const TupleT* winx_call insert(const TupleT& val)
	{
		return PrimaryKeyActT::insert(this, val);
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestRelation

#if defined(STD_UNITTEST)

template <class LogT>
class TestRelation : public TestCase
{
	WINX_TEST_SUITE(TestRelation);
		WINX_TEST(testBasic);
	WINX_TEST_SUITE_END();

public:
	void testBasic(LogT& log)
	{
		typedef NS_STDEXT::AutoAlloc AllocT;
		typedef std::pair<std::string, int> TupleT;
		typedef NS_STDEXT::Relation<TupleT, 3, 0, NS_STDEXT::HashMapIndexing, AllocT> RelationT;
		typedef RelationT::Indexing<0> Indexing0;
		typedef RelationT::Indexing<1> Indexing1;
		
		AllocT alloc;
		RelationT rel(alloc);
		
		rel.insert(TupleT("Mon", 1));
		rel.insert(TupleT("Monday", 1));
		rel.insert(TupleT("Tue", 2));
		rel.insert(TupleT("Wed", 3));
		rel.insert(TupleT("Wednesday", 3));
		AssertExp(rel.size() == 5);
		
		Indexing1::range rg = rel.select<1>(3);
		AssertExp(std::distance(rg.first, rg.second) == 2);
		for (Indexing1::iterator it = rg.first; it != rg.second; ++it) {
			const TupleT& i = Indexing1::item(it);
			log.printString(i.first).newline();
			AssertExp(i.first == "Wed" || i.first == "Wednesday");
			AssertExp(i.second == 3);
		}

		Indexing0::range rg2 = rel.select<0>("Mon");
		AssertExp(std::distance(rg2.first, rg2.second) == 1);
		Indexing0::iterator it2 = rg2.first;
		const TupleT& i2 = Indexing0::item(it2);
		AssertExp(i2.first == "Mon" && i2.second == 1);

		AssertExp(rel.count<1>(1) == 2);
		AssertExp(rel.erase<1>(1) == 2);
		AssertExp(rel.count<1>(1) == 0);
		AssertExp(rel.size() == 3);
		
		Indexing0::range rg3 = rel.select<0>("Mon");
		AssertExp(std::distance(rg3.first, rg3.second) == 0);
		AssertExp(rel.count<0>("Monday") == 0);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_RELATION_H */

