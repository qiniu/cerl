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
// Module: stdext/tchar/memop.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-23 12:33:34
// 
// $Id: memop.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TCHAR_MEMOP_H
#define STDEXT_TCHAR_MEMOP_H

#ifndef __template_xchar_inline
#define __template_xchar_inline	// template <class xchar> inline
#endif

#ifndef __xdecl
#define __xdecl		// __cdecl
#endif

__template_xchar_inline
const xchar* __xdecl __xcharfunc(memchr)(const xchar *_S, xchar _C, size_t _N)
        {for (; 0 < _N; ++_S, --_N)
                if (*_S == _C)
                        return (_S);
        return (0); }

__template_xchar_inline 
int __xdecl __xcharfunc(memcmp)(const xchar *_S1, const xchar *_S2, size_t _N)
        {for (; 0 < _N; ++_S1, ++_S2, --_N)
                if (*_S1 != *_S2)
                        return (*_S1 < *_S2 ? -1 : +1);
        return (0); }

__template_xchar_inline 
xchar* __xdecl __xcharfunc(memcpy)(xchar *_S1, const xchar *_S2, size_t _N)
        {xchar *_Su1 = _S1;
        for (; 0 < _N; ++_Su1, ++_S2, --_N)
                *_Su1 = *_S2;
        return (_S1); }

__template_xchar_inline 
xchar* __xdecl __xcharfunc(memmove)(xchar *_S1, const xchar *_S2, size_t _N)
        {xchar *_Su1 = _S1;
        if (_S2 < _Su1 && _Su1 < _S2 + _N)
                for (_Su1 += _N, _S2 += _N; 0 < _N; --_N)
                        *--_Su1 = *--_S2;
        else
                for (; 0 < _N; --_N)
                        *_Su1++ = *_S2++;
        return (_S1); }

__template_xchar_inline 
xchar* __xdecl __xcharfunc(memset)(xchar *_S, xchar _C, size_t _N)
        {xchar *_Su = _S;
        for (; 0 < _N; ++_Su, --_N)
                *_Su = _C;
        return (_S); }

// -------------------------------------------------------------------------
// $Log: memop.h,v $
// Revision 1.1  2006/10/18 12:13:39  xushiwei
// stdext as independent component
//
// Revision 1.1  2006/08/23 04:48:32  xushiwei
// STL-Extension:
//   namespace std::tchar(std::tchar::strlen, std::tchar::strlwr, etc)
//

#endif /* STDEXT_TCHAR_MEMOP_H */
