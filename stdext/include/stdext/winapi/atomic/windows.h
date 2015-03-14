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
// Module: stdext/winapi/atomic/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: windows.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_ATOMIC_WINDOWS_H
#define STDEXT_WINAPI_ATOMIC_WINDOWS_H

// -------------------------------------------------------------------------

// we use our own implementation of InterlockedExchangePointer because of problems with the one in system headers
__forceinline PVOID WINAPI winx_InterlockedExchangePointer(PVOID* Target, PVOID Value)
{
	return reinterpret_cast<PVOID>(static_cast<LONG_PTR>(
		InterlockedExchange(
			reinterpret_cast<LONG*>(Target),
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(Value))
			)
		));
}

#undef InterlockedExchangePointer
#define InterlockedExchangePointer winx_InterlockedExchangePointer

// -------------------------------------------------------------------------

#if defined(WINX_DETAIL_WINSDK_VC6)

__forceinline LONG WINAPI winx_InterlockedCompareExchange(
	LPLONG Destination, LONG Exchange, LONG Comperand)
{
	return (LONG)InterlockedCompareExchange(
		(PVOID*)Destination, (PVOID)Exchange, (PVOID)Comperand);
}

__forceinline PVOID WINAPI winx_InterlockedCompareExchangePointer(
	PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return InterlockedCompareExchange(
		Destination, Exchange, Comperand);
}

#define InterlockedCompareExchange			winx_InterlockedCompareExchange
#define InterlockedCompareExchangePointer	winx_InterlockedCompareExchangePointer

#elif !defined(InterlockedCompareExchangePointer)

__forceinline PVOID WINAPI winx_InterlockedCompareExchangePointer(
	PVOID* Destination, PVOID Exchange, PVOID Comperand)
{
	return reinterpret_cast<PVOID>(static_cast<LONG_PTR>(
		InterlockedCompareExchange(
			reinterpret_cast<LONG*>(Destination),
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(Exchange)),
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(Comperand))
			)
		));
}

#define InterlockedCompareExchangePointer winx_InterlockedCompareExchangePointer

#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_WINAPI_ATOMIC_WINDOWS_H */
