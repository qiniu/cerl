#define WIN32_LEAN_AND_MEAN 
#include <async/Application.h>

void cerl_callback test(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	printf("now it is in 'test' fiber!\n");
	p.service->quit();
}

int main()
{
	cerl::FiberInit fiberInit;
	cerl::IoService service;
	service.run(test);
	return 0;
}
