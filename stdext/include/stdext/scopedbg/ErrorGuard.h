/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/scopedbg/ErrorGuard.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Contributor: sting.feng@gmail.com
// Date: 2007-2-1 13:56:50
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_SCOPEDBG_ERRORGUARD_H
#define STDEXT_SCOPEDBG_ERRORGUARD_H

#if defined(_MSC_VER)
	#ifndef STDEXT_SCOPEDBG_CHECKEXCEPTION_H
	#include "CheckException.h"
	#endif
#else
	#error "todo"
#endif

#ifndef STDEXT_SCOPEDBG_SCOPELOG_H
#include "ScopeLog.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// WINX_GUARD_LOG

typedef ThreadLog<FileScopeLog>	ThreadFileScopeLog;
typedef ThreadLogInit<FileScopeLog>	ThreadFileScopeLogInit;

#define WINX_THREAD_LOG_INIT()		std::ThreadFileScopeLog::init()
#define WINX_THREAD_LOG_TERM()		std::ThreadFileScopeLog::term()

#define WINX_THREAD_LOG				std::ThreadFileScopeLog::instance()

// =========================================================================
// class ExceptionGuard

#pragma warning(disable:4996) // XXX  was declared deprecated

class _ExceptionGuardBase
{
private:
	const char* m_func;
	const char* m_file;
	const int m_line;

public:
	FileScopeLog& m_log;

public:
	_ExceptionGuardBase(const char* func, const char* file, int line)
		: m_func(func), m_file(file), m_line(line), m_log(WINX_THREAD_LOG)
	{
		m_log.traceScopeMessage(m_func, "Enter", m_file, m_line);
		m_log.enterScope();
	}

	template <class CharT>
	void winx_call operator()(const CharT* fmt, ...)
	{
		try
		{
			va_list args;
			va_start(args, fmt);
			m_log.vtrace(fmt, args).newline();
			va_end(args);
		}
		catch (...)
		{
			m_log.reportGuardError("Exception", 0, "Exception when dumping params", m_file, m_line);
		}
	}

	void winx_call onLeave(bool isNormal)
	{
		if (!isNormal)
		{
			char msg[1024];
			sprintf(msg, "Exception in '%s'", m_func);
			m_log.reportGuardError("Exception", 0, msg, m_file, m_line);
		}
		m_log.leaveScope();
		if (isNormal)
		{
			m_log.traceScopeMessage(m_func, "Leave", m_file, m_line);
		}
	}
};

#pragma warning(default:4996) // XXX  was declared deprecated

typedef CheckException<_ExceptionGuardBase> ExceptionGuard;

#ifndef WINX_GUARD
#define WINX_GUARD(func)													\
	std::ExceptionGuard _winx_guard(func, __FILE__, __LINE__);				\
	_winx_guard
#endif

#ifndef WINX_GUARD_LOG
#define WINX_GUARD_LOG _winx_guard.m_log
#endif

#ifndef WINX_EXCEPTION_GUARD
#define WINX_EXCEPTION_GUARD(func)											\
	WINX_GUARD(func)
#endif

// =========================================================================
// reportGuardWin32Error

template <class LogT>
inline void winx_call reportGuardWin32Error(
	LogT& log, LONG error, const char* file, int line, LPCSTR general = "Win32Error")
{
	LPSTR lpMsgBuf;
	::FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPSTR)&lpMsgBuf,
		0, NULL
		);
	log.reportGuardError(general, error, lpMsgBuf, file, line);
	::LocalFree(lpMsgBuf);
}

// -------------------------------------------------------------------------
// class Win32ErrorGuard

class Win32ErrorGuard
{
private:
	const LONG& m_result;
	const char* m_file;
	const int m_line;

public:
	Win32ErrorGuard(const LONG& result, const char* file, int line)
		: m_result(result), m_file(file), m_line(line) {
	}
	~Win32ErrorGuard() {
		if (m_result != 0) {
			reportGuardWin32Error(WINX_THREAD_LOG, m_result, m_file, m_line);
		}
	}
};

#ifndef WINX_WIN32_ERROR_GUARD
#define WINX_WIN32_ERROR_GUARD(result)										\
	std::Win32ErrorGuard _winx_guard_win32err(result, __FILE__, __LINE__)
#endif

// -------------------------------------------------------------------------
// class HResultErrorGuard

class HResultErrorGuard
{
private:
	const LONG& m_result;
	const char* m_file;
	const int m_line;

public:
	HResultErrorGuard(const LONG& result, const char* file, int line)
		: m_result(result), m_file(file), m_line(line) {
	}
	~HResultErrorGuard() {
		if (FAILED(m_result)) {
			reportGuardWin32Error(WINX_THREAD_LOG, m_result, m_file, m_line, "HResultError");
		}
	}
};

#ifndef WINX_HRESULT_ERROR_GUARD
#define WINX_HRESULT_ERROR_GUARD(result)									\
	std::HResultErrorGuard _winx_guard_hresult(result, __FILE__, __LINE__)
#endif

// =========================================================================
// class TestErrorGuard

template <class LogT>
class TestErrorGuard
{
	WINX_TEST_SUITE(TestErrorGuard);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void foo()
	{
		WINX_GUARD("TestErrorGuard::foo()");
	}

	HRESULT foo(int arg1, const char* arg2)
	{
		LONG result;
		WINX_GUARD("TestErrorGuard::foo(arg1, arg2)")("Params: arg1=%d, arg2='%s'", arg1, arg2);
		WINX_WIN32_ERROR_GUARD(result);

		foo();

		::CreateDirectoryA("C:\\", NULL);
		result = ::GetLastError();
		throw 0;
	}

	void test(LogT& log)
	{
		ThreadFileScopeLogInit logInit("/__ErrorGuard__.log");
		{
			try
			{
				foo();
				foo(100, "abc");
			}
			catch (...) {
			}
		}
	}
};

// =========================================================================
// $Log: $

NS_STDEXT_END


#endif /* STDEXT_SCOPEDBG_ERRORGUARD_H */
