//test socket io through epoll
//

//#define CERL_VERBOSE
#define WIN32_LEAN_AND_MEAN

#include <stdext/time/timespec.h>
#include <async/Io.h>
#include <async/Socket.h>
#include <async/Application.h>
#include <vector>


int			g_maxCount = 1000000;
DWORD		g_start, g_stop;
std::vector<int>	g_aConns;
int			g_nSocks = 10000;


int setnonblk(int fd)
{
	return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

int main(int argc, char** argv)
{
	int socket1 = cerl::connectSocket("localhost", 8888);
	if(socket1 == -1)
	{
		perror("socket create failed: ");
		return 0;
	}

	for (int i = 0; i < g_nSocks; ++i)
	{
		int socket1 = cerl::connectSocket("localhost", 8888);
		if(socket1 == -1)
		{
			perror("socket create failed:\n ");
			return 0;
		}
		g_aConns.push_back(socket1);
	}

	//epoll
	int epfd = epoll_create(10000);
	if(epfd == -1)
	{
		perror("listen failed: ");
		close(socket1);
		return 0;
	}

	epoll_event	ev = {0};
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.ptr = NULL;

	DWORD g_start = GetTickCount();
	for (int i = 0; i < g_nSocks; ++i)
	{
		epoll_ctl(epfd, EPOLL_CTL_ADD, g_aConns[i], &ev);
	}
	double time = GetTickCount() - g_start;
	printf("add sockets: total count: %d; total time: %fs; msg per second: %f.\n", g_nSocks, time/1000, g_nSocks*1000/time);

	g_start = GetTickCount();
	for (int i = 0; i < 1000000; ++i)
	{
		ev.data.fd = socket1;

		epoll_ctl(epfd, EPOLL_CTL_ADD, socket1, &ev);
		epoll_ctl(epfd, EPOLL_CTL_DEL, socket1, NULL);
	}
	time = GetTickCount() - g_start;
	printf("add/del socket: total count: %d; total time: %fs; msg per second: %f.\n", g_maxCount, time/1000, g_maxCount*1000/time);

	g_start = GetTickCount();
	for (int i = 0; i < g_nSocks; ++i)
	{
		epoll_ctl(epfd, EPOLL_CTL_DEL, g_aConns[i], &ev);
	}
	time = GetTickCount() - g_start;
	printf("del sockets: total count: %d; total time: %fs; msg per second: %f.\n", g_nSocks, time/1000, g_nSocks*1000/time);


	close(epfd);
	close(socket1);

	return 0;
}
