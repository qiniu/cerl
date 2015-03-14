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
// Module: stdext/p/Hashtable.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-9-6 10:08:57
// 
// $Id: Hashtable.h 2009-9-6 10:08:57 xushiwei Exp $
// -----------------------------------------------------------------------*/

/*
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef STDEXT_P_HASHTABLE_H
#define STDEXT_P_HASHTABLE_H

#ifndef STDEXT_HASH_H
#include "../Hash.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#if defined(_MSC_VER) && (_MSC_VER == 1200)
#define WINX_PHASH_TYPENAME_
#else
#define WINX_PHASH_TYPENAME_	typename
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class HashtableNode_

template <class ValueT>
struct HashtableNode_
{
	HashtableNode_* m_next;
	ValueT m_val;
};

// -------------------------------------------------------------------------
// class PHashtable::iterator, const_iterator

template <class ValueT, class KeyT, class HashT,
          class ExtractKeyT, class EqualKeyT, class PoolT>
class PHashtable;

template <class ValueT, class KeyT, class HashT,
          class ExtractKeyT, class EqualKeyT, class PoolT>
struct HashtableIter_;

template <class ValueT, class KeyT, class HashT,
          class ExtractKeyT, class EqualKeyT, class PoolT>
struct HashtableCIter_;

template <class ValueT, class KeyT, class HashT,
          class ExtractKeyT, class EqualKeyT, class PoolT>
struct HashtableIter_
{
  typedef PHashtable<ValueT, KeyT, HashT, ExtractKeyT, EqualKeyT, PoolT> HashtableT;
  typedef HashtableIter_<ValueT, KeyT, HashT, ExtractKeyT, EqualKeyT, PoolT> iterator;
  typedef HashtableCIter_<ValueT, KeyT, HashT, ExtractKeyT, EqualKeyT, PoolT> const_iterator;
  typedef HashtableNode_<ValueT> NodeT;

  typedef forward_iterator_tag iterator_category;
  typedef ValueT value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef ValueT& reference;
  typedef ValueT* pointer;

  NodeT* m_cur;
  HashtableT* m_ht;

  HashtableIter_(NodeT* n, HashtableT* tab) 
    : m_cur(n), m_ht(tab) {}
  HashtableIter_() {}
  reference operator*() const { return m_cur->m_val; }
  pointer operator->() const { return &(operator*()); }
  iterator& operator++();
  iterator operator++(int);
  bool operator==(const iterator& it) const
    { return m_cur == it.m_cur; }
  bool operator!=(const iterator& it) const
    { return m_cur != it.m_cur; }
};


template <class ValueT, class KeyT, class HashT,
          class ExtractKeyT, class EqualKeyT, class PoolT>
struct HashtableCIter_
{
  typedef PHashtable<ValueT, KeyT, HashT, ExtractKeyT, EqualKeyT, PoolT> HashtableT;
  typedef HashtableIter_<ValueT, KeyT, HashT, ExtractKeyT, EqualKeyT,PoolT> iterator;
  typedef HashtableCIter_<ValueT, KeyT, HashT, ExtractKeyT, EqualKeyT, PoolT> const_iterator;
  typedef HashtableNode_<ValueT> NodeT;

  typedef forward_iterator_tag iterator_category;
  typedef ValueT value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef const ValueT& reference;
  typedef const ValueT* pointer;

  const NodeT* m_cur;
  const HashtableT* m_ht;

  HashtableCIter_(const NodeT* n, const HashtableT* tab)
    : m_cur(n), m_ht(tab) {}
  HashtableCIter_() {}
  HashtableCIter_(const iterator& it) 
    : m_cur(it.m_cur), m_ht(it.m_ht) {}
  reference operator*() const { return m_cur->m_val; }
  pointer operator->() const { return &(operator*()); }
  const_iterator& operator++();
  const_iterator operator++(int);
  bool operator==(const const_iterator& it) const 
    { return m_cur == it.m_cur; }
  bool operator!=(const const_iterator& it) const 
    { return m_cur != it.m_cur; }
};

inline unsigned long hash_next_prime_(unsigned long n)
{
	enum { num_primes = 28 };

	static const unsigned long prime_list[num_primes] =
	{
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
		1610612741ul, 3221225473ul, 4294967291ul
	};

	const unsigned long* first = prime_list;
	const unsigned long* last = prime_list + (int)num_primes;
	const unsigned long* pos = std::lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}

// -------------------------------------------------------------------------
// class PHashtable

template <class ValueT, class KeyT, class HashT,
          class ExtractKeyT, class EqualKeyT, class PoolT>
class PHashtable
{
private:
  PHashtable(const PHashtable&);
  void operator=(const PHashtable&);

public:
  typedef KeyT key_type;
  typedef ValueT value_type;
  typedef HashT hasher;
  typedef EqualKeyT key_equal;

  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;

  hasher hash_funct() const { return m_hash; }
  key_equal key_eq() const { return m_equals; }

private:
  typedef HashtableNode_<ValueT> NodeT;

private:
  std::vector<NodeT*>  m_buckets;
  PoolT&		  m_pool;
  size_type       m_num_elements;
  hasher          m_hash;
  key_equal       m_equals;
  ExtractKeyT     m_get_key;

public:
  typedef HashtableIter_<ValueT,KeyT,HashT,ExtractKeyT,EqualKeyT,PoolT> iterator;
  typedef HashtableCIter_<ValueT,KeyT,HashT,ExtractKeyT,EqualKeyT,PoolT> const_iterator;

  friend struct HashtableIter_<ValueT,KeyT,HashT,ExtractKeyT,EqualKeyT,PoolT>;
  friend struct HashtableCIter_<ValueT,KeyT,HashT,ExtractKeyT,EqualKeyT,PoolT>;

public:
  PHashtable(PoolT& a, size_type n = 100)
    : m_pool(a),
      m_num_elements(0)
  {
    _M_initialize_buckets(n);
  }

  PHashtable(const PoolT& a, const PHashtable& ht)
    : m_pool(a),
      m_hash(ht.m_hash),
      m_equals(ht.m_equals),
      m_get_key(ht.m_get_key),
      m_num_elements(0)
  {
    _M_copy_from(ht);
  }

  PHashtable& copy(const PHashtable& ht)
  {
    if (&ht != this) {
      clear();
      m_hash = ht.m_hash;
      m_equals = ht.m_equals;
      m_get_key = ht.m_get_key;
      _M_copy_from(ht);
    }
    return *this;
  }

  ~PHashtable() { clear(); }

  size_type size() const { return m_num_elements; }
  size_type max_size() const { return size_type(-1); }
  bool empty() const { return size() == 0; }

  void swap(PHashtable& ht)
  {
	NS_STDEXT::swap_object(this, &ht);
  }

  iterator begin()
  { 
    for (size_type n = 0; n < m_buckets.size(); ++n)
      if (m_buckets[n])
        return iterator(m_buckets[n], this);
    return end();
  }

  iterator end() { return iterator(0, this); }

  const_iterator begin() const
  {
    for (size_type n = 0; n < m_buckets.size(); ++n)
      if (m_buckets[n])
        return const_iterator(m_buckets[n], this);
    return end();
  }

  const_iterator end() const { return const_iterator(0, this); }

  bool operator==(const PHashtable& ht2) const
  {
	  typedef typename PHashtable<ValueT,KeyT,HashT,ExtractKeyT,EqualKeyT,PoolT>::NodeT NodeT;
	  if (m_buckets.size() != ht2.m_buckets.size())
		  return false;
	  for (int n = 0; n < m_buckets.size(); ++n)
	  {
		  NodeT* cur1 = m_buckets[n];
		  NodeT* cur2 = ht2.m_buckets[n];
		  for (; cur1 && cur2 && cur1->m_val == cur2->m_val; cur1 = cur1->m_next, cur2 = cur2->m_next) {}
		  if (cur1 || cur2)
			  return false;
	  }
	  return true;
  }

  bool operator!=(const PHashtable& ht2) const
  {
	  return !(*this == ht2);
  }

public:
  size_type bucket_count() const { return m_buckets.size(); }

  size_type elems_in_bucket(size_type bucket) const
  {
    size_type result = 0;
    for (NodeT* cur = m_buckets[bucket]; cur; cur = cur->m_next)
      result += 1;
    return result;
  }

  std::pair<iterator, bool> insert_unique(const value_type& obj)
  {
    resize(m_num_elements + 1);
    return insert_unique_noresize(obj);
  }

  iterator insert_equal(const value_type& obj)
  {
    resize(m_num_elements + 1);
    return insert_equal_noresize(obj);
  }

  std::pair<iterator, bool> insert_unique_noresize(const value_type& obj);
  iterator insert_equal_noresize(const value_type& obj);
 
  template <class _InputIterator>
  void insert_unique(_InputIterator f, _InputIterator l)
  {
    insert_unique(f, l, __ITERATOR_CATEGORY(f));
  }

  template <class _InputIterator>
  void insert_equal(_InputIterator f, _InputIterator l)
  {
    insert_equal(f, l, __ITERATOR_CATEGORY(f));
  }

  template <class _InputIterator>
  void insert_unique(_InputIterator f, _InputIterator l,
                     input_iterator_tag)
  {
    for ( ; f != l; ++f)
      insert_unique(*f);
  }

  template <class _InputIterator>
  void insert_equal(_InputIterator f, _InputIterator l,
                    input_iterator_tag)
  {
    for ( ; f != l; ++f)
      insert_equal(*f);
  }

  template <class _ForwardIterator>
  void insert_unique(_ForwardIterator f, _ForwardIterator l,
                     forward_iterator_tag)
  {
    size_type n = 0;
    distance(f, l, n);
    resize(m_num_elements + n);
    for ( ; n > 0; --n, ++f)
      insert_unique_noresize(*f);
  }

  template <class _ForwardIterator>
  void insert_equal(_ForwardIterator f, _ForwardIterator l,
                    forward_iterator_tag)
  {
    size_type n = 0;
    distance(f, l, n);
    resize(m_num_elements + n);
    for ( ; n > 0; --n, ++f)
      insert_equal_noresize(*f);
  }

  reference find_or_insert(const value_type& obj);

  iterator find(const key_type& key) 
  {
    size_type n = _M_bkt_num_key(key);
    NodeT* first;
    for ( first = m_buckets[n];
          first && !m_equals(m_get_key(first->m_val), key);
          first = first->m_next)
      {}
    return iterator(first, this);
  }

  const_iterator find(const key_type& key) const
  {
    size_type n = _M_bkt_num_key(key);
    const NodeT* first;
    for ( first = m_buckets[n];
          first && !m_equals(m_get_key(first->m_val), key);
          first = first->m_next)
      {}
    return const_iterator(first, this);
  }

  size_type count(const key_type& key) const
  {
    const size_type n = _M_bkt_num_key(key);
    size_type result = 0;

    for (const NodeT* cur = m_buckets[n]; cur; cur = cur->m_next)
      if (m_equals(m_get_key(cur->m_val), key))
        ++result;
    return result;
  }

  std::pair<iterator, iterator> equal_range(const key_type& key);
  std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

  size_type erase(const key_type& key);
  void erase(const iterator& it);
  void erase(iterator first, iterator last);

  void erase(const const_iterator& it);
  void erase(const_iterator first, const_iterator last);

  void resize(size_type num_elements_hint);
  void clear();

  static size_type node_size() {
	  return sizeof(NodeT);
  }

private:
  size_type _M_next_size(size_type n) const
    { return hash_next_prime_(n); }

  void _M_initialize_buckets(size_type n)
  {
    const size_type n_buckets = _M_next_size(n);
    m_buckets.reserve(n_buckets);
    m_buckets.insert(m_buckets.end(), n_buckets, (NodeT*) 0);
    m_num_elements = 0;
  }

  size_type _M_bkt_num_key(const key_type& key) const
  {
    return _M_bkt_num_key(key, m_buckets.size());
  }

  size_type _M_bkt_num(const value_type& obj) const
  {
    return _M_bkt_num_key(m_get_key(obj));
  }

  size_type _M_bkt_num_key(const key_type& key, size_t n) const
  {
    return m_hash(key) % n;
  }

  size_type _M_bkt_num(const value_type& obj, size_t n) const
  {
    return _M_bkt_num_key(m_get_key(obj), n);
  }

  NodeT* _M_new_node(const value_type& obj)
  {
    NodeT* n = (NodeT*)m_pool.allocate(sizeof(NodeT));
    n->m_next = NULL;
	WINX_TRY
	{
		new(&n->m_val) ValueT(obj);
	}
	WINX_UNWIND(m_pool.deallocate(n));
    return n;
  }
  
  template <class NodeT2>
  void _M_delete_node(NodeT2* n)
  {
    n->~NodeT2();
    m_pool.deallocate(n);
  }

  void _M_erase_bucket(const size_type n, NodeT* first, NodeT* last);
  void _M_erase_bucket(const size_type n, NodeT* last);

  void _M_copy_from(const PHashtable& ht);

};

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&
HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>::operator++()
{
  const NodeT* old = m_cur;
  m_cur = m_cur->m_next;
  if (!m_cur) {
    size_type bucket = m_ht->_M_bkt_num(old->m_val);
    while (!m_cur && ++bucket < m_ht->m_buckets.size())
      m_cur = m_ht->m_buckets[bucket];
  }
  return *this;
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>
HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>::operator++(int)
{
  iterator tmp = *this;
  ++*this;
  return tmp;
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
HashtableCIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&
HashtableCIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>::operator++()
{
  const NodeT* old = m_cur;
  m_cur = m_cur->m_next;
  if (!m_cur) {
    size_type bucket = m_ht->_M_bkt_num(old->m_val);
    while (!m_cur && ++bucket < m_ht->m_buckets.size())
      m_cur = m_ht->m_buckets[bucket];
  }
  return *this;
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline HashtableCIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>
HashtableCIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>::operator++(int)
{
  const_iterator tmp = *this;
  ++*this;
  return tmp;
}

/*
#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline forward_iterator_tag
iterator_category(const HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&)
{
  return forward_iterator_tag();
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline ValueT* 
value_type(const HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&)
{
  return (ValueT*) 0;
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline typename PHashtable<ValueT,KeyT,_HF,_ExK,_EqK,_All>::difference_type*
distance_type(const HashtableIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&)
{
  return (PHashtable<ValueT,KeyT,_HF,_ExK,_EqK,_All>::difference_type*) 0;
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline forward_iterator_tag
iterator_category(const HashtableCIter_<ValueT,KeyT,_HF,
                                                  _ExK,_EqK,_All>&)
{
  return forward_iterator_tag();
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline ValueT* 
value_type(const HashtableCIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&)
{
  return (ValueT*) 0;
}

template <class ValueT, class KeyT, class _HF, class _ExK, class _EqK, 
          class _All>
inline typename PHashtable<ValueT,KeyT,_HF,_ExK,_EqK,_All>::difference_type*
distance_type(const HashtableCIter_<ValueT,KeyT,_HF,_ExK,_EqK,_All>&)
{
  return (PHashtable<ValueT,KeyT,_HF,_ExK,_EqK,_All>::difference_type*) 0;
}

#endif // __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
std::pair<WINX_PHASH_TYPENAME_ PHashtable<ValueT, KeyT, _HF, _Ex, _Eq, _All>::iterator, bool> 
PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>
  ::insert_unique_noresize(const value_type& obj)
{
  const size_type n = _M_bkt_num(obj);
  NodeT* first = m_buckets[n];

  for (NodeT* cur = first; cur; cur = cur->m_next) 
    if (m_equals(m_get_key(cur->m_val), m_get_key(obj)))
      return std::pair<iterator, bool>(iterator(cur, this), false);

  NodeT* tmp = _M_new_node(obj);
  tmp->m_next = first;
  m_buckets[n] = tmp;
  ++m_num_elements;
  return std::pair<iterator, bool>(iterator(tmp, this), true);
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::iterator 
PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::insert_equal_noresize(const value_type& obj)
{
  const size_type n = _M_bkt_num(obj);
  NodeT* first = m_buckets[n];

  for (NodeT* cur = first; cur; cur = cur->m_next) 
    if (m_equals(m_get_key(cur->m_val), m_get_key(obj))) {
      NodeT* tmp = _M_new_node(obj);
      tmp->m_next = cur->m_next;
      cur->m_next = tmp;
      ++m_num_elements;
      return iterator(tmp, this);
    }

  NodeT* tmp = _M_new_node(obj);
  tmp->m_next = first;
  m_buckets[n] = tmp;
  ++m_num_elements;
  return iterator(tmp, this);
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::reference 
PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::find_or_insert(const value_type& obj)
{
  resize(m_num_elements + 1);

  size_type n = _M_bkt_num(obj);
  NodeT* first = m_buckets[n];

  for (NodeT* cur = first; cur; cur = cur->m_next)
    if (m_equals(m_get_key(cur->m_val), m_get_key(obj)))
      return cur->m_val;

  NodeT* tmp = _M_new_node(obj);
  tmp->m_next = first;
  m_buckets[n] = tmp;
  ++m_num_elements;
  return tmp->m_val;
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
std::pair<typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::iterator,
     typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::iterator> 
PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::equal_range(const key_type& key)
{
  typedef std::pair<iterator, iterator> _Pii;
  const size_type n = _M_bkt_num_key(key);

  for (NodeT* first = m_buckets[n]; first; first = first->m_next)
    if (m_equals(m_get_key(first->m_val), key)) {
      for (NodeT* cur = first->m_next; cur; cur = cur->m_next)
        if (!m_equals(m_get_key(cur->m_val), key))
          return _Pii(iterator(first, this), iterator(cur, this));
      for (size_type __m = n + 1; __m < m_buckets.size(); ++__m)
        if (m_buckets[__m])
          return _Pii(iterator(first, this),
                     iterator(m_buckets[__m], this));
      return _Pii(iterator(first, this), end());
    }
  return _Pii(end(), end());
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
std::pair<typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::const_iterator, 
     typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::const_iterator> 
PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>
  ::equal_range(const key_type& key) const
{
  typedef std::pair<const_iterator, const_iterator> _Pii;
  const size_type n = _M_bkt_num_key(key);

  for (const NodeT* first = m_buckets[n] ;
       first; 
       first = first->m_next) {
    if (m_equals(m_get_key(first->m_val), key)) {
      for (const NodeT* cur = first->m_next;
           cur;
           cur = cur->m_next)
        if (!m_equals(m_get_key(cur->m_val), key))
          return _Pii(const_iterator(first, this),
                      const_iterator(cur, this));
      for (size_type __m = n + 1; __m < m_buckets.size(); ++__m)
        if (m_buckets[__m])
          return _Pii(const_iterator(first, this),
                      const_iterator(m_buckets[__m], this));
      return _Pii(const_iterator(first, this), end());
    }
  }
  return _Pii(end(), end());
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
typename PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::size_type 
PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::erase(const key_type& key)
{
  const size_type n = _M_bkt_num_key(key);
  NodeT* first = m_buckets[n];
  size_type erased = 0;

  if (first) {
    NodeT* cur = first;
    NodeT* next = cur->m_next;
    while (next) {
      if (m_equals(m_get_key(next->m_val), key)) {
        cur->m_next = next->m_next;
        _M_delete_node(next);
        next = cur->m_next;
        ++erased;
        --m_num_elements;
      }
      else {
        cur = next;
        next = cur->m_next;
      }
    }
    if (m_equals(m_get_key(first->m_val), key)) {
      m_buckets[n] = first->m_next;
      _M_delete_node(first);
      ++erased;
      --m_num_elements;
    }
  }
  return erased;
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::erase(const iterator& it)
{
  NodeT* p = it.m_cur;
  if (p) {
    const size_type n = _M_bkt_num(p->m_val);
    NodeT* cur = m_buckets[n];

    if (cur == p) {
      m_buckets[n] = cur->m_next;
      _M_delete_node(cur);
      --m_num_elements;
    }
    else {
      NodeT* next = cur->m_next;
      while (next) {
        if (next == p) {
          cur->m_next = next->m_next;
          _M_delete_node(next);
          --m_num_elements;
          break;
        }
        else {
          cur = next;
          next = cur->m_next;
        }
      }
    }
  }
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::erase(iterator first, iterator last)
{
  size_type first_bucket = first.m_cur ? 
    _M_bkt_num(first.m_cur->m_val) : m_buckets.size();
  size_type last_bucket = last.m_cur ? 
    _M_bkt_num(last.m_cur->m_val) : m_buckets.size();

  if (first.m_cur == last.m_cur)
    return;
  else if (first_bucket == last_bucket)
    _M_erase_bucket(first_bucket, first.m_cur, last.m_cur);
  else {
    _M_erase_bucket(first_bucket, first.m_cur, 0);
    for (size_type n = first_bucket + 1; n < last_bucket; ++n)
      _M_erase_bucket(n, 0);
    if (last_bucket != m_buckets.size())
      _M_erase_bucket(last_bucket, last.m_cur);
  }
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
inline void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::erase(const_iterator first, const_iterator last)
{
  erase(iterator(const_cast<NodeT*>(first.m_cur),
                 const_cast<PHashtable*>(first.m_ht)),
        iterator(const_cast<NodeT*>(last.m_cur),
                 const_cast<PHashtable*>(last.m_ht)));
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
inline void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::erase(const const_iterator& it)
{
  erase(iterator(const_cast<NodeT*>(it.m_cur),
                 const_cast<PHashtable*>(it.m_ht)));
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::resize(size_type num_elements_hint)
{
  const size_type old_n = m_buckets.size();
  if (num_elements_hint > old_n)
  {
    const size_type n = _M_next_size(num_elements_hint);
    if (n > old_n)
	{
      std::vector<NodeT*> tmp(n);
      WINX_TRY {
        for (size_type bucket = 0; bucket < old_n; ++bucket) {
          NodeT* first = m_buckets[bucket];
          while (first) {
            size_type __new_bucket = _M_bkt_num(first->m_val, n);
            m_buckets[bucket] = first->m_next;
            first->m_next = tmp[__new_bucket];
            tmp[__new_bucket] = first;
            first = m_buckets[bucket];          
          }
        }
        m_buckets.swap(tmp);
      }
#ifdef WINX_USE_EXCEPTIONS
      catch(...) {
        for (size_type bucket = 0; bucket < tmp.size(); ++bucket) {
          while (tmp[bucket]) {
            NodeT* next = tmp[bucket]->m_next;
            _M_delete_node(tmp[bucket]);
            tmp[bucket] = next;
          }
        }
        throw;
      }
#endif /* WINX_USE_EXCEPTIONS */
    }
  }
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>
  ::_M_erase_bucket(const size_type n, NodeT* first, NodeT* last)
{
  NodeT* cur = m_buckets[n];
  if (cur == first)
    _M_erase_bucket(n, last);
  else {
    NodeT* next;
    for (next = cur->m_next; 
         next != first; 
         cur = next, next = cur->m_next)
      ;
    while (next != last) {
      cur->m_next = next->m_next;
      _M_delete_node(next);
      next = cur->m_next;
      --m_num_elements;
    }
  }
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>
  ::_M_erase_bucket(const size_type n, NodeT* last)
{
  NodeT* cur = m_buckets[n];
  while (cur != last) {
    NodeT* next = cur->m_next;
    _M_delete_node(cur);
    cur = next;
    m_buckets[n] = cur;
    --m_num_elements;
  }
}

template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>::clear()
{
  for (size_type i = 0; i < m_buckets.size(); ++i) {
    NodeT* cur = m_buckets[i];
    while (cur != 0) {
      NodeT* next = cur->m_next;
      _M_delete_node(cur);
      cur = next;
    }
    m_buckets[i] = 0;
  }
  m_num_elements = 0;
}

    
template <class ValueT, class KeyT, class _HF, class _Ex, class _Eq, class _All>
void PHashtable<ValueT,KeyT,_HF,_Ex,_Eq,_All>
  ::_M_copy_from(const PHashtable& ht)
{
  m_buckets.clear();
  m_buckets.reserve(ht.m_buckets.size());
  m_buckets.insert(m_buckets.end(), ht.m_buckets.size(), (NodeT*) 0);
  WINX_TRY {
    for (size_type i = 0; i < ht.m_buckets.size(); ++i) {
      const NodeT* cur = ht.m_buckets[i];
      if (cur) {
        NodeT* cpy = _M_new_node(cur->m_val);
        m_buckets[i] = cpy;

        for (NodeT* next = cur->m_next; 
             next; 
             cur = next, next = cur->m_next) {
          cpy->m_next = _M_new_node(next->m_val);
          cpy = cpy->m_next;
        }
      }
    }
    m_num_elements = ht.m_num_elements;
  }
  WINX_UNWIND(clear());
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_P_HASHTABLE_H */

// Local Variables:
// mode:C++
// End:
