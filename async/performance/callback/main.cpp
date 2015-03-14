#include <sys/time.h>
#include <stdio.h>

void test()
{
}

int main()
{
	struct timeval tpend, tpstart;
	int count = 0;

	gettimeofday(&tpstart, 0);
	while(count++ < 1000000)
		test();
	gettimeofday(&tpend, 0);
	
	float timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec; 
	timeuse /= 1000000; 
	printf("callback 切换1百万次耗时: %f 秒\n",timeuse); 
	return 0;
}
