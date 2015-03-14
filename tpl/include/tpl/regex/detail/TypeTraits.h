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
// Module: tpl/regex/detail/TypeTraits.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: TypeTraits.h 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_DETAIL_TYPETRAITS_H
#define TPL_REGEX_DETAIL_TYPETRAITS_H

#ifndef TPL_BASIC_H
#include "../Basic.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class ArrayTypeTraits

template <class ValueT>
struct ArrayTypeTraits
{
	typedef ValueT type;
	typedef ValueT const_type;
	typedef ValueT& reference;
	typedef const ValueT& const_reference;
};

template <class ValueT, size_t n>
struct ArrayTypeTraits<ValueT[n]>
{
	typedef ValueT* type;
	typedef const ValueT* const_type;
	typedef ValueT* reference;
	typedef const ValueT* const_reference;
};

template <class ValueT, size_t n>
struct ArrayTypeTraits<const ValueT[n]> // it seems that this is impossible (tested in g++)
{
	typedef const ValueT* type;
	typedef const ValueT* const_type;
	typedef const ValueT* reference;
	typedef const ValueT* const_reference;
};

// -------------------------------------------------------------------------
// class LValueTraits

template <class ValueT>
class Var;

template <class ValueT>
struct LValueTraits {
	typedef ValueT type;
	typedef ValueT& reference;
};

template <class ValueT>
struct LValueTraits<Var<ValueT> > {
	typedef ValueT type;
	typedef ValueT& reference;
};

// -------------------------------------------------------------------------
// class SmartRefTraits

template <class ValueT>
struct SmartRefTraits
{
};

template <class ValueT>
struct SmartRefTraits<const Var<ValueT>&>
{
	typedef const ValueT& type;
	typedef const ValueT& const_type;
};

template <class ValueT>
struct SmartRefTraits<Var<ValueT>&>
{
	typedef ValueT& type;
	typedef const ValueT& const_type;
};

template <class ValueT>
struct SmartRefTraits<const ValueT&>
{
	typedef ArrayTypeTraits<ValueT> Tr_;
	typedef typename Tr_::type type;
	typedef typename Tr_::const_type const_type;
};

template <class ValueT>
struct SmartRefTraits<ValueT&>
{
	typedef ArrayTypeTraits<ValueT> Tr_;
	typedef typename Tr_::reference type;
	typedef typename Tr_::const_reference const_type;
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_DETAIL_TYPETRAITS_H */

