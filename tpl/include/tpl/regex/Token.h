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
// Module: tpl/regex/Token.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Token.h 977 2009-03-27 17:34:00Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_TOKEN_H
#define TPL_REGEX_TOKEN_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Token

template <int tokenType, class RegExT, class PrevTokenT = void>
class Token
{
private:
	const RegExT m_x;
	const PrevTokenT m_y;

public:
	Token() : m_x(), m_y()
	{
	}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, int& rTokenType) const
	{
		if (m_x.match(ar, context))
		{
			rTokenType = tokenType;
			return true;
		}
		return m_y.match(ar, context, rTokenType);
	}
};

template <int tokenType, class RegExT>
class Token<tokenType, RegExT, void>
{
private:
	const RegExT m_x;

public:
	Token() : m_x()
	{
	}

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, int& rTokenType) const
	{
		if (m_x.match(ar, context))
		{
			rTokenType = tokenType;
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// class TokenRule

struct TagAssigToken {};

class AssigToken
{
public:
	template <class IntT, class Iterator, class TokenRuleT>
	static IntT TPL_CALL get(Iterator pos, Iterator pos2, const TokenRuleT* r) {
		return r->getTokenType();
	}
	TPL_ASSIG_DEFVALTYPE(int)
};

TPL_ASSIG_(TagAssigToken, AssigToken)

template <class TokenT>
class TokenRule
{
private:
	const TokenT m_x;
	mutable int m_tokenType;

public:
	TokenRule() : m_x()
	{
	}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigToken assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		return m_x.match(ar, context, m_tokenType);
	}

	int TPL_CALL getTokenType() const {
		return m_tokenType;
	}
};

// -------------------------------------------------------------------------
// LinkToken

template <int tokenType, class RegExT, class PrevTokenT>
struct LinkToken
{
	typedef Token<tokenType, RegExT, PrevTokenT> token_type;
};

template <int tokenType, class PrevTokenT>
struct LinkToken<tokenType, void, PrevTokenT>
{
	typedef PrevTokenT token_type;
};

// -------------------------------------------------------------------------
//
//	TPL_TOKENS_BEGIN(TokensName)
//		TPL_TOKEN(tokenType, RegExT)
//	TPL_TOKENS_END();
//

#define TPL_INVALID_TOKEN_(nVal)											\
	template <>																\
	struct Traits_<nVal> {													\
		typedef void rule_type;												\
	}

#define TPL_TOKENS_BEGIN(Tokens)											\
namespace Tokens															\
{																			\
	enum { FIRST_LINE_ = __LINE__ };										\
	template <int nVal>														\
	struct Traits_ {														\
		typedef void rule_type;												\
	};

#define TPL_TOKEN(tokenType, RegExT)										\
	enum { tokenType = __LINE__ - FIRST_LINE_ };							\
																			\
	template <>																\
	struct Traits_<tokenType> {												\
		typedef RegExT rule_type;											\
	};

#define TPL_TOKENS_END()													\
	enum { TOKEN_TYPE_MAX = __LINE__ - FIRST_LINE_ };						\
	TPL_INVALID_TOKEN_(0);													\
	TPL_INVALID_TOKEN_(TOKEN_TYPE_MAX);										\
	template <int nVal, int nValMax>										\
	struct CollectionT_														\
	{																		\
		typedef typename Traits_<nVal>::rule_type SelfT;					\
		typedef typename CollectionT_<nVal+1, nValMax>::TokenT PrevT;		\
		typedef typename NS_TPL::LinkToken<nVal, SelfT, PrevT>::token_type TokenT; \
	};																		\
	template <int nValMax>													\
	struct CollectionT_<nValMax, nValMax>									\
	{																		\
		typedef void TokenT;												\
	};																		\
	typedef CollectionT_<1, TOKEN_TYPE_MAX>::TokenT token_type;				\
	typedef NS_TPL::TokenRule<token_type> rule_type;						\
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_TOKEN_H */
