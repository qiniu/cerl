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
// Module: tpl/c/Lex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Lex.h 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_C_LEX_H
#define TPL_C_LEX_H

#ifndef TPL_REGEX_MATCH_H
#include "../regex/Match.h"
#endif

#ifndef TPL_REGEX_TOKEN_H
#include "../regex/Token.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function c_find_continuable_eol<bEat>()

template <bool bEat = false>
class CFindContinuableEol
{
public:
	enum { character = 0 };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		for (;;)
		{
			typename SourceT::int_type c = ar.get();
			switch (c)
			{
			case '\r':
				if (!bEat)
					ar.unget('\r');
				else if (ar.peek() == '\n')
					ar.get();
				return true;
			case '\n':
				if (!bEat)
					ar.unget('\n');
				return true;
			case '\\':
				if (ar.get() == '\r') {
					if (ar.peek() == '\n')
						ar.get();
				}
				break;
			case SourceT::endch:
				return true;
			}
		}
	}

	TPL_SIMPLEST_GRAMMAR_();
};

template <bool bEat>
inline Rule<CFindContinuableEol<bEat> > const TPL_CALL c_find_continuable_eol() {
	return Rule<CFindContinuableEol<bEat> >();
}

inline Rule<CFindContinuableEol<false> > const TPL_CALL c_find_continuable_eol() {
	return Rule<CFindContinuableEol<false> >();
}

// -------------------------------------------------------------------------
// function cpp_single_line_comment<bEatEol>()

typedef Ch<'/'> ChDiv_;

typedef UAnd<ChDiv_, CFindContinuableEol<false> > CppSingleLineCommentEnd_;
typedef UAnd<ChDiv_, CFindContinuableEol<true> > CppSingleLineCommentEnd_EatEol_;

typedef UAnd<ChDiv_, CppSingleLineCommentEnd_> CppSingleLineCommentU;
typedef UAnd<ChDiv_, CppSingleLineCommentEnd_EatEol_> CppSingleLineCommentEatEolU;

TPL_REGEX_GUARD(CppSingleLineCommentU, CppSingleLineCommentG, TagAssigNone);
TPL_REGEX_GUARD(CppSingleLineCommentEatEolU, CppSingleLineCommentEatEolG, TagAssigNone);

inline Rule<CppSingleLineCommentG> const TPL_CALL cpp_single_line_comment() {
	return Rule<CppSingleLineCommentG>();
}

template <bool bEatEol = false>
struct CppSingleLineCommentTraits {
	typedef CppSingleLineCommentG rule_type;
};

template <>
struct CppSingleLineCommentTraits<true> {
	typedef CppSingleLineCommentEatEolG rule_type;
};

template <bool bEatEol>
inline Rule<typename CppSingleLineCommentTraits<bEatEol>::rule_type> const TPL_CALL cpp_single_line_comment() {
	return Rule<typename CppSingleLineCommentTraits<bEatEol>::rule_type>();
}

// -------------------------------------------------------------------------
// function c_comment()

typedef Ch<'*'> ChMul_;

struct FindCCommentEnd_ : UFindStr<char> {
	FindCCommentEnd_() : UFindStr<char>("*/") {}
};

typedef UAnd<ChMul_, FindCCommentEnd_> CCommentEnd_;
typedef UAnd<ChDiv_, CCommentEnd_> CCommentU;

TPL_REGEX_GUARD(CCommentU, CCommentG, TagAssigNone);

inline Rule<CCommentG> const TPL_CALL c_comment() {
	return Rule<CCommentG>();
}

// -------------------------------------------------------------------------
// function cpp_comment<bEatEol>()

typedef Or<CppSingleLineCommentEnd_, CCommentEnd_> CppCommentEnd_;
typedef Or<CppSingleLineCommentEnd_EatEol_, CCommentEnd_> CppCommentEnd_EatEol_;

typedef UAnd<ChDiv_, CppCommentEnd_> CppCommentU;
typedef UAnd<ChDiv_, CppCommentEnd_EatEol_> CppCommentEatEolU;

TPL_REGEX_GUARD(CppCommentU, CppCommentG, TagAssigNone);
TPL_REGEX_GUARD(CppCommentEatEolU, CppCommentEatEolG, TagAssigNone);

inline Rule<CppCommentG> const TPL_CALL cpp_comment() {
	return Rule<CppCommentG>();
}

template <bool bEatEol = false>
struct CppCommentTraits {
	typedef CppCommentG rule_type;
};

template <>
struct CppCommentTraits<true> {
	typedef CppCommentEatEolG rule_type;
};

template <bool bEatEol>
inline Rule<typename CppCommentTraits<bEatEol>::rule_type> const TPL_CALL cpp_comment() {
	return Rule<typename CppCommentTraits<bEatEol>::rule_type>();
}

// =========================================================================
// function c_skip(), cpp_skip()

typedef Lst<SkipWhiteSpaces, CCommentG> CSkipG; // for C
typedef Lst<SkipWhiteSpaces, CppCommentEatEolG> CppSkipG; // for C++

inline Rule<CSkipG> const TPL_CALL c_skip() {
	return Rule<CSkipG>();
}

inline Rule<CppSkipG> const TPL_CALL cpp_skip() {
	return Rule<CppSkipG>();
}

TPL_CONST(Rule<CSkipG>, c_skip_);
TPL_CONST(Rule<CppSkipG>, cpp_skip_);

// =========================================================================
// function c_skip_non_eol() - deprecated

typedef Lst<SkipNonEolSpaces, CCommentG> CSkipNonEolG; // for C Preprocessor, deprecated

__deprecated("Deprecated, please use function 'tpl::c_pp_skip()'.")
inline Rule<CSkipNonEolG> const TPL_CALL c_skip_non_eol() {
	return Rule<CSkipNonEolG>();
}

TPL_CONST(Rule<CSkipNonEolG>, c_skip_non_eol_);

// =========================================================================
// function c_pp_skip()

typedef And<Ch<'\\'>, StrictEolG> CEscapeEol_;
typedef Or<NonEolSpace, CEscapeEol_> CPpSpaceG;
typedef Repeat0<CPpSpaceG> CPpSkipSpaceG;

typedef Lst<CPpSkipSpaceG, CCommentG> CPpSkipG; // for C Preprocessor
typedef Lst<CPpSkipSpaceG, CppCommentG> CPpSkipPlusG;

inline Rule<CPpSkipG> const TPL_CALL c_pp_skip() {
	return Rule<CPpSkipG>();
}

TPL_CONST(Rule<CPpSkipG>, c_pp_skip_);
TPL_CONST(Rule<CPpSkipPlusG>, c_pp_skip_plus_);

// =========================================================================
// function c_string(), c_char()

struct TagAssigCString {};
struct TagAssigCChar {};

template <int delim = '\"'>
class CStringTraits
{
private:
	typedef Ch<delim> Delim_;
	
	typedef Ch<'\\'> Esc_;
	typedef UAnd<Esc_, ChAny> EscChar_;
	
	typedef ChNot<delim, '\r', '\n'> NotStopChar_;
	typedef Or<EscChar_, NotStopChar_> Char_;
	typedef Repeat0<Char_> Chars_;
	
public:
	typedef UAnd<Delim_, Chars_, Delim_> rule_type;
};

typedef CStringTraits<'\"'>::rule_type CStringU;
typedef CStringTraits<'\''>::rule_type CCharU;

TPL_REGEX_GUARD(CStringU, CStringG, TagAssigCString);
TPL_REGEX_GUARD(CCharU, CCharG, TagAssigCChar);

typedef Or<CStringU, CCharU> CStringOrCharU;
typedef Or<CStringG, CCharG> CStringOrCharG;

inline Rule<CStringG> const TPL_CALL c_string() {
	return Rule<CStringG>();
}

inline Rule<CCharG> const TPL_CALL c_char() {
	return Rule<CCharG>();
}

inline Rule<CStringOrCharG> const TPL_CALL c_string_or_char() {
	return Rule<CStringOrCharG>();
}

// =========================================================================
// function c_hex_integer(), c_oct_integer(), c_integer()

struct TagAssigCHexInteger {};
struct TagAssigCOctInteger {};
struct TagAssigCHexOrOctInteger {};
struct TagAssigCUInteger {};
struct TagAssigCInteger {};

typedef Ch<'0'> CChar0_;
typedef Ch<'x', 'X'> CCharX_;
typedef Repeat0<OctDigit> OctIntegerOrNon_;

typedef UAnd<CChar0_, CCharX_, HexIntegerU> CHexIntegerU;
typedef UAnd<CChar0_, OctIntegerOrNon_> COctIntegerU;

TPL_REGEX_GUARD(CHexIntegerU, CHexIntegerG, TagAssigCHexInteger);
TPL_REGEX_GUARD0(COctIntegerU, COctIntegerG, TagAssigCOctInteger);

typedef UAnd<CChar0_, Or<And<CCharX_, HexIntegerG>, OctIntegerOrNon_> > CHexOrOctIntegerU;

TPL_REGEX_GUARD0(CHexOrOctIntegerU, CHexOrOctIntegerG, TagAssigCHexOrOctInteger);

typedef Or<CHexOrOctIntegerG, UIntegerG> CUIntegerU;
typedef Or<CHexOrOctIntegerG, IntegerG> CIntegerU;

TPL_REGEX_GUARD0(CUIntegerU, CUIntegerG, TagAssigCUInteger);
TPL_REGEX_GUARD0(CIntegerU, CIntegerG, TagAssigCInteger);

inline Rule<CHexIntegerG> const TPL_CALL c_hex_integer() {
	return Rule<CHexIntegerG>();
}

inline Rule<COctIntegerG> const TPL_CALL c_oct_integer() {
	return Rule<COctIntegerG>();
}

inline Rule<CUIntegerG> const TPL_CALL c_u_integer() {
	return Rule<CUIntegerG>();
}

inline Rule<CIntegerG> const TPL_CALL c_integer() {
	return Rule<CIntegerG>();
}

// -------------------------------------------------------------------------
// function c_integer_suffix, c_integer_with_suffix

typedef Ch<'U', 'u'> ChU_;
typedef Ch<'L', 'l'> ChL_;

typedef And<ChU_, Repeat01<ChL_> > CIntSuffixU_;
typedef And<ChL_, Repeat01<ChU_> > CIntSuffixL_;
typedef Or<CIntSuffixU_, Or<CIntSuffixL_, Eps> > CIntSuffix_;
typedef Not<ChMask<STD_CTYPE_ALPHA | STD_CTYPE_DIGIT> > CNotAlphaOrDigit_;
typedef And<CIntSuffix_, Peek<CNotAlphaOrDigit_> > CIntSuffixG;
typedef CIntSuffixG CIntegerSuffixG;

typedef And<CIntegerG, CIntegerSuffixG> CIntegerWithSuffixG;

inline Rule<CIntegerSuffixG> const TPL_CALL c_integer_suffix() {
	return Rule<CIntegerSuffixG>();
}

inline Rule<CIntegerWithSuffixG> const TPL_CALL c_integer_with_suffix() {
	return Rule<CIntegerWithSuffixG>();
}

// =========================================================================
// function c_token()

#define TPL_C_NONDIV_OPMASK			(STD_OPTYPE_EXCLAMATION | STD_OPTYPE_QUESTION | STD_OPTYPE_SHARP |				\
	STD_OPTYPE_PARENTHESIS_L | STD_OPTYPE_PARENTHESIS_R | STD_OPTYPE_ADD | STD_OPTYPE_SUB | STD_OPTYPE_MUL |		\
	STD_OPTYPE_LT | STD_OPTYPE_GT | STD_OPTYPE_EQ | STD_OPTYPE_DOT | STD_OPTYPE_COLON | STD_OPTYPE_PERCENT |		\
	STD_OPTYPE_AND | STD_OPTYPE_OR | STD_OPTYPE_XOR | STD_OPTYPE_SQUARE_BRACKET_L | STD_OPTYPE_SQUARE_BRACKET_R |	\
	STD_OPTYPE_BRACE_L | STD_OPTYPE_BRACE_R | STD_OPTYPE_SEMICOLON | STD_OPTYPE_COMMA | STD_OPTYPE_REVERSE)

typedef OpMask<TPL_C_NONDIV_OPMASK> COpNonDivG;		// op_mask<TPL_C_NONDIV_OPMASK>()
typedef And<Ch<'/'>, PeekNot<'/', '*'> > COpDivG;	// ('/' + peek_not<'/', '*'>())

TPL_TOKENS_BEGIN(CTokens)
	TPL_TOKEN(symbol, CSymbolG)
	TPL_TOKEN(op_nondiv, COpNonDivG)
	TPL_TOKEN(op_div, COpDivG)
	TPL_TOKEN(c_string, CStringG)
	TPL_TOKEN(c_char, CCharG)
	TPL_TOKEN(integer, CIntegerWithSuffixG)
TPL_TOKENS_END();

typedef CTokens::rule_type CTokenG;

inline Rule<CTokenG> const TPL_CALL c_token() {
	return Rule<CTokenG>();
}

// -------------------------------------------------------------------------
// function c_token_ext()

TPL_TOKENS_BEGIN(CTokensExt)
	TPL_TOKEN(whites, WhiteSpaces)
	TPL_TOKEN(comment, CppCommentG)
	TPL_TOKEN(symbol, CSymbolG)
	TPL_TOKEN(op_nondiv, COpNonDivG)
	TPL_TOKEN(op_div, COpDivG)
	TPL_TOKEN(c_string, CStringG)
	TPL_TOKEN(c_char, CCharG)
	TPL_TOKEN(integer, CIntegerWithSuffixG)
TPL_TOKENS_END();

typedef CTokensExt::rule_type CTokenExtG;

inline Rule<CTokenExtG> const TPL_CALL c_token_ext() {
	return Rule<CTokenExtG>();
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_C_LEX_H */
