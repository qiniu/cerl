/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/handle/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: windows.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IOCP_WINDOWS_H
#define ASYNC_IOCP_WINDOWS_H

#ifndef ASYNC_MEMORY_H
#include "../Memory.h"
#endif

#define CERL_IOCP_SUPPORT_BIND // support IoCompletionPort::bind

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class IoCompletionPort

#pragma pack(1)

class IoService;
class IoCompletionPort
{
public:
	struct Message
	{
		DWORD bytes;
		ULONG_PTR key;
		LPOVERLAPPED overlapped;
	};

private:
	HANDLE handle;

	friend class IoService;

public:
	explicit IoCompletionPort()
	{
		handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	}

	~IoCompletionPort()
	{
		CERL_VLOG("IoCompletionPort", ("TERM: IoCompletionPort\n"));
		CloseHandle(handle);
	}

	void cerl_call bind(HANDLE fd, ULONG_PTR key) const
	{
		CreateIoCompletionPort(fd, handle, key, 0);
	}

public:
	void cerl_call post(DWORD bytes, ULONG_PTR key, LPOVERLAPPED overlapped)
	{
		PostQueuedCompletionStatus(handle, bytes, key, overlapped);
	}

	BOOL cerl_call get(Message& msg, DWORD timeout = INFINITE)
	{
		return GetQueuedCompletionStatus(handle, &msg.bytes, &msg.key, &msg.overlapped, timeout);
	}

	BOOL cerl_call get(LPDWORD bytes, PULONG_PTR key, LPOVERLAPPED* overlapped, DWORD timeout = INFINITE)
	{
		return GetQueuedCompletionStatus(handle, bytes, key, overlapped, timeout);
	}
};

#pragma pack()

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_IOCP_WINDOWS_H */
