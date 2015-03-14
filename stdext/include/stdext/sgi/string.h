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
// Module: sgi/string.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: string.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_STRING_H__
#define __SGI_STRING_H__

// -------------------------------------------------------------------------
// string.h

#if defined(X_CC_VC6)
	#ifndef __SGI_VC6_STRING_H__
	#include "vc6/string.h"
	#endif
#elif defined(X_STL_GCC)
	#ifndef __SGI_GCC_STRING_H__
	#include "gcc/string.h"
	#endif
#else
	#ifndef __SGI_STL_STRING
	#include "../../../../stl/string"
	#endif
#endif

// -------------------------------------------------------------------------
// $Log: string.h,v $

#endif /* __SGI_STRING_H__ */
