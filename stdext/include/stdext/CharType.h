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
// Module: stdext/CharType.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 10:08:49
// 
// $Id: CharType.h,v 1.4 2006/12/03 07:51:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_CHARTYPE_H
#define STDEXT_CHARTYPE_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct DigitTable

#define STD_DIGIT_TABLE_BASE	'0'
#define STD_DIGIT_TABLE_MAX		('f' - '0' + 1)
#define STD_DIGIT_INVALID		(0xff)

template <class Unused>
struct DigitTableT
{
	static BYTE data[STD_DIGIT_TABLE_MAX];

	static unsigned toDigit(int c) {
		unsigned index = c - STD_DIGIT_TABLE_BASE;
		return index < STD_DIGIT_TABLE_MAX ? data[index] : STD_DIGIT_INVALID;
	}
};

template <class Unused>
BYTE DigitTableT<Unused>::data[STD_DIGIT_TABLE_MAX] =
{
	0,	// 0 [48]
	1,	// 1 [49]
	2,	// 2 [50]
	3,	// 3 [51]
	4,	// 4 [52]
	5,	// 5 [53]
	6,	// 6 [54]
	7,	// 7 [55]
	8,	// 8 [56]
	9,	// 9 [57]
	STD_DIGIT_INVALID,	// : [58]
	STD_DIGIT_INVALID,	// ; [59]
	STD_DIGIT_INVALID,	// < [60]
	STD_DIGIT_INVALID,	// = [61]
	STD_DIGIT_INVALID,	// > [62]
	STD_DIGIT_INVALID,	// ? [63]
	STD_DIGIT_INVALID,	// @ [64]
	10,	// A [65]
	11,	// B [66]
	12,	// C [67]
	13,	// D [68]
	14,	// E [69]
	15,	// F [70]
	STD_DIGIT_INVALID,	// G [71]
	STD_DIGIT_INVALID,	// H [72]
	STD_DIGIT_INVALID,	// I [73]
	STD_DIGIT_INVALID,	// J [74]
	STD_DIGIT_INVALID,	// K [75]
	STD_DIGIT_INVALID,	// L [76]
	STD_DIGIT_INVALID,	// M [77]
	STD_DIGIT_INVALID,	// N [78]
	STD_DIGIT_INVALID,	// O [79]
	STD_DIGIT_INVALID,	// P [80]
	STD_DIGIT_INVALID,	// Q [81]
	STD_DIGIT_INVALID,	// R [82]
	STD_DIGIT_INVALID,	// S [83]
	STD_DIGIT_INVALID,	// T [84]
	STD_DIGIT_INVALID,	// U [85]
	STD_DIGIT_INVALID,	// V [86]
	STD_DIGIT_INVALID,	// W [87]
	STD_DIGIT_INVALID,	// X [88]
	STD_DIGIT_INVALID,	// Y [89]
	STD_DIGIT_INVALID,	// Z [90]
	STD_DIGIT_INVALID,	// [ [91]
	STD_DIGIT_INVALID,	// \ [92]
	STD_DIGIT_INVALID,	// ] [93]
	STD_DIGIT_INVALID,	// ^ [94]
	STD_DIGIT_INVALID,	// _ [95]
	STD_DIGIT_INVALID,	// ` [96]
	10,	// a [97]
	11,	// b [98]
	12,	// c [99]
	13,	// d [100]
	14,	// e [101]
	15,	// f [102]
};

typedef DigitTableT<void> DigitTable;

// -------------------------------------------------------------------------
// STD_CTYPE_XXX

#define STD_CTYPE_UPPER				0x01		/* upper case letter[A-Z] */
#define STD_CTYPE_LOWER				0x02		/* lower case letter[a-z] */
#define STD_CTYPE_DIGIT				0x04		/* digit[0-9] */
#define STD_CTYPE_UNDERLINE			0x08		/* underline[_] */
#define STD_CTYPE_XDIGIT			0x10		/* xdigit[0-9a-fA-F] */
#define STD_CTYPE_SPACE				0x20		/* [ \t\r\n] */
#define STD_CTYPE_ADD				0x40		/* [+] */
#define STD_CTYPE_SUB				0x80		/* [-], dash(���ۺ�)/hyphen(l�ַ�) */
#define STD_CTYPE_MUL				0x100		/* [*] */
#define STD_CTYPE_DIV				0x200		/* [/] */
#define STD_CTYPE_LT				0x400		/* [<] */
#define STD_CTYPE_GT				0x800		/* [>] */
#define STD_CTYPE_EQ				0x1000		/* [=] */
#define STD_CTYPE_RDIV				0x2000		/* [\], right-division, anti-slash */
#define STD_CTYPE_DOT				0x4000		/* [.] */
#define STD_CTYPE_COLON				0x8000		/* [:], colon */
#define STD_CTYPE_PERCENT			0x10000		/* [%] */
#define STD_CTYPE_AND				0x20000		/* [&] */
#define STD_CTYPE_OR				0x40000		/* [|] */
#define STD_CTYPE_MAX_CHAR			128

