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
// Module: stdext/text/BasicString.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: BasicString.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_BASICSTRING_H
#define STDEXT_TEXT_BASICSTRING_H

#ifndef STDEXT_TEXT_RANGE_H
#include "Range.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// WINX_FIND_OPS_

#define WINX_FIND_OPS_(It, findOp, findC, findS, Bgn, End, Const)			\
																			\
	It winx_call findOp(const CharT ch, It from) Const						\
		{return findC(from, End, ch); }										\
																			\
	It winx_call findOp(const CharT ch) Const								\
		{return findC(Bgn, End, ch); }										\
																			\
	It winx_call findOp(const String_& pattern, It from) Const				\
		{return findS(from, End, pattern.begin(), pattern.end()); }			\
																			\
	It winx_call findOp(const String_& pattern) Const						\
		{return findS(Bgn, End, pattern.begin(), pattern.end()); }			\
																			\
	It winx_call findOp(const CharT* pattern, size_type len, It from) Const	\
		{return findS(from, End, pattern, pattern + len); }					\
																			\
	It winx_call findOp(const CharT* pattern, size_type len) Const			\
		{return findS(Bgn, End, pattern, pattern + len); }

#define WINX_FIND_CONST_(findOp, findC, findS)								\
	WINX_FIND_OPS_(const_iterator, findOp, findC, findS, begin(), end(), const)

#define WINX_FIND_NONCONST_(findOp, findC, findS)							\
	WINX_FIND_OPS_(iterator, findOp, findC, findS, begin(), end(), WINX_NOTHING_)

#define WINX_RFIND_CONST_(findOp, findC, findS)								\
	WINX_FIND_OPS_(const_reverse_iterator, findOp, findC, findS, rbegin(), rend(), const)

#define WINX_RFIND_NONCONST_(findOp, findC, findS)							\
	WINX_FIND_OPS_(reverse_iterator, findOp, findC, findS, rbegin(), rend(), WINX_NOTHING_)

#define WINX_NOTHING_	/* nothing */

#define WINX_FIND_ALL_(findOp, findC, findS)								\
	WINX_FIND_CONST_(findOp, findC, findS)									\
	WINX_FIND_NONCONST_(findOp, findC, findS)	

#define WINX_RFIND_ALL_(findOp, findC, findS)								\
	WINX_RFIND_CONST_(findOp, findC, findS)									\
	WINX_RFIND_NONCONST_(findOp, findC, findS)	

#define WINX_BASICSTRING_USING_												\
public:																		\
	using Base::begin;														\
	using Base::end;														\
	using Base::rbegin;														\
	using Base::rend

// -------------------------------------------------------------------------
// class BasicString

template <class CharT>
class BasicString : public Range<const CharT*, CharT>
{
private:
	typedef Range<const CharT*, CharT> Base;
	WINX_BASICSTRING_USING_;
	
	typedef std::basic_string<CharT> StlString_;
	typedef BasicString String_;
	typedef BasicString Myt_;

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename Base::value_type value_type;

	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

private:
	template <class AllocT, class Iterator>
	void winx_call init(AllocT& alloc, Iterator first, size_type cch)
	{
		Base::first = (CharT*)alloc.allocate(cch * sizeof(CharT));
		Base::second = std::copy(first, first + cch, (CharT*)Base::first);
	}
	
	template <class AllocT>
	void winx_call init(AllocT& alloc, size_type count, CharT ch)
	{
		Base::first = (CharT*)alloc.allocate(count * sizeof(CharT));
		Base::second = Base::first + count;
		std::fill_n((CharT*)Base::first, count, ch);
	}

public:
	BasicString() {}
	BasicString(const CharT& ch)
		: Base(&ch, &ch + 1) {
	}
	BasicString(const Base& s)
		: Base(s.begin(), s.end()) {
	}
	BasicString(const CharT* szVal)
		: Base(szVal, NS_STDEXT::end(szVal)) {
	}

