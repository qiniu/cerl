#define WIN32_LEAN_AND_MEAN
//#define CERL_MULTI_IOSERVICE
#define CERL_VERBOSE

#include <venus/Application.h>

#include "AbacusImpl.h"

void cerl_callback testServer(void* lpParam)
{
	cerl::FiberParam p(lpParam);

	printf("Starting Abacus...\n");

	calc::AbacusImpl abacus;
	SOCKET socket = cerl::listenSocket(8633);

	cerl::Shell::enterToQuit();

	abacus._run(p.self, socket);
}

int main()
{
	cerl::Application app;
	app.run(testServer);
	return 0;
}
