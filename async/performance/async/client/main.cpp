//test socket io through epoll
//
//#define CERL_VERBOSE

#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include "../../DataFrame.h"


bool go = false;
char* addr;
const TestData dataout =
{
	{ sizeof(NS_CERL::MailHeader), 0, VENUS_MAIL_MAGIC_CODE, 0x456, (NS_CERL::FID)1},
	NS_CERL_IO::vtArrayMask | NS_CERL_IO::vtI1 | TEST_DATA_SIZE,
	{0},
	NS_CERL_IO::vtNull 
};

void cerl_callback run(LPVOID param)
{
	cerl::FiberParam p(param);

	SOCKET socket1 = cerl::connectSocket(p.self, addr, 8888); //10.34.7.62"localhost", 5039);
	if(socket1 == INVALID_SOCKET)
	{
		printf("connection failed\n");
		p.service->quit();
		return;
	}

	cerl::SocketFileObject fileObj;
	cerl::SocketFile file(p.self, fileObj);
	TestData datain;

	fileObj.open_handle(p.self, (SOCKET)socket1);

	fileObj.set_read_timeout(CERL_INFINITE);
	fileObj.set_write_timeout(CERL_INFINITE);

	printf("connection ok!.\n");

	for(int i=0; ; ++i)
	{
		if(go)
		{
			if(file.write(&dataout, sizeof(dataout)) == 0)
			{
				printf("write error.\n");
				break;
			}
            //printf("wirte successfully.\n");
			if(file.read(&datain, sizeof(datain)) == 0)
			{
				printf("read error.\n");
				break;
			}
		}
		else
		{
			Sleep(1);
		}
	}
	p.service->quit();

	fileObj.close();
}

THREADRET WINAPI clientCallback(void* param)
{
	cerl::IoService app;
	app.run(run, NULL, 16000); 
	return 0;
}

int main(int argc, char** argv)
{
	cerl::ApplicationInit init;

	if (argc != 3)
	{
		printf("Usage: %s <ConnectionCount> <IP>\n", argv[0]);
		return 1;
	}

	int ncli = 0;
	if (sscanf(argv[1], "%d", &ncli) != 1)
	{
		printf("param error.\n");
	}
	addr = argv[2];

	for(int i=0; i<ncli; ++i)
	{
		HTHREAD hThread = CreateThread(clientCallback, NULL);
		printf("start client # %d\n", i);
	}

	printf("ready...\n");
	Sleep(15000);
	go = true;
	printf("go!\n");
	Sleep(1000000);

	return 0;
}
