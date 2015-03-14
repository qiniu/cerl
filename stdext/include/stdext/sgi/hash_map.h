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
// Module: sgi/hash_map.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: hash_map.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_HASH_MAP_H__
#define __SGI_HASH_MAP_H__

// -------------------------------------------------------------------------
// hash_map/hash_multimap

#ifndef __SGI_CONFIG_H__
#include "config.h"
#endif

#ifndef __SGI_VECTOR_H__
#include "vector.h"
#endif

#ifndef __SGI_ITERATOR_H__
#include "iterator.h"
#endif

#if defined(__STL_NEED_TYPENAME)
#define typename
#endif

#ifndef __TYPE_TRAITS_H
#include "../../../../stl/type_traits.h"
#endif

#ifndef __SGI_STL_INTERNAL_PAIR_H
#include "../../../../stl/stl_pair.h"
#endif

#ifndef __SGI_STL_INTERNAL_HASHTABLE_H
#include "../../../../stl/stl_hashtable.h"
#endif 

#ifndef __SGI_STL_HASH_MAP_H
#include "../../../../stl/hash_map.h"
#endif

#if defined(__STL_NEED_TYPENAME)
#undef typename
#endif

// -------------------------------------------------------------------------
// $Log: hash_map.h,v $

#endif /* __SGI_HASH_MAP_H__ */
