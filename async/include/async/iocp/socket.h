/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/iocp/socket.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: socket.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IOCP_SOCKET_H
#define ASYNC_IOCP_SOCKET_H

#ifndef ASYNC_BASIC_H
#include "../Basic.h"
#endif

#ifndef ASYNC_SOCKET_H
#include "../Socket.h"
#endif

#ifndef _SYS_EPOLL_H
#include <sys/epoll.h>
#endif

#ifndef _ERRNO_H
#include <errno.h>
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// typedef of epoll_event::events

typedef	__uint32_t	epoll_event_t;

// -------------------------------------------------------------------------
// max number of epoll events

enum { ASYNC_EPOLLSIZE = 65535 };

// -------------------------------------------------------------------------
// socket operation types

namespace detail
{
	enum SocketOpType
	{
		sot_connect = 1,
		sot_accept = 2,
		sot_data = 3,
		sot_iocp = 4
	};
} //namespace detail

// -------------------------------------------------------------------------
// class IoCompletionPort

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
	detail::SocketOpType const	m_opType;
	SOCKET m_fdin, m_fdout;
	SOCKET m_epollfd;

	friend class IoService;

public:
	IoCompletionPort()
		: m_opType(detail::sot_iocp)
	{
		m_epollfd = epoll_create(ASYNC_EPOLLSIZE+1);
		if (m_epollfd == INVALID_SOCKET)
		{
			CERL_WARN("epoll", ("WARNING: epoll create failed: %d\n", errno));
			return;
		}
		CERL_VLOG("epoll", ("INFO: epoll created: %d, iocp: %p\n", m_epollfd, this));

		SOCKET uxsock[2];
		if (socketpair(AF_UNIX, SOCK_DGRAM, 0, uxsock) < 0)
		{
			CERL_WARN("iocp", ("WARN: IoCompletionPort::IoCompletionPort - create unix domain socket failed.\n"));
			m_fdin = INVALID_SOCKET;
			return;
		}

		m_fdin = uxsock[0];
		m_fdout = uxsock[1];

		sink(m_fdout, EPOLLIN | EPOLLERR | EPOLLHUP, this);
	}

public:
	void cerl_call post(DWORD bytes, ULONG_PTR key, LPOVERLAPPED overlapped)
	{
		Message const msg = { bytes, key, overlapped };
		if (write(m_fdin, &msg, sizeof(msg)) != sizeof(msg))
		{
			CERL_WARN("iocp", ("WARN: IoCompletionPort::post failed!\n"));
		}
	}

public:
	void sink(SOCKET socket, epoll_event_t evts, void* data)
	{
		epoll_act(socket, EPOLL_CTL_ADD, evts, data);
	}

	void unsink(SOCKET socket)
	{
		epoll_ctl(m_epollfd, EPOLL_CTL_DEL, socket, NULL);
	}

	void modify(SOCKET socket, epoll_event_t evts, void* data)
	{
		epoll_act(socket, EPOLL_CTL_MOD, evts, data);
	}

private:
	void epoll_act(SOCKET socket, int action, epoll_event_t evts, void* data)
	{
		epoll_event	epevt;
		epevt.events = evts;
		epevt.data.ptr = data;

		if (epoll_ctl(m_epollfd, action, socket, &epevt) == -1)
		{
			CERL_WARN("epoll", ("WARNING:    epoll_ctl failed: %d\n", errno));
			perror("epoll");
			return;
		}
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_IOCP_SOCKET_H */
