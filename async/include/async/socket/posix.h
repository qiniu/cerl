/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/socket/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: posix.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_SOCKET_POSIX_H
#define ASYNC_SOCKET_POSIX_H

#ifndef ASYNC_BASIC_H
#include "../Basic.h"
#endif

#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

//#define SOCKET_ERROR		(-1)
//#define INVALID_SOCKET		(-1)
enum {
    INVALID_SOCKET = -1,
    SOCKET_ERROR = -1
};

typedef int SOCKET;

inline int cerl_call WSAGetLastError()
{
	return errno;
}

inline int cerl_call closesocket(SOCKET sd)
{
    return close(sd);
}

NS_CERL_BEGIN

// =========================================================================
// struct SOCKADDR_IN

typedef sockaddr_in SOCKADDR_IN;

// =========================================================================
// createSocket

inline SOCKET cerl_call createSocket()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

// =========================================================================
// class SocketInit

class SocketInit {};

// =========================================================================

NS_CERL_END

#endif /* ASYNC_SOCKET_POSIX_H */ 
