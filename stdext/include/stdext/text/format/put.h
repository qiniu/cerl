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
// Module: stdext/text/format/put.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: printf.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_PUT_H
#define STDEXT_TEXT_FORMAT_PUT_H

#ifndef STDEXT_TEXT_APPEND_H
#include "../Append.h"
#endif

#ifndef STDEXT_TEXT_STRINGALGO_H
#include "../StringAlgo.h"
#endif

#ifndef STDEXT_INT_TRAITS_H
#include "../../int/Traits.h"
#endif

#ifndef STDEXT_TCHAR_H
#include "../../tchar.h"
#endif

#ifndef STDEXT_TEXT_FORMAT_PRINTYPE_H
#include "printype.h"
#endif

#ifndef NS_STDEXT_FORMAT
#define NS_STDEXT_FORMAT			NS_STDEXT::ns_format
#define NS_STDEXT_FORMAT_BEGIN		namespace NS_STDEXT { namespace ns_format {
#define NS_STDEXT_FORMAT_END		} }
#endif

NS_STDEXT_FORMAT_BEGIN

// -------------------------------------------------------------------------

struct FormatParams
{
	unsigned flags;
	unsigned specifier;
	int width;
	int precision;
};

template <class StringT, class CharT>
const CharT* winx_call getFormatParams(FormatParams& params, StringT& dest, const CharT* fmt)
{
	//
	// Flags
	//
	params.flags = 0;
	for (;;)
	{
		const unsigned flag = PrinType::is(STD_PRINTYPE_FLAGS, *fmt);
		if (flag)
		{
			params.flags |= flag;
			++fmt;
		}
		else
		{
			break;
		}
	}

	//
	// Width
	//
	params.width = 0;
	while (*fmt >= '0' && *fmt <= '9')
	{
		params.width = params.width * 10 + (*fmt - '0');
		++fmt;
	}

	//
	// .Precision
	//
	if (*fmt == '.')
	{
		++fmt;
		params.precision = 0;
		while (*fmt >= '0' && *fmt <= '9')
		{
			params.precision = params.precision * 10 + (*fmt - '0');
			++fmt;
		}
	}
	else
	{
		params.precision = 1;
	}

	//
	// Length
	//
	while (PrinType::is(STD_PRINTYPE_LENGTH, *fmt))
		++fmt;

	//
	// Specifier
	//
	params.specifier = PrinType::is(STD_PRINTYPE_SPECIFIER, *fmt);
	if (params.specifier)
		++fmt;

	return fmt;
}

// -------------------------------------------------------------------------

#define STD_PRINTYPE_FLAGS_NEGATIVE STD_PRINTYPE_RESERVED

template <class StringT, class CharT>
void winx_call formatVal(StringT& dest, const FormatParams& params, const CharT* text, const CharT* textEnd)
{
    CharT prefix[2];
	/* numeric prefix -- up to two characters */
    
	int prefixlen = 0;
	/* length of prefix -- 0 means no prefix */

    int padding;
	/* amount of padding, negative means zero */

	const unsigned flags = params.flags;

    if (params.specifier & (STD_PRINTYPE_F | STD_PRINTYPE_I_DEC))
	{
        if (flags & STD_PRINTYPE_FLAGS_NEGATIVE)
		{
            /* prefix is a '-' */
            prefix[0] = '-';
            prefixlen = 1;
        }
        else if (flags & (STD_PRINTYPE_FLAGS_ADD | STD_PRINTYPE_FLAGS_SPACE))
		{
            /* prefix is '+' or ' ' */
            prefix[0] = ((flags & STD_PRINTYPE_FLAGS_ADD) ? '+' : ' ');
            prefixlen = 1;
        }
    }
	else if (flags & STD_PRINTYPE_FLAGS_SHARP)
	{
		if (params.specifier & STD_PRINTYPE_I_HEX)
		{
			/* alternate form means '0x' prefix */
			prefix[0] = '0';
			prefix[1] = (params.specifier & STD_PRINTYPE_I_HEX_UPPER) ? 'X' : 'x';
			prefixlen = 2;
		}
		else if (params.specifier & STD_PRINTYPE_I_OCT)
		{
			/* alternate form means '0' prefix */
			prefix[0] = '0';
			prefixlen = 1;
		}
	}

    /*
	 *  calculate amount of padding
	 *  -- might be negative
     *  but this will just mean zero
	 */
    padding = params.width - (int)(textEnd - text) - prefixlen;

    /* put out the padding, prefix, and text */

	if (padding > 0 && !(flags & (STD_PRINTYPE_FLAGS_SUB | STD_PRINTYPE_FLAGS_ZERO)))
	{
		/* pad on left with blanks */
		NS_STDEXT_TEXT::append(dest, padding, (CharT)' ');
	}

	/* write prefix */
	if (prefixlen > 0)
		NS_STDEXT_TEXT::append(dest, prefix, prefix + prefixlen);

	if (padding > 0 && (flags & STD_PRINTYPE_FLAGS_ZERO) && !(flags & STD_PRINTYPE_FLAGS_SUB))
	{
		/* write leading zeros */
		NS_STDEXT_TEXT::append(dest, padding, (CharT)'0');
	}

	/* write text */
	NS_STDEXT_TEXT::append(dest, text, textEnd);

	if (padding > 0 && (flags & STD_PRINTYPE_FLAGS_SUB))
	{
		/* pad on right with blanks */
		NS_STDEXT_TEXT::append(dest, padding, (CharT)' ');
	}
}

