#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <async/Event.h>

// -------------------------------------------------------------------------

void cerl_callback job1(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	cerl::FiberEvent::Notifier ev((cerl::FiberEvent*)p.val);

	cerl::timed_yield(p.self, 1000);

	ev.notify(p.self);

	printf("job1 notified!\n");
}

void cerl_callback job2(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	cerl::FiberEvent::Notifier ev((cerl::FiberEvent*)p.val);

	cerl::timed_yield(p.self, 2000);

	ev.notify(p.self);

	printf("job2 notified!\n");
}

void cerl_callback testEvent(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	
	cerl::FiberEvent ev;

	p.service->startFiber(p.self, job1, &ev);
	p.service->startFiber(p.self, job2, &ev);
	
	printf("waiting ...\n");
	bool fOk = ev.timed_wait(p.self, 1000);
	printf("wait done: %d!\n", (int)fOk);

	cerl::timed_yield(p.self, 2000);

	p.service->quit();
}

// -------------------------------------------------------------------------

int main()
{
	cerl::Application app;
	app.run(testEvent);
	return 0;
}