	template <class Tr, class AllocT>
	BasicString(const std::basic_string<CharT, Tr, AllocT>& s)
		: Base(std::iterToPointer(s.begin()), std::iterToPointer(s.end())) {
	}

	template <class AllocT>
	BasicString(const std::vector<CharT, AllocT>& s)
		: Base(std::iterToPointer(s.begin()), std::iterToPointer(s.end())) {
	}

	BasicString(const CharT* pszVal, size_type cch)
		: Base(pszVal, pszVal + cch) {
	}

	BasicString(const CharT* first, const CharT* last)
		: Base(first, last) {
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, const String_& s) {
		init(alloc, s.begin(), s.size());
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, const value_type* pszVal, size_type cch) {
		init(alloc, pszVal, cch);
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, size_type count, value_type ch) {
		init(alloc, count, ch);
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_, class Iterator>
	BasicString(WINX_ALLOC_TYPE_& alloc, Iterator first, Iterator last) {
		init(alloc, first, last - first);
	}

public:
	template <WINX_ALLOC_TEMPLATE_ARGS_>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, const String_& s) {
		init(alloc, s.begin(), s.size());
		return *this;
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, const CharT* pszVal, size_type cch) {
		init(alloc, pszVal, cch);
		return *this;
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, size_type count, value_type ch) {
		init(alloc, count, ch);
		return *this;
	}

	template <WINX_ALLOC_TEMPLATE_ARGS_, class Iterator>
	Myt_& winx_call assign(WINX_ALLOC_TYPE_& alloc, Iterator first, Iterator last) {
		init(alloc, first, last - first);
		return *this;
	}

	Myt_& winx_call assign(const Myt_& s) {
		Base::operator=(s);
		return *this;
	}

	void winx_call assign(const CharT* first_, const CharT* second_) {
		Base::first = first_;
		Base::second = second_;
	}

	void winx_call attach(const CharT* szVal) {
		Base::assign(szVal, NS_STDEXT::end(szVal));
	}

	void winx_call attach(const CharT* szVal, size_type cch) {
		Base::assign(szVal, szVal + cch);
	}

	const Myt_& winx_call operator=(const Range<const CharT*, CharT>& s) {
		Base::operator=(s);
		return *this;
	}

public:
	const CharT* winx_call data() const {
		return Base::first;
	}

	const CharT& winx_call at(size_type i) const {
		if (i <= Base::size())
			throw_out_of_range_();
		return Base::first[i];
	}

	const CharT& winx_call operator[](size_type i) const {
		return Base::first[i];
	}

	operator const BasicArray<CharT>&() const {
		return *(const BasicArray<CharT>*)this;
	}

	template <class _Tr, class _Alloc2>
	static Myt_ winx_call cast(const basic_string<CharT, _Tr, _Alloc2>& src) {
		return Myt_(src.data(), src.length());
	}

	template <class _Alloc2>
	static Myt_ winx_call cast(const vector<CharT, _Alloc2>& builder) {
		return Myt_(iterToPointer(builder.begin()), builder.size());
	}

	StlString_ winx_call stl_str() const {
		return StlString_(Base::first, Base::second);
	}

	Myt_ winx_call left(size_type n) const
	{
		const CharT* last = Base::first + n;
		if (Base::second < last)
			return *this;
		return Myt_(Base::first, last);
	}

	Myt_ winx_call right(size_type n) const
	{
		const CharT* s = Base::second - n;
		if (s < Base::first)
			return *this;
		return Myt_(s, Base::second);
	}

	Myt_ winx_call substr(size_type from) const
	{
		const size_type cch = Base::size() - from;
		if ((difference_type)cch < 0)
			return Myt_();
		return Myt_(Base::first + from, cch);
	}

	Myt_ winx_call substr(size_type from, size_type cch) const
	{
		const size_type cchMax = Base::size() - from;
		if ((difference_type)cchMax < 0)
			return Myt_();
		return Myt_(Base::first + from, cchMax < cch ? cchMax : cch);
	}
	
public:
	WINX_FIND_CONST_(find, std::find, std::search)
	WINX_FIND_CONST_(rfind, std::rfind, std::find_end)

