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
// Module: stdext/text/TextPool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: TextPool.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_TEXTPOOL_H
#define STDEXT_TEXT_TEXTPOOL_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

#ifndef STDEXT_TEXT_STRINGALGO_H
#include "StringAlgo.h"
#endif

#ifndef STDEXT_DEQUE_H
#include "../Deque.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

#define WINX_TP_USING_DEQUE_												\
public:																		\
	using Base::insert;														\
	using Base::erase;														\
	using Base::clear;														\
	using Base::begin;														\
	using Base::end;														\
	using Base::rbegin;														\
	using Base::rend;														\
	using Base::size

// -------------------------------------------------------------------------
// class TextPool

template <class CharT, class AllocT = DefaultAlloc>
class TextPool : public Deque<CharT, AllocT>
{
private:
	typedef Deque<CharT, AllocT> Base, BaseClass;
	WINX_TP_USING_DEQUE_;
	
	typedef std::basic_string<CharT> StlString_;
	typedef BasicString<CharT> String_;
	typedef TextPool Myt_;

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

public:
	explicit TextPool(AllocT& alloc)
		: Base(alloc) {}

	TextPool(AllocT& alloc, const String_ s)
		: Base(alloc, s.begin(), s.end()) {}

	TextPool(AllocT& alloc, size_type cch, CharT ch)
		: Base(alloc, cch, ch) {}

	TextPool(AllocT& alloc, const CharT* s, size_type count) 
		: Base(alloc, s, s+count) {} 

	template <class _InputIterator>
	TextPool(AllocT& alloc, _InputIterator first, _InputIterator last)
		: Base(alloc, first, last) {}

public:
	template <class AllocT2>
	BasicString<CharT> winx_call str(AllocT2& alloc) const {
		return BasicString<CharT>(alloc, begin(), end());
	}

	template <class AllocT2>
	BasicString<CharT> winx_call substr(
		AllocT2& alloc, size_type from = 0, size_type cch = (size_type)-1) const
	{
		size_type cchMax = size() - from;
		if ((difference_type)cchMax < 0)
			throw_out_of_range_();
		size_type cchLength = (cchMax < cch ? cchMax : cch);
		const_iterator it = begin() + from;
		return BasicString<CharT>(alloc, it, it + cchLength);
	}

	StlString_ winx_call stl_str() const {
		return StlString_(begin(), end());
	}

	size_type winx_call length() const {
		return size();
	}

public:
	template <class _InputerIterator>
	Myt_& winx_call assign(_InputerIterator first, _InputerIterator last)
	{
		Base::assign(first, last);
		return *this;
	}

	Myt_& winx_call assign(const Base& s)
    {
		Base::copy(s);
		return *this;
	}

	Myt_& winx_call assign(const String_ s)
    {
		Base::assign(s.begin(), s.end());
		return *this;
	}

	Myt_& winx_call assign(const CharT* s, size_type cch)
	{
		Base::assign(s, s + cch);
		return *this;
	}

	Myt_& winx_call assign(size_type cch, CharT ch)
	{
		Base::assign(cch, ch);
		return *this;
	}

	Myt_& winx_call operator=(const String_ s)
	{
		Base::assign(s.begin(), s.end());
		return *this;
	}

public:
	template <class _InputerIterator>
	Myt_& winx_call append(_InputerIterator first, _InputerIterator last)
	{
		Base::insert(end(), first, last);
		return *this;
	}

	Myt_& winx_call append(const Base& s)
	{
		Base::insert(end(), s.begin(), s.end());
		return *this;
	}

	Myt_& winx_call append(const String_ s)
    {
		Base::insert(end(), s.begin(), s.end());
		return *this;
	}

	Myt_& winx_call append(const CharT* s, size_type cch)
	{
		Base::insert(end(), s, s + cch);
		return *this;
	}

	Myt_& winx_call append(size_type cch, CharT ch)
	{
		Base::insert(end(), cch, ch);
		return *this;
	}

	Myt_& winx_call operator+=(const String_ s)
	{
		Base::insert(end(), s.begin(), s.end());
		return *this;
	}

