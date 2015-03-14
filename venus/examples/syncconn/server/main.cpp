#define WIN32_LEAN_AND_MEAN
//#define CERL_VERBOSE
#define VENUS_BALANCE_SERVER
#include <async/Application.h>
#include "../TestSvrImpl.h"


int g_nclimax = 0;

void cerl_callback testServer(void* lpParam)
{
	printf("Starting test ...\n");
	cerl::FiberParam p(lpParam);
	testsvr::TestSvrImpl tester(g_nclimax);
	SOCKET socket = cerl::listenSocket(8888);
	tester._run(p.self, socket);
	p.service->quit();
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <ClientCounts>\n", argv[0]);
		return 1;
	}

	if (sscanf(argv[1], "%d", &g_nclimax) != 1)
	{
		printf("param error.\n");
		return 1;
	}

	cerl::Application app;
	app.run(testServer);
	return 0;
}
