#define TPL_USE_AUTO_ALLOC
#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/c/Lex.h>	// cpp_comment()
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
	
	impl::Rule rItem( alloc,
		find_set<'/', '\'', '\"'>()/append(result) + 
		(
			cpp_comment() |		/*	I will be removed haha~  */
								//	Multiline \
									comments are also allowed. haha~
			('/' | c_string_or_char())/append(result)
		));
	
	// ---- do match ----

	const std::FileBuf file(__FILE__);
	file >> *rItem + done()/append(result);

	for (std::vector<impl::Leaf>::iterator it = result.begin(); it != result.end(); ++it)
		std::cout << *it;
		
	return 0;
}