	WINX_FIND_CONST_(find_first_of, std::find, std::find_first_of)
	WINX_RFIND_CONST_(find_last_of, std::find, std::find_first_of)

	WINX_FIND_CONST_(find_first_not_of, std::find_not, std::find_first_not_of)
	WINX_RFIND_CONST_(find_last_not_of, std::find_not, std::find_first_not_of)

public:
	bool winx_call contains(const String_& b) const {
		return find(b) != end();
	}

	bool winx_call contains(const CharT* b, size_type blen) const {
		return find(b, blen) != end();
	}
	
	bool winx_call startWith(const String_& b) const {
		return substr(0, b.size()) == b;
	}
	
	bool winx_call startWithI(const String_& b) const { // startWithIgnoreCase
		return substr(0, b.size()).icompare(b) == 0;
	}

	bool winx_call endWith(const String_& b) const {
		return substr(Base::size() - b.size()) == b;
	}

	bool winx_call endWithI(const String_& b) const { // endWithIgnoreCase
		return substr(Base::size() - b.size()).icompare(b) == 0;
	}
};

typedef BasicString<char> String;
typedef BasicString<wchar_t> WString;

#if defined(UNICODE) || defined(_UNICODE)
typedef WString TString;
#else
typedef String TString;
#endif

#define TempString BasicString

// -------------------------------------------------------------------------

#define WINX_STRING_PRED_OP_(op)											\
																			\
template <class CharT, class T2> __forceinline								\
	bool winx_call operator op(const BasicString<CharT>& a, const T2& b)	\
    {return (a.compare(b) op 0); }											\
																			\
template <class CharT> __forceinline										\
    bool winx_call operator op(const CharT* a, const BasicString<CharT>& b)	\
    {return (b.compare(a) op 0); }											\
																			\
template <class CharT, class Tr, class AllocT> __forceinline				\
    bool winx_call operator op(const std::basic_string<CharT, Tr, AllocT>& a, \
							   const BasicString<CharT>& b)					\
    {return (b.compare(a) op 0); }											\
																			\
template <class CharT, class AllocT> __forceinline							\
    bool winx_call operator op(const std::vector<CharT, AllocT>& a,			\
							   const BasicString<CharT>& b)					\
    {return (b.compare(a) op 0); }

WINX_STRING_PRED_OP_(==)
WINX_STRING_PRED_OP_(!=)
WINX_STRING_PRED_OP_(<=)
WINX_STRING_PRED_OP_(<)
WINX_STRING_PRED_OP_(>=)
WINX_STRING_PRED_OP_(>)

// -------------------------------------------------------------------------

#if defined(WINX_HAS_OSTREAM)

template <class CharT, class Tr>
inline std::basic_ostream<CharT, Tr>& 
winx_call operator<<(std::basic_ostream<CharT, Tr>& os, const BasicString<CharT>& v) {
	std::copy(v.begin(), v.end(), std::ostream_iterator<CharT, CharT, Tr>(os));
	return os;
}

#endif

// -------------------------------------------------------------------------
// class OutputBasicString

template <class DataT, class AllocT>
class OutputBasicString
{
private:
	AllocT& m_alloc;
	BasicString<DataT>& m_str;

public:
	typedef DataT* pointer;
	typedef DataT* iterator;
	typedef DataT value_type;

public:
	OutputBasicString(AllocT& alloc, BasicString<DataT>& s)
		: m_alloc(alloc), m_str(s) {
	}

	void winx_call resize(size_t newSize) {
		m_str.attach(STD_NEW_ARRAY(m_alloc, DataT, newSize), newSize);
	}

	iterator winx_call begin() {
		return (iterator)m_str.data();
	}

	iterator winx_call end() {
		return (iterator)m_str.end();
	}

	iterator winx_call erase(iterator it) {
		std::copy(it + 1, end(), it);
		m_str.attach(m_str.data(), m_str.size() - 1);
		return it;
	}
};

