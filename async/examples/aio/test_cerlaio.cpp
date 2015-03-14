#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <async/Application.h>

void co_test(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	//system("rm 1.txt");
	remove("1.txt");


	int fd = open("1.txt", O_RDWR | O_CREAT, 0600);
	assert(-1 != fd);

	{
		const char* data = "hello world";
		const size_t datalen = strlen(data);

		assert(datalen == cerl::aio_pwrite(fd, data, datalen, 0));
		assert(datalen == cerl::aio_pwrite(fd, data, datalen, 2));
	}

	{
		char buf[1024];
		assert(4 == cerl::aio_pread(fd, buf, 4, 0));
		assert(memcmp(buf, "hehe", 4) == 0);

		assert(13 == cerl::aio_pread(fd, buf, 1024, 0));
		assert(memcmp(buf, "hehello world", 13) == 0);
	}

	close(fd);

	fd = open("2.txt", O_RDWR | O_CREAT, 0600);
	assert(-1 != fd);

	{
		iovec vec[2];
		vec[0].iov_base = (void*)"hello ";
		vec[0].iov_len = 6;
		vec[1].iov_base = (void*)"world";
		vec[1].iov_len = 5;

		assert(11 == cerl::aio_pwritev(fd, vec, 2, 0));
		assert(11 == cerl::aio_pwritev(fd, vec, 2, 2));
	}

	{
		char buf[3][32];
		iovec vec[3] = { {(void*)buf[0], 2}, {(void*)buf[1], 6}, {(void*)buf[2], 32} };
		assert(13 == cerl::aio_preadv(fd, vec, 3, 0));
		assert(memcmp(vec[0].iov_base, "he", 2) == 0);
		assert(memcmp(vec[1].iov_base, "hello ", 6) == 0);
		assert(memcmp(vec[2].iov_base, "world", 5) == 0);
	}

	close(fd);

	cerl::Application::quit();
}

int main()
{
	cerl::LibAioInit libAioInit;
	cerl::Application app;
	app.run(co_test);

	return 0;
}

