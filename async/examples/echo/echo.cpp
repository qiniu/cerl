//#define CERL_VERBOSE 1
#define WIN32_LEAN_AND_MEAN
#include <async/Application.h>

// =========================================================================

void cerl_callback echoSession(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	char buf[256];

	cerl::SocketFileObject sObj;
	sObj.open_handle((SOCKET)(size_t)(p.val));

	cerl::SocketFile s(sObj);
//	s.set_read_timeout(CERL_INFINITE);
	for (;;)
	{
		const size_t cb = s.read_some(buf, sizeof(buf));
		if (cb == 0) // failed
		{
			sObj.dbg_close("ACCEPT");
			return;
		}
		s.write(buf, cb);
	}
}

void cerl_callback echoServer(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::ListenSocket s;
	if (s.open_handle(cerl::listenSocket(8888)) == S_OK)
	{
		for (;;)
		{
			const SOCKET sdAccept = s.accept();
			if (sdAccept != INVALID_SOCKET)
				p.service->startFiber(p.self, echoSession, (LPVOID)sdAccept);
		}
	}
	else
	{
		s.close();
		p.service->quit();
	}
}

// =========================================================================

int main()
{
	cerl::Application app;
	app.run(echoServer);
	return 0;
}

// =========================================================================
