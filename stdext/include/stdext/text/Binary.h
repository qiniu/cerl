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
// Module: stdext/text/Binary.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2010-7-22 22:56:07
// 
// $Id: Binary.h,v 1.1 2010-7-22 22:56:07 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_BINARY_H
#define STDEXT_TEXT_BINARY_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../CharType.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// function bin2hex, hex2bin

inline char* winx_call bin2hex(
	char* buf /* bytes * 2 */, const BYTE* bin /* bytes */, const size_t bytes)
{
	const char* const hex = "0123456789abcdef";
	for (const BYTE* binEnd = bin + bytes; bin != binEnd; ++bin)
	{
		*buf++ = hex[*(unsigned char*)bin >> 4];
		*buf++ = hex[*bin & 0xf];
	}
	return buf;
}

inline bool winx_call hex2bin(
	BYTE* bin /* bytes */, const size_t bytes, const char* buf /* bytes * 2 */)
{
	typedef NS_STDEXT::DigitTable DigitTable;

	for (const BYTE* binEnd = bin + bytes; bin != binEnd; ++bin)
	{
		const unsigned c1 = DigitTable::toDigit(*buf++);
		const unsigned c2 = DigitTable::toDigit(*buf++);
		if (c1 >= 16 || c2 >= 16)
			return false;
		*bin = (c1 << 4) | c2;
	}
	return true;
}

// -------------------------------------------------------------------------
// function bin2hexStr

inline const char* winx_call bin2hexStr(
	char* buf /* bytes * 2 + 1 */, const BYTE* bin /* bytes */, const size_t bytes)
{
	*bin2hex(buf, bin, bytes) = '\0';
	return buf;
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_BINARY_H */
