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
// Module: sgi/gcc/iterator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: iterator.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_GCC_ITERATOR_H__
#define __SGI_GCC_ITERATOR_H__

#if !defined(X_STL_GCC)
#error "only for compiling sgi-stl for gcc"
#endif

// -------------------------------------------------------------------------

#if !defined(_ITERATOR)
#include <iterator>
#endif

#ifndef __SGI_STL_ITERATOR
	#define __SGI_STL_ITERATOR
#endif

#ifndef __SGI_STL_ITERATOR_H
	#define __SGI_STL_ITERATOR_H
#endif

#ifndef __SGI_STL_INTERNAL_ITERATOR_BASE_H
	#define __SGI_STL_INTERNAL_ITERATOR_BASE_H
#endif

#ifndef __SGI_STL_INTERNAL_ITERATOR_H
	#define __SGI_STL_INTERNAL_ITERATOR_H
#endif

#ifndef __SGI_STL_INTERNAL_RAW_STORAGE_ITERATOR_H
	#define __SGI_STL_INTERNAL_RAW_STORAGE_ITERATOR_H
#endif

// -------------------------------------------------------------------------

namespace std {

// The base classes input_iterator, output_iterator, forward_iterator,
// bidirectional_iterator, and random_access_iterator are not part of
// the C++ standard.  (They have been replaced by struct iterator.)
// They are included for backward compatibility with the HP STL.

template <class _Tp, class _Distance> struct input_iterator {
  typedef input_iterator_tag iterator_category;
  typedef _Tp                value_type;
  typedef _Distance          difference_type;
  typedef _Tp*               pointer;
  typedef _Tp&               reference;
};

struct output_iterator {
  typedef output_iterator_tag iterator_category;
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;
};

template <class _Tp, class _Distance> struct forward_iterator {
  typedef forward_iterator_tag iterator_category;
  typedef _Tp                  value_type;
  typedef _Distance            difference_type;
  typedef _Tp*                 pointer;
  typedef _Tp&                 reference;
};


template <class _Tp, class _Distance> struct bidirectional_iterator {
  typedef bidirectional_iterator_tag iterator_category;
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Tp&                       reference;
};

template <class _Tp, class _Distance> struct random_access_iterator {
  typedef random_access_iterator_tag iterator_category;
  typedef _Tp                        value_type;
  typedef _Distance                  difference_type;
  typedef _Tp*                       pointer;
  typedef _Tp&                       reference;
};

}; // namespace std

// -------------------------------------------------------------------------

namespace __STD
{
	using std::input_iterator;
	using std::input_iterator_tag;
	using std::output_iterator;
	using std::output_iterator_tag;
	using std::forward_iterator;
	using std::forward_iterator_tag;
	using std::bidirectional_iterator;
	using std::bidirectional_iterator_tag;
	using std::random_access_iterator;
	using std::random_access_iterator_tag;
	using std::reverse_iterator;
};

// -------------------------------------------------------------------------
// $Log: iterator.h,v $

#endif /* __SGI_GCC_ITERATOR_H__ */
