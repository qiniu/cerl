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
// Module: tpl/binary/Operator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Operator.h 618 2008-06-01 03:33:32Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_OPERATOR_H
#define TPL_BINARY_OPERATOR_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// operator,

template <class MatchT1, class MatchT2>
class MatchLst
{
private:
	MatchT1 m_x;
	MatchT2 m_y;

public:
	MatchLst(const MatchT1& x, const MatchT2& y)
		: m_x(x), m_y(y) {}

public:
	typedef int result_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(const SourceT& stg, ContextT& context, result_type& result) {
		typename MatchT1::result_type res1;
		if (m_x.match(stg, context, res1)) {
			typename MatchT2::result_type res2;
			return m_y.match(stg, context, res2);
		}
		return false;
	}
};

template <class MatchT1, class MatchT2>
__forceinline
StgRule<MatchLst<MatchT1, MatchT2> >
TPL_CALL operator,(const StgRule<MatchT1>& x, const StgRule<MatchT2>& y) {
	return StgRule<MatchLst<MatchT1, MatchT2> >(x, y);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_OPERATOR_H */
