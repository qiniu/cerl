#define WIN32_LEAN_AND_MEAN
//#define CERL_VERBOSE
#include <async/Application.h>
#include <async/Direct.h>
#include <async/Mutex.h>

struct Ctx
{
	cerl::FiberMutex* mutex;
	int count;
};

void cerl_callback testMutex(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	Ctx* ctx = (Ctx*)p.val;

	while (true)
	{
		printf("self: %p, lock '%d' try acquire\n", p.self, ctx->count);
		bool b = ctx->mutex->timed_acquire(p.self, 1300);
		printf("self: %p, lock '%d' result: %d, current: %p\n", p.self, ctx->count, b, ctx->mutex->getCurrentFiber());
		if (b)
		{
			cerl::sleep(p.self, 1000);
			ctx->mutex->release();
		}
		//printf("self: %p, unlock '%d', current: %p\n", p.self, ctx->count, ctx->mutex->getCurrentFiber());
	}
}

void cerl_callback mainProc(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	cerl::FiberMutex mutex;

	for (int i=0; i<2; i++)
	{
		Ctx* ctx = new Ctx;
		ctx->mutex = &mutex;
		ctx->count = i;

		p.service->startFiber(p.self, testMutex, ctx);
	}

	cerl::sleep(p.self, 1000 * 100);
	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(mainProc);
	return 0;
}
