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
// Module: std/stdexcept.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: stdexcept.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STD_STDEXCEPT_H
#define STD_STDEXCEPT_H

#ifndef STDEXT_PLATFORM_H
#include "../stdext/Platform.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_STL_SGI)
	#ifndef __SGI_STDEXCEPT
	#include "../../../stl/stdexcept"
	#endif
#else
	#if !defined(_STDEXCEPT_) && !defined(_STDEXCEPT)
	#include <stdexcept>
	#endif
#endif

// -------------------------------------------------------------------------
// $Log: stdexcept.h,v $

#endif /* STD_STDEXCEPT_H */
