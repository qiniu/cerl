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
// Module: stdext/text/concat/algo.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: algo.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_CONCAT_ALGO_H
#define STDEXT_TEXT_CONCAT_ALGO_H

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// concat

template <class ContainerContT>
struct ConcatTraits
{
	typedef typename ContainerContT::value_type container_type;
	typedef typename container_type::value_type char_type;
	typedef BasicString<char_type> return_type;
};

template <class AllocT, class Iterator>
typename ConcatTraits<iterator_traits_alter<Iterator> >::return_type
winx_call concat(AllocT& alloc, const Iterator first, const Iterator second)
{
	typedef ConcatTraits<iterator_traits_alter<Iterator> > Traits;
	typedef typename Traits::container_type ContainerT;
	typedef typename Traits::char_type ValueT;
	typedef typename Traits::return_type RetType;

	Iterator it;
	size_t len = 0;
	for (it = first; it != second; ++it)
	{
		len += (*it).size();
	}

	ValueT* buf = STD_NEW_ARRAY(alloc, ValueT, len);
	for (it = first; it != second; ++it)
	{
		const ContainerT& cont = *it;
		buf = std::copy(cont.begin(), cont.end(), buf);
	}

	return RetType(buf-len, buf);
}

template <class AllocT, class ContainerT>
__forceinline
typename ConcatTraits<ContainerT>::return_type
winx_call concat(AllocT& alloc, const ContainerT& cont) {
	return concat(alloc, cont.begin(), cont.end());
}

// -------------------------------------------------------------------------

template <class AllocT, class CharT>
BasicString<CharT> winx_call concat(
	AllocT& alloc, const BasicString<CharT>* vals[], size_t count)
{
	size_t i, len = 0;
	for (i = 0; i < count; ++i)
		len += vals[i]->size();

	CharT* buf = STD_NEW_ARRAY(alloc, CharT, len);
	for (i = 0; i < count; ++i)
		buf = std::copy(vals[i]->begin(), vals[i]->end(), buf);

	return BasicString<CharT>(buf-len, len);
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2)
{
	const String* vals[] = { &v1, &v2 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2)
{
	const WString* vals[] = { &v1, &v2 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2, const String& v3)
{
	const String* vals[] = { &v1, &v2, &v3 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2, const WString& v3)
{
	const WString* vals[] = { &v1, &v2, &v3 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2, const String& v3, const String& v4)
{
	const String* vals[] = { &v1, &v2, &v3, &v4 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2, const WString& v3, const WString& v4)
{
	const WString* vals[] = { &v1, &v2, &v3, &v4 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2, const String& v3, const String& v4, const String& v5)
{
	const String* vals[] = { &v1, &v2, &v3, &v4, &v5 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2, const WString& v3, const WString& v4, const WString& v5)
{
	const WString* vals[] = { &v1, &v2, &v3, &v4, &v5 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2,
	const String& v3, const String& v4, const String& v5, const String& v6)
{
	const String* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2,
	const WString& v3, const WString& v4, const WString& v5, const WString& v6)
{
	const WString* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2, const String& v3, const String& v4,
	const String& v5, const String& v6, const String& v7)
{
	const String* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2, const WString& v3, const WString& v4,
	const WString& v5, const WString& v6, const WString& v7)
{
	const WString* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2, const String& v3, const String& v4,
	const String& v5, const String& v6, const String& v7, const String& v8)
{
	const String* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2, const WString& v3, const WString& v4,
	const WString& v5, const WString& v6, const WString& v7, const WString& v8)
{
	const WString* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline String winx_call concat(
	AllocT& alloc, const String& v1, const String& v2, const String& v3, const String& v4,
	const String& v5, const String& v6, const String& v7, const String& v8, const String& v9)
{
	const String* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9 };
	return concat(alloc, vals, countof(vals));
}

template <class AllocT>
inline WString winx_call concat(
	AllocT& alloc, const WString& v1, const WString& v2, const WString& v3, const WString& v4,
	const WString& v5, const WString& v6, const WString& v7, const WString& v8, const WString& v9)
{
	const WString* vals[] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9 };
	return concat(alloc, vals, countof(vals));
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_CONCAT_ALGO_H */

