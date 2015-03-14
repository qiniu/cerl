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
// Module: tpl/regex/Terminal.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Terminal.h 992 2009-03-31 14:33:25Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_TERMINAL_H
#define TPL_REGEX_TERMINAL_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../../../../stdext/include/stdext/CharType.h"
#endif

#ifndef STDEXT_OPTYPE_H
#include "../../../../stdext/include/stdext/OpType.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// function ch_any

// Usage: ch_any()	--- means: matching any character.

class ChAny
{
public:
	enum { character = 0 };
	enum { endch = -1 };

	typedef ExplicitConvertible convertible_type;
	typedef TagAssigChar assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_REQUIRE(endch == (int)SourceT::endch, ConstConsistent_);
		TPL_REQUIRE(sizeof(int) == sizeof(typename SourceT::int_type), SizeConsistent_);
		return ar.get() != SourceT::endch;
	}

	bool TPL_CALL operator()(int c) const {
		return c != endch;
	}
};

__forceinline Rule<ChAny> const TPL_CALL ch_any() {
	return Rule<ChAny>();
}

// -------------------------------------------------------------------------
// class EqCh

template <class PredT>
class EqCh : public PredT // Match a Char
{
public:
	EqCh() {}
	
	template <class T1>
	EqCh(const T1& x) : PredT(x) {}

	template <class T1, class T2>
	EqCh(const T1& x, const T2& y) : PredT(x, y) {}

	template <class T1, class T2, class T3>
	EqCh(const T1& x, const T2& y, const T3& z) : PredT(x, y, z) {}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigChar assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		typename SourceT::int_type c = ar.get();
		if (PredT::operator()(c))
			return true;
		ar.unget(c);
		return false;
	}
};

// -------------------------------------------------------------------------
// function ch_range

// Usage: ch_range<'0', '9'>()	--- means: [0-9]

template <int m_c1, int m_c2>
class ChRg_
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return c >= m_c1 && c <= m_c2;
	}
};

template <int m_c1, int m_c2>
class ChRange : public EqCh<ChRg_<m_c1, m_c2> > {
};

template <int m_c1, int m_c2> 
__forceinline Rule<ChRange<m_c1, m_c2> > const TPL_CALL ch_range() {
	return Rule<ChRange<m_c1, m_c2> >();
}

// -------------------------------------------------------------------------
// function ch_not

// Usage: ch_not<'a'>()			--- means: ~ch('a')
// Usage: ch_not<'a', 'b'>()	--- means: ~ch('a', 'b')

template <int m_c1, int m_c2 = m_c1, int m_c3 = m_c2, int m_c4 = m_c3>
class N_
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c1 != c && m_c2 != c && m_c3 != c && m_c4 != c;
	}
};

template <int m_c1, int m_c2, int m_c3>
class N_<m_c1, m_c2, m_c3>
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c1 != c && m_c2 != c && m_c3 != c;
	}
};

template <int m_c1, int m_c2>
class N_<m_c1, m_c2>
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c1 != c && m_c2 != c;
	}
};

template <int m_c>
class N_<m_c>
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c != c;
	}
};

template <int m_c1, int m_c2 = m_c1, int m_c3 = m_c2, int m_c4 = m_c3>
class ChNot : public EqCh<N_<m_c1, m_c2, m_c3, m_c4> > {
};

template <int m_c>
__forceinline Rule<ChNot<m_c> > const TPL_CALL ch_not() {
	return Rule<ChNot<m_c> >();
}

template <int m_c1, int m_c2>
__forceinline Rule<ChNot<m_c1, m_c2> > const TPL_CALL ch_not() {
	return Rule<ChNot<m_c1, m_c2> >();
}

template <int m_c1, int m_c2, int m_c3>
__forceinline Rule<ChNot<m_c1, m_c2, m_c3> > const TPL_CALL ch_not() {
	return Rule<ChNot<m_c1, m_c2, m_c3> >();
}

