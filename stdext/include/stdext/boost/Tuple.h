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
// Module: stdext/boost/Tuple.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 15:43:17
// 
// $Id: Tuple.h 602 2008-05-29 05:21:08Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_BOOST_TUPLE_H
#define STDEXT_BOOST_TUPLE_H

#ifndef BOOST_TUPLE_HPP
#include <boost/tuple/tuple.hpp>
#endif

#ifndef STDEXT_TUPLE_H
#include "../Tuple.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// TupleItemTraits

template <
  int Index,
  class T0, class T1, class T2,
  class T3, class T4, class T5,
  class T6, class T7, class T8,
  class T9>
struct TupleItemTraits<Index, boost::tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >
{
	typedef boost::tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> TupleT;
	typedef typename boost::tuples::element<Index, TupleT>::type value_type;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;
	
	static reference winx_call get(TupleT& o) {
		return boost::tuples::template get<Index>(o);
	}

	static const_reference winx_call get(const TupleT& o) {
		return boost::tuples::template get<Index>(o);
	}
};

// -------------------------------------------------------------------------
// TupleTraits

template <class T>
struct TupleDestructorTraits  {
	enum { HasDestructor = 
		DestructorTraits<typename T::head_type>::HasDestructor |
		TupleDestructorTraits<typename T::tail_type>::HasDestructor };
};

template<>
struct TupleDestructorTraits<boost::tuples::tuple<> > {
	enum { HasDestructor = 0 };
};

template<>
struct TupleDestructorTraits<boost::tuples::null_type> {
	enum { HasDestructor = 0 };
};

template <
  class T0, class T1, class T2,
  class T3, class T4, class T5,
  class T6, class T7, class T8,
  class T9>
struct TupleTraits<boost::tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> >
{
	typedef boost::tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> TupleT;
	enum { Fields = boost::tuples::length<TupleT>::value };
	enum { HasDestructor = TupleDestructorTraits<TupleT>::HasDestructor };
};

// -------------------------------------------------------------------------
// $Log: Tuple.h,v $

NS_STDEXT_END

#endif /* STDEXT_BOOST_TUPLE_H */

