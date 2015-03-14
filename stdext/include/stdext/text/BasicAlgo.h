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
// Module: stdext/text/BasicAlgo.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: BasicString.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_BASICALGO_H
#define STDEXT_TEXT_BASICALGO_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../CharType.h"
#endif

#if defined(X_STL_SGI)
	#ifndef __SGI_STL_ALGORITHM
	#include "../../../../stl/algorithm"
	#endif
#else
	#ifndef _ALGORITHM_
	#include <algorithm>
	#endif
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// end, length

template <class InputIt>
inline InputIt winx_call end(InputIt it)
{
	while (*it)
		++it;
	return it;
}

template <class InputIt>
inline size_t winx_call length(InputIt it)
{
	InputIt first = it;
	while (*it)
		++it;
	return std::distance(first, it);
}

// -------------------------------------------------------------------------
// find_first_of, find_first_not_of

template <class InputIt, class Ty>
inline InputIt winx_call find_first_of(
	InputIt first, InputIt last, const Ty* cset)
{
	const Ty* str;
	for (; first != last; ++first)
	{
		for (str = cset; *str; ++str)
		{
			if (*str == *first)
				return first;
		}
	}
	return first;
}

template <class InputIt, class Ty>
inline InputIt winx_call find_first_not_of(
	InputIt first, InputIt last, const Ty* cset)
{
	const Ty* str;
	for (; first != last; ++first)
	{
		for (str = cset; *str; ++str)
		{
			if (*str == *first)
				break;
		}
		if (!*str)
			break;
	}
	return first;
}

template <class InputIt, class ForwardIt>
inline InputIt winx_call find_first_not_of(
	InputIt first, InputIt last, ForwardIt set_first, ForwardIt set_last)
{
	ForwardIt set_it;
	for (; first != last; ++first)
	{
		for (set_it = set_first; set_it != set_last; ++set_it)
		{
			if (*set_it == *first)
				break;
		}
		if (set_it == set_last)
			break;
	}
	return first;
}

template <class InputIt, class _ValT>
inline InputIt winx_call find_not(
	InputIt first, InputIt last, const _ValT& val)
{
	for (; first != last; ++first)
	{
		if (*first != val)
			break;
	}
	return first;
}

template <class _BidiIt, class _ValT>
inline _BidiIt winx_call rfind(
	_BidiIt first, _BidiIt last, const _ValT& val)
{
	_BidiIt last0 = last;
	while (first != last)
	{
		if (*--last == val)
			return last;
	}
	return last0;
}

// -------------------------------------------------------------------------
// search

template <class ForwardIt, class Ty>
inline ForwardIt winx_call search(
	ForwardIt first, ForwardIt last, const Ty* pattern)
{
	return std::search(
		first, last,
		pattern, end(pattern)
		);
}

// -------------------------------------------------------------------------
// match, match_symbol, match_csymbol

template<class InputIt1, class InputIt2>
inline InputIt1 winx_call match(
	InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	InputIt1 first = first1;
	for (;; ++first1, ++first2)
	{
		if (first2 == last2)
			return first1;
		if (first1 == last1)
			return first;
		if (*first1 != *first2)
			return first;
	}
}

template<class InputIt, class Ty>
inline InputIt winx_call match(
	InputIt first, InputIt last, const Ty* str)
{
	InputIt first1 = first;
	for (;; ++first, ++str)
	{
		if (!*str)
			return first;
		if (first == last)
			return first1;
		if (*first != *str)
			return first1;
	}
}

template <class InputIt, class ForwardIt>
inline InputIt winx_call match_symbol(
	InputIt first, InputIt last,
	ForwardIt set_first, ForwardIt set_last1, ForwardIt set_last2)
{
	if (first == last)
		return first;

	if (std::find(set_first, set_last1, *first) == set_last1)
		return first;

	return find_first_not_of(++first, last, set_first, set_last2);
}

template <class InputIt>
inline InputIt winx_call match_csymbol(
	InputIt first, InputIt last)
{
	if (first == last)
		return first;

	if (!CharType::isCSymbolFirstChar(*first))
		return first;

	return std::find_if(++first, last, CharType::NotIsCSymbolNextChar());
}

// -------------------------------------------------------------------------
// find_not_csymbol, find_digit_or_csymbol

template <class InputIt>
inline InputIt winx_call find_not_csymbol(
	InputIt first, InputIt last)
{
	return std::find_if(first, last, CharType::NotIsCSymbolNextChar());
}

template <class InputIt>
inline InputIt winx_call find_digit_or_csymbol(
	InputIt first, InputIt last)
{
	return std::find_if(first, last, CharType::IsCSymbolNextChar());
}

// -------------------------------------------------------------------------
// compare_by

