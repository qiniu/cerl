/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/socketio/sockfile/twophase.h
// Creator: mohuafeng
// Email: longshanksmo@gmail.com
// Date: 2010-3-18 17:45:34
// 
// $Id: twophase.h 2754 2010-05-05 10:25:27Z lijie $
// -----------------------------------------------------------------------*/

#ifndef ASYNC_SOCKETIO_EPOLL_TOWPHASES_H
#define ASYNC_SOCKETIO_EPOLL_TOWPHASES_H


NS_CERL_BEGIN

// -------------------------------------------------------------------------

class SocketFileObject
{
private:
	typedef NS_STDEXT::Mutex Mutex;

	detail::SocketOpType const	m_opType;
	int					m_socket;
	epoll_event_t		m_oldevts;
	IoService*			m_service;
	Queue				m_readQue;
	Queue				m_writeQue;
	Timeout				m_readTimeout;
	Timeout				m_writeTimeout;
	BOOL* m_fQuit;
    
	friend class IoService;

public:
	typedef	SocketOverlapped overlapped_type;

public:
	SocketFileObject()
		: m_opType(detail::sot_data),
		m_socket(INVALID_SOCKET),
		m_service(NULL),
		m_readTimeout(CERL_DEFAULT_TIMEOUT),
		m_writeTimeout(CERL_DEFAULT_TIMEOUT),
		m_fQuit(NULL)
	{
	}

	~SocketFileObject()
	{
		close();
        if (m_fQuit)
            *m_fQuit = TRUE;
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
		cerl::detail::setnonblk(m_socket);
		return S_OK;
	}

	SOCKET cerl_call get_handle() const // get native socket handle
	{
		return m_socket;
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
		}
	}

#if defined(CERL_DIAGNOST)
	void cerl_call dbg_close(const char* op)
	{
		close();
	}
#else
#define dbg_close(op) close()
#endif

	void cerl_call set_read_timeout(Timeout to) {
		m_readTimeout = to;
	}

	Timeout cerl_call get_read_timeout() const {
		return m_readTimeout;
	}

	void cerl_call set_write_timeout(Timeout to) {
		m_writeTimeout = to;
	}

	Timeout cerl_call get_write_timeout() const {
		return m_writeTimeout;
	}

public:
	size_t cerl_call read_some(SocketOverlapped& o, void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.m_fiber);

		if (!good())
			return 0;

		CERL_VLOG("Io", ("INFO: SocketFileObject::read_some - begin\n"));
		o.m_buffer = buf;
		o.m_bufsz = cb;
		o.m_result = 0;
		o.m_wakeup = m_service->messageOfSchedule(o.m_fiber);

		{
			m_readQue.push(&o);

			epoll_event_t evts = EPOLLIN | EPOLLERR | EPOLLHUP;
			if (!m_writeQue.empty())
				evts |= EPOLLOUT;

			bind_event(evts);
		}

		CERL_VLOG("Io", ("INFO: SocketFileObject::read_some - yield\n"));

		if (!timed_yield(o.m_fiber, m_readTimeout))
		{
			CERL_WARN("Io", ("WARNING: SocketFileObject::read_some - timeout, socket: %d, time: %d.\n", m_socket, m_readTimeout));
			close();
		}

		return o.m_result;
	}

	size_t cerl_call write_some(SocketOverlapped& o, const void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.m_fiber);

		if (!good())
			return 0;

		CERL_VLOG("Io", ("INFO: SocketFileObject::write_some - begin\n"));
		o.m_buffer = (void*)buf;
		o.m_bufsz = cb;
		o.m_result = 0;
		o.m_wakeup = m_service->messageOfSchedule(o.m_fiber);

		{
			m_writeQue.push(&o);

			epoll_event_t evts = EPOLLOUT | EPOLLERR | EPOLLHUP;
			if (!m_readQue.empty())
				evts |= EPOLLIN;

			bind_event(evts);
		}

		CERL_VLOG("Io", ("INFO: SocketFileObject::write_some - yield\n"));

		if (!timed_yield(o.m_fiber, m_writeTimeout))
		{
			CERL_WARN("Io", ("WARNING: SocketFileObject::read_some - timeout, socket: %d, time: %d.\n", m_socket, m_writeTimeout));
			close();
		}

		return o.m_result;
	}

