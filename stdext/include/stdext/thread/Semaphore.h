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
// Module: stdext/thread/Semaphore.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Semaphore.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_SEMAPHORE_H
#define STDEXT_THREAD_SEMAPHORE_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

#if defined(X_OS_LINUX)
#define STDEXT_POSIX_SEMAPHORE
#ifndef _SEMAPHORE_H
#include <semaphore.h>
#endif
#endif

NS_STDEXT_BEGIN

// =========================================================================
// class WinSemaphore

#if defined(STDEXT_THREAD_WINDOWS)

class WinSemaphore
{
private:
	HANDLE m_sem;
	
private:
	WinSemaphore(const WinSemaphore&);
	void operator=(const WinSemaphore&);

public:
	typedef LONG value_type;

public:
	explicit WinSemaphore(value_type lInitialCount = 0)
	{
		enum { MaxCount = 2147483647L };
		m_sem = CreateSemaphore(NULL, lInitialCount, MaxCount, NULL);
	}
	~WinSemaphore()
	{
		CloseHandle(m_sem);
	}

	void winx_call acquire()
	{
		WaitForSingleObject(m_sem, INFINITE);
	}

	void winx_call release()
	{
		ReleaseSemaphore(m_sem, 1, NULL);
	}
};

#endif // defined(STDEXT_THREAD_WINDOWS)

// =========================================================================
// class PosixSemaphore

#if defined(STDEXT_POSIX_SEMAPHORE)

class PosixSemaphore
{
private:
	PosixSemaphore(const PosixSemaphore&);
	void operator=(const PosixSemaphore&);

private:
	sem_t m_sem;

public:
	typedef unsigned value_type;

public:
	explicit PosixSemaphore(value_type lInitialCount = 0) {
		sem_init(&m_sem, 0, lInitialCount);
	}
	~PosixSemaphore() {
		sem_destroy(&m_sem);
	}

	void winx_call acquire() {
		sem_wait(&m_sem);
	}

	void winx_call release() {
		sem_post(&m_sem);
	}
};

#endif

// =========================================================================
// class Semaphore

#if defined(STDEXT_THREAD_WINDOWS)

typedef WinSemaphore Semaphore;

#else

typedef PosixSemaphore Semaphore;

#endif

// =========================================================================

NS_STDEXT_END

#endif /* STDEXT_THREAD_SEMAPHORE_H */
