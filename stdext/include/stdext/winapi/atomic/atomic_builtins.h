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
// Module: stdext/winapi/atomic/atomic_builtins.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: atomic_builtins.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_ATOMIC_ATOMIC_BUILTINS_H
#define STDEXT_WINAPI_ATOMIC_ATOMIC_BUILTINS_H

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedIncrement(volatile LPLONG lpAddend)
{
	return __sync_add_and_fetch(lpAddend, 1);
}

__forceinline LONG WINAPI InterlockedDecrement(volatile LPLONG lpAddend)
{
    return __sync_sub_and_fetch(lpAddend, 1);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedExchange(volatile LPLONG Target, LONG Value)
{
	__sync_synchronize();
	return __sync_lock_test_and_set(Target, Value);
}

__forceinline PVOID WINAPI InterlockedExchangePointer(volatile PVOID* Target, PVOID Value)
{
	__sync_synchronize();
	return (PVOID)__sync_lock_test_and_set(Target, Value);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedExchangeAdd(volatile LPLONG Addend, LONG Value)
{
    return __sync_fetch_and_add(Addend, Value);
}

// -------------------------------------------------------------------------

__forceinline LONG WINAPI InterlockedCompareExchange(
    volatile LPLONG Destination, LONG Exchange, LONG Comperand)
{
	return __sync_val_compare_and_swap(Destination, Comperand, Exchange);
}

__forceinline PVOID WINAPI InterlockedCompareExchangePointer(
    volatile PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return (PVOID)__sync_val_compare_and_swap(Destination, Comperand, Exchange);
}

// -------------------------------------------------------------------------
// InterlockedExchange64

__forceinline LONG64 WINAPI InterlockedExchange64(
	volatile PLONG64 Target, LONG64 Value)
{
	__sync_synchronize();
	return __sync_lock_test_and_set(Target, Value);
}

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_WINAPI_ATOMIC_ATOMIC_BUILTINS_H */

