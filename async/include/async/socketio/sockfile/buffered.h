/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/socketio/buf_epoll.h
// Creator: mohuafeng
// Email: longshanksmo@gmail.com
// Date: 2010-3-18 17:45:34
// 
// $Id: buffered.h 2754 2010-05-05 10:25:27Z lijie $
// -----------------------------------------------------------------------*/

#ifndef ASYNC_SOCKETIO_EPOLL_BUFFERED_H
#define ASYNC_SOCKETIO_EPOLL_BUFFERED_H


// -------------------------------------------------------------------------
NS_CERL_BEGIN

namespace detail
{
	// class SocketReadBuffer
	class SocketReadBuffer
	{
	public:
		typedef char buf_type;
		typedef size_t size_type;

	private:
		buf_type*	m_buf;
		buf_type*	m_end;
		buf_type*	m_cur;
		size_type	m_size;

	public:
		SocketReadBuffer()
			: m_buf(NULL), m_end(NULL), m_cur(NULL), m_size(0)
		{
		}

	public:
		bool cerl_call good() const
		{
			return m_buf != NULL && m_size != 0 && m_end >= m_cur && m_end <= (m_buf + m_size) 
				&& m_cur >= m_buf && m_cur <= m_end;
		}

		bool cerl_call is_empty() const
		{
			return m_cur >= m_end;
		}

		bool cerl_call is_full() const {
			return get_free_size() == 0;
		}

	public:
		buf_type* cerl_call get_buffer() {
			return m_buf;
		}

		size_type cerl_call get_buf_size() const {
			return m_size;
		}

		void cerl_call set_buffer(buf_type* buf, size_type size)
		{
			m_cur += buf - m_buf;
			m_end += buf - m_buf;
			m_buf = buf;
			m_size = size;
		}

		size_type cerl_call get_size() const {
			return m_end - m_cur;
		}

		size_type cerl_call get_free_size() const {
			return m_size - (m_end - m_buf);
		}

	public:
		size_type cerl_call get(void* data, size_type cb)
		{
			size_type nbuf = get_size();
			size_type tocopy = cb > nbuf ? nbuf : cb;

			if (tocopy == 0)
				return 0;

			memcpy(data, m_cur, tocopy);

			if (tocopy == nbuf)
			{
				m_end = m_cur = m_buf;
			}
			else
			{
				m_cur += tocopy;
			}

			return tocopy;
		}

		int cerl_call load(int socket)
		{
			size_type got = 0;

			if (!good())
			{
				CERL_WARN("Io", ("WARNING: SocketReadBuffer::load - not good, socket: %d.\n", socket));
				return -1;
			}

			if (is_full())
				return get_size();

			while(1)
			{
				got = ::recv(socket, m_end, get_free_size(), 0);
				CERL_VLOG("Io", ("INFO: SocketReadBuffer::load - socket: %d, bytes to recieve: %d, bytes received: %d\n", socket, get_free_size(), got));
				if (got > 0)
					break;
				
				if (got == 0)
					return -1;
	
				if (errno != EINTR)
				{
					CERL_WARN("Io", ("WARN: SocketReadBuffer::load - read data failed, socket: %d, errno: %d.\n", socket, errno));
					got = 0;
					break;
				}
			}

			m_end += got;
			return (int)got;
		}
	};
} // namespace detail

// -------------------------------------------------------------------------

class SocketFileObject
{
private:
	typedef NS_STDEXT::Mutex Mutex;
	typedef detail::SocketReadBuffer ReadBuffer;
	typedef detail::SocketReadBuffer::buf_type buf_type;
	typedef detail::SocketReadBuffer::size_type size_type;

	detail::SocketOpType const	m_opType;
	int					m_socket;
	epoll_event_t		m_oldevts;
	IoService*			m_service;
	Queue				m_readQue;
	Queue				m_writeQue;
	Timeout				m_readTimeout;
	Timeout				m_writeTimeout;
	BOOL* 				m_fQuit;
	ReadBuffer			m_readBuf;

	enum { DefSockBufSize = 16000 };
    
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
		cerl::detail::setnonblk(m_socket);

		set_read_buffer_size(DefSockBufSize);

		m_oldevts = 0;
		bind_event(EPOLLIN | EPOLLERR | EPOLLHUP);

