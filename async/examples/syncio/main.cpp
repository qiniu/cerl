#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <async/Buffered.h>

void cerl_callback testBufferedReader()
{
	cerl::SyncFile file;
	if (file.open_to_read(__FILE__) != S_OK)
		return;

	typedef cerl::BufferedReader<cerl::SyncFile> Reader;

	char buf[128];
	Reader reader(file, buf, sizeof(buf));
	for (;;)
	{
		Reader::range_type d = reader.get_some();
		if (d.empty())
			break;
		printf("%s", std::string(d.begin(), d.end()).c_str());
	}
}

void cerl_callback testBufferedReader2()
{
	cerl::SyncFile file;
	if (file.open_to_read(__FILE__) != S_OK)
		return;

	typedef cerl::BufferedReader<cerl::SyncFile> Reader;

	int c;
	char buf[128];
	Reader reader(file, buf, sizeof(buf));
	while ((c = reader.get()) != Reader::endch)
		putc(c, stdout);
}

void cerl_callback testBufferedReader3()
{
	cerl::SyncFile file;
	if (file.open_to_read(__FILE__) != S_OK)
		return;

	typedef cerl::BufferedReader<cerl::SyncFile> Reader;

	size_t cb;
	char buf[128];
	char buf2[165];
	Reader reader(file, buf, sizeof(buf));
	fwrite(buf2, reader.get(buf2, 65), 1, stdout);
	while ((cb = reader.get(buf2, sizeof(buf2))) != 0)
		fwrite(buf2, cb, 1, stdout);
}

// =========================================================================

int main()
{
	testBufferedReader3();
	return 0;
}

// =========================================================================
