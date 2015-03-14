//test socket io through epoll
//
//#define CERL_VERBOSE
//#include <async/Io.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <async/Socket.h>
#include <async/Application.h>
#include <sys/times.h>



void cerl_callback run(LPVOID param)
{
	cerl::FiberParam p(param);

	int socket1 = cerl::listenSocket(5039);
	if(socket1 == -1)
	{
		p.service->quit();
		return;
	}

	cerl::ListenSocket s;
	if (s.open_handle((SOCKET)socket1) != S_OK)
	{
		CERL_VLOG("test", ("Listen Socket failed\n"));
		p.service->quit();
		close(socket1);
		return;
	}

	SOCKET	clisock = s.accept();
	if(clisock == 0)
	{
		CERL_VLOG("test", ("Accept failed\n"));
		close((int)clisock);
		p.service->quit();
		return;
	}

	cerl::SocketFileObject fileObj;
	fileObj.open_handle((SOCKET)clisock);

	cerl::SocketFile file(fileObj);

	printf("socket file opened.\n");

	//char buf[64];
	int data;

	tms tm;
	clock_t	t = times(&tm);
	printf("start: %d\n", t);

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

	NS_STDEXT::print("10000 msg in %d ms\n", (int)((double)(times(&tm)-t)*1000)/sysconf(_SC_CLK_TCK));
	fileObj.close();

	p.service->quit();
}

int main()
{
	cerl::Application	app;
	app.run(run, NULL, 16000); 
	printf("leaving main fun\n");
	return 0;
}
