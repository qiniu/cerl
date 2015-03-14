#define WIN32_LEAN_AND_MEAN
//#define VENUS_CONNECTION_IMPL_DATAQUE
//#define CERL_VERBOSE

#include <async/Application.h>
#include "../sdl_testsvr.h"


bool go = false;
char* ip;


void cerl_callback testClient(void* lpParam)
{
	cerl::FiberParam p(lpParam);

	NS_STDEXT::DefaultAlloc alloc;
	cerl::Connections conns(alloc);
	cerl::Connection conn = cerl::connect(conns, cerl::NInformation(ip, 8888));

	if (conn.good())
	{
		printf("connection established.\n");
	}
	else
	{
		p.service->quit();
	}

	testsvr::TestSvr tester(conn);
	testsvr::TestSvr::_TestResult res;
	testsvr::MsgData data;

	printf("size of data is %zu bytes.\n", sizeof(data));

	for (int i = 0; ; ++i)
	{
		if(go)
		{
			//printf("send #%d\n", i);
			cerl::ScopedAlloc alloc(CerlBlockPool(p.self));
			tester.test(alloc, res, data);
			if (testsvr::code_ok != res && testsvr::code_true != res)
			{
				printf("call success count: %d\n", i);
				// printf("do rpc failed\n");
				//break;
				cerl::sleep(p.self, 1000);
			}
		}
		else
		{
			Sleep(1);
		}
	}

	p.service->quit();
}

THREADRET WINAPI clientCallback(void* param)
{
	cerl::IoService app;
	app.run(testClient); 
	return 0;
}

int main(int argc, char** argv)
{
	cerl::ApplicationInit init;
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
	Sleep(3000);
	go = true;
	printf("go!\n");
	Sleep(1000000);

	return 0;
}
