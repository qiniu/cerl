#define WIN32_LEAN_AND_MEAN 
#include <async/Application.h>

cerl::Fiber fiberMain;
cerl::Fiber fiberTest;

void cerl_callback test(LPVOID lpParam)
{
	printf("now it is in %s!\n", (char*)cerl::getFiberData(fiberTest));
	cerl::switchToFiber(fiberTest, fiberMain);
}

int main()
{
	cerl::Pools pools;
	cerl::FiberInit fiberInit;
	cerl::Fibers fibers(pools);
	cerl::ThreadToFiber threadToFiber;
	fiberMain = threadToFiber.convert((LPVOID)"MainFiber");
	fiberTest = fibers.create(test, (LPVOID)"TestFiber");
	cerl::switchToFiber(fiberMain, fiberTest);
	printf("back to %s!\n", (char*)cerl::getFiberData(fiberMain));
	fibers.destroy(fiberTest);
	threadToFiber.unconvert();
	return 0;
}
