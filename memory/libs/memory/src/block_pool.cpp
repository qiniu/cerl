//
//  boost/memory/block_pool.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include "stdafx.h"
#include <boost/memory/scoped_alloc.hpp>

// -------------------------------------------------------------------------

using namespace NS_BOOST_MEMORY;

tls_block_pool_t g_tls_blockPool;

STDAPI_(tls_block_pool_t*) boost_TlsBlockPool()
{
	return &g_tls_blockPool;
}

// -------------------------------------------------------------------------
// class tls_block_pool_init

class tls_block_pool_init
{
public:
	tls_block_pool_init() { g_tls_blockPool.init(); }
	~tls_block_pool_init() { g_tls_blockPool.term(); }
};

tls_block_pool_init g_tls_blockPoolInit;

// -------------------------------------------------------------------------
// $Log: $
