/* ------------------------------------------------------------------------
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
// Module: stdext/Storage.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 09:48:40
// 
// $Id: Storage.h,v 1.4 2006/12/22 10:17:16 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_STORAGE_H
#define STDEXT_STORAGE_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_ARRAY_H
#include "Array.h"
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class StorageBase

class StorageBase
{
public:
	operator bool() const { return TRUE; }

	static int winx_call good()	{ return TRUE; }
	static int winx_call select(int mask) { return 0; }
	static void winx_call flush() {}
};

// -------------------------------------------------------------------------
// interface ILogStorage

interface ILogStorage
{
	virtual void winx_call put(int ch) = 0;
	virtual void winx_call putw(wint_t wch) = 0;

	virtual void winx_call put(size_t count, int ch) = 0;
	virtual void winx_call putw(size_t count, wint_t wch) = 0;

	virtual void winx_call put(const char* s, size_t count) = 0;
	virtual void winx_call put(const WCHAR* s, size_t count) = 0;

	virtual void winx_call putv(const char* fmt, va_list args) = 0;
	virtual void winx_call putv(const WCHAR* fmt, va_list args) = 0;

	virtual void winx_call flush() {}
};

// -------------------------------------------------------------------------
// class FILEStorage

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

template <bool bOwn = true, class Base = ILogStorage>
class FILEStorageT : public Base
{
protected:
	FILE* m_fp;
	
public:
	typedef char char_type;

public:
	FILEStorageT() : m_fp(NULL) {}
	FILEStorageT(FILE* fp) : m_fp(fp) {}
	FILEStorageT(LPCSTR szFile, bool bCheckOwn = true) {
		WINX_ASSERT(bOwn || !bCheckOwn);
		m_fp = fopen(szFile, "w");
	}

	template <class LogT>
	FILEStorageT(LogT& log) : m_fp(log.storage().m_fp) {
	}

	~FILEStorageT() {
		if (bOwn)
			close();
	}

	int winx_call good() const
	{
		return m_fp != NULL;
	}

	void winx_call open(LPCSTR szFile, bool bCheckOwn = true)
	{
		WINX_ASSERT(bOwn || !bCheckOwn);
		WINX_ASSERT(m_fp == NULL);
		m_fp = fopen(szFile, "w");
	}
	
	void winx_call close()
	{
		if (m_fp) {
			fclose(m_fp);
			m_fp = NULL;
		}
	}

	void winx_call flush()
	{
		if (m_fp)
			fflush(m_fp);
	}
	
	void winx_call assign(FILE* fp)	{ m_fp = fp; }

public:
	void winx_call put(int ch)		{ putc(ch, m_fp); }
	void winx_call putw(wint_t wch)	{ putwc(wch, m_fp); }
	
	void winx_call put(size_t count, int ch)
	{
		while (count--)
			putc(ch, m_fp);
	}
	
	void winx_call putw(size_t count, wint_t wch)
	{
		while (count--)
			putwc(wch, m_fp);
	}
	
	void winx_call put(const char* s, size_t count)	 { fwrite(s, count, sizeof(char), m_fp); }
	void winx_call put(const WCHAR* s, size_t count) { fwrite(s, count, sizeof(WCHAR), m_fp); }
	
	void winx_call putv(const char* fmt, va_list args)  { vfprintf(m_fp, fmt, args); }
	void winx_call putv(const WCHAR* fmt, va_list args)	{ vfwprintf(m_fp, fmt, args); }
};

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

typedef FILEStorageT<false, StorageBase> FILEStorage;

// -------------------------------------------------------------------------
// class StringStorage

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

template <class StringT, class Base = StorageBase>
class StringStorage : public StringT, public Base
{
public:
	typedef typename StringT::value_type char_type;
	typedef StringT storage_type;

public:
	void winx_call put(int ch)
	{
		StringT::append(1, ch);
	}
	
	void winx_call putw(wint_t wch)
	{
		StringT::append(1, wch);
	}
	
	void winx_call put(size_t count, int ch)
	{
		StringT::append(count, ch);
	}
	
	void winx_call putw(size_t count, wint_t wch)
	{
		StringT::append(count, wch);
	}
	
	void winx_call put(const char* s, size_t count)
	{
		StringT::append(s, count);
	}
	
	void winx_call put(const WCHAR* s, size_t count)
	{
		StringT::append(s, count);
	}

private:
	enum { __STD_LOG_BUFSIZE = 1024 };
	
public:
	void winx_call putv(const char* fmt, va_list args)
	{
		char buf[__STD_LOG_BUFSIZE];
		int cch = _vsnprintf(buf, __STD_LOG_BUFSIZE, fmt, args);
		if (cch < 0)
			cch = __STD_LOG_BUFSIZE;
		StringT::append(buf, cch);
	}
	
	void winx_call putv(const WCHAR* fmt, va_list args)	{
		WCHAR buf[__STD_LOG_BUFSIZE];
		int cch = _vsnwprintf(buf, __STD_LOG_BUFSIZE, fmt, args);
		if (cch < 0)
			cch = __STD_LOG_BUFSIZE;
		StringT::append(buf, cch);
	}
};

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

// -------------------------------------------------------------------------
// class MultiStorage

template <UINT nStorage = 0, class StorageContainer = Array<ILogStorage*, nStorage> >
class MultiStorage : public StorageBase
{
private:
	StorageContainer m_stgs;
	int m_enables;

public:
	typedef char char_type;

public:
	MultiStorage() : m_enables(-1) {}

	void winx_call clear()
	{
		m_stgs.clear();
	}

	void winx_call add(ILogStorage& stg)
	{
		m_stgs.push_back(&stg);
	}

	int winx_call select(int enables)
	{
		int old = m_enables;
		m_enables = enables;
		return old;
	}

	int winx_call enabled(size_t i)
	{
		return m_enables & (1 << i);
	}

	void winx_call flush() {
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->flush();
	}
	
	void winx_call put(int ch)	{
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->put(ch);
	}

	void winx_call putw(wint_t wch)	{
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->putw(wch);
	}
	
	void winx_call put(size_t count, int ch) {
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->put(count, ch);
	}
	
	void winx_call putw(size_t count, wint_t wch) {
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->putw(count, wch);
	}
	
	void winx_call put(const char* s, size_t count)	 {
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->put(s, count);
	}

	void winx_call put(const WCHAR* s, size_t count) {
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->put(s, count);
	}
	
	void winx_call putv(const char* fmt, va_list args)  {
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->putv(fmt, args);
	}

	void winx_call putv(const WCHAR* fmt, va_list args)	{
		for (size_t i = 0; i < m_stgs.size(); ++i)
			if (enabled(i))
				m_stgs[i]->putv(fmt, args);
	}
};

// -------------------------------------------------------------------------
// $Log: Storage.h,v $

NS_STDEXT_END

#endif /* STDEXT_STORAGE_H */
