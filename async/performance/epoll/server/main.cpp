//test socket io through epoll
//

//#define CERL_VERBOSE
#define WIN32_LEAN_AND_MEAN

#include <stdext/time/timespec.h>
#include <async/Io.h>
#include <async/Socket.h>
#include "../../doio.h"
#include "../../DataFrame.h"
#include <async/Application.h>


struct SockData
{
	int epfd;
	int	socket;
	TestData data;
	size_t size;
};

int			g_nclients = 0, g_nclimax = 0;
int			g_nMsg = 0, g_totalMsg;
int			g_maxCount = 1000000;
DWORD		g_start, g_stop;
bool		g_started = false;


int setnonblk(int fd)
{
	return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

void dorw(epoll_event& evt, SockData* sd)
{
	
	epoll_event	newEvt = {0};
	newEvt.data.ptr = sd;
	if(evt.events & (EPOLLIN))
	{
		sd->size = do_recv(sd->socket, (char*)&(sd->data), sizeof(sd->data));
		if(sd->size <= 0)
		{
			perror("call of recv error:");
			close(sd->socket);
			close(sd->epfd);
			delete sd;
			exit(1);
		}
		newEvt.events = EPOLLOUT;
	}
	if(evt.events & (EPOLLOUT))
	{
		sd->size = do_send(sd->socket, (char*)&(sd->data), sizeof(sd->data));
		if(sd->size <= 0)
		{
			perror("call of send error:");
			close(sd->socket);
			close(sd->epfd);
			delete sd;
			exit(1);
		}
		newEvt.events = EPOLLIN;
	}

	epoll_ctl(sd->epfd, EPOLL_CTL_MOD, sd->socket, &newEvt);

	if(g_started)
	{
		++g_nMsg;
		if(g_nMsg >= g_maxCount)
		{
			g_stop = GetTickCount();
			g_totalMsg = g_nMsg;
			printf("measure stop: %u.\n", g_stop);

			double time = (double)(g_stop - g_start)/1000;
			printf("max msg: %d; total msg: %d; total time: %fs; msg per second: %f.\n", g_maxCount, g_totalMsg, time, g_totalMsg/time);
			close(sd->epfd);
			exit(0);
		}
	}
}

void doaccept(epoll_event& evt, SockData* sd)
{
	sockaddr_in	rmtaddr = {0};
	socklen_t	salen = sizeof(rmtaddr);

	printf("connection arrieves.\n");

	int sockacc = accept(sd->socket, (sockaddr*)&rmtaddr, &salen);
	if(sockacc == -1)
	{
		perror("accept failed: ");
		close(sd->epfd);
		close(sd->socket);
		exit(1);
	}

	if(setnonblk(sd->socket) == -1)
	{
		perror("set custom connection nonblocking failed: ");
		close(sd->socket);
		exit(1);
	}

	int nRet, opt;
	opt =1;
	nRet = setsockopt(sockacc, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: createSocket - setsockopt failed!\n"));

	int rcvbfsz = -1, sndbfsz = -1;
	socklen_t nto = sizeof(rcvbfsz);
	getsockopt(sockacc, SOL_SOCKET, SO_RCVBUF, &rcvbfsz, &nto);
	getsockopt(sockacc, SOL_SOCKET, SO_SNDBUF, &sndbfsz, &nto);
	printf("read buffer: %d, send buffer: %d\n", rcvbfsz, sndbfsz);

	SockData*	sdcli = new SockData;
	sdcli->epfd = sd->epfd;
	sdcli->socket = sockacc;

	epoll_event	evtcli = {0};
	evtcli.events = EPOLLIN;
	evtcli.data.ptr = sdcli;
	if(epoll_ctl(sd->epfd, EPOLL_CTL_ADD, sockacc, &evtcli) == -1)
	{
		perror("add custum connetion to epoll failed:\n");
		close(sd->epfd);
		close(sd->socket);
		exit(1);
	}
	printf("connection established: %d.\n", g_nclients);
	++g_nclients;
	if (g_nclients>=g_nclimax)
	{
		epoll_ctl(sd->epfd, EPOLL_CTL_DEL, sd->socket, NULL);
		printf("INFO: %d clients have been accepted.\n", g_nclimax);

		Sleep(16000);

		printf("INFO: begin test ...\n");
		g_start = GetTickCount();
		g_started = true;
		g_nMsg = 0;
		printf("measure start: %u.\n", (UINT)g_start);
	}
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

	int socket1 = cerl::listenSocket(8888);
	if(socket1 == -1)
	{
		perror("socket create failed: ");
		return 0;
	}

	//epoll
	int epfd = epoll_create(1000);
	if(epfd == -1)
	{
		perror("listen failed: ");
		close(socket1);
		return 0;
	}

	SockData listenSock = {0};
	listenSock.epfd = epfd;
	listenSock.socket = socket1;

	epoll_event	ev = {0};
	ev.events = EPOLLIN;
	ev.data.ptr = &listenSock;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, socket1, &ev) == -1)
	{
		perror("add listener to epoll failed:\n");
		close(epfd);
		close(socket1);
		return 0;
	}
	printf("listener %d added to epoll\n", socket1);

	epoll_event rcvev[128];

	bool isRun = true;

	while(isRun)
	{
		int nev = epoll_wait(epfd, rcvev, 128, -1);
		if(nev == -1)
		{
			perror("epoll wait failed:\n");
			close(epfd);
			close(socket1);
			return 0;
		}

		for(int i=0; i < nev; ++i)
		{
			SockData*	sd = (SockData*)rcvev[i].data.ptr;
			if(sd->socket != socket1)
			{
				dorw(rcvev[i], sd);
			}
			else
			{
				doaccept(rcvev[i], sd);
			}
		}
	}
	close(socket1);
	close(epfd);

	return 0;
}
