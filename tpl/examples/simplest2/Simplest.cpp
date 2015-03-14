#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/regex/Match.h> // minimization of including tpl

using namespace tpl;

// -------------------------------------------------------------------------
// simplest

// A simplest example.

// What we use:
//	* Rules: /append(), %, real(), ws()
//	* Matching: operator>>

int main()
{
	std::vector<double> values; // you can change vector to other stl containers.
	
	if ( "-.1 -0.1 +32. -22323.2e+12" >> real()/append(values) % ws() )
	{
		for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << *it << "\n";
		}
	}
	return 0;
}

// -------------------------------------------------------------------------

