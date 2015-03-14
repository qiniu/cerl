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
// Module: stdext/detail/GlobalVar.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2004-4-5 20:30:40
// 
// $Id: GlobalVar.h,v 1.5 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DETAIL_GLOBALVAR_H
#define STDEXT_DETAIL_GLOBALVAR_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct GlobalVar

namespace detail
{
	template <class Type, int nInst>
	struct GlobalVar
	{
		static Type g_var;
	};

	template <class Type, int nInst>
	Type GlobalVar<Type, nInst>::g_var;
}

// -------------------------------------------------------------------------

#define WINX_DETAIL_GIDX_TLS_MUTEX_	0	// see <stdext/thread/TLS.h>

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_DETAIL_GLOBALVAR_H */
