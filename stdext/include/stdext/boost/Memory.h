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
// Module: stdext/boost/Memory.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 15:43:17
// 
// $Id: Memory.h 1427 2010-04-05 10:16:37Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_BOOST_MEMORY_H
#define STDEXT_BOOST_MEMORY_H

// -------------------------------------------------------------------------

#define NS_BOOST_MEMORY_BEGIN	namespace NS_STDEXT {
#define NS_BOOST_MEMORY_END		}
#define NS_BOOST_MEMORY			NS_STDEXT
#define NS_BOOST_MEMORY_POLICY_BEGIN
#define NS_BOOST_MEMORY_POLICY_END
#define NS_BOOST_MEMORY_POLICY	NS_STDEXT

#ifndef NS_BOOST_DETAIL_BEGIN
#define NS_BOOST_DETAIL_BEGIN	namespace NS_STDEXT {
#define NS_BOOST_DETAIL_END		}
#define NS_BOOST_DETAIL			NS_STDEXT
#endif

#ifndef BOOST_MEMORY_HPP
#include "../../../../memory/boost/memory.hpp"
#endif

// -------------------------------------------------------------------------

#endif /* STDEXT_BOOST_MEMORY_H */
