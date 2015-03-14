#define WIN32_LEAN_AND_MEAN 
#include <async/Application.h>
#include <vector>

LPVOID mainFiber = NULL;

VOID WINAPI runme(LPVOID lpFiberParameter)
{
	for (;;)
	{
		::SwitchToFiber(mainFiber);
	}
}

enum { FIBERCOUNT = 2000 };
LPVOID fibers[FIBERCOUNT];

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage: %s <switch times>", argv[0]);
		return 1;
	}

	int ntimes = 0;
	if (::sscanf(argv[1], "%d", &ntimes) != 1)
	{
		printf("usage: %s <switch times>", argv[0]);
		return 1;
	}

	mainFiber = ::GetCurrentFiber();

	int i = 0;
	DWORD start = GetTickCount();

	for(; i<FIBERCOUNT; ++i)
	{
		fibers[0] = CreateFiberEx(4096, 0, 0, runme, NULL);
		::SwitchToFiber(fibers[0]);
		::DeleteFiber(fibers[0]);
	}

	DWORD stop = GetTickCount();
	double alltime = ((double)(stop-start));

	printf("start %d fibers using %f ms, %f ms per fiber.\n", FIBERCOUNT, alltime, alltime/FIBERCOUNT);

	if (FIBERCOUNT > 2000)
		return 0;
	
	start = GetTickCount();

	for(i =0; i<ntimes; ++i)
	{
		::SwitchToFiber(fibers[i%2000]);
	}

	stop = GetTickCount();
	alltime = ((double)(stop-start));

	printf("switch %d times using %f ms, %f ms per fiber.\n", ntimes, alltime, alltime/ntimes);

	return 0;
}