private:
	void cerl_call internal_wakeup_(Queue& q)
	{
		for (;;)
		{
			if (q.empty())
				break;
			SocketOverlapped* o = (SocketOverlapped*)q.pop();
			o->m_result = 0;
			o->m_wakeup.postST();
		}
	}

	void cerl_call internal_close_()
	{
		if (m_socket != INVALID_SOCKET)
		{
			m_service->iocpMT.unsink(m_socket);
			m_oldevts = 0;
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			internal_wakeup_(m_readQue);
			internal_wakeup_(m_writeQue);
		}
	}

	void cerl_call internal_read_()
	{
		if (m_readQue.empty())
			return;

		SocketOverlapped* o = (SocketOverlapped*)m_readQue.pop();

		CERL_VLOG("Io", ("INFO: SocketFileObject::internal_read - enter read, socket %d, %zu bytes to read.\n", m_socket, o->m_bufsz));

		while(1)
		{
			o->m_result = ::recv(m_socket, o->m_buffer, o->m_bufsz, 0);
			CERL_VLOG("Io", ("INFO: SocketFileObject::internal_read - bufsz: %zu, bytes received: %d\n", o->m_bufsz, o->m_result));
			if (o->m_result >= 0) // error process here should be replaced by error notify system
				break;
			
			if (errno != EINTR)
			{
				CERL_WARN("Io", ("WARN: SocketFileObject::internal_read - read data failed\n"));
				o->m_result = 0;
				break;
			}
		}

		epoll_event_t evts = 0;
		if (!m_readQue.empty())
			evts |= EPOLLIN | EPOLLERR | EPOLLHUP;
		if (!m_writeQue.empty())
			evts |= EPOLLOUT | EPOLLERR | EPOLLHUP;
		bind_event(evts);

		m_service->resumeFiber(o->m_wakeup.sn, o->m_wakeup.fiber);
	}

	void cerl_call internal_write_()
	{
		if (m_writeQue.empty())
			return;

		SocketOverlapped* o = (SocketOverlapped*)m_writeQue.pop();

		CERL_VLOG("Io", ("INFO: SocketFileObject::internal_write - enter write, socket %d, %zu bytes to write.\n", m_socket, o->m_bufsz));

		while(1)
		{
			o->m_result = ::send(m_socket, o->m_buffer, o->m_bufsz, MSG_DONTWAIT | MSG_NOSIGNAL);
			CERL_VLOG("Io", ("INFO: SocketFileObject::internal_write - bufsz: %zu, bytes sended: %d\n", o->m_bufsz, o->m_result));
			if (o->m_result >= 0) // error process here should be replaced by error notify system
				break;

			if (errno != EINTR)
			{
				CERL_WARN("Io", ("WARN: SocketFileObject::internal_write - write data failed\n"));
				o->m_result = 0;
				break;
			}
		}

		epoll_event_t evts = 0;
		if (!m_readQue.empty())
			evts |= EPOLLIN | EPOLLERR | EPOLLHUP;
		if (!m_writeQue.empty())
			evts |= EPOLLOUT | EPOLLERR | EPOLLHUP;
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
			CERL_WARN("Io", ("WARN: SocketFileObject::handle_event - connection hangup or error. socket: %d.\n", m_socket));
			internal_close_();
		}
		else
		{
			BOOL fQuit = FALSE;
			m_fQuit = &fQuit;

			if (epevt.events & EPOLLIN)
				internal_read_();
			if (fQuit)
				return;

			if (epevt.events & EPOLLOUT)
				internal_write_();
			if (fQuit)
				return;

			m_fQuit = NULL;
		}
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_SOCKETIO_WINDOWS_H */
