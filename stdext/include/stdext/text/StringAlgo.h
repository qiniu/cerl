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
// Module: stdext/text/StringAlgo.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: StringAlgo.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_STRINGALGO_H
#define STDEXT_TEXT_STRINGALGO_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

#ifndef STDEXT_TEXT_RANGE_H
#include "Range.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../CharType.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// codepage_t

enum codepage_t
{
	cp_unknown	= -1,
	cp_auto		= 0,
	cp_utf7		= 65000,
	cp_utf8		= 65001,
	cp_shiftjis	= 932,
	cp_gbk		= 936,
	cp_big5		= 950,
	cp_1252		= 1252,
};

// -------------------------------------------------------------------------
// iconv

template <class StringT>
inline void winx_call iconv(
	codepage_t from, const char* str, size_t cch, StringT& dest)
{
	int cch2 = MultiByteToWideChar(from, 0, str, cch, NULL, 0);
	wchar_t* str2 = std::resize(dest, cch2);
	MultiByteToWideChar(from, 0, str, cch, str2, cch2);
}

template <class StringT>
inline void winx_call iconv(
	const wchar_t* str, size_t cch, codepage_t to, StringT& dest)
{
	int cch2 = WideCharToMultiByte(to, 0, str, (int)cch, NULL, 0, NULL, NULL);
	char* str2 = std::resize(dest, cch2);
	WideCharToMultiByte(to, 0, str, (int)cch, str2, cch2, NULL, NULL);
}

template <class AllocT>
inline BasicString<wchar_t> winx_call iconv(
	AllocT& alloc, codepage_t from, const char* str, size_t cch)
{
	int cch2 = MultiByteToWideChar(from, 0, str, (int)cch, NULL, 0);
	wchar_t* str2 = STD_ALLOC_ARRAY(alloc, wchar_t, cch2);
	MultiByteToWideChar(from, 0, str, (int)cch, str2, cch2);
	return BasicString<wchar_t>(str2, cch2);
}

template <class AllocT>
inline BasicString<char> winx_call iconv(
	AllocT& alloc, const wchar_t* str, size_t cch, codepage_t to)
{
	int cch2 = WideCharToMultiByte(to, 0, str, (int)cch, NULL, 0, NULL, NULL);
	char* str2 = STD_ALLOC_ARRAY(alloc, char, cch2);
	WideCharToMultiByte(to, 0, str, (int)cch, str2, cch2, NULL, NULL);
	return BasicString<char>(str2, cch2);
}

template <class AllocT>
__forceinline BasicString<wchar_t> winx_call iconv(
	AllocT& alloc, codepage_t from, const String& str)
{
	return iconv(alloc, from, str.data(), str.size());
}

template <class AllocT>
__forceinline BasicString<char> winx_call iconv(
	AllocT& alloc, const WString& str, codepage_t to)
{
	return iconv(alloc, str.data(), str.size(), to);
}

// -------------------------------------------------------------------------
// upper/lower

template <class CharT, class AllocT, class TransT>
inline BasicString<CharT> winx_call transform(
	AllocT& alloc, const BasicString<CharT>& str, const TransT& trans)
{
	size_t n = str.size();
	CharT* p = STD_NEW_ARRAY(alloc, CharT, n);
	std::transform(str.begin(), str.end(), p, trans);
	return BasicString<CharT>(p, n);
}

template <class AllocT>
__forceinline BasicString<char> winx_call upper(AllocT& alloc, const String& str) {
	return transform(alloc, str, ToUpper<char>());
}

template <class AllocT>
__forceinline BasicString<wchar_t> winx_call upper(AllocT& alloc, const WString& str) {
	return transform(alloc, str, ToUpper<wchar_t>());
}

template <class AllocT>
__forceinline BasicString<char> winx_call lower(AllocT& alloc, const String& str) {
	return transform(alloc, str, ToLower<char>());
}

template <class AllocT>
__forceinline BasicString<wchar_t> winx_call lower(AllocT& alloc, const WString& str) {
	return transform(alloc, str, ToLower<wchar_t>());
}

// -------------------------------------------------------------------------
// append

__forceinline
std::vector<char>& winx_call append(std::vector<char>& s, const String& str) {
	s.insert(s.end(), str.begin(), str.end());
	return s;
}

