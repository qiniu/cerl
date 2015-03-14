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
// Module: stdext/winapi/posix/pthread.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: pthread.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_POSIX_PTHREAD_H
#define STDEXT_WINAPI_POSIX_PTHREAD_H

#ifndef STDEXT_WINAPI_WINDEF_H
#include "../windef.h"
#endif

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

// -------------------------------------------------------------------------
// CriticalSection

typedef pthread_mutex_t RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION PCRITICAL_SECTION;
typedef PRTL_CRITICAL_SECTION LPCRITICAL_SECTION;

__forceinline VOID WINAPI InitializeCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_init(lpCriticalSection, NULL);
}

__forceinline VOID WINAPI EnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_lock(lpCriticalSection);
}

__forceinline BOOL WINAPI TryEnterCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	return pthread_mutex_trylock(lpCriticalSection) == 0;
}

__forceinline VOID WINAPI LeaveCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_unlock(lpCriticalSection);
}

__forceinline VOID WINAPI DeleteCriticalSection(
    LPCRITICAL_SECTION lpCriticalSection)
{
	pthread_mutex_destroy(lpCriticalSection);
}

// -------------------------------------------------------------------------

__forceinline DWORD WINAPI GetCurrentThreadId()
{
#if defined(PTW32_VERSION)
	return (DWORD)pthread_self().p;
#else
	return pthread_self();
#endif
}

// -------------------------------------------------------------------------

#ifndef TLS_OUT_OF_INDEXES
#define TLS_OUT_OF_INDEXES (DWORD)0xFFFFFFFF
#endif

#ifndef S_OK
#define S_OK 0
#endif

typedef pthread_key_t TLSINDEX;

__forceinline TLSINDEX WINAPI TlsAlloc(void)
{
	pthread_key_t key;
	if (pthread_key_create(&key, NULL) != S_OK)
		return TLS_OUT_OF_INDEXES;
	else
		return key;
}

__forceinline BOOL WINAPI TlsFree(TLSINDEX key)
{
	return pthread_key_delete(key) == S_OK;
}

__forceinline BOOL WINAPI TlsSetValue(TLSINDEX key, LPVOID lpTlsValue)
{
	return pthread_setspecific(key, lpTlsValue) == S_OK;
}

__forceinline LPVOID WINAPI TlsGetValue(TLSINDEX key)
{
	return pthread_getspecific(key);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_WINAPI_POSIX_PTHREAD_H */
