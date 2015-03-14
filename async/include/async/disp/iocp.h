/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/disp/iocp.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: iocp.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_DISP_IOCP_H
#define ASYNC_DISP_IOCP_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#if !defined(CERL_IOCP_SUPPORT_BIND)
#error "only support Windows platform!"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// IoService::run()

inline void cerl_call IoService::run()
{
	enum { ASYNC_EVENTSIZE = 128 };

	for (;;)
	{
		processMessageQueueST_();

		if (quitLockRef == QUIT_MASK)
			break;
		
		if (iocpST.empty())
		{
			DWORD bytes;
			ULONG_PTR key = ClientIoNoop;
			LPOVERLAPPED overlapped;
			GetQueuedCompletionStatus(iocpMT.handle, &bytes, &key, &overlapped, INFINITE);
			processMessage_(bytes, key, overlapped);
		}
		else
		{
			for (int i = 0; i < ASYNC_EVENTSIZE; ++i)
			{
				DWORD bytes;
				ULONG_PTR key = ClientIoNoop;
				LPOVERLAPPED overlapped;
				GetQueuedCompletionStatus(iocpMT.handle, &bytes, &key, &overlapped, 0);
				if (!processMessage_(bytes, key, overlapped))
				{
					CERL_ASSERT(key == ClientIoNoop);
					break;
				}
			}
		}
	}
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_DISP_IOCP_H */
