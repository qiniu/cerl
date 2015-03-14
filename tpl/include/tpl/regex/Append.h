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
// Module: tpl/regex/Append.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Append.h 1320 2009-11-18 16:37:24Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_APPEND_H
#define TPL_REGEX_APPEND_H

#ifndef TPL_REGEX_ASSIGN_H
#include "Assign.h"
#endif

#ifndef TPL_REGEX_DETAIL_STLHEADERS_H
#include "detail/STLHeaders.h"
#endif

NS_TPL_BEGIN

// =========================================================================

#define TPL_ACTION_CONTAINER_METHOD1_(Op, op, ValueT)						\
template <class ContainerT>													\
class Op																	\
{																			\
private:																	\
	ContainerT& m_result;													\
																			\
public:																		\
	Op(ContainerT& result) : m_result(result) {								\
	}																		\
																			\
	typedef ValueT value_type;												\
																			\
	template <class ValueT2>												\
	void TPL_CALL operator()(const ValueT2& val) const {					\
		m_result.op(val);													\
	}																		\
};

#define TPL_ACTION_CONTAINER_FN1_(Action, Op, op)							\
																			\
template <class ContainerT> __forceinline									\
Action<Op<ContainerT> > TPL_CALL op(ContainerT& result) {					\
	return Action<Op<ContainerT> >(result);									\
}																			\
																			\
template <class ContainerT> __forceinline									\
Action<Op<ContainerT> > TPL_CALL op(tpl::Var<ContainerT>& result) {			\
	return Action<Op<ContainerT> >(result.val);								\
}

#define TPL_ACTION_METHOD1_(Op, op)											\
	TPL_ACTION_CONTAINER_METHOD1_(Op, op, typename ContainerT::value_type)	\
	TPL_ACTION_CONTAINER_FN1_(tpl::Action, Op, op)

#define TPL_ACTION_METHOD1_EX_(Op, op, ValueT)								\
	TPL_ACTION_CONTAINER_METHOD1_(Op, op, ValueT)							\
	TPL_ACTION_CONTAINER_FN1_(tpl::Action, Op, op)

// =========================================================================
// public macro

