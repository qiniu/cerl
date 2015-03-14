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
// Module: stdext/time/timespec.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: timespec.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TIME_TIMESPEC_H
#define STDEXT_TIME_TIMESPEC_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#if defined(X_OS_WINDOWS)
#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC 1
struct timespec {
	long tv_sec;
	long tv_nsec;
};
#endif /* HAVE_STRUCT_TIMESPEC */
#endif

NS_STDEXT_BEGIN

// =========================================================================
// function ptw32_filetime_to_timespec_
// see http://winx.googlecode.com/svn/tags/pthreads-w32-2-8-0/ptw32_timespec.c

#if defined(_WIN32) || defined(_WIN64)

/*
* time between jan 1, 1601 and jan 1, 1970 in units of 100 nanoseconds
*/
#define STDEXT_TIMESPEC_TO_FILETIME_OFFSET_ \
	( ((LONGLONG) 27111902 << 32) + (LONGLONG) 3577643008 )

/*
* -------------------------------------------------------------------
* converts FILETIME (as set by GetSystemTimeAsFileTime), where the time is
* expressed in 100 nanoseconds from Jan 1, 1601,
* into struct timespec
* where the time is expressed in seconds and nanoseconds from Jan 1, 1970.
* -------------------------------------------------------------------
*/
inline void winx_call ptw32_filetime_to_timespec_(const FILETIME* ft, struct timespec* ts)
{
	ts->tv_sec =
		(int) ((*(LONGLONG *) ft - STDEXT_TIMESPEC_TO_FILETIME_OFFSET_) / 10000000);
	ts->tv_nsec =
		(int) ((*(LONGLONG *) ft - STDEXT_TIMESPEC_TO_FILETIME_OFFSET_ -
		((LONGLONG) ts->tv_sec * (LONGLONG) 10000000)) * 100);
}

#endif

// =========================================================================
// is_valid_time

inline bool winx_call is_valid_time(const timespec& t)
{
	return t.tv_nsec < 1000000000L; // 1 billion
}

// =========================================================================
// get_current_time

#if defined(_WIN32) || defined(_WIN64)

inline void winx_call get_current_time(timespec& t)
{
	FILETIME curr;
	GetSystemTimeAsFileTime(&curr);
	ptw32_filetime_to_timespec_(&curr, &t);
}

#else

inline void winx_call get_current_time(timespec& t)
{
	clock_gettime(CLOCK_REALTIME, &t);
}

#endif

// =========================================================================
// init_duration_in_ms

inline void winx_call init_duration_in_ms(timespec& dur, const long ms)
{
	const ldiv_t ld = ldiv(ms, 1000);
	dur.tv_sec = ld.quot;
	dur.tv_nsec = ld.rem * 1000000;
}

// =========================================================================
// add_time_duration

inline void winx_call add_time_duration(timespec& t, const timespec& dur)
{
	WINX_ASSERT(is_valid_time(t));
	WINX_ASSERT(is_valid_time(t));

	const ldiv_t ld = ldiv(t.tv_nsec + dur.tv_nsec, 1000000000L);
	t.tv_sec += dur.tv_sec + ld.quot;
	t.tv_nsec = ld.rem;
}

// =========================================================================
// get_until_time

inline void winx_call get_until_time(timespec& t, const long ms) // t = current_time + ms
{
	timespec dur;
	init_duration_in_ms(dur, ms);
	get_current_time(t);
	add_time_duration(t, dur);
}

// =========================================================================

NS_STDEXT_END

#endif /* STDEXT_TIME_TIMESPEC_H */
