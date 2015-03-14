//
//  boost/memory/basic.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_BASIC_HPP
#define BOOST_MEMORY_BASIC_HPP

// =========================================================================

#pragma pack() // default pack
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif
// warning: identifier was truncated to '255' characters in the debug information

#ifndef BOOST_MEMORY_CALL
#define BOOST_MEMORY_CALL
#endif

#ifndef NS_BOOST_MEMORY_BEGIN
#define NS_BOOST_MEMORY_BEGIN	namespace boost { namespace memory {
#define NS_BOOST_MEMORY_END		} }
#define NS_BOOST_MEMORY			boost::memory
#endif

// =========================================================================
// BOOST_MEMORY_ASSERT - diagnost

#if defined(_MSC_VER) && !defined(_INC_CRTDBG)
#include <crtdbg.h> // _CrtSetDbgFlag, _ASSERTE
#endif

#ifndef BOOST_MEMORY_ASSERT
	#if defined(ASSERT)
		#define BOOST_MEMORY_ASSERT(e)		ASSERT(e)
	#elif defined(_ASSERTE)
		#define BOOST_MEMORY_ASSERT(e)		_ASSERTE(e)
	#elif defined(BOOST_ASSERT)
		#define BOOST_MEMORY_ASSERT(e)		BOOST_ASSERT(e)
	#else
		#ifdef _DEBUG
		#ifndef assert
		#include <cassert>
		#endif
		#define BOOST_MEMORY_ASSERT(e)		assert(e)
		#else
		#define BOOST_MEMORY_ASSERT(e)
		#endif
	#endif
#endif

// -------------------------------------------------------------------------
// macro BOOST_MEMORY_DBG_FILL

#ifndef BOOST_MEMORY_DBG_FILL
#if defined(_DEBUG)
#define BOOST_MEMORY_DBG_FILL(p, cb)	memset(p, 0xcd, cb)
#else
#define BOOST_MEMORY_DBG_FILL(p, cb)
#endif
#endif

// -------------------------------------------------------------------------

#ifndef BOOST_MEMORY_STATIC_ASSERT
#if defined(BOOST_STATIC_ASSERT)
#define BOOST_MEMORY_STATIC_ASSERT(e)	BOOST_STATIC_ASSERT(e)
#else
#define BOOST_MEMORY_STATIC_ASSERT(e)	BOOST_MEMORY_ASSERT(e)
#endif
#endif

// -------------------------------------------------------------------------

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	#define BOOST_MEMORY_NO_PARTIAL_SPECIALIZATION
#elif defined(_MSC_VER)
	#if (_MSC_VER <= 1200)
	#define BOOST_MEMORY_NO_PARTIAL_SPECIALIZATION
	#endif
#endif

#if defined(BOOST_NO_FUNCTION_TYPE_SPECIALIZATIONS)
	#define BOOST_MEMORY_NO_FUNCTION_TYPE_SPECIALIZATIONS
#elif defined(_MSC_VER)
	#if (_MSC_VER <= 1200)
	#define BOOST_MEMORY_NO_FUNCTION_TYPE_SPECIALIZATIONS
	#endif
#endif

// -------------------------------------------------------------------------

#if !defined(_MSC_VER)
#ifndef __forceinline
#define __forceinline inline
#endif
#ifndef __stdcall
#define __stdcall
#endif
#endif

// =========================================================================
// constructor_traits, destructor_traits

#ifndef BOOST_MEMORY_TYPE_TRAITS_HPP
#include "type_traits.hpp"
#endif

// =========================================================================
// BOOST_MEMORY_THREAD_CHECKER

#if defined(_WIN32) || defined(_WIN64)

#ifndef _WINDOWS_
#include <windows.h>
#endif

#else

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

#endif

NS_BOOST_MEMORY_BEGIN