template <class StringT, class CharT>
inline void winx_call formatString(StringT& dest, const FormatParams& params, const CharT* text, const CharT* textEnd)
{
	const unsigned flags = params.flags;

    /*
	 *  calculate amount of padding
	 *  -- might be negative
     *  but this will just mean zero
	 */
	const int padding = params.width - (int)(textEnd - text);

    /* put out the padding, prefix, and text */

	if (padding > 0 && !(flags & STD_PRINTYPE_FLAGS_SUB))
	{
		/* pad on left with blanks */
		NS_STDEXT_TEXT::append(dest, padding, (CharT)' ');
	}

	/* write text */
	NS_STDEXT_TEXT::append(dest, text, textEnd);

	if (padding > 0 && (flags & STD_PRINTYPE_FLAGS_SUB))
	{
		/* pad on right with blanks */
		NS_STDEXT_TEXT::append(dest, padding, (CharT)' ');
	}
}

template <class CharT, class UIntT>
const CharT* winx_call formatUInt(CharT* textEnd, UIntT val, unsigned specifier, int precision = 1)
{
	int radix;
	const char* dig = "0123456789abcdef";
	
	if (specifier & (STD_PRINTYPE_I_DEC | STD_PRINTYPE_I_UDEC))
	{
		radix = 10;
	}
	else if (specifier & STD_PRINTYPE_I_HEX)
	{
		radix = 16;
		if (specifier & STD_PRINTYPE_I_HEX_UPPER)
			dig = "0123456789ABCDEF";
	}
	else
	{
		radix = 8;
	}

	while (precision-- > 0 || val != 0)
	{
		*--textEnd = (CharT)dig[val % radix];
		val /= radix;
	}

	return textEnd;
}

template <class CharT>
struct Utils
{
	template <class StringT>
	static void winx_call formatPtr(StringT& dest, const void* val)
	{
		CharT buf[64];
		const CharT* text = formatUInt(
			buf + countof(buf), (size_t)val, STD_PRINTYPE_I_HEX, 8);
		NS_STDEXT_TEXT::append(dest, text, buf + countof(buf));
	}
};

// -------------------------------------------------------------------------
// format Integer

template <class StringT, class CharT, class IntT>
const CharT* winx_call putInt(StringT& dest, const CharT* fmt, IntT val, unsigned specifierAny)
{
	typedef NS_STDEXT::IntTypeTraits<IntT> Traits;
	typedef typename Traits::signed_type signed_type;
	typedef typename Traits::unsigned_type unsigned_type;

	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);
	WINX_ASSERT(params.specifier & (STD_PRINTYPE_I | STD_PRINTYPE_CHAR | STD_PRINTYPE_ANY));

