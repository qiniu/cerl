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
// Module: sgi/functional.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: functional.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_FUNCTIONAL_H__
#define __SGI_FUNCTIONAL_H__

#ifndef __SGI_CONFIG_H__
#include "config.h"
#endif

// -------------------------------------------------------------------------
// functional.h

#if defined(X_STL_GCC)
	#ifndef __SGI_GCC_FUNCTIONAL_H__
	#include "gcc/functional.h"
	#endif
#else
	#ifndef __SGI_STL_FUNCTIONAL
	#include "../../../../stl/functional"
	#endif
#endif

// -------------------------------------------------------------------------
// $Log: functional.h,v $

#endif /* __SGI_FUNCTIONAL_H__ */
