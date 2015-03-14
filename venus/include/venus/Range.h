/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Range.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Range.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef VENUS_RANGE_H
#define VENUS_RANGE_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_RANGE_H
#include "../../../stdext/include/stdext/text/Range.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../../../stdext/include/stdext/text/BasicString.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class String

typedef NS_STDEXT::String String;

using NS_STDEXT::g_str;
using NS_STDEXT::arrayof;

// =========================================================================
// class Range, BasicArray, BasicString

#if !defined(X_CC_VC6)

using NS_STDEXT::Range;
using NS_STDEXT::BasicArray;
using NS_STDEXT::BasicString;

#else

//
// class Range
//
template <
	class Iterator,
	class ValueT = WINX_RG_TYPENAME_ std::iterator_traits_alter<Iterator>::value_type>
class Range : public NS_STDEXT::Range<Iterator, ValueT>
{
private:
	typedef NS_STDEXT::Range<Iterator, ValueT> Base;

public:
	Range() {}
	Range(Iterator first_, Iterator second_)
		: Base(first_, second_) {}
	
	template <class ContainerT>
	explicit Range(const ContainerT& cont)
		: Base(cont) {
	}
	
	template <class ContainerT>
	explicit Range(ContainerT& cont)
		: Base(cont) {
	}
};

//
// class BasicArray
//
template <class Type>
class BasicArray : public NS_STDEXT::BasicArray<Type>
{
private:
	typedef NS_STDEXT::BasicArray<Type> Base;
	typedef typename Base::iterator Iterator;
	
public:
	typedef typename Base::size_type size_type;
	
public:
	BasicArray() {}
	BasicArray(const Base& inst)
		: Base(inst) {}
	BasicArray(Iterator arr, size_type n)
		: Base(arr, arr+n) {}
	BasicArray(Iterator first_, Iterator second_)
		: Base(first_, second_) {}
	
	template <class AllocT, class Iterator2>
	BasicArray(AllocT& alloc_, Iterator2 first_, Iterator2 second_)
		: Base(alloc_, first_, second_) {
	}
	
	template <WINX_ALLOC_TEMPLATE_ARGS_, class ContainerT>
	BasicArray(WINX_ALLOC_TYPE_& alloc, const ContainerT& cont)
		: Base(alloc, cont) {
	}
	
	template <class ContainerT>
	BasicArray(SystemAlloc& alloc, const ContainerT& cont)
		: Base(alloc, cont) {
	}
};

//
// class BasicString
//
template <class CharT>
class BasicString : public NS_STDEXT::BasicString<CharT>
{
private:
	typedef NS_STDEXT::BasicString<CharT> Base;

public:
	typedef typename Base::size_type size_type;

public:
	BasicString() {}
	BasicString(const CharT* pszVal, size_type cch)
		: Base(pszVal, pszVal + cch) {
	}
	BasicString(const CharT* first, const CharT* last)
		: Base(first, last) {
	}
	BasicString(const NS_STDEXT::BasicArray<CharT>& s)
		: Base(s) {
	}
	BasicString(const NS_STDEXT::BasicString<CharT>& s)
		: Base(s) {
	}
	
	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, const String_& s)
		: Base(alloc, s) {
	}
	
	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, const value_type* pszVal, size_type cch)
		: Base(alloc, pszVal, cch) {
	}
	
	template <WINX_ALLOC_TEMPLATE_ARGS_>
	BasicString(WINX_ALLOC_TYPE_& alloc, size_type count, value_type ch)
		: Base(alloc, count, ch) {
	}
	
	template <WINX_ALLOC_TEMPLATE_ARGS_, class Iterator>
	BasicString(WINX_ALLOC_TYPE_& alloc, Iterator first, Iterator last)
		: Base(alloc, first, last) {
	}
};

#endif

// =========================================================================
// $Log: $

NS_CERL_END

#endif /* VENUS_RANGE_H */
