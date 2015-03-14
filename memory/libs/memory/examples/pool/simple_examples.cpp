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

#ifndef	countof
#define countof(array)						(sizeof(array)/sizeof(array[0]))
#endif

void testPool()
{
	int i;
	NS_BOOST_MEMORY::pool alloc(sizeof(int));
	NS_BOOST_MEMORY::pool alloc2(sizeof(double));

	int* p[3000];
	for (i = 0; i < countof(p); ++i)
	{
		p[i] = (int*)alloc.allocate();
		*p[i] = i;
	}
	for (i = 0; i < countof(p); ++i)
	{
		alloc.deallocate(p[i]);
	}

	void* p1 = alloc.allocate();
	void* p2 = alloc.allocate();
	void* p3 = alloc.allocate();

	alloc.deallocate(p2);
	alloc.deallocate(p1);
	alloc.deallocate(p3);

	//
	// Note: It's ok even if you forget to free allocated memory!
	//
	for (i = 0; i < 1000; ++i)
		alloc.allocate();
}

void testScopedPool()
{
	int i;
	NS_BOOST_MEMORY::block_pool recycle;
	NS_BOOST_MEMORY::scoped_pool alloc(recycle, sizeof(int));
	NS_BOOST_MEMORY::scoped_pool alloc2(alloc.get_alloc(), sizeof(double));

	void* p[3000];
	for (i = 0; i < countof(p); ++i)
		p[i] = alloc.allocate();
	for (i = 0; i < countof(p); ++i)
		alloc.deallocate(p[i]);

	void* p1 = alloc.allocate();
	void* p2 = alloc.allocate();
	void* p3 = alloc.allocate();

	alloc.deallocate(p2);
	alloc.deallocate(p1);
	alloc.deallocate(p3);

	//
	// Note: It's ok even if you forget to free allocated memory!
	//
	for (i = 0; i < 1000; ++i)
		alloc.allocate();
}

void testPools()
{
	NS_BOOST_MEMORY::pools alloc;

	void* p = alloc.allocate(134);
	alloc.deallocate(p, 134);
	//alloc.deallocate(p, 134);
}

int main()
{
	NS_BOOST_MEMORY::enableMemoryLeakCheck();

	testPool();
	testScopedPool();
	testPools();
	return 0;
}
