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
// Module: stdext/patch/string.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-22 17:37:57
// 
// $Id: string.h,v 1.1 2006/12/22 10:19:55 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_PATCH_STRING_H
#define STDEXT_PATCH_STRING_H

// -------------------------------------------------------------------------

#if defined(X_STL_VC6)

#if defined(_MSC_VER) && (_MSC_VER == 1200)
#include "vc6/string.h"
#endif

#endif

// -------------------------------------------------------------------------
// $Log: string.h,v $
// Revision 1.1  2006/12/22 10:19:55  xushiwei
// STL-Patch: std::basic_string::_Split function bugfix (vc6)
//

#endif /* STDEXT_PATCH_STRING_H */
