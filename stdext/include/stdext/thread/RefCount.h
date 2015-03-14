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
// Module: stdext/thread/RefCount.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: RefCount.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREAD_REFCOUNT_H
#define STDEXT_THREAD_REFCOUNT_H

#ifndef STDEXT_THREAD_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// class RefCountMT

class RefCountMT
{
public:
	typedef LONG value_type;

private:
	value_type m_nRef;

public:
	RefCountMT(value_type nRef)
		: m_nRef(nRef)
	{
	}

	value_type winx_call acquire()
	{
		return InterlockedIncrement(&m_nRef);
	}

	value_type winx_call release()
	{
		return InterlockedDecrement(&m_nRef);
	}

	operator value_type()
	{
		return m_nRef;
	}
};

// =========================================================================
// class RefCountST

class RefCountST
{
public:
	typedef long value_type;

private:
	value_type m_nRef;

public:
	RefCountST(value_type nRef) : m_nRef(nRef)
	{
	}

	value_type winx_call acquire() { return ++m_nRef; }
	value_type winx_call release() { return --m_nRef; }

	operator value_type()
	{
		return m_nRef;
	}
};

// =========================================================================
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_THREAD_REFCOUNT_H */
