/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Basic.h 2576 2010-04-20 07:20:23Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_BASIC_H
#define ASYNC_BASIC_H

#ifndef STDEXT_BASIC_H
#include "../../../stdext/include/stdext/Basic.h"
#endif

#ifndef STDEXT_STRING_H
#include "../../../stdext/include/stdext/String.h"
#endif

#if (0)
#define CERL_VERBOSE
#define CERL_LOG_VERBOSE
#define CERL_LOG_NORMAL
#define CERL_LOG_WARN
#define CERL_LOG_ERROR
#define CERL_LOG_NOTHING
#endif

// -------------------------------------------------------------------------
// CERL_LOGLEVEL

#define CERL_LOGLEVEL_VERBOSE	(-1)
#define CERL_LOGLEVEL_NORMAL	0
#define CERL_LOGLEVEL_WARN		1
#define CERL_LOGLEVEL_ERROR		2
#define CERL_LOGLEVEL_NOTHING	3

#if defined(CERL_LOG_VERBOSE) || defined(CERL_VERBOSE)
#define CERL_LOGLEVEL	CERL_LOGLEVEL_VERBOSE
#elif defined(CERL_LOG_NORMAL)
#define CERL_LOGLEVEL	CERL_LOGLEVEL_NORMAL
#elif defined(CERL_LOG_WARN)
#define CERL_LOGLEVEL	CERL_LOGLEVEL_WARN
#elif defined(CERL_LOG_ERROR)
#define CERL_LOGLEVEL	CERL_LOGLEVEL_ERROR
#elif defined(CERL_LOG_NOTHING)
#define CERL_LOGLEVEL	CERL_LOGLEVEL_NOTHING
#else
#define CERL_LOGLEVEL	CERL_LOGLEVEL_NORMAL
#endif

// -------------------------------------------------------------------------

#if defined(X_CC_VC)
#pragma warning(disable:4101) // 'xxx' unreferenced local variable
#endif

// -------------------------------------------------------------------------

#ifndef NS_CERL
#define NS_CERL			cerl
#define NS_CERL_BEGIN	namespace cerl {
#define NS_CERL_END		}
#endif

#ifndef cerl_call
#define cerl_call		winx_call
#endif

#ifndef cerl_callback
#define cerl_callback	WINAPI
#endif

#ifndef cerl_mtcall
#define cerl_mtcall		winx_call /* 多个线程都可能调用该函数（不能访问BlockPool），但是该函数不需要锁（外部已经lock）。 */
#endif

#ifndef cerl_mt
#define cerl_mt			winx_call /* 多个线程同时调用，且需要考虑线程安全问题。 */
#endif

#ifndef CERL_ASSERT
#define CERL_ASSERT(e)	WINX_ASSERT(e)
#endif

#ifndef CERL_TRACE
#define CERL_TRACE		WINX_TRACE
#endif

#if defined(_DEBUG) && !defined(CERL_DIAGNOST)
#define CERL_DIAGNOST
#endif

//
// CERL_TRACE_IF(hasError, ("Failed: %d", errno));
//
#if defined(_DEBUG)
#define CERL_TRACE_IF(e, y)	if (e) CERL_TRACE y
#else
#define	CERL_TRACE_IF(e, y)	(void)0
#endif

//
// CERL_TR: 正常调试信息
//
// eg. CERL_TR("Send", ("Send %d bytes\n", bytes));
//
#ifndef CERL_TR
#define CERL_TR(f, y)	CERL_TRACE y
#endif

#ifdef CERL_ENABLE_LOG_MODULE
#ifndef LOG_LOGAPI_H
#include "../../../log/include/log/API.h"
#endif
#else
//
// CERL_VLOG: 详细日志信息
//
// eg. CERL_VLOG("Send", ("Send %d bytes\n", bytes));
//
#ifndef CERL_VLOG
#if (CERL_LOGLEVEL <= CERL_LOGLEVEL_VERBOSE)
#define CERL_VLOG(f, y)	NS_STDEXT::print y
#else
#define CERL_VLOG(f, y)	(void)0
#endif
#endif

//
// CERL_LOG: 正常日志信息
//
// eg. CERL_LOG("Send", ("Send %d bytes\n", bytes));
//
#if (CERL_LOGLEVEL <= CERL_LOGLEVEL_NORMAL)
#ifndef CERL_LOG
#define CERL_LOG(f, y)	NS_STDEXT::print y
#endif
#else
#define CERL_LOG(f, y)	(void)0
#endif

//
// CERL_WARN: 异常情况，但可容忍（逻辑考虑了这种异常情况并已经进行处理）
//
// eg. CERL_WARN("Send", ("Send %d bytes\n", bytes));
//
#if (CERL_LOGLEVEL <= CERL_LOGLEVEL_WARN)
#ifndef CERL_WARN
#define CERL_WARN(f, y)	NS_STDEXT::print y
#endif
#else
#define CERL_WARN(f, y)	(void)0
#endif

//
// CERL_ERROR: 异常情况，且出乎意料（程序还没有针对这种情况进行处理）
//
// eg. CERL_ERROR("Recv", ("Recv failed: %d\n", errno));
//
#if (CERL_LOGLEVEL <= CERL_LOGLEVEL_ERROR)
#ifndef CERL_ERROR
#define CERL_ERROR(f, y)													\
	do {																	\
		NS_STDEXT::print y;													\
		CERL_ASSERT(false && #y);											\
	} while (0)
#endif
#else
#define CERL_ERROR(f, y)	(void)0
#endif

#endif  //CERL_ENABLE_LOG_MODULE

// -------------------------------------------------------------------------
// CERL_FOR

#if defined(X_CC_VC6)
#define CERL_FOR		if (0) else for
#else
#define CERL_FOR		for
#endif

// -------------------------------------------------------------------------
// class String, CString

NS_CERL_BEGIN

typedef NS_STDEXT::String String;
typedef NS_STDEXT::CString CString;

NS_CERL_END

// -------------------------------------------------------------------------
// class LocalStorage

NS_CERL_BEGIN

template <size_t nMax>
class LocalStorage
{
private:
	void* m_localData[nMax];

public:
	LocalStorage()
	{
		memset(m_localData, 0, sizeof(void*) * nMax);
	}

	void* cerl_call get(size_t index)
	{
		CERL_ASSERT(index >= 0 && index < nMax);
		return m_localData[index];
	}

	void cerl_call set(size_t index, void* data)
	{
		CERL_ASSERT(index >= 0 && index < nMax);
		m_localData[index] = data;
	}
};

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_BASIC_H */
