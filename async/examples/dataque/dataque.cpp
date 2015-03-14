#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>
#include <async/DataQueue.h>
#include <async/Buffered.h>

struct QSessionParam
{
	cerl::Pool* pool;
	SOCKET sd;
};

void cerl_callback dataQSession(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	const QSessionParam qsp = *(QSessionParam*)p.val;

	cerl::SocketFileObject sObj;
	sObj.open_handle(qsp.sd);

	cerl::SocketFile s(sObj);

	char buf[16];
	std::vector<char> line;
	cerl::BufferedReader<cerl::SocketFile> ar(s, buf, sizeof(buf));
	cerl::DataQueue<cerl::SocketFileObject> dq(sObj, *qsp.pool);
	dq.start(p.self);
	for (;;)
	{
		if (!cerl::getline(ar, line)) // failed
		{
			sObj.dbg_close("ACCEPT");
			return;
		}
		line.push_back('\0');
		printf("%s\n", &line[0]);
		if (line.size() == 1 && line[0] == 'q')
		{
			p.service->quit();
			return;
		}
		dq.put_message(p.self, &line[0], line.size());
	}
}

void cerl_callback testDataQueue(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	cerl::Pools& pools = p.service->pools;

	QSessionParam qsp = { &pools.get_pool(512), INVALID_SOCKET };

	cerl::ListenSocket s;
	if (s.open_handle(cerl::listenSocket(8888)) == S_OK)
	{
		for (;;)
		{
			const SOCKET sdAccept = s.accept();
			if (sdAccept != INVALID_SOCKET)
			{
				qsp.sd = sdAccept;
				p.service->startFiber(p.self, dataQSession, &qsp);
			}
		}
	}
	else
	{
		s.close();
		p.service->quit();
	}

	p.service->quit();
}

// =========================================================================

int main()
{
	cerl::Application app;
	app.run(testDataQueue);
	return 0;
}

// =========================================================================
