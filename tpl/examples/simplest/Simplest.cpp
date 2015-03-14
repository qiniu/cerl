#include <vector>		// std::vector
#include <iostream> 	// std::cout
#include <tpl/RegExp.h>

using namespace tpl;

// -------------------------------------------------------------------------
// simplest

// A simplest example.

// What we use:
//	* Rules: /append(), %, real(), ws()
//	* Matching: tpl::simple::match()

int main()
{
	std::vector<double> values; // you can change vector to other stl containers.
	
	if ( simple::match("-.1 -0.1 +32. -22323.2e+12", real()/append(values) % ws()) )
	{
		for (std::vector<double>::iterator it = values.begin(); it != values.end(); ++it)
		{
			std::cout << *it << "\n";
		}
	}
	return 0;
}

// -------------------------------------------------------------------------

