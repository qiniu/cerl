/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/IoCompletionPort.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: IoCompletionPort.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IOCOMPLETIONPORT_H
#define ASYNC_IOCOMPLETIONPORT_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_OS_LINUX)
#include "iocp/socket.h"
#elif defined(X_OS_WINDOWS)
#include "iocp/windows.h"
#else
#error "error: Unsupport OS platform!!!"
#endif

// -------------------------------------------------------------------------
// class IoCompletionPortST

#if !defined(_DEQUE_) && !defined(_DEQUE)
#include <deque>
#endif

NS_CERL_BEGIN

class IoCompletionPortST : private std::deque<IoCompletionPort::Message>
{
public:
	typedef IoCompletionPort::Message Message;

private:
	typedef std::deque<IoCompletionPort::Message> Base;

public:
	using Base::size;
	using Base::empty;
	using Base::front;
	using Base::pop_front;

public:
	void cerl_call post(DWORD bytes, ULONG_PTR key, LPOVERLAPPED overlapped)
	{
		Message const msg = { bytes, key, overlapped };
		Base::push_back(msg);
	}
};

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_IOCOMPLETIONPORT_H */
