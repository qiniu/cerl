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
// Module: stdext/vc/uuidof.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_VC_UUIDOF_H
#define STDEXT_VC_UUIDOF_H

// =========================================================================

#if defined(STD_NO_WINSDK)

#ifndef STDEXT_WINAPI_WTYPES_H
#include "../winapi/wtypes.h"
#endif

#else

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#endif

// =========================================================================

extern "C" {

extern const IID IID_IUnknown;
extern const IID IID_IClassFactory;
extern const IID IID_IMalloc;
extern const IID IID_IMallocSpy;
extern const IID IID_ILockBytes;
extern const IID IID_ISequentialStream;
extern const IID IID_IStream;
extern const IID IID_IStorage;
extern const IID IID_IPersistStorage;
extern const IID IID_IEnumSTATSTG;
extern const IID IID_IDispatch;
extern const IID IID_ITypeLib;
extern const IID IID_ITypeLib2;
extern const IID IID_ICreateErrorInfo;
extern const IID IID_IErrorInfo;

} // extern "C"

// =========================================================================
// WINX_RLS_DEFINE_GUID/WINX_RLS_DEFINE_GUID_ALTER
// WINX_RLS_GUIDOF

template <class TheClass>
struct _WinxGuidTraits {
};

#define WINX_RLS_DEFINE_GUID(												\
	TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)				\
																			\
template <>																	\
struct _WinxGuidTraits<TheClass> {											\
	template <class nInst>													\
	struct _GuidOf_ {														\
		static const GUID guid;												\
	};																		\
	typedef _GuidOf_<int> _Class;											\
};																			\
																			\
template <class nInst>														\
const GUID _WinxGuidTraits<TheClass>::_GuidOf_<nInst>::guid = {				\
	x, s1, s2, { c1, c2, c3, c4, c5, c6, c7, c8 }							\
};

#define WINX_RLS_DEFINE_GUID_ALTER(TheClass, guidref)						\
																			\
template <>																	\
struct _WinxGuidTraits<TheClass> {											\
	template <class nInst>													\
	struct _GuidOf_ {														\
		static const GUID& guid;											\
	};																		\
	typedef _GuidOf_<int> _Class;											\
};																			\
																			\
template <class nInst>														\
const GUID& _WinxGuidTraits<TheClass>::_GuidOf_<nInst>::guid = guidref;

#define WINX_RLS_GUIDOF(TheClass)	_WinxGuidTraits<TheClass>::_Class::guid

// =========================================================================
// WINX_DBG_VERIFY_GUID/WINX_DBG_REGISTER_GUID

#if defined(_MSC_VER)
#define WINX_DBG_VERIFY_GUID(sz, guid)										\
	do																		\
	{																		\
		struct __declspec(uuid(sz)) _CoClass;								\
		WINX_ASSERT_ONCE(__uuidof(_CoClass) == guid);						\
	}																		\
	while (0)
#else
#define WINX_DBG_VERIFY_GUID(sz, guid)
#endif

#if defined(_DEBUG)
#define WINX_DBG_REGISTER_GUID(guid, lpsz)
#else
#define WINX_DBG_REGISTER_GUID(guid, lpsz)
#endif

// -------------------------------------------------------------------------
// WINX_DBG_DEFINE_GUID/WINX_DBG_DEFINE_GUID_ALTER
// WINX_DBG_GUIDOF

template <class TheClass>
struct _WinxDbgGuidTraits {
};

#define WINX_DBG_DEFINE_GUID(												\
	TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)				\
template <>																	\
struct _WinxDbgGuidTraits<TheClass>											\
{																			\
	static const GUID& guid()												\
	{																		\
		static const GUID _guid	=											\
			{x, s1, s2, { c1, c2, c3, c4, c5, c6, c7, c8 }};				\
		WINX_DBG_REGISTER_GUID(_guid, L #TheClass);							\
		WINX_DBG_VERIFY_GUID(sz, _guid);									\
		return _guid;														\
	}																		\
};

#define WINX_DBG_DEFINE_GUID_ALTER(TheClass, guidref)						\
template <>																	\
struct _WinxDbgGuidTraits<TheClass>											\
{																			\
	static const GUID& guid()												\
	{																		\
		return guidref;														\
	}																		\
};

#define WINX_DBG_GUIDOF(TheClass)	_WinxDbgGuidTraits<TheClass>::guid()


// =========================================================================
// DEFINE_IID/DEFINE_CLSID/DEFINE_GUID_ALTER/__uuid

#if defined(_DEBUG)

#ifndef __uuid
#define __uuid(TheClass)			WINX_DBG_GUIDOF(TheClass)
#endif

#ifndef	DEFINE_IID
#define DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	interface Interface;													 \
	WINX_DBG_DEFINE_GUID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)
