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
// Module: stdext/Counter.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 17:57:28
// 
// $Id: Counter.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_COUNTER_H
#define STDEXT_COUNTER_H

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#if !defined(STD_NO_WINSDK)
#ifndef _WINBASE_
#include <winbase.h>
#endif
#else
#ifndef STDEXT_WINAPI_WINBASE_H
#include "winapi/winbase.h"
#endif
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class PCounter - PerformanceCounter

template <bool fAutoReset = false>
class PCounter
{
public:
	typedef UINT64 value_type;
	enum {
		radix10_cstr_size = 24, // INT64最大的数为：9223372036854775807，计19位。
		radix16_cstr_size = 24,
	};

private:
	value_type m_tick;
	
private:
	static value_type __freq()
	{
		value_type _freq = 0;
		QueryPerformanceFrequency(&(LARGE_INTEGER&)_freq);
		if (_freq == 0)
		{
			WINX_REPORT("QueryPerformanceFrequency is unsupported\n");
			_freq = 1;
		}
		return _freq;
	}
	
public:
	PCounter()
	{
		freq();
		start();
	}

	void winx_call start()
	{
		QueryPerformanceCounter(&(LARGE_INTEGER&)m_tick);
	}

	__forceinline value_type winx_call duration()
	{
		value_type tickNow;
		QueryPerformanceCounter(&(LARGE_INTEGER&)tickNow);
		if (fAutoReset)
		{
			value_type tickDur = tickNow - m_tick;
			m_tick = tickNow;
			return tickDur;
		}
		else
		{
			return tickNow - m_tick;
		}
	}

public:
	static value_type winx_call freq()
	{
		static value_type s_freq = __freq();
		return s_freq;
	}

public:
	static void winx_call toRadix10(
		IN const value_type& value,
		OUT WCHAR radix10[radix10_cstr_size]
		)
	{
		_ui64tow(value, radix10, 10);
	}

	static void winx_call toRadix10(
		IN const value_type& value,
		OUT char radix10[radix10_cstr_size]
		)
	{
		_ui64toa(value, radix10, 10);
	}
	
	static void winx_call toRadix16(
		IN const value_type& value,
		OUT WCHAR radix16[radix16_cstr_size]
		)
	{
		_ui64tow(value, radix16, 16);
	}

	static void winx_call toRadix16(
		IN const value_type& value,
		OUT char radix16[radix16_cstr_size]
		)
	{
		_ui64toa(value, radix16, 16);
	}

public:
	template <class LogT>
	static void winx_call trace(LogT& log, const value_type& ticks)
	{
		double msVal = (INT64)ticks * 1000.0 / (INT64)freq();
		char szTicks[radix10_cstr_size];
		toRadix10(ticks, szTicks);
		log.trace(
			"---> Elapse %s ticks (%.2lf ms) (%.2lf min) ...\n",
			szTicks, msVal, msVal/60000.0
			);
	}

	template <class LogT>
	static void winx_call trace(LogT& log, const char* msg, const value_type& ticks)
	{
		double msVal = (INT64)ticks * 1000.0 / (INT64)freq();
		char szTicks[radix10_cstr_size];
		toRadix10(ticks, szTicks);
		log.trace(
			"---> %s: elapse %s ticks (%.2lf ms) (%.2lf min) ...\n",
			msg, szTicks, msVal, msVal/60000.0
			);
	}

	template <class LogT>
	__forceinline value_type winx_call trace(LogT& log)
	{
		value_type dur = duration();
		trace(log, dur);
		return dur;
	}

	template <class LogT>
	__forceinline value_type winx_call trace(LogT& log, const char* msg)
	{
		value_type dur = duration();
		trace(log, msg, dur);
		return dur;
	}
};

typedef PCounter<false> PerformanceCounter;

// -------------------------------------------------------------------------
// class Accumulator

template <class CounterT = PerformanceCounter>
class AccumulatorT
{
public:
	typedef typename CounterT::value_type value_type;
	typedef size_t size_type;
	
private:
	value_type m_acc;
	size_type m_count;

public:
	AccumulatorT() {
		m_acc = 0;
		m_count = 0;
	}
	
	void winx_call start() {
		m_acc = 0;
		m_count = 0;
	}
	
	size_type winx_call count() const {
		return m_count;
	}
	
	const value_type& winx_call accumulate() const {
		return m_acc;
	}

	void winx_call accumulate(const value_type& val) {
		m_acc += val;
		++m_count;
	}
	
	__forceinline void winx_call accumulate(CounterT& counter) {
		m_acc += counter.duration();
		++m_count;
	}
	
	template <class LogT>
	void winx_call trace_avg(LogT& log) {
		log.print("Average: ");
		CounterT::trace(log, m_acc/m_count);
	}

	template <class LogT>
	void winx_call trace(LogT& log) {
		CounterT::trace(log, m_acc);
	}
};

typedef AccumulatorT<> Accumulator;

// -------------------------------------------------------------------------
// $Log: Counter.h,v $

NS_STDEXT_END

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

#endif /* STDEXT_COUNTER_H */
