//test socket io through epoll
//

//#define CERL_VERBOSE
#define ASYNC_NO_TIMED_RW
#define WIN32_LEAN_AND_MEAN
#define ASYNC_SOCKET_BUFFERED

#include <async/Application.h>

#include "../../DataFrame.h"


int			g_nclients = 0, g_nclimax = 0;
int			g_nMsg = 0, g_totalMsg;
int			g_maxCount = 500000;
DWORD		g_start, g_stop;
bool		g_started = false;


void cerl_callback clientCallback(LPVOID param)
{
	cerl::FiberParam p(param);

	cerl::SocketFileObject fileObj;
	cerl::SocketFile file(p.self, fileObj);
	SOCKET cliSock = *(SOCKET*)p.val;
	fileObj.open_handle(p.self, cliSock);
	fileObj.set_read_timeout(CERL_INFINITE);
	fileObj.set_write_timeout(CERL_INFINITE);

	TestData data;
	
	for(;;)
	{
		size_t n = file.read(&data, sizeof(data));
		//printf("data recieve %d bytes\n", n);
		if(n == 0)
			break;
		if(file.write(&data, n) == 0)
			break;
		//printf("data send %d bytes\n", n);
		if (g_started)
		{
			++g_nMsg;
			if(g_nMsg >= g_maxCount)
			{
				g_stop = GetTickCount();
				g_totalMsg = g_nMsg;
				printf("measure stop: %u.\n", (UINT)g_stop);

				double time = (double)(g_stop - g_start)/1000;
				printf("max msg: %d; total msg: %d; total time: %fs; msg per second: %f.\n", g_maxCount, g_totalMsg, time, g_totalMsg/time);
				exit(0);
			}
		}
	}

	fileObj.close();
}

void cerl_callback listener(LPVOID param)
{
	cerl::FiberParam p(param);

	SOCKET listenSocket = cerl::listenSocket(8888, 1024);
	if(listenSocket == INVALID_SOCKET)
	{
		printf("ERROR: listening port failed.\n");
		p.service->quit();
		return;
	}

	cerl::ListenSocket s;
	if (s.open_handle(p.self, listenSocket) != S_OK)
		return;

	printf("INFO: listening port ...\n");

	printf("INFO: begin accepting clients...\n");
	for (int i=0; i<g_nclimax; ++i)
	{
		SOCKET sdAccept = s.accept(p.self);
		if (sdAccept == INVALID_SOCKET)
		{
			printf("ERROR: Accepted failed!\n");
			p.service->quit();
			return;
		}
		printf(" Accepted a client! #: %d, fd: %d\n", g_nclients, sdAccept);

		int nRet, opt;
		opt =1;
		nRet = setsockopt(sdAccept, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: createSocket - setsockopt failed!\n"));

		int rcvbfsz = -1, sndbfsz = -1;
		socklen_t nto = sizeof(rcvbfsz);
		getsockopt(sdAccept, SOL_SOCKET, SO_RCVBUF, &rcvbfsz, &nto);
		getsockopt(sdAccept, SOL_SOCKET, SO_SNDBUF, &sndbfsz, &nto);
		printf("read buffer: %d, send buffer: %d\n", rcvbfsz, sndbfsz);

		p.service->startFiber(p.self, clientCallback, &sdAccept);
		++g_nclients;
	}
	printf("INFO: %d clients have been accepted.\n", g_nclimax);

	Sleep(16000);

	printf("INFO: begin test ...\n");
	g_start = GetTickCount();
	g_started = true;
	g_nMsg = 0;
	printf("measure start: %u.\n", (UINT)g_start);

	cerl::yield(p.self);

	s.close();

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
	app.run(listener, NULL); 
	printf("leaving main fun\n");
	return 0;
}
