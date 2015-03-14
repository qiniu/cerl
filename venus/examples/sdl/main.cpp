#define WIN32_LEAN_AND_MEAN
//#define CERL_VERBOSE
//#define VENUS_MAILTRACE

#include <async/Application.h>

#include "CalcImpl.h"

void cerl_callback testServer(void* lpParam)
{
	printf("Starting Abacus...\n");
	cerl::FiberParam p(lpParam);
	userfunc::CalcImpl abacus;
	SOCKET socket = cerl::listenSocket(8633);
	abacus._run(p.self, socket);
	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(testServer);
	return 0;
}
