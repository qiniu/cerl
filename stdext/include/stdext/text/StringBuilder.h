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
// Module: stdext/text/StringBuilder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: StringBuilder.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_STRINGBUILDER_H
#define STDEXT_TEXT_STRINGBUILDER_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

#ifndef STDEXT_TEXT_STRINGALGO_H
#include "StringAlgo.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

#define WINX_BSB_USING_VECTOR_												\
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
// class BasicStringBuilder

template <class CharT>
class BasicStringBuilder : public std::vector<CharT>
{
private:
	typedef std::vector<CharT> Base, BaseClass;
	WINX_BSB_USING_VECTOR_;
	
	typedef BasicString<CharT> String_;
	typedef BasicStringBuilder Myt_;

private:
	void operator=(const BasicStringBuilder&);

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

public:
	BasicStringBuilder() {}

	explicit BasicStringBuilder(const BasicStringBuilder& s)
		: Base(s) {}
	explicit BasicStringBuilder(const String_& s)
		: Base(s.begin(), s.end()) {}

	BasicStringBuilder(size_type cch, CharT ch)
		: Base(cch, ch) {}

	BasicStringBuilder(const CharT* s, size_type count) 
		: Base(s, s+count) {} 

	template <class _InputIterator>
	BasicStringBuilder(_InputIterator first, _InputIterator last)
		: Base(first, last) {}

public:
	BasicString<CharT> winx_call cast_str() const {
		return BasicString<CharT>(iterToPointer(begin()), size());
	}

	BasicString<CharT> winx_call cast_substr(
		size_type from = 0, size_type cch = (size_type)-1) const
	{
		size_type cchMax = Base::size() - from;
		if ((difference_type)cchMax < 0)
			throw_out_of_range_();
		
		size_type cchLength = (cchMax < cch ? cchMax : cch);
		return BasicString<CharT>(iterToPointer(Base::begin() + from), cchLength);
	}

public:
	const CharT* winx_call c_str() {
		Base::push_back('\0');
		Base::pop_back();
		return iterToPointer(begin());
	}
	
	template <class AllocT>
	BasicString<CharT> winx_call str(AllocT& alloc) const {
		return BasicString<CharT>(alloc, iterToPointer(begin()), size());
	}

	template <class AllocT>
	BasicString<CharT> winx_call substr(
		AllocT& alloc, size_type from = 0, size_type cch = (size_type)-1) const
	{
		return BasicString<CharT>(alloc, cast_substr(from, cch));
	}

	basic_string<CharT> winx_call stl_str() const
	{
		return basic_string<CharT>(iterToPointer(begin()), size());
	}

	const CharT* winx_call data() const {
		return iterToPointer(begin());
	}

