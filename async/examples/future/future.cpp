#define WIN32_LEAN_AND_MEAN
//#define CERL_VERBOSE
#include <async/Application.h>
#include <async/Future.h>
#include <async/Direct.h>

void cerl_callback future1(LPVOID lpParam)
{
	cerl::FutureParam p(lpParam);

	cerl::mkdir(p.self, "d:/test");
	printf("future1: mkdir d:/test\n");

	cerl::FileObject fileObj;
	fileObj.open_to_write("d:/test/abc1.txt");
	printf("future1: open_to_write\n");

	cerl::File file(fileObj);

	printf("future1: begin write Hello1\n");
	file.write("Hello1!", 7);
	printf("future1: write Hello1\n");

	printf("future1: begin write Hello\n");
	file.write("\nHello!", 7);
	printf("future1: write Hello\n");

	fileObj.close();

	printf("finished future1\n");
}

void cerl_callback future2(LPVOID lpParam)
{
	cerl::FutureParam p(lpParam);

	cerl::mkdir(p.self, "d:/test");
	printf("future2: mkdir d:/test\n");

	cerl::FileObject fileObj;
	fileObj.open_to_write("d:/test/abc2.txt");
	printf("future2: open_to_write\n");

	cerl::File file(fileObj);

	printf("future2: begin write Hello2\n");
	file.write("Hello2!", 7);
	printf("future2: write Hello2\n");

	printf("future2: begin write Hello\n");
	file.write("\nHello!", 7);
	printf("future2: write Hello\n");

	fileObj.close();

	printf("finished future2\n");
}

void cerl_callback testFuture(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	
	cerl::Future f1(p.self, future1);
	cerl::Future f2(p.self, future2);

	f1.wait(p.self);
	f2.wait(p.self);

	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(testFuture);
	return 0;
}
