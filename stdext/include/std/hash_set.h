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
// Module: std/hash_set.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: hash_set.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STD_HASH_SET_H
#define STD_HASH_SET_H

#ifndef STDEXT_PLATFORM_H
#include "../stdext/Platform.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_STL_NET)
	#ifndef _HASH_SET_
	#include <hash_set>
	#endif
#elif defined(X_STL_GCC)
	#ifndef _HASH_SET
	#include <ext/hash_set>
	#endif
#elif defined(NO_SGISTL)
	#define WINX_NO_HASH_SET_
#else
	#ifndef __SGI_HASH_SET_H__
	#include "../stdext/sgi/hash_set.h"
	#endif
#endif

// -------------------------------------------------------------------------
// $Log: hash_set.h,v $

#endif /* STD_HASH_SET_H */
