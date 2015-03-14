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
// Module: stdext/archive/Text.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Text.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_TEXT_H
#define STDEXT_ARCHIVE_TEXT_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../text/BasicString.h"
#endif

#ifndef NS_STDEXT_IO_TEXT
#define NS_STDEXT_IO_TEXT			NS_STDEXT::io
#define NS_STDEXT_IO_TEXT_BEGIN		NS_STDEXT_BEGIN namespace io {
#define NS_STDEXT_IO_TEXT_END		} NS_STDEXT_END
#endif

NS_STDEXT_IO_TEXT_BEGIN

// -------------------------------------------------------------------------
// ==== TextReader ====

//
// get one char
//

template <class ReadArchiveT, class ConditionT>
inline typename ReadArchiveT::int_type
	winx_call get_first_not_of(ReadArchiveT& ar, ConditionT cond)
{
	typedef typename ReadArchiveT::int_type int_type;

	int_type ch;
	while (	cond(ch = ar.get()) );
	return ch;
}

template <class ReadArchiveT>
inline typename ReadArchiveT::int_type winx_call getnws(ReadArchiveT& ar)
{
	typedef typename ReadArchiveT::int_type int_type;

	int_type ch;
	while (	::isspace(ch = ar.get()) );
	return ch;
}

//
// skip chars
//

template <class ReadArchiveT>
inline size_t winx_call skip_eol(ReadArchiveT& ar)
{
	typedef typename ReadArchiveT::int_type int_type;

	const int_type ch = ar.peek();
	if (ch == 0x0a)
	{
		ar.get();
		if (ar.peek() == 0x0d) {
			ar.get();
			return 2;
		}
		return 1;
	}
	else if (ch == 0x0d)
	{
		ar.get();
		if (ar.peek() == 0x0a) {
			ar.get();
			return 2;
		}
		return 1;
	}
	return 0;
}

template <class ReadArchiveT>
inline void winx_call skipws(ReadArchiveT& ar)
{
	typedef typename ReadArchiveT::int_type int_type;

	int_type ch;
	while (	::isspace(ch = ar.get()) );
	ar.unget(ch);
}

template <class ReadArchiveT, class ConditionT>
inline size_t winx_call skip_while(ReadArchiveT& ar, ConditionT cond)
{
	typedef typename ReadArchiveT::int_type int_type;
	typedef typename ReadArchiveT::size_type size_type;

	WINX_ASSERT( !cond(ReadArchiveT::endch) );

	size_type count = 0;
	int_type ch;
	while ( cond(ch = ar.get()) )
		++count;
	ar.unget(ch);
	return count;
}

template <class ReadArchiveT>
inline size_t winx_call skip_csymbol(ReadArchiveT& ar)
{
	typedef typename ReadArchiveT::int_type int_type;

	int_type ch = ar.get();
	if ( CharType::isCSymbolFirstChar(ch) ) {
		return skip_while(ar, CharType::IsCSymbolNextChar()) + 1;
	}
	else {
		ar.unget(ch);
		return 0;
	}
}

//
// get a text string
//

template <class ReadArchiveT, class StringT, class ConditionT>
inline size_t winx_call get_while(
	ReadArchiveT& ar, StringT& s, ConditionT cond, size_t count = 0)
{
	count += skip_while(ar, cond);
	ar.reget(count, std::resize(s, count), count);
	return count;
}

template <class ReadArchiveT, class AllocT, class ConditionT>
inline size_t winx_call get_while(
	ReadArchiveT& ar, AllocT& alloc, BasicString<typename ReadArchiveT::char_type>& s, ConditionT cond)
{
	OutputBasicString<typename ReadArchiveT::char_type, AllocT> s1(alloc, s);
	return get_while(ar, s1, cond);
}

template <class ReadArchiveT, class StringT>
inline size_t winx_call getline(ReadArchiveT& ar, StringT& s)
{
	return get_while(ar, s, CharType::NotIsEOL()) + skip_eol(ar);
}

