/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Debug.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Debug.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef VENUS_DEBUG_H
#define VENUS_DEBUG_H

#ifndef VENUS_SERIALIZATION_H
#include "Serialization.h"
#endif

#ifndef VENUS_DUMP_H
#include "Dump.h"
#endif

// -------------------------------------------------------------------------
// struct Fid2Name

NS_CERL_BEGIN

struct Fid2Name
{
	int fid;
	const char* name;
	bool cerl_call operator==(const int fid2) const {
		return fid == fid2;
	}
};

inline const char* cerl_call fid2name(FID fid, const Fid2Name* tbl, size_t n)
{
	const Fid2Name* tbl_end = tbl + n;
	const Fid2Name* it = std::find(tbl, tbl_end, fid);
	if (it != tbl_end)
		return it->name;
	return NULL;
}

NS_CERL_END

// -------------------------------------------------------------------------
// struct _DebugResult

NS_CERL_BEGIN

struct _DebugResult
{
	cerl::Code _code;
	cerl::String value;

	_DebugResult() {}
	_DebugResult(cerl::Code code) {
		_code = code;
	}
	operator cerl::Code() const {
		return _code;
	}
	void cerl_call operator=(cerl::Code code) {
		_code = code;
	}
	template <class LogT>
	void cerl_call dumpCode(LogT& log) const {
		NS_CERL_IO::dumpCode(log, _code);
	}
};

NS_CERL_END

// -------------------------------------------------------------------------
// put(os, const _DebugResult& result), get(is, _DebugResult& result)

NS_CERL_IO_BEGIN

template <class LogT>
inline void cerl_call dump(LogT& log, const _DebugResult& result)
{
	dump(log, '{');
	result.dumpCode(log);
	if (result._code == code_ok)
	{
		dump(log, ", ");
		dump(log, result.value);
	}
	dump(log, '}');
}

template <class OutputStreamT>
inline void cerl_call put(OutputStreamT& os, const _DebugResult& result)
{
	NS_CERL_IO::put(os, result._code);
	if (result._code == code_ok)
	{
		NS_CERL_IO::put(os, result.value);
	}
}

template <class InputStreamT>
inline bool cerl_call get(InputStreamT& is, _DebugResult& result)
{
	if (!NS_CERL_IO::get(is, result._code))
		return false;

	if (result._code == code_ok)
	{
		return NS_CERL_IO::getString(is, result.value);
	}
	return true;
}

NS_CERL_IO_END

// -------------------------------------------------------------------------
// dump State

NS_CERL_BEGIN

enum DebugCode
{
	debug_invalid_op = 0,
	debug_dump_state = 1,
	debug_init_state = 2,
};

template <class ConnectionT>
inline String cerl_call dumpState(Fiber* self, ConnectionT& conn)
{
	_DebugResult result;
	conn.call(self, result, CERL_FID_DEBUG, (Code)debug_dump_state);
	return result.value;
}

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* VENUS_DEBUG_H */
