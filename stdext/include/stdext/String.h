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
// Module: stdext/String.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:48:49
// 
// $Id: String.h,v 1.3 2007/01/10 09:35:16 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_STRING_H
#define STDEXT_STRING_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICALGO_H
#include "text/BasicAlgo.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "text/BasicString.h"
#endif

#ifndef STDEXT_TEXT_CSTRING_H
#include "text/CString.h"
#endif

#ifndef STDEXT_TEXT_STRINGBUILDER_H
#include "text/StringBuilder.h"
#endif

#ifndef STDEXT_TEXT_STRINGALGO_H
#include "text/StringAlgo.h"
#endif

#ifndef STDEXT_TEXT_FORMAT_H
#include "text/Format.h"
#endif

#ifndef STDEXT_TEXT_CAST_H
#include "text/Cast.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class tstring

#if defined(UNICODE)
typedef std::basic_string<WCHAR> tstring;
#else
typedef std::basic_string<char> tstring;
#endif

// -------------------------------------------------------------------------
// trim

template <class _E, class Tr, class AllocT>
inline void winx_call trim(std::basic_string<_E, Tr, AllocT>& str, const _E* blanks)
{
	typedef std::basic_string<_E, Tr, AllocT> StringT;
	typedef typename StringT::size_type size_type;

	size_type pos1 = str.find_first_not_of(blanks);
	if (pos1 == StringT::npos)
	{
		str.erase();
		return;
	}

	size_type count = str.find_last_not_of(blanks) - pos1 + 1;
	if (str.length() != count)
		str = str.substr(pos1, count);
}

template <class Tr, class AllocT>
inline void winx_call trim(std::basic_string<char, Tr, AllocT>& str)
{
	const char blanks[] = { ' ', '\t', '\r', '\n', '\0' };
	trim(str, blanks);
}

template <class Tr, class AllocT>
inline void winx_call trim(std::basic_string<WCHAR, Tr, AllocT>& str)
{
	const WCHAR blanks[] =
	{
		/* 支持中文空格 */
		' ', 12288, '\t', '\r', '\n', '\0'
	};
	trim(str, blanks);
}

// -------------------------------------------------------------------------
// icompare

template <class CharT, class Tr, class AllocT>
inline int winx_call icompare(
	const std::basic_string<CharT, Tr, AllocT>& a, const CharT b[])
{
	return compare_by(a.begin(), a.end(), b, CompareNoCase<CharT>());
}

template <class CharT, class Tr, class AllocT, class ContainerT>
inline int winx_call icompare(
	const std::basic_string<CharT, Tr, AllocT>& a, const ContainerT& b)
{
	return compare_by(a.begin(), a.end(), b.begin(), b.end(), CompareNoCase<CharT>());
}

template <class CharT, class AllocT>
inline int winx_call icompare(
	const std::vector<CharT, AllocT>& a, const CharT b[])
{
	return compare_by(a.begin(), a.end(), b, CompareNoCase<CharT>());
}

template <class CharT, class AllocT, class ContainerT>
inline int winx_call icompare(
	const std::vector<CharT, AllocT>& a, const ContainerT& b)
{
	return compare_by(a.begin(), a.end(), b.begin(), b.end(), CompareNoCase<CharT>());
}

template <class Iterator, class CharT>
inline int winx_call icompare(
	const Range<Iterator>& a, const CharT b[])
{
	return compare_by(a.begin(), a.end(), b, CompareNoCase<CharT>());
}

template <class Iterator, class ContainerT>
inline int winx_call icompare(
	const Range<Iterator>& a, const ContainerT& b)
{
	typedef typename Range<Iterator>::char_type CharT;
	return compare_by(a.begin(), a.end(), b.begin(), b.end(), CompareNoCase<CharT>());
}

// -------------------------------------------------------------------------
// strupper, strlower

template <class StringT>
inline void winx_call strupper(StringT& str)
{
    typename StringT::iterator itEnd = str.end();
    for (typename StringT::iterator it = str.begin(); it != itEnd; ++it)
    {
		*it = ::toupper(*it);
    }
}

template <class StringT>
inline void winx_call strlower(StringT& str)
{
    typename StringT::iterator itEnd = str.end();
    for (typename StringT::iterator it = str.begin(); it != itEnd; ++it)
    {
		*it = ::tolower(*it);
    }
}

// -------------------------------------------------------------------------
// replaceText

template <class StringT>
inline void winx_call replaceText(
	StringT& szText, const StringT& szFindText, const StringT& szReplaceText)
{
	typedef typename StringT::size_type size_type;
	size_type pos = 0;
	while (
		(pos = szText.find(szFindText, pos)) != StringT::npos
		)
	{
		szText.replace(pos, szFindText.size(), szReplaceText);
		pos += szReplaceText.size();
	}
}

// -------------------------------------------------------------------------
// class TestString

#if defined(STD_UNITTEST)

template <class LogT>
class TestString : public TestCase
{
public:
	WINX_TEST_SUITE(TestString);
		WINX_TEST(testTrim);
		WINX_TEST(testUpperLower);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testTrim(LogT& log)
	{
		std::string s = " \t  abc 123  \r\n";
		NS_STDEXT::trim(s);
		AssertExp(s == "abc 123");
	}

	void testUpperLower(LogT& log)
	{
		std::string s = "abc123";
		NS_STDEXT::strupper(s);
		AssertExp(s == "ABC123");
		NS_STDEXT::strlower(s);
		AssertExp(s == "abc123");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: String.h,v $

NS_STDEXT_END

#endif /* STDEXT_STRING_H */
