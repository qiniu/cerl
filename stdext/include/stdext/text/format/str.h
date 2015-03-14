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
// Module: stdext/text/format/str.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: str.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_STR_H
#define STDEXT_TEXT_FORMAT_STR_H

#ifndef STDEXT_BASIC_H
#include "../../Basic.h"
#endif

#ifndef STDEXT_TCHAR_H
#include "../../tchar.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../BasicString.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// macro WINX_STRBUF_SIZE

template <class Target>
struct StringBufTraits_ {};

#define WINX_STRBUF_TRAITS(Type, nBufSize) \
template <> \
struct StringBufTraits_<Type> { \
	enum { value = nBufSize }; \
};

#define WINX_STRBUF_SIZE(Target) \
	NS_STDEXT::StringBufTraits_<Target>::value

// -------------------------------------------------------------------------
// function to_str (integer/real)

WINX_STRBUF_TRAITS(int, 32)
WINX_STRBUF_TRAITS(unsigned int, 32)
WINX_STRBUF_TRAITS(long, 64)
WINX_STRBUF_TRAITS(unsigned long, 64)

template <class E>
__forceinline E* winx_call to_str(E buf[], int i, int radix = 10) {
	return tchar::itoa(i, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], unsigned int i, int radix = 10) {
	return tchar::ultoa(i, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], long i, int radix = 10) {
	return tchar::ltoa(i, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], unsigned long i, int radix = 10) {
	return tchar::ultoa(i, buf, radix);
}

#ifndef _NO_INT64

WINX_STRBUF_TRAITS(__int64, 64)
WINX_STRBUF_TRAITS(__uint64, 64)

template <class E>
__forceinline E* winx_call to_str(E buf[], const __int64& i64, int radix = 10) {
	return tchar::i64toa(i64, buf, radix);
}

template <class E>
__forceinline E* winx_call to_str(E buf[], const __uint64& i64, int radix = 10) {
	return tchar::ui64toa(i64, buf, radix);
}

#endif

WINX_STRBUF_TRAITS(float, 64)
WINX_STRBUF_TRAITS(double, 64)

// -------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

__forceinline char* winx_call to_str(char buf[], double val, int ndigit = 12) {
	return _gcvt(val, ndigit, buf);
}

inline wchar_t* winx_call to_str(wchar_t buf[], double val, int ndigit = 12) {
	enum { bufsize = WINX_STRBUF_SIZE(double) };
	char cbuf[bufsize];
	const char* src = _gcvt(val, ndigit, cbuf);
	for (size_t i = 0; (buf[i] = src[i]) != '\0'; ++i);
	return buf;
}

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

// -------------------------------------------------------------------------
// function str/wstr

#define WINX_STRING_FORMAT_IMPL_(str, E) 	\
											\
template <class AllocT, class Target>		\
__forceinline BasicString<E> winx_call str(AllocT& alloc, const Target& val) {	\
	enum { bufsize = WINX_STRBUF_SIZE(Target) };	\
	E buf[bufsize];									\
	return BasicString<E>(alloc, to_str(buf, val));	\
}											\
											\
template <class AllocT, class Target>		\
__forceinline BasicString<E> winx_call str(AllocT& alloc, const Target& val, int radix) {	\
	enum { bufsize = WINX_STRBUF_SIZE(Target) };			\
	E buf[bufsize];											\
	return BasicString<E>(alloc, to_str(buf, val, radix));	\
}															\
															\
template <class AllocT, class Tr, class AllocT2>			\
__forceinline BasicString<E> winx_call str(AllocT& alloc, const std::basic_string<E, Tr, AllocT2>& val) { \
	return BasicString<E>(alloc, val.begin(), val.end());	\
}															\
															\
template <class AllocT, class AllocT2>						\
__forceinline BasicString<E> winx_call str(AllocT& alloc, const std::vector<E, AllocT2>& val) {	\
	return BasicString<E>(alloc, val.begin(), val.end());	\
}															\
															\