#define STD_CTYPE_PATH_SEP			(STD_CTYPE_DIV|STD_CTYPE_RDIV)
#define STD_CTYPE_ALPHA				(STD_CTYPE_UPPER|STD_CTYPE_LOWER)
#define STD_SYMBOL_FIRST_CHAR		(STD_CTYPE_ALPHA)
#define STD_SYMBOL_NEXT_CHAR		(STD_SYMBOL_FIRST_CHAR|STD_CTYPE_DIGIT)
#define STD_CSYMBOL_FIRST_CHAR		(STD_CTYPE_ALPHA|STD_CTYPE_UNDERLINE)
#define STD_CSYMBOL_NEXT_CHAR		(STD_CSYMBOL_FIRST_CHAR|STD_CTYPE_DIGIT)
#define STD_XMLSYMBOL_FIRST_CHAR	(STD_CSYMBOL_FIRST_CHAR)
#define STD_XMLSYMBOL_NEXT_CHAR		(STD_CSYMBOL_NEXT_CHAR|STD_CTYPE_SUB)

// -------------------------------------------------------------------------

#define STD_CTYPE_IS_(mask, c)												\
	(STD_CTYPE_MAX_CHAR > (unsigned)(c) ? ((mask) & data[c]) : 0)

#define STD_CTYPE_OP_(op, is)												\
	struct op {																\
		unsigned winx_call operator()(int c) const { return is(c); }		\
	}

