#include <libaio.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

int main()
{
	int fd = open("1.txt", O_RDWR | O_CREAT, 0600);
	if (fd == -1)
	{
		printf("open file failed: %s\n", strerror(errno));
		return 1;
	}

	io_context_t ctx = NULL;
	if (0 != io_setup(1024, &ctx))
	{
		printf("io_setup failed: %s\n", strerror(errno));
		return 1;
	}

	int actived_events = 0;
	int offset = 0;
	
	std::string data;
	data.reserve(8 * 1024 * 1024);
	for (size_t i=0; i<1024*1024; ++i)
	{
		data += "0123456\n";
	}

	struct io_event evt;
	while (true)
	{
		/*
		if (actived_events < 50)
		{
			while (actived_events < 100)
			{
				iovec iov[2];
				iov[0].iov_base = (void*)data.c_str();
				iov[0].iov_len = data.size();
				iov[1].iov_base = (void*)"hello\n";
				iov[1].iov_len = 6;

				iocb cb;
				io_prep_pwritev(&cb, fd, iov, 2, offset);
				offset += data.size() + 6;
				struct iocb* cbs[] = {&cb};
				struct iocb** pcbs = cbs;

				int n = 0;
				if (1 != (n = io_submit(ctx, 1, pcbs)))
				{
					printf("io_submit failed: %s, ret: %d, actived_events: %d\n", strerror(errno), n, actived_events);
					return 1;
				}

				actived_events ++;
			}
		}
		*/

		int n = io_getevents(ctx, 1, 1, &evt, NULL);
		if (-1 == n)
		{
			printf("io_getevents failed: %s\n", strerror(errno));
			return 1;
		}
		if (n == 0)
		{
			printf("no events\n");
			usleep(1000 * 5);
			continue;
		}

		actived_events --;

		if (evt.res != data.size() + 6 )
		{
			printf("res failed: %d\n", evt.res);
		}
	}

	io_destroy(ctx);

	close(fd);

	return 0;
}

