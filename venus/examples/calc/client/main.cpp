#define WIN32_LEAN_AND_MEAN
#define CERL_VERBOSE

#include "../calc.h"
#include <venus/Application.h>

void cerl_callback testClient(void* lpParam)
{
	cerl::FiberParam p(lpParam);

#if (1)
	calc::Abacus abacus = cerl::connect("127.0.0.1", 8633);
#else
	cerl::Connections conns(CerlMemPools(p.self));
	cerl::NInformation local("127.0.0.1", 8633);
	calc::Abacus abacus = cerl::connect(p.self, conns, local);
#endif

	int a , b ;
	
	while (1)
	{
		printf("Input two integer, '0 0' to halt! \n> ");
		if (scanf("%d %d", &a, &b) != 2)
			break;
		if (0 == a && 0 == b)
			break;
		
		cerl::ScopedAlloc alloc(CerlBlockPool(p.self));
#if (1)
		calc::Abacus::_AddResult result;
		abacus.add(alloc, result, a, b);
#else
		calc::Abacus::_AddResult result;
		calc::Abacus::_AddArgs args = { a, b };
		cerl::multi_call(alloc, &abacus, &result, 1, 1, calc::Abacus::code_add, args);
#endif
		if (cerl::code_ok == result)
		{
			printf("Add Result: '%d'!\n", result.sum);
		}
		else
		{
			printf("Add Operation Error with code '0x%08X'!\n", result._code);
		}
	}

	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(testClient);
	return 0;
}
