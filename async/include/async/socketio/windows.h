/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/socketio/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: windows.h 2754 2010-05-05 10:25:27Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_SOCKETIO_WINDOWS_H
#define ASYNC_SOCKETIO_WINDOWS_H

#ifndef ASYNC_FILEIO_WINDOWS_H
#include "../fileio/windows.h"
#endif

#ifndef ASYNC_SOCKET_H
#include "../Socket.h"
#endif

#ifndef ASYNC_TIMER_H
#include "../Timer.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class SocketFileObject

class SocketFileObject
{
protected:
	SOCKET s;
	Timeout	m_readTimeout;
	Timeout	m_writeTimeout;

public:
	typedef SOCKET handle_type;
	typedef SocketOverlapped overlapped_type;

public:
	SocketFileObject()
		: s(INVALID_SOCKET),
		  m_readTimeout(CERL_DEFAULT_TIMEOUT),
		  m_writeTimeout(CERL_DEFAULT_TIMEOUT)
	{
	}
	~SocketFileObject()
	{
		if (s != INVALID_SOCKET)
			dbg_closesocket("SOCKET", s);
	}

public:
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

	bool cerl_call good() const
	{
		return s != INVALID_SOCKET;
	}

	SOCKET cerl_call detach()
	{
		SOCKET sdOld = s;
		s = INVALID_SOCKET;
		return sdOld;
	}

	HRESULT cerl_call open_handle(SOCKET s)
	{
		return open_handle(getCurrentFiber(), s);
	}

	HRESULT cerl_call open_handle(Fiber self, SOCKET s)
	{
		CERL_ASSERT(!good());
		if (s == INVALID_SOCKET)
			return E_ACCESSDENIED;

		CerlIoService(self)->iocpMT.bind((HANDLE)s, (ULONG_PTR)IoService::ClientIoReadWrite);
		this->s = s;
		return S_OK;
	}

	SOCKET cerl_call get_handle() const // get native socket handle
	{
		return s;
	}

	void cerl_call close()
	{
		if (s != INVALID_SOCKET)
		{
			dbg_closesocket("SOCKET", s);
			s = INVALID_SOCKET;
		}
	}

#if defined(CERL_DIAGNOST)
	void cerl_call dbg_close(const char* op)
	{
		if (s != INVALID_SOCKET)
		{
			dbg_closesocket(op, s);
			s = INVALID_SOCKET;
		}
	}
#else
#define dbg_close(op) close()
#endif

	size_t cerl_call read_some(SocketOverlapped& o, void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.fiber);

		using namespace detail;

		size_t result = 0;
		IoService* const service = CerlIoService(o.fiber);
		OVERLAPPED_IO* const ovlp = (OVERLAPPED_IO*)service->overlappedPool.allocate();
		ovlp->init(o, result);

		DWORD dwFlags = 0;
		WSABUF wsaBuf = { cb, (char*)buf };
		if (WSARecv(s, &wsaBuf, 1, NULL, &dwFlags, &ovlp->Overlapped, NULL) == SOCKET_ERROR)
		{
			const DWORD nRet = WSAGetLastError();
			if (nRet != ERROR_IO_PENDING)
			{
				CERL_WARN("Socket", ("WARN: SocketFileObject::read_some - WSARecv failed: %d\n", nRet));
				close();
				return 0;
			}
		}
		if (!timed_yield(o.fiber, m_readTimeout))
		{
			CERL_WARN("Socket", ("WARN: SocketFileObject::read_some - timeout, time: %d, fiber: %p\n", m_readTimeout, o.fiber));
			close();
		}

		CERL_ASSERT(result <= cb);
		return result;
	}

	size_t cerl_call write_some(SocketOverlapped& o, const void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.fiber);

		using namespace detail;

		size_t result = 0;
		IoService* const service = CerlIoService(o.fiber);
		OVERLAPPED_IO* const ovlp = (OVERLAPPED_IO*)service->overlappedPool.allocate();
		ovlp->init(o, result);

		WSABUF wsaBuf = { cb, (char*)buf };
		if (WSASend(s, &wsaBuf, 1, NULL, 0, &ovlp->Overlapped, NULL) == SOCKET_ERROR)
		{
			const DWORD nRet = WSAGetLastError();
			if (nRet != ERROR_IO_PENDING)
			{
				CERL_WARN("Socket", ("WARN: SocketFileObject::write_some - WSASend failed: %d\n", nRet));
				close();
				return 0;
			}
		}
		if (!timed_yield(o.fiber, m_writeTimeout))
		{
			CERL_WARN("Socket", ("WARN: SocketFileObject::write_some - timeout\n"));
			close();
		}

		CERL_ASSERT(result <= cb);
		return result;
	}
};

