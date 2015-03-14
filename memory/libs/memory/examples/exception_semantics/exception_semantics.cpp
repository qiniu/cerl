//
//  exception_semantics.cpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#include <cstdio>
#include <boost/memory.hpp>

class A
{
public:
	A() {}
	void init() {
		printf("A::A\n");
	}
	~A() {
		printf("A::~A\n");
	}
};

class B
{
public:
	B() {}
	void init() {
		printf("B::B\n");
	}
	~B() {
		printf("B::~B\n");
	}
};

class Foo
{
private:
	A m_a;
	B m_b;

public:
	Foo() {
		m_a.init();
		throw 1;
		m_b.init();
	}
	~Foo() {
		printf("Foo::~Foo\n");
	}
};

void testExceptionSemantics()
{
	try
	{
		Foo* p = new Foo;
	}
	catch (...)
	{
		printf("Unexcepted\n");
	}

	printf("Now, let's test operator new of GC Allocator...\n");
	try
	{
		boost::auto_alloc alloc;
		Foo* p = BOOST_NEW(alloc, Foo);
	}
	catch (...)
	{
		printf("Unexcepted\n");
	}
}
