/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/exmaples/calc/calc_base.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: calc_base.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef CALC_BASE_H
#define CALC_BASE_H

#ifndef VENUS_IO_H
#include <venus/Io.h>
#endif

#pragma pack(1)

namespace calc {
	
enum { code_ok = cerl::code_ok };
enum { code_error = cerl::code_error };
enum { code_true = cerl::code_true };
enum { code_false = cerl::code_false };

// =========================================================================
// class AbacusBase
 
class AbacusBase
{
public:
	enum { code_add = 1 };

#if defined(VENUS_DEBUG_H)
	static const char* cerl_call _fid2name(cerl::FID fid)
	{
		static const cerl::Fid2Name g_tbl[] =
		{
			{ code_add, "add" },
		};
		return cerl::fid2name(fid, g_tbl, countof(g_tbl));
	}
#endif

	typedef struct {
		cerl::UInt32 a;
		cerl::UInt32 b;
	}_AddArgs;

	typedef struct _tagAddResult {
		cerl::Code _code;
		struct {
			cerl::UInt32 sum;
		};

		_tagAddResult(cerl::Code code = cerl::code_error) {
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
	}_AddResult;
};

}

NS_CERL_IO_BEGIN

template <class LogT>
inline void dump(LogT& log, const calc::AbacusBase::_AddArgs& args)
{
	NS_CERL_IO::dump(log, '{');
	NS_CERL_IO::dump(log, args.a);
	NS_CERL_IO::dump(log, ", ");
	NS_CERL_IO::dump(log, args.b);
	NS_CERL_IO::dump(log, '}');
}

template <class AllocT>
inline void copy(AllocT& alloc, calc::AbacusBase::_AddArgs& dest, const calc::AbacusBase::_AddArgs& src)
{
	dest = src;
}

template <class OutputStreamT>
inline void put(OutputStreamT& wr, const calc::AbacusBase::_AddArgs& args)
{
	NS_CERL_IO::put(wr, args.a);
	NS_CERL_IO::put(wr, args.b);
}

template <class InputStreamT>
inline bool get(InputStreamT& ar, calc::AbacusBase::_AddArgs& args)
{
	return NS_CERL_IO::get(ar, args.a) 
		&& NS_CERL_IO::get(ar, args.b);
}

template <class LogT>
inline void cerl_call dump(LogT& log, const calc::AbacusBase::_AddResult& result)
{
	NS_CERL_IO::dump(log, '{');
	result.dumpCode(log);
	switch (result._code)
	{
	case cerl::code_ok:
		NS_CERL_IO::dump(log, ", ");
		NS_CERL_IO::dump(log, result.sum);
		break;
	default:
		break;
	}
	NS_CERL_IO::dump(log, '}');
}

template <class AllocT>
inline void copy(AllocT& alloc, calc::AbacusBase::_AddResult& dest, const calc::AbacusBase::_AddResult& src)
{
	dest = src;
}

template <class OutputStreamT>
inline void put(OutputStreamT& wr, const calc::AbacusBase::_AddResult& result)
{
	NS_CERL_IO::put(wr, result._code);

	switch (result._code)
	{
	case cerl::code_ok:
		NS_CERL_IO::put(wr, result.sum);
		break;
	default:
		break;
	}
}

template <class InputStreamT>
inline bool get(InputStreamT& ar, calc::AbacusBase::_AddResult& result)
{
	if (!NS_CERL_IO::get(ar, result._code))
		return false;
	
	switch(result._code)
	{
	case cerl::code_ok:
		return NS_CERL_IO::get(ar, result.sum);
	default:
		return true;
	}
}

NS_CERL_IO_END

#pragma pack()

#endif /* CALC_BASE_H */
