//
//  simple_examples.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include <boost/memory.hpp>

// -------------------------------------------------------------------------

void testAutoAlloc()
{
	NS_BOOST_MEMORY::auto_alloc alloc;

	int* intObj = BOOST_MEMORY_NEW(alloc, int);
	int* intObjWithArg = BOOST_MEMORY_NEW(alloc, int)(10);
	int* intArray = BOOST_MEMORY_NEW_ARRAY(alloc, int, 100);
	int* intBuf = BOOST_MEMORY_ALLOC(alloc, int);
	int* intArrayBuf = BOOST_MEMORY_ALLOC_ARRAY(alloc, int, 100);
	
	NS_BOOST_MEMORY::auto_alloc* subAlloc = BOOST_MEMORY_NEW(alloc, NS_BOOST_MEMORY::auto_alloc);

	int* e = BOOST_MEMORY_NEW(*subAlloc, int);
}

void testScopedAlloc()
{
	NS_BOOST_MEMORY::block_pool recycle;
	NS_BOOST_MEMORY::scoped_alloc alloc(recycle);
	
	int* intObj = BOOST_MEMORY_NEW(alloc, int);
	int* intObjWithArg = BOOST_MEMORY_NEW(alloc, int)(10);
	int* intArray = BOOST_MEMORY_NEW_ARRAY(alloc, int, 100);
	int* intBuf = BOOST_MEMORY_ALLOC(alloc, int);
	int* intArrayBuf = BOOST_MEMORY_ALLOC_ARRAY(alloc, int, 100);
	
	NS_BOOST_MEMORY::scoped_alloc* suballoc = BOOST_MEMORY_NEW(alloc, NS_BOOST_MEMORY::scoped_alloc)(alloc.get_alloc());

	int* e = BOOST_MEMORY_NEW(*suballoc, int);
}

int main()
{
	NS_BOOST_MEMORY::enableMemoryLeakCheck();

	testAutoAlloc();
	testScopedAlloc();

	return 0;
}