inline unsigned long BOOST_MEMORY_CALL getCurrentThreadId()
{
#if defined(_WIN32) || defined(_WIN64)
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

class thread_checker
{
private:
	unsigned long m_threadId;

public:
	thread_checker()
	{
		m_threadId = getCurrentThreadId();
	}

	bool BOOST_MEMORY_CALL check() const
	{
		return m_threadId == getCurrentThreadId();
	}
};

#if defined(_DEBUG)
#define BOOST_MEMORY_THREAD_CHECKER		NS_BOOST_MEMORY::thread_checker boostmm_thread_checker
#define BOOST_MEMORY_CHECK_THREAD()		BOOST_MEMORY_ASSERT(boostmm_thread_checker.check())
#else
#define BOOST_MEMORY_THREAD_CHECKER
#define BOOST_MEMORY_CHECK_THREAD()		(void)0
#endif

NS_BOOST_MEMORY_END

// =========================================================================
// NEW, NEW_ARRAY, ALLOC, ALLOC_ARRAY

#if !defined(_NEW_) && !defined(_NEW)
#include <new>	// new
#endif

NS_BOOST_MEMORY_BEGIN

class unmanaged_
{
public:
	template <class Type>
	__forceinline Type* BOOST_MEMORY_CALL operator->*(Type* p) const
	{
		return p;
	}
};

template <class AllocT>
class managed_
{
private:
	AllocT& m_alloc;

public:
	explicit managed_(AllocT& alloc) : m_alloc(alloc) {}

	template <class Type>
	__forceinline Type* BOOST_MEMORY_CALL operator->*(Type* p) const
	{
		m_alloc.manage(p, destructor_traits<Type>::destruct);
		return p;
	}
};

template <class AllocT>
__forceinline unmanaged_ BOOST_MEMORY_CALL get_managed__(AllocT& alloc, int fnZero)
{
	return unmanaged_();
}

template <class AllocT>
__forceinline 
managed_<AllocT> BOOST_MEMORY_CALL get_managed__(AllocT& alloc, destructor_t fn)
{
	return managed_<AllocT>(alloc);
}

NS_BOOST_MEMORY_END

#define BOOST_MEMORY_ALLOC(alloc, Type)					((Type*)(alloc).allocate(sizeof(Type)))
#define BOOST_MEMORY_ALLOC_ARRAY(alloc, Type, count)	((Type*)(alloc).allocate(sizeof(Type)*(count)))

#define BOOST_MEMORY_NEW_ARRAY(alloc, Type, count) 		NS_BOOST_MEMORY::array_factory<Type>::create(alloc, count)
#define BOOST_MEMORY_DESTRUCTOR(Type)					NS_BOOST_MEMORY::destructor_traits<Type>::destruct

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
//
// Backward options:
// 	not strict in accord with normal C++ semantics but a bit faster
//
#define BOOST_MEMORY_NEW(alloc, Type)					\
	::new((alloc).allocate(sizeof(Type), BOOST_MEMORY_DESTRUCTOR(Type))) Type

#else

#define BOOST_MEMORY_UNMANAGED_ALLOC_(alloc, Type)		\
	::new((alloc).unmanaged_alloc(sizeof(Type), BOOST_MEMORY_DESTRUCTOR(Type))) Type

#define BOOST_MEMORY_GET_MANAGED_(alloc, Type)			\
	NS_BOOST_MEMORY::get_managed__(alloc, BOOST_MEMORY_DESTRUCTOR(Type))

#define BOOST_MEMORY_NEW(alloc, Type)					\
	BOOST_MEMORY_GET_MANAGED_(alloc, Type) ->* BOOST_MEMORY_UNMANAGED_ALLOC_(alloc, Type)

#endif

#define BOOST_MEMORY_UNMANAGED_NEW(alloc, Type)			\
	::new((alloc).allocate(sizeof(Type))) Type

// =========================================================================
// function enableMemoryLeakCheck

#if !defined(_MAP_) || defined(_MAP)
#include <map>
#endif

NS_BOOST_MEMORY_BEGIN

class DbgMemoryState
{
private:
	typedef std::pair<const char*, int> FileLine;
	typedef std::map<void*, FileLine> Container;

	Container m_data;

public:
	void BOOST_MEMORY_CALL allocate(void* p, const char* file = NULL, int line = 0)
	{
		BOOST_MEMORY_ASSERT(m_data.count(p) == 0);
		const Container::value_type v = Container::value_type(p, FileLine(file, line));
		m_data.insert(v);
	}

	void BOOST_MEMORY_CALL deallocate(void* p)
	{
		const Container::iterator it = m_data.find(p);
		BOOST_MEMORY_ASSERT(it != m_data.end() || !"deallocate invalid memory address!");
		m_data.erase(it);
	}

	void BOOST_MEMORY_CALL clear()
	{
		m_data.clear();
	}

	void BOOST_MEMORY_CALL reportMemoryLeak()
	{
		// todo;
	}
};

class FakeMemoryState
{
public:
	void BOOST_MEMORY_CALL allocate(void* p, const char* file = NULL, int line = 0) {}
	void BOOST_MEMORY_CALL deallocate(void* p) {}
	void BOOST_MEMORY_CALL clear() {}
	void BOOST_MEMORY_CALL reportMemoryLeak() {}
};

#if defined(_DEBUG)
typedef DbgMemoryState MemoryState;
#else
typedef FakeMemoryState MemoryState;
#endif

inline void BOOST_MEMORY_CALL enableMemoryLeakCheck()
{
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
}

NS_BOOST_MEMORY_END

// =========================================================================
// function swap_object

#if !defined(_CSTRING_) && !defined(_CSTRING)
#include <cstring> // memcpy
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H)
#include <malloc.h>	// _alloca
#endif

#if !defined(_alloca) && !defined(_MSC_VER)
#define _alloca alloca
#endif

NS_BOOST_MEMORY_BEGIN

template <class Type>
void swap_object(Type* a, Type* b)
{
	void* t = _alloca(sizeof(Type));
	memcpy(t, a, sizeof(Type));
	memcpy(a, b, sizeof(Type));
	memcpy(b, t, sizeof(Type));
}

NS_BOOST_MEMORY_END

// =========================================================================

#endif /* BOOST_MEMORY_BASIC_HPP */
