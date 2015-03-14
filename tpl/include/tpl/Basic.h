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
// Module: tpl/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Basic.h 731 2008-06-11 19:52:41Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_BASIC_H
#define TPL_BASIC_H

#ifndef STDEXT_BASIC_H
#include "../../../stdext/include/stdext/Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../../../stdext/include/stdext/Memory.h"
#endif

#ifndef TPL_UNMANAGED_NEW
#define TPL_UNMANAGED_NEW(alloc, Type)	new((alloc).allocate(sizeof(Type))) Type
#endif

#ifndef TPL_ALLOC_ARRAY
#define TPL_ALLOC_ARRAY(alloc, Type, n)	((Type*)(alloc).allocate((n)*sizeof(Type)))
#endif

#ifndef TPL_ALLOC
#define TPL_ALLOC(alloc, Type)	((Type*)(alloc).allocate(sizeof(Type)))
#endif

#ifndef TPL_NEW
#if defined(BOOST_NEW)
#define TPL_NEW(alloc, Type)	BOOST_NEW(alloc, Type)
#else
#define TPL_NEW(alloc, Type)	STD_NEW(alloc, Type)
#endif
#endif

#ifndef TPL_CALL
#if defined(winx_call)
#define TPL_CALL winx_call
#else
#define TPL_CALL
#endif
#endif

#ifndef TPL_ASSERT
#if defined(WINX_ASSERT)
#define TPL_ASSERT(e)	WINX_ASSERT(e)
#elif defined(_ASSERTE)
#define TPL_ASSERT(e)	_ASSERTE(e)
#else
#define TPL_ASSERT(e)	0
#endif
#endif

#ifndef TPL_TRACE
#if defined(WINX_TRACE)
#define TPL_TRACE	WINX_TRACE
#elif defined(_DEBUG)
#define TPL_TRACE	printf
#else
#define TPL_TRACE	__noop
#endif
#endif

#ifndef TPL_TRACEW
#if defined(WINX_TRACEW)
#define TPL_TRACEW	WINX_TRACEW
#elif defined(_DEBUG)
#define TPL_TRACEW	wprintf
#else
#define TPL_TRACEW	__noop
#endif
#endif

#ifndef NS_TPL_BEGIN
#define NS_TPL_BEGIN			namespace tpl {
#define NS_TPL_END				}
#define NS_TPL					tpl
#endif

#if !defined(__forceinline) && !defined(_MSC_VER)
#define __forceinline inline
#endif

#if defined(_MSC_VER)
#pragma warning(disable: 4503)
#endif

NS_TPL_BEGIN

// =========================================================================
// class DefaultAllocator

#if defined(TPL_USE_AUTO_ALLOC)
typedef NS_STDEXT::AutoFreeAlloc DefaultAllocator;
#else
typedef NS_STDEXT::ScopedAlloc DefaultAllocator;
#endif

// =========================================================================
// TPL_REQUIRE(e, Require) -- see BOOST_STATIC_ASSERT(e)

#if !defined(X_CC_VC6)

template <bool bOk>
struct RequireFeatureTraits {
	typedef struct RequireFeatureOk {} feature_type;
};

template <>
struct RequireFeatureTraits<false> {
	struct RequireFeatureFailed {};
};

template <class T1, class T2>
struct RequireClassTraits {
	struct RequireClassFailed {};
};

template <class ClassT>
struct RequireClassTraits<ClassT, ClassT> {
	typedef ClassT class_type;
};

#ifndef TPL_REQUIRE
#define TPL_REQUIRE(e, RequireFeature_)										\
	typedef typename tpl::RequireFeatureTraits<(e) != 0>::feature_type RequireFeature_;
#endif

#ifndef TPL_REQUIRE_CLASS
#define TPL_REQUIRE_CLASS(T1, T2, RequireFeature_)							\
	typedef typename tpl::RequireClassTraits<T1, T2>::class_type RequireFeature_;
#endif

#endif // #if !defined(X_CC_VC6)

// =========================================================================
// TPL_CONST

#define TPL_CONST(Type, var)	const Type var = Type()
#define TPL_CONST_EX(Type, var)	const Type var

// =========================================================================
// class SelectValueType

#if !defined(X_CC_VC6)

struct DefaultType {};

template <class ValueT, class DefaultValT>
struct SelectValueType {
	typedef ValueT value_type;
};

template <class DefaultValT>
struct SelectValueType<DefaultType, DefaultValT> {
	typedef DefaultValT value_type;
};

#endif // #if !defined(X_CC_VC6)

// =========================================================================
// class IsConvertible - macro TPL_CONVERTIBLE

#if defined(TPL_USE_BOOST_IS_CONVERTIBLE)

#pragma warning(disable:4819)

#ifndef BOOST_TT_IS_CONVERTIBLE_HPP_INCLUDED
#include <boost/type_traits/is_convertible.hpp>
#endif

#define TPL_CONVERTIBLE(From, To)	boost::is_convertible<From, To>::value

#else

template <class From, class To>
struct IsConvertible
{
	struct AnyConversion_
	{
		template <typename T1>
		AnyConversion_(const T1&);
	};
	
	struct No_ { char b[0x100]; };
	struct Yes_ { char b[0x200]; };

	static No_ check_(AnyConversion_, ...);
	static Yes_ check_(To*, int);
	static From* inst_;

	enum {
		value = (sizeof(check_(inst_, 0)) == sizeof(Yes_))
	};
};

#define TPL_CONVERTIBLE(From, To)			tpl::IsConvertible<From, To>::value

#endif // defined(TPL_USE_BOOST_IS_CONVERTIBLE)

// -------------------------------------------------------------------------
// TPL_TEMPLATE_CONVERTIBLE

#if !defined(X_CC_VC6)

template <class From, template <class ArgT> class To>
struct IsTemplateConvertible
{
	struct AnyConversion_
	{
		template <typename T1>
		AnyConversion_(const T1&);
	};
	
	struct No_ { char b[0x100]; };
	struct Yes_ { char b[0x200]; };

	template <class ArgT>
	static Yes_ check_(To<ArgT>*, int);
	static No_ check_(AnyConversion_, ...);
	static From* inst_;

	enum {
		value = (sizeof(check_(inst_, 0)) == sizeof(Yes_))
	};
};

#define TPL_TEMPLATE_CONVERTIBLE(From, To)	tpl::IsTemplateConvertible<From, To>::value

#endif // #if !defined(X_CC_VC6)

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BASIC_H */

