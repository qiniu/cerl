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
// Module: stdext/vc/declspec.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2003-10-5 13:20:48
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_VC_DECLSPEC_H
#define STDEXT_VC_DECLSPEC_H

// =========================================================================
// function calltype

#if !defined(_MSC_VER)

#ifndef __cdecl
#define __cdecl
#endif

#ifndef __stdcall
#define __stdcall // __attribute__((stdcall))
#endif

#ifndef __fastcall
#define __fastcall // __attribute__((fastcall))
#endif

#ifndef __forceinline
#define __forceinline inline
#endif

#endif

// =========================================================================
// WINX_DEFINE_SELECTANY, WINX_SELECTANY

#define _WINX_SELECTANY_CLS(Var)		_TWinx ## Var
#define _WINX_SELECTANY(Var, type)		_TWinx ## Var<type>::_g_inst

#define WINX_DEFINE_SELECTANY(Type, Var)									\
																			\
template <class _Unused>													\
struct _WINX_SELECTANY_CLS(Var)												\
{																			\
	static Type _g_inst;													\
};																			\
																			\
template <class _Unused>													\
Type _WINX_SELECTANY(Var, _Unused)

#define WINX_SELECTANY(Var)		_WINX_SELECTANY(Var, int)

// -------------------------------------------------------------------------
// DEFINE_SELECTANY, SELECTANY

#if defined(_MSC_VER)

#define DEFINE_SELECTANY(Type, Var)	__declspec(selectany) Type Var
#define SELECTANY(Var)				Var

#else

#define DEFINE_SELECTANY(Type, Var)	WINX_DEFINE_SELECTANY(Type, Var)
#define SELECTANY(Var)				WINX_SELECTANY(Var)

#endif

// =========================================================================
// uuidof

#ifndef STDEXT_VC_UUIDOF_H
#include "uuidof.h"
#endif

#ifndef STDEXT_VC_PROPERTY_H
#include "property.h"
#endif

// =========================================================================
// $Log: $

#endif /* STDEXT_VC_DECLSPEC_H */
