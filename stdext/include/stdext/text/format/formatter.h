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
// Module: stdext/text/format/formatter.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: formatter.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_FORMATTER_H
#define STDEXT_TEXT_FORMAT_FORMATTER_H

#ifndef STDEXT_TEXT_FORMAT_PRINTF_H
#include "printf.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

namespace format_detail
{
	struct NullAction
	{
		template <class StringT>
		static void winx_call afterPrint(StringT& dest) {}
	};
}

// -------------------------------------------------------------------------
// class FormatAppender

template <class StringT, class ActionT = format_detail::NullAction>
class FormatAppender
{
protected:
	StringT& dest;

public:
	explicit FormatAppender(StringT& o)
		: dest(o)
	{
	}

	template <class CharT>
	void winx_call print(const CharT* fmt)
	{
		format_detail::put(dest, fmt);
		ActionT::afterPrint(dest);
	}

	template <class CharT, class ArgT1>
	void winx_call print(const CharT* fmt, const ArgT1& arg1)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1);
		ActionT::afterPrint(dest);
	}

	template <class CharT, class ArgT1, class ArgT2>
	void winx_call print(const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2);
		ActionT::afterPrint(dest);
	}

	template <class CharT, class ArgT1, class ArgT2, class ArgT3>
	void winx_call print(const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3);
		ActionT::afterPrint(dest);
	}

	template <class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4>
	void winx_call print(
		const CharT* fmt,
		const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3, arg4);
		ActionT::afterPrint(dest);
	}

	template <class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5>
	void winx_call print(
		const CharT* fmt,
		const ArgT1& arg1, const ArgT2& arg2,
		const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5);
		ActionT::afterPrint(dest);
	}

	template <
		class CharT,
		class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6>
	void winx_call print(
		const CharT* fmt,
		const ArgT1& arg1, const ArgT2& arg2,
		const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6);
		ActionT::afterPrint(dest);
	}

	template <
		class CharT,
		class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7>
	void winx_call print(
		const CharT* fmt,
		const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
		const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		ActionT::afterPrint(dest);
	}

	template <
		class CharT,
		class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8>
	void winx_call print(
		const CharT* fmt,
		const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
		const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		ActionT::afterPrint(dest);
	}

	template <
		class CharT,
		class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8, class ArgT9>
	void winx_call print(
		const CharT* fmt,
		const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4,
		const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8, const ArgT9& arg9)
	{
		NS_STDEXT::formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
		ActionT::afterPrint(dest);
	}
};

template <class StringT>
inline FormatAppender<StringT> winx_call formatter(StringT& dest)
{
	NS_STDEXT_TEXT::clear(dest);
	return FormatAppender<StringT>(dest);
}

template <class StringT, class AfterPrintT>
inline FormatAppender<StringT> winx_call formatter(StringT& dest, AfterPrintT action)
{
	NS_STDEXT_TEXT::clear(dest);
	return FormatAppender<StringT, AfterPrintT>(dest);
}

template <class StringT>
inline FormatAppender<StringT> winx_call formatAppender(StringT& dest)
{
	return FormatAppender<StringT>(dest);
}

template <class StringT, class AfterPrintT>
inline FormatAppender<StringT> winx_call formatAppender(StringT& dest, AfterPrintT action)
{
	return FormatAppender<StringT, AfterPrintT>(dest);
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_FORMAT_FORMATTER_H */
