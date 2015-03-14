//test socket io through epoll
//

//#define CERL_VERBOSE
#define WIN32_LEAN_AND_MEAN

#include <stdext/time/timespec.h>
#include <async/Io.h>
#include <async/Socket.h>
#include <async/Application.h>



int main(int argc, char** argv)
{
	cerl::SocketInit socketinit;
	int socket1 = cerl::listenSocket(6022);
	if(socket1 == -1)
	{
		perror("socket create failed: ");
		return 0;
	}
	printf("listening socket %d...\n", socket1);

	sockaddr_in	rmtaddr = {0};
	socklen_t	salen = sizeof(rmtaddr);

	int sockacc = accept(socket1, (sockaddr*)&rmtaddr, &salen);
	if(sockacc < 0)
	{
		perror("accept failed: ");
		close(socket1);
		return 1;
	}
	printf("accept client: socket: %d\n", sockacc);

	cerl::detail::setnonblk(sockacc);

	Sleep(10);

	char buf = 0;
	DWORD g_start = GetTickCount();
	int count = 0;
	for (; ; ++count)
	{
		int nread = recv(sockacc, &buf, sizeof(buf), 0);
		if (nread < 0)
		{
			perror("recv failed:");
			break;
		}
	}
	double time = GetTickCount() - g_start;
	printf("read socket: total count: %d; total time: %fs; msg per second: %f.\n", count, time/1000, count*1000/time);


	close(socket1);
	close(sockacc);

	return 0;
}