lzRetry:
	if (params.specifier & STD_PRINTYPE_I)
	{
		CharT buf[64];
		const CharT* text;

		if (params.specifier & STD_PRINTYPE_I_DEC)
		{
			if (val < 0)
			{
				params.flags |= STD_PRINTYPE_FLAGS_NEGATIVE;
				val = -val;
			}
			text = formatUInt(buf + countof(buf), (signed_type)val, params.specifier, params.precision);
		}
		else
		{
			text = formatUInt(buf + countof(buf), (unsigned_type)val, params.specifier, params.precision);
		}

		formatVal(dest, params, text, buf + countof(buf));
	}
	else if (params.specifier & STD_PRINTYPE_CHAR)
	{
		NS_STDEXT_TEXT::append(dest, (CharT)val);
	}
	else if (params.specifier & STD_PRINTYPE_ANY)
	{
		WINX_ASSERT(!(specifierAny & STD_PRINTYPE_ANY));
		params.specifier = specifierAny;
		goto lzRetry;
	}
	
	return fmt;
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, INT64 val)
{
	return putInt(dest, fmt, val, STD_PRINTYPE_I_DEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, UINT64 val)
{
	return putInt(dest, fmt, (INT64)val, STD_PRINTYPE_I_UDEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, long val)
{
	return putInt(dest, fmt, (long)val, STD_PRINTYPE_I_DEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, unsigned long val)
{
	return putInt(dest, fmt, (long)val, STD_PRINTYPE_I_UDEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, unsigned int val)
{
	return putInt(dest, fmt, (int)val, STD_PRINTYPE_I_UDEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, int val)
{
	return putInt(dest, fmt, val, STD_PRINTYPE_I_DEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, unsigned short val)
{
	return putInt(dest, fmt, (int)val, STD_PRINTYPE_I_UDEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, short val)
{
	return putInt(dest, fmt, (int)val, STD_PRINTYPE_I_DEC);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, unsigned char val)
{
	return putInt(dest, fmt, (int)val, STD_PRINTYPE_CHAR);
}

template <class StringT, class CharT>
inline const CharT* winx_call put(StringT& dest, const CharT* fmt, char val)
{
	return putInt(dest, fmt, (int)val, STD_PRINTYPE_CHAR);
}

// -------------------------------------------------------------------------
// format Pointer

template <class StringT, class CharT>
const CharT* winx_call put(StringT& dest, const CharT* fmt, const void* val)
{
	WINX_ASSERT(*fmt == 'p');

	if (*fmt == 'p')
	{
		++fmt;
		Utils<CharT>::formatPtr(dest, val);
	}
	return fmt;
}

// -------------------------------------------------------------------------
// format String

template <class StringT, class CharT>
const CharT* winx_call putString(StringT& dest, const CharT* fmt, const CharT* text, const CharT* textEnd)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);
	WINX_ASSERT(params.specifier & (STD_PRINTYPE_STRING | STD_PRINTYPE_ANY));

	formatString(dest, params, text, textEnd);
	return fmt;
}

template <class StringT, class CharT>
const CharT* winx_call put(StringT& dest, const CharT* fmt, const CharT* val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);
	WINX_ASSERT(params.specifier & (STD_PRINTYPE_STRING | STD_PRINTYPE_PTR | STD_PRINTYPE_ANY));

	if (params.specifier & (STD_PRINTYPE_STRING | STD_PRINTYPE_ANY))
		formatString(dest, params, val, NS_STDEXT::end(val));
	else if (params.specifier & STD_PRINTYPE_PTR)
		Utils<CharT>::formatPtr(dest, val);

	return fmt;
}

template <class StringT>
inline const char* winx_call put(StringT& dest, const char* fmt, const String& val)
{
	return putString(dest, fmt, val.begin(), val.end());
}

template <class StringT>
inline const char* winx_call put(StringT& dest, const char* fmt, const WString& val)
{
	StringBuilder s;
	iconv(val.data(), val.size(), cp_auto, s);
	return putString(dest, fmt, std::iterToPointer(s.begin()), std::iterToPointer(s.end()));
}

template <class StringT>
inline const wchar_t* winx_call put(StringT& dest, const wchar_t* fmt, const WString& val)
{
	return putString(dest, fmt, val.begin(), val.end());
}

// -------------------------------------------------------------------------
// format Float-Pointer

template <class StringT>
const char* winx_call put(StringT& dest, const char* fmt, const double val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);

	char buf[64];
	_gcvt(val, 12, buf);
	NS_STDEXT_TEXT::append(dest, buf);
	
	return fmt;
}

template <class StringT>
const wchar_t* winx_call put(StringT& dest, const wchar_t* fmt, const double val)
{
	FormatParams params;
	fmt = getFormatParams(params, dest, fmt);
	
	char buf[64];
	wchar_t wbuf[64];
	_gcvt(val, 12, buf);
	for (size_t i = 0; (wbuf[i] = buf[i]) != '\0'; ++i);
	NS_STDEXT_TEXT::append(dest, wbuf);
	
	return fmt;
}

// -------------------------------------------------------------------------
// format GUID

template <class StringT>
const char* winx_call put(StringT& dest, const char* fmt, const GUID& val)
{
	WINX_ASSERT(*fmt == 'a');

	if (*fmt == 'a')
		++fmt;

	char str[40];
	char* strEnd = StringFromGUID(str, val);
	NS_STDEXT_TEXT::append(dest, str, strEnd);

	return fmt;
}

// -------------------------------------------------------------------------

NS_STDEXT_FORMAT_END

#endif /* STDEXT_TEXT_FORMAT_PUT_H */