__forceinline
std::vector<wchar_t>& winx_call append(std::vector<wchar_t>& s, const WString& str) {
	s.insert(s.end(), str.begin(), str.end());
	return s;
}

// -------------------------------------------------------------------------
// trim/trimLeft/trimRight

template <class Iterator>
inline Iterator winx_call trimLeft(Iterator first, Iterator last)
{
	return std::find_if(first, last, CharType::NotIsSpace());
}

template <class Iterator>
inline Iterator winx_call trimRight(Iterator first, Iterator last)
{
	const CharType::NotIsSpace notSpace = CharType::NotIsSpace();
	while (first != last)
	{
		if (notSpace(*--last))
			return ++last;
	}
	return first;
}

template <class CharT>
inline BasicString<CharT> winx_call trimLeft(const BasicString<CharT>& s)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	const iterator last = s.end();
	return BasicString<CharT>(trimLeft(s.begin(), last), last);
}

template <class CharT>
inline BasicString<CharT> winx_call trimRight(const BasicString<CharT>& s)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	const iterator first = s.begin();
	return BasicString<CharT>(first, trimRight(first, s.end()));
}

template <class CharT>
inline BasicString<CharT> winx_call trim(const BasicString<CharT>& s)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	const iterator last = s.end();
	const iterator first = trimLeft(s.begin(), last);
	return BasicString<CharT>(first, trimRight(first, last));
}

// -------------------------------------------------------------------------
// split

enum ExplodeFlags
{
	efEraseEmpty = 0x01,
	efTrim = 0x02,
	efDefault = efEraseEmpty | efTrim
};

template <int flags, class CharT, class ContainerT>
inline void winx_call split2(CharT sep, const BasicString<CharT>& s, ContainerT& cont)
{
	typedef typename BasicString<CharT>::const_iterator iterator;
	
	const iterator last = s.end();
	
	iterator it, first = s.begin();
	for (;;)
	{
		if (efTrim & flags)
			first = trimLeft(first, last);
		it = std::find(first, last, sep);
		if (it == last)
			break;
		if (efTrim & flags)
		{
			const iterator it2 = trimRight(first, it);
			if (!(efEraseEmpty & flags) || it2 != first)
				cont.push_back(BasicString<CharT>(first, it2));
		}
		else
		{
			if (!(efEraseEmpty & flags) || it != first)
				cont.push_back(BasicString<CharT>(first, it));
		}
		first = ++it;
	}
	if (first != it)
	{
		if (efTrim & flags)
		{
			const iterator it2 = trimRight(first, it);
			if (!(efEraseEmpty & flags) || it2 != first)
				cont.push_back(BasicString<CharT>(first, it2));
		}
		else
		{
			if (!(efEraseEmpty & flags) || it != first)
				cont.push_back(BasicString<CharT>(first, it));
		}
	}
}

template <class CharT, class ContainerT>
__forceinline
void winx_call split(CharT sep, const BasicString<CharT>& s, ContainerT& cont) {
	split2<efDefault>(sep, s, cont);
}

template <int flags, class CharT, class AllocT>
__forceinline
BasicArray<BasicString<CharT> >
winx_call split2(AllocT& alloc, CharT sep, const BasicString<CharT>& s) {
	std::vector<BasicString<CharT> > cont;
	split2<flags>(sep, s, cont);
	return BasicArray<BasicString<CharT> >(alloc, cont);
}

template <class CharT, class AllocT>
__forceinline
BasicArray<BasicString<CharT> >
winx_call split(AllocT& alloc, CharT sep, const BasicString<CharT>& s) {
	return split2<efDefault>(alloc, sep, s);
}

// -------------------------------------------------------------------------
// explode = split

template <int flags, class CharT, class ContainerT>
__forceinline
void winx_call explode2(CharT sep, const BasicString<CharT>& s, ContainerT& cont) {
	split2<flags>(sep, s, cont);
}

template <class CharT, class ContainerT>
__forceinline
void winx_call explode(CharT sep, const BasicString<CharT>& s, ContainerT& cont) {
	split2<efDefault>(sep, s, cont);
}

template <int flags, class CharT, class AllocT>
__forceinline
BasicArray<BasicString<CharT> >
winx_call explode2(AllocT& alloc, CharT sep, const BasicString<CharT>& s) {
	return split2<flags>(alloc, sep, s);
}