template <class AllocT>										\
__forceinline BasicString<E> winx_call str(AllocT& alloc, const E* val) { \
	return BasicString<E>(alloc, val);						\
}															\
															\
template <class AllocT>										\
__forceinline BasicString<E> winx_call str(AllocT& alloc, const E val) { \
	return BasicString<E>(alloc, &val, 1);					\
}

WINX_STRING_FORMAT_IMPL_(str, char)
WINX_STRING_FORMAT_IMPL_(wstr, wchar_t)

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestFormat

#if defined(STD_UNITTEST)

#include <sstream>
#include <cmath>
#include "StringAlgo.h"
#include "../Rand.h"
#include "../Memory.h"

template <class LogT>
class TestFormat : public TestCase
{
	WINX_TEST_SUITE(TestFormat);
		WINX_TEST(testBasic);
		WINX_TEST(testIntToStr);
		WINX_TEST(testRealToStr);
	WINX_TEST_SUITE_END();

public:
	template <class IntT>
	void doTestIntToStr(const IntT& val, LogT& log)
	{
		NS_STDEXT::AutoAlloc alloc;
		std::ostringstream os;
		os << val;
		AssertExp(os.str() == NS_STDEXT::str(alloc, val));
	}

	void testIntToStr(LogT& log)
	{
		NS_STDEXT::Rand rnd;
		size_t i;
		for (i = 0; i < 1000; ++i) {
			const int val = rnd.rand();
			doTestIntToStr(val, log);
		}		
		for (i = 0; i < 1000; ++i) {
			const long val = rnd.rand();
			doTestIntToStr(val, log);
		}
		for (i = 0; i < 1000; ++i) {
			const unsigned long val = rnd.rand();
			doTestIntToStr(val, log);
		}
		for (i = 0; i < 1000; ++i) {
			const unsigned int val = rnd.rand();
			doTestIntToStr(val, log);
		}
#ifndef _NO_INT64
		for (i = 0; i < 1000; ++i) {
			const __int64 val = ((__int64)rnd.rand() << 32) | rnd.rand();
			doTestIntToStr(val, log);
		}
		for (i = 0; i < 1000; ++i) {
			const __uint64 val = (__uint64)(((__int64)rnd.rand() << 32) | rnd.rand());
			doTestIntToStr(val, log);
		}
#endif
	}

	template <class RealT>
	void doTestRealToStr(const RealT& val, LogT& log)
	{
		NS_STDEXT::AutoAlloc alloc;
		NS_STDEXT::String s = NS_STDEXT::str(alloc, val);
		std::string s1 = s.stl_str();
		std::istringstream is(s1);
		RealT val2;
		is >> val2;
		AssertExp(fabs(val - val2) < 1e-10);
	}
	
	void testRealToStr(LogT& log)
	{
		NS_STDEXT::Rand rnd;
		size_t i;
		for (i = 0; i < 1000; ++i) {
			const float val = (float)rnd.frand();
			doTestRealToStr(val, log);
		}		
		for (i = 0; i < 1000; ++i) {
			const double val = (double)rnd.frand();
			doTestRealToStr(val, log);
		}
	}
	
	void testBasic(LogT& log)
	{
		NS_STDEXT::AutoAlloc alloc;
		NS_STDEXT::String s;
		NS_STDEXT::WString ws;
		
		s = NS_STDEXT::str(alloc, 123);
		AssertExp(s == "123");
		
		ws = NS_STDEXT::wstr(alloc, 123, 16);
		AssertExp(NS_STDEXT::upper(alloc, ws) == L"7B");
		
		s = NS_STDEXT::str(alloc, 12.3);
		AssertExp(s == "12.3");
		
		ws = NS_STDEXT::wstr(alloc, 12.3);
		AssertExp(ws == L"12.3");
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_FORMAT_STR_H */

