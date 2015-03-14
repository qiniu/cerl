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
// Module: sgi/config.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: config.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_CONFIG_H__
#define __SGI_CONFIG_H__

#if defined(_MSC_VER)
#pragma warning(disable:4010) // vc6: single-line comment contains line-continuation character
#endif

// -------------------------------------------------------------------------
// std::allocator

#define __STL_USE_STD_ALLOCATORS
#define __SGI_STL_INTERNAL_ALLOC_H
#define __SGI_STL_ALLOC_H

// -------------------------------------------------------------------------

#if defined(X_STL_GCC)
	#include <algorithm>
#else
	#ifndef __STL_CONFIG_H
	#include "../../../../stl/stl_config.h"
	#endif
#endif

// -------------------------------------------------------------------------

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

// -------------------------------------------------------------------------
// namespace stdext

#undef typename
#undef __STD 
#undef __STL_BEGIN_NAMESPACE 
#undef __STL_END_NAMESPACE 
#undef __STL_USE_NAMESPACE_FOR_RELOPS
#undef __STL_BEGIN_RELOPS_NAMESPACE 
#undef __STL_END_RELOPS_NAMESPACE 
#undef __STD_RELOPS 
#undef __STL_USE_NAMESPACES

#define __STL_USE_NAMESPACES

#define __STL_BEGIN_NAMESPACE namespace __STD {
#define __STL_END_NAMESPACE }
#define __STL_USE_NAMESPACE_FOR_RELOPS
#define __STL_BEGIN_RELOPS_NAMESPACE namespace __STD { namespace rel_ops {
#define __STL_END_RELOPS_NAMESPACE } }
#define __STD_RELOPS __STD::rel_ops

#if defined(X_STL_GCC)
#define __STD __gnu_cxx
	#define __STL_CLASS_PARTIAL_SPECIALIZATION
	#define __STL_FUNCTION_TMPL_PARTIAL_ORDER
	#define __STL_MEMBER_TEMPLATES
	#define __STL_HAS_NAMESPACES
	#undef	__STL_TEMPLATE_NULL
	#define __STL_TEMPLATE_NULL template<>
	#define __STL_USE_NEW_IOSTREAMS
	#define __SGI_STDEXCEPT
	#define __SGI_STL_INTERNAL_CONSTRUCT_H
	#define __SGI_STL_INTERNAL_UNINITIALIZED_H
	#define __SGI_STL_INTERNAL_TEMPBUF_H
	#define __TYPE_TRAITS_H
	#define _BACKWARD_NEW_H 1
	#define _BACKWARD_IOSTREAM_H 1
	#ifndef _NEW
	#include <new>
	#endif
	#ifndef _IOSTREAM
	#include <iostream>
	#endif
#elif defined(X_CC_VC_NET)
#define __STD stdext
	#define __value	___value
	#define __STL_FUNCTION_TMPL_PARTIAL_ORDER
	#define __STL_MEMBER_TEMPLATES
	#define __STL_HAS_NAMESPACES
	#undef	__STL_TEMPLATE_NULL
	#define __STL_TEMPLATE_NULL template<>
	#define __STL_USE_NEW_IOSTREAMS
	#define __SGI_STDEXCEPT
	#define _BACKWARD_NEW_H 1
	#define _BACKWARD_IOSTREAM_H 1
	#ifndef _NEW_
	#include <new>
	#endif
	#ifndef __TYPE_TRAITS_H
	#include "../../../../stl/type_traits.h"
	#endif
	#ifndef __CONCEPT_CHECKS_H
	#include "../../../../stl/concept_checks.h"
	#endif
	namespace __STD {
		using std::streamoff;
		using std::streampos;
		using std::istream;
		using std::basic_ostream;
		using std::basic_istream;
		using std::basic_streambuf;
		using std::allocator;
		using std::exception;
		using std::range_error;
		using std::length_error;
	}
#else
#define __STD stdext
#endif

// -------------------------------------------------------------------------

#define _WINX_GC_ALLOCATOR

namespace __STD
{
#if defined(X_CC_VC6)
	template <class _Tp, class _Alloc>
	struct _Alloc_traits
	{
		enum { _S_instanceless = 0 };
		typedef _Tp value_type;
		typedef NS_BOOST_MEMORY::stl_allocator<
			value_type, NS_STDEXT::DefaultAlloc> allocator_type;
	};
#else
	template <class _Tp, class _Alloc>
	struct _Alloc_traits
	{
		enum { _S_instanceless = 0 };
		typedef _Tp value_type;
		typedef typename _Alloc::template rebind<value_type>::other allocator_type;
	};
#endif
}

// -------------------------------------------------------------------------

#ifndef __SGI_ITERATOR_H__
#include "iterator.h"
#endif

#ifndef __SGI_ALGORITHM_H__
#include "algorithm.h"
#endif

// -------------------------------------------------------------------------

#endif /* __SGI_CONFIG_H__ */
