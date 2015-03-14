/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Socket.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Socket.h 2512 2010-04-19 03:03:28Z yuxihe $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_SOCKET_H
#define ASYNC_SOCKET_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef CERL_BACKLOG
#define CERL_BACKLOG	512
#endif

// -------------------------------------------------------------------------

NS_CERL_BEGIN

typedef UINT32 Port;
typedef UINT32 Address;

NS_CERL_END

// -------------------------------------------------------------------------

#if defined(X_OS_LINUX)
#include "socket/posix.h"
#elif defined(X_OS_WINDOWS)
#include "socket/windows.h"
#include <ws2tcpip.h>
#else
#error "error: Unsupport OS platform!!!"
#endif

// -------------------------------------------------------------------------

//
// dbg_closesocket
//
#if defined(CERL_DIAGNOST)

inline void cerl_call dbg_closesocket(const char* op, SOCKET sd)
{
	closesocket(sd);
	CERL_LOG("closesocket", ("STOP-%s: %d\n", op, sd));
}

#else
#define dbg_closesocket(op, sd)	closesocket(sd)
#endif

// -------------------------------------------------------------------------

NS_CERL_BEGIN

//
// getAddress
//
inline Address cerl_call getAddress(sockaddr_in* addr)
{
	return ntohl(addr->sin_addr.s_addr);
}

inline Address cerl_call getAddress(const char* hostname)
{
#if !defined(X_OS_WINDOWS)
	if (*hostname == 0)
		return 0x7F000001;
#endif

	struct addrinfo* addrlist;
	int s = 0;
	if ((s = getaddrinfo(hostname, NULL, NULL, &addrlist)) != 0)
	{
		CERL_WARN("Address", ("getaddrinfo '%s' failed: %s\n", hostname, gai_strerror(s)));
		return 0;
	}
	CERL_ASSERT(addrlist);

	Address result = ntohl(((struct sockaddr_in*)addrlist->ai_addr)->sin_addr.s_addr);
	freeaddrinfo(addrlist);
	return result;
}

inline Address cerl_call getAddress(const String& host)
{
	const size_t n = host.size();

	char* const hostname = (char*)_alloca(n + 1);
	memcpy(hostname, host.data(), n);
	hostname[n] = '\0';

	return getAddress(hostname);
}

//
// struct SockaddrIn
//
struct SockaddrIn : public SOCKADDR_IN
{
public:
	SockaddrIn(Port port)
	{
		sin_family = AF_INET;
		sin_port = htons(port);
		sin_addr.s_addr = htonl(INADDR_ANY);
	}

	SockaddrIn(Address ip, Port port)
	{
		sin_family = AF_INET;
		sin_port = htons(port);
		sin_addr.s_addr = htonl(ip);
	}

	SockaddrIn(const char* host_, Port port_)
	{
		sin_family = AF_INET;
		sin_port = htons(port_);
		sin_addr.s_addr = htonl(cerl::getAddress(host_));
	}

	SockaddrIn(const char* host)
	{
		sin_family = AF_INET;
		Address ip_;
		Port port_;
		const char* sport_ = strchr(host, ':');
		if (sport_)
		{
			ip_ = cerl::getAddress(String(host, sport_));
			port_ = atoi(sport_ + 1);
		}
		else
		{
			ip_ = 0;
			port_ = 0;
		}
		sin_port = htons(port_);
		sin_addr.s_addr = htonl(ip_);
	}

	SockaddrIn(const in_addr& ip, Port port)
	{
		sin_family = AF_INET;
		sin_port = htons(port);
		sin_addr = ip;
	}

	__forceinline int cerl_call bind(SOCKET sd) const
	{
		return ::bind(sd, (sockaddr*)this, sizeof(SOCKADDR_IN));
	}

	__forceinline int cerl_call connect(SOCKET sd) const
	{
		return ::connect(sd, (sockaddr*)this, sizeof(SOCKADDR_IN));
	}
};


//
//  Create a listening socket, bind, and set up its listening backlog.
// 
inline SOCKET cerl_call listenSocket(
	const SockaddrIn& si, int backlog = CERL_BACKLOG)
{
	SOCKET sd = createSocket();
	CERL_TRACE_IF(INVALID_SOCKET == sd, ("ERROR: listenSocket - createSocket failed!\n"));

	int value = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char*)&value, sizeof(value));

	int nRet = si.bind(sd);
	CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: listenSocket - bind failed!\n"));

	if (SOCKET_ERROR != nRet)
	{
		nRet = listen(sd, backlog);
		CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: listenSocket - listen failed!\n"));

		if (SOCKET_ERROR != nRet)
		{
			CERL_LOG("Socket", ("LISTEN: %d {PORT: %d}\n", sd, ntohs(si.sin_port)));

			return sd;
		}
	}

	closesocket(sd);
	return INVALID_SOCKET;
}

//
//  Create a listening socket, bind, and set up its listening backlog.
// 
template <class HostT>
inline SOCKET cerl_call listenSocketEx(
	HostT& local, unsigned bind_retry_times, int backlog = CERL_BACKLOG)
{
	SOCKET sd = createSocket();
	CERL_TRACE_IF(INVALID_SOCKET == sd, ("ERROR: listenSocket - createSocket failed!\n"));

	int nRet;
	SockaddrIn si(local.ip, local.port);
	for (;;)
	{
		if ((nRet = si.bind(sd)) != SOCKET_ERROR)
			break;
		if (bind_retry_times == 0)
			goto lzError;
		si.sin_port = htons(++local.port);
		--bind_retry_times;
	}

	nRet = listen(sd, backlog);
	CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: listenSocket - listen failed!\n"));
	if (nRet != SOCKET_ERROR)
	{
		CERL_TRACE("LISTEN: %d {PORT: %d}\n", sd, local.port);
		return sd;
	}

lzError:
	closesocket(sd);
	return INVALID_SOCKET;
}

inline SOCKET cerl_call connectSocket(const SockaddrIn& si)
{
	SOCKET sd = createSocket();
	CERL_TRACE_IF(INVALID_SOCKET == sd, ("ERROR: connectSocket - createSocket failed!\n"));

	if (0 == si.connect(sd))
	{
		CERL_LOG("Socket", ("CONNECT: %d {PORT: %d}\n", sd, ntohs(si.sin_port)));
		return sd;
	}

	CERL_WARN("Socket", ("connectSocket failed! errno: %d, port: %d!\n", WSAGetLastError(), ntohs(si.sin_port)));
	closesocket(sd);
	return INVALID_SOCKET;
}

inline SOCKET cerl_call connectSocket(const char* hostname, Port port)
{
	const SockaddrIn si(getAddress(hostname), port);
	return connectSocket(si);
}

inline size_t cerl_call sendn(SOCKET s, const void* buf, size_t len, int flags = 0)
{
	size_t bytes = 0;
	for (;;)
	{
		const int t = ::send(s, (const char*)buf, len, flags);
		if (t == (int)len)
			return bytes + len;
		if (t <= 0)
			return bytes;
		buf = (const char*)buf + t;
		len -= (size_t)t;
		bytes += t;
	}
}

inline size_t cerl_call recvn(SOCKET s, void* buf, size_t len, int flags = 0)
{
	size_t bytes = 0;
	for (;;)
	{
		const int t = ::recv(s, (char*)buf, len, flags);
		if (t == (int)len)
			return bytes + len;
		if (t <= 0)
			return bytes;
		buf = (char*)buf + t;
		len -= t;
		bytes += t;
	}
}

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_SOCKET_H */
