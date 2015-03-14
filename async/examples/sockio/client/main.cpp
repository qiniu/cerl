//test socket io through epoll
//
//#define CERL_VERBOSE
#include <async/Application.h>



void cerl_callback run(LPVOID param)
{
	cerl::FiberParam p(param);

	SOCKET socket1 = cerl::connectSocket(p.self, "localhost", (cerl::Port)5039); //"10.34.7.62", 5039);
	if(socket1 == INVALID_SOCKET)
	{
		printf("connection failed\n");
		p.service->quit();
		return;
	}

	cerl::SocketFileObject fileObj;
	fileObj.open_handle((SOCKET)socket1);

	cerl::SocketFile file(fileObj);

	printf("socket file opened.\n");

	//char input[64];
	int res;

	for(int i=0; ; ++i)
	{
		//printf("enter words: ");
		//gets(input);
		//if(!strcmp(input, "exit"))
		//	break;
		//else
		{
			if(file.write_some(&i, sizeof(i)) == 0)
			{
				printf("write error.\n");
				break;
			}
			if(file.read_some(&res, sizeof(res)) == 0)
			{
				printf("read error.\n");
				break;
			}
			//printf("msg return: %d\n", res);
		}
	}
	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(run, NULL, 16000); 
	printf("leaving main fun\n");
	return 0;
}
