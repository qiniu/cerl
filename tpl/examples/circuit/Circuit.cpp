#define TPL_USE_AUTO_ALLOC
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// circuit

// Note:
//	if two rules A, B circularly refer to each other (or a rule A refer to itself),
//	we need use Rule::Var to break the circuit.

int main()
{
	typedef simple impl;

	// ---- define rules ----

	impl::Allocator alloc;

	std::vector<int> values;

	impl::Rule::Var rDoc;
	rDoc.assign( alloc, integer()/append(values) + !(',' + rDoc) );

	// This is only an example. A better way to do the same thing is:
	//	rDoc.assign( alloc, integer()/append(values) % ',' );

	// ---- do match ----

	if ("10,97,37" >> rDoc) {	
		for (std::vector<int>::const_iterator it = values.begin(); it != values.end(); ++it) {
			std::cout << *it << "\n";
		}
	}
	return 0;
}

// -------------------------------------------------------------------------

