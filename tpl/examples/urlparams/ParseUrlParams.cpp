#include <map>			// std::map/multimap
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// parse_url_params

#define MAP				map		// can be map or multimap
#define OVERWRITE		true	// can be true or false
#define RULE			*(	\
	*ch('&') + find('=')/assign(key) + '=' + find_set<'&', EOF>()/insert<OVERWRITE>(values, key) )

int main()
{
	std::string key;
	std::MAP<std::string, std::string> values;
	
	if ( simple::match("&page=1&x=2&&lang=en&x=3", RULE) )
	{
		for (std::MAP<std::string, std::string>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << it->first << " = " << it->second << "\n";
		}
	}
	return 0;
}

// -------------------------------------------------------------------------

