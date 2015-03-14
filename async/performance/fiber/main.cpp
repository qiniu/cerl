#define WIN32_LEAN_AND_MEAN 

#include <async/Application.h>

cerl::Fiber fiberMain;
cerl::Fiber fiberTest;

void cerl_callback test(LPVOID lpParam)
{
//	printf("now it is in %s!\n", (char*)cerl::getFiberData(fiberTest));
	for (;;)
	{
		cerl::switchToFiber(fiberTest, fiberMain);
	}
}

int main(int argc, char** argv)
{
	int ntimes;
	if (argc != 2 || sscanf(argv[1], "%d", &ntimes) != 1)
	{
		printf("usage: %s <run times>.\n", argv[0]);
		return 1;
	}

	struct timeval tpend, tpstart;
	cerl::Pools pools;
	cerl::FiberInit fiberInit;
	cerl::Fibers fibers(pools);
	cerl::ThreadToFiber threadToFiber;
	LPVOID arg = NULL;
	fiberMain = threadToFiber.convert((LPVOID)"MainFiber");

	int count = 0;
	DWORD start = GetTickCount();
	while (count++ < ntimes)
	{
		fiberTest = fibers.create(test, (LPVOID)"TestFiber");
		cerl::switchToFiber(fiberMain, fiberTest);
		fibers.destroy(fiberTest);
	}

	threadToFiber.unconvert();

	double timeuse = GetTickCount() - start;
	printf("fiber create+switch+delete %d times use %fms, %fms/round\n", ntimes, timeuse, timeuse/ntimes);
	return 0;
}
