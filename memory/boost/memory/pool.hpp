//
//  boost/memory/pool.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_POOL_HPP
#define BOOST_MEMORY_POOL_HPP

#ifndef BOOST_MEMORY_POOL_ALLOC_HPP
#include "pool_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_BLOCKPOOL_HPP
#include "block_pool.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class pool, scoped_pool

typedef pool_alloc<NS_BOOST_MEMORY_POLICY::stdlib> pool;
typedef pool_alloc<NS_BOOST_MEMORY_POLICY::scoped> scoped_pool;

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_POOL_HPP */
