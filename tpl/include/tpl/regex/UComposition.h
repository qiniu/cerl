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
// Module: tpl/regex/UComposition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: UComposition.h 968 2009-03-27 05:48:45Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_UCOMPOSITION_H
#define TPL_REGEX_UCOMPOSITION_H

#if (0)
#define TPL_NO_REDUCE_NAME
#endif

#ifndef TPL_REGEX_OPERATOR_H
#include "Operator.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class UAnd

// Usage: UAnd<Rule1,Rule2>		--- means: Rule1 Rule2 (but no transaction)
// Note: Rule1 and Rule2 cannot contain any Mark operators (See <tpl/regex/Mark.h>)
// If Rule1 or Rule2 contains Mark operators, you should use And operator instead of UAnd.

template <class RegExT1, class RegExT2, class RegExT3 = void, class RegExT4 = void>
class UAnd
{
private:
	const RegExT1 m_x;
	const RegExT2 m_y;
	const RegExT3 m_z;
	const RegExT4 m_u;

public:
	UAnd() : m_x(), m_y(), m_z(), m_u() {
		TPL_ASSERT(
			RegExT1::character == 0 && RegExT2::character == 0 &&
			RegExT3::character == 0 && RegExT4::character == 0);
	}
	UAnd(const RegExT1& x, const RegExT2& y, const RegExT3& z, const RegExT4& u)
		: m_x(x), m_y(y), m_z(z), m_u(u) {
		TPL_ASSERT(
			RegExT1::character == 0 && RegExT2::character == 0 &&
			RegExT3::character == 0 && RegExT4::character == 0);
	}

public:
	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context) && m_y.match(ar, context) &&
			m_z.match(ar, context) && m_u.match(ar, context);
	}
};

template <class RegExT1, class RegExT2, class RegExT3>
class UAnd<RegExT1, RegExT2, RegExT3, void>
{
private:
	const RegExT1 m_x;
	const RegExT2 m_y;
	const RegExT3 m_z;

public:
	UAnd() : m_x(), m_y(), m_z() {
		TPL_ASSERT(
			RegExT1::character == 0 && RegExT2::character == 0 &&
			RegExT3::character == 0);
	}
	UAnd(const RegExT1& x, const RegExT2& y, const RegExT3& z) : m_x(x), m_y(y), m_z(z) {
		TPL_ASSERT(
			RegExT1::character == 0 && RegExT2::character == 0 &&
			RegExT3::character == 0);
	}

public:
	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context) && m_y.match(ar, context) && m_z.match(ar, context);
	}
};

template <class RegExT1, class RegExT2>
class UAnd<RegExT1, RegExT2, void, void>
{
private:
	const RegExT1 m_x;
	const RegExT2 m_y;

public:
	UAnd() : m_x(), m_y() {
		TPL_ASSERT(RegExT1::character == 0 && RegExT2::character == 0);
	}
	UAnd(const RegExT1& x, const RegExT2& y) : m_x(x), m_y(y) {
		TPL_ASSERT(RegExT1::character == 0 && RegExT2::character == 0);
	}

public:
	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context) && m_y.match(ar, context);
	}
};

// -------------------------------------------------------------------------
// class URepeat

template <class RegExT, unsigned nMin, unsigned nMax = UINT_MAX>
class URepeat // Rule{nMin, nMax}
{
private:
	RegExT m_x;

public:
	URepeat() {
		TPL_ASSERT(RegExT::character == 0);
	}
	URepeat(const RegExT& x) : m_x(x) {
		TPL_ASSERT(RegExT::character == 0);
	}

public:
	enum { character = 0 };

	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		unsigned n;
		for (n = 0; n < nMax; ++n) {
			if (!m_x.match(ar, context))
				break;
		}
		return n >= nMin;
	}
};

// -------------------------------------------------------------------------
// class UGuard

// Usage: UGuad<Rule>	--- usually Rule contains some UAnd operators.
// Note: Rule cannot contain any Mark operators (See <tpl/regex/Mark.h>)

template <class RegExT, class AssigTag>
class UGuard
{
private:
	RegExT m_x;

public:
	UGuard() {
		TPL_ASSERT(RegExT::character == 0);
	}
	UGuard(const RegExT& x) : m_x(x) {
		TPL_ASSERT(RegExT::character == 0);
	}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef AssigTag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::iterator pos = ar.position();
		if (m_x.match(ar, context))
			return true;
		ar.seek(pos);
		return false;
	}
};

// -------------------------------------------------------------------------
// TPL_REGEX_GUARD, TPL_REGEX_GUARD0

#ifndef TPL_REGEX_GUARD
#if defined(TPL_NO_REDUCE_NAME)
#define TPL_REGEX_GUARD(UnGuard, Guard, AssigTag)							\
	typedef UGuard<UnGuard, AssigTag> Guard;
