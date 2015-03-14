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
// Module: tpl/regex/RuleTraits.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: RuleTraits.h 794 2008-07-04 06:27:26Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_RULETRAITS_H
#define TPL_REGEX_RULETRAITS_H

#ifndef TPL_REGEX_DETAIL_TYPETRAITS_H
#include "detail/TypeTraits.h"
#endif

#ifndef TPL_REGEX_BASIC_H
#include "Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// RuleTraits_

template <class ValueT>
struct RuleTraits_ {
};

template <>
struct RuleTraits_<char> {
	typedef Ch1 rule_type;
};

template <>
struct RuleTraits_<unsigned char> {
	typedef Ch1 rule_type;
};

template <>
struct RuleTraits_<wchar_t> {
	typedef Ch1 rule_type;
};

template <>
struct RuleTraits_<int> {
	typedef Ch1 rule_type;
};

template <>
struct RuleTraits_<const char*> {
	typedef EqStr rule_type;
};

template <>
struct RuleTraits_<const unsigned char*> {
	typedef EqStr_<unsigned char> rule_type;
};

template <>
struct RuleTraits_<const wchar_t*> {
	typedef EqWStr rule_type;
};

template <class RegExT>
struct RuleTraits_<Rule<RegExT> > {
	typedef RegExT rule_type;
};

// -------------------------------------------------------------------------
// RuleTraits

template <class ValueT>
struct RuleTraits
{
	typedef typename ArrayTypeTraits<ValueT>::const_type value_type;
	typedef typename RuleTraits_<value_type>::rule_type rule_type;
};

// -------------------------------------------------------------------------
// RuleCompose1

template <
	class T1,
	template <class ArgT1> class Compose,
	int bRule = TPL_TEMPLATE_CONVERTIBLE(T1, Rule)
	>
struct RuleCompose1 {
	typedef Rule<Compose<typename T1::rule_type> > rule_type;
	typedef Grammar<Compose<typename T1::rule_type> > grammar_type;
};

template <
	class T1,
	template <class ArgT1> class Compose
	>
struct RuleCompose1<T1, Compose, 0> {
	typedef Rule<Compose<typename RuleTraits<T1>::rule_type> > rule_type;
	typedef Grammar<Compose<typename RuleTraits<T1>::rule_type> > grammar_type;
};

// -------------------------------------------------------------------------
// RuleCompose2

template <
	class T1, class T2,
	template <class ArgT1, class ArgT2> class Compose,
	int bRule = TPL_TEMPLATE_CONVERTIBLE(T1, Rule)
	>
struct RuleCompose2 {
	typedef Rule<Compose<typename T1::rule_type, T2> > rule_type;
	typedef Grammar<Compose<typename T1::rule_type, T2> > grammar_type;
};

template <
	class T1, class T2,
	template <class ArgT1, class ArgT2> class Compose
	>
struct RuleCompose2<T1, T2, Compose, 0> {
	typedef Rule<Compose<typename RuleTraits<T1>::rule_type, T2> > rule_type;
	typedef Grammar<Compose<typename RuleTraits<T1>::rule_type, T2> > grammar_type;
};

// -------------------------------------------------------------------------
// RuleTemplateCompose2

template <
	class T1, template <class ArgT> class T2,
	template <class ArgT1, template <class ArgT> class ArgT2> class Compose,
	int bRule = TPL_TEMPLATE_CONVERTIBLE(T1, Rule)
	>
struct RuleTemplateCompose2 {
	typedef Rule<Compose<typename T1::rule_type, T2> > rule_type;
	typedef Grammar<Compose<typename T1::rule_type, T2> > grammar_type;
};

template <
	class T1, template <class ArgT> class T2,
	template <class ArgT1, template <class ArgT> class ArgT2> class Compose
	>
struct RuleTemplateCompose2<T1, T2, Compose, 0> {
	typedef Rule<Compose<typename RuleTraits<T1>::rule_type, T2> > rule_type;
	typedef Grammar<Compose<typename RuleTraits<T1>::rule_type, T2> > grammar_type;
};

// -------------------------------------------------------------------------
// RuleOrGrCompose2

template <
	class T1, class T2,
	template <class ArgT1, class ArgT2> class Compose,
	template <class ArgT1, class ArgT2> class GCompose,
	int bGrammar = TPL_TEMPLATE_CONVERTIBLE(T1, Grammar)
	>
struct RuleOrGrCompose2 {
	typedef Grammar<GCompose<typename T1::grammar_type, T2> > type;
};

template <
	class T1, class T2,
	template <class ArgT1, class ArgT2> class Compose,
	template <class ArgT1, class ArgT2> class GCompose
	>
struct RuleOrGrCompose2<T1, T2, Compose, GCompose, 0> {
	typedef RuleCompose2<T1, T2, Compose> Tr_;
	typedef typename Tr_::rule_type type;
};

