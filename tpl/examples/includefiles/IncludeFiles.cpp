#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/c/Lex.h>
#include <tpl/RegExp.h>
/* is */ # /* this */ include /* supported? *//*
 no problem! */ "stdext/FileBuf.h"

using namespace tpl;

// -------------------------------------------------------------------------
// list include files

#define LIST_INC_STDFILE		\
	('<' + find('>')/append(result) + '>')

#define LIST_INC_FILE			\
	('\"' + find('\"')/append(result) + '\"')
	
#define LIST_INC				\
	gr(LIST_INC_STDFILE | LIST_INC_FILE)

#define LIST_GRAMMAR			\
	*(!(gr('#') + "include" + LIST_INC) + find_strict_eol<true>())

int main()
{
	std::vector<std::string> result;

	const std::FileBuf file(__FILE__);
	file >> c_skip_non_eol_[LIST_GRAMMAR];

	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
		std::cout << *it << '\n';
	return 0;
}

// -------------------------------------------------------------------------