template <int m_c1, int m_c2, int m_c3, int m_c4>
__forceinline Rule<ChNot<m_c1, m_c2, m_c3, m_c4> > const TPL_CALL ch_not() {
	return Rule<ChNot<m_c1, m_c2, m_c3, m_c4> >();
}

// -------------------------------------------------------------------------
// function ch_mask

template <unsigned mask>
class ChMask : public EqCh<NS_STDEXT::IsCharType<mask> >{
};

template <unsigned mask>
__forceinline Rule<ChMask<mask> > const TPL_CALL ch_mask() {
	return Rule<ChMask<mask> >();
}

// -------------------------------------------------------------------------
// function op_mask

template <unsigned mask>
class OpMask : public EqCh<NS_STDEXT::IsOpType<mask> >{
};

template <unsigned mask>
__forceinline Rule<OpMask<mask> > const TPL_CALL op_mask() {
	return Rule<OpMask<mask> >();
}

// -------------------------------------------------------------------------
// function ch

// Usage: ch<'a'>()			--- means: ch('a')
// Usage: ch<'a', 'b'>()	--- means: ch('a') | ch('b')

template <int m_c1, int m_c2 = m_c1, int m_c3 = m_c2, int m_c4 = m_c3>
class C_
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c1 == c || m_c2 == c || m_c3 == c || m_c4 == c;
	}
};

template <int m_c1, int m_c2, int m_c3>
class C_<m_c1, m_c2, m_c3>
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c1 == c || m_c2 == c || m_c3 == c;
	}
};

template <int m_c1, int m_c2>
class C_<m_c1, m_c2>
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c1 == c || m_c2 == c;
	}
};

template <int m_c>
class C_<m_c>
{
public:
	__forceinline bool TPL_CALL operator()(int c) const {
		return m_c == c;
	}
};

template <int m_c1, int m_c2 = m_c1, int m_c3 = m_c2, int m_c4 = m_c3>
class Ch : public EqCh<C_<m_c1, m_c2, m_c3, m_c4> > {
};

template <int m_c>
__forceinline Rule<Ch<m_c> > const TPL_CALL ch() {
	return Rule<Ch<m_c> >();
}

template <int m_c1, int m_c2>
__forceinline Rule<Ch<m_c1, m_c2> > const TPL_CALL ch() {
	return Rule<Ch<m_c1, m_c2> >();
}

template <int m_c1, int m_c2, int m_c3>
__forceinline Rule<Ch<m_c1, m_c2, m_c3> > const TPL_CALL ch() {
	return Rule<Ch<m_c1, m_c2, m_c3> >();
}

template <int m_c1, int m_c2, int m_c3, int m_c4>
__forceinline Rule<Ch<m_c1, m_c2, m_c3, m_c4> > const TPL_CALL ch() {
	return Rule<Ch<m_c1, m_c2, m_c3, m_c4> >();
}

// -------------------------------------------------------------------------
// function ch()

// Usage: ch('a')
// Sorry that we can't use 'a' directly instead of ch('a') in all case.

class C1
{
private:
	int m_c;

public:
	C1(char c) : m_c((unsigned char)c) {}
	C1(unsigned char c) : m_c(c) {}
	C1(short c) : m_c((unsigned short)c) {}
	C1(wchar_t c) : m_c(c) {}
	C1(int c) : m_c(c) {}

	bool TPL_CALL operator()(int c) const {
		return m_c == c;
	}
};

class C2
{
private:
	int m_c1;
	int m_c2;

public:
	C2(int c1, int c2) : m_c1(c1), m_c2(c2) {}

	bool TPL_CALL operator()(int c) const {
		return m_c1 == c || m_c2 == c;
	}
};

class C3
{
private:
	int m_c1;
	int m_c2;
	int m_c3;

public:
	C3(int c1, int c2, int c3) : m_c1(c1), m_c2(c2), m_c3(c3) {}

	bool TPL_CALL operator()(int c) const {
		return m_c1 == c || m_c2 == c || m_c3 == c;
	}
};

