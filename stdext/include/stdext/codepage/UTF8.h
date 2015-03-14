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
// Module: stdext/codepage/UTF8.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: UTF8.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_CODEPAGE_UTF8_H
#define STDEXT_CODEPAGE_UTF8_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

#if defined(X_ENCODE_UCS2)
#define WINX_UCS_BYTES	2
#elif defined(X_ENCODE_UCS4)
#define WINX_UCS_BYTES	4
#else
#error "Unknown UCS Bytes!!!"
#endif

// -------------------------------------------------------------------------

/*
 * From RFC 2044:
 *
 * UCS-4 range (hex.)           UTF-8 octet sequence (binary)
 * 0000 0000 - 0000 007F   			0xxxxxxx
 * 0000 0080 - 0000 07FF   			110xxxxx 10xxxxxx
 * 0000 0800 - 0000 FFFF   			1110xxxx 10xxxxxx 10xxxxxx
 * 0001 0000 - 001F FFFF   			11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 0020 0000 - 03FF FFFF   			111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 0400 0000 - 7FFF FFFF   			1111110x 10xxxxxx ... 10xxxxxx
 */

#define WINX_UTF8_CHAR_BYTES		6
#define WINX_UTF8_CONTINUE_BYTE(b) 	(((b) & 0x3F) | 0x80)

struct UTF8
{
	static UINT8* winx_call fromUCS2(const UINT16 in, UINT8* outBuf)
	{
		if (in < 0x80) {
			*outBuf++ = (UINT8)in;
			goto lzRet;
		}
		else if (in < 0x800) {
			*outBuf++ = (UINT8)((in >> 6) | 0xC0);
			goto lz00;
		}
		else {
			*outBuf++ = (UINT8)((in >> 12) | 0xE0);
		}
		*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in >> 6);
lz00:	*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in);
lzRet:	return outBuf;
	}

	static UINT8* winx_call fromUCS4(const UINT32 in, UINT8* outBuf)
	{
		WINX_ASSERT(in < 0x80000000);

		if (in < 0x80) {
			*outBuf++ = (UINT8)in;
			goto lzRet;
		}
		else if (in < 0x800) {
			*outBuf++ = (UINT8)((in >> 6) | 0xC0);
			goto lz00;
		}
		else if (in < 0x10000) { // 16
			*outBuf++ = (UINT8)((in >> 12) | 0xE0);
			goto lz06;
		}
		else if (in < 0x200000) { // 21
			*outBuf++ = (UINT8)((in >> 18) | 0xF0);
			goto lz12;
		}
		else if (in < 0x4000000) { // 26
			*outBuf++ = (UINT8)((in >> 24) | 0xF8);
			goto lz18;
		}
		else if (in < 0x80000000) { // 31
			*outBuf++ = (UINT8)((in >> 30) | 0xFC);
		}
		else {
			*outBuf++ = '?';
			goto lzRet;
		}
		*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in >> 24);
lz18:	*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in >> 18);
lz12:	*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in >> 12);
lz06:	*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in >> 6);
lz00:	*outBuf++ = (UINT8)WINX_UTF8_CONTINUE_BYTE(in);
lzRet:	return outBuf;
	}
	
	static UINT8* winx_call fromW(const WCHAR in, UINT8* outBuf)
	{
		WINX_ASSERT(sizeof(WCHAR) == WINX_UCS_BYTES);
		#if (WINX_UCS_BYTES == 2)
			return fromUCS2(in, outBuf);
		#elif (WINX_UCS_BYTES == 4)
			return fromUCS4(in, outBuf);
		#endif
	}
};

// -------------------------------------------------------------------------
// $Log: UTF8.h,v $

NS_STDEXT_END

#endif /* STDEXT_CODEPAGE_UTF8_H */
