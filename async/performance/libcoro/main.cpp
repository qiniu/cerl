#include <sys/time.h>
#include <stdio.h>
#include <async/Fiber.h>
#include <async/Application.h>
#define CORO_ASM 1
extern "C" {
#include <async/fiber/libcoro/coro.h>
}

coro_context* testContext = (coro_context*)malloc(sizeof(coro_context));
coro_context* mainContext = (coro_context*)malloc(sizeof(coro_context));

void cerl_callback test(void* args)
{
	for(;;)
	{
		//printf("\nthis is the test\n");
		coro_transfer(testContext, mainContext);
	}
}

int main()
{
	coro_initialize();
	enum { STACKSIZE = 4096 };

	struct timeval tpend, tpstart;
	void* stack;
	stack = malloc(STACKSIZE);
	void* args = NULL;
	int count = 0;

	coro_create(testContext, test, args, stack, STACKSIZE);
	gettimeofday(&tpstart, 0);
	while(count++ < 1000000)
	{
		coro_transfer(mainContext, testContext);
	}
	gettimeofday(&tpend, 0);
	
	float timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("libcoro 切换1百万次耗时: %f 秒\n",timeuse); 

	free(stack);
//	printf("\nreturn to the main\n");
	return 0;
}