	void winx_call erase() {
		clear();
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
	
	void winx_call copy(const Base& s)
	{
		Base::operator=(s);
	}

public:
	template <class _InputerIterator>
	Myt_& winx_call append(_InputerIterator first, _InputerIterator last)
	{
		Base::insert(end(), first, last);
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

public:
	template <class _RandIterator>
	Myt_& winx_call replace(
		iterator first, iterator last,
		_RandIterator bfirst, _RandIterator blast)
	{
		std::replace(WINX_BASE, first, last, bfirst, blast);
		return *this;
	}

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
	WINX_RFIND_ALL_(find_last_of, std::find, std::find_first_of)
	WINX_FIND_ALL_(find_first_not_of, std::find_not, std::find_first_not_of)
	WINX_RFIND_ALL_(find_last_not_of, std::find_not, std::find_first_not_of)

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

typedef BasicStringBuilder<char> StringBuilder;
typedef BasicStringBuilder<wchar_t> WStringBuilder;

#if defined(WINX_HAS_OSTREAM)

template <class CharT, class Tr>
inline std::basic_ostream<CharT, Tr>& 
winx_call operator<<(std::basic_ostream<CharT, Tr>& os, const std::vector<CharT>& v) {
	std::copy(v.begin(), v.end(), std::ostream_iterator<CharT, CharT, Tr>(os));
	return os;
}

#endif

// -------------------------------------------------------------------------

template <class CharT, class _T2> __forceinline
bool winx_call operator==(const BasicStringBuilder<CharT>& a, const _T2& b)
	{return a.compare(b) == 0; }

template <class CharT, class _T2> __forceinline
bool winx_call operator!=(const BasicStringBuilder<CharT>& a, const _T2& b)
	{return a.compare(b) != 0; }

template <class CharT, class _T2> __forceinline
bool winx_call operator<(const BasicStringBuilder<CharT>& a, const _T2& b)
	{return a.compare(b) < 0; }

template <class CharT, class _T2> __forceinline
bool winx_call operator>(const BasicStringBuilder<CharT>& a, const _T2& b)
	{return a.compare(b) > 0; }

template <class CharT, class _T2> __forceinline
bool winx_call operator<=(const BasicStringBuilder<CharT>& a, const _T2& b)
	{return a.compare(b) <= 0; }

template <class CharT, class _T2> __forceinline
bool winx_call operator>=(const BasicStringBuilder<CharT>& a, const _T2& b)
	{return a.compare(b) >= 0; }

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestStringBuilder

#if defined(STD_UNITTEST)

template <class LogT>
class TestStringBuilder : public TestCase
{
	WINX_TEST_SUITE(TestStringBuilder);
		WINX_TEST(testConstruct);
		WINX_TEST(testSubstr);
		WINX_TEST(testAssign);
		WINX_TEST(testAppend);
		WINX_TEST(testReplace);
	WINX_TEST_SUITE_END();

public:
	void testConstruct(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);

		NS_STDEXT::StringBuilder a1('a');
		AssertExp(a1.size() == 1 && a1[0] == 'a');

		NS_STDEXT::StringBuilder a2(3, 'a');
		AssertExp(a2.cast_str() == "aaa");

		NS_STDEXT::StringBuilder a("Hello");
		AssertExp(a.cast_str() == "Hello");

		NS_STDEXT::StringBuilder b("Hello", 4);
		AssertExp(b == "Hell");

		NS_STDEXT::StringBuilder c(b.begin(), b.end());
		NS_STDEXT::String d = c.str(alloc);
		AssertExp(d == b && b == d);
	}

	void testSubstr(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);

		NS_STDEXT::StringBuilder a("Hello");
		AssertExp(a.cast_substr(1, 3) == "ell");
		AssertExp(a.substr(alloc, 1, 3) == "ell");
		AssertExp(a.substr(alloc, 1) == "ello");
		AssertExp(a.substr(alloc, 5) == "");
	}

	void testAssign(LogT& log)
	{
		NS_STDEXT::StringBuilder a;
		
		a.assign('a');
		AssertExp(a.cast_str() == "a");

		a.assign(3, 'a');
		AssertExp(a.cast_str() == "aaa");

		a.assign("Hello");
		AssertExp(a.cast_str() == "Hello");

		NS_STDEXT::StringBuilder b;
		b.assign("Hello", 2);
		AssertExp(b.cast_str() == "He");

		a.assign(b);
		AssertExp(a.cast_str() == "He");

		a.copy(a);
		AssertExp(a.cast_str() == "He");

		std::string t = "Hello, world!";
		a.assign(t);
		AssertExp(a.cast_str() == t);
	}

	void testAppend(LogT& log)
	{
		NS_STDEXT::StringBuilder a("Hello");
		a.append(3, '!');
		AssertExp(a == "Hello!!!");

		a.append('!');
		AssertExp(a == "Hello!!!!");

		a.append(' ').append(std::string("world"));
		AssertExp(a == "Hello!!!! world");
	
		NS_STDEXT::StringBuilder temp('!');
		a.append(temp);
		AssertExp(a == "Hello!!!! world!");
		
		a.append(std::vector<char>(2, '!'));
		AssertExp(a == "Hello!!!! world!!!");
	
		a.append(NS_STDEXT::String("!", 1));
		AssertExp(a == "Hello!!!! world!!!!");
	}

	void testReplace(LogT& log)
	{
		NS_STDEXT::StringBuilder a("Hello!!!! world! Good!");
		NS_STDEXT::StringBuilder::iterator it = a.find("!!!!");
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

#endif /* STDEXT_TEXT_STRINGBUILDER_H */

