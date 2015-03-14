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
// Module: tpl/regex/grammar/Action.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Action.h 811 2008-07-08 04:51:39Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_ACTION_H
#define TPL_REGEX_GRAMMAR_ACTION_H

#ifndef TPL_REGEX_BASIC_H
#include "../Basic.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "../Action.h"
#endif

#ifndef TPL_REGEX_CASE_H
#include "../Case.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GAct0

template <class GrammarT, class ActionT>
class GAct0
{
public:
	const GrammarT m_x;
	const ActionT m_action;

public:
	GAct0() : m_x(), m_action() {}
	GAct0(const GrammarT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { character = GrammarT::character };

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		if (m_x.match(ar, context, skipper_)) {
			m_action();
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// operator/

// Usage: Rule/SimpleAction, Grammar/SimpleAction
// Usage: Case/Action ==> Case[eps()/Action]

template <class T1, class T2>
struct SimpleActionTraits_ {
	typedef typename RuleOrGrCompose2<T1, T2, Act0, GAct0>::type type;
};

template <class T1, class T2>
struct SimpleActionTraits_<Case<T1>, T2> {
	typedef Condition<CaseAct<T1, T2> > type;
};

template <class T1, class T2>
struct SimpleActionTraits_<Condition<T1>, T2> {
	typedef Condition<CondAct<T1, T2> > type;
};

template <class T1, class T2>
inline typename SimpleActionTraits_<T1, T2>::type const
TPL_CALL operator/(const T1& x, const SimpleAction<T2>& y) {
	return typename SimpleActionTraits_<T1, T2>::type(x, y);
}

// =========================================================================
// class GAct

template <class GrammarT, class ActionT>
class GAct
{
public:
	const GrammarT m_x;
	const ActionT m_action;

public:
	GAct() : m_x(), m_action() {}
	GAct(const GrammarT& x, const ActionT& act)
		: m_x(x), m_action(act) {}

public:
	enum { character = GrammarT::character };

	typedef typename GrammarT::assig_tag assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		TPL_ASSIG_PREPARE(assig_tag, typename ActionT::value_type)

		const iterator pos = ar.position();
		if (m_x.match(ar, context, skipper_)) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, &m_x));
			m_action(val);
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// operator/

// Usage: Rule/Action, Grammar/Action

template <class T1, class T2>
inline typename RuleOrGrCompose2<T1, T2, Act, GAct>::type const
TPL_CALL operator/(const T1& x, const Action<T2>& y) {
	return typename RuleOrGrCompose2<T1, T2, Act, GAct>::type(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_ACTION_H */

