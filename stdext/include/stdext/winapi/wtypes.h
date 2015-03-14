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
// Module: stdext/winapi/wtypes.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_WTYPES_H
#define STDEXT_WINAPI_WTYPES_H

#if !defined(STD_NO_WINSDK)
#error "Don't include <stdext/winapi/wtypes.h>"
#endif

#ifndef STDEXT_WINAPI_WINDEF_H
#include "windef.h"
#endif

// =========================================================================

typedef unsigned char byte;
typedef wchar_t WCHAR;

typedef WCHAR			OLECHAR;
typedef OLECHAR*		LPOLESTR;
typedef const OLECHAR*	LPCOLESTR;
typedef OLECHAR*		BSTR;
typedef double			DATE;

// =========================================================================

#if defined(_UNICODE)
typedef WCHAR TCHAR;
#else
typedef CHAR TCHAR;
#endif

// =========================================================================

typedef /* [string] */ WCHAR __RPC_FAR *LPWSTR;

typedef /* [string] */ TCHAR __RPC_FAR *LPTSTR;

typedef /* [string] */ const WCHAR __RPC_FAR *LPCWSTR;

typedef /* [string] */ const TCHAR __RPC_FAR *LPCTSTR;

// =========================================================================

#if defined(__GNUG__)
typedef long long __int64;
typedef unsigned long long __uint64;
#endif

typedef __int64 LONG64, *PLONG64;
typedef __int64 INT64, *PINT64;

typedef __uint64 ULONG64, *PULONG64;
typedef __uint64 DWORD64, *PDWORD64;
typedef __uint64 UINT64, *PUINT64;

typedef __int64 LONGLONG;
typedef __uint64 ULONGLONG;

// =========================================================================

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    };
    struct {
        DWORD LowPart;
        DWORD HighPart;
    } u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;
typedef ULARGE_INTEGER *PULARGE_INTEGER;

// =========================================================================

#ifndef _FILETIME_
#define _FILETIME_

typedef struct  _FILETIME
    {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
    }	FILETIME;

typedef struct _FILETIME __RPC_FAR *PFILETIME;
typedef struct _FILETIME __RPC_FAR *LPFILETIME;

#endif // !_FILETIME

#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_

typedef struct  _SYSTEMTIME
    {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
    }	SYSTEMTIME;

typedef struct _SYSTEMTIME __RPC_FAR *PSYSTEMTIME;
typedef struct _SYSTEMTIME __RPC_FAR *LPSYSTEMTIME;

#endif // !_SYSTEMTIME

// -------------------------------------------------------------------------

#ifndef _SECURITY_ATTRIBUTES_
#define _SECURITY_ATTRIBUTES_
typedef struct  _SECURITY_ATTRIBUTES
    {
    DWORD nLength;
    /* [size_is] */ LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
    }	SECURITY_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES __RPC_FAR *PSECURITY_ATTRIBUTES;
typedef struct _SECURITY_ATTRIBUTES __RPC_FAR *LPSECURITY_ATTRIBUTES;

#endif // !_SECURITY_ATTRIBUTES_

// =========================================================================

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;
#pragma pack()

typedef GUID IID;
typedef GUID CLSID;

typedef CLSID __RPC_FAR* LPCLSID;
typedef IID __RPC_FAR*	LPIID;

#ifdef __cplusplus
} // extern "C"
#endif

// =========================================================================

typedef const IID& REFIID;
typedef const CLSID& REFCLSID;
typedef const GUID& REFGUID;

// =========================================================================
// HANDLE

#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

DECLARE_HANDLE(HINSTANCE);

typedef PVOID HANDLE;
typedef HINSTANCE HMODULE;      /* HMODULEs can be used in place of HINSTANCEs */

typedef HANDLE HGLOBAL;
typedef HANDLE HLOCAL;

typedef /* [wire_marshal] */ void __RPC_FAR *HMETAFILEPICT;

// -------------------------------------------------------------------------

DECLARE_HANDLE(HGDIOBJ);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HCURSOR);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HSTR);
DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HWINSTA);
DECLARE_HANDLE(HKL);
DECLARE_HANDLE(HACCEL);

DECLARE_HANDLE(HGLRC);          // OpenGL
DECLARE_HANDLE(HDESK);
DECLARE_HANDLE(HMETAFILE);
DECLARE_HANDLE(HENHMETAFILE);

// -------------------------------------------------------------------------

DECLARE_HANDLE(HKEY);
typedef HKEY *PHKEY;

//
// Requested Key access mask type.
//

typedef ACCESS_MASK REGSAM;

// =========================================================================
// $Log: $

#endif /* STDEXT_WINAPI_WTYPES_H */

