#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>

// =========================================================================

void cerl_callback testTimer(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	for (int i = 0; i < 5000; ++i)
	{
		cerl::sleep(p.self, 500);
		printf("sleep - %d\n", i);
	}
	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(testTimer);
	return 0;
}

// =========================================================================
