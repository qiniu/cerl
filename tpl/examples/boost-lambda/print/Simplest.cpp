#include <iostream>
#include <tpl/RegExp.h>
#include <tpl/boost/Lambda.h>

using namespace tpl;
using namespace boost::lambda;

int main()
{
	Lambda<double> exec;
	"-.1 -0.1 +32. -22323.2e+12" >> real()/exec[std::cout << _1 << constant('\n')] % ws();
	return 0;
}

