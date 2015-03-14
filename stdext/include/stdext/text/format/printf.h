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
// Module: stdext/text/format/print.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: print.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_PRINTF_H
#define STDEXT_TEXT_FORMAT_PRINTF_H

#ifndef STDEXT_TEXT_FORMAT_PUT_H
#include "put.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

namespace format_detail
{
	template <class StringT, class CharT, class ArgT>
	const CharT* winx_call put(StringT& dest, const CharT* fmt, const ArgT& arg)
	{
		const CharT* p;

Retry:	p = tchar::strchr(fmt, (CharT)'%');
		if (p != NULL)
		{
			NS_STDEXT_TEXT::append(dest, fmt, p);
			++p;

			if (*p != (CharT)'%')
			{
				return NS_STDEXT_FORMAT::put(dest, p, arg);
			}
			else
			{
				fmt = ++p;
				NS_STDEXT_TEXT::append(dest, (CharT)'%');
				goto Retry;
			}
		}
		else
		{
			static const CharT s_null = CharT();
			NS_STDEXT_TEXT::append(dest, fmt);
			return &s_null;
		}
	}

	template <class StringT, class CharT>
	void winx_call put(StringT& dest, const CharT* fmt)
	{
		const CharT* p;

Retry:	p = tchar::strchr(fmt, (CharT)'%');
		if (p == NULL)
		{
			NS_STDEXT_TEXT::append(dest, fmt);
		}
		else
		{
			++p;
			NS_STDEXT_TEXT::append(dest, fmt, p);
			if (*p == (CharT)'%')
				++p;

			fmt = p;
			goto Retry;
		}
	}
}

// -------------------------------------------------------------------------
// http://www.cplusplus.com/reference/clibrary/cstdio/print/

template <class StringT, class CharT>
inline void winx_call formatAppend(StringT& dest, const CharT* fmt)
{
	format_detail::put(dest, fmt);
}

template <class StringT, class CharT, class ArgT1>
void winx_call formatAppend(StringT& dest, const CharT* fmt, const ArgT1& arg1)
{
	fmt = format_detail::put(dest, fmt, arg1);
	format_detail::put(dest, fmt);
}

template <class StringT, class CharT, class ArgT1, class ArgT2>
void winx_call formatAppend(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	format_detail::put(dest, fmt);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3>
void winx_call formatAppend(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	format_detail::put(dest, fmt);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	fmt = format_detail::put(dest, fmt, arg4);
	format_detail::put(dest, fmt);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	fmt = format_detail::put(dest, fmt, arg4);
	fmt = format_detail::put(dest, fmt, arg5);
	format_detail::put(dest, fmt);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	fmt = format_detail::put(dest, fmt, arg4);
	fmt = format_detail::put(dest, fmt, arg5);
	fmt = format_detail::put(dest, fmt, arg6);
	format_detail::put(dest, fmt);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	fmt = format_detail::put(dest, fmt, arg4);
	fmt = format_detail::put(dest, fmt, arg5);
	fmt = format_detail::put(dest, fmt, arg6);
	fmt = format_detail::put(dest, fmt, arg7);
	format_detail::put(dest, fmt);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	fmt = format_detail::put(dest, fmt, arg4);
	fmt = format_detail::put(dest, fmt, arg5);
	fmt = format_detail::put(dest, fmt, arg6);
	fmt = format_detail::put(dest, fmt, arg7);
	fmt = format_detail::put(dest, fmt, arg8);
	format_detail::put(dest, fmt);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8, class ArgT9>
void winx_call formatAppend(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4,
	const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8, const ArgT9& arg9)
{
	fmt = format_detail::put(dest, fmt, arg1);
	fmt = format_detail::put(dest, fmt, arg2);
	fmt = format_detail::put(dest, fmt, arg3);
	fmt = format_detail::put(dest, fmt, arg4);
	fmt = format_detail::put(dest, fmt, arg5);
	fmt = format_detail::put(dest, fmt, arg6);
	fmt = format_detail::put(dest, fmt, arg7);
	fmt = format_detail::put(dest, fmt, arg8);
	fmt = format_detail::put(dest, fmt, arg9);
	format_detail::put(dest, fmt);
}

// -------------------------------------------------------------------------

template <class StringT, class CharT>
inline void winx_call format(StringT& dest, const CharT* fmt)
{
	NS_STDEXT_TEXT::clear(dest);
	format_detail::put(dest, fmt);
}

template <class StringT, class CharT, class ArgT1>
inline void winx_call format(StringT& dest, const CharT* fmt, const ArgT1& arg1)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1);
}

template <class StringT, class CharT, class ArgT1, class ArgT2>
inline void winx_call format(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3>
inline void winx_call format(StringT& dest, const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4>
inline void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4);
}

template <class StringT, class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5>
inline void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6>
inline void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7>
inline void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8>
inline void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template <
	class StringT, class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8, class ArgT9>
inline void winx_call format(
	StringT& dest, const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4,
	const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8, const ArgT9& arg9)
{
	NS_STDEXT_TEXT::clear(dest);
	formatAppend(dest, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

// -------------------------------------------------------------------------

template <class CharT>
inline void winx_call print(const CharT* fmt)
{
	format_detail::put(*stdout, fmt);
}

template <class CharT, class ArgT1>
inline void winx_call print(const CharT* fmt, const ArgT1& arg1)
{
	formatAppend(*stdout, fmt, arg1);
}

template <class CharT, class ArgT1, class ArgT2>
inline void winx_call print(const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2)
{
	formatAppend(*stdout, fmt, arg1, arg2);
}

template <class CharT, class ArgT1, class ArgT2, class ArgT3>
inline void winx_call print(const CharT* fmt, const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3);
}

template <class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4>
inline void winx_call print(
	const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3, arg4);
}

template <class CharT, class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5>
inline void winx_call print(
	const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3, arg4, arg5);
}

template <
	class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6>
inline void winx_call print(
	const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2,
	const ArgT3& arg3, const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3, arg4, arg5, arg6);
}

template <
	class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7>
inline void winx_call print(
	const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

template <
	class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8>
inline void winx_call print(
	const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3,
	const ArgT4& arg4, const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

template <
	class CharT,
	class ArgT1, class ArgT2, class ArgT3, class ArgT4, class ArgT5, class ArgT6, class ArgT7, class ArgT8, class ArgT9>
inline void winx_call print(
	const CharT* fmt,
	const ArgT1& arg1, const ArgT2& arg2, const ArgT3& arg3, const ArgT4& arg4,
	const ArgT5& arg5, const ArgT6& arg6, const ArgT7& arg7, const ArgT8& arg8, const ArgT9& arg9)
{
	formatAppend(*stdout, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_FORMAT_PRINTF_H */
