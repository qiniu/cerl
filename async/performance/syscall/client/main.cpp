/*
 * performance test of socket api - client
 */


#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <venus/Debug.h>
#include <venus/Mail.h>

const int bufsz = 32000;


int main(int argc, char** argv)
{
	SOCKET sock = cerl::connectSocket("127.0.0.1", 6022); //localhost
	if (INVALID_SOCKET == sock)
	{
		perror("socket connection failed.\n");
	}
	printf("socket connect success socket: %d.\n", sock);
	
	cerl::detail::setnonblk(sock);

	char buf[bufsz];

	//DWORD g_start = GetTickCount();
	//int count = 0;
	//for(int i=0; i < 100000; ++i)
	{
		int nsent = send(sock, buf, sizeof(buf), 0);
		if(nsent == -1)
		{
			perror("send data error!\n");
			exit(1);
		}

		printf("sent %d bytes.\n", nsent);
		//++count;
	}
	//double time = GetTickCount() - g_start;
	//printf("read socket: total count: %d; total time: %fs; msg per second: %f.\n", count, time/1000, count*1000/time);

	Sleep(100000);
	
	close(sock);

	return 0;
}
