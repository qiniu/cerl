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
// Module: stdext/msvcrt/atlconv.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 14:55:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MSVCRT_ATLCONV_H
#define STDEXT_MSVCRT_ATLCONV_H

#ifndef _INC_WCHAR
#include <wchar.h>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif

#if defined(STD_NO_WINSDK)
#ifndef STDEXT_WINAPI_WINNLS_H
#include "../winapi/winnls.h"
#endif
#endif

// -------------------------------------------------------------------------
// winx_A2WHelper_/winx_W2AHelper_

#if defined(X_OS_WINDOWS)

inline LPWSTR winx_call winx_A2WHelper_(LPWSTR lpw, LPCSTR lpa, int nChars)
{
	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	lpw[0] = '\0';
	MultiByteToWideChar(
		CP_ACP, 0, lpa, nChars /* nAnsiChar */, lpw, nChars);
	return lpw;
}

inline LPSTR winx_call winx_W2AHelper_(LPSTR lpa, LPCWSTR lpw, int nWideChars)
{
	// verify that no illegal character present
	// since lpa was allocated based on the size of lpw
	// don't worry about the number of chars
	lpa[0] = '\0';
	WideCharToMultiByte(
		CP_ACP, 0, lpw, nWideChars, lpa, nWideChars<<1 /* nAnsiChar */, NULL, NULL);
	return lpa;
}

#else

#ifndef assert
#include <assert.h>
#endif

__forceinline LPWSTR winx_call winx_A2WHelper_(LPWSTR lpw, LPCSTR lpa, int nChars)
{
	assert(lpa[nChars-1] == 0);
	mbstowcs(lpw, lpa, nChars);
	return lpw;
}

inline LPSTR winx_call winx_W2AHelper_(LPSTR lpa, LPCWSTR lpw, int nWideChars)
{
	assert(lpw[nWideChars-1] == 0);
	wcstombs(lpa, lpw, nWideChars);
	return lpa;
}

#endif

// -------------------------------------------------------------------------
// WINX_A2W/WINX_W2A

#if defined(_MSC_VER)
#define WINX_USES_CONVERSION												\
	int _convert; _convert; LPCWSTR _lpw; _lpw; LPCSTR _lpa; _lpa
#else
#define WINX_USES_CONVERSION												\
	int _convert; _convert = 0; LPCWSTR _lpw; _lpw = 0; LPCSTR _lpa; _lpa = 0
#endif

#define WINX_A2W(lpa)	\
	( _convert = (int)(strlen(_lpa = lpa)+1), \
		winx_A2WHelper_((LPWSTR)_alloca(_convert<<1), _lpa, _convert) )

#define WINX_W2A(lpw)	\
	( _convert = (int)(wcslen(_lpw = lpw)+1), \
		winx_W2AHelper_((LPSTR)_alloca(_convert<<1), _lpw, _convert) )

#define WINX_A2CW(lpa) ((LPCWSTR)WINX_A2W(lpa))
#define WINX_W2CA(lpw) ((LPCSTR)WINX_W2A(lpw))

// -------------------------------------------------------------------------
// ATL_A2W/ATL_W2A

#define ATL_USES_CONVERSION \
	WINX_USES_CONVERSION

#define ATL_A2W(lpa) (\
	((_lpa = lpa) == NULL) ? NULL : (\
		_convert = (int)(strlen(_lpa)+1),\
		winx_A2WHelper_((LPWSTR)_alloca(_convert<<1), _lpa, _convert)))

#define ATL_W2A(lpw) (\
	((_lpw = lpw) == NULL) ? NULL : (\
		_convert = (int)(wcslen(_lpw)+1), \
		winx_W2AHelper_((LPSTR)_alloca(_convert<<1), _lpw, _convert)))

#define ATL_A2CW(lpa) ((LPCWSTR)ATL_A2W(lpa))
#define ATL_W2CA(lpw) ((LPCSTR)ATL_W2A(lpw))

// -------------------------------------------------------------------------
// WINX_T2A/WINX_A2T/WINX_T2W/WINX_W2T
// WINX_T2CA/WINX_A2CT/WINX_T2CW/WINX_W2CT

#ifdef _UNICODE
	#define WINX_T2A WINX_W2A
	#define WINX_A2T WINX_A2W
	#define WINX_T2CA WINX_W2CA
	#define WINX_A2CT WINX_A2CW
	inline LPWSTR WINX_T2W(LPTSTR lp) { return lp; }
	inline LPTSTR WINX_W2T(LPWSTR lp) { return lp; }
	inline LPCWSTR WINX_T2CW(LPCTSTR lp) { return lp; }
	inline LPCTSTR WINX_W2CT(LPCWSTR lp) { return lp; }
#else
	#define WINX_T2W WINX_A2W
	#define WINX_W2T WINX_W2A
	#define WINX_T2CW WINX_A2CW
	#define WINX_W2CT WINX_W2CA
	inline LPSTR WINX_T2A(LPTSTR lp) { return lp; }
	inline LPTSTR WINX_A2T(LPSTR lp) { return lp; }
	inline LPCSTR WINX_T2CA(LPCTSTR lp) { return lp; }
	inline LPCTSTR WINX_A2CT(LPCSTR lp) { return lp; }
#endif

// -------------------------------------------------------------------------
// ATL_T2A/ATL_A2T/ATL_T2W/ATL_W2T
// ATL_T2CA/ATL_A2CT/ATL_T2CW/ATL_W2CT

#ifdef _UNICODE
	#define ATL_T2A ATL_W2A
	#define ATL_A2T ATL_A2W
	#define ATL_T2CA ATL_W2CA
	#define ATL_A2CT ATL_A2CW
	inline LPWSTR ATL_T2W(LPTSTR lp) { return lp; }
	inline LPTSTR ATL_W2T(LPWSTR lp) { return lp; }
	inline LPCWSTR ATL_T2CW(LPCTSTR lp) { return lp; }
	inline LPCTSTR ATL_W2CT(LPCWSTR lp) { return lp; }
#else
	#define ATL_T2W ATL_A2W
	#define ATL_W2T ATL_W2A
	#define ATL_T2CW ATL_A2CW
	#define ATL_W2CT ATL_W2CA
	inline LPSTR ATL_T2A(LPTSTR lp) { return lp; }
	inline LPTSTR ATL_A2T(LPSTR lp) { return lp; }
	inline LPCSTR ATL_T2CA(LPCTSTR lp) { return lp; }
	inline LPCTSTR ATL_A2CT(LPCSTR lp) { return lp; }
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_MSVCRT_ATLCONV_H */
