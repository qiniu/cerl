#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <async/Direct.h>

// =========================================================================

typedef cerl::DirentExamples<void> Dirent;

void cerl_callback testFiber(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	Dirent::mkdir_byTaskRunner(p.self, "d:/test");
	Dirent::mkdir_byRunTask(p.self, "d:/test/dir1");

	cerl_runtask
	{
		Sleep(2000);
		printf("sleep done!\n");
	}

	printf("MKDIR\n");

	cerl::FileObject fileObj;
	fileObj.open_to_write("d:/test/abc.txt");

	cerl::File file(fileObj);
	file.write("Hello!", 6);
	file.write("\nHello!", 7);

	fileObj.close();

	p.service->quit();
}

// =========================================================================

int main()
{
	cerl::Application app;
	app.run(testFiber);
	return 0;
}

// =========================================================================
