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
// Module: stdext/thread/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Basic.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_BASIC_H
#define STDEXT_THREAD_BASIC_H

#ifndef STDEXT_WINAPI_WINBASE_H
#include "../winapi/winbase.h"
#endif

// -------------------------------------------------------------------------

#if defined(_WIN32) || defined(_WIN64)
#define STDEXT_THREAD_WINDOWS
#endif

// -------------------------------------------------------------------------

#if !defined(STDEXT_THREAD_WINDOWS) || defined(PTW32_VERSION)

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

#define STDEXT_HAS_PTHREAD

#endif

// -------------------------------------------------------------------------

#if !defined(STDEXT_THREAD_WINDOWS)

#define STDEXT_HAS_ATOMIC_BUILTINS

#endif

// -------------------------------------------------------------------------
//	$Log: $

#endif /* STDEXT_THREAD_BASIC_H */
