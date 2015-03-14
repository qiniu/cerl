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
// Module: stdext/thread/Condition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: Condition.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_CONDITION_H
#define STDEXT_THREAD_CONDITION_H

#ifndef STDEXT_THREAD_MUTEX_H
#include "Mutex.h"
#endif

#ifndef STDEXT_TIME_TIMESPEC_H
#include "../time/timespec.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// class WinCondition

#if defined(STDEXT_THREAD_WINDOWS)

class WinCondition
{
public:
	typedef WinMutex mutex_type;
	typedef size_t duration_type;

private:
	typedef CRITICAL_SECTION& MutexRef;

private:
	WinCondition(const WinCondition&);
	void operator=(const WinCondition&);

public:
	void cerl_call notify_all();

public:
	void cerl_call wait(MutexRef cs);
	bool cerl_call timed_wait(MutexRef cs, duration_type ms);
};

#endif // defined(STDEXT_THREAD_WINDOWS)

// =========================================================================
// class PthreadCondition

#if defined(STDEXT_HAS_PTHREAD)

class PthreadCondition
{
private:
	typedef pthread_mutex_t& MutexRef;
	typedef struct timespec TimeSpec;

private:
	PthreadCondition(const PthreadCondition&);
	void operator=(const PthreadCondition&);

public:
	typedef PthreadMutex mutex_type;
	typedef long duration_type;

private:
	pthread_cond_t m_cond;

public:
	PthreadCondition() {
		pthread_cond_init(&m_cond, NULL);
	}
	~PthreadCondition() {
		pthread_cond_destroy(&m_cond);
	}

public:
	void cerl_call notify_one() {
		pthread_cond_signal(&m_cond);
	}

	void cerl_call notify_all() {
		pthread_cond_broadcast(&m_cond);
	}

public:
	void cerl_call wait(MutexRef cs) {
		pthread_cond_wait(&m_cond, &cs);
	}

	bool cerl_call timed_wait(MutexRef cs, const TimeSpec& abstime) {
		return pthread_cond_timedwait(&m_cond, &cs, &abstime) == 0; // != ETIMEDOUT;
	}

	bool cerl_call timed_wait(MutexRef cs, duration_type ms)
	{
		timespec abstime;
		get_until_time(abstime, ms);
		return timed_wait(cs, abstime);
	}
};

#endif

// =========================================================================
// class Condition

#if defined(STDEXT_THREAD_WINDOWS)

typedef WinCondition Condition;

#else

typedef PthreadCondition Condition;

#endif

// =========================================================================

NS_STDEXT_END

#endif /* STDEXT_THREAD_CONDITION_H */
