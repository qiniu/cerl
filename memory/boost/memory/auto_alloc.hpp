//
//  boost/memory/auto_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_AUTO_ALLOC_HPP
#define BOOST_MEMORY_AUTO_ALLOC_HPP

#ifndef BOOST_MEMORY_REGION_ALLOC_HPP
#include "region_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_SYSTEM_ALLOC_HPP
#include "system_alloc.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class auto_alloc

#if defined(_MSC_VER) && (_MSC_VER <= 1200) // VC++ 6.0

class auto_alloc : public region_alloc<NS_BOOST_MEMORY_POLICY::stdlib>
{
private:
	typedef region_alloc<NS_BOOST_MEMORY_POLICY::stdlib> BaseClass;
	typedef BaseClass::alloc_type AllocT;
	
public:
	auto_alloc() {}
	explicit auto_alloc(AllocT alloc) {}

	__forceinline void BOOST_MEMORY_CALL swap(auto_alloc& o) {
		BaseClass::swap(o);
	}

	__forceinline void BOOST_MEMORY_CALL clear() {
		BaseClass::clear();
	}

	__forceinline void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize) {
		return BaseClass::reallocate(p, oldSize, newSize);
	}

	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb) {
		return BaseClass::allocate(cb);
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero) {
		return BaseClass::allocate(cb);
	}

	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn) {
		return BaseClass::allocate(cb, fn);
	}
#endif
	
	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, destructor_t fn) {
		return BaseClass::unmanaged_alloc(cb, fn);
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, destructor_t fn) {
		BaseClass::manage(p, fn);
	}

	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, int fnZero) {
		return BaseClass::allocate(cb);
	}

	__forceinline static void BOOST_MEMORY_CALL manage(void* p, int fnZero) {
		// no action
	}

	__forceinline static void BOOST_MEMORY_CALL deallocate(void* p, size_t cb) {
		// no action
	}

	__forceinline static void BOOST_MEMORY_CALL deallocate(void* p) {
		// no action
	}

	template <class Type>
	__forceinline static void BOOST_MEMORY_CALL destroy(Type* obj) {
		// no action
	}

	template <class Type>
	__forceinline static void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count) {
		// no action
	}
};

#else

typedef region_alloc<NS_BOOST_MEMORY_POLICY::stdlib> auto_alloc;

#endif

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_AUTO_ALLOC_HPP */
