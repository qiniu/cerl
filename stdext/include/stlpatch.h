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
// Module: stlpatch.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-22 18:01:56
// 
// $Id: stlpatch.h,v 1.1 2006/12/22 10:19:55 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STLPATCH_H
#define STLPATCH_H

#ifndef STDEXT_PLATFORM_H
#include "stdext/Platform.h"
#endif

#if !defined(_STDLIB_H) && !defined(_INC_STDLIB)
#include <stdlib.h>
#endif

// -------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#ifndef STDEXT_PATCH_STRING_H
#include "stdext/patch/string.h"
#endif

namespace std {

// -------------------------------------------------------------------------
// iterToPointer - convert a vector/string iterator to pointer

#if !defined(_MSC_VER)

#ifndef __forceinline
#define __forceinline inline
#endif

#endif

#if defined(X_STL_NET) // visual c++ .net

#if _MSC_VER < 1600

template <class _It>
__forceinline typename _It::pointer iterToPointer(_It it)
	{return (typename _It::pointer)it._Myptr; }

#else // _MSC_VER = 1600: visual c++ 2010

template <class _It>
__forceinline typename _It::pointer iterToPointer(_It it)
	{return (typename _It::pointer)it._Ptr; }

#endif

template <class _Ty>
__forceinline _Ty* iterToPointer(_Ty* it)
	{return it;}

template <class _Ty>
__forceinline const _Ty* iterToPointer(const _Ty* it)
	{return it;}

#elif defined(X_STL_GCC) || defined(X_STL_SGI) // sgi-stl

template <class _It>
__forceinline typename _It::pointer iterToPointer(_It it)
	{return &*it; }

template <class _Ty>
__forceinline _Ty* iterToPointer(_Ty* it)
	{return it;}

template <class _Ty>
__forceinline const _Ty* iterToPointer(const _Ty* it)
	{return it;}

#else

template <class _It>
__forceinline _It iterToPointer(_It it)
	{return it; }

#endif

#define _ConvIt	iterToPointer

// -------------------------------------------------------------------------
// resize

template <class _Container>
__forceinline 
typename _Container::value_type* resize(_Container& container, size_t newSize)
	{container.resize(newSize);
	 return iterToPointer(container.begin()); }

// -------------------------------------------------------------------------
// $Log: stlpatch.h,v $

} // namespace std

#endif /* STLPATCH_H */
