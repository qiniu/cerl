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
// Module: sgi/list.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: list.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_LIST_H__
#define __SGI_LIST_H__

// -------------------------------------------------------------------------
// list

#ifndef __SGI_CONFIG_H__
#include "config.h"
#endif

#ifndef __SGI_ITERATOR_H__
#include "iterator.h"
#endif

#if defined(X_STL_SGI)
	#ifndef __SGI_STL_LIST_H
	#include "../../../../stl/list.h"
	#endif
#else
	#ifndef _LIST_
	#include <list>
	#endif
#endif

#ifndef __SGI_STL_INTERNAL_LIST_H
	#define __SGI_STL_INTERNAL_LIST_H
#endif

// -------------------------------------------------------------------------

namespace __STD
{
	using std::list;
}

// -------------------------------------------------------------------------
// slist

#if defined(X_STL_GCC)
	#ifndef _SLIST
	#include <ext/slist>
	#endif 
#else
	#ifndef __SGI_STL_SLIST_H
	#include "../../../../stl/slist.h"
	#endif 
#endif

// -------------------------------------------------------------------------
// $Log: list.h,v $
//

#endif /* __SGI_LIST_H__ */
