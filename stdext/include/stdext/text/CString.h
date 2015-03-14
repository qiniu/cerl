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
// Module: stdext/text/CString.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-9-6 10:08:57
// 
// $Id: CString.h 2009-9-6 10:08:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_CSTRING_H
#define STDEXT_TEXT_CSTRING_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef STDEXT_TCHAR_H
#include "../tchar.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class ScopedCString

#if (0)

template <class CharT>
class ScopedCString : public BasicString<CharT>
{
private:
	template <class AllocT, class Iterator>
	void winx_call init(AllocT& alloc, Iterator first, size_type cch)
	{
		Base::first = (CharT*)alloc.allocate((cch + 1) * sizeof(CharT));
		Base::second = std::copy(first, first + cch, (CharT*)Base::first);
		*(CharT*)Base::second = CharT();
	}

	template <class AllocT>
	void winx_call init_n(AllocT& alloc, size_type count, CharT ch)
	{
		Base::first = (CharT*)alloc.allocate((count + 1) * sizeof(CharT));
		Base::second = Base::first + count;
		std::fill_n((CharT*)Base::first, count, ch);
		*(CharT*)Base::second = CharT();
	}

public:
	template <class AllocT>
	ScopedCString(AllocT& alloc, const String_& s) {
		init(alloc, s.begin(), s.size());
	}

	template <class AllocT>
	ScopedCString(AllocT& alloc, const value_type* pszVal, size_type cch) {
		init(alloc, pszVal, cch);
	}

	template <class AllocT>
	ScopedCString(AllocT& alloc, size_type count, value_type ch) {
		init_n(alloc, count, ch);
	}

	template <class AllocT, class Iterator>
	ScopedCString(AllocT& alloc, Iterator first, Iterator last) {
		init(alloc, first, last - first);
	}

	template <class AllocT>
	Myt_& winx_call assign(AllocT& alloc, const String_& s) {
		init(alloc, s.begin(), s.size());
		return *this;
	}

	template <class AllocT>
	Myt_& winx_call assign(AllocT& alloc, const CharT* pszVal, size_type cch) {
		init(alloc, pszVal, cch);
		return *this;
	}

	template <class AllocT>
	Myt_& winx_call assign(AllocT& alloc, size_type count, value_type ch) {
		init_n(alloc, count, ch);
		return *this;
	}

	template <class AllocT, class Iterator>
	Myt_& winx_call assign(AllocT& alloc, Iterator first, Iterator last) {
		init(alloc, first, last - first);
		return *this;
	}
};

#endif

// -------------------------------------------------------------------------
// class BasicCString

#pragma pack(1)

template <class CharT, class AllocT = TlsPools>
class BasicCString : public BasicString<CharT>
{
private:
	typedef BasicString<CharT> Base;

	using Base::attach;

public:
	typedef AllocT alloc_type;

	typedef typename Base::size_type size_type;
	typedef typename Base::difference_type difference_type;
	typedef typename Base::value_type value_type;

	typedef typename Base::iterator iterator;
	typedef typename Base::const_iterator const_iterator;
	typedef typename Base::reverse_iterator reverse_iterator;
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

	using Base::size;
	using Base::length;
	using Base::empty;
	using Base::begin;
	using Base::end;
	using Base::rbegin;
	using Base::rend;
	using Base::compare_by;
	using Base::icompare;
	
	using Base::data;
	using Base::at;
	using Base::left;
	using Base::right;
	using Base::substr;
	using Base::find;
	using Base::rfind;
	using Base::find_first_of;
	using Base::find_last_of;
	using Base::find_first_not_of;
	using Base::find_last_not_of;
	using Base::contains;
	using Base::startWith;
	using Base::startWithI;
	using Base::endWith;
	using Base::endWithI;
	using Base::stl_str;
	using Base::operator[];

private:
	typedef std::basic_string<CharT> StlString_;
	typedef BasicString<CharT> String_;
	typedef BasicCString Myt_;
	
	struct StringData
	{
		CharT str[2];
	};

	static StringData s_null;

	static size_type winx_call alloc_size(size_type cch)
	{
		return (cch + 2) * sizeof(CharT);
	}

	template <class Iterator>
	void winx_call init(Iterator first, size_type cch)
	{
		Base::first = (CharT*)AllocT::allocate(alloc_size(cch));
		Base::second = std::copy(first, first + cch, (CharT*)Base::first);
		*(CharT*)Base::second = CharT();
		*((CharT*)Base::second + 1) = CharT(1);
	}

	void winx_call init_n(size_type count, CharT ch)
	{
		Base::first = (CharT*)AllocT::allocate(alloc_size(count));
		Base::second = Base::first + count;
		std::fill_n((CharT*)Base::first, count, ch);
		*(CharT*)Base::second = CharT();
		*((CharT*)Base::second + 1) = CharT(1);
	}

	void winx_call acquire() const
	{
		CharT& ref = *((CharT*)Base::second + 1);
		if (++ref == 0)
		{
			if (Base::first != s_null.str)
			{
				--ref;
				((Myt_*)this)->init(Base::first, Base::second - Base::first);
			}
		}
	}

	void winx_call release()
	{
		CharT& ref = *((CharT*)Base::second + 1);
		if (--ref == 0)
		{
			if (Base::first != s_null.str)
			{
				AllocT::deallocate((void*)Base::first, alloc_size(Base::second - Base::first));
				Base::assign(s_null.str, s_null.str);
			}
		}
	}

public:
	BasicCString()
		: Base(s_null.str, s_null.str) {
	}
	
