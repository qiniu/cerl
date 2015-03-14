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
// Module: tpl/regex/Case.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Case.h 636 2008-06-11 07:19:52Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CASE_H
#define TPL_REGEX_CASE_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

#ifndef TPL_REGEX_PREDICATE_H
#include "Predicate.h"
#endif

#ifndef TPL_REGEX_CONDITION_H
#include "Condition.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#include "grammar/Condition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class CondSel

template <class ValueT, class ConditionT>
class CondSel
{
public:
	const ValueT& m_x;
	const ConditionT m_y;
	
public:
	CondSel() : m_x(), m_y() {}
	CondSel(const ValueT& x, const ConditionT& y) : m_x(x), m_y(y) {}
	
public:
	enum { character = ConditionT::character };

	typedef TagAssigNone assig_tag;
	typedef ExplicitConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_y.match_if(m_x, ar, context) == matchOk;
	}

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const {
		return m_y.match_if(m_x, ar, context, skipper_) == matchOk;
	}
};

// -------------------------------------------------------------------------
// class Switch

template <class ValueT>
class Switch
{
private:
	const ValueT& m_val;
	
public:
	Switch(const ValueT& val) : m_val(val) {}
	
	template <class ConditionT>
	Rule<CondSel<ValueT, ConditionT> > const TPL_CALL operator[](const Condition<ConditionT>& cond_) const {
		return Rule<CondSel<ValueT, ConditionT> >(m_val, cond_);
	}

	template <class ConditionT>
	Grammar<CondSel<ValueT, ConditionT> > const TPL_CALL operator[](const GCondition<ConditionT>& cond_) const {
		return Grammar<CondSel<ValueT, ConditionT> >(m_val, cond_);
	}
};

template <class ValueT>
inline Switch<ValueT> const TPL_CALL switch_(const ValueT& val) {
	return Switch<ValueT>(val);
}

template <class ValueT>
inline Switch<ValueT> const TPL_CALL switch_(const Var<ValueT>& var_) {
	return Switch<ValueT>(var_.val);
}

// =========================================================================
// class Case

template <class PredT>
class Case : public PredT
{
public:
	Case() : PredT() {}
	
	template <class T1>
	explicit Case(const T1& x) : PredT(x) {}

public:
	template <class NextT>
	Condition<CondBind<PredT, NextT> > const TPL_CALL operator[](const Rule<NextT>& next_) const {
		return Condition<CondBind<PredT, NextT> >(*(const PredT*)this, next_);
	}

	template <class NextT>
	GCondition<GCondBind<PredT, NextT> > const TPL_CALL operator[](const Grammar<NextT>& next_) const {
		return GCondition<GCondBind<PredT, NextT> >(*(const PredT*)this, next_);
	}
};

// -------------------------------------------------------------------------
// function default_

class True_
{
public:
	template <class ValueT2>
	bool TPL_CALL operator()(const ValueT2& val) const {
		return true;
	}
};

inline Case<True_> const TPL_CALL default_() {
	return Case<True_>();
}

// -------------------------------------------------------------------------
// function case_

template <class ValueT>
struct PredTraits_ {
	typedef ValueT pred_type;
};

template <class ValueT>
struct PredTraits_<const ValueT&> {
	typedef ValueT pred_type;
};

/*
template <class PredT>
struct PredTraits_<Predicate<PredT> > {
	typedef PredT pred_type;
};

template <class PredT>
struct PredTraits_<const Predicate<PredT>& > {
	typedef PredT pred_type;
};
*/

#define TPL_PRED_EQ_(Type)						\
template <>										\
struct PredTraits_<Type> {						\
	typedef EQ<Type> pred_type;					\
};												\
												\
template <>										\
struct PredTraits_<Type const&> {				\
	typedef EQ<Type const&> pred_type;			\
};

TPL_PRED_EQ_(bool)
TPL_PRED_EQ_(char)
TPL_PRED_EQ_(wchar_t)
TPL_PRED_EQ_(short)
TPL_PRED_EQ_(int)
TPL_PRED_EQ_(long)
TPL_PRED_EQ_(unsigned short)
TPL_PRED_EQ_(unsigned int)
TPL_PRED_EQ_(unsigned long)

TPL_PRED_EQ_(char*)
TPL_PRED_EQ_(wchar_t*)
TPL_PRED_EQ_(char const*)
TPL_PRED_EQ_(wchar_t const*)

template <class RefT>
struct PredRefTraits_ {
	typedef SmartRefTraits<RefT> Tr_;
	typedef PredTraits_<typename Tr_::const_type> Tr2_;
	typedef typename Tr2_::pred_type pred_type;
};

template <class PredT>
inline Case<PredT> const TPL_CALL case_() {
	return Case<PredT>();
}

template <class ValueT>
inline Case<typename PredRefTraits_<const ValueT&>::pred_type> const
TPL_CALL case_(const ValueT& val) {
	return Case<typename PredRefTraits_<const ValueT&>::pred_type>(val);
}

template <class ValueT>
inline Case<typename PredRefTraits_<ValueT&>::pred_type> const
TPL_CALL case_(ValueT& val) {
	return Case<typename PredRefTraits_<ValueT&>::pred_type>(val);
}

template <class PredT>
inline Case<PredT> const
TPL_CALL case_(const Predicate<PredT>& val) {
	return Case<PredT>(*static_cast<const PredT*>(&val));
}

template <class PredT>
inline Case<PredT> const
TPL_CALL case_(Predicate<PredT>& val) {
	return Case<PredT>(*static_cast<const PredT*>(&val));
}

// =========================================================================
// class CaseAct

template <class PredT, class ActT>
class CaseAct
{
public:
	const PredT m_pred;
	const ActT m_act;

public:
	CaseAct() : m_pred(), m_act() {}
	CaseAct(const PredT& pred, const ActT& act)
		: m_pred(pred), m_act(act) {}

public:
	enum { character = 0 };

	template <class ValueT, class SourceT, class ContextT>
	MatchCode TPL_CALL match_if(const ValueT& val, SourceT& ar, ContextT& context) const
	{
		if (!m_pred(val))
			return matchFailed;
		m_act();
		return matchOk;
	}
};

// Usage: Case/Action ==> Case[eps()/Action]
// Implementation: move to <tpl/regex/grammar/Action.h>

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CASE_H */
