#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <async/Buffered.h>

void cerl_callback testBufferedReader(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::FileObject fileObj;
	if (fileObj.open_to_read(__FILE__) != S_OK)
		return;

	cerl::File file(fileObj);

	typedef cerl::BufferedReader<cerl::File> Reader;

	char buf[128];
	Reader reader(file, buf, sizeof(buf));
	for (;;)
	{
		Reader::range_type d = reader.get_some();
		if (d.empty())
			break;
		printf("%s", std::string(d.begin(), d.end()).c_str());
	}
	p.service->quit();
}

void cerl_callback testBufferedReader2(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::FileObject fileObj;
	if (fileObj.open_to_read(__FILE__) != S_OK)
		return;

	cerl::File file(fileObj);

	typedef cerl::BufferedReader<cerl::File> Reader;

	int c;
	char buf[128];
	Reader reader(file, buf, sizeof(buf));
	while ((c = reader.get()) != Reader::endch)
		putc(c, stdout);
	p.service->quit();
}

void cerl_callback testBufferedReader3(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::FileObject fileObj;
	if (fileObj.open_to_read(__FILE__) != S_OK)
		return;

	cerl::File file(fileObj);

	typedef cerl::BufferedReader<cerl::File> Reader;

	size_t cb;
	char buf[128];
	char buf2[165];
	Reader reader(file, buf, sizeof(buf));
	fwrite(buf2, reader.get(buf2, 65), 1, stdout);
	while ((cb = reader.get(buf2, sizeof(buf2))) != 0)
		fwrite(buf2, cb, 1, stdout);
	p.service->quit();
}

// =========================================================================

int main()
{
	cerl::Application app;
	app.run(testBufferedReader3);
	return 0;
}

// =========================================================================
