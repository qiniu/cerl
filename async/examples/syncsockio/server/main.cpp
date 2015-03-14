//test socket io through epoll
//
//#define CERL_VERBOSE
//#include <async/Io.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <async/Socket.h>
#include <async/Application.h>
#include <sys/times.h>



void cerl_callback run()
{
	int socket1 = cerl::listenSocket(5039);
	if(socket1 == -1)
	{
		return;
	}

	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	SOCKET	clisock = ::accept(socket1, (sockaddr*)&addr, &addrlen);
	if(clisock == 0)
	{
		CERL_VLOG("test", ("Accept failed\n"));
		close((int)clisock);
		return;
	}

	cerl::SyncSocketFile file;
	file.open_handle((SOCKET)clisock);

	printf("socket file opened.\n");

	//char buf[64];
	int data;

	tms tm;
	clock_t	t = times(&tm);
	printf("start: %d\n", (int)t);

	for(int i =0; i<10000; i++)
	{
		//printf("******************\nwait for client\n");
		//printf("round # %d\n", i);

		ptrdiff_t n = file.read_some(&data, sizeof(data)); 

		if(n == 0)
		{
			printf("connection failed.\n");
			break;
		}

		//printf("message from client: %s\n******************\n", buf);

		//if(!strcmp(buf, "quit"))
		//	break;

		//printf("******************\nresponse to client\n");

		n = file.write_some(&data, sizeof(data));

		if(n == 0)
		{
			printf("connection failed.\n");
			break;
		}

		//printf("response over.\n******************\n");

	}

	printf("10000 msg in %f\n", ((double)(times(&tm)-t))/sysconf(_SC_CLK_TCK));
	file.close();
}

int main()
{
	run();
	printf("leaving main fun\n");
	return 0;
}
