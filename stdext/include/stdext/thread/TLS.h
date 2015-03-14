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
// Module: stdext/thread/TLS.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: TLS.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_TLS_H
#define STDEXT_THREAD_TLS_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_THREAD_MUTEX_H
#include "Mutex.h"
#endif

#ifndef STDEXT_DETAIL_GLOBALVAR_H
#include "../detail/GlobalVar.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

#define WINX_DETAIL_GVAR_TLS_MUTEX_	detail::GlobalVar<Mutex, WINX_DETAIL_GIDX_TLS_MUTEX_>::g_var

// -------------------------------------------------------------------------
// class WinTlsKey

#if defined(STDEXT_THREAD_WINDOWS)

typedef DWORD TLSINDEX;

class WinTlsKey
{
private:
	TLSINDEX m_key;

public:
	WinTlsKey()
		: m_key(TLS_OUT_OF_INDEXES) {
	}
	
	bool winx_call good() const {
		return m_key != TLS_OUT_OF_INDEXES;
	}

	int winx_mtcall create() {
		Mutex::scoped_lock lock(WINX_DETAIL_GVAR_TLS_MUTEX_);
		if (m_key == TLS_OUT_OF_INDEXES)
			m_key = TlsAlloc();
		return 0;
	}

	void winx_call clear() {
		TlsFree(m_key);
		m_key = TLS_OUT_OF_INDEXES;
	}

	void winx_call put(void* p) const {
		TlsSetValue(m_key, p);
	}

	void* winx_call get() const {
		return TlsGetValue(m_key);
	}
};

#endif // defined(STDEXT_THREAD_WINDOWS)

// -------------------------------------------------------------------------
// class PthreadTlsKey

#if defined(STDEXT_HAS_PTHREAD)

class PthreadTlsKey
{
private:
	pthread_key_t m_key;

public:
	PthreadTlsKey()
		: m_key(TLS_OUT_OF_INDEXES) {
	}

	bool winx_call good() const {
		return m_key != TLS_OUT_OF_INDEXES;
	}

	int winx_mtcall create() {
		Mutex::scoped_lock lock(WINX_DETAIL_GVAR_TLS_MUTEX_);
		if (m_key == TLS_OUT_OF_INDEXES)
			pthread_key_create(&m_key, NULL);
		return 0;
	}

	void winx_call clear() {
		pthread_key_delete(m_key);
		m_key = TLS_OUT_OF_INDEXES;
	}

	void winx_call put(void* p) const {
		pthread_setspecific(m_key, p);
	}

	void* winx_call get() const {
		return pthread_getspecific(m_key);
	}
};

#endif // !defined(_WIN32)

// -------------------------------------------------------------------------
// class TlsKey

#if defined(STDEXT_THREAD_WINDOWS)

typedef WinTlsKey TlsKey;

#else

typedef PthreadTlsKey TlsKey;

#endif

// -------------------------------------------------------------------------
// class TlsPtr - removed, use boost::thread_specific_ptr instead of TlsPtr

#if defined(BOOST_THREAD_TSS_HPP)
#define TlsPtr thread_specific_ptr

using boost::thread_specific_ptr;

#endif

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_THREAD_TLS_H */
