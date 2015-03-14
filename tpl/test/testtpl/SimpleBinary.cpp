#include "stdafx.h"
#include <tpl/BinRegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// simple_binary

struct SimpleBinary
{
	UINT32 tag;
	UINT16 cw;
	UINT16 cl;
	UINT16 w[24];
	UINT32 l[8];
};

void simple_binary()
{
	typedef SimpleBinaryImplementation impl;

	SimpleBinary b = {
		123456,
		24,
		8,
		{ 1, 2, 3, 4, 5, 6, 7, 8 },
		{ 9, 10, 11, 12 }
	};

	UINT32 tag;
	Var<UINT16> cw;
	Var<UINT16> cl;

	impl::Array<UINT16> w;
	impl::Array<UINT32> l;

	impl::Allocator alloc;
	impl::Rule rDoc( alloc,
		c_type<UINT32>(tag) +
		c_type<UINT16>(cw) +
		c_type<UINT16>(cl) +
		c_array<UINT16>(w, cw) +
		c_array<UINT32>(l, cl)
		);

	if (impl::match(&b, sizeof(b), rDoc)) {
		std::cout << tag << '\n'
			<< cw.val << '\n' << cl.val << '\n'
			<< w << '\n' << l << '\n';
	}
}

// -------------------------------------------------------------------------