// -------------------------------------------------------------------------
// function g_str

template <class CharT>
inline BasicString<CharT> winx_call g_str(const CharT* s)
{
	return BasicString<CharT>(s, end(s));
}

// -------------------------------------------------------------------------

NS_STDEXT_END

STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<char>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<wchar_t>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<int>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<long>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<short>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<unsigned int>);
STD_NO_DESTRUCTOR(NS_STDEXT::BasicString<unsigned long>);

// -------------------------------------------------------------------------

#if defined(X_CC_VC6)

#ifndef STD_ITERATOR_H
#include "../../std/iterator.h"
#endif

namespace std {

template <>
struct iterator_traits_alter<NS_STDEXT::String*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::String			value_type;
	typedef ptrdiff_t					difference_type;
	typedef NS_STDEXT::String*			pointer;
	typedef NS_STDEXT::String&			reference;
};

template <>
struct iterator_traits_alter<const NS_STDEXT::String*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::String			value_type;
	typedef ptrdiff_t					difference_type;
	typedef const NS_STDEXT::String*	pointer;
	typedef const NS_STDEXT::String&	reference;
};

template <>
struct iterator_traits_alter<NS_STDEXT::WString*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::WString			value_type;
	typedef ptrdiff_t					difference_type;
	typedef NS_STDEXT::WString*			pointer;
	typedef NS_STDEXT::WString&			reference;
};

template <>
struct iterator_traits_alter<const NS_STDEXT::WString*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::WString			value_type;
	typedef ptrdiff_t					difference_type;
	typedef const NS_STDEXT::WString*	pointer;
	typedef const NS_STDEXT::WString&	reference;
};

} // namespace std

#endif

// -------------------------------------------------------------------------
// class TestBasicString

#if defined(STD_UNITTEST)

template <class LogT>
class TestBasicString : public TestCase
{
	WINX_TEST_SUITE(TestBasicString);
		WINX_TEST(testConstructor);
		WINX_TEST(testFind);
		WINX_TEST(testICompare);
	WINX_TEST_SUITE_END();

public:
	void testFind(LogT& log)
	{
		const NS_STDEXT::String a1("Hello", 5);

		NS_STDEXT::String::iterator it = a1.find("ell");
		AssertExp(it != a1.end());
		AssertExp(*it == 'e');

		it = a1.find('e');
		AssertExp(it != a1.end());
		AssertExp(*it == 'e');

		std::vector<char> pattern(2, 'l');
		it = a1.find(pattern);
		AssertExp(it != a1.end());
		AssertExp(*it == 'l');
		AssertExp(*(it+1) == 'l');
	}

	void testICompare(LogT& log)
	{
		NS_STDEXT::String a("Hello", 5);
		NS_STDEXT::String b("hEllO", 5);
		AssertExp(a.icompare(b) == 0);
	}

	void testConstructor(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);

		const NS_STDEXT::String a1("Hello", 4);
		AssertExp(a1.compare("Hell") == 0);

		NS_STDEXT::String a2 = a1.substr(3);
		AssertExp(a2.compare("l") == 0);

		NS_STDEXT::String a3 = a1.substr(1, 2);
		AssertExp(a3.compare("el") == 0);

		const NS_STDEXT::String a4(alloc, a1);
		AssertExp(a4 == "Hell");
		AssertExp(a1 == a4);

		std::string stl("1234");
		AssertExp(NS_STDEXT::String::cast(stl) == "1234");
		AssertExp(NS_STDEXT::String::cast(stl).size() == 4);

		NS_STDEXT::String a5(alloc, stl);
		AssertExp(a5 == stl && stl == a5);

		NS_STDEXT::String a6 = a5;
		AssertExp(a5.data() == a6.data() && a5.size() == a6.length());
		AssertExp(a6.substr(1) == "234");
		AssertExp(a6.substr(1, 1) == "2");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_BASICSTRING_H */