template<class InputIt1, class InputIt2, class ComprT>
inline int winx_call compare_by(
	InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, ComprT cmp)
{
	for (;; ++first1, ++first2)
	{
		if (first1 == last1)
			return first2 == last2 ? 0 : -1;
		if (first2 == last2)
			return 1;
		int res = cmp(*first1, *first2);
		if (res != 0)
			return res;
	}
}

template<class InputIt, class Ty, class ComprT>
inline int winx_call compare_by(
	InputIt first, InputIt last, const Ty* str, ComprT cmp)
{
	for (;; ++first, ++str)
	{
		if (first == last)
			return *str == 0 ? 0 : -1;
		int res = cmp(*first, *str);
		if (res != 0)
			return res;
		if (*str == 0)
			return 1;
	}
}

// -------------------------------------------------------------------------
// compare

__forceinline int winx_call compare(char a, char b) {
	return (unsigned char)a - (unsigned char)b;
}

__forceinline int winx_call compare(wchar_t a, wchar_t b) {
	return a - b;
}

__forceinline int winx_call compare(const char* src, const char* dst) {
	return strcmp(src, dst);
}

__forceinline int winx_call compare(const wchar_t* src, const wchar_t* dst) {
	return wcscmp(src, dst);
}

template<class InputIt1, class InputIt2>
inline int winx_call compare(
	InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	for (;; ++first1, ++first2)
	{
		if (first1 == last1)
			return first2 == last2 ? 0 : -1;
		if (first2 == last2)
			return 1;
		if (*first1 != *first2)
			return compare(*first1, *first2);
	}
}

template<class InputIt, class Ty>
inline int winx_call compare(
	InputIt first, InputIt last, const Ty* str)
{
	for (;; ++first, ++str)
	{
		if (first == last)
			return *str == 0 ? 0 : -1;
		if (*first != *str)
			return compare(*first, *str);
		if (*str == 0)
			return 1;
	}
}

// -------------------------------------------------------------------------
// replace

template <class Container, class RandIterator>
inline void winx_call replace(
	Container& container,
	typename Container::iterator first,
	typename Container::iterator last,
	RandIterator bfirst, RandIterator blast)
{
	typedef typename Container::size_type size_type;
	typedef typename Container::difference_type difference_type;

	const size_type cchDelete = last - first;
	const size_type cchInsert = blast - bfirst;
	const difference_type delta = cchInsert - cchDelete;
	if (delta > 0)
	{
		std::copy(bfirst, bfirst + cchDelete, first);
		container.insert(last, bfirst + cchDelete, blast);
	}
	else
	{
		std::copy(bfirst, blast, first);
		if (delta)
			container.erase(last + delta, last);
	}
}

template <class Container>
inline void winx_call replace(
	Container& container,
	typename Container::iterator first,
	typename Container::iterator last,
	typename Container::size_type count,
	const typename Container::value_type& val)
{
	typedef typename Container::size_type size_type;
	typedef typename Container::difference_type difference_type;

	const size_type cchDelete = last - first;
	const difference_type delta = count - cchDelete;
	if (delta > 0)
	{
		std::fill_n(first, cchDelete, val);
		container.insert(last, delta, val);
	}
	else
	{
		std::fill_n(first, count, val);
		if (delta)
			container.erase(last + delta, last);
	}
}

// -------------------------------------------------------------------------
// strecpy

template <class xchar>
inline xchar* winx_call strecpy(xchar* pszDest, const xchar* pszSrc)
{
	while ((*pszDest = *pszSrc) != 0)
	{
		++pszDest;
		++pszSrc;
	}
	return pszDest;
}

namespace tchar { using NS_STDEXT::strecpy; }

// -------------------------------------------------------------------------
// strdup

#if defined(X_CC_VC_NET)
#pragma push_macro("strdup")
#undef strdup
#endif

template <class AllocT, class CharT>
inline CharT* winx_call strdup(AllocT& alloc, const CharT* s)
{
	size_t len = length(s) + 1;
	CharT* s2 = (CharT*)alloc.allocate(sizeof(CharT) * len);
	std::copy(s, s+len, s2);
	return s2;
}

#if defined(X_CC_VC_NET)
#pragma pop_macro("strdup")
#endif

// -------------------------------------------------------------------------
// class StringEqualTo, StringLess

template <class CharT>
class StringEqualTo
{
public:
	bool winx_call operator()(const CharT* a, const CharT* b) const {
		return compare(a, b) == 0;
	}
};

template <class CharT>
class StringLess
{
public:
	bool winx_call operator()(const CharT* a, const CharT* b) const {
		return compare(a, b) < 0;
	}
};

// -------------------------------------------------------------------------
// $Log: BasicAlgo.h,v $

NS_STDEXT_END

#endif /* STDEXT_TEXT_BASICALGO_H */