#endif

#ifndef DEFINE_CLSID
#define DEFINE_CLSID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	class TheClass;															  \
	WINX_DBG_DEFINE_GUID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)
#endif

#ifndef DEFINE_GUID_ALTER
#define DEFINE_GUID_ALTER(TheClass, guid)									\
	WINX_DBG_DEFINE_GUID_ALTER(TheClass, guid)
#endif

#else // !defined(_DEBUG)

#ifndef __uuid
#define __uuid(TheClass)			WINX_RLS_GUIDOF(TheClass)
#endif

#ifndef	DEFINE_IID
#define DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	interface Interface;													 \
	WINX_RLS_DEFINE_GUID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)
#endif

#ifndef DEFINE_CLSID
#define DEFINE_CLSID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	class TheClass;															  \
	WINX_RLS_DEFINE_GUID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)
#endif

#ifndef DEFINE_GUID_ALTER
#define DEFINE_GUID_ALTER(TheClass, guid)									\
	WINX_RLS_DEFINE_GUID_ALTER(TheClass, guid)
#endif

#endif // !defined(_DEBUG)

// =========================================================================
// DEFINE_IID_ALTER/DEFINE_CLSID_ALTER

#ifndef DEFINE_IID_ALTER
#define DEFINE_IID_ALTER(Interface)		DEFINE_GUID_ALTER(Interface, IID_##Interface)
#define DEFINE_CLSID_ALTER(ComClass)	DEFINE_GUID_ALTER(ComClass, CLSID_##ComClass)
#endif

#ifndef interface
#if defined(_WIN32) || defined(_WIN64)
#include <objbase.h>
#else
#define interface	struct
#endif
#endif

interface IUnknown;
interface IClassFactory;
interface IMalloc;
interface IMallocSpy;
interface ILockBytes;
interface ISequentialStream;
interface IStream;
interface IStorage;
interface IPersistStorage;
interface IEnumSTATSTG;
interface IDispatch;
interface ITypeLib;
interface ITypeLib2;
interface ICreateErrorInfo;
interface IErrorInfo;

DEFINE_IID_ALTER(IUnknown)
DEFINE_IID_ALTER(IClassFactory)
DEFINE_IID_ALTER(IMalloc)
DEFINE_IID_ALTER(IMallocSpy)
DEFINE_IID_ALTER(ILockBytes)
DEFINE_IID_ALTER(ISequentialStream)
DEFINE_IID_ALTER(IStream)
DEFINE_IID_ALTER(IStorage)
DEFINE_IID_ALTER(IPersistStorage)
DEFINE_IID_ALTER(IEnumSTATSTG)
DEFINE_IID_ALTER(IDispatch)
DEFINE_IID_ALTER(ITypeLib)
DEFINE_IID_ALTER(ITypeLib2)
DEFINE_IID_ALTER(ICreateErrorInfo)
DEFINE_IID_ALTER(IErrorInfo)

// =========================================================================
// __uuidof

#if !defined(_MSC_VER)
#define WINX_USE_DEFINE_IID
#define __uuidof(TheClass)		__uuid(TheClass)
#endif

// =========================================================================
// WINX_UUID/WINX_DEFINE_IID/WINX_DEFINE_CLSID

#if defined(WINX_USE_DEFINE_IID)

#define WINX_UUID(Interface)	__uuid(Interface)

#define WINX_DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)

#define WINX_DEFINE_CLSID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	DEFINE_CLSID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)

#else

#define WINX_UUID(Interface)	__uuidof(Interface)

#define WINX_DEFINE_IID(Interface, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	interface __declspec(uuid(sz)) Interface;

#define WINX_DEFINE_CLSID(TheClass, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8) \
	class __declspec(uuid(sz)) TheClass;

#endif

// =========================================================================
// $Log: $

#endif /* STDEXT_VC_UUIDOF_H */
