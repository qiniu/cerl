//test socket io through epoll
//
//#define CERL_VERBOSE
#define CERL_NO_TIMER
#include <async/Application.h>



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

	cerl::SocketFileObject sObj;
	cerl::SocketFile file(sObj);

	sObj.open_handle((SOCKET)clisock);
	printf("socket file opened.\n");

	char buf[128];

	for(;;)
	{
		printf("******************\nwait for client\n");

		ptrdiff_t n = file.read_some(buf, 128); 

		if(n == 0)
		{
			printf("connection failed.\n");
			break;
		}

		printf("message from client: %s\n******************\n", buf);

		if(!strcmp(buf, "quit"))
			break;

		printf("******************\nresponse to client\n");

		n = file.write_some(buf, n); 

		if(n == 0)
		{
			printf("connection failed.\n");
			break;
		}

		printf("response over.\n******************\n");

	}
	sObj.close();

	p.service->quit();
}

int main()
{
	cerl::Application	app;
	app.run(run, NULL, 16000); 
	printf("leaving main fun\n");
	return 0;
}