#else
#define TPL_REGEX_GUARD(UnGuard, Guard, AssigTag)							\
	class Guard : public UGuard<UnGuard, AssigTag> {};
#endif
#endif

#ifndef TPL_REGEX_GUARD0
#define TPL_REGEX_GUARD0(UnGuard, Guard, AssigTag)							\
	class Guard : public UnGuard {											\
	public:																	\
		typedef AssigTag assig_tag;											\
		typedef AutoConvertible convertible_type;							\
	};
#endif

// =========================================================================
// function strict_eol(), eol()

// strict_eol = \r\n? | \n
// eol = strict_eol | eof

typedef Ch<'\r'> Return;
typedef Ch<'\n'> NewLine;

typedef Or<UAnd<Return, Repeat01<NewLine> >, NewLine> StrictEolU;
typedef Or<StrictEolU, Eof> EolU;

TPL_REGEX_GUARD0(StrictEolU, StrictEolG, TagAssigNone)
TPL_REGEX_GUARD0(EolU, EolG, TagAssigNone)

inline Rule<StrictEolG> TPL_CALL strict_eol() {
	return Rule<StrictEolG>();
}

inline Rule<EolG> TPL_CALL eol() {
	return Rule<EolG>();
}

// =========================================================================
// function not_eol()

typedef Ch<'\r', '\n'> EolSpace;

typedef Not<EolSpace> NotEol;

inline Rule<NotEol> const TPL_CALL not_eol() {
	return Rule<NotEol>();
}

// =========================================================================
// function c_symbol, xml_symbol, etc.

// Usage: c_symbol()		--- means: matching a C symbol. that is: [a-zA-Z_][0-9a-zA-Z_]*
// Usage: xml_symbol()		--- means: matching a XML symbol. that is: [a-zA-Z_][0-9a-zA-Z_\-]*

typedef UAnd<CSymbolFirstChar, Repeat0<CSymbolNextChar> > CSymbolU;
typedef UAnd<XmlSymbolFirstChar, Repeat0<XmlSymbolNextChar> > XmlSymbolU;

TPL_REGEX_GUARD0(CSymbolU, CSymbolG, TagAssigNone)
TPL_REGEX_GUARD0(XmlSymbolU, XmlSymbolG, TagAssigNone)

inline Rule<CSymbolG> TPL_CALL c_symbol() {
	return Rule<CSymbolG>();
}

inline Rule<XmlSymbolG> TPL_CALL xml_symbol() {
	return Rule<XmlSymbolG>();
}

TPL_CONST(Rule<CSymbolG>, c_symbol_);
TPL_CONST(Rule<XmlSymbolG>, xml_symbol_);

// -------------------------------------------------------------------------
// function u_integer, integer, etc.

// Usage: u_integer()		--- means: matching an Unsigned IntegerU. that is: d+
// Usage: integer()			--- means: matching an IntegerU. that is: [+-]?d+

typedef Repeat1<Digit> UIntegerU;
typedef Repeat1<XDigit> HexIntegerU;
typedef Repeat1<OctDigit> OctIntegerU;
typedef Repeat1<BinDigit> BinIntegerU;

typedef Ch<'+', '-'> Sign;
typedef UAnd<Repeat01<Sign>, UIntegerU> IntegerU; // [+-]?d+

TPL_REGEX_GUARD0(UIntegerU, UIntegerG, TagAssigUInteger)
TPL_REGEX_GUARD0(HexIntegerU, HexIntegerG, TagAssigHexInteger)
TPL_REGEX_GUARD0(OctIntegerU, OctIntegerG, TagAssigOctInteger)
TPL_REGEX_GUARD0(BinIntegerU, BinIntegerG, TagAssigBinInteger)
TPL_REGEX_GUARD(IntegerU, IntegerG, TagAssigInteger)

inline Rule<UIntegerG> TPL_CALL u_integer() {
	return Rule<UIntegerG>();
}

inline Rule<HexIntegerG> TPL_CALL hex_integer() {
	return Rule<HexIntegerG>();
}

inline Rule<OctIntegerG> TPL_CALL oct_integer() {
	return Rule<OctIntegerG>();
}

inline Rule<BinIntegerG> TPL_CALL bin_integer() {
	return Rule<BinIntegerG>();
}

inline Rule<IntegerG> TPL_CALL integer() {
	return Rule<IntegerG>();
}

// -------------------------------------------------------------------------
// DateTime

typedef Repeat<Digit, 1, 2> Digit12_;
typedef Repeat<Digit, 2, 2> Digit2_;
typedef Repeat<Digit, 4, 4> Digit4_;
typedef Repeat<Digit2_, 1, 2> Digit24_;

TPL_REGEX_GUARD(Digit4_, Year4G, TagAssigUInteger)
TPL_REGEX_GUARD(Digit2_, Year2G, TagAssigUInteger)
TPL_REGEX_GUARD(Digit24_, YearG, TagAssigUInteger)