	Myt_& winx_call operator+=(const Base& s)
	{
		Base::insert(end(), s.begin(), s.end());
		return *this;
	}

#if !defined(X_CC_VC6) // (bug) vc++ 6.0
	template <class _RandIterator>
	Myt_& winx_call replace(
		iterator first, iterator last,
		_RandIterator bfirst, _RandIterator blast)
	{
		std::replace(WINX_BASE, first, last, bfirst, blast);
		return *this;
	}
#endif

	Myt_& winx_call replace(
		iterator first, iterator last, size_type count, CharT ch)
	{
		std::replace(WINX_BASE, first, last, count, ch);
		return *this;
	}

	Myt_& winx_call replace(
		iterator first, iterator last, const CharT* s, size_type cch)
	{
		std::replace(WINX_BASE, first, last, s, s + cch);
		return *this;
	}

	Myt_& winx_call replace(iterator first, iterator last, const String_ s)
	{
		std::replace(WINX_BASE, first, last, s.begin(), s.end());
		return *this;
	}

public:
	Myt_& winx_call insert(iterator it, const Base& s)
	{
		Base::insert(it, s.begin(), s.end());
		return *this;
	}

	Myt_& winx_call insert(iterator it, const String_ s)
    {
		Base::insert(it, s.begin(), s.end());
		return *this;
	}

	Myt_& winx_call insert(iterator it, const CharT* s, size_type cch)
	{
		Base::insert(it, s, s + cch);
		return *this;
	}

public:
	WINX_FIND_ALL_(find, std::find, std::search)
	WINX_FIND_ALL_(rfind, std::rfind, std::find_end)
	WINX_FIND_ALL_(find_first_of, std::find, std::find_first_of)
	WINX_FIND_ALL_(find_first_not_of, std::find_not, std::find_first_not_of)

//	WINX_RFIND_ALL_(find_last_of, std::find, std::find_first_of)
//	WINX_RFIND_ALL_(find_last_not_of, std::find_not, std::find_first_not_of)

public:
	template <class ContainerT>
	int winx_call compare(const ContainerT& b) const
		{return std::compare(begin(), end(), b.begin(), b.end()); }

	int winx_call compare(const CharT* b, size_type blen) const
		{return std::compare(begin(), end(), b, b + blen); }

	int winx_call compare(const CharT* b) const
		{return std::compare(begin(), end(), b); }

public:
	template <class ContainerT, class ComprT>
	int winx_call compare_by(const ContainerT& b, ComprT cmp) const
		{return std::compare_by(begin(), end(), b.begin(), b.end(), cmp); }

	template <class ComprT>
	int winx_call compare_by(const CharT* b, size_type blen, ComprT cmp) const
		{return std::compare_by(begin(), end(), b, b + blen, cmp); }

	template <class ComprT>
	int winx_call compare_by(const CharT* b, ComprT cmp) const
		{return std::compare_by(begin(), end(), b, cmp); }

private:
	typedef CompareNoCase<CharT> ComareNoCase_;

public:
	template <class ContainerT>
	int winx_call icompare(const ContainerT& b) const
		{return compare_by(b, ComareNoCase_()); }

	int winx_call icompare(const CharT* b, size_type blen) const
		{return compare_by(b, blen, ComareNoCase_()); }

	int winx_call icompare(const CharT* b) const
		{return compare_by(b, ComareNoCase_()); }

public:
	bool winx_call contains(const String_ b) const
		{return find(b) != end(); }

	bool winx_call contains(const CharT* b, size_type blen) const
		{return find(b, blen) != end(); }
};

// -------------------------------------------------------------------------

template <class CharT, class AllocT, class T2> __forceinline
bool winx_call operator==(const TextPool<CharT, AllocT>& a, const T2& b)
	{return a.compare(b) == 0; }

template <class CharT, class AllocT> __forceinline
bool winx_call operator==(const BasicString<CharT>& a, const TextPool<CharT, AllocT>& b)
	{return b.compare(a) == 0; }

template <class CharT, class AllocT> __forceinline
bool winx_call operator==(const BasicString<CharT>& a, TextPool<CharT, AllocT>& b)
	{return b.compare(a) == 0; }

template <class CharT, class AllocT, class T2> __forceinline
bool winx_call operator!=(const TextPool<CharT, AllocT>& a, const T2& b)
	{return a.compare(b) != 0; }

template <class CharT, class AllocT, class T2> __forceinline
bool winx_call operator<(const TextPool<CharT, AllocT>& a, const T2& b)
	{return a.compare(b) < 0; }