template <class CharT, class AllocT>
__forceinline
BasicArray<BasicString<CharT> >
winx_call explode(AllocT& alloc, CharT sep, const BasicString<CharT>& s) {
	return split2<efDefault>(alloc, sep, s);
}

// -------------------------------------------------------------------------
// implode

template <class AllocT, class GlueIt, class Iterator>
inline
BasicString<typename iterator_traits_alter<Iterator>::value_type::value_type>
winx_call implode(AllocT& alloc, const GlueIt glue, size_t n, const Iterator first, const size_t count)
{
	typedef typename iterator_traits_alter<Iterator>::value_type ContainerT;
	typedef typename ContainerT::value_type ValueT;
	
	if (count < 2)
		return count ? BasicString<ValueT>(alloc, *first) : BasicString<ValueT>();
	
	Iterator it = first;
	size_t i, len = 0;
	for (i = 0; i < count; ++i)
		len += (*it++).size();

	len += (count-1)*n;
	it = first;
	ValueT* buf = STD_NEW_ARRAY(alloc, ValueT, len);
	for (i = 1; i < count; ++i) {
		const ContainerT& cont = *it++; 
		buf = std::copy(cont.begin(), cont.end(), buf);
		buf = std::copy(glue, glue+n, buf);
	}
	
	const ContainerT& cont = *it;
	buf = std::copy(cont.begin(), cont.end(), buf);
	return BasicString<ValueT>(buf-len, buf);
}

template <class AllocT, class Iterator>
__forceinline
BasicString<char>
winx_call implode(AllocT& alloc, const String& glue, Iterator first, size_t count) {
	return implode(alloc, glue.begin(), glue.size(), first, count);
}

template <class AllocT, class Iterator>
__forceinline
BasicString<wchar_t>
winx_call implode(AllocT& alloc, const WString& glue, Iterator first, size_t count) {
	return implode(alloc, glue.begin(), glue.size(), first, count);
}

template <class AllocT, class Iterator>
__forceinline
BasicString<char>
winx_call implode(AllocT& alloc, const char glue, Iterator first, size_t count) {
	return implode(alloc, &glue, 1, first, count);
}

template <class AllocT, class Iterator>
__forceinline
BasicString<wchar_t>
winx_call implode(AllocT& alloc, const wchar_t glue, Iterator first, size_t count) {
	return implode(alloc, &glue, 1, first, count);
}

template <class AllocT, class ContainerT>
__forceinline
BasicString<char>
winx_call implode(AllocT& alloc, const String& glue, const ContainerT& cont) {
	return implode(alloc, glue.begin(), glue.size(), cont.begin(), cont.size());
}

template <class AllocT, class ContainerT>
__forceinline
BasicString<wchar_t>
winx_call implode(AllocT& alloc, const WString& glue, const ContainerT& cont) {
	return implode(alloc, glue.begin(), glue.size(), cont.begin(), cont.size());
}

template <class AllocT, class ContainerT>
__forceinline
BasicString<char>
winx_call implode(AllocT& alloc, const char glue, const ContainerT& cont) {
	return implode(alloc, &glue, 1, cont.begin(), cont.size());
}

