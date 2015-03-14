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
// Module: stdext/Tuple.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Tuple.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TUPLE_H
#define STDEXT_TUPLE_H

#ifndef STDEXT_STATICALGO_H
#include "StaticAlgo.h"
#endif

#ifndef STDEXT_HASH_H
#include "Hash.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// TupleItemTraits

template <int Index, class TupleT>
struct TupleItemTraits
{
};

template <class FirstT, class SecondT>
struct TupleItemTraits<0, std::pair<FirstT, SecondT> >
{
	typedef FirstT value_type;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;
	
	static reference winx_call get(std::pair<FirstT, SecondT>& o) {
		return o.first;
	}

	static const_reference winx_call get(const std::pair<FirstT, SecondT>& o) {
		return o.first;
	}
};

template <class FirstT, class SecondT>
struct TupleItemTraits<1, std::pair<FirstT, SecondT> >
{
	typedef SecondT value_type;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;

	static reference winx_call get(std::pair<FirstT, SecondT>& o) {
		return o.second;
	}

	static const_reference winx_call get(const std::pair<FirstT, SecondT>& o) {
		return o.second;
	}
};

// -------------------------------------------------------------------------
// TupleTraits

template <class TupleT>
struct TupleTraits
{
};

template <class FirstT, class SecondT>
struct TupleTraits<std::pair<FirstT, SecondT> >
{
	enum { Fields = 2 };
	enum { HasDestructor = 
		DestructorTraits<FirstT>::HasDestructor |
		DestructorTraits<SecondT>::HasDestructor };
};

// -------------------------------------------------------------------------
// class TupleSelectKey

template <class TupleT, int FieldMasks>
class TupleSelectKey : public TupleT
{
private:
	typedef MasksTraits<FieldMasks> Tr_;
	typedef TupleItemTraits<StaticLog2<Tr_::Head>::value, TupleT> ItemTr_;
	typedef TupleSelectKey<TupleT, Tr_::Tail> NextT;

public:
	TupleSelectKey() : TupleT() {}
	TupleSelectKey(const TupleT& t) : TupleT(t) {
	}
	
	size_t winx_call hash_value() const
	{
		Hash<typename ItemTr_::value_type> hashf;
		return hashf(ItemTr_::get(*this)) * 5 + ((const NextT*)this)->hash_value();
	}
	
	bool winx_call operator<(const TupleSelectKey& b) const
	{
		typename ItemTr_::const_reference ia = ItemTr_::get(*this);
		typename ItemTr_::const_reference ib = ItemTr_::get(b);
		if (ia < ib)
			return true;
		else if (ib < ia)
			return false;
		else
			return *(const NextT*)this < *(const NextT*)&b;
	}

	bool winx_call operator>(const TupleSelectKey& b) const
	{
		typename ItemTr_::const_reference ia = ItemTr_::get(*this);
		typename ItemTr_::const_reference ib = ItemTr_::get(b);
		if (ia > ib)
			return true;
		else if (ib > ia)
			return false;
		else
			return *(const NextT*)this > *(const NextT*)&b;
	}

	bool winx_call operator==(const TupleSelectKey& b) const
	{
		typename ItemTr_::const_reference ia = ItemTr_::get(*this);
		typename ItemTr_::const_reference ib = ItemTr_::get(b);
		if (ia == ib)
			return *(const NextT*)this == *(const NextT*)&b;
		else
			return false;
	}
};

template <class TupleT>
class TupleSelectKey<TupleT, 0>
{
public:
	size_t winx_call hash_value() const {
		return 0;
	}
	
	bool winx_call operator<(const TupleSelectKey& b) const {
		return false;
	}

	bool winx_call operator>(const TupleSelectKey& b) const {
		return false;
	}

	bool winx_call operator==(const TupleSelectKey& b) const {
		return true;
	}
};

template <class TupleT, int FieldMasks>
class Hash<TupleSelectKey<TupleT, FieldMasks> >
{
private:
	typedef TupleSelectKey<TupleT, FieldMasks> KeyT;
	
public:
	size_t winx_call operator()(const KeyT& v) const {
		return v.hash_value();
	}
};

// -------------------------------------------------------------------------
// $Log: Tuple.h,v $

NS_STDEXT_END

#ifdef BOOST_TUPLE_HPP
#include "boost/Tuple.h"
#endif

#endif /* STDEXT_TUPLE_H */

