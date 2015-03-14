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
// Module: stdext/int/Traits.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Traits.h 2004-7-9 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_INT_TRAITS_H
#define STDEXT_INT_TRAITS_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// Int TypeTraits

template <class IntT>
struct IntTypeTraits
{
};

#define WINX_INT_TYPETRAITS_(IntT, isSigned, SignedT, UnsignedT)			\
	template <>																\
	struct IntTypeTraits<IntT>												\
	{																		\
		enum { fsigned = isSigned };										\
		typedef SignedT signed_type;										\
		typedef UnsignedT unsigned_type;									\
	}

#define WINX_INT_TYPEPAIR_(IntT, UIntT)										\
	WINX_INT_TYPETRAITS_(IntT, TRUE, IntT, UIntT);							\
	WINX_INT_TYPETRAITS_(UIntT, FALSE, IntT, UIntT)

// -------------------------------------------------------------------------

WINX_INT_TYPEPAIR_(char, unsigned char);
WINX_INT_TYPEPAIR_(short, unsigned short);
WINX_INT_TYPEPAIR_(int, unsigned int);
WINX_INT_TYPEPAIR_(long, unsigned long);
WINX_INT_TYPEPAIR_(INT64, UINT64);

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_INT_TRAITS_H */