template <class AllocT, class ContainerT>
__forceinline
BasicString<wchar_t>
winx_call implode(AllocT& alloc, const wchar_t glue, const ContainerT& cont) {
	return implode(alloc, &glue, 1, cont.begin(), cont.size());
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#ifndef STDEXT_TEXT_CONCAT_ALGO_H
#include "concat/algo.h"
#endif

// -------------------------------------------------------------------------
// class TestStringAlgo

#if defined(STD_UNITTEST)

template <class LogT>
class TestStringAlgo : public TestCase
{
	WINX_TEST_SUITE(TestStringAlgo);
		WINX_TEST(testConcat);
		WINX_TEST(testConcat2);
		WINX_TEST(testImplode);
		WINX_TEST(testExplode);
		WINX_TEST(testConv);
		WINX_TEST(testTrim);
//		WINX_TEST(testIconv);
	WINX_TEST_SUITE_END();

public:
	void testIconv(LogT& log)
	{
		NS_STDEXT::ScopedAlloc alloc;

		NS_STDEXT::WString s1 = NS_STDEXT::iconv(alloc, NS_STDEXT::cp_auto, "Hello, world!");
		AssertExp(s1 == L"Hello, world!");

		NS_STDEXT::String s2 = NS_STDEXT::iconv(alloc, s1, NS_STDEXT::cp_utf8);
		AssertExp(s2 == "Hello, world!");
	}
	
	void testConv(LogT& log)
	{
		NS_STDEXT::ScopedAlloc alloc;

		std::string s1 = "ABC";
		AssertExp(NS_STDEXT::lower(alloc, s1) == "abc");
		
		NS_STDEXT::String s2(alloc, "abc");
		AssertExp(NS_STDEXT::upper(alloc, s2) == "ABC");
	}
	
	void testTrim(LogT& log)
	{
		NS_STDEXT::ScopedAlloc alloc;
		NS_STDEXT::String s(alloc, " \t Hello, world! \t\n");
		
		AssertExp(NS_STDEXT::trim(s) == "Hello, world!");
		AssertExp(NS_STDEXT::trimLeft(s) == "Hello, world! \t\n");
		AssertExp(NS_STDEXT::trimRight(s) == " \t Hello, world!");
	}

	void testImplode(LogT& log)
	{
		NS_STDEXT::AutoAlloc alloc;

		std::list<std::string> lst;
		lst.push_back("Hello,");
		lst.push_back("world!");
		lst.push_back("I");
		lst.push_back("am");
		lst.push_back("xushiwei!\n");

		NS_STDEXT::String s = NS_STDEXT::implode(alloc, ' ', lst);
		AssertExp(s == "Hello, world! I am xushiwei!\n");
	
		s = NS_STDEXT::implode(alloc, " -> ", lst);
		AssertExp(s == "Hello, -> world! -> I -> am -> xushiwei!\n");
	}
	
	void testExplode(LogT& log)
	{
		NS_STDEXT::AutoAlloc alloc;

		NS_STDEXT::String s(alloc, "Hello, world!  I am xushiwei!");

		NS_STDEXT::BasicArray<NS_STDEXT::String> arr = NS_STDEXT::explode(alloc, ' ', s);
		AssertExp(arr.size() == 5);
		AssertExp(
			arr[0] == "Hello," &&
			arr[1] == "world!" &&
			arr[2] == "I" &&
			arr[3] == "am" &&
			arr[4] == "xushiwei!");
		
		arr = NS_STDEXT::explode2<0>(alloc, ' ', s);
		AssertExp(arr.size() == 6);
		AssertExp(
			arr[0] == "Hello," &&
			arr[1] == "world!" &&
			arr[2] == "" &&
			arr[3] == "I" &&
			arr[4] == "am" &&
			arr[5] == "xushiwei!");
	}

	void testConcat2(LogT& log)
	{
		NS_STDEXT::AutoAlloc alloc;
		{
			std::list<std::string> lst;
			lst.push_back("Hello,");
			lst.push_back(" ");
			lst.push_back("world!");
		
			NS_STDEXT::String s = NS_STDEXT::concat(alloc, lst);
			AssertExp(s == "Hello, world!");
		}
		{
			std::vector<std::string> vec;
			vec.push_back("Hello,");
			vec.push_back(" ");
			vec.push_back("world!");
		
			NS_STDEXT::String s = NS_STDEXT::concat(alloc, vec);
			AssertExp(s == "Hello, world!");
		}
	}

	void testConcat(LogT& log)
	{
		NS_STDEXT::ScopedAlloc alloc;

		NS_STDEXT::String s[4];
		s[0].attach("Hello");
		s[1].assign(alloc, ", ");
		s[2] = NS_STDEXT::String(alloc, "world");
		s[3].assign(alloc, 3, '!');

		NS_STDEXT::String result = NS_STDEXT::concat(alloc, s, countof(s));
		AssertExp(result == "Hello, world!!!");

		NS_STDEXT::String result2 = NS_STDEXT::concat(alloc, "Hello", "!!!");
		AssertExp(result2 == "Hello!!!");

		NS_STDEXT::String result3 = NS_STDEXT::concat(alloc, s[0], s[1], s[2], s[3]);
		AssertExp(result == result3);

		char s2buf[] = "world";
		std::vector<char> s2(s2buf, NS_STDEXT::end(s2buf));
		std::string s1 = result2.stl_str();
		NS_STDEXT::String result4 = NS_STDEXT::concat(alloc, s1, " ", s2, "!!", "!");
		AssertExp(result4 == "Hello!!! world!!!");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_STRINGALGO_H */

