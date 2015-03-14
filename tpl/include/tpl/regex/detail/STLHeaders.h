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
// Module: tpl/regex/detail/STLHeaders.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: STLHeaders.h 795 2008-07-04 17:14:19Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_DETAIL_STLHEADERS_H
#define TPL_REGEX_DETAIL_STLHEADERS_H

// -------------------------------------------------------------------------

#if defined(_GLIBCXX_STACK) || defined(_STACK_) || defined(_STACK)
#define TPL_HAS_STACK
#endif

// -------------------------------------------------------------------------
// std::queue

#if defined(_GLIBCXX_QUEUE) || defined(_QUEUE_) || defined(_QUEUE)
#define TPL_HAS_QUEUE
#endif

// -------------------------------------------------------------------------
// std::deque

#if defined(_GLIBCXX_DEQUE) || defined(_DEQUE_) || defined(_DEQUE)
#define TPL_HAS_DEQUE
#endif

// -------------------------------------------------------------------------
// std::vector

#if defined(_GLIBCXX_VECTOR) || defined(_VECTOR_) || defined(_VECTOR)
#define TPL_HAS_VECTOR
#endif

// -------------------------------------------------------------------------
// std::list

#if defined(_GLIBCXX_LIST) || defined(_LIST_) || defined(_LIST)
#define TPL_HAS_LIST
#endif

// -------------------------------------------------------------------------
// std::set, std::multiset

#if defined(_GLIBCXX_SET) || defined(_SET_) || defined(_SET)
#define TPL_HAS_SET
#define TPL_HAS_MULTI_SET
#endif

// -------------------------------------------------------------------------
// std::map, std::multimap

#if defined(_GLIBCXX_MAP) || defined(_MAP_) || defined(_MAP)
#define TPL_HAS_MAP
#define TPL_HAS_MULTI_MAP
#endif

// -------------------------------------------------------------------------
// std::basic_ostream

#if defined(_GLIBCXX_OSTREAM) || defined(_OSTREAM_) || defined(_OSTREAM)
#define TPL_HAS_OSTREAM
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* TPL_REGEX_DETAIL_STLHEADERS_H */

