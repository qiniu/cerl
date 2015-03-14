#define WIN32_LEAN_AND_MEAN 
#include <async/Io.h>
#include <async/Application.h>

void cerl_callback hashserverSession(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
}

void cerl_callback hashserver(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::ListenSocket s;
	if (s.open_handle(cerl::listenSocket(8888)) == S_OK)
	{
		for (;;)
		{
			const SOCKET sdAccept = s.accept();
			if (sdAccept != INVALID_SOCKET)
				p.service->startFiber(p.self, hashserverSession, (LPVOID)sdAccept);
		}
	}
	else
	{
		s.close();
		p.service->quit();
	}
}

int main()
{
	cerl::Application app;
	app.run(hashserver);
	return 0;
}
