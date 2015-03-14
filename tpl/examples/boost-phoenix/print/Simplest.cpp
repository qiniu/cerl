#include <iostream>
#include <tpl/RegExp.h>
#include <tpl/boost/Phoenix.h>

using namespace tpl;
using namespace phoenix;

int main()
{
	Phoenix<double> exec;
	"-.1 -0.1 +32. -22323.2e+12" >> real()/exec[std::cout << arg1 << const_('\n')] % ws();
	return 0;
}

