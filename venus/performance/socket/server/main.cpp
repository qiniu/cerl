/*
 * performance test for socket api - server
 */
#define WIN32_LEAN_AND_MEAN

//#include <arpa/inet.h>
#include <async/Socket.h>
#include <stdext/thread/Thread.h>
#include "../../doio.h"
#include "../../DataFrame.h"


int			g_nclients = 0;
int			g_nMsg = 0, g_totalMsg;
int			g_maxCount = 1000000;
DWORD		g_start, g_stop;
bool		g_started = false;


THREADRET WINAPI clientCallback(void* param)
{
	SOCKET cliSock = (SOCKET)(size_t)param;

	TestData buf;

	for (;;)
	{
		if (!do_recv(cliSock, (char*)&buf, sizeof(buf)))
			break;
		do_send(cliSock, (char*)&buf, sizeof(buf));

		++g_nMsg;
		if(!g_started)
		{
			g_start = GetTickCount();
			g_started = true;
			printf("measure start: %us.\n", g_start);
		}

		if(g_nMsg >= g_maxCount)
		{
			g_stop = GetTickCount();
			g_totalMsg = g_nMsg;
			printf("measure stop: %us.\n", g_stop);
			break;
		}
	}

	--g_nclients;

	if (g_nclients <= 0)
	{
		double time = (double)(g_stop - g_start)/1000;
		printf("max msg: %d; total msg: %d; total time: %fs; msg per second: %f.\n", g_maxCount, g_totalMsg, time, g_totalMsg/time);
	}


	closesocket(cliSock);
	return 0;
}

int main()
{
	cerl::SocketInit socketinit;
	(void)socketinit;

	SOCKET listener = cerl::listenSocket(8888);
	for(;;)
	{
		sockaddr_in addr = {0};
#if defined(X_OS_LINUX)
		size_t len = sizeof(addr);
#else
		int len = sizeof(addr);
#endif
	 	SOCKET cli = accept(listener, (sockaddr*)&addr, (socklen_t*)&len);
		if(INVALID_SOCKET == cli)
		{
#if defined(X_OS_LINUX)
			printf("invalid client connection.\n");
			continue;
#elif defined(X_OS_WINDOWS)
			int err = ::WSAGetLastError();
			printf("invalid client connection: %d \n", err);
			continue;
#endif
		}

		printf("client connection established: #%d\n", g_nclients);

		HTHREAD hThread = CreateThread(clientCallback, (void*)cli);
		CloseThread(hThread);

		++g_nclients;
	}

	return 0;
}
