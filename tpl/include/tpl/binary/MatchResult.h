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
// Module: tpl/binary/MatchResult.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: MatchResult.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_MATCHRESULT_H
#define TPL_BINARY_MATCHRESULT_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_STLHEADERS_H
#include "../regex/STLHeaders.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class Arr

// function _get_value

#if defined(TPL_BYTESWAP)
#error "todo"
#else

template <class TypeT, class Iterator>
__forceinline TypeT TPL_CALL _get_value(Iterator it) {
	char val[sizeof(TypeT)];
	for (size_t i = 0; i < sizeof(TypeT); ++i)
		val[i] = *it++;
	return *(const TypeT*)val;
}

template <class TypeT>
__forceinline TypeT TPL_CALL _get_value(const char* it) {
	TypeT val;
	memcpy(&val, it, sizeof(TypeT));
	return val;
}

#endif

// class Arr

template <class TypeT, class Iterator>
class Arr
{
private:
	Iterator m_arr;
	size_t m_count;

public:
	typedef TypeT value_type;
	typedef size_t size_type;

public:
	Arr() : m_count(0) {}
	Arr(Iterator arr_, size_t count_) : m_arr(arr_), m_count(count_) {}

	size_type TPL_CALL size() const {
		return m_count;
	}

#if defined(TPL_BYTESWAP)
#error "todo"
#else
	value_type TPL_CALL operator[](size_type i) const {
		Iterator it = m_arr;
		std::advance(it, sizeof(TypeT) * i);
		return _get_value<TypeT>(it);
	}
#endif
};

#if defined(TPL_HAS_OSTREAM)

template <class CharT, class Tr, class TypeT, class Iterator>
inline std::basic_ostream<CharT, Tr>& 
TPL_CALL operator<<(std::basic_ostream<CharT, Tr>& os, const Arr<TypeT, Iterator>& v) {
	for (size_t i = 0; i < v.size(); ++i)
		os << v[i] << '\t';
	return os;
}

#endif

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_MATCHRESULT_H */
