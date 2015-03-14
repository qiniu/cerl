#define WIN32_LEAN_AND_MEAN
//#define CERL_VERBOSE
#include <async/Application.h>
#include <async/Direct.h>
#include <async/Mutex.h>
#include <async/Future.h>

void cerl_callback futureM1(LPVOID lpParam)
{
	printf("---> future1\n");

	cerl::FutureParam p(lpParam);
	cerl::FiberNestableMutex& mutex = *(cerl::FiberNestableMutex*)p.val;

	cerl::mkdir(p.self, "d:/test");
	printf("future1: mkdir d:/test\n");

	cerl::FileObject fileObj;
	fileObj.open_to_write("d:/test/abc1.txt");
	printf("future1: open_to_write\n");

	cerl::File file(fileObj);

	{
		cerl::FiberNestableMutex::scoped_lock lock2(p.self, mutex);
		printf("future1: begin write Hello1\n");
		file.write("Hello1!", 7);
		printf("future1: write Hello1\n");
	}

	printf("future1: begin write Hello\n");
	file.write("\nHello!", 7);
	printf("future1: write Hello\n");

	fileObj.close();

	printf("finished future1\n");
}

void cerl_callback futureM2(LPVOID lpParam)
{
	printf("---> future2\n");

	cerl::FutureParam p(lpParam);
	cerl::FiberNestableMutex& mutex = *(cerl::FiberNestableMutex*)p.val;

	cerl::FiberNestableMutex::scoped_lock lock(p.self, mutex);

	cerl::mkdir(p.self, "d:/test");
	printf("future2: mkdir d:/test\n");

	cerl::FileObject fileObj;
	fileObj.open_to_write("d:/test/abc2.txt");
	printf("future2: open_to_write\n");

	cerl::File file(fileObj);

	{
		cerl::FiberNestableMutex::scoped_lock lock2(p.self, mutex);
		printf("future2: begin write Hello2\n");
		file.write("Hello2!", 7);
		printf("future2: write Hello2\n");
	}

	printf("future2: begin write Hello\n");
	file.write("\nHello!", 7);
	printf("future2: write Hello\n");

	fileObj.close();

	printf("finished future2\n");
}

void cerl_callback testMutex(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	cerl::FiberNestableMutex mutex;

	cerl::Future f1(p.self, futureM1, &mutex);
	cerl::Future f2(p.self, futureM2, &mutex);

	printf("---> back testMutex\n");

	f1.wait(p.self);
	f2.wait(p.self);

	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(testMutex);
	return 0;
}
