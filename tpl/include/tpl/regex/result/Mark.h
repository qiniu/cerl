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
// Module: tpl/regex/result/Mark.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Mark.h 810 2008-07-07 20:45:03Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_RESULT_MARK_H
#define TPL_REGEX_RESULT_MARK_H

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

#ifndef TPL_REGEX_RULETRAITS_H
#include "../RuleTraits.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------

#define TPL_REGEX_PTR_BITS		(sizeof(size_t) * 8)
#define TPL_REGEX_ARRAY_MARK	((size_t)(1) << (TPL_REGEX_PTR_BITS - 2))
#define TPL_REGEX_NODE_MARK		((size_t)(2) << (TPL_REGEX_PTR_BITS - 2))
#define TPL_REGEX_FALGS_MASK	((size_t)(3) << (TPL_REGEX_PTR_BITS - 2))

struct TagNodeType {};

template <class ValueT>
struct MarkTraits_ {
	static size_t TPL_CALL mark(size_t tag, bool bArray) {
		if (bArray)
			return tag | TPL_REGEX_ARRAY_MARK;
		else
			return tag;
	}
};

template <>
struct MarkTraits_<TagNodeType> {
	static size_t TPL_CALL mark(size_t tag, bool bArray) {
		if (bArray)
			return tag | (TPL_REGEX_NODE_MARK | TPL_REGEX_ARRAY_MARK);
		else
			return tag | TPL_REGEX_NODE_MARK;
	}
};

// -------------------------------------------------------------------------
// class Mark

template <class ValueT, class TagCharT = char>
class Mark
{
private:
	const TagCharT* const m_tag;

private:
	Mark(const Mark&);
	void operator=(const Mark&);

public:
	typedef ValueT value_type;
	
	Mark(const TagCharT* tag_ = NULL, bool bArray_ = false)
		: m_tag((const TagCharT*)MarkTraits_<ValueT>::mark((size_t)tag_, bArray_)) {
	}

	bool TPL_CALL operator==(const Mark& b) const {
		return this == &b;
	}

	bool TPL_CALL operator!=(const Mark& b) const {
		return this != &b;
	}

	const TagCharT* TPL_CALL tag() const {
		return (const TagCharT*)((size_t)m_tag & ~TPL_REGEX_FALGS_MASK);
	}
	
	size_t TPL_CALL isArray() const {
		return (size_t)m_tag & TPL_REGEX_ARRAY_MARK;
	}

	size_t TPL_CALL isNode() const {
		return (size_t)m_tag & TPL_REGEX_NODE_MARK;
	}
	
	bool TPL_CALL isLeaf() const {
		return !isNode();
	}
};

// -------------------------------------------------------------------------
// class LeafAssign

template <class RegExT, class MarkT>
class LeafAssign
{
private:
	const MarkT& m_mark;
	const RegExT m_x;

public:
	LeafAssign(const MarkT& val, const RegExT& x)
		: m_mark(val), m_x(x) {}

public:
	enum { character = RegExT::character | CHARACTER_MARKED };

	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_ASSIG_PREPARE(assig_tag, typename MarkT::value_type)
		
		const iterator pos = ar.position();
		const bool matched = m_x.match(ar, context);
		if (matched) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, &m_x));
			context.insertLeaf(m_mark, val);
		}
		return matched;
	}
};

// -------------------------------------------------------------------------
// class NodeAssign

template <class RegExT, class MarkT>
class NodeAssign
{
private:
	const MarkT& m_mark;
	const RegExT m_x;

public:
	NodeAssign(const MarkT& val, const RegExT& x)
		: m_mark(val), m_x(x) {}

public:
	enum { character = RegExT::character | CHARACTER_MARKED };

	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename ContextT::scope_trans_type trans(context);
		typename ContextT::scope_type scope(context, m_mark);
		if (m_x.match(ar, context))
			return true;
		trans.rollback();
		return false;
	}
};

template <class GrammarT, class MarkT>
class GNodeAssign
{
private:
	const MarkT& m_mark;
	const GrammarT m_x;

public:
	GNodeAssign(const MarkT& val, const GrammarT& x)
		: m_mark(val), m_x(x) {}

public:
	enum { character = GrammarT::character | CHARACTER_MARKED };

	typedef typename GrammarT::assig_tag assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		typename ContextT::scope_trans_type trans(context);
		typename ContextT::scope_type scope(context, m_mark);
		if (m_x.match(ar, context, skipper_))
			return true;
		trans.rollback();
		return false;
	}
};

// -------------------------------------------------------------------------
// function operator/

// Usage: Rule/Mark, Rule/NodeMark, Grammar/NodeMark

template <class T1, class ValueT, class TagCharT>
struct MarkOpTraits_ {
	typedef typename RuleCompose2<T1, Mark<ValueT, TagCharT>, LeafAssign>::rule_type type;
};

template <class T1, class TagCharT>
struct MarkOpTraits_<T1, TagNodeType, TagCharT> {
	typedef typename RuleOrGrCompose2<T1, Mark<TagNodeType, TagCharT>, NodeAssign, GNodeAssign>::type type;
};

template <class T1, class ValueT, class TagCharT>
inline typename MarkOpTraits_<T1, ValueT, TagCharT>::type const
TPL_CALL operator/(const T1& x, const Mark<ValueT, TagCharT>& y) {
	return typename MarkOpTraits_<T1, ValueT, TagCharT>::type(y, x);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_RESULT_MARK_H */