#define TPL_ACTION_METHOD0(fn)												\
	TPL_SIMPLE_ACTION_METHOD0_(SAct0_##fn, fn)

#define TPL_ACTION_METHOD1(fn, ValueT)										\
	TPL_ACTION_METHOD1_EX_(Act1_##fn, fn, ValueT)							\
	TPL_SIMPLE_ACTION_METHOD1_(SAct1_##fn, fn)

#define TPL_SIMPLE_ACTION_METHOD2(fn)										\
	TPL_SIMPLE_ACTION_METHOD2_(SAct2_##fn, fn)

// =========================================================================
// class Append

template <class ContainerT>
class Append
{
private:
	ContainerT& m_result;

public:
	Append(ContainerT& result) : m_result(result) {
	}

	typedef typename ContainerT::value_type value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_result.append(val);
	}
};

// Usage: Rule/append(cont)

template <class ContainerT> __forceinline
Action<Append<ContainerT> > TPL_CALL append(ContainerT& result) {
	return Action<Append<ContainerT> >(result);
}

template <class ContainerT> __forceinline
Action<Append<ContainerT> > TPL_CALL append(Var<ContainerT>& result) {
	return Action<Append<ContainerT> >(result.val);
}

// =========================================================================
// function push

TPL_ACTION_METHOD1_(Push_, push)

#if defined(TPL_HAS_STACK)

template <class ValueT, class Container>
class Append<std::stack<ValueT, Container> > : public Push_<std::stack<ValueT, Container> >
{
public:
	Append(std::stack<ValueT, Container>& result)
		: Push_<std::stack<ValueT, Container> >(result) {}
};

#endif // defined(TPL_HAS_STACK)

#if defined(TPL_HAS_QUEUE)

template <class ValueT, class Container>
class Append<std::queue<ValueT, Container> > : public Push_<std::queue<ValueT, Container> >
{
public:
	Append(std::queue<ValueT, Container>& result)
		: Push_<std::queue<ValueT, Container> >(result) {}
};

#endif // defined(TPL_HAS_QUEUE)

// =========================================================================
// class PushBack_/PushFront_

TPL_ACTION_METHOD1_(PushBack_, push_back)
TPL_ACTION_METHOD1_(PushFront_, push_front)

#if defined(TPL_HAS_VECTOR)

template <class ValueT, class Ax>
class Append<std::vector<ValueT, Ax> > : public PushBack_<std::vector<ValueT, Ax> >
{
public:
	Append(std::vector<ValueT, Ax>& result)
		: PushBack_<std::vector<ValueT, Ax> >(result) {}
};

template <class ValueT, class Ax, class ValueT2>
class AppendVal<std::vector<ValueT, Ax>, ValueT2> : public PushBackVal<std::vector<ValueT, Ax>, ValueT2>
{
public:
	AppendVal(std::vector<ValueT, Ax>& result, ValueT2 value_)
		: PushBackVal<std::vector<ValueT, Ax>, ValueT2>(result, value_) {}
};

#endif // defined(TPL_HAS_VECTOR)

#if defined(TPL_HAS_DEQUE)

template <class ValueT, class Ax>
class Append<std::deque<ValueT, Ax> > : public PushBack_<std::deque<ValueT, Ax> >
{
public:
	Append(std::deque<ValueT, Ax>& result)
		: PushBack_<std::deque<ValueT, Ax> >(result) {}
};

#endif

#if defined(TPL_HAS_LIST)

template <class ValueT, class Ax>
class Append<std::list<ValueT, Ax> > : public PushBack_<std::list<ValueT, Ax> >
{
public:
	Append(std::list<ValueT, Ax>& result)
		: PushBack_<std::list<ValueT, Ax> >(result) {}
};

#endif

// =========================================================================
// class Insert_

TPL_ACTION_METHOD1_(Insert_, insert)

#if defined(TPL_HAS_SET)

template <class ValueT, class Pr, class Ax>
class Append<std::set<ValueT, Pr, Ax> > : public Insert_<std::set<ValueT, Pr, Ax> >
{
public:
	Append(std::set<ValueT, Pr, Ax>& result)
		: Insert_<std::set<ValueT, Pr, Ax> >(result) {}
};

template <class ValueT, class Pr, class Ax>
class Append<std::multiset<ValueT, Pr, Ax> > : public Insert_<std::multiset<ValueT, Pr, Ax> >
{
public:
	Append(std::multiset<ValueT, Pr, Ax>& result)
		: Insert_<std::multiset<ValueT, Pr, Ax> >(result) {}
};

#endif

// =========================================================================
// class InsertPair_

template <class ContainerT, bool bOverwrite = false>
class InsertPair_
{
private:
	typedef typename ContainerT::key_type KeyT;
	typedef typename ContainerT::mapped_type MappedT;
	
	ContainerT& m_result;
	const KeyT& m_key;

public:
	InsertPair_(ContainerT& result, const KeyT& key)
		: m_result(result), m_key(key) {
	}
	
	typedef MappedT value_type;

	void TPL_CALL operator()(const MappedT& val) const {
		m_result.insert(typename ContainerT::value_type(m_key, val));
	}
};

template <class ContainerT>
class InsertPair_<ContainerT, true>
{
private:
	typedef typename ContainerT::key_type KeyT;
	typedef typename ContainerT::mapped_type MappedT;
	
	ContainerT& m_result;
	const KeyT& m_key;

public:
	InsertPair_(ContainerT& result, const KeyT& key)
		: m_result(result), m_key(key) {
	}
	
	typedef MappedT value_type;

	void TPL_CALL operator()(const MappedT& val) const {
		m_result[m_key] = val;
	}
};

template <class ContainerT, class KeyT>
inline Action<InsertPair_<ContainerT> >
TPL_CALL insert_pair(ContainerT& result, const KeyT& key) {
	return Action<InsertPair_<ContainerT> >(result, key);
}

template <bool bOverwrite, class ContainerT, class KeyT>
inline Action<InsertPair_<ContainerT, bOverwrite> >
TPL_CALL insert_pair(ContainerT& result, const KeyT& key) {
	return Action<InsertPair_<ContainerT, bOverwrite> >(result, key);
}

// -------------------------------------------------------------------------
// function insert

#if defined(TPL_HAS_MAP)

template <bool bOverwrite, class KeyT, class MappedT, class Pr, class Ax>
__forceinline
Action<InsertPair_<std::map<KeyT, MappedT, Pr, Ax>, bOverwrite> >
TPL_CALL
insert(std::map<KeyT, MappedT, Pr, Ax>& result, const KeyT& key) {
	return Action<InsertPair_<std::map<KeyT, MappedT, Pr, Ax>, bOverwrite> >(result, key);
}

template <class KeyT, class MappedT, class Pr, class Ax>
__forceinline
Action<InsertPair_<std::map<KeyT, MappedT, Pr, Ax>, false> >
TPL_CALL
insert(std::map<KeyT, MappedT, Pr, Ax>& result, const KeyT& key) {
	return Action<InsertPair_<std::map<KeyT, MappedT, Pr, Ax>, false> >(result, key);
}

template <bool bOverwrite, class KeyT, class MappedT, class Pr, class Ax>
__forceinline
Action<InsertPair_<std::multimap<KeyT, MappedT, Pr, Ax>, false> >
TPL_CALL
insert(std::multimap<KeyT, MappedT, Pr, Ax>& result, const KeyT& key) {
	return Action<InsertPair_<std::multimap<KeyT, MappedT, Pr, Ax>, false> >(result, key);
}

template <class KeyT, class MappedT, class Pr, class Ax>
__forceinline
Action<InsertPair_<std::multimap<KeyT, MappedT, Pr, Ax>, false> >
TPL_CALL
insert(std::multimap<KeyT, MappedT, Pr, Ax>& result, const KeyT& key) {
	return Action<InsertPair_<std::multimap<KeyT, MappedT, Pr, Ax>, false> >(result, key);
}

#endif

// =========================================================================
// class PushBackPair_

template <class ContainerT>
class PushBackPair_
{
private:
	typedef typename ContainerT::value_type PairT;
	
	typedef typename PairT::first_type KeyT;
	typedef typename PairT::second_type MappedT;
	
	ContainerT& m_result;
	const KeyT& m_key;

public:
	PushBackPair_(ContainerT& result, const KeyT& key)
		: m_result(result), m_key(key) {
	}
	
	typedef MappedT value_type;

	void TPL_CALL operator()(const MappedT& val) const {
		m_result.push_back(typename ContainerT::value_type(m_key, val));
	}
};

template <class ContainerT, class KeyT>
inline Action<PushBackPair_<ContainerT> >
TPL_CALL push_back_pair(ContainerT& result, const KeyT& key) {
	return Action<PushBackPair_<ContainerT> >(result, key);
}

// =========================================================================
// class Put_

template <class ArchiveT>
class Put_
{
private:
	typedef typename ArchiveT::char_type char_type;

	ArchiveT& m_result;

public:
	Put_(ArchiveT& result)
		: m_result(result) {
	}

	typedef NS_STDEXT::Range<const char_type*> value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_result.put(val.begin(), val.size());
	}
};

template <class ArchiveT>
inline Action<Put_<ArchiveT> > TPL_CALL put(ArchiveT& result) {
	return Action<Put_<ArchiveT> >(result);
}

// =========================================================================

NS_TPL_END

#endif /* TPL_REGEX_APPEND_H */