	BasicCString(const BasicCString& s)
		: Base(s) {
		s.acquire();
	}

	explicit BasicCString(const String_& s) {
		init(s.begin(), s.size());
	}

	BasicCString(const value_type* pszVal, size_type cch) {
		init(pszVal, cch);
	}

	BasicCString(size_type count, value_type ch) {
		init_n(count, ch);
	}

	template <class Iterator>
	BasicCString(Iterator first, Iterator last) {
		init(first, last - first);
	}

	~BasicCString() {
		release();
	}

public:
	Myt_& winx_call operator=(const Myt_& s) {
		s.acquire();
		release();
		Base::assign(s);
		return *this;
	}

	Myt_& winx_call assign(const Myt_& s) {
		return *this = s;
	}

	Myt_& winx_call assign(const String_& s) {
		release();
		init(s.begin(), s.size());
		return *this;
	}

	Myt_& winx_call assign(const CharT* pszVal, size_type cch) {
		release();
		init(pszVal, cch);
		return *this;
	}

	Myt_& winx_call assign(size_type count, value_type ch) {
		release();
		init_n(count, ch);
		return *this;
	}

	template <class Iterator>
	Myt_& winx_call assign(Iterator first, Iterator last) {
		release();
		init(first, last - first);
		return *this;
	}

public:
	template <class ContainerT>
	int winx_call compare(const ContainerT& b) const {
		return NS_STDEXT::compare(Base::first, Base::second, b.begin(), b.end());
	}

	int winx_call compare(const Myt_& b) const {
		return tchar::strcmp(Base::first, b.first);
	}

	int winx_call compare(const value_type* b) const {
		return tchar::strcmp(Base::first, b);
	}

	int winx_call compare(const value_type* b, size_type blen) const {
		return NS_STDEXT::compare(Base::first, Base::second, b, b + blen);
	}

public:
	operator const CharT*() const {
		return Base::first;
	}

	const BasicString<CharT>& winx_call str() const {
		return *this;
	}

	const CharT* winx_call c_str() const {
		return Base::first;
	}

	void winx_call swap(BasicCString& b) {
		std::swap(Base::first, b.first);
		std::swap(Base::second, b.second);
	}
};

template <class CharT, class AllocT>
typename BasicCString<CharT, AllocT>::StringData BasicCString<CharT, AllocT>::s_null;

typedef BasicCString<char> CString;
typedef BasicCString<wchar_t> WCString;

#pragma pack()

// -------------------------------------------------------------------------

#define WINX_CSTRING_PRED_OP_(op)											\
																			\
template <class CharT, class AllocT, class T2> __forceinline				\
	bool winx_call operator op(const BasicCString<CharT, AllocT>& a, const T2& b) \
    {return (a.compare(b) op 0); }											\
																			\
template <class CharT, class AllocT> __forceinline							\
    bool winx_call operator op(const CharT* a, const BasicCString<CharT, AllocT>& b) \
    {return (b.compare(a) op 0); }											\
																			\
template <class CharT, class Tr, class AllocT, class AllocT2> inline		\
    bool winx_call operator op(const std::basic_string<CharT, Tr, AllocT>& a, \
							   const BasicCString<CharT, AllocT2>& b)		\
    {return (b.compare(a) op 0); }											\
																			\
template <class CharT, class AllocT, class AllocT2> inline					\
    bool winx_call operator op(const std::vector<CharT, AllocT>& a,			\
							   const BasicCString<CharT, AllocT2>& b)		\
    {return (b.compare(a) op 0); }

WINX_CSTRING_PRED_OP_(==)
WINX_CSTRING_PRED_OP_(!=)
WINX_CSTRING_PRED_OP_(<=)
WINX_CSTRING_PRED_OP_(<)
WINX_CSTRING_PRED_OP_(>=)
WINX_CSTRING_PRED_OP_(>)

// -------------------------------------------------------------------------

NS_STDEXT_END

// -------------------------------------------------------------------------

#if defined(X_CC_VC6)

#ifndef STD_ITERATOR_H
#include "../../std/iterator.h"
#endif

namespace std {
	
template <>
struct iterator_traits_alter<NS_STDEXT::CString*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::CString			value_type;
	typedef ptrdiff_t					difference_type;
	typedef NS_STDEXT::CString*			pointer;
	typedef NS_STDEXT::CString&			reference;
};

template <>
struct iterator_traits_alter<const NS_STDEXT::CString*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::CString			value_type;
	typedef ptrdiff_t					difference_type;
	typedef const NS_STDEXT::CString*	pointer;
	typedef const NS_STDEXT::CString&	reference;
};

template <>
struct iterator_traits_alter<NS_STDEXT::WCString*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::WCString			value_type;
	typedef ptrdiff_t					difference_type;
	typedef NS_STDEXT::WCString*		pointer;
	typedef NS_STDEXT::WCString&		reference;
};

template <>
struct iterator_traits_alter<const NS_STDEXT::WCString*> {
	typedef random_access_iterator_tag	iterator_category;
	typedef NS_STDEXT::WCString			value_type;
	typedef ptrdiff_t					difference_type;
	typedef const NS_STDEXT::WCString*	pointer;
	typedef const NS_STDEXT::WCString&	reference;
};

} // namespace std

#endif

#endif /* STDEXT_TEXT_CSTRING_H */
