/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/socketio/sync.h
// Creator: Li Jie
// Email: cpunion@gmail.com
// Date: 2010-3-18 17:45:34
// 
// $Id: sync.h 2787 2010-05-06 09:05:20Z lijie $
// -----------------------------------------------------------------------*/

#ifndef ASYNC_SOCKETIO_SYNC_H
#define ASYNC_SOCKETIO_SYNC_H

#if !defined(_INC_ERRNO) && !defined(_ERRNO_H)
#include <errno.h>
#endif

#include "../Socket.h"

NS_CERL_BEGIN

// -------------------------------------------------------------------------

class SyncSocketFile
{
private:
	int					m_socket;
	Timeout				m_readTimeout;
	Timeout				m_writeTimeout;
    
public:
	SyncSocketFile()
		: m_socket(INVALID_SOCKET),
		m_readTimeout(CERL_DEFAULT_TIMEOUT),
		m_writeTimeout(CERL_DEFAULT_TIMEOUT)
	{
	}

	~SyncSocketFile()
	{
		close();
	}

public:
	HRESULT cerl_call open_handle(SOCKET s)
	{
		CERL_ASSERT(m_socket == INVALID_SOCKET);

		if (s == INVALID_SOCKET)
			return E_ACCESSDENIED;

		m_socket = s;
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
			::closesocket(m_socket);
			m_socket = INVALID_SOCKET;
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

#if defined(X_OS_WINDOWS)
		::setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&to, sizeof(to));
#else
		timeval timeout = {to / 1000, (to % 1000) * 1000};
		::setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif
	}

	Timeout cerl_call get_read_timeout() const {
		return m_readTimeout;
	}

	void cerl_call set_write_timeout(Timeout to) {
		m_writeTimeout = to;
		
#if defined(X_OS_WINDOWS)
		::setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&to, sizeof(to));
#else
		timeval timeout = {to / 1000, (to % 1000) * 1000};
		::setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
#endif
	}

	Timeout cerl_call get_write_timeout() const {
		return m_writeTimeout;
	}

public:
	size_t cerl_call read_some(void* buf, size_t cb)
	{
		if (!good())
			return 0;

RETRY:
		int bytes = ::recv(m_socket, (char*)buf, cb, 0);

		if (bytes > 0)
			return bytes;

		if (bytes == -1 && errno == EINTR)
			goto RETRY;

		return 0;
	}

	size_t cerl_call read(void* buf, size_t cb)
	{
		return read_some(buf, cb);
	}

	size_t cerl_call write_some(const void* buf, size_t cb)
	{
		if (!good())
			return 0;

RETRY:
#if defined(X_OS_WINDOWS)
		int bytes = ::send(m_socket, (const char*)buf, cb, 0);
#else
		int bytes = ::send(m_socket, (const char*)buf, cb, MSG_NOSIGNAL);
#endif
		if (bytes > 0)
			return bytes;

		if (bytes == -1 && errno == EINTR)
			goto RETRY;

		return 0;
	}

	size_t cerl_call write(const void* buf, size_t cb)
	{
		return write_some(buf, cb);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_SOCKETIO_SYNC_H */
