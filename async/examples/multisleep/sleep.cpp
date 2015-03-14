#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>

// =========================================================================

void cerl_callback testTimer(LPVOID lpParam)
{
	printf("start testTimer fiber\n");

	cerl::FiberParam p(lpParam);

	for (int i = 0; i < 5; ++i)
	{
		cerl::sleep(p.self, 1000);
		printf("sleep - %d\n", i);
	}
}

void cerl_callback mainProc(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	for (int i=0; i<10; i++)
	{
		p.service->startFiber(p.self, testTimer, NULL);
	}
	cerl::sleep(p.self, 1000000);
}

int main()
{
	cerl::Application app;
	app.run(mainProc);
	return 0;
}

// =========================================================================