typedef EqCh<C1> Ch1;
typedef EqCh<C2> Ch2;
typedef EqCh<C3> Ch3;

template <class CharT>
__forceinline Rule<Ch1> const TPL_CALL ch(const CharT& x) {
	return Rule<Ch1>(x);
}

__forceinline Rule<Ch2> const TPL_CALL ch(const int c1, const int c2) {
	return Rule<Ch2>(c1, c2);
}

__forceinline Rule<Ch3> const TPL_CALL ch(const int c1, const int c2, const int c3) {
	return Rule<Ch3>(c1, c2, c3);
}

// -------------------------------------------------------------------------
// function space, alpha, ...

// Usage: space()		--- means: matching ONE Writespace Character
// Usage: alpha()		--- means: matching ONE Alpha Character
// Usage: ...

typedef NS_STDEXT::CharType CharType;

typedef Ch<' ', '\t'> NonEolSpace;
typedef Ch<'0', '1'> BinDigit;
typedef ChRange<'0', '9'> Digit;
typedef ChRange<'0', '7'> OctDigit;
typedef ChRange<'a', 'z'> LowerAlpha;
typedef ChRange<'A', 'Z'> UpperAlpha;

typedef EqCh<CharType::IsSpace> Space;
typedef EqCh<CharType::IsAlpha> Alpha;
typedef EqCh<CharType::IsXDigit> XDigit, HexDigit;

typedef EqCh<CharType::IsSymbolFirstChar> SymbolFirstChar;
typedef EqCh<CharType::IsSymbolNextChar> SymbolNextChar;

typedef EqCh<CharType::IsCSymbolFirstChar> CSymbolFirstChar;
typedef EqCh<CharType::IsCSymbolNextChar> CSymbolNextChar;

typedef EqCh<CharType::IsXmlSymbolFirstChar> XmlSymbolFirstChar;
typedef EqCh<CharType::IsXmlSymbolNextChar> XmlSymbolNextChar;

typedef EqCh<CharType::NotIsAlpha> NotAlpha;

inline Rule<NonEolSpace> const TPL_CALL non_eol_space() {
	return Rule<NonEolSpace>();
}

inline Rule<Space> const TPL_CALL space() {
	return Rule<Space>();
}

inline Rule<Alpha> const TPL_CALL alpha() {
	return Rule<Alpha>();
}

inline Rule<NotAlpha> const TPL_CALL not_alpha() {
	return Rule<NotAlpha>();
}

inline Rule<LowerAlpha> const TPL_CALL lower() {
	return Rule<LowerAlpha>();
}

inline Rule<UpperAlpha> const TPL_CALL upper() {
	return Rule<UpperAlpha>();
}

inline Rule<Digit> const TPL_CALL digit() {
	return Rule<Digit>();
}

inline Rule<XDigit> const TPL_CALL xdigit() {
	return Rule<XDigit>();
}

inline Rule<OctDigit> const TPL_CALL oct_digit() {
	return Rule<OctDigit>();
}

inline Rule<BinDigit> const TPL_CALL bin_digit() {
	return Rule<BinDigit>();
}

inline Rule<SymbolFirstChar> const TPL_CALL symbol_first_char() {
	return Rule<SymbolFirstChar>();
}

inline Rule<SymbolNextChar> const TPL_CALL symbol_next_char() {
	return Rule<SymbolNextChar>();
}

inline Rule<CSymbolFirstChar> const TPL_CALL c_symbol_first_char() {
	return Rule<CSymbolFirstChar>();
}

inline Rule<CSymbolNextChar> const TPL_CALL c_symbol_next_char() {
	return Rule<CSymbolNextChar>();
}

inline Rule<XmlSymbolFirstChar> const TPL_CALL xml_symbol_first_char() {
	return Rule<XmlSymbolFirstChar>();
}

inline Rule<XmlSymbolNextChar> const TPL_CALL xml_symbol_next_char() {
	return Rule<XmlSymbolNextChar>();
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_TERMINAL_H */