template <class ReadArchiveT, class AllocT, class CharT>
inline size_t winx_call getline(
	ReadArchiveT& ar, AllocT& alloc, BasicString<CharT>& s)
{
	OutputBasicString<CharT, AllocT> s1(alloc, s);
	return getline(ar, s1);
}

template <class ReadArchiveT, class StringT>
inline size_t winx_call get_csymbol(ReadArchiveT& ar, StringT& s)
{
	size_t count = skip_csymbol(ar);
	ar.reget(count, std::resize(s, count), count);
	return count;
}

template <class ReadArchiveT, class AllocT>
inline size_t winx_call get_csymbol(
	ReadArchiveT& ar, AllocT& alloc, BasicString<typename ReadArchiveT::char_type>& s)
{
	OutputBasicString<typename ReadArchiveT::char_type, AllocT> s1(alloc, s);
	return get_csymbol(ar, s1);
}

template <class ReadArchiveT, class UIntType>
inline UIntType winx_call get_uint(ReadArchiveT& ar, UIntType preval, unsigned radix = 10)
{
	typedef typename ReadArchiveT::int_type int_type;

	unsigned dig;
	int_type ch;
	while ( (dig = DigitTable::toDigit(ch = ar.get())) < radix )
		preval = preval * radix + dig;
	ar.unget(ch);
	return preval;
}

template <class ReadArchiveT>
inline UINT winx_call get_uint(ReadArchiveT& ar)
{
	return get_uint(ar, (UINT)0, 10);
}

template <class ReadArchiveT, class UIntType>
inline HRESULT winx_call scan_uint(ReadArchiveT& ar, UIntType& val, unsigned radix = 10)
{
	typedef typename ReadArchiveT::int_type int_type;

	int_type ch = getnws(ar);
	unsigned dig = DigitTable::toDigit(ch);
	if (dig < radix)
	{
		val = get_uint(ar, (UIntType)dig, radix);
		return S_OK;
	}
	else
	{
		ar.unget(ch);
		val = 0;
		return E_UNEXPECTED;
	}
}

template <class ReadArchiveT, class StringT>
inline HRESULT winx_call scan_csymbol(ReadArchiveT& ar, StringT& s)
{
	skipws(ar);
	return get_csymbol(ar, s) ? S_OK : E_UNEXPECTED;
}

template <class ReadArchiveT, class AllocT>
inline HRESULT winx_call scan_csymbol(
	ReadArchiveT& ar, AllocT& alloc, BasicString<typename ReadArchiveT::char_type>& s)
{
	OutputBasicString<typename ReadArchiveT::char_type, AllocT> s1(alloc, s);
	return scan_csymbol(ar, s1);
}

// -------------------------------------------------------------------------
// ==== TextWriter ====

template <class WriteArchiveT>
inline void winx_call print(
	WriteArchiveT& ar, const String& s) throw(IoException)
{
	ar.put(s.data(), s.size());
}

template <class WriteArchiveT>
inline void winx_call print(
	WriteArchiveT& ar, const WString& s) throw(IoException)
{
	ar.put(s.data(), s.size());
}

template <class WriteArchiveT, class DataIt, class SepT>
inline void winx_call printLines(
	WriteArchiveT& ar, DataIt first, DataIt last, const SepT& sep) throw(IoException)
{
	for (; first != last; ++first)
	{
		print(ar, *first);
		print(ar, sep);
	}
}

template <class WriteArchiveT, class DataIt>
inline void winx_call printLines(
	WriteArchiveT& ar, DataIt first, DataIt last) throw(IoException)
{
	typedef typename WriteArchiveT::char_type CharT;
	for (; first != last; ++first)
	{
		print(ar, *first);
		ar.put((CharT)'\n');
	}
}

template <class WriteArchiveT, class ContainerT>
inline void winx_call printLines(
	WriteArchiveT& ar, const ContainerT& rg) throw(IoException)
{
	printLines(ar, rg.begin(), rg.end());
}

// -------------------------------------------------------------------------
// $Log: Text.h,v $

NS_STDEXT_IO_TEXT_END

#endif /* STDEXT_ARCHIVE_TEXT_H */
