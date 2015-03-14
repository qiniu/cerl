#define WIN32_LEAN_AND_MEAN
//#define CERL_VERBOSE

#include <venus/Application.h>

#include "../CalcAImpl.h"

void cerl_callback testServer(void* lpParam)
{
	cerl::FiberParam p(lpParam);

	printf("Starting Abacus...\n");

	SOCKET socket = cerl::listenSocket(8633);
	recursivecall::CalcAImpl abacus;
	cerl::Shell::enterToQuit();
	abacus._run(p.self, socket);
}

int main()
{
	cerl::Application app;
	app.run(testServer);
	return 0;
}
