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
// Module: tpl/binary/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Basic.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_BASIC_H
#define TPL_BINARY_BASIC_H

#ifndef TPL_REGEX_BASIC_H
#include "../regex/Basic.h"
#endif

#if defined(WINX_BYTESWAP) || defined(BYTESWAP) || defined(__BYTESWAP__)
#define TPL_BYTESWAP
#endif

NS_TPL_BEGIN

// =========================================================================
// class StgRule

template <class RuleT, class SubMatchT>
class SubMatch
{
private:
	RuleT m_rule;
	SubMatchT m_subMatch;

public:
	SubMatch(const RuleT& rule_, const SubMatchT& subMatch_)
		: m_rule(rule_), m_subMatch(subMatch_) {
	}

public:
	typedef typename SubMatchT::result_type result_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(const SourceT& stg, ContextT& context, result_type& result) {
		typedef typename RuleT::result_type ResultT;
		ResultT res;
		if (m_rule.match(stg, context, res))
			return m_subMatch.match(res, context, result);
		return false;
	}
};

template <class RuleT>
class StgRule : public RuleT
{
public:
	template <class ArgT>
	StgRule(const ArgT& x) : RuleT(x) {}

	template <class SubMatchT>
	StgRule<SubMatch<RuleT, SubMatchT> > TPL_CALL operator[](const StgRule<SubMatchT>& subMatch_) const {
		return StgRule<SubMatch<RuleT, SubMatchT> >(*this, subMatch_);
	}
};

// =========================================================================
// struct WStringTraits

template <class ArgT>
struct WStringTraits {
};

template <>
struct WStringTraits<LPCWSTR> {
	LPCWSTR value_type;
};

template <>
struct WStringTraits<LPWSTR> {
	LPCWSTR value_type;
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_BASIC_H */
