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
// Module: stdext/winsdk.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 15:24:12
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINSDK_H
#define STDEXT_WINSDK_H

#ifndef STDEXT_PLATFORM_H
#include "../Platform.h"
#endif

// -------------------------------------------------------------------------

#if !defined(STD_NO_WINSDK)

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef _OBJBASE_H_
#include <objbase.h>		// for CoTaskMemAlloc, CoTaskMemFree
#endif

#else

#ifndef STDEXT_WINAPI_WTYPES_H
#include "winapi/wtypes.h"
#endif

#ifndef STDEXT_WINAPI_OBJBASE_H
#include "winapi/objbase.h"
#endif

#endif // defined(STD_NO_WINSDK)

#if !defined(_W64)
typedef signed char INT8, *PINT8;
typedef signed short INT16, *PINT16;
typedef unsigned char UINT8, *PUINT8;
typedef unsigned short UINT16, *PUINT16;
#endif

// -------------------------------------------------------------------------
// StringFromGUID

#ifndef STDEXT_MSVCRT_H
#include "msvcrt.h"
#endif

inline LPSTR StringFromGUID(LPSTR idstr /* char[37] at least */, REFGUID guid)
{
	static const char* hex = "0123456789ABCDEF";
	char* s;
	int i;

	snprintf(idstr, 32, "%08lX-%04X-%04X-%02X%02X-",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1]);

	s = &idstr[24];

	for (i = 2; i < 8; i++) {
		*s++ = hex[guid.Data4[i]>>4];
		*s++ = hex[guid.Data4[i] & 0xf];
	}

	*s = '\0';
	return s;
}

#if defined(STD_NO_WINSDK)

inline int StringFromGUID2(REFGUID guid, LPOLESTR lpsz, int cchMax)
{
	if (cchMax < 39)
		return 0;

	char buf[40];
	char* bufEnd = StringFromGUID(buf, guid);
	*lpsz++ = '{';
	for (char* p = buf; p != bufEnd; ++p)
		*lpsz++ = *p;
	*lpsz++ = '}';
	*lpsz = '\0';
	return 39;
}

#endif

// -------------------------------------------------------------------------

#endif /* STDEXT_WINSDK_H */
