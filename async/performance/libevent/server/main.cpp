
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#else
#include <windows.h>
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <async/Socket.h>

#include <event.h>

struct SockData
{
	int socket;
	char data[256];
	size_t bytes;
};

int listener;
int			g_nclients = 0;
int			g_nMsg = 0, g_totalMsg;
int			g_maxCount = 100000;
timespec	g_start, g_stop;
bool		g_started = false;


void do_read(int fd, short event, void* arg);
void do_write(int fd, short event, void* arg);


void do_accept(int fd, short event, void* arg)
{
	//event_add(*(event*)arg, NULL);

	sockaddr_in addr;
	size_t	addrlen;

	int cli_sock = accept(fd, (sockaddr*)&addr, (socklen_t*)&addrlen);
	if(cli_sock == -1)
	{
		perror("accept failed: ");
		return;
	}

	SockData* sd = new SockData;
	sd->socket = cli_sock;

	::event evt;
	event_set(&evt, cli_sock, EV_READ, do_read, sd);

	event_add(&evt, NULL);
}

void do_read(int fd, short event, void* arg)
{
	if(fd != listener)
	{
		SockData* sd = (SockData*)arg;

		sd->bytes = recv(sd->socket, sd->data, 256, 0);

		::event evt;
		event_set(&evt, fd, EV_WRITE, do_write, sd);

		event_add(&evt, NULL);
	}
	else
	{
		do_accept(fd, event, arg);
	}
}

void do_write(int fd, short event, void* arg)
{
	SockData* sd = (SockData*)arg;

	sd->bytes = send(sd->socket, sd->data, sd->bytes, 0);


	++g_nMsg;
	if(!g_started)
	{
		clock_gettime(CLOCK_REALTIME, &g_start);
		g_started = true;
		printf("measure start: %lds %ldns.\n", g_start.tv_sec, g_start.tv_nsec);
	}
	if(g_nMsg >= g_maxCount)
	{
		clock_gettime(CLOCK_REALTIME, &g_stop);
		g_totalMsg = g_nMsg;
		printf("measure stop: %lds %ldns.\n", g_stop.tv_sec, g_stop.tv_nsec);

		double time = (double)(g_stop.tv_sec - g_start.tv_sec)
			+ ((double)(g_stop.tv_nsec - g_start.tv_nsec))/1000000000;

		printf("max msg: %d; total msg: %d; total time: %fs; msg per second: %f.\n", g_maxCount, g_totalMsg, time, g_totalMsg/time);

		exit(0);
	}

	::event evt;
	event_set(&evt, fd, EV_READ, do_read, sd);

	event_add(&evt, NULL);
}

int main()
{
	listener = cerl::listenSocket(5189);
	if(listener == -1)
	{
		printf("listen failed.\n");
		return 1;
	}

	event_init();

	event evt;

	event_set(&evt, listener, EV_READ, do_read, &evt);

	event_add(&evt, NULL);

	event_dispatch();
	
	return 0;
}
