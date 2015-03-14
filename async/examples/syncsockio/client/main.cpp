//test socket io through epoll
//
//#define CERL_VERBOSE
#include <async/Application.h>



void cerl_callback run()
{
	SOCKET socket1 = cerl::connectSocket("localhost", 5039); //"10.34.7.62", 5039);
	if(socket1 == INVALID_SOCKET)
	{
		printf("connection failed\n");
		return;
	}

	cerl::SyncSocketFile file;
	file.open_handle((SOCKET)socket1);

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
}

int main()
{
	run();
	printf("leaving main fun\n");
	return 0;
}