#define STD_CTYPE_OP_AND_NOT_(op, is)										\
	STD_CTYPE_OP_(op, is);													\
	STD_CTYPE_OP_(Not##op, !is)

// -------------------------------------------------------------------------
// class CharType

template <class Unused>
struct CharTypeT
{
	static unsigned data[STD_CTYPE_MAX_CHAR];

	static unsigned winx_call is(unsigned typeMask, int c) {
		return STD_CTYPE_IS_(typeMask, c);
	}

	static unsigned winx_call isDigit(int c) {
		return STD_CTYPE_IS_(STD_CTYPE_DIGIT, c);
	}

	static unsigned winx_call isLower(int c) {
		return STD_CTYPE_IS_(STD_CTYPE_LOWER, c);
	}

	static unsigned winx_call isUpper(int c) {
		return STD_CTYPE_IS_(STD_CTYPE_UPPER, c);
	}

	static unsigned winx_call isAlpha(int c) {
		return STD_CTYPE_IS_(STD_CTYPE_ALPHA, c);
	}
	
	static unsigned winx_call isXDigit(int c) {
		return STD_CTYPE_IS_(STD_CTYPE_XDIGIT, c);
	}

	static unsigned winx_call isSpace(int c) {
		return STD_CTYPE_IS_(STD_CTYPE_SPACE, c);
	}

	static unsigned winx_call isSymbolFirstChar(int c) {
		return STD_CTYPE_IS_(STD_SYMBOL_FIRST_CHAR, c);
	}

	static unsigned winx_call isSymbolNextChar(int c) {
		return STD_CTYPE_IS_(STD_SYMBOL_NEXT_CHAR, c);
	}

	static unsigned winx_call isCSymbolFirstChar(int c) {
		return STD_CTYPE_IS_(STD_CSYMBOL_FIRST_CHAR, c);
	}

	static unsigned winx_call isCSymbolNextChar(int c) {
		return STD_CTYPE_IS_(STD_CSYMBOL_NEXT_CHAR, c);
	}

	static unsigned winx_call isXmlSymbolFirstChar(int c) {
		return STD_CTYPE_IS_(STD_XMLSYMBOL_FIRST_CHAR, c);
	}

	static unsigned winx_call isXmlSymbolNextChar(int c) {
		return STD_CTYPE_IS_(STD_XMLSYMBOL_NEXT_CHAR, c);
	}

	static unsigned winx_call isPathSeparator(int c) {
		return c == '/' || c == '\\';
	}

	static unsigned winx_call isUnderline(int c) {
		return c == '_';
	}

	static unsigned winx_call isEOL(int c) {
		return c == 0x0a || c == 0x0d || c == -1;
	}

	struct IsDigit;				struct NotIsDigit;
	struct IsLower;				struct NotIsLower;
	struct IsUpper;				struct NotIsUpper;
	struct IsAlpha;				struct NotIsAlpha;
	struct IsXDigit;			struct NotIsXDigit;
	struct IsCSymbolFirstChar;	struct NotIsCSymbolFirstChar;
	struct IsCSymbolNextChar;	struct NotIsCSymbolNextChar;
	struct IsXmlSymbolFirstChar;struct NotIsXmlSymbolFirstChar;
	struct IsXmlSymbolNextChar;	struct NotIsXmlSymbolNextChar;
	struct IsUnderline;			struct NotIsUnderline;
	struct IsPathSeparator;		struct NotIsPathSeparator;
	struct IsEOL;				struct NotIsEOL;
	struct IsSpace;				struct NotIsSpace;

	STD_CTYPE_OP_AND_NOT_(IsDigit, isDigit);
	STD_CTYPE_OP_AND_NOT_(IsLower, isLower);
	STD_CTYPE_OP_AND_NOT_(IsUpper, isUpper);
	STD_CTYPE_OP_AND_NOT_(IsAlpha, isAlpha);
	STD_CTYPE_OP_AND_NOT_(IsXDigit, isXDigit);
	STD_CTYPE_OP_AND_NOT_(IsUnderline, isUnderline);
	STD_CTYPE_OP_AND_NOT_(IsPathSeparator, isPathSeparator);
	STD_CTYPE_OP_AND_NOT_(IsSymbolFirstChar, isSymbolFirstChar);
	STD_CTYPE_OP_AND_NOT_(IsSymbolNextChar, isSymbolNextChar);
	STD_CTYPE_OP_AND_NOT_(IsCSymbolFirstChar, isCSymbolFirstChar);
	STD_CTYPE_OP_AND_NOT_(IsCSymbolNextChar, isCSymbolNextChar);
	STD_CTYPE_OP_AND_NOT_(IsXmlSymbolFirstChar, isXmlSymbolFirstChar);
	STD_CTYPE_OP_AND_NOT_(IsXmlSymbolNextChar, isXmlSymbolNextChar);
	STD_CTYPE_OP_AND_NOT_(IsEOL, isEOL);
	STD_CTYPE_OP_AND_NOT_(IsSpace, isSpace);
};

template <class Unused>
unsigned CharTypeT<Unused>::data[STD_CTYPE_MAX_CHAR] =
{
	0,	//   [0]
	0,	//   [1]
	0,	//   [2]
	0,	//   [3]
	0,	//   [4]
	0,	//   [5]
	0,	//   [6]
	0,	//   [7]
	0,	//   [8]
	STD_CTYPE_SPACE,	//   [9]
	STD_CTYPE_SPACE,	//   [10]
	0,	//   [11]
	0,	//   [12]
	STD_CTYPE_SPACE,	//   [13]
	0,	//   [14]
	0,	//   [15]
	0,	//   [16]
	0,	//   [17]
	0,	//   [18]
	0,	//   [19]
	0,	//   [20]
	0,	//   [21]
	0,	//   [22]
	0,	//   [23]
	0,	//   [24]
	0,	//   [25]
	0,	//   [26]
	0,	//   [27]
	0,	//   [28]
	0,	//   [29]
	0,	//   [30]
	0,	//   [31]
	STD_CTYPE_SPACE,					//   [32]
	0,	// ! [33]
	0,	// " [34]
	0,	// # [35]
	0,	// $ [36]
	STD_CTYPE_PERCENT,					// % [37]
	STD_CTYPE_AND,						// & [38]
	0,	// ' [39]
	0,	// ( [40]
	0,	// ) [41]
	STD_CTYPE_MUL,						// * [42]
	STD_CTYPE_ADD,						// + [43]
	0,	// , [44]
	STD_CTYPE_SUB,						// - [45]
	STD_CTYPE_DOT,						// . [46]
	STD_CTYPE_DIV,						// / [47]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 0 [48]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 1 [49]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 2 [50]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 3 [51]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 4 [52]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 5 [53]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 6 [54]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 7 [55]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 8 [56]
	STD_CTYPE_DIGIT|STD_CTYPE_XDIGIT,	// 9 [57]
	STD_CTYPE_COLON,					// : [58]
	0,	// ; [59]
	STD_CTYPE_LT,						// < [60]
	STD_CTYPE_EQ,						// = [61]
	STD_CTYPE_GT,						// > [62]
	0,	// ? [63]
	0,	// @ [64]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// A [65]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// B [66]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// C [67]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// D [68]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// E [69]
	STD_CTYPE_UPPER|STD_CTYPE_XDIGIT,	// F [70]
	STD_CTYPE_UPPER,	// G [71]
	STD_CTYPE_UPPER,	// H [72]
	STD_CTYPE_UPPER,	// I [73]
	STD_CTYPE_UPPER,	// J [74]
	STD_CTYPE_UPPER,	// K [75]
	STD_CTYPE_UPPER,	// L [76]
	STD_CTYPE_UPPER,	// M [77]
	STD_CTYPE_UPPER,	// N [78]
	STD_CTYPE_UPPER,	// O [79]
	STD_CTYPE_UPPER,	// P [80]
	STD_CTYPE_UPPER,	// Q [81]
	STD_CTYPE_UPPER,	// R [82]
	STD_CTYPE_UPPER,	// S [83]
	STD_CTYPE_UPPER,	// T [84]
	STD_CTYPE_UPPER,	// U [85]
	STD_CTYPE_UPPER,	// V [86]
	STD_CTYPE_UPPER,	// W [87]
	STD_CTYPE_UPPER,	// X [88]
	STD_CTYPE_UPPER,	// Y [89]
	STD_CTYPE_UPPER,	// Z [90]
	0,	// [ [91]
	STD_CTYPE_RDIV,		// \ [92]
	0,	// ] [93]
	0,	// ^ [94]
	STD_CTYPE_UNDERLINE,	// _ [95]
	0,	// ` [96]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// a [97]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// b [98]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// c [99]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// d [100]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// e [101]
	STD_CTYPE_LOWER|STD_CTYPE_XDIGIT,	// f [102]
	STD_CTYPE_LOWER,	// g [103]
	STD_CTYPE_LOWER,	// h [104]
	STD_CTYPE_LOWER,	// i [105]
	STD_CTYPE_LOWER,	// j [106]
	STD_CTYPE_LOWER,	// k [107]
	STD_CTYPE_LOWER,	// l [108]
	STD_CTYPE_LOWER,	// m [109]
	STD_CTYPE_LOWER,	// n [110]
	STD_CTYPE_LOWER,	// o [111]
	STD_CTYPE_LOWER,	// p [112]
	STD_CTYPE_LOWER,	// q [113]
	STD_CTYPE_LOWER,	// r [114]
	STD_CTYPE_LOWER,	// s [115]
	STD_CTYPE_LOWER,	// t [116]
	STD_CTYPE_LOWER,	// u [117]
	STD_CTYPE_LOWER,	// v [118]
	STD_CTYPE_LOWER,	// w [119]
	STD_CTYPE_LOWER,	// x [120]
	STD_CTYPE_LOWER,	// y [121]
	STD_CTYPE_LOWER,	// z [122]
	0,	// { [123]
	STD_CTYPE_OR,		// | [124]
	0,	// } [125]
	0,	// ~ [126]
	0,	// del [127]
};

typedef CharTypeT<void> CharType;

// -------------------------------------------------------------------------
// class IsCharType

template <unsigned typeMask>
struct IsCharType {
	unsigned winx_call operator()(int c) const {
		return CharType::is(typeMask, c);
	}
};

// -------------------------------------------------------------------------
// CharTraits

template <class CharT>
struct CharTraits {
};

template <>
struct CharTraits<char>
{
	typedef int int_type;

	static int_type winx_call upper(int_type c) {
		return ::toupper(c);
	}

	static int_type winx_call lower(int_type c) {
		return ::tolower(c);
	}
};

template <>
struct CharTraits<wchar_t>
{
	typedef wint_t int_type;
	
	static int_type winx_call upper(int_type c) {
		return towupper(c);
	}

	static int_type winx_call lower(int_type c) {
		return towlower(c);
	}
};

// -------------------------------------------------------------------------
// ToUpper/ToLower/CompareNoCase

template <class CharT>
struct ToUpper
{
	typedef CharTraits<CharT> Tr_;
	typedef typename Tr_::int_type int_type;
	
	int_type winx_call operator()(int_type c) const {
		return Tr_::upper(c);
	}
};

template <class CharT>
struct ToLower
{
	typedef CharTraits<CharT> Tr_;
	typedef typename Tr_::int_type int_type;
	
	int_type winx_call operator()(int_type c) const {
		return Tr_::lower(c);
	}
};

template <class CharT>
struct CompareNoCase
{
	typedef CharTraits<CharT> Tr_;
	typedef typename Tr_::int_type int_type;

	int_type winx_call operator()(int_type c1, int_type c2) const {
		return Tr_::upper(c1) - Tr_::upper(c2);
	}
};

// -------------------------------------------------------------------------
// $Log: CharType.h,v $

NS_STDEXT_END

#endif /* STDEXT_CHARTYPE_H */
