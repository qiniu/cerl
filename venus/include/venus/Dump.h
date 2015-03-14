/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Dump.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Dump.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef VENUS_DUMP_H
#define VENUS_DUMP_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

#ifndef NS_CERL_IO
#define NS_CERL_IO			NS_CERL::io
#define NS_CERL_IO_BEGIN	NS_CERL_BEGIN namespace io {
#define NS_CERL_IO_END		} NS_CERL_END
#endif

NS_CERL_IO_BEGIN

// =========================================================================
// dump Basic Types

template <class LogT>
inline void cerl_call dump(LogT& log, bool val)
{
	log.print((int)val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, char val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, Byte val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, Int16 val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, UInt16 val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, Int32 val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, UInt32 val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, Int64 val)
{
	log.print(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, UInt64 val)
{
	log.print(val);
}

template <class LogT>
inline void winx_call dump(LogT& log, const char* val)
{
	log.printString(val);
}

template <class LogT>
inline void cerl_call dump(LogT& log, const NInformation& val)
{
	log.print(val.ip)
		.print(':')
		.print(val.port);
}

template <class LogT>
inline void winx_call dump(LogT& log, const NoArgs& val)
{
	dump(log, "void");
}

// =========================================================================
// dump Array Types

template <class LogT>
inline void cerl_call dump(LogT& log, const CharRange& val)
{
	log.print(val.size());
	log.print(":\"");
	if (val.size() > 64)
	{
		log.printString(val.begin(), val.begin() + 64);
		log.print("...");
	}
	else
	{
		log.printString(val.begin(), val.end());
	}
	log.print('\"');
}

template <class LogT>
inline void cerl_call dump(LogT& log, const String& val)
{
	dump(log, *(const CharRange*)&val);
}

//
// compile error (gcc) fixed: move code to <venus/Io.inl>
//
template <class LogT, class DataIt>
void winx_call dumpArray(
	LogT& log, DataIt first, size_t count,
	const char* bracketL = "[", const char* bracketR = "]", const char* dataSep = ", ");

template <class LogT, class Type>
void cerl_call dump(LogT& log, const NS_STDEXT::Range<const Type*, Type>& val);

template <class LogT, class Type, size_t m_size>
void cerl_call dump(LogT& log, const Array<Type, m_size>& val);

// =========================================================================
// dump Code

template <class LogT>
inline void winx_call dumpCode(LogT& log, Code code)
{
	if (code <= 2) {
		static const char* g_descs[] = {
			"false", "true", "ok"
		};
		dump(log, g_descs[code]);
		return;
	}
	else if (code >= 0xf000) {
		static const char* g_descs[] = {
			"error", "timeout_error", "format_error", "unknown_fid",
			"input_error", "encounter_exception", "socket_error", "undefined",
			"exists_already",
		};
		const size_t idx = (code ^ 0xffff);
		if (idx < countof(g_descs))
		{
			dump(log, g_descs[idx]);
			return;
		}
	}
	dump(log, "code:");
	dump(log, code);
}

// =========================================================================

NS_CERL_IO_END

#endif /* VENUS_DUMP_H */
