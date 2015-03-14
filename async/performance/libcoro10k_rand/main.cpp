#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <async/Fiber.h>
#include <async/Application.h>
#define CORO_ASM 1
extern "C" {
#include <async/fiber/libcoro/coro.h>
}


enum { FIBERNUM = 10000 };

coro_context mainContext = {0};
coro_context testContexts[FIBERNUM];

int g_nrounds = 0;
int randNum;

void cerl_callback test(void* args)
{
	for(;;)
	{
		//printf("\nthis is the test\n");
//		coro_transfer(&testContexts[g_nrounds%FIBERNUM], &mainContext);
		coro_transfer(&testContexts[randNum], &mainContext);
	}
}

int main()
{
	coro_initialize();
	enum { STACKSIZE = 4096 };

	struct timeval tpend, tpstart;

	for (int i=0; i<FIBERNUM; ++i)
	{
		coro_create(&testContexts[i], test, NULL, malloc(STACKSIZE), STACKSIZE);
	}

	gettimeofday(&tpstart, 0);
	while(g_nrounds++ < 1000000)
	{
		randNum = rand() % FIBERNUM;
//		coro_transfer(&mainContext, &testContexts[g_nrounds%FIBERNUM]);
		coro_transfer(&mainContext, &testContexts[randNum]);
	}
	gettimeofday(&tpend, 0);
	
	float timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("libcoro 切换1百万次耗时: %f 秒\n",timeuse); 

//	printf("\nreturn to the main\n");
	return 0;
}
