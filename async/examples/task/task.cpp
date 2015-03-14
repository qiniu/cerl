#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>

void cerl_callback doSth(LPVOID lpParam)
{
	Sleep(1000);
	printf("doSth%d done!\n", (int)(size_t)lpParam);
}

void cerl_callback testTask(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	if (p.val == NULL)
	{
		cerl::startFiber(p.self, testTask, (LPVOID)1);
		cerl::startFiber(p.self, testTask, (LPVOID)2);
	}

	cerl::runTask(p.self, doSth, (LPVOID)1);
	cerl::runTask(p.self, doSth, (LPVOID)2);
}

void cerl_callback test(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::startFiber(p.self, testTask, 0);
	
	cerl::sleep(p.self, 4000);
	printf("quit test\n");

	int ret = cerl::unlink_byRunTask(p.self, "foo.bar");
	assert(ret == -1);
	assert(errno == ENOENT);

	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(test);
	return 0;
}
