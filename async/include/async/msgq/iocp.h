/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/msgq/iocp.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: iocp.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_MSGQ_IOCP_H
#define ASYNC_MSGQ_IOCP_H

#ifndef ASYNC_LIST_H
#include "../List.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class WinMessageQueue

class WinMessageQueue
{
public:
	typedef ListNode Node;

protected:
	HANDLE iocp;

public:
	WinMessageQueue()
	{
		iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	}
	~WinMessageQueue()
	{
		CloseHandle(iocp);
	}

	void cerl_call push(Node* val)
	{
		PostQueuedCompletionStatus(iocp, 0, 0, (LPOVERLAPPED)val);
	}

	Node* cerl_call pop()
	{
		DWORD bytes;
		ULONG_PTR key;
		LPOVERLAPPED overlapped = NULL;
		GetQueuedCompletionStatus(iocp, &bytes, &key, &overlapped, INFINITE);
		return (Node*)overlapped;
	}
};

typedef WinMessageQueue MessageQueue;

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_MSGQ_IOCP_H */
