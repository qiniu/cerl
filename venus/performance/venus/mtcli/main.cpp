/*
 * performance test of socket api - client
 */


#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <venus/Debug.h>
#include <venus/Mail.h>
#include "../../DataFrame.h"
#include "../../doio.h"


bool go = false;
const char* ip;
const TestData data =
{
	{ sizeof(NS_CERL::MailHeader), 0, VENUS_MAIL_MAGIC_CODE, 0x456, (NS_CERL::FID)1},
	NS_CERL_IO::vtArrayMask | NS_CERL_IO::vtI1 | TEST_DATA_SIZE,
	{0},
	NS_CERL_IO::vtNull 
};

THREADRET WINAPI clientCallback(void* param)
{
	printf("client thread started.\n");
	SOCKET sock = cerl::connectSocket(ip, 8888); //localhost
	if (INVALID_SOCKET == sock)
	{
		printf("socket connection failed.\n");
	}
	printf("socket connect success.\n");

	TestData recvdata;

	for(int i=0;; ++i)
	{
		if(go)
		{
			if (!do_send(sock, (const char*)&data, sizeof(data)))
			{
				perror("send data error!\n");
				break;
			}
			//printf("send data %d bytes.\n", sizeof(data));
			if (!do_recv(sock, (char*)&recvdata, sizeof(recvdata)))
			{
				perror("data receive error!\n");
				break;
			}
			//printf("receive data %d bytes.\n", sizeof(data));
		}
		else
		{
			Sleep(100);
		}
	}
	
	closesocket(sock);
	return 0;
}

int main(int argc, char** argv)
{
	cerl::ApplicationInit init(0);
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
	ip = argv[2];

	for (int i=0; i<ncli; ++i)
	{
		printf("starting client #%d.\n", i);
		HTHREAD hThread = CreateThread(clientCallback, NULL);
		if (hThread == 0)
		{
			printf("client thread start failed.\n");
			break;
		}
		CloseThread(hThread);
		Sleep(100);
	}

	printf("ready...\n");
	Sleep(15000);
	go = true;
	printf("go!\n");
	Sleep(1000000);

	return 0;
}
