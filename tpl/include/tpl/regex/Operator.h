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
// Module: tpl/regex/Operator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Operator.h 1392 2009-12-30 07:32:50Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_OPERATOR_H
#define TPL_REGEX_OPERATOR_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_RULETRAITS_H
#include "RuleTraits.h"
#endif

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

NS_TPL_BEGIN

// =========================================================================
// operator+

// Usage: Rule1 + Rule2

template <class RegExT1, class RegExT2>
class And // Rule1 Rule2
{
public:
	const RegExT1 m_x;
	const RegExT2 m_y;

public:
	And() : m_x(), m_y() {}
	And(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

public:
	enum { character = RegExT1::character | RegExT2::character };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename ContextT::template trans_type<RegExT1::character> trans(ar, context);
		if (m_x.match(ar, context) && m_y.match(ar, context))
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Rule<And<T1, T2> > const TPL_CALL operator+(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<And<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(+, And)

// =========================================================================
// operator|

// Usage: Rule1 | Rule2

template <class RegExT1, class RegExT2>
class Or // Rule1 | Rule2
{
private:
	typedef typename RegExT1::convertible_type CT1;
	typedef typename RegExT2::convertible_type CT2;
	typedef typename RegExT1::assig_tag AT1;
	typedef typename RegExT2::assig_tag AT2;

public:
	const RegExT1 m_x;
	const RegExT2 m_y;

public:
	Or() : m_x(), m_y() {}
	Or(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {}

public:
	enum { character = RegExT1::character | RegExT2::character };

	typedef typename AndConvertable<CT1, CT2>::convertible_type convertible_type;
	typedef typename AndAssigTag<AT1, AT2>::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		return m_x.match(ar, context) || m_y.match(ar, context);
	}

	bool TPL_CALL operator()(int c) const {
		return m_x(c) || m_y(c);
	}
};

template <class T1, class T2> __forceinline
Rule<Or<T1, T2> > const TPL_CALL operator|(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<Or<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(|, Or)

// =========================================================================
// operator/

// Usage: Rule1 / Rule2		--- Restriction

template <class RegExT1, class RegExT2>
class Restr // Rule1 / Rule2
{
public:
	const RegExT1 m_x;
	const RegExT2 m_y;

private:
	typedef typename RegExT1::convertible_type CT1;
	typedef typename RegExT2::convertible_type CT2;

public:
	Restr() : m_x(), m_y() {}
	Restr(const RegExT1& x, const RegExT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = RegExT1::character | RegExT2::character };

	typedef typename OrConvertable<CT1, CT2>::convertible_type convertible_type;
	typedef typename RegExT1::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		typename ContextT::template trans_type<character> trans(ar, context);
		if (m_x.match(ar, context)) {
			typename SourceT::iterator pos2 = ar.position();
			typename SourceT::restriction_type restr(ar, pos, pos2);
			if (m_y.match(ar, context))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Rule<Restr<T1, T2> > const TPL_CALL operator/(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<Restr<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(/, Restr)

// =========================================================================
// operator* (Unary)

// Usage: *Rule

template <class RegExT>
class Repeat0 // Rule*
{
public:
	const RegExT m_x;

public:
	Repeat0() : m_x() {}
	Repeat0(const RegExT& x) : m_x(x) {}

public:
	enum { character = RegExT::character };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
#if defined(_DEBUG)
		typename SourceT::iterator pos = ar.position();
		while (m_x.match(ar, context))
		{
			TPL_ASSERT(ar.position() != pos);
			pos = ar.position();
		}
#else
		while (m_x.match(ar, context))
			;
#endif
		return true;
	}
};

template <class T1> __forceinline
Rule<Repeat0<T1> > const TPL_CALL operator*(const Rule<T1>& x) {
	return Rule<Repeat0<T1> >(x);
}

TPL_RULE_UNARY_OP_(*, Repeat0)

// =========================================================================
// operator+ (Unary)

// Usage: +Rule

template <class RegExT>
class Repeat1 // Rule+
{
public:
	const RegExT m_x;

public:
	Repeat1() : m_x() {}
	Repeat1(const RegExT& x) : m_x(x) {}

public:
	enum { character = RegExT::character };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
#if defined(_DEBUG)
		typename SourceT::iterator pos = ar.position();
		unsigned n = 0;
		while (m_x.match(ar, context))
		{
			TPL_ASSERT(ar.position() != pos);
			pos = ar.position();
			++n;
		}
		return n > 0;
#else
		unsigned n = 0;
		while (m_x.match(ar, context))
			++n;
		return n > 0;
#endif
	}
};

template <class T1> __forceinline
Rule<Repeat1<T1> > const TPL_CALL operator+(const Rule<T1>& x) {
	return Rule<Repeat1<T1> >(x);
}

TPL_RULE_UNARY_OP_(+, Repeat1)

// =========================================================================
// operator!

// Usage: !Rule
// Note: Sorry that we don't have operator?

template <class RegExT>
class Repeat01 // Rule?
{
public:
	const RegExT m_x;

public:
	Repeat01() : m_x() {}
	Repeat01(const RegExT& x) : m_x(x) {}

public:
	enum { character = RegExT::character };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		m_x.match(ar, context);
		return true;
	}
};

template <class T1> __forceinline
Rule<Repeat01<T1> > const TPL_CALL operator!(const Rule<T1>& x) {
	return Rule<Repeat01<T1> >(x);
}

TPL_RULE_UNARY_OP_(!, Repeat01)

// =========================================================================
// operator~

// Usage: ~ChRange

template <class RegExT>
class Not // ~Rule
{
public:
	const RegExT m_x;

public:
	Not() : m_x() {}
	Not(const RegExT& x) : m_x(x) {}

public:
	enum { character = RegExT::character };

	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::int_type c = ar.get();
		if (!m_x(c))
			return true;
		ar.unget(c);
		return false;
	}

	bool TPL_CALL operator()(int c) const {
		return !m_x(c);
	}

private:
	TPL_REQUIRE_CLASS(typename RegExT::assig_tag, TagAssigChar, ChRuleRequire_);
};

template <class T1> __forceinline
Rule<Not<T1> > const TPL_CALL operator~(const Rule<T1>& x) {
	return Rule<Not<T1> >(x);
}

TPL_RULE_UNARY_OP_(~, Not)

// =========================================================================
// function peek

// Usage: peek(ChRange)

template <class PredT>
class Peek // ~Rule
{
public:
	const PredT m_x;

public:
	Peek() : m_x() {}
	Peek(const PredT& x) : m_x(x) {}

public:
	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x(ar.peek()) != 0;
	}
};

template <int m_c1, int m_c2 = m_c1, int m_c3 = m_c2, int m_c4 = m_c3>
class PeekCh : public Peek<C_<m_c1, m_c2, m_c3, m_c4> > {
};

template <class T1> __forceinline
Rule<Peek<T1> > const TPL_CALL peek(const Rule<T1>& x) {
	return Rule<Peek<T1> >(x);
}

template <int m_c>
__forceinline Rule<PeekCh<m_c> > const TPL_CALL peek() {
	return Rule<PeekCh<m_c> >();
}

template <int m_c1, int m_c2>
__forceinline Rule<PeekCh<m_c1, m_c2> > const TPL_CALL peek() {
	return Rule<PeekCh<m_c1, m_c2> >();
}

template <int m_c1, int m_c2, int m_c3>
__forceinline Rule<PeekCh<m_c1, m_c2, m_c3> > const TPL_CALL peek() {
	return Rule<PeekCh<m_c1, m_c2, m_c3> >();
}

template <int m_c1, int m_c2, int m_c3, int m_c4>
__forceinline Rule<PeekCh<m_c1, m_c2, m_c3, m_c4> > const TPL_CALL peek() {
	return Rule<PeekCh<m_c1, m_c2, m_c3, m_c4> >();
}

// =========================================================================
// function peek_not

template <int m_c1, int m_c2 = m_c1, int m_c3 = m_c2, int m_c4 = m_c3>
class PeekNot : public Peek<N_<m_c1, m_c2, m_c3, m_c4> > {
};

template <int m_c>
__forceinline Rule<PeekNot<m_c> > const TPL_CALL peek_not() {
	return Rule<PeekNot<m_c> >();
}

template <int m_c1, int m_c2>
__forceinline Rule<PeekNot<m_c1, m_c2> > const TPL_CALL peek_not() {
	return Rule<PeekNot<m_c1, m_c2> >();
}

template <int m_c1, int m_c2, int m_c3>
__forceinline Rule<PeekNot<m_c1, m_c2, m_c3> > const TPL_CALL peek_not() {
	return Rule<PeekNot<m_c1, m_c2, m_c3> >();
}

template <int m_c1, int m_c2, int m_c3, int m_c4>
__forceinline Rule<PeekNot<m_c1, m_c2, m_c3, m_c4> > const TPL_CALL peek_not() {
	return Rule<PeekNot<m_c1, m_c2, m_c3, m_c4> >();
}

// =========================================================================
// function repeat

// Usage:
//	1. repeat<n>(Rule)		--- means: Rule{n}
//	2. repeat<n, m>(Rule)	--- means: Rule{n, m}
//	3. repeat<0, 1>(Rule)	--- means: Rule{0, 1}, that is: Rule?
//	4. repeat_ge<n>(Rule)	--- means: Rule{n,}

template <class RegExT, unsigned nMin, unsigned nMax = UINT_MAX>
class Repeat // Rule{nMin, nMax}
{
public:
	const RegExT m_x;

public:
	Repeat() : m_x() {}
	Repeat(const RegExT& x) : m_x(x) {}

public:
	enum { character = RegExT::character };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		unsigned n;
		typename ContextT::template trans_type<character> trans(ar, context);
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context))
				break;
		}
		if (n >= nMin)
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class RegExT>
class Repeat<RegExT, 0, 1> : public Repeat01<RegExT>
{
public:
	Repeat<RegExT, 0, 1>() {}
	Repeat<RegExT, 0, 1>(const RegExT& x) : Repeat01<RegExT>(x) {}
};

