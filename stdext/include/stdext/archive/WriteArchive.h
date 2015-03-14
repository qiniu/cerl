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
// Module: stdext/archive/WriteArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: WriteArchive.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_WRITEARCHIVE_H
#define STDEXT_ARCHIVE_WRITEARCHIVE_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_RANGE_H
#include "../text/Range.h"
#endif

#ifndef STDEXT_ARCHIVE_TEXT_H
#include "Text.h"
#endif

#ifndef STDEXT_ARCHIVE_BINARY_H
#include "Binary.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class WriteArchive

template <class Handle, class StreamHandle, class CacheT = ArchiveCache>
class WriteArchive
{
private:
	typedef typename CacheT::allocator_type AllocT;

private:
	WriteArchive(const WriteArchive&);
	void operator=(const WriteArchive&);

public:
	enum { cacheSize = CacheT::cacheSize };
	enum { roundSize = CacheT::roundSize };

public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef typename CacheT::allocator_type allocator_type;

	typedef typename StreamHandle::int_type	int_type;
	typedef typename StreamHandle::char_type char_type;
	typedef typename StreamHandle::uchar_type uchar_type;

	typedef typename StreamHandle::pos_type	pos_type;
	typedef typename StreamHandle::off_type	off_type;
	
protected:
	char_type*	m_lpBufStart;
	size_type	m_nBufSize;
	char_type*	m_lpBufCur;
	char_type*	m_lpBufMax;

	StreamHandle m_handle;
	AllocT m_alloc;
	
public:
	explicit WriteArchive(AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufStart;
		m_lpBufMax	 = m_lpBufStart + m_nBufSize;
		// m_lpBufCur - m_lpBufStart为已经写入数据的缓存！
	}

	explicit WriteArchive(Handle hFile, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufStart;
		m_lpBufMax	 = m_lpBufStart + m_nBufSize;
		m_handle.open_handle(hFile);
	}

	explicit WriteArchive(LPCWSTR szFile, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufStart;
		m_lpBufMax	 = m_lpBufStart + m_nBufSize;
		m_handle.open_to_write(szFile);
	}

	explicit WriteArchive(LPCSTR szFile, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufStart;
		m_lpBufMax	 = m_lpBufStart + m_nBufSize;
		m_handle.open_to_write(szFile);
	}

	~WriteArchive() throw(IoException)
	{
		if (m_lpBufCur != m_lpBufStart)
		{
			m_handle.put(m_lpBufStart, (size_type)(m_lpBufCur - m_lpBufStart));
		}
		m_alloc.deallocate(m_lpBufStart, m_nBufSize);
	}

	void winx_call clear_cache() throw(IoException)
	{
		if (m_lpBufCur != m_lpBufStart)
		{
			//@@bugfix: 此流程会引发异常。做出的修改的目的是避免再次引发异常。
			//2005-3-23
			char_type* lpBufCur = m_lpBufCur;
			m_lpBufCur = m_lpBufStart;
			m_handle.put(m_lpBufStart, (size_type)(lpBufCur - m_lpBufStart));
		}
	}

	void winx_call flush() throw(IoException)
	{
		clear_cache();
		m_handle.flush();
	}

	pos_type winx_call tell() const
	{
		return m_handle.tell() + (difference_type)(m_lpBufCur - m_lpBufStart);
	}

	void winx_call seek(pos_type offset) throw(IoException)
	{
		clear_cache();
		m_handle.seek(static_cast<off_type>(offset), SEEK_SET);
		m_lpBufCur = m_lpBufStart;
	}

