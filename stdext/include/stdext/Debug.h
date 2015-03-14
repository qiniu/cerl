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
// Module: stdext/Debug.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Contributor: sting.feng@gmail.com
// Date: 2007-2-3 17:32:38
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DEBUG_H
#define STDEXT_DEBUG_H

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// DbgFillMemory

#if !defined(DbgFillMemory)

#ifdef _DEBUG
#define DbgFillMemory(p, len)	memset(p, 0xcd, len)
#else
#define DbgFillMemory(p, len)	0
#endif

#endif

// -------------------------------------------------------------------------
// _DEBUG_NEW

#ifndef _DEBUG_NEW
#define _DEBUG_NEW			new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// -------------------------------------------------------------------------
// WINX_ASSERT/WINX_TRACE - diagnost

#if defined(ASSERT)
#define WINX_ASSERT(e)		ASSERT(e)
#elif defined(_ASSERTE)
#define WINX_ASSERT(e)		_ASSERTE(e)
#else
#ifdef _DEBUG
#define WINX_ASSERT(e)		assert(e)
#else
#define WINX_ASSERT(e)
#endif
#endif

#if defined(ASSERT_ONCE)
#define WINX_ASSERT_ONCE(e)	ASSERT_ONCE(e)
#elif defined(_DEBUG)
#define WINX_ASSERT_ONCE(e)													\
do {																		\
	static int fReported = 0;												\
	if (!fReported && !(e))													\
	{																		\
		WINX_ASSERT(e);														\
		fReported = 1;														\
	}																		\
} while(0)
#else
#define WINX_ASSERT_ONCE(e)	0
#endif

#if defined(REPORTA)
#define WINX_REPORT(msg)	REPORTA(msg)
#elif defined(REPORT)
#define WINX_REPORT(msg)	REPORT(msg)
#else
#define WINX_REPORT(msg)	WINX_ASSERT(!msg)
#endif

#if defined(REPORT_ONCE)
#define WINX_REPORT_ONCE(msg)	REPORT_ONCE(msg)
#else
#define WINX_REPORT_ONCE(msg)	WINX_ASSERT_ONCE(!msg)
#endif

#if defined(VERIFY)
#define WINX_VERIFY(e)		VERIFY(e)
#elif defined(_DEBUG)
#define WINX_VERIFY(e)		WINX_ASSERT(e)
#else
#define WINX_VERIFY(e)		e
#endif

#if defined(_DEBUG)
#define WINX_VERIFY_EQ(e, v)	WINX_VERIFY((e) == v)
#else
#define WINX_VERIFY_EQ(e, v)	e
#endif

#if defined(ATLTRACE)
#define WINX_TRACE			ATLTRACE
#elif defined(TRACEA)
#define WINX_TRACE			TRACEA
#elif defined(TRACE)
#define WINX_TRACE			TRACE
#elif defined(_DEBUG)
#define WINX_TRACE			printf
#else
#define WINX_TRACE			__noop
#endif

#if defined(ASSERT_OK)
#define WINX_ASSERT_OK(hr)	ASSERT_OK(hr)
#else
#define WINX_ASSERT_OK(hr)	WINX_ASSERT(SUCCEEDED(hr))
#endif

#if defined(VERIFY_OK)
#define WINX_VERIFY_OK(e)	VERIFY_OK(e)
#elif defined(_DEBUG)
#define WINX_VERIFY_OK(e)	WINX_ASSERT_OK(e)
#else
#define WINX_VERIFY_OK(e)	e
#endif

// -------------------------------------------------------------------------
// WINX_DEBUG_ONLY

#if defined(_DEBUG)
#define WINX_DEBUG_ONLY(e)	e
#else
#define WINX_DEBUG_ONLY(e)	NULL
#endif

// -------------------------------------------------------------------------
// WINX_ASSERT_DERIVE - diagnost

#if defined(ASSERT_DERIVE)
#define WINX_ASSERT_DERIVE(TestClass, BaseClass)	ASSERT_DERIVE(TestClass, BaseClass)
#else
#define WINX_ASSERT_DERIVE(TestClass, BaseClass)	0
#endif

// -------------------------------------------------------------------------
// WINX_STATIC_ASSERT

#if defined(STATIC_ASSERT)
#define WINX_STATIC_ASSERT(e)	STATIC_ASSERT(e)
#elif defined(BOOST_STATIC_ASSERT)
#define WINX_STATIC_ASSERT(e)	BOOST_STATIC_ASSERT(e)
#else
#define WINX_STATIC_ASSERT(e)	WINX_ASSERT(e)
#endif

// -------------------------------------------------------------------------
// WINX_BREAK

#if defined(_CrtDbgBreak)
#define WINX_BREAK()			_CrtDbgBreak()
#else
#define WINX_BREAK()			__asm { int 3 }
#endif

// -------------------------------------------------------------------------
// WINX_DBG_BREAK

#if defined(_DEBUG)
#define WINX_DBG_BREAK()		WINX_BREAK()
#else
#define WINX_DBG_BREAK()
#endif

// -------------------------------------------------------------------------
// class DllMainInit

#ifndef DLL_PROCESS_ATTACH
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH  2
#define DLL_THREAD_DETACH  3
#define DLL_PROCESS_DETACH 0
#endif

#if defined(_DEBUG)

inline void winx_call DllMainInit(
	HANDLE hModule, DWORD ul_reason_for_call, long nBreakAlloc = 0)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
		if (nBreakAlloc > 0)
			_CrtSetBreakAlloc(nBreakAlloc);
		break;
	case DLL_PROCESS_DETACH:
		char szModule[_MAX_PATH];
		GetModuleFileNameA((HMODULE)hModule, szModule, _MAX_PATH);
		OutputDebugStringA("-----> Terminating ");
		OutputDebugStringA(szModule);
		OutputDebugStringA(" ...\n");
		break;
	}
}

#else

inline void winx_call DllMainInit(
	HANDLE hModule, DWORD ul_reason_for_call, long nBreakAlloc = 0)
{
}

#endif

// -------------------------------------------------------------------------
// class ThreadCallerCheck

class ThreadCallerCheck
{
private:
	DWORD m_threadId;

public:
	ThreadCallerCheck() {
		m_threadId = GetCurrentThreadId();
	}

	bool winx_call isMe() const {
		return m_threadId == GetCurrentThreadId();
	}
};

#if defined(_DEBUG)

#define WINX_THREAD_CALLER_CHECK()											\
	NS_STDEXT::ThreadCallerCheck winx_threadCallerCheck_;					\
	NS_STDEXT::ThreadCallerCheck& winx_threadCallerCheck() {				\
		return winx_threadCallerCheck_;										\
	}

#define WINX_THREAD_CALLER_CHECK_S()										\
	NS_STDEXT::ThreadCallerCheck& winx_threadCallerCheck() {				\
		static NS_STDEXT::ThreadCallerCheck winx_threadCallerCheck_;		\
		return winx_threadCallerCheck_;										\
	}

#define WINX_CHECK_THREAD()													\
	WINX_ASSERT(winx_threadCallerCheck().isMe())

#else

#define WINX_THREAD_CALLER_CHECK()
#define WINX_THREAD_CALLER_CHECK_S()
#define WINX_CHECK_THREAD()

#endif

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_DEBUG_H */
