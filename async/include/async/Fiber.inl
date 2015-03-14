/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Fiber.inl
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Fiber.inl 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_INL
#define ASYNC_FIBER_INL

#ifndef ASYNC_FIBER_H
#include "Fiber.h"
#endif

// -------------------------------------------------------------------------
// import libcoro

#if defined(ASYNC_FIBER_CORO_H)

extern "C"
{
	#include "fiber/libcoro/coro.c"
};

#endif

// -------------------------------------------------------------------------
// import wincoro

#if defined(ASYNC_FIBER_WINCORO_H)

#include "fiber/wincoro/coro.c"

#endif

// -------------------------------------------------------------------------

#endif /* ASYNC_FIBER_INL */
