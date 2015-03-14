/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Mail.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Mail.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef VENUS_MAIL_H
#define VENUS_MAIL_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

#ifndef VENUS_SERIALIZATION_H
#include "Serialization.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------

typedef UINT32 MailNo;

// -------------------------------------------------------------------------
// struct MailHeader

#define VENUS_MAIL_MAGIC_CODE 0x99008633

#pragma pack(1)

struct MailHeader
{
	UINT16 cbHeader;
	UINT16 reserved;
	UINT32 magic;
	UINT32 mailno;
    UINT32 fid;
};

#pragma pack()

// -------------------------------------------------------------------------
/*
#define VENUS_MAILNO_RET_MASK		0x40000000UL
#define VENUS_MAILNO_ASYNC_MASK		0x80000000UL
#define VENUS_MAILNO_SYNC_MASK		0x3FFFFFFFUL
#define VENUS_MAILNO_MASKS			(VENUS_MAILNO_RET_MASK | VENUS_MAILNO_ASYNC_MASK)

__forceinline int cerl_call mailno_is_sync(UINT32 mailno) {
	return !(mailno & VENUS_MAILNO_MASKS);
}

__forceinline int cerl_call mailno_is_ret(UINT32 mailno) {
	return mailno & VENUS_MAILNO_RET_MASK;
}

__forceinline int cerl_call mailno_is_async(UINT32 mailno) {
	return mailno == VENUS_MAILNO_ASYNC_MASK;
}
*/

// -------------------------------------------------------------------------
// function put/get/skip mail body

template <class OutputStreamT, class MailBodyT>
inline bool cerl_call putMailBody(OutputStreamT& os, const MailBodyT& val)
{
	NS_CERL_IO::put(os, val);
	return NS_CERL_IO::put_vt_null(os);
}

template <class InputStreamT, class MailBodyT>
inline bool cerl_call getMailBody(InputStreamT& is, MailBodyT& val)
{
	return NS_CERL_IO::get(is, val) && NS_CERL_IO::check_vt_null(is);
}

template <class InputStreamT>
inline void cerl_call skipMailBody(InputStreamT& is)
{
	while (NS_CERL_IO::skip_one(is) > NS_CERL_IO::vtNull);
}

// -------------------------------------------------------------------------
// function dump mail

class Connection;

namespace detail {

struct Fid2Name_
{
	int fid;
	const char* name;
	bool cerl_call operator==(const int fid2) const {
		return fid == fid2;
	}
};

inline const char* cerl_call get_fid_(int fid, char buf[16])
{
	const Fid2Name_ g_tbl[] =
	{
		{ CERL_FID_DEBUG, "debug" },
	};

	const Fid2Name_* tbl_end = g_tbl + countof(g_tbl);
	const Fid2Name_* it = std::find(g_tbl, tbl_end, fid);
	if (it != tbl_end)
		return it->name;

	sprintf(buf, "%.8X", fid);
	return buf;
}

template <class ServerT>
inline const char* _fid2name(ServerT* server, int fid) {
	return server->_fid2name((FID)fid);
}

inline const char* _fid2name(Connection* server, int fid) {
	return NULL;
}

} // namespace detail

template <class LogT, class ServerT, class ResultT>
inline void cerl_call dumpMail(
	LogT& log, const char* op, ServerT* server, int fid, const ResultT& args, bool fReply = false)
{
	char buf[16];
	const char* name = detail::_fid2name(server, fid);

#if (0)
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	printf("[%.8x] ", li.LowPart);
#endif

	NS_CERL_IO::dump(log, op);
	NS_CERL_IO::dump(log, ": ");
	if (fReply)
		NS_CERL_IO::dump(log, "ret ");
	NS_CERL_IO::dump(log, name ? name : detail::get_fid_(fid, buf));
	NS_CERL_IO::dump(log, " - ");
	NS_CERL_IO::dump(log, args);
	log.newline();
}

// -------------------------------------------------------------------------
// CERL_DUMP_MAIL

#if defined(CERL_DEBUG)
#define CERL_DUMP_REQUEST(self, op, server, fid, args)			cerl::dumpMail(CerlDefaultLog(self), op, server, fid, args)
#define CERL_DUMP_REPLY(self, op, server, fid, result)			cerl::dumpMail(CerlDefaultLog(self), op, server, fid, result, true)
#else
#define CERL_DUMP_REQUEST(self, op, server, fid, args)			(void)NULL
#define CERL_DUMP_REPLY(self, op, server, fid, result)			(void)NULL
#endif

// -------------------------------------------------------------------------

#define CERL_DUMP_SEND_REQUEST(self, server, fid, args)			CERL_DUMP_REQUEST(self, "SEND", server, fid, args)
#define CERL_DUMP_RECV_REQUEST(self, server, fid, args)			CERL_DUMP_REQUEST(self, "RECV", server, fid, args)
#define CERL_DUMP_SEND_REPLY(self, server, fid, result)			CERL_DUMP_REPLY(self, "SEND", server, fid, result)
#define CERL_DUMP_RECV_REPLY(self, server, fid, result)			CERL_DUMP_REPLY(self, "DUMP", server, fid, result)

// -------------------------------------------------------------------------

NS_CERL_END

#endif