template <class CharT, class AllocT, class T2> __forceinline
bool winx_call operator>(const TextPool<CharT, AllocT>& a, const T2& b)
	{return a.compare(b) > 0; }

template <class CharT, class AllocT, class T2> __forceinline
bool winx_call operator<=(const TextPool<CharT, AllocT>& a, const T2& b)
	{return a.compare(b) <= 0; }

template <class CharT, class AllocT, class T2> __forceinline
bool winx_call operator>=(const TextPool<CharT, AllocT>& a, const T2& b)
	{return a.compare(b) >= 0; }

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestTextPool

#if defined(STD_UNITTEST)

template <class LogT>
class TestTextPool : public TestCase
{
	WINX_TEST_SUITE(TestTextPool);
		WINX_TEST(testConstruct);
		WINX_TEST(testSubstr);
		WINX_TEST(testAssign);
		WINX_TEST(testAppend);
		WINX_TEST(testReplace);
	WINX_TEST_SUITE_END();

public:
	void testConstruct(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;

		NS_STDEXT::TextPool<char> a1(alloc, 'a');
		AssertExp(a1.size() == 1 && a1[0] == 'a');

		NS_STDEXT::TextPool<char> a2(alloc, 3, 'a');
		AssertExp(a2 == "aaa");

		NS_STDEXT::TextPool<char> a(alloc, "Hello");
		AssertExp(a == "Hello");

		NS_STDEXT::TextPool<char> b(alloc, "Hello", 4);
		AssertExp(b == "Hell");

		NS_STDEXT::TextPool<char> c(alloc, b.begin(), b.end());
		NS_STDEXT::String d = c.str(alloc);
		AssertExp(d == b && b == d);
	}

	void testSubstr(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;
		NS_STDEXT::TextPool<char> a(alloc, "Hello");
		AssertExp(a.substr(alloc, 1, 3) == "ell");
		AssertExp(a.substr(alloc, 1) == "ello");
		AssertExp(a.substr(alloc, 5) == "");
	}

	void testAssign(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;
		NS_STDEXT::TextPool<char> a(alloc);
		
		a.assign('a');
		AssertExp(a == "a");

		a.assign(3, 'a');
		AssertExp(a == "aaa");

		a.assign("Hello");
		AssertExp(a == "Hello");

		NS_STDEXT::TextPool<char> b(alloc);
		b.assign("Hello", 2);
		AssertExp(b == "He");

		a.assign(b);
		AssertExp(a == "He");

		a.assign(a);
		AssertExp(a == "He");

		std::string t = "Hello, world!";
		a.assign(t);
		AssertExp(a == t);
	}

	void testAppend(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;
		NS_STDEXT::TextPool<char> a(alloc, "Hello");
		a.append(3, '!');
		AssertExp(a == "Hello!!!");

		a.append('!');
		AssertExp(a == "Hello!!!!");

		a.append(' ').append(std::string("world"));
		AssertExp(a == "Hello!!!! world");
	
		NS_STDEXT::TextPool<char> temp(alloc, '!');
		a.append(temp);
		AssertExp(a == "Hello!!!! world!");
		
		a.append(std::vector<char>(2, '!'));
		AssertExp(a == "Hello!!!! world!!!");
	
		a.append(NS_STDEXT::String("!", 1));
		AssertExp(a == "Hello!!!! world!!!!");
	}

	void testReplace(LogT& log)
	{
		NS_STDEXT::DefaultAlloc alloc;

		NS_STDEXT::TextPool<char> a(alloc, "Hello!!!! world! Good!");
		NS_STDEXT::TextPool<char>::iterator it = a.find("!!!!");
		AssertExp(it != a.end());
		AssertExp(*(it-1) == 'o' && *it == '!');

		a.replace(it, it+4, ',');
		AssertExp(a == "Hello, world! Good!");

		it = a.find('!');
		AssertExp(it != a.end());
		AssertExp(*it == '!');

		a.replace(it, it+1, "!!!");
		AssertExp(a == "Hello, world!!! Good!");

		it = a.rfind("Good!");
		AssertExp(*it == 'G');

		a.replace(it, it+5, '#');
		AssertExp(a == "Hello, world!!! #");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_TEXTPOOL_H */

