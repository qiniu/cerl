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
// Module: stdext/thread/Mutex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Mutex.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_MUTEX_H
#define STDEXT_THREAD_MUTEX_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// class ScopedLock

template <class LockT>
class ScopedLock
{
private:
	LockT& m_lock;

private:
	ScopedLock(const ScopedLock&);
	void operator=(const ScopedLock&);

public:
	ScopedLock(LockT& lock) : m_lock(lock)
	{
		m_lock.acquire();
	}
	~ScopedLock()
	{
		m_lock.release();
	}
};

// =========================================================================
// class WinMutex

#if defined(STDEXT_THREAD_WINDOWS)

class WinMutex
{
private:
	CRITICAL_SECTION m_cs;
	
private:
	WinMutex(const WinMutex&);
	void operator=(const WinMutex&);

public:
	WinMutex()
	{
		InitializeCriticalSection(&m_cs);
	}
	~WinMutex()
	{
		DeleteCriticalSection(&m_cs);
	}

	void winx_call acquire()
	{
		EnterCriticalSection(&m_cs);
	}

	void winx_call release()
	{
		LeaveCriticalSection(&m_cs);
	}
	
	operator CRITICAL_SECTION&()
	{
		return m_cs;
	}

#if defined(_DEBUG)
	bool winx_call good() const // debug only
	{
		const char* p = (const char*)&m_cs;
		for (size_t i = 0; i < sizeof(m_cs); ++i)
		{
			if (p[i] != 0)
				return true;
		}
		return false;
	}
#endif

public:
	typedef ScopedLock<WinMutex> scoped_lock;
};

#endif // defined(STDEXT_THREAD_WINDOWS)

// =========================================================================
// class PthreadMutex

#if defined(STDEXT_HAS_PTHREAD)

class PthreadMutex
{
private:
	PthreadMutex(const PthreadMutex&);
	void operator=(const PthreadMutex&);

private:
	pthread_mutex_t m_cs;

public:
	PthreadMutex() {
		pthread_mutex_init(&m_cs, NULL);
	}
	~PthreadMutex() {
		pthread_mutex_destroy(&m_cs);
	}

	void winx_call acquire() {
		pthread_mutex_lock(&m_cs);
	}

	void winx_call release() {
		pthread_mutex_unlock(&m_cs);
	}

	operator pthread_mutex_t&() {
		return m_cs;
	}

#if defined(_DEBUG)
	bool winx_call good() const // debug only
	{
		return true;
	}
#endif

public:
	typedef ScopedLock<PthreadMutex> scoped_lock;
};

#endif

// =========================================================================
// class Mutex

#if defined(STDEXT_THREAD_WINDOWS)

typedef WinMutex Mutex;

#else

typedef PthreadMutex Mutex;

#endif

// =========================================================================
// class FakeMutex

class FakeMutex
{
public:
	void winx_call acquire() {}
	void winx_call release() {}

#if defined(_DEBUG)
	bool winx_call good() const {
		return true;
	}
#endif

public:
	class scoped_lock
	{
	public:
		scoped_lock(FakeMutex& cs) {
		}
	};
};

// =========================================================================
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_THREAD_MUTEX_H */