// =========================================================================
// function gr(): convert a rule to be a grammar

template <class RuleT>
inline typename RuleCompose1<RuleT, Gr>::grammar_type
TPL_CALL gr(const RuleT& x) {
	return typename RuleCompose1<RuleT, Gr>::grammar_type(x);
}

// =========================================================================
// TPL_RULE_UNARY_OP_, TPL_REGEX_BINARY_OP_

#define TPL_RULE_CH_BINARY_OP1_(op, Op, CharT)										\
template <class T2> __forceinline													\
Rule< Op<Ch1, T2> > TPL_CALL operator op(CharT x, const Rule<T2>& y)				\
	{ return ch(x) op y; }															\
template <class T1> __forceinline													\
Rule< Op<T1, Ch1> > TPL_CALL operator op(const Rule<T1>& x, CharT y)				\
	{ return x op ch(y); }

#define TPL_RULE_CH_BINARY_OP_(op, Op)												\
	TPL_RULE_CH_BINARY_OP1_(op, Op, char)											\
	TPL_RULE_CH_BINARY_OP1_(op, Op, unsigned char)									\
	TPL_RULE_CH_BINARY_OP1_(op, Op, wchar_t)

// -------------------------------------------------------------------------

#define TPL_RULE_STR_BINARY_OP1_(op, Op, CharT)										\
template <class T2> __forceinline													\
Rule< Op<EqStr_<CharT>, T2> > TPL_CALL operator op(const CharT* x, const Rule<T2>& y)	\
	{ return str(x) op y; }															\
template <class T1> __forceinline													\
Rule< Op<T1, EqStr_<CharT> > > TPL_CALL operator op(const Rule<T1>& x, const CharT* y)	\
	{ return x op str(y); }

#define TPL_RULE_STR_BINARY_OP_(op, Op)												\
	TPL_RULE_STR_BINARY_OP1_(op, Op, char)											\
	TPL_RULE_STR_BINARY_OP1_(op, Op, unsigned char)									\
	TPL_RULE_STR_BINARY_OP1_(op, Op, wchar_t)

// -------------------------------------------------------------------------

#define TPL_RULE_UNARY_OP_(op, Op)

#define TPL_REGEX_BINARY_OP_(op, Op)										\
	TPL_RULE_CH_BINARY_OP_(op, Op)											\
	TPL_RULE_STR_BINARY_OP_(op, Op)

// =========================================================================
// TPL_GRAMMAR_UNARY_OP_, TPL_GRAMMAR_BINARY_OP_

#define TPL_GRAMMAR_CH_BINARY_OP1_(op, Op, CharT)									\
template <class T2> __forceinline													\
Grammar< Op<Gr<Ch1>, T2> > const TPL_CALL operator op(CharT x, const Grammar<T2>& y) \
	{ return gr(x) op y; }															\
template <class T1> __forceinline													\
Grammar< Op<T1, Gr<Ch1> > > const TPL_CALL operator op(const Grammar<T1>& x, CharT y) \
	{ return x op gr(y); }

#define TPL_GRAMMAR_CH_BINARY_OP_(op, Op)											\
	TPL_GRAMMAR_CH_BINARY_OP1_(op, Op, char)										\
	TPL_GRAMMAR_CH_BINARY_OP1_(op, Op, unsigned char)								\
	TPL_GRAMMAR_CH_BINARY_OP1_(op, Op, wchar_t)

// -------------------------------------------------------------------------

#define TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, CharT)									\
template <class T2> __forceinline													\
Grammar< Op<Gr<EqStr_<CharT> >, T2> > const TPL_CALL operator op(const CharT* x, const Grammar<T2>& y) \
	{ return gr(x) op y; }															\
template <class T1> __forceinline													\
Grammar< Op<T1, Gr<EqStr_<CharT> > > > const TPL_CALL operator op(const Grammar<T1>& x, const CharT* y) \
	{ return x op gr(y); }

#define TPL_GRAMMAR_STR_BINARY_OP_(op, Op)											\
	TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, char)										\
	TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, unsigned char)								\
	TPL_GRAMMAR_STR_BINARY_OP1_(op, Op, wchar_t)

// -------------------------------------------------------------------------

#define TPL_GRAMMAR_UNARY_OP_(op, Op)

#define TPL_GRAMMAR_BINARY_OP_(op, Op)										\
	TPL_GRAMMAR_CH_BINARY_OP_(op, Op)										\
	TPL_GRAMMAR_STR_BINARY_OP_(op, Op)										\
	TPL_GRAMMAR_RULE_BINARY_OP_(op, Op)

// =========================================================================

NS_TPL_END

#endif /* TPL_REGEX_RULETRAITS_H */

