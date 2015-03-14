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
// Module: stdext/search/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-8-8 20:30:40
// 
// $Id: Basic.h 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_SEARCH_BASIC_H
#define STDEXT_SEARCH_BASIC_H

// -------------------------------------------------------------------------

#ifndef _INC_CTYPE
#include <ctype.h>
#endif

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "../archive/Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// c++ std::istream adapter

// I use 'istream_type' because I don't want to include <istream>.
template <class istream_type>
class istream2archive : public istream_type
{
public:
	enum { endch = -1 };
};

// -------------------------------------------------------------------------
// c style string adapter

template <class CharT>
class cstr2archive
{
public:
	enum { endch = -1 };

	typedef CharT char_type;
	typedef typename ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;
	
private:
	const CharT* m_p;
	
public:
	cstr2archive(const CharT* p) : m_p(p) {}
	
	__forceinline int_type winx_call get()
	{
		return *m_p ? (uchar_type)*m_p++ : (int_type)endch;
	}

	__forceinline const CharT* winx_call tell() const
	{
		return m_p;
	}
};

// -------------------------------------------------------------------------
// iterator adapter

template <class iterator_type>
class iterator2archive
{
private:
	typedef ArchiveIteratorTraits<iterator_type> Tr_;

public:
	enum { endch = -1 };

	typedef iterator_type iterator;
	typedef typename Tr_::value_type char_type;
	typedef typename ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;
	
private:
	iterator m_it;
	iterator m_itEnd;
	
public:
	iterator2archive(iterator it, iterator itEnd)
		: m_it(it), m_itEnd(itEnd) {
	}

	__forceinline int_type winx_call get()
	{
		return m_it != m_itEnd ? (uchar_type)*m_it++ : (int_type)endch;
	}

	__forceinline iterator winx_call tell() const
	{
		return m_it;
	}
};

// -------------------------------------------------------------------------
// limited_archive

template <class archive_type>
class limited_archive
{
public:
	enum { endch = archive_type::endch };

	typedef typename archive_type::value_type char_type;
	typedef typename archive_type::uchar_type uchar_type;
	typedef typename archive_type::int_type int_type;
	
private:
	archive_type& m_ar;
	size_t m_rest;
	
public:
	limited_archive(archive_type& ar, size_t limit)
		: m_ar(ar), m_rest(limit) {
	}

	__forceinline int_type winx_call get()
	{
		if (m_rest)
		{
			--m_rest;
			return m_ar.get();
		}
		return (int_type)endch;
	}
};

// -------------------------------------------------------------------------
// Finder Strategy

/*
@class MatchCase
@brief
	The MatchCase class is provided as a strategy of class \<Finder>. It means the
	searching algorithm will process in case-sensitive mode.
@arg CharT
	The data type of a single character. It must be the same as you pass to \<Finder> class.
@see Finder, MatchNoCase
@*/
template <class CharT>
struct MatchCase
{
	typedef CharT char_type;
	typedef typename ArchiveCharTraits<char_type>::int_type int_type;

	template <class Iterator>
	__forceinline static void winx_call copy(char_type* dest, Iterator src, size_t n)
	{
		while (n--)
			*dest++ = *src++;
	}

	template <class ArchiveT>
	__forceinline static typename ArchiveT::int_type winx_call get(ArchiveT& ar)
	{
		return ar.get();
	}
	
	__forceinline static int_type winx_call transform(int_type c)
	{
		return c;
	}
};

/*
@class MatchNoCase
@brief
	The MatchNoCase class is provided as a strategy of class \<Finder>. It means the
	searching algorithm will process in non case-sensitive mode.
@arg CharT
	The data type of a single character. It must be the same as you pass to \<Finder> class.
@see Finder, MatchCase
@*/
template <class CharT>
struct MatchNoCase
{
	typedef CharT char_type;
	typedef ToLower<CharT> TransformT;
	typedef ArchiveCharTraits<char_type> Tr;
	typedef typename Tr::int_type int_type;

	template <class Iterator>
	static void winx_call copy(char_type* dest, Iterator src, size_t n)
	{
		const TransformT up = TransformT();
		while (n--)
			*dest++ = up( *src++ );
	}

	template <class ArchiveT>
	static typename ArchiveT::int_type winx_call get(ArchiveT& ar)
	{
		const TransformT up = TransformT();
		const int_type ch = ar.get();
		return ch != Tr::endch ? up(ch) : Tr::endch;
	}

	static int_type winx_call transform(int_type ch)
	{
		const TransformT up = TransformT();
		return ch != Tr::endch ? up(ch) : Tr::endch;
	}
};

// -------------------------------------------------------------------------
// $Log: Basic.h,v $

NS_STDEXT_END

#endif /* STDEXT_SEARCH_BASIC_H */