		return S_OK;
	}

	SOCKET cerl_call get_handle() const // get native socket handle
	{
		return m_socket;
	}

	bool cerl_call good() const
	{
		return m_socket != INVALID_SOCKET && m_readBuf.good();
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

	void cerl_call set_read_buffer_size(size_type size) 
	{
		if (m_service == NULL)
			return;

		if (size == 0)
		{
			if (m_readBuf.get_buf_size() > 0)
			{
				Pool& pool = m_service->pools.get_pool(m_readBuf.get_buf_size());
				pool.deallocate(m_readBuf.get_buffer());
			}
			m_readBuf.set_buffer(0, 0);
			return;
		}

		Pool& pool = m_service->pools.get_pool(size);
		size_type oldSize = m_readBuf.get_buf_size();
		size_type newSize = pool.alloc_size();
		if (newSize <= oldSize)
			return;

		buf_type* oldBuf = m_readBuf.get_buffer();
		buf_type* newBuf = NULL;
		
        newBuf = (buf_type*)pool.allocate();
        memcpy(newBuf, oldBuf, oldSize);

		if (oldSize != 0)
			m_service->pools.get_pool(oldSize).deallocate(oldBuf);

		m_readBuf.set_buffer(newBuf, newSize);
	}

	void cerl_call set_write_buffer_size(size_type size) {
	}

public:
	size_t cerl_call read_some(SocketOverlapped& o, void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.m_fiber);
		CERL_VLOG("Io", ("INFO: SocketFileObject::read_some - begin\n"));

		if (!good())
		{
			CERL_WARN("Io", ("WARNING: SocketFileObject::read_some - not good. socket: %d\n", m_socket));
			return 0;
		}

		// read data from buffer, if empty, yield. and the fiber will be waked up by internal_read()
		if (m_readBuf.is_empty())
		{
			o.m_wakeup = m_service->messageOfSchedule(o.m_fiber);
			m_readQue.push(&o);

			CERL_VLOG("Io", ("INFO: SocketFileObject::read_some - yield\n"));
			if (!timed_yield(o.m_fiber, m_readTimeout))
			{
				CERL_WARN("Io", ("WARNING: SocketFileObject::read_some - timeout, socket: %d, time: %d, sn: %d.\n", m_socket, m_readTimeout, CerlFiberSerialNo(o.m_fiber)));
				close();
				return 0;
			}
		}

		int got = m_readBuf.get(buf, cb);
		CERL_VLOG("Io", ("INFO: SocketFileObject::read_some - read %d bytes.\n", got));

		if (!m_readBuf.is_full())
		{
			epoll_event_t evts = EPOLLIN;
			if (!m_writeQue.empty())
				evts |= EPOLLOUT | EPOLLERR | EPOLLHUP;
			bind_event(evts);
		}

		return got;
	}

	size_t cerl_call write_some(SocketOverlapped& o, const void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.m_fiber);

		if (!good())
		{
			CERL_WARN("Io", ("WARNING: SocketFileObject::write_some - not good. socket: %d\n", m_socket));
			return 0;
		}

		while(1)
		{
			int nsent = send(m_socket, buf, cb, MSG_DONTWAIT | MSG_NOSIGNAL);
			if (nsent >= 0)
				return nsent;

			if (errno == EAGAIN)
				break;

			if (errno != EINTR)
			{
				CERL_WARN("Io", ("WARN: SocketFileObject::internal_write_some - send data failed. socket: %d, errno: %d\n", m_socket, errno));
				return 0;
			}
		}

		CERL_VLOG("Io", ("INFO: SocketFileObject::write_some - begin\n"));
		o.m_buffer = (void*)buf;
		o.m_bufsz = cb;
		o.m_result = 0;
		o.m_wakeup = m_service->messageOfSchedule(o.m_fiber);

		{
			m_writeQue.push(&o);

			epoll_event_t evts = EPOLLOUT | EPOLLERR | EPOLLHUP;
			if (!m_readBuf.is_full())
				evts |= EPOLLIN | EPOLLERR | EPOLLHUP;
			bind_event(evts);
		}

		CERL_VLOG("Io", ("INFO: SocketFileObject::write_some - yield\n"));

		if (!timed_yield(o.m_fiber, m_writeTimeout))
		{
			CERL_WARN("Io", ("WARNING: SocketFileObject::write_some - timeout, socket: %d, time: %d.\n", m_socket, m_writeTimeout));
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
			set_read_buffer_size(0);
		}
	}

	void cerl_call internal_read_()
	{
		// read data from socket and fill the read buffer
		int loaded = m_readBuf.load(m_socket); 
		if (loaded == 0)
			return;

		if (m_readBuf.is_full())
		{
			epoll_event_t evts = 0;
			if (!m_writeQue.empty())
				evts |= EPOLLOUT | EPOLLERR | EPOLLHUP;
			bind_event(evts);
		}

		if (loaded == -1)
		{
			CERL_VLOG("Io", ("WARNING: SocketFileObject::internal_read - the socket is shutdown, socket: %d.\n", m_socket));
			internal_close_();
			return;
		}

		SocketOverlapped* o = (SocketOverlapped*)m_readQue.pop();
		if (o == NULL)
			return;

		m_service->resumeFiber(o->m_wakeup.sn, o->m_wakeup.fiber);
	}

	void cerl_call internal_write_()
	{
		if (m_writeQue.empty())
			return;

		SocketOverlapped* o = (SocketOverlapped*)m_writeQue.pop();

		CERL_VLOG("Io", ("INFO: SocketFileObject::internal_write - enter write, socket %d, %d bytes to write.\n", m_socket, o->m_bufsz));

		while(1)
		{
			o->m_result = ::send(m_socket, o->m_buffer, o->m_bufsz, MSG_DONTWAIT | MSG_NOSIGNAL);
			CERL_VLOG("Io", ("INFO: SocketFileObject::internal_write - bufsz: %d, bytes sended: %d\n", o->m_bufsz, o->m_result));
			if (o->m_result > 0) // error process here should be replaced by error notify system
				break;

			if (o->m_result == 0)
			{
				CERL_WARN("Io", ("WARNING: SocketFileObject::internal_write - the socket is shutdown, socket: %d.\n", m_socket));
				internal_close_();
				return;
			}

			if (errno != EINTR)
			{
				CERL_WARN("Io", ("WARN: SocketFileObject::internal_write - write data failed\n"));
				o->m_result = 0;
				break;
			}
		}

		epoll_event_t evts = 0;
		if (!m_readBuf.is_full())
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
