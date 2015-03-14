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
// Module: stdext/winapi/shlwapi.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_SHLWAPI_H
#define STDEXT_WINAPI_SHLWAPI_H

#define LWSTDAPI          inline HRESULT STDAPICALLTYPE
#define LWSTDAPI_(type)   inline type STDAPICALLTYPE

// =========================================================================

LWSTDAPI_(LPSTR) PathFindExtensionA(LPCSTR pszPath)
{
	NotImplementFeature("PathFindExtensionA");
	return NULL;
}

// =========================================================================

LWSTDAPI SHCreateStreamOnFileA(LPCSTR pszFile, DWORD grfMode, struct IStream **ppstm)
{
	NotImplementFeature("SHCreateStreamOnFileA");
	return S_OK;
}

LWSTDAPI SHCreateStreamOnFileW(LPCWSTR pszFile, DWORD grfMode, struct IStream **ppstm)
{
	NotImplementFeature("SHCreateStreamOnFileW");
	return S_OK;
}

// =========================================================================
// $Log: $

#endif /* STDEXT_WINAPI_SHLWAPI_H */
