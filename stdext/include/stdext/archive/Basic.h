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
// Module: stdext/archive/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Basic.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_BASIC_H
#define STDEXT_ARCHIVE_BASIC_H

#if (0)
#define BYTESWAP
#define __BYTESWAP__
#endif

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#ifndef STDEXT_CHARTYPE_H
#include "../CharType.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef ARCHIVE_CACHE_SIZE
#define ARCHIVE_CACHE_SIZE	4096
#endif

#ifndef ARCHIVE_ROUND_SIZE
#define ARCHIVE_ROUND_SIZE	4096
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class ArchiveCharTraits

template <class CharT>
class ArchiveCharTraits
{
};

template <>
class ArchiveCharTraits<char>
{
public:
	typedef char char_type;
	typedef unsigned char uchar_type;
	typedef int int_type;

	enum { endch = -1 };
};

template <>
class ArchiveCharTraits<WCHAR>
{
public:
	typedef WCHAR char_type;
	typedef WCHAR uchar_type;
	typedef int int_type;

	enum { endch = -1 };
};

// -------------------------------------------------------------------------
// class ArchiveIteratorTraits

template <class Iterator>
class ArchiveIteratorTraits
{
public:
	typedef typename Iterator::value_type value_type;
	typedef value_type char_type;
};

template <>
class ArchiveIteratorTraits<char*>
{
public:
	typedef char value_type;
	typedef value_type char_type;
};

template <>
class ArchiveIteratorTraits<const char*>
{
public:
	typedef char value_type;
	typedef value_type char_type;
};

template <>
class ArchiveIteratorTraits<WCHAR*>
{
public:
	typedef WCHAR value_type;
	typedef value_type char_type;
};

template <>
class ArchiveIteratorTraits<const WCHAR*>
{
public:
	typedef WCHAR value_type;
	typedef value_type char_type;
};

// -------------------------------------------------------------------------
// toInt, toUInt

__forceinline int  winx_call toInt(int li)		{ return li; }
__forceinline UINT winx_call toUInt(UINT li)	{ return li; }

// -------------------------------------------------------------------------
// copyMemory

__forceinline void winx_call copyMemory(char* dest, const char* src, size_t n)
{
	memcpy(dest, src, n);
}

__forceinline void winx_call copyMemory(WCHAR* dest, const WCHAR* src, size_t n)
{
	wmemcpy(dest, src, n);
}

// -------------------------------------------------------------------------
// isStgWritable

inline BOOL winx_call isStgWritable(IStorage* pstg)
{
	STATSTG statstg;
	pstg->Stat(&statstg, STATFLAG_NONAME);
	return statstg.grfMode & (STGM_READWRITE|STGM_WRITE);
}

inline BOOL winx_call isStgWritable(IStream* pstm)
{
	STATSTG statstg;
	pstm->Stat(&statstg, STATFLAG_NONAME);
	return statstg.grfMode & (STGM_READWRITE|STGM_WRITE);
}

// -------------------------------------------------------------------------
// class IoException

class IoException : public Exception
{
private:
	HRESULT m_scode;

public:
	IoException(HRESULT sc, const char* errMsg = "Io Exception")
		: Exception(errMsg), m_scode(sc) {
	}
	
	HRESULT winx_call getError() const {
		return m_scode;
	}
};

// -------------------------------------------------------------------------
// class ArchiveCache

class ArchiveCache
{
public:
	typedef DefaultStaticAlloc allocator_type;
	
	enum { cacheSize = ARCHIVE_CACHE_SIZE };
	enum { roundSize = ARCHIVE_ROUND_SIZE };
};

// -------------------------------------------------------------------------
// WINX_BYTESWAP

#if defined(BYTESWAP) || defined(__BYTESWAP__)
#define WINX_BYTESWAP
#endif

#if !defined(WINX_BYTESWAP)
	#define _WinxByteSwap(val)
	#define _WinxByteSwap16(val)
	#define _WinxByteSwap32(val)
	#define _WinxByteSwap64(val)
	#define _WinxByteSwapStruct(val)
	#define _WinxByteSwap16Array(val)
	#define _WinxByteSwap32Array(val)
	#define _WinxByteSwapStructArray(val)
#else
	#error "todo: BYTESWAP support"
#endif

// -------------------------------------------------------------------------
// put_n

template <class _OStream, class _CharT>
inline void winx_call put_n_(_OStream& __o, size_t __n, const _CharT& __val)
{
    for (size_t __i = 0; __i < __n; __i++)
		__o.put(__val);
}

template <class _OStream, class _CharT>
inline void winx_call put_n(_OStream& __o, size_t __n, const _CharT& __val)
{
	__o.put(__n, __val);
}

// -------------------------------------------------------------------------
// $Log: Basic.h,v $

NS_STDEXT_END

#endif /* STDEXT_ARCHIVE_BASIC_H */
