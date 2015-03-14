/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/socket/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: windows.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_SOCKET_WINDOWS_H
#define ASYNC_SOCKET_WINDOWS_H

#ifndef ASYNC_BASIC_H
#include "../Basic.h"
#endif

#ifndef _WINSOCK2API_
#include <winsock2.h>
#endif

#ifndef _MSWSOCK_
#include <mswsock.h> 
#endif

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")

NS_CERL_BEGIN

// =========================================================================
// createSocket

//
// Create a socket with all the socket options we need, namely disable buffering 
// and set linger.
//
inline SOCKET cerl_call createSocket()
{
	int nRet, nBufLen = 0;

	SOCKET sd = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	CERL_TRACE_IF(INVALID_SOCKET == sd, ("ERROR: createSocket failed!\n"));

	// Disable send buffering on the socket.  Setting SO_SNDBUF
	// to 0 causes winsock to stop bufferring sends and perform
	// sends directly from our buffers, thereby reducing CPU usage.
	nRet = setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (char*)&nBufLen, sizeof(nBufLen));
	CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: createSocket - setsockopt failed!\n"));

	// Disable receive buffering on the socket.  Setting SO_RCVBUF  
	// to 0 causes winsock to stop bufferring receive and perform 
	// receives directly from our buffers, thereby reducing CPU usage. 
	nRet = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (char *)&nBufLen, sizeof(nBufLen));
	CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: createSocket - setsockopt failed!\n"));

	LINGER lingerStruct;
	lingerStruct.l_onoff = 0;
	lingerStruct.l_linger = 0;
	nRet = setsockopt(sd, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct) ); 
	CERL_TRACE_IF(SOCKET_ERROR == nRet, ("ERROR: createSocket - setsockopt failed!\n"));

	return sd;
}

// =========================================================================
// class SocketInit

class SocketInit
{
public:
	SocketInit()
	{
		WSADATA wsaData;
		const WORD wVersionRequested = MAKEWORD(2, 2);
		WSAStartup(wVersionRequested, &wsaData);
	}
	~SocketInit()
	{
		WSACleanup();
	}
};

// =========================================================================

NS_CERL_END

#endif /* ASYNC_SOCKET_WINDOWS_H */
