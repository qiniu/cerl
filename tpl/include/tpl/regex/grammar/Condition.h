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
// Module: tpl/regex/grammar/Condition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Condition.h 731 2008-06-25 21:47:21Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#define TPL_REGEX_GRAMMAR_CONDITION_H

#ifndef TPL_REGEX_ACTION_H
#include "../Action.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GCond

template <class RegExT, class ConditionT>
class GCond
{
public:
	const RegExT m_x;
	const ConditionT m_y;
	
public:
	GCond() : m_x(), m_y() {}
	GCond(const RegExT& x, const ConditionT& y) : m_x(x), m_y(y) {}
	
public:
	enum { character = RegExT::character | ConditionT::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		TPL_ASSIG_PREPARE1(typename RegExT::assig_tag)

		skipper_.match(ar, context);
		typename ContextT::template trans_type<RegExT::character> trans(ar, context);
		const iterator pos = ar.position();
		if (m_x.match(ar, context)) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, &m_x));
			if (m_y.match_if(val, ar, context, skipper_) == matchOk)
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class RegExT, class ConditionT>
struct IndexOpTraits<Rule<RegExT>, GCondition<ConditionT> >
{
	typedef Grammar<GCond<RegExT, ConditionT> > const result_type;

	static result_type TPL_CALL call(const Rule<RegExT>& rule_, const GCondition<ConditionT>& cond_) {
		return result_type(rule_, cond_);
	}
};

// =========================================================================
// class GCondBind

template <class PredT, class NextT>
class GCondBind
{
public:
	const PredT m_pred;
	const NextT m_next;

public:
	GCondBind() : m_pred(), m_next() {}
	GCondBind(const PredT& pred_, const NextT& next_)
		: m_pred(pred_), m_next(next_) {}
	
public:
	enum { character = NextT::character };
	
	template <class ValueT, class SourceT, class ContextT, class SkipperT>
	MatchCode TPL_CALL match_if(
		const ValueT& val, SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		if (m_pred(val))
			return m_next.match(ar, context, skipper_) ? matchOk : matchStop;
		else
			return matchFailed;
	}
};

// =========================================================================
// operator,

template <class CondT1, class CondT2>
class GCondOr
{
private:
	const CondT1 m_x;
	const CondT2 m_y;
	
public:
	GCondOr(const CondT1& x, const CondT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = CondT1::character | CondT2::character };
	
	template <class ValueT, class SourceT, class ContextT, class SkipperT>
	MatchCode TPL_CALL match_if(
		const ValueT& val, SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		MatchCode result = m_x.match_if(val, ar, context, skipper_);
		if (result != matchFailed)
			return result;
		else
			return m_y.match_if(val, ar, context, skipper_);
	}
};

template <class T1, class T2>
__forceinline
GCondition<GCondOr<T1, T2> > const
TPL_CALL operator,(const GCondition<T1>& x, const GCondition<T2>& y) {
	return GCondition<GCondOr<T1, T2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_CONDITION_H */
