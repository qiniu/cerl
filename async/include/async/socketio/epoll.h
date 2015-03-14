#ifndef ASYNC_SOCKETIO_EPOLL_H
#define ASYNC_SOCKETIO_EPOLL_H

#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>

#include "../IoService.h"
#include "../List.h"
#include "../Socket.h"
#include "../Timer.h"

NS_CERL_BEGIN

class IoService;

// -------------------------------------------------------------------------

namespace detail
{
	class ConnectSocket;

	inline int cerl_call setnonblk(int fd)
	{
		unsigned long on = 1;
		return ::ioctl(fd, FIONBIO, &on);
	}

	inline epoll_event_t cerl_call set_epoll_event(
		IoCompletionPort* iocp, int socket, epoll_event_t evts, epoll_event_t oldevts, void* data)
	{
		CERL_VLOG("socket file object", ("INFO:   bind event: %d, old: %d, socket: %d, iocp: %p\n", evts, oldevts, socket, iocp));
		if (evts == oldevts)
			return	evts;

		if (evts == 0)
			iocp->unsink(socket);
		else if (oldevts == 0)
			iocp->sink(socket, evts, data);
		else if (oldevts != 0)
			iocp->modify(socket, evts, data);

		return evts;
	}

}	//namespace detail

// -------------------------------------------------------------------------

class SocketOverlapped : public ListNode
{
private:
	SocketOverlapped(const SocketOverlapped&);
	void operator=(const SocketOverlapped&);

	friend class IoService;
	friend class SocketFileObject;
	friend class ListenSocket;
	friend class detail::ConnectSocket;
	friend SOCKET cerl_call connectSocket(Fiber self, const SockaddrIn& si);

public:
	explicit SocketOverlapped(Fiber fiber) 
		: m_fiber(fiber)
		, m_buffer(NULL)
		, m_bufsz(0)
		, m_result(0)
	{
		m_fiber = fiber;
	}

	void cerl_call setFiber(Fiber fiber)
	{
		CERL_VLOG("SocketOverlapped", ("INFO: SocketOverlapped::setFiber - set fiber: %p.\n", fiber));
		m_fiber = fiber;
	}

	Fiber cerl_call getFiber() const
	{
		return m_fiber;
	}

private:
	Fiber				m_fiber;
	void*				m_buffer;
	size_t				m_bufsz;
	int					m_result;
	IoService::ScheduleMessage m_wakeup;
};

// -------------------------------------------------------------------------

NS_CERL_END


#if defined(ASYNC_SOCKET_TWOPHASES)
#ifndef ASYNC_SOCKETIO_EPOLL_TOWPHASES_H
#include "sockfile/twophase.h"
#endif
#else
#ifndef ASYNC_SOCKETIO_EPOLL_BUFFERED_H
#include "sockfile/buffered.h"
#endif
#endif

NS_CERL_BEGIN


// -------------------------------------------------------------------------
// class ListenSocket

class ListenSocket
{
private:
	typedef NS_STDEXT::Mutex Mutex;

	detail::SocketOpType const	m_opType;
	int					m_socket;
	epoll_event_t		m_oldevts;
	IoService*			m_service;
	Queue				m_acptQue;

	friend class IoService;

public:
	ListenSocket()
		: m_opType(detail::sot_accept),
		m_socket(INVALID_SOCKET),
		m_service(NULL)
	{
	}
	~ListenSocket()
	{
		close();
	}

public:
	HRESULT cerl_call open_handle(SOCKET s)
	{
		return open_handle(getCurrentFiber(), s);
	}

	HRESULT cerl_call open_handle(Fiber self, SOCKET s)
	{
		CERL_ASSERT(m_socket == INVALID_SOCKET);

		if (s == INVALID_SOCKET)
			return E_ACCESSDENIED;

		m_service = CerlIoService(self);
		m_socket = s;
		m_oldevts = 0;
		return S_OK;
	}

	bool cerl_call good() const
	{
		return m_socket != INVALID_SOCKET;
	}

	void cerl_call close()
	{
		if (m_socket != INVALID_SOCKET)
		{
			internal_close_();
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}
	}

public:
	SOCKET cerl_call accept()
	{
		return accept(getCurrentFiber());
	}

	SOCKET cerl_call accept(Fiber self)
	{
		IoService* const service = CerlIoService(self);

		SocketOverlapped ovlp(self);
		sockaddr_in	addr;

		ovlp.m_wakeup = service->messageOfSchedule(self);
		ovlp.m_buffer = &addr;
		ovlp.m_bufsz = sizeof(addr);

		{
			m_acptQue.push(&ovlp);
			bind_event(EPOLLIN | EPOLLERR | EPOLLHUP);
		}

		service->yield(ovlp.m_fiber);

		return ovlp.m_result;
	}

private:
	void cerl_call internal_close_()
	{
		if (m_socket != INVALID_SOCKET)
		{
			m_service->iocpMT.unsink(m_socket);
			m_oldevts = 0;
			for (;;)
			{
				SocketOverlapped* o = (SocketOverlapped*)m_acptQue.pop();
				if (o == NULL)
					break;
				o->m_result = INVALID_SOCKET;
				o->m_wakeup.postST();
			}
		}
	}

