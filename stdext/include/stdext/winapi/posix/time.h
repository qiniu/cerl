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
// Module: stdext/winapi/posix/time.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: time.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_POSIX_TIME_H
#define STDEXT_WINAPI_POSIX_TIME_H

#ifndef STDEXT_WINAPI_WINDEF_H
#include "../windef.h"
#endif

// -------------------------------------------------------------------------
// int clock_getres(clockid_t clk_id, struct timespec *res);
// int clock_gettime(clockid_t clk_id, struct timespec *tp);
// int clock_settime(clockid_t clk_id, const struct timespec *tp);

#ifndef _TIME_H
#include <time.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

// See http://linux.die.net/man/3/clock_gettime
// On POSIX systems on which these functions are available, the symbol _POSIX_TIMERS
// is defined in <unistd.h> to a value greater than 0.
// The symbols _POSIX_MONOTONIC_CLOCK, _POSIX_CPUTIME, _POSIX_THREAD_CPUTIME indicate 
// that CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID are available.
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0) && defined(_POSIX_MONOTONIC_CLOCK)
#define STDEXT_HAS_MONOTONIC_CLOCK
#endif

// -------------------------------------------------------------------------

#if defined(STDEXT_HAS_MONOTONIC_CLOCK)

__forceinline BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lp)
{
	lp->QuadPart = 1000000000;
	return TRUE;
}

__forceinline BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lp)
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    UINT64 ticks = (UINT64)now.tv_sec * 1000000000 + now.tv_nsec;
    lp->QuadPart = ticks;
    return TRUE;
}

__forceinline DWORD WINAPI GetTickCount()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (long)now.tv_sec * 1000 + (now.tv_nsec / 1000000);
}

#endif

// -------------------------------------------------------------------------

__forceinline VOID WINAPI Sleep(DWORD dwMilliseconds)
{
	/** The usleep() function suspends execution of the calling process for (at least)
	    usec microseconds. The sleep may be lengthened slightly by any system activity
		or by the time spent processing the call or by the granularity of system timers.
	*/
	usleep(dwMilliseconds * 1000);
}

__forceinline VOID WINAPI SleepEx(DWORD dwMilliseconds, BOOL bAlertable)
{
	usleep(dwMilliseconds * 1000);
}

// -------------------------------------------------------------------------

#endif /* STDEXT_WINAPI_POSIX_TIME_H */