template <unsigned nMin, class T1> __forceinline
Rule<Repeat<T1, nMin> > const TPL_CALL repeat_ge(const Rule<T1>& x) {
	return Rule<Repeat<T1, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Rule<Repeat<T1, nMin, nMin> > const TPL_CALL repeat(const Rule<T1>& x) {
	return Rule<Repeat<T1, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Rule<Repeat<T1, nMin, nMax> > const TPL_CALL repeat(const Rule<T1>& x) {
	return Rule<Repeat<T1, nMin, nMax> >(x);
}

// =========================================================================
// operator%

// Usage: Rule1 % Rule2		--- means: Rule1 + *(Rule2 + Rule1)

#if 1

template <class T1, class T2>
class Lst : public And<T1, Repeat0<And<T2, T1> > > // Rule1 % Rule2 <=> Rule1 (Rule2 Rule1)*
{
private:
	typedef And<T1, Repeat0<And<T2, T1> > > Base;

public:
	Lst() {}
	Lst(const Rule<T1>& x, const Rule<T2>& y) : Base(x + *(y + x)) {}
};

#else

template <class T1, class T2>
class Lst // Rule1 % Rule2 <=> Rule1 (Rule2 Rule1)*
{
public:
	typedef int size_type;

private:
	const And<T2, T1> m_y_and_x;
	mutable size_type m_count;

public:
	Lst() : m_y_and_x() {}
	Lst(const Rule<T1>& x, const Rule<T2>& y) : m_y_and_x(y + x) {}

public:
	enum { character = T1::character | T2::character };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigLst assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		if (m_y_and_x.m_y.match(ar, context)) {
			size_type n = 1;
			while (m_y_and_x.match(ar, context))
				++n;
			m_count = n;
			return true;
		}
		m_count = 0;
		return false;
	}

	size_type TPL_CALL size() const {
		return m_count;
	}
};

#endif

template <class T1, class T2> __forceinline
Rule<Lst<T1, T2> > const TPL_CALL operator%(const Rule<T1>& x, const Rule<T2>& y) {
	return Rule<Lst<T1, T2> >(x, y);
}

TPL_REGEX_BINARY_OP_(%, Lst)

// =========================================================================
// function ws, skipws

// Usage: ws()				--- means: matching Whitespaces. that is: w+
// Usage: skipws()			--- means: skip Whitespaces. that is: w*

typedef Repeat0<Space> SkipWhiteSpaces; // w*
typedef Repeat1<Space> WhiteSpaces; // w+

typedef Repeat0<NonEolSpace> SkipNonEolSpaces;
typedef Repeat1<NonEolSpace> NonEolSpaces;

inline Rule<WhiteSpaces> const TPL_CALL ws() {
	return Rule<WhiteSpaces>();
}

inline Rule<SkipWhiteSpaces> const TPL_CALL skipws() {
	return Rule<SkipWhiteSpaces>();
}

inline Rule<NonEolSpaces> const TPL_CALL non_eol_ws() {
	return Rule<NonEolSpaces>();
}

inline Rule<SkipNonEolSpaces> const TPL_CALL skip_non_eol_ws() {
	return Rule<SkipNonEolSpaces>();
}

TPL_CONST(Rule<SkipWhiteSpaces>, skipws_);
TPL_CONST(Rule<SkipNonEolSpaces>, skip_non_eol_ws_);

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_OPERATOR_H */