	void winx_call seek_end() throw(IoException)
	{
		clear_cache();
		m_handle.seek(0, SEEK_END);
		m_lpBufCur = m_lpBufStart;
	}
	
public:
	void winx_call reput(size_type offset, const char_type* lpBuf, size_type cch) throw(IoException)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);
		
		if (offset <= (size_type)(m_lpBufCur - m_lpBufStart))
		{
			copyMemory(m_lpBufCur - offset, lpBuf, cch);
		}
		else
		{
			pos_type curpos = tell();
			seek(curpos - offset);
			put(lpBuf, cch);
			seek(curpos);
		}
	}

	size_type winx_call put(const char_type* const lpBuf, const size_type cch) throw(IoException)
	{
		WINX_ASSERT((lpBuf || cch == 0) && m_lpBufStart && m_lpBufCur);
		
		// 1. 如果缓存足够
		const size_type cbUnused = (size_type)(m_lpBufMax - m_lpBufCur);
		if (cch <= cbUnused)
		{
			copyMemory(m_lpBufCur, lpBuf, cch);
			m_lpBufCur += cch;
		}
		else
		{
			// 2.1 先将目标数据Copy到缓存，直到填满为止
			copyMemory(m_lpBufCur, lpBuf, cbUnused);
			m_handle.put(m_lpBufStart, m_nBufSize); // IO操作最好按磁盘的块边界对齐
			
			// 2.2 写满各整页
			const size_type cchRest = cch - cbUnused;
			const size_type cchMod = cchRest % m_nBufSize;
			const size_type nWritten = cchRest - cchMod;
			if (nWritten)
				m_handle.put(lpBuf + cbUnused, nWritten);
			
			// 2.3 写最后一页
			copyMemory(m_lpBufStart, lpBuf + cbUnused + nWritten, cchMod);
			m_lpBufCur = m_lpBufStart + cchMod;
		}
		return cch;
	}

	size_type winx_call put(char_type ch) throw(IoException)
	{
		if (m_lpBufCur < m_lpBufMax)
		{
			*m_lpBufCur++ = ch;
		}
		else
		{
			m_handle.put(m_lpBufStart, m_nBufSize); // 写操作最好按磁盘的块边界对齐
			*m_lpBufStart = ch;
			m_lpBufCur = m_lpBufStart + 1;
		}
		return 1;
	}

	size_type winx_call put(const BasicString<char_type>& s) throw(IoException)
	{
		return put(s.data(), s.size());
	}

public:
	HRESULT winx_call open(LPCWSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_write(szFile);
	}

	HRESULT winx_call open(LPCSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_write(szFile);
	}
	
	HRESULT winx_call open_to_append(LPCWSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_append(szFile);
	}

	HRESULT winx_call open_to_append(LPCSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_append(szFile);
	}

	void winx_call open_handle(Handle hFile)
	{
		if (!m_handle.good())
			m_handle.open_handle(hFile);
	}

	void winx_call close() throw(IoException)
	{
		clear_cache();
		m_handle.close();
	}

	void winx_call attach(Handle hFile) throw(IoException)
	{
		clear_cache();
		m_handle.attach(hFile);
	}

	Handle winx_call detach() throw(IoException)
	{
		clear_cache();
		return m_handle.detach();
	}

public:
	bool winx_call operator!() const
	{
		return m_handle.bad();
	}

	bool winx_call good() const
	{
		return m_handle.good();
	}

	bool winx_call bad() const
	{
		return m_handle.bad();
	}

public:
	template <class DataIt, class SepT>
	WriteArchive& winx_call printLines(
		DataIt first, DataIt last, const SepT& sep) throw(IoException)
	{
		NS_STDEXT_IO_TEXT::printLines(*this, first, last, sep);
		return *this;
	}

	template <class DataIt>
	WriteArchive& winx_call printLines(
		DataIt first, DataIt last) throw(IoException)
	{
		NS_STDEXT_IO_TEXT::printLines(*this, first, last);
		return *this;
	}

	template <class ContainerT>
	WriteArchive& winx_call printLines(
		const ContainerT& rg) throw(IoException)
	{
		NS_STDEXT_IO_TEXT::printLines(*this, rg.begin(), rg.end());
		return *this;
	}

	template <class ContainerT, class SepT>
	WriteArchive& winx_call printLines(
		const ContainerT& rg, const SepT& sep) throw(IoException)
	{
		NS_STDEXT_IO_TEXT::printLines(*this, rg.begin(), rg.end(), sep);
		return *this;
	}

public:
	size_t winx_call write(const void* buf, size_t cb) throw(IoException) {
		return put((const char*)buf, cb);
	}

	void winx_call puts(const char* lpBuf, size_t cch) throw(IoException) {
		NS_STDEXT_IO_BINARY::puts(*this, lpBuf, cch);
	}

	void winx_call puts(const String& s) throw(IoException) {
		NS_STDEXT_IO_BINARY::puts(*this, s);
	}

	void winx_call wputs(const UINT16* s, size_t cch) throw(IoException) {
		NS_STDEXT_IO_BINARY::wputs(*this, s, cch);
	}

	void winx_call wputs(const BasicString<UINT16>& s) throw(IoException) {
		NS_STDEXT_IO_BINARY::wputs(*this, s);
	}
};

// -------------------------------------------------------------------------
// $Log: WriteArchive.h,v $

NS_STDEXT_END

#endif /* STDEXT_ARCHIVE_WRITEARCHIVE_H */
