#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>

using namespace std;

int setnonblk(int fd)
{
	return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

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

	sockaddr_in myaddr = {0};
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(5039);
	inet_pton(AF_INET, "10.34.7.62", &myaddr.sin_addr);
	socklen_t	malen = sizeof(myaddr);

	if(bind(socket1, (sockaddr*)&myaddr, malen) == -1)
	{
		perror("connection bind failed: ");
		close(socket1);
		return 0;
	}
	printf("connection binded.\n");

	if(setnonblk(socket1) == -1)
	{
		perror("set nonblocking failed: ");
		close(socket1);
		return 0;
	}
	printf("set listen socket nonblocking success.\n");

	if(listen(socket1, 100) == -1)
	{
		perror("listen failed: ");
		close(socket1);
		return 0;
	}
	printf("listening...\n");

	//epoll
	int epfd = epoll_create(1000);
	if(epfd == -1)
	{
		perror("listen failed: ");
		close(socket1);
		return 0;
	}
	printf("epoll created.\n");

	epoll_event	ev = {0};
	ev.events = EPOLLIN|EPOLLET;
	ev.data.fd = socket1;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, socket1, &ev) == -1)
	{
		perror("add listener to epoll failed:\n");
		close(epfd);
		close(socket1);
		return 0;
	}
	printf("listener added to epoll\n");

	epoll_event rcvev[128];

	bool	isRun = true;
	while(isRun)
	{
		int nev = epoll_wait(epfd, rcvev, 128, -1);
		if(nev == -1)
		{
			perror("epoll wait failed:\n");
			close(epfd);
			close(socket1);
			return 0;
		}
		printf("event arrieving.\n");

		for(int i=0; i < nev; ++i)
		{
			if(rcvev[i].data.fd == socket1)
			{
				sockaddr_in	rmtaddr = {0};
				socklen_t	salen = sizeof(rmtaddr);

				printf("connection arrieves.\n");

				int sockacc = accept(socket1, (sockaddr*)&rmtaddr, &salen);
				if(sockacc == -1)
				{
					perror("accept failed: ");
					close(epfd);
					close(socket1);
					return 0;
				}

				char strAddr[20];
				inet_ntop(AF_INET, &rmtaddr.sin_addr, strAddr, 20);

				printf("connection established:\n    addr: %s, port: %d.\n", 
				strAddr, ntohs(rmtaddr.sin_port));

				if(setnonblk(socket1) == -1)
				{
					perror("set custom connection nonblocking failed: ");
					close(socket1);
					return 0;
				}
				printf("set custom socket nonblocking success.\n");

				epoll_event	evt = {0};
				evt.events = EPOLLIN | EPOLLET ;
				evt.data.fd = sockacc;
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, sockacc, &evt) == -1)
				{
					perror("add custum connetion to epoll failed:\n");
					close(epfd);
					close(socket1);
					return 0;
				}
				printf(" custum connection epoll added.\n");

			}
			else
			{
				char bufdata[200];
				epoll_event	newEvt = {0};
				newEvt.events = EPOLLET ;
				newEvt.data.fd = rcvev[i].data.fd;
				if(rcvev[i].events & (EPOLLIN))
				{
					int nres = read(rcvev[i].data.fd, bufdata, 200);
					if(nres == -1)
					{
						perror("call of read error:");
					}
					if(!strcmp(bufdata, "quit"))
					{
						isRun = false;
						break;
					}
					printf("  data arrived: %s\n", bufdata);
					newEvt.events |= EPOLLOUT;
				}
				if(rcvev[i].events & (EPOLLOUT))
				{
					int nres = write(rcvev[i].data.fd, bufdata, 200);
					if(nres == -1)
					{
						perror("call of write error:");
					}
					printf("  data send: %s\n", bufdata);
					newEvt.events |= EPOLLIN;
				}
				epoll_ctl(epfd, EPOLL_CTL_MOD, rcvev[i].data.fd, &newEvt);
			}
		}
		
	}


/*	bool isRun = true;

	while(isRun)
	{
		char buf[200];

		while(1)
		{
			read(sockacc, buf, 200);
			printf("read over. \n");
			if(!strcmp(buf, "quit"))
			{
				isRun = false;
				cout << "bye bye." << endl;
				break;
			}

			printf("recieve: %s\n", buf);

			write(sockacc, buf, strlen(buf) + 1);
		}

	}
*/
	close(epfd);
	close(socket1);

	return 1;
}
