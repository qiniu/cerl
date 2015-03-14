/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Shell.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Shell.h 2552 2010-04-20 01:37:45Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_SHELL_H
#define ASYNC_SHELL_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef ASYNC_IOSERVICEPOOL_H
#include "IoServicePool.h"
#endif

#ifndef ASYNC_IO_H
#include "Io.h"
#endif

#ifndef ASYNC_NINFORMATION_H
#include "NInformation.h"
#endif

#ifndef ASYNC_TIMER_H
#include "Timer.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class ApplicationInit

class ApplicationInit
{
	FiberInit fiberInit;
	SocketInit socketInit;
	AioInit aioInit;
	TimerInit timerInit;
	IoServicePoolInit servicesInit;

public:
	explicit ApplicationInit(size_t threadCount = 2, size_t timerPrecision = 500, size_t timerSlots = 31)
		: servicesInit(threadCount), timerInit(timerPrecision, timerSlots)
	{
	}
};

// -------------------------------------------------------------------------
// class Application

class Application : public ApplicationInit, public IoServicePool
{
public:
	explicit Application(size_t threadCount = 2, size_t timerPrecision = 500, size_t timerSlots = 31)
		: ApplicationInit(threadCount, timerPrecision, timerSlots)
	{
	}

public:
	class AutoQuit
	{
	public:
		~AutoQuit()
		{
			Application::quit();
		}
	};
};

// -------------------------------------------------------------------------
// class Shell

class Shell
{
private:
	struct ShellParam
	{
		FiberCallback fnShell;
		void* startParam;
		size_t dwStackSize;
	};

	static THREADRET WINAPI ThreadMain(LPVOID lpParam)
	{
		ShellParam p = *(ShellParam*)lpParam;
		free(lpParam);

		IoService service;
		service.run(p.fnShell, p.startParam, p.dwStackSize);

		return 0;
	}

	static void cerl_callback PressEnterToQuit(void* lpParam)
	{
		FiberParam p(lpParam);

		for (;;)
		{
			printf("Press enter to quit...\n");
			getchar();
			break;
		}

		p.service->quit();
		Application::quit();
	}

public:
	static void cerl_call start(FiberCallback fnShell, void* startParam = NULL, size_t dwStackSize = 0)
	{
		ShellParam* p = (ShellParam*)malloc(sizeof(ShellParam));
		p->fnShell = fnShell;
		p->startParam = startParam;
		p->dwStackSize = dwStackSize;
		CloseThread(CreateThread(ThreadMain, p));
	}

	static void cerl_call enterToQuit()
	{
		start(PressEnterToQuit);
	}
};

inline void cerl_call startShell(FiberCallback fnShell, void* startParam = NULL, size_t dwStackSize = 0)
{
	Shell::start(fnShell, startParam, dwStackSize);
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_SHELL_H */
