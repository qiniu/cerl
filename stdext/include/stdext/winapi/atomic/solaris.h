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
// Module: stdext/winapi/atomic/solaris.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: solaris.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_ATOMIC_SOLARIS_H
#define STDEXT_WINAPI_ATOMIC_SOLARIS_H

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedIncrement(volatile LPLONG lpAddend)
{
    return atomic_inc_32_nv(lpAddend);
}

__forceinline LONG WINAPI InterlockedDecrement(volatile LPLONG lpAddend)
{
    return atomic_dec_32_nv(lpAddend);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedExchange(volatile LPLONG Target, LONG Value)
{
	return atomic_swap_32(Target, Value);
}

__forceinline PVOID WINAPI InterlockedExchangePointer(volatile PVOID* Target, PVOID Value)
{
	return atomic_swap_ptr(Target, Value);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedExchangeAdd(volatile LPLONG Addend, LONG Value)
{
    return atomic_add_32_nv(Addend, Value) - Value;
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedCompareExchange(
    volatile LPLONG Destination, LONG Exchange, LONG Comperand)
{
	return atomic_cas_32(Destination, Comperand, Exchange);
}

__forceinline PVOID WINAPI InterlockedCompareExchangePointer(
    volatile PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return atomic_cas_ptr(Destination, Comperand, Exchange);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* BOOST_DETAIL_WINAPI_ATOMIC_SOLARIS_HPP */
