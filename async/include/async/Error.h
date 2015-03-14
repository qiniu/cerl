/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Error.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Error.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_ERROR_H
#define ASYNC_ERROR_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef ASYNC_MEMORY_H
#include "Memory.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// dup

inline String cerl_call dup(ScopedAlloc& alloc, const String& s)
{
	const size_t cb = s.size();
	char* s2 = STD_ALLOC_ARRAY(alloc, char, cb);
	memcpy(s2, s.data(), cb);
	return String(s2, cb);
}

// -------------------------------------------------------------------------
// strdup

#if defined(X_CC_VC_NET)
#pragma push_macro("strdup")
#undef strdup
#endif

inline const char* cerl_call strdup(ScopedAlloc& alloc, const String& s)
{
	const size_t cb = s.size();
	char* s2 = STD_ALLOC_ARRAY(alloc, char, cb + 1);
	memcpy(s2, s.data(), cb);
	s2[cb] = '\0';
	return s2;
}

#if defined(X_CC_VC_NET)
#pragma pop_macro("strdup")
#endif

// -------------------------------------------------------------------------
// type Error

struct Error
{
	HRESULT code;
	const char* message;
	const Error* child;
};

#if defined(X_CC_VC_NET)
#pragma push_macro("strdup")
#undef strdup
#endif

inline Error* cerl_call createError(
	ScopedAlloc& alloc,	HRESULT code, const String& message, const Error* child = NULL)
{
	Error* e = STD_ALLOC(alloc, Error);
	e->code = code;
	e->message = strdup(alloc, message);
	e->child = child;
	return e;
}

#if defined(X_CC_VC_NET)
#pragma pop_macro("strdup")
#endif

// -------------------------------------------------------------------------
// CERL_NO_ERROR

#define CERL_NO_ERROR	((cerl::Error*)0)

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_ERROR_H */
