//test file io through aio
//
//#define CERL_VERBOSE
#include <async/Application.h>
#include <iostream>

void cerl_callback test(LPVOID param)
{
	using namespace std;

	cerl::FiberParam p(param);

	cerl::FileObject fileObj;
	fileObj.open_to_write("test.txt");
	if (!fileObj.good())
	{
		perror("file open failed");
	}

	cerl::File file(fileObj);

	char buf[200];

	strcpy(buf, "oh, we do it.");

	printf("******************\nbefore write_some 1\n");

	ptrdiff_t n = file.write_some(buf, strlen(buf));

	printf("after write_some 1\n******************\n");

	printf("******************\nbefore read_some 1\n");

	n = file.read_some(buf, 100);

	cout << "read file 1: \n" << buf << endl;

	printf("after read_some 1\n******************\n");

	strcpy(buf, "but, just do it.");

	printf("******************\nbefore write_some 2\n");

	n = file.write_some(buf, strlen(buf));

	printf("after write_some 2\n******************\n");

	printf("******************\nbefore read_some 2\n");

	n = file.read_some(buf, 100);

	cout << "read file 2: \n" << buf << endl;

	printf("after read_some 2\n******************\n");

	cout << "leaving core fun" << endl;

	fileObj.close();

	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(test);
	return 0;
}
