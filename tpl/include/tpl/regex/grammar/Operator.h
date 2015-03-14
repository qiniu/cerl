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
// Module: tpl/regex/grammar/Operator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Operator.h 1361 2009-11-24 04:57:24Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_OPERATOR_H
#define TPL_REGEX_GRAMMAR_OPERATOR_H

#ifndef TPL_REGEX_TERMINAL_H
#include "../Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "../Composition.h"
#endif

#if !defined(_LIMITS_) && !defined(_LIMITS)
#include <climits>
#endif

NS_TPL_BEGIN

// =========================================================================
// operator+

// Usage: Grammar1 >> Grammar2
// Usage: Grammar1 + Grammar2

template <class GrammarT1, class GrammarT2>
class GAnd // Grammar1 Grammar2
{
public:
	const GrammarT1 m_x;
	const GrammarT2 m_y;

public:
	GAnd() : m_x(), m_y() {}
	GAnd(const GrammarT1& x, const GrammarT2& y) : m_x(x), m_y(y) {}

public:
	enum { character = GrammarT1::character | GrammarT2::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		typename ContextT::template trans_type<GrammarT1::character> trans(ar, context);
		if (m_x.match(ar, context, skipper_) && m_y.match(ar, context, skipper_))
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Grammar<GAnd<T1, T2> > const TPL_CALL operator+(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GAnd<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(+, GAnd)

// =========================================================================
// operator|

// Usage: Grammar1 | Grammar2

template <class GrammarT1, class GrammarT2>
class GOr // Grammar1 | Grammar2
{
public:
	const GrammarT1 m_x;
	const GrammarT2 m_y;

public:
	GOr() : m_x(), m_y() {}
	GOr(const GrammarT1& x, const GrammarT2& y) : m_x(x), m_y(y) {}

public:
	enum { character = GrammarT1::character | GrammarT2::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		skipper_.match(ar, context);
		// 	--> NOTE: @@add this code for performance optimization!
		// 	--> BUGFIX: @@may change meanings of Mark/Action/etc! So we disable this optimization!
		// 	--> NOTE2: @@enable this optimization: don't use mark/action in skipper rule!
		return m_x.match(ar, context, skipper_) || m_y.match(ar, context, skipper_);
	}
};

template <class T1, class T2> __forceinline
Grammar<GOr<T1, T2> > const TPL_CALL operator|(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GOr<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(|, GOr)

// =========================================================================
// operator/

// Usage: Grammar1 / Grammar2		--- Restriction

template <class GrammarT1, class GrammarT2>
class GRestr // Grammar1 / Grammar2
{
public:
	const GrammarT1 m_x;
	const GrammarT2 m_y;

public:
	GRestr() : m_x(), m_y() {}
	GRestr(const GrammarT1& x, const GrammarT2& y)
		: m_x(x), m_y(y) {}

public:
	enum { character = GrammarT1::character | GrammarT2::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		typename SourceT::iterator pos = ar.position();
		typename ContextT::template trans_type<character> trans(ar, context);
		if (m_x.match(ar, context, skipper_)) {
			typename SourceT::iterator pos2 = ar.position();
			typename SourceT::restriction_type restr(ar, pos, pos2);
			if (m_y.match(ar, context, skipper_))
				return true;
		}
		trans.rollback(ar);
		return false;
	}
};

template <class T1, class T2> __forceinline
Grammar<GRestr<T1, T2> > const TPL_CALL operator/(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GRestr<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(/, GRestr)

// =========================================================================
// operator* (Unary)

// Usage: *Grammar

template <class GrammarT>
class GRepeat0 // Grammar*
{
public:
	const GrammarT m_x;

public:
	GRepeat0() : m_x() {}
	GRepeat0(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
#if defined(_DEBUG)
		typename SourceT::iterator pos = ar.position();
		while (m_x.match(ar, context, skipper_))
		{
			TPL_ASSERT(ar.position() != pos);
			pos = ar.position();
		}
#else
		while (m_x.match(ar, context, skipper_))
			;
#endif
		return true;
	}
};

template <class T1> __forceinline
Grammar<GRepeat0<T1> > const TPL_CALL operator*(const Grammar<T1>& x) {
	return Grammar<GRepeat0<T1> >(x);
}

TPL_GRAMMAR_UNARY_OP_(*, GRepeat0)

// =========================================================================
// operator+ (Unary)

// Usage: +Grammar

template <class GrammarT>
class GRepeat1 // Grammar+
{
public:
	const GrammarT m_x;

public:
	GRepeat1() : m_x() {}
	GRepeat1(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
#if defined(_DEBUG)
		typename SourceT::iterator pos = ar.position();
		unsigned n = 0;
		while (m_x.match(ar, context, skipper_))
		{
			TPL_ASSERT(ar.position() != pos);
			pos = ar.position();
			++n;
		}
		return n > 0;
#else
		unsigned n = 0;
		while (m_x.match(ar, context, skipper_))
			++n;
		return n > 0;
#endif
	}
};

template <class T1> __forceinline
Grammar<GRepeat1<T1> > const TPL_CALL operator+(const Grammar<T1>& x) {
	return Grammar<GRepeat1<T1> >(x);
}

TPL_GRAMMAR_UNARY_OP_(+, GRepeat1)

// =========================================================================
// operator!

// Usage: !Grammar
// Note: Sorry that we don't have operator?

template <class GrammarT>
class GRepeat01 // Grammar?
{
public:
	const GrammarT m_x;

public:
	GRepeat01() : m_x() {}
	GRepeat01(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		m_x.match(ar, context, skipper_);
		return true;
	}
};

template <class T1> __forceinline
Grammar<GRepeat01<T1> > const TPL_CALL operator!(const Grammar<T1>& x) {
	return Grammar<GRepeat01<T1> >(x);
}

TPL_GRAMMAR_UNARY_OP_(!, GRepeat01)

// =========================================================================
// function repeat

// Usage:
//	1. repeat<n>(Grammar)		--- means: Grammar{n}
//	2. repeat<n, m>(Grammar)	--- means: Grammar{n, m}
//	3. repeat<0, 1>(Grammar)	--- means: Grammar{0, 1}, that is: Grammar?
//	4. repeat_ge<n>(Grammar)	--- means: Grammar{n,}

template <class GrammarT, unsigned nMin, unsigned nMax = UINT_MAX>
class GRepeat // Grammar{nMin, nMax}
{
public:
	const GrammarT m_x;

public:
	GRepeat() : m_x() {}
	GRepeat(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		unsigned n;
		typename ContextT::template trans_type<character> trans(ar, context);
		for (n = 0; n < nMax; ++n)
		{
			if (!m_x.match(ar, context, skipper_))
				break;
		}
		if (n >= nMin)
			return true;
		trans.rollback(ar);
		return false;
	}
};

template <class GrammarT>
class GRepeat<GrammarT, 0, 1> : public GRepeat01<GrammarT>
{
public:
	GRepeat<GrammarT, 0, 1>() {}
	GRepeat<GrammarT, 0, 1>(const GrammarT& x) : GRepeat01<GrammarT>(x) {}
};

template <unsigned nMin, class T1> __forceinline
Grammar<GRepeat<T1, nMin> > const TPL_CALL repeat_ge(const Grammar<T1>& x) {
	return Grammar<GRepeat<T1, nMin> >(x);
}

template <unsigned nMin, class T1> __forceinline
Grammar<GRepeat<T1, nMin, nMin> > const TPL_CALL repeat(const Grammar<T1>& x) {
	return Grammar<GRepeat<T1, nMin, nMin> >(x);
}

template <unsigned nMin, unsigned nMax, class T1> __forceinline
Grammar<GRepeat<T1, nMin, nMax> > const TPL_CALL repeat(const Grammar<T1>& x) {
	return Grammar<GRepeat<T1, nMin, nMax> >(x);
}

// =========================================================================
// operator%

// Usage: Grammar1 % Grammar2		--- means: Grammar1 + *(Grammar2 + Grammar1)

#if 0

template <class T1, class T2>
class GLst : public GAnd<T1, GRepeat0<GAnd<T2, T1> > > // Grammar1 % Grammar2 <=> Grammar1 (Grammar2 Grammar1)*
{
private:
	typedef GAnd<T1, GRepeat0<GAnd<T2, T1> > > Base;

public:
	GLst() {}
	GLst(const Grammar<T1>& x, const Grammar<T2>& y) : Base(x + *(y + x)) {}
};

#else

template <class T1, class T2>
class GLst // Grammar1 % Grammar2 <=> Grammar1 (Grammar2 Grammar1)*
{
public:
	typedef int size_type;

private:
	const GAnd<T2, T1> m_y_and_x;
	mutable size_type m_count;

public:
	GLst() : m_y_and_x() {}
	GLst(const Grammar<T1>& x, const Grammar<T2>& y) : m_y_and_x(y + x) {}

public:
	enum { character = T1::character | T2::character };

	typedef TagAssigLst assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const
	{
		if (m_y_and_x.m_y.match(ar, context, skipper_)) {
			size_type n = 1;
			while (m_y_and_x.match(ar, context, skipper_))
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
Grammar<GLst<T1, T2> > const TPL_CALL operator%(const Grammar<T1>& x, const Grammar<T2>& y) {
	return Grammar<GLst<T1, T2> >(x, y);
}

TPL_GRAMMAR_BINARY_OP_(%, GLst)

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_OPERATOR_H */