TPL_REGEX_GUARD0(Digit12_, MonthG, TagAssigUInteger)
TPL_REGEX_GUARD0(Digit12_, DayG, TagAssigUInteger)
TPL_REGEX_GUARD0(Digit12_, HourG, TagAssigUInteger)
TPL_REGEX_GUARD0(Digit12_, MinuteG, TagAssigUInteger)
TPL_REGEX_GUARD0(Digit12_, SecondG, TagAssigUInteger)

inline Rule<YearG> TPL_CALL t_year()		{ return Rule<YearG>(); }
inline Rule<MonthG> TPL_CALL t_month()		{ return Rule<MonthG>(); }
inline Rule<DayG> TPL_CALL t_day()			{ return Rule<DayG>(); }
inline Rule<HourG> TPL_CALL t_hour()		{ return Rule<HourG>(); }
inline Rule<MinuteG> TPL_CALL t_minute()	{ return Rule<MinuteG>(); }
inline Rule<SecondG> TPL_CALL t_second()	{ return Rule<SecondG>(); }

// -------------------------------------------------------------------------
// function strict_fraction, fraction, etc.

typedef Ch<'.'> DecimalPointer;

typedef UAnd<DecimalPointer, UIntegerU> DecimalPointerStarted_; // \.d+
typedef UAnd<DecimalPointer, Repeat0<Digit> > DecimalPointerStarted0_; // \.d*

typedef UAnd<UIntegerU, DecimalPointerStarted0_> DigitStartedUStrictFraction_; // d+\.d*
typedef Or<DecimalPointerStarted_, DigitStartedUStrictFraction_> UStrictFractionU; // \.d+ | d+\.d*

typedef UAnd<UIntegerU, Repeat01<DecimalPointerStarted0_> > DigitStartedUFraction_; // d+(\.d*)?
typedef Or<DecimalPointerStarted_, DigitStartedUFraction_> UFractionU; // \.d+ | d+(\.d*)?

typedef UAnd<Repeat01<Sign>, UStrictFractionU> StrictFractionU;
typedef UAnd<Repeat01<Sign>, UFractionU> FractionU;

TPL_REGEX_GUARD(UStrictFractionU, UStrictFractionG, TagAssigUFraction)
TPL_REGEX_GUARD(StrictFractionU, StrictFractionG, TagAssigFraction)
TPL_REGEX_GUARD(UFractionU, UFractionG, TagAssigUFraction)
TPL_REGEX_GUARD(FractionU, FractionG, TagAssigFraction)

inline Rule<UStrictFractionG> TPL_CALL u_strict_fraction()
{
	return Rule<UStrictFractionG>();
}

inline Rule<StrictFractionG> TPL_CALL strict_fraction()
{
	return Rule<StrictFractionG>();
}

inline Rule<UFractionG> TPL_CALL u_fraction()
{
	return Rule<UFractionG>();
}

inline Rule<FractionG> TPL_CALL fraction()
{
	return Rule<FractionG>();
}

// -------------------------------------------------------------------------
// function real, strict_real, etc.

// Usage: real()			--- means: matching a RealU Number or an IntegerU
// Usage: strict_real()		--- means: matching a RealU Number (NOT including an IntegerU)

typedef Ch<'E', 'e'> ExponentSign;

typedef UAnd<ExponentSign, IntegerU> ExponentU; // [Ee][+-]?d+

typedef UAnd<DecimalPointerStarted_, Repeat01<ExponentU> > DPS_UStrictReal_;

typedef UAnd<DecimalPointerStarted0_, Repeat01<ExponentU> > DPS0_UStrictReal_;
typedef Or<DPS0_UStrictReal_, ExponentU> DS_UStrictRealSuffix_;
typedef UAnd<UIntegerU, DS_UStrictRealSuffix_> DigitStartUStrictReal_;

typedef Or<DPS_UStrictReal_, DigitStartUStrictReal_> UStrictRealU;
typedef UAnd<Repeat01<Sign>, UStrictRealU> StrictRealU;

typedef UAnd<UFractionU, Repeat01<ExponentU> > URealU;
typedef UAnd<Repeat01<Sign>, URealU> RealU;

TPL_REGEX_GUARD(UStrictRealU, UStrictRealG, TagAssigUReal)
TPL_REGEX_GUARD(StrictRealU, StrictRealG, TagAssigReal)
TPL_REGEX_GUARD(URealU, URealG, TagAssigUReal)
TPL_REGEX_GUARD(RealU, RealG, TagAssigReal)

inline Rule<UStrictRealG> TPL_CALL u_strict_real()
{
	return Rule<UStrictRealG>();
}

inline Rule<StrictRealG> TPL_CALL strict_real()
{
	return Rule<StrictRealG>();
}

inline Rule<URealG> TPL_CALL u_real()
{
	return Rule<URealG>();
}

inline Rule<RealG> TPL_CALL real()
{
	return Rule<RealG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_UCOMPOSITION_H */
