#define WIN32_LEAN_AND_MEAN
//#define VENUS_CONNECTION_IMPL_DATAQUE
//#define CERL_VERBOSE

#include <async/Application.h>
#include "../sdl_testsvr.h"


bool go = false;
char* ip;


void cerl_callback testClient()
{
	cerl::Pools pools;
	cerl::BlockPool recycle;
	cerl::Pool& pool = pools.get_pool(VENUS_CONNECTION_BUFSIZE);
	cerl::SyncConnection conn(cerl::NInformation(ip, 8888), pool);

	if (conn.good())
	{
		printf("connection established.\n");
	}
	else
	{
		return;
	}

	testsvr::SyncTestSvr tester(conn);
	testsvr::TestSvr::_TestResult res;
	testsvr::MsgData data;

	printf("size of data is %zu bytes.\n", sizeof(data));

	for (int i = 0; ; ++i)
	{
		if(go)
		{
			//printf("send #%d\n", i);
			cerl::ScopedAlloc alloc(recycle);
			tester.test(alloc, res, data);
			if (testsvr::code_ok != res && testsvr::code_true != res)
			{
				printf("test success count: %d\n", i);
				break;
			}
		}
		else
		{
			Sleep(1);
		}
	}
}

THREADRET WINAPI clientCallback(void* param)
{
	testClient();
	return 0;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <ConnectionCount> <IP>\n", argv[0]);
		return 1;
	}

	int ncli = 0;
	if (sscanf(argv[1], "%d", &ncli) != 1)
	{
		printf("param error.\n");
	}
	ip = argv[2];

	for (int i=0; i<ncli; ++i)
	{
		CreateThread(clientCallback, NULL);
		printf("start client # %d\n", i);
		Sleep(100);
	}

	printf("ready...\n");
	Sleep(15000);
	go = true;
	printf("go!\n");
	Sleep(1000000);

	return 0;
}
