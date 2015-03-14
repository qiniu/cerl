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
// Module: stdext/archive/MemArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: MemArchive.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_MEMARCHIVE_H
#define STDEXT_ARCHIVE_MEMARCHIVE_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_TEXT_H
#include "Text.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class MemWriteArchive

// BaseStg = std::vector, std::deque or std::basic_string
template <class BaseStg>
class MemWriteArchive
{
private:
	MemWriteArchive(const MemWriteArchive&);
	void operator=(const MemWriteArchive&);

public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename BaseStg::value_type char_type;
	typedef typename ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;

	typedef size_type pos_type;
	typedef difference_type off_type;
	
protected:
	typedef typename BaseStg::iterator InnerPosT;

	BaseStg* m_stg;
	InnerPosT m_pos;

public:
	MemWriteArchive()
		: m_stg(NULL)
	{
	}

	explicit MemWriteArchive(BaseStg& stg)
		: m_stg(&stg)
	{
		m_pos = stg.begin();
	}

	void winx_call clear_cache()
	{
	}

	void winx_call flush()
	{
	}

	pos_type winx_call tell() const
	{
		return m_pos - m_stg->begin();
	}

	void winx_call seek(pos_type offset)
	{
		if (offset > size())
			m_stg->resize(offset);
		m_pos = m_stg->begin() + offset;
	}

	void winx_call seek_end()
	{
		m_pos = m_stg->end();
	}
	
public:
	void winx_call reput(size_type offset, const char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset > 0 && offset >= cch);
		
		std::copy(lpBuf, lpBuf + cch, m_pos - offset);
	}

	size_type winx_call put(const char_type* lpBuf, const size_type cch)
	{
		const InnerPosT posEnd = m_stg->end();
		const size_type rest = posEnd - m_pos;
		if (rest == 0) {
			m_stg->insert(posEnd, lpBuf, lpBuf + cch);
		}
		else if (rest < cch) {
			std::copy(lpBuf, lpBuf + rest, m_pos);
			m_stg->insert(posEnd, lpBuf + rest, lpBuf + cch);
		}
		else {
			std::copy(lpBuf, lpBuf + cch, m_pos);
			m_pos += cch;
			return cch;
		}
		m_pos = m_stg->end();
		return cch;
	}

	size_type winx_call put(char_type ch)
	{
		const InnerPosT posEnd = m_stg->end();
		if (m_pos != posEnd) {
			*m_pos++ = ch;
		}
		else {
			m_stg->insert(posEnd, ch);
			m_pos = m_stg->end();
		}
		return 1;
	}

	size_type winx_call put(const BasicString<char_type> s) throw(IoException)
	{
		return put(s.data(), s.size());
	}

public:
	HRESULT winx_call open(BaseStg& stg)
	{
		if (good())
			return E_ACCESSDENIED;
		m_stg = &stg;
		m_pos = stg.begin();
		return S_OK;
	}

	void winx_call close()
	{
		m_stg = NULL;
	}

public:
	pos_type winx_call size() const
	{
		WINX_ASSERT(m_stg);
		return m_stg->size();
	}

	bool winx_call operator!() const
	{
		return m_stg == NULL;
	}

	bool winx_call good() const
	{
		return m_stg != NULL;
	}

	bool winx_call bad() const
	{
		return m_stg == NULL;
	}
};

// -------------------------------------------------------------------------
// class MemReadArchive

template <class Iterator>
class MemReadArchive
{
public:
	typedef typename ArchiveIteratorTraits<Iterator>::char_type char_type;

private:
	typedef ArchiveCharTraits<char_type> _Tr;

public:
	enum { endch = _Tr::endch };

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename _Tr::int_type int_type;
	typedef typename _Tr::uchar_type uchar_type;

	typedef size_type pos_type;
	typedef difference_type off_type;
	typedef Iterator iterator;
	typedef Iterator const_iterator;

protected:
	Iterator m_pos;
	Iterator m_first;
	Iterator m_last;

private:
	MemReadArchive(const MemReadArchive&);
	void operator=(const MemReadArchive&);
	
public:
	MemReadArchive(Iterator first, Iterator last)
		: m_pos(first), m_first(m_pos), m_last(last) {
	}

	explicit MemReadArchive(Iterator first)
		: m_pos(first), m_first(first), m_last(std::end(first)) {
	}

	explicit MemReadArchive(char_type* first)
		: m_pos(first), m_first(first), m_last(std::end((Iterator)first)) {
	}