// -------------------------------------------------------------------------
// class ListenSocket

class ListenSocket
{
private:
	enum { SOCKADDR_BUFSIZE = sizeof(SOCKADDR_IN) + 16 };

	SOCKET s;
	LPFN_ACCEPTEX lpfnAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockaddrs;

public:
	ListenSocket()
		: s(INVALID_SOCKET)
	{
	}
	~ListenSocket()
	{
		close();
	}

	bool cerl_call good() const
	{
		return s != INVALID_SOCKET;
	}

	HRESULT cerl_call open_handle(SOCKET s)
	{
		return open_handle(getCurrentFiber(), s);
	}

	HRESULT cerl_call open_handle(Fiber self, SOCKET s)
	{
		CERL_ASSERT(!good());
		if (s == INVALID_SOCKET)
			return E_ACCESSDENIED;

		CerlIoService(self)->iocpMT.bind((HANDLE)s, (ULONG_PTR)IoService::ClientIoAccept);
		this->s = s;

		// Get AccpetEx Function
		{
			DWORD dwBytes = 0;
			GUID guidAcceptEx = WSAID_ACCEPTEX;
			const int nRet = WSAIoctl(
				s, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidAcceptEx, sizeof(guidAcceptEx),
				&lpfnAcceptEx,sizeof(lpfnAcceptEx), &dwBytes, NULL, NULL);
			CERL_ASSERT(nRet != SOCKET_ERROR);
		}

#if defined(_DEBUG)
		// Get GetAcceptExSockaddrs Function
		{
			DWORD dwBytes = 0;
			GUID guidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
			const int nRet = WSAIoctl(
				s, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidGetAcceptExSockaddrs, sizeof(guidGetAcceptExSockaddrs),
				&lpfnGetAcceptExSockaddrs, sizeof(lpfnGetAcceptExSockaddrs), &dwBytes, NULL, NULL);
			CERL_ASSERT(nRet != SOCKET_ERROR);
		}
#endif
		return S_OK;
	}

	void cerl_call close()
	{
		if (s != INVALID_SOCKET)
		{
			dbg_closesocket("LISTEN", s);
			s = INVALID_SOCKET;
		}
	}

	SOCKET cerl_call accept()
	{
		return accept(getCurrentFiber());
	}

