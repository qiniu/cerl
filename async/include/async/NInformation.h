/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/NInformation.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: NInformation.h 2792 2010-05-06 10:27:46Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_NINFORMATION_H
#define ASYNC_NINFORMATION_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef ASYNC_IO_H
#include "Io.h"
#endif

#ifndef STDEXT_TEXT_CAST_H
#include "../../../stdext/include/stdext/text/Cast.h"
#endif

#ifndef STDEXT_HASH_H
#include "../../../stdext/include/stdext/Hash.h"
#endif

// -------------------------------------------------------------------------
// struct NInformation

NS_CERL_BEGIN

struct NInformation
{
	union
	{
		struct
		{
			UINT32 ip;
			UINT32 port;
		};
		UINT64 val;
	};

public:
	NInformation() { val = 0; }
	NInformation(Address addr_, Port port_)
	{
		ip = addr_;
		port = port_;
	}
	NInformation(const char* host_, Port port_)
	{
		ip = cerl::getAddress(host_);
		port = port_;
	}
	NInformation(const String& host_, Port port_)
	{
		ip = cerl::getAddress(host_);
		port = port_;
	}
	NInformation(const char* host)
	{
		const char* port_ = strchr(host, ':');
		if (port_)
		{
			ip = cerl::getAddress(String(host, port_));
			port = atoi(port_ + 1);
		}
		else
		{
			val = 0;
		}
	}
	NInformation(const String& host)
	{
		const String::const_iterator it = host.find(':');
		if (it != host.end())
		{
			typedef NS_STDEXT::Cast<UINT32> CastT;
			const String host_(host.begin(), it);
			ip = cerl::getAddress(host_);
			port = CastT::get(it+1, host.end(), 0);
		}
		else
		{
			val = 0;
		}
	}

	Address cerl_call getAddress() const {
		return ip;
	}

	Port cerl_call getPort() const {
		return port;
	}

	bool cerl_call good() const {
		return ip && port;
	}

	bool cerl_call operator==(const NInformation& b) const {
		return val == b.val;
	}

	bool cerl_call operator!=(const NInformation& b) const {
		return !operator==(b);
	}

	bool cerl_call operator<(const NInformation& b) const {
		return val < b.val;
	}

};


template<class T>
inline void cerl_call ip2str(T& dest, UINT32 ip)
{
	const unsigned char* addr = (unsigned char*)&ip;
	NS_STDEXT::formatAppend(dest, "%u.%u.%u.%u", addr[3], addr[2], addr[1], addr[0]);
}

template<class T>
inline void cerl_call port2str(T& dest, UINT32 port)
{
	NS_STDEXT::formatAppend(dest, "%u", port);
}

template<class T>
inline void cerl_call ninfo2str(T& dest, const NInformation& ninfo)
{
	ip2str(dest, ninfo.getAddress());
	dest.append(":");
	port2str(dest, ninfo.getPort());
}


typedef NInformation NInformationArg;
typedef std::vector<cerl::NInformation> NInfoArray;


NS_CERL_END

STD_NO_DESTRUCTOR(NS_CERL::NInformation)

// -------------------------------------------------------------------------
// class Hash<NInformation>

NS_STDEXT_BEGIN

template <>
class Hash<NS_CERL::NInformation>
{
public:
	size_t winx_call operator()(const NS_CERL::NInformation& v) const {
		return v.ip ^ v.port;
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// function connectSocket

NS_CERL_BEGIN

inline SOCKET cerl_call connectSocket(const NInformation& ni)
{
	return connectSocket(SockaddrIn(ni.ip, ni.port));
}

inline SOCKET cerl_call connectSocket(Fiber self, const NInformation& ni, int timeout = 10000)
{
	return connectSocket(self, SockaddrIn(ni.ip, ni.port), timeout);
}

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_NINFORMATION_H */
