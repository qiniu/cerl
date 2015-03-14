#define TPL_USE_AUTO_ALLOC
#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>
#include <stdext/FileBuf.h>
#pragma warning(disable:4010)

using namespace tpl;

// -------------------------------------------------------------------------
// remove_cpp_comments()

int main()
{
	typedef SimpleImplementation impl;

	// ---- define rules ----

	std::vector<impl::Leaf> result;

	impl::Allocator alloc;

	impl::Rule::Var rCppEol;

	rCppEol.assign( alloc,
		find_set<'\r', '\n', '\\'>() + 
		(
			'\\' + !eol() + rCppEol | eol()/append(result)
		));

	Var<char> delim;
	impl::Rule rString( alloc, ch('\'', '\"')/assign(delim) + *('\\' + ch_any() | ~delim) + delim );

	impl::Rule rItem( alloc,
		find_set<'/', '\'', '\"'>()/append(result) + 
		(
			"/*" + find<true>("*/") |		/*	I will be removed haha~  */
			"//" + rCppEol |				//	Multiline \
												comments are also allowed. haha~
			('/' | rString)/append(result)
		));
	
	impl::Rule rDoc( alloc, *rItem + done()/append(result) );

	// ---- do match ----

	const std::FileBuf file(__FILE__);
	impl::match(file, rDoc);
	
	for (std::vector<impl::Leaf>::iterator it = result.begin(); it != result.end(); ++it)
		std::cout << *it;

	return 0;
}

