/*
	Description: 
	Author:			SDL Compiler
*/

#ifndef TESTSVR_TESTSVR_IMPL_H
#define TESTSVR_TESTSVR_IMPL_H

#ifndef SDL_TESTSVR_BASE_H
#include "sdl_testsvr_base.h"
#endif

#ifndef VENUS_SERVER_H
#include <venus/Server.h>
#endif

namespace testsvr {

class TestSvrImpl : public cerl::Server, public TestSvrBase 
{
	int			m_nclimax, m_nclients;
	int			m_nMsg, m_totalMsg;
	int			m_maxCount;
	DWORD		m_start, m_stop;
	bool		m_started;

public:
	void cerl_call onAccept(cerl::Caller* caller)
	{
		SOCKET cliSock = caller->getSocket().resource.get_handle();
		printf(" Accepted a client! #: %d, fd: %d\n", m_nclients, cliSock);

		int nRet, opt;
		opt =1;
		nRet = setsockopt(cliSock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt));
		CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: createSocket - setsockopt failed!\n"));

		int rcvbfsz = -1, sndbfsz = -1;
#if defined(X_OS_LINUX)
		socklen_t nto = sizeof(rcvbfsz);
#else
		int nto = sizeof(rcvbfsz);
#endif
		getsockopt(cliSock, SOL_SOCKET, SO_RCVBUF, (char*)&rcvbfsz, &nto);
		getsockopt(cliSock, SOL_SOCKET, SO_SNDBUF, (char*)&sndbfsz, &nto);
		printf("read buffer: %d, send buffer: %d\n", rcvbfsz, sndbfsz);

		++m_nclients;
		if (m_nclients >= m_nclimax)
		{
			printf("INFO: %d clients have been accepted.\n", m_nclimax);

			//Sleep(3000);

			printf("INFO: begin test ...\n");
			m_start = GetTickCount();
			m_started = true;
			m_nMsg = 0;
			printf("measure start: %u.\n", (UINT)m_start);
		}
	}

public:
	explicit TestSvrImpl(int totalClient)
		: m_nclimax(totalClient), m_nclients(0), m_nMsg(0), m_totalMsg(0),
		m_maxCount(10000), m_start(0), m_stop(0), m_started(false)
	{
	}

	void cerl_call test(
		cerl::Caller* caller,
		_TestResult& result,
		const MsgData& val)
	{
		//printf("receive: #%d. %d bytes\n", m_nMsg, sizeof(val));
		result = code_ok;
		if(m_started)
		{
			++m_nMsg;
			if(m_nMsg >= m_maxCount)
			{
				m_stop = GetTickCount();
				m_totalMsg = m_nMsg;
				printf("measure stop: %u.\n", (UINT)m_stop);

				double time = (double)(m_stop - m_start)/1000;
				printf("max msg: %d; total msg: %d; total time: %fs; msg per second: %f.\n", m_maxCount, m_totalMsg, time, m_totalMsg/time);
				cerl::Application::quit();
				//exit(0);
			}
		}
	}

	void cerl_call _run(cerl::Fiber self, SOCKET socketListen);
};

} //namespace

#include "TestSvrStub.h"

#endif /* TESTSVR_TESTSVR_IMPL_H */ 