	template <class ContainerT>
	explicit MemReadArchive(const ContainerT& cont)
		: m_pos(cont.begin()), m_first(m_pos), m_last(cont.end()) {
	}

public:
	class Restriction
	{
	private:
		MemReadArchive& m_ar;
		Iterator m_pos;
		Iterator m_first;
		Iterator m_last;

	public:
		Restriction(MemReadArchive& ar, Iterator first, Iterator last)
			: m_ar(ar), m_pos(ar.m_pos), m_first(ar.m_first), m_last(ar.m_last) 
		{
			ar.m_first = ar.m_pos = first;
			ar.m_last = last;
		}

		~Restriction()
		{
			m_ar.m_pos = m_pos;
			m_ar.m_first = m_first;
			m_ar.m_last = m_last;
		}
	};

	typedef Restriction restriction_type;

public:
	void winx_call clear_cache()
	{
	}

	pos_type winx_call tell() const
	{
		return m_pos - m_first;
	}

	const_iterator winx_call position() const
	{
		return m_pos;
	}

	void winx_call seek(const_iterator pos)
	{
		m_pos = pos;
	}

	void winx_call seek(pos_type offset)
	{
		if (offset > size())
			throw std::out_of_range("Seek file position - out of range!");
		m_pos = m_first + offset;
	}

	void winx_call seek_end()
	{
		m_pos = m_last;
	}

public:
	size_type winx_call skip(size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax) {
			m_pos = m_last;
			return nRest;
		}
		else {
			m_pos += nMax;
			return nMax;
		}
	}

	bool winx_call try_skip(size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax)
			return false;
		else {
			m_pos += nMax;
			return true;
		}
	}

	bool winx_call try_get(char_type* lpBuf, size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax)
			return false;
		else {
			std::copy(m_pos, m_pos + nMax, lpBuf);
			m_pos += nMax;
			return true;
		}
	}

	const char_type* winx_call get(size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax) {
			m_pos = m_last;
			return NULL;
		}
		else {
			const char_type* rgData = iterToPointer(m_pos);
			m_pos += nMax;
			return rgData;
		}
	}

	size_type winx_call get(char_type* lpBuf, size_type nMax)
	{
		const size_type nRest = m_last - m_pos;
		if (nRest < nMax) {
			std::copy(m_pos, m_last, lpBuf);
			m_pos = m_last;
			return nRest;
		}
		else {
			std::copy(m_pos, m_pos + nMax, lpBuf);
			m_pos += nMax;
			return nMax;
		}
	}

	size_type winx_call get(char_type* ch)
	{
		if (m_pos != m_last) {
			*ch = *m_pos++;
			return 1;
		}
		else {
			return 0;
		}
	}

	int_type winx_call get()
	{
		if (m_pos != m_last) {
			return (uchar_type)*m_pos++;
		}
		else {
			return endch;
		}
	}

	void winx_call reget(size_type offset, char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset >= 0 && offset >= cch);

		std::copy(m_pos - offset, m_pos - offset + cch, lpBuf);
	}

	int winx_call unget()
	{
		if (m_pos != m_first) {
			--m_pos;
			return 1;
		}
		return 0;
	}

	void winx_call unget(int_type ch)
	{
		if (ch != endch)
			unget();
	}

	int_type winx_call peek()
	{
		if (m_pos != m_last) {
			return (uchar_type)*m_pos;
		}
		else {
			return endch;
		}
	}

public:
	pos_type winx_call size() const {
		return m_last - m_first;
	}

	bool winx_call operator!() const {
		return false;
	}

	bool winx_call good() const {
		return true;
	}

	bool winx_call bad() const {
		return false;
	}

public:
	void winx_call skipws() {
		NS_STDEXT_IO_TEXT::skipws(*this);
	}

	int_type winx_call getnws() {
		return NS_STDEXT_IO_TEXT::getnws(*this);
	}

	template <class StringT>
	size_type winx_call getline(StringT& s) {
		return NS_STDEXT_IO_TEXT::getline(*this, s);
	}

	template <class AllocT2>
	size_type winx_call getline(AllocT2& alloc, BasicString<char_type>& s) {
		return NS_STDEXT_IO_TEXT::getline(*this, alloc, s);
	}
};

// -------------------------------------------------------------------------
// $Log: MemArchive.h,v $

NS_STDEXT_END

#endif /* STDEXT_ARCHIVE_MEMARCHIVE_H */