	void cerl_call internal_accept_()
	{
		CERL_VLOG("Io", ("INFO: ListenSocket::internal_accept\n"));
		CERL_ASSERT(!m_acptQue.empty());

		SocketOverlapped* o = (SocketOverlapped*)m_acptQue.pop();

		while(1)
		{
			o->m_result = ::accept(m_socket, (sockaddr*)o->m_buffer, (socklen_t*)&o->m_bufsz);
			if (o->m_result != INVALID_SOCKET) // error process here should be replaced by error notify system
				break;

			if (errno != EINTR)
			{
				CERL_WARN("Io", ("WARN: ListenSocket::internal_accept - accept failed!\n"));
				break;
			}
		}

		const epoll_event_t evts = m_acptQue.empty() ? 0 : EPOLLIN | EPOLLERR | EPOLLHUP;
		bind_event(evts);

		m_service->resumeFiber(o->m_wakeup.sn, o->m_wakeup.fiber);
	}

	void cerl_call bind_event(epoll_event_t evts)
	{
		m_oldevts = detail::set_epoll_event(&m_service->iocpMT, m_socket, evts, m_oldevts, this);
	}

	void cerl_call handle_event(const epoll_event& epevt)
	{
		if (epevt.events & (EPOLLERR | EPOLLHUP))
		{
			internal_close_();
		}
		else
		{
			CERL_ASSERT(epevt.events & EPOLLIN);
			internal_accept_();
		}
	}
};

// -------------------------------------------------------------------------
// function connectSocket

namespace detail {

	class ConnectSocket
	{
	public:
		detail::SocketOpType const m_opType;
		SOCKET m_socket;
		IoService* const m_service;
		SocketOverlapped ovlp;
		bool m_isInProc;

		friend class IoService;
		friend SOCKET cerl_call connectSocket(Fiber self, const SockaddrIn& si);

	public:
		explicit ConnectSocket(Fiber self, SOCKET sdConnect, const SockaddrIn& si)
			: m_opType(detail::sot_connect),
			m_socket(INVALID_SOCKET),
			m_service(CerlIoService(self)),
			ovlp(self),
			m_isInProc(false)
		{
			ovlp.m_wakeup = m_service->messageOfSchedule(self);
			//ovlp.m_buffer = (void*)&si;
			//ovlp.m_bufsz = sizeof(si);

			detail::setnonblk(sdConnect);

			if (::connect(sdConnect, (sockaddr*)&si, sizeof(si)) == -1)
			{
				if (errno == EINPROGRESS)
				{
					CERL_VLOG("Connect", ("INFO: detail::ConnectSocket::ConnectSocket - connection in process.\n"));
					m_service->iocpMT.sink(sdConnect, EPOLLOUT | EPOLLERR | EPOLLHUP, this);
					m_socket = sdConnect;
					m_isInProc = true;
				}
				else
				{
					CERL_WARN("Connect", ("WARNING: detail::ConnectSocket::ConnectSocket - connect error: %d,", errno));
					perror("");
					close(m_socket);
				}
			}
			else
			{
				CERL_VLOG("Connect", ("INFO: detail::ConnectSocket::ConnectSocket - connection ok.\n"));
				m_socket = sdConnect;
			}

		}

		void cerl_call internal_close_()
		{
			ovlp.m_result = INVALID_SOCKET;
			m_service->iocpMT.unsink(m_socket);
			ovlp.m_wakeup.postST();
		}

		void cerl_call internal_connect_()
		{
			CERL_VLOG("Io", ("INFO: ConnectSocket::internal_connect\n"));

			ovlp.m_result = m_socket;

			m_service->resumeFiber(ovlp.m_wakeup.sn, ovlp.m_wakeup.fiber);
		}

		void cerl_call handle_event(const epoll_event& epevt)
		{
			if (epevt.events & (EPOLLERR | EPOLLHUP))
			{
				internal_close_();

				::closesocket(m_socket);
				m_socket = INVALID_SOCKET;
			}
			else
			{
				CERL_ASSERT(epevt.events & EPOLLOUT);
				m_service->iocpMT.unsink(m_socket);
				internal_connect_();
			}
		}
	};

} // namespace detail

inline SOCKET cerl_call connectSocket(Fiber self, const SockaddrIn& si, int timeout = 10000)
{
	CERL_FIBER_ASSERT_SELF(self);

	SOCKET sdConnect = (SOCKET)createSocket();
	if (sdConnect == INVALID_SOCKET)
	{
		CERL_WARN("Socket", ("WARN: createSocket failed: %d\n", errno));
		return INVALID_SOCKET;
	}

	detail::ConnectSocket conn(self, sdConnect, si);
	if (!conn.m_isInProc)
	{
		return conn.m_socket;
	}
	else
	{
		//CerlIoService(self)->yield(self);
		if (!timed_yield(self, timeout))
		{
			CERL_WARN("Socket", ("WARN: connectSocket timeout.\n"));
			::closesocket(conn.m_socket);
			return INVALID_SOCKET;
		}

		return conn.m_socket;
	}

}

inline SOCKET cerl_call connectSocket(Fiber self, const char* hostname, Port port)
{
	const SockaddrIn si(hostname, port);
	return connectSocket(self, si);
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_SOCKETIO_WINDOWS_H */
