//
//  boost/memory/type_traits.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_TYPE_TRAITS_HPP
#define BOOST_MEMORY_TYPE_TRAITS_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

NS_BOOST_MEMORY_BEGIN

// =========================================================================
// class constructor_traits

template <class Type>
struct constructor_traits
{
	static void BOOST_MEMORY_CALL construct(void* data)
	{
		new(data) Type;
	}

	static void BOOST_MEMORY_CALL constructArray(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			new(array + i) Type;
	}
};

// -------------------------------------------------------------------------
// BOOST_MEMORY_NO_CONSTRUCTOR

#define BOOST_MEMORY_NO_CONSTRUCTOR_(Type)									\
template <>																	\
struct constructor_traits< Type >											\
{																			\
	static void BOOST_MEMORY_CALL construct(void* data) {}					\
	static void BOOST_MEMORY_CALL constructArray(Type*, size_t) {}			\
};

#define BOOST_MEMORY_NO_CONSTRUCTOR(Type)									\
	NS_BOOST_MEMORY_BEGIN													\
		BOOST_MEMORY_NO_CONSTRUCTOR_(Type)									\
	NS_BOOST_MEMORY_END

// =========================================================================
// class destructor_traits

typedef void BOOST_MEMORY_CALL BOOST_FnDestructor(void* data);
typedef BOOST_FnDestructor* destructor_t;

template <class Type>
struct destructor_traits
{
	enum { hasDestructor = 1 };

	typedef destructor_t destructor_type;

	static void BOOST_MEMORY_CALL destruct(void* data)
	{
		((Type*)data)->~Type();
	}

	static void BOOST_MEMORY_CALL destructArray(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			array[i].~Type();
	}
};

template <class Type>
inline void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
{
	destructor_traits<Type>::destructArray(array, count);
}

// -------------------------------------------------------------------------
// BOOST_MEMORY_NO_DESTRUCTOR

#define BOOST_MEMORY_NO_DESTRUCTOR_(Type)									\
template <>																	\
struct destructor_traits< Type >											\
{																			\
	typedef int destructor_type;											\
																			\
	enum { hasDestructor = 0 };												\
	enum { destruct = 0 };													\
																			\
	static void BOOST_MEMORY_CALL destructArray(Type*, size_t) {}			\
};

#define BOOST_MEMORY_NO_DESTRUCTOR(Type)									\
	NS_BOOST_MEMORY_BEGIN													\
		BOOST_MEMORY_NO_DESTRUCTOR_(Type)									\
	NS_BOOST_MEMORY_END

// =========================================================================
// class array_factory

#pragma pack(1)

template <class Type>
struct array_factory_has_destructor
{
	struct destructor_header {
		size_t count;
	};

	static size_t BOOST_MEMORY_CALL alloc_size(size_t count)
	{
		return sizeof(destructor_header) + sizeof(Type)*count;
	}

	static void BOOST_MEMORY_CALL destruct(void* data)
	{
		destructor_header* hdr = (destructor_header*)data;
		destructor_traits<Type>::destructArray((Type*)(hdr + 1), hdr->count);
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	template <class AllocT>
	static Type* BOOST_MEMORY_CALL create(AllocT& alloc, size_t count)
	{
		destructor_header* hdr =
			(destructor_header*)alloc.allocate(
				sizeof(destructor_header) + sizeof(Type)*count, destruct);
		Type* array = (Type*)(hdr + 1);
		hdr->count = count;
		constructor_traits<Type>::constructArray(array, count);
		return array;
	}
#else
	template <class AllocT>
	static Type* BOOST_MEMORY_CALL create(AllocT& alloc, size_t count)
	{
		destructor_header* hdr =
			(destructor_header*)alloc.unmanaged_alloc(
				sizeof(destructor_header) + sizeof(Type)*count, destruct);
		Type* array = (Type*)(hdr + 1);
		hdr->count = count;
		constructor_traits<Type>::constructArray(array, count);
		alloc.manage(hdr, destruct);
		return array;
	}
#endif
	
	static char* BOOST_MEMORY_CALL buffer(Type* array)
	{
		return (char*)array - sizeof(destructor_header);
	}

	static size_t BOOST_MEMORY_CALL size(Type* array)
	{
		return ((destructor_header*)array - 1)->count;
	}
};

#pragma pack()

template <class Type>
struct array_factory_no_destructor
{
	enum { destruct = 0 };

	typedef void destructor_header;

	static size_t BOOST_MEMORY_CALL alloc_size(size_t count)
	{
		return sizeof(Type)*count;
	}

	template <class AllocT>
	static Type* BOOST_MEMORY_CALL create(AllocT& alloc, size_t count)
	{
		Type* array = (Type*)alloc.allocate(sizeof(Type)*count);
		constructor_traits<Type>::constructArray(array, count);
		return array;
	}

	static char* BOOST_MEMORY_CALL buffer(Type* array)
	{
		return (char*)array;
	}

	static size_t BOOST_MEMORY_CALL size(Type* array)
	{
		BOOST_MEMORY_ASSERT( !"Don't call me!!!" );
		return 0;
	}
};

#if defined(BOOST_MEMORY_NO_PARTIAL_SPECIALIZATION)

template <class Type>
struct array_factory_impl_
{
	template <int hasDestructor>
	struct traits_ : array_factory_has_destructor<Type> {};

	template <>
	struct traits_<0> : array_factory_no_destructor<Type> {};
	
	typedef traits_<destructor_traits<Type>::hasDestructor> traits;
};

template <class Type>
struct array_factory : array_factory_impl_<Type>::traits
{
};

#else

template <class Type, int hasDestructor = destructor_traits<Type>::hasDestructor>
struct array_factory : array_factory_has_destructor<Type> {};

template <class Type>
struct array_factory<Type, 0> : array_factory_no_destructor<Type> {};

#endif

// =========================================================================
// C Standard Types Support

#define BOOST_MEMORY_DECL_CTYPE_(Type)										\
	BOOST_MEMORY_NO_CONSTRUCTOR_(Type);										\
	BOOST_MEMORY_NO_DESTRUCTOR_(Type)

#define BOOST_MEMORY_DECL_CTYPE(Type)										\
	BOOST_MEMORY_NO_CONSTRUCTOR(Type);										\
	BOOST_MEMORY_NO_DESTRUCTOR(Type)

// -------------------------------------------------------------------------

BOOST_MEMORY_DECL_CTYPE_(bool);
BOOST_MEMORY_DECL_CTYPE_(float);
BOOST_MEMORY_DECL_CTYPE_(double);

BOOST_MEMORY_DECL_CTYPE_(int);
BOOST_MEMORY_DECL_CTYPE_(unsigned int);

BOOST_MEMORY_DECL_CTYPE_(char);
BOOST_MEMORY_DECL_CTYPE_(unsigned char);

BOOST_MEMORY_DECL_CTYPE_(short);
BOOST_MEMORY_DECL_CTYPE_(unsigned short);

BOOST_MEMORY_DECL_CTYPE_(long);
BOOST_MEMORY_DECL_CTYPE_(unsigned long);

// =========================================================================

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_TYPE_TRAITS_HPP */
