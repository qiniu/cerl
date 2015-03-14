#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// simple_grammar

// A simple grammar example.

// What we use:
//	* Rules: /append(), %, real(), gr(','), skipws()
//	* Matching: operator>>

int main()
{
	std::vector<double> values; // you can change vector to other stl containers.
	
	if ( " -.1 , -0.1 , +32. , -22323.2e+12 " >> skipws_[real()/append(values) % gr(',')] )
	{
		for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << *it << "\n";
		}
	}
	return 0;
}

// -------------------------------------------------------------------------