	SOCKET cerl_call accept(Fiber self)
	{
		SOCKET sdAccept = createSocket();
		if (sdAccept == INVALID_SOCKET)
		{
			CERL_WARN("Socket", ("WARN: createSocket failed - %d\n", WSAGetLastError()));
			return INVALID_SOCKET;
		}

		//
		// pay close attention to these parameters and buffer lengths
		//
		char buffer[SOCKADDR_BUFSIZE*2];
		detail::OVERLAPPED_ACCEPT o;
		ZeroMemory(&o, sizeof(o));
		o.fiber = self;
		o.initSerialNo();

		DWORD dwRecvNumBytes = 0;
		if (
			lpfnAcceptEx(
				s, sdAccept, buffer, 0,	SOCKADDR_BUFSIZE, SOCKADDR_BUFSIZE,
				&dwRecvNumBytes, &o.Overlapped) == SOCKET_ERROR
			)
		{
			const DWORD nRet = WSAGetLastError();
			if (nRet != ERROR_IO_PENDING)
			{
				CERL_WARN("Socket", ("WARN: AcceptEx failed - %d\n", nRet));
				dbg_closesocket("ACCEPT", sdAccept);
				return INVALID_SOCKET;
			}
		}

		CERL_VLOG("Socket", ("INFO: ListenSocket(%d)::accept\n", s));
		yield(o.fiber);
		
		// When the AcceptEx function returns, the socket sAcceptSocket is  
		// in the default state for a connected socket. The socket sAcceptSocket  
		// does not inherit the properties of the socket associated with  
		// sListenSocket parameter until SO_UPDATE_ACCEPT_CONTEXT is set on  
		// the socket. Use the setsockopt function to set the SO_UPDATE_ACCEPT_CONTEXT  
		// option, specifying sAcceptSocket as the socket handle and sListenSocket  
		// as the option value.  
		if (setsockopt(sdAccept, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&s, sizeof(s)) == SOCKET_ERROR)
		{
			CERL_VLOG("Socket", ("INFO: ListenSocket(%d)::accept canceled\n", s));
			closesocket(sdAccept);
			return INVALID_SOCKET;
		}

		CERL_VLOG("Socket", ("INFO: ListenSocket(%d)::accepted, socket: %d\n", s, sdAccept));

#if defined(_DEBUG)
		DWORD dwBytes = 0;
		SOCKADDR* lpLocalSockaddr = NULL;
		SOCKADDR* lpRemoteSockaddr = NULL;
		int	nLocalSockaddrLen = 0;
		int	nRemoteSockaddrLen = 0;
		lpfnGetAcceptExSockaddrs(
			buffer, 0, SOCKADDR_BUFSIZE, SOCKADDR_BUFSIZE,
			&lpLocalSockaddr, &nLocalSockaddrLen, &lpRemoteSockaddr, &nRemoteSockaddrLen
			);
		const Address ipRemote = getAddress((sockaddr_in*)lpRemoteSockaddr);
		CERL_LOG("accept", ("ACCEPT: %d (IP: %x)\n", sdAccept, ipRemote));
#endif

		return sdAccept;
	}
};

// -------------------------------------------------------------------------
// connectSocket

inline SOCKET cerl_call connectSocket(Fiber self, const SockaddrIn& si)
{
	return connectSocket(si);
/*
	CERL_FIBER_ASSERT_SELF(self);

	SOCKET sdConnect = createSocket();
	if (sdConnect == INVALID_SOCKET)
	{
		CERL_WARN("Socket", ("createSocket failed: %d\n", WSAGetLastError()));
		return INVALID_SOCKET;
	}

	// Get AccpetEx Function
	static LPFN_CONNECTEX lpfnConnectEx = NULL;
	if (lpfnConnectEx == NULL)
	{
		DWORD dwBytes = 0;
		GUID guid = WSAID_CONNECTEX;
		const int nRet = WSAIoctl(
			sdConnect, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid),
			&lpfnConnectEx,sizeof(lpfnConnectEx), &dwBytes, NULL, NULL);
		CERL_ASSERT(nRet != SOCKET_ERROR);
	}

	CerlIoService(self)->iocp.bind((HANDLE)sdConnect, (ULONG_PTR)IoService::ClientIoReadWrite);

	DWORD dwByteSended;
	detail::OVERLAPPED_IO o;
	ZeroMemory(&o, sizeof(o));
	o.fiber = self;
	o.initSerialNo();

	if (
		!lpfnConnectEx(
		sdConnect, (LPSOCKADDR)&si, sizeof(SOCKADDR_IN),
		NULL, 0, &dwByteSended, &o.Overlapped))
	{
		const DWORD nRet = WSAGetLastError();
		if (nRet != ERROR_IO_PENDING)
		{
			CERL_WARN("Socket", ("ERROR: ConnectEx failed - %d\n", nRet));
			closesocket(sdConnect);
			return INVALID_SOCKET;
		}
	}

	yield(self);

	if (setsockopt(sdConnect, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, NULL, 0) == SOCKET_ERROR)
	{
		closesocket(sdConnect);
		CERL_WARN("setsockopt", ("setsockopt(SO_UPDATE_ACCEPT_CONTEXT) failed to update accept socket: %d\n", WSAGetLastError()));
		return INVALID_SOCKET;
	}

	return sdConnect;
*/
}

inline SOCKET cerl_call connectSocket(Fiber self, const char* hostname, Port port)
{
	const SockaddrIn si(hostname, port);
	return connectSocket(si);
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_SOCKETIO_WINDOWS_H */
