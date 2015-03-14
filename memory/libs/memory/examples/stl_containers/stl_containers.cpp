//
//  stl_contianers.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//

#include <set>
#include <map>
#include <list>
#include <deque>
#include <cstdio>

#include <boost/memory.hpp>

using NS_BOOST_MEMORY::stl_allocator;

// -------------------------------------------------------------------------

enum { Count = 100 };

void testDeque()
{
	printf("\n===== Deque (scoped_alloc) =====\n");
	NS_BOOST_MEMORY::block_pool recycle;
	NS_BOOST_MEMORY::scoped_alloc alloc(recycle);
	std::deque<int, stl_allocator<int> > s(alloc);
	for (int i = 0; i < Count; ++i)
		s.push_back(i);
}

void testList()
{
	printf("\n===== List (scoped_alloc) =====\n");
	NS_BOOST_MEMORY::block_pool recycle;
	NS_BOOST_MEMORY::scoped_alloc alloc(recycle);
	std::list<int, stl_allocator<int> > s(alloc);
	for (int i = 0; i < Count; ++i)
		s.push_back(i);
}

void testSet()
{
	printf("\n===== Set (scoped_alloc) =====\n");
	NS_BOOST_MEMORY::block_pool recycle;
	NS_BOOST_MEMORY::scoped_alloc alloc(recycle);
	std::set<int, std::less<int>, stl_allocator<int> > s(std::less<int>(), alloc);
	for (int i = 0; i < Count; ++i)
		s.insert(i);
}

void testMap()
{
	printf("\n===== Map (scoped_alloc) =====\n");
	NS_BOOST_MEMORY::block_pool recycle;
	NS_BOOST_MEMORY::scoped_alloc alloc(recycle);
	std::map<int, int, std::less<int>, stl_allocator<int> > s(std::less<int>(), alloc);
	for (int i = 0; i < Count; ++i)
		s.insert(std::pair<int, int>(i, i));
}

int main()
{
	testDeque();
	testList();
	testSet();
	testMap();

	return 0;
}
