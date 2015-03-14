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
// Module: stdext/thread/Thread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Thread.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_THREAD_H
#define STDEXT_THREAD_THREAD_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

// =========================================================================

#if defined(STDEXT_THREAD_WINDOWS)

typedef HANDLE HTHREAD;
typedef DWORD THREADRET;

inline HTHREAD WINAPI CreateThread(
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter = NULL,
    size_t dwStackSize = 0)
{
    return CreateThread(NULL, dwStackSize, lpStartAddress, lpParameter, 0, NULL);
}

inline BOOL WINAPI CloseThread(HTHREAD hThread)
{
    return CloseHandle(hThread);
}

inline void WINAPI WaitThread(HTHREAD hThread)
{
	WaitForSingleObject(hThread, INFINITE);
};

inline void WINAPI WaitThread(HTHREAD hThread, THREADRET* exitCode)
{
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, exitCode);
};

// =========================================================================

#elif defined(STDEXT_HAS_PTHREAD)

typedef pthread_t HTHREAD;
typedef void* THREADRET;

typedef THREADRET (WINAPI *PTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

inline HTHREAD WINAPI CreateThread(
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter = NULL,
    size_t dwStackSize = 0)
{
	HTHREAD hThread;
	const int nRet = pthread_create(&hThread, NULL, lpStartAddress, lpParameter);
	return (nRet == 0 ? hThread : NULL);
}

inline void WINAPI ExitThread(THREADRET exitCode)
{
	pthread_exit(exitCode);
}

inline BOOL WINAPI SwitchToThread()
{
	return TRUE;
}

inline BOOL WINAPI CloseThread(HTHREAD hThread)
{
	return TRUE;
}

inline void WINAPI WaitThread(HTHREAD hThread)
{
	pthread_join(hThread, NULL);
};

inline void WINAPI WaitThread(HTHREAD hThread, THREADRET* exitCode)
{
	pthread_join(hThread, exitCode);
};

#endif

// =========================================================================
// class Thread

NS_STDEXT_BEGIN

class Thread
{
public:
	HTHREAD hThread;

public:
	Thread(
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter = NULL,
		size_t dwStackSize = 0)
	{
		hThread = CreateThread(lpStartAddress, lpParameter, dwStackSize);
	}

	~Thread()
	{
		CloseThread(hThread);
	}

	void winx_call join() const // = boost::thread::join
	{
		WaitThread(hThread);
	}

	void winx_call wait() const
	{
		WaitThread(hThread);
	}
};

NS_STDEXT_END

// =========================================================================

#endif /* STDEXT_THREAD_THREAD_H */
