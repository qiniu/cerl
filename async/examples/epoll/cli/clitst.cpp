#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <error.h>
#include <string>
#include <string.h>

using namespace std;



int main()
{
	int socket1 = socket(AF_INET, SOCK_STREAM, 0);
	if(socket1 == -1)
	{
		perror("socket create failed: ");
		close(socket1);
		return 0;
	}
	printf("socket created: %d\n", socket1);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5039);
	inet_pton(AF_INET, "192.168.1.106", &addr.sin_addr);

	if(connect(socket1, (sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("connect failed: ");
		close(socket1);
		return 0;
	}
	printf("connection established.\n");

	char input[200];

	while(1)
	{
		printf("enter words: ");
		gets(input);
		if(!strcmp(input, "exit"))
			break;
		else
		{
			write(socket1, input, strlen(input)+1);
			read(socket1, input, 200);
			printf("msg return: %s\n", input);
		}
	}

	close(socket1);

	return 1;
}
