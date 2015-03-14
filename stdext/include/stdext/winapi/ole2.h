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
// Module: stdext/winapi/ole2.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_OLE2_H
#define STDEXT_WINAPI_OLE2_H

#ifndef STDEXT_WINAPI_OBJBASE_H
#include "objbase.h"
#endif

#ifndef STDEXT_WINAPI_OLEAUTO_H
#include "oleauto.h"
#endif

#define WINOLEAPI        inline HRESULT STDAPICALLTYPE
#define WINOLEAPI_(type) inline type STDAPICALLTYPE

// -------------------------------------------------------------------------
/* Storage Utility APIs */

WINOLEAPI GetHGlobalFromILockBytes(LPLOCKBYTES plkbyt, HGLOBAL FAR* phglobal)
{
	NotImplementFeature("GetHGlobalFromILockBytes");
	return S_OK;
}

WINOLEAPI CreateILockBytesOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease,
									 LPLOCKBYTES FAR* pplkbyt)
{
	NotImplementFeature("CreateILockBytesOnHGlobal");
	return S_OK;
}

WINOLEAPI GetHGlobalFromStream(LPSTREAM pstm, HGLOBAL FAR* phglobal)
{
	NotImplementFeature("GetHGlobalFromStream");
	return S_OK;
}

WINOLEAPI CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease,
								 LPSTREAM FAR* ppstm)
{
	NotImplementFeature("CreateStreamOnHGlobal");
	return S_OK;
}

// -------------------------------------------------------------------------
/* Clipboard APIs */

interface IDataObject;

WINOLEAPI  OleSetClipboard(IDataObject* pDataObj)
{
	NotImplementFeature("OleSetClipboard");
	return S_OK;
}

WINOLEAPI  OleGetClipboard(IDataObject** ppDataObj)
{
	NotImplementFeature("OleGetClipboard");
	return S_OK;
}

WINOLEAPI  OleFlushClipboard()
{
	NotImplementFeature("OleFlushClipboard");
	return S_OK;
}

WINOLEAPI  OleIsCurrentClipboard(IDataObject* pDataObj)
{
	NotImplementFeature("OleIsCurrentClipboard");
	return S_OK;
}

// -------------------------------------------------------------------------
/* Drag/Drop APIs */

interface IDataObject;
interface IDropSource;

WINOLEAPI  DoDragDrop(
	IDataObject* pDataObj,
	IDropSource* pDropSource,
	DWORD dwOKEffects,
	LPDWORD pdwEffect)
{
	NotImplementFeature("DoDragDrop");
	return S_OK;
}

// =========================================================================
// $Log: $

#endif /* STDEXT_WINAPI_OLE2_H */
