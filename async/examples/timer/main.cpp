#define WIN32_LEAN_AND_MEAN 
#include <async/FastTimer.h>

// =========================================================================

DWORD g_tick;

class MyTimer : public cerl::FastTimer
{
public:
	void cerl_call onTimer(void* caller)
	{
		printf("onTimer: %d\n", (int)(GetTickCount() - g_tick));
	}

	void cerl_call release()
	{
		delete this;
	}
};

int main()
{
	cerl::FastTimerQueue tq;

	MyTimer* timer1 = new MyTimer;
	MyTimer* timer2 = new MyTimer;
	MyTimer* timer3 = new MyTimer;

	g_tick = GetTickCount();
	tq.start(timer1, 1000);
	tq.start(timer2, 2000);
	tq.start(timer3, 0);

	timer2->stop();
	timer2->release();

	tq.restart(timer3, 2000);

	for (size_t i = 0; i < 5; ++i)
	{
		tq.tick();
		Sleep(tq.getPrecision());
	}

	timer1->release();
	timer3->release();

	return 0;
}
