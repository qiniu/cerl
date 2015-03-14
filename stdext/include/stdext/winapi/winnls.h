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
// Module: stdext/winapi/winnls.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 14:55:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_WINNLS_H
#define STDEXT_WINAPI_WINNLS_H

#ifndef STDEXT_WINAPI_WINBASE_H
#include "winbase.h"
#endif

// -------------------------------------------------------------------------
//
//  Code Page Default Values.
//
#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation

// -------------------------------------------------------------------------

inline int WINAPI MultiByteToWideChar(
    UINT     CodePage,
    DWORD    dwFlags,
    LPCSTR   lpMultiByteStr,
    int      cchMultiByte,
    LPWSTR   lpWideCharStr,
    int      cchWideChar)
{
	NotImplementFeature("MultiByteToWideChar");
	return 0;
}

inline int WINAPI WideCharToMultiByte(
    UINT     CodePage,
    DWORD    dwFlags,
    LPCWSTR  lpWideCharStr,
    int      cchWideChar,
    LPSTR    lpMultiByteStr,
    int      cchMultiByte,
    LPCSTR   lpDefaultChar,
    LPBOOL   lpUsedDefaultChar)
{
	NotImplementFeature("WideCharToMultiByte");
	return 0;
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_WINAPI_WINNLS_H */
