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
// Module: stdext/LargeInteger.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 20:12:04
// 
// $Id: LargeInteger.h,v 1.1 2006/11/30 03:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_LARGEINTEGER_H
#define STDEXT_LARGEINTEGER_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

#ifndef LISet32
#define LISet32(li, v) ((li).HighPart = (v) < 0 ? -1 : 0, (li).LowPart = (v))
#endif

#ifndef ULISet32
#define ULISet32(li, v) ((li).HighPart = 0, (li).LowPart = (v))
#endif

// -------------------------------------------------------------------------
// class ULargeInteger

class LargeInteger;
class ULargeInteger
{
public:
	union {
		struct {
			DWORD LowPart;
			DWORD HighPart;
		};
		struct {
			DWORD LowPart;
			DWORD HighPart;
		} u;
	    ULONGLONG QuadPart;
	};

private:
	typedef ULargeInteger ThisClass;

public:
	ULargeInteger() { QuadPart = 0; }
	ULargeInteger(INT rhs) { QuadPart = rhs; }
	ULargeInteger(LONG rhs) { QuadPart = rhs; }
	ULargeInteger(UINT rhs) { QuadPart = rhs; }
	ULargeInteger(ULONG rhs) { QuadPart = rhs; }
	ULargeInteger(UINT64 rhs) { QuadPart = rhs; }
	ULargeInteger(const ULARGE_INTEGER& rhs) { QuadPart = rhs.QuadPart; }
	ULargeInteger(const ULargeInteger& rhs) { QuadPart = rhs.QuadPart; }
	ULargeInteger(const LargeInteger& rhs);

	const ThisClass& winx_call operator=(const LargeInteger& rhs);
	const ThisClass& winx_call operator=(const ULargeInteger& rhs)
	{
		QuadPart = rhs.QuadPart;
		return *this;
	}

	const ThisClass& winx_call operator=(const ULARGE_INTEGER& rhs)
	{
		QuadPart = rhs.QuadPart;
		return *this;
	}

	operator ULARGE_INTEGER&()
	{
		return *(ULARGE_INTEGER*)this;
	}
	
	operator const ULARGE_INTEGER&() const
	{
		return *(const ULARGE_INTEGER*)this;
	}

public:
	const ThisClass& winx_call operator+=(INT32 rhs)
	{
		QuadPart += rhs;
		return *this;
	}
	const ThisClass& winx_call operator+=(UINT32 rhs)
	{
		QuadPart += rhs;
		return *this;
	}
	const ThisClass& winx_call operator-=(INT32 rhs)
	{
		QuadPart -= rhs;
		return *this;
	}
	const ThisClass& winx_call operator-=(UINT32 rhs)
	{
		QuadPart -= rhs;
		return *this;
	}
	const ThisClass& winx_call operator++()
	{
		++QuadPart;
		return *this;
	}
	const ThisClass& winx_call operator--()
	{
		--QuadPart;
		return *this;
	}
	ThisClass winx_call operator++(int)
	{
		ThisClass tmp = *this;
		operator++();
		return tmp;
	}
	ThisClass winx_call operator--(int)
	{
		ThisClass tmp = *this;
		operator--();
		return tmp;
	}

	ThisClass winx_call operator+(INT32 rhs) const
	{
		ThisClass tmp = *this;
		return tmp += rhs;
	}
	ThisClass winx_call operator-(INT32 rhs) const
	{
		ThisClass tmp = *this;
		return tmp -= rhs;
	}

	LONGLONG winx_call operator-() const
	{
		return -(LONGLONG)QuadPart;
	}
	
	bool winx_call operator==(const ThisClass& rhs) const
	{
		return QuadPart == rhs.QuadPart;
	}
	bool winx_call operator<(const ThisClass& rhs) const
	{
		return QuadPart < rhs.QuadPart;
	}
	bool winx_call operator<=(const ThisClass& rhs) const
	{
		return QuadPart <= rhs.QuadPart;
	}
	bool winx_call operator!=(const ThisClass& rhs) const
	{
		return QuadPart != rhs.QuadPart;
	}
	bool winx_call operator>(const ThisClass& rhs) const
	{
		return QuadPart > rhs.QuadPart;
	}
	bool winx_call operator>=(const ThisClass& rhs) const
	{
		return QuadPart >= rhs.QuadPart;
	}	
};

// -------------------------------------------------------------------------
// class LargeInteger

class LargeInteger
{
public:
	union
	{
		struct {
			DWORD LowPart;
			LONG HighPart;
		};
		struct {
			DWORD LowPart;
			LONG HighPart;
		} u;
		LONGLONG QuadPart;
	};

private:
	typedef LargeInteger ThisClass;

public:
	LargeInteger()  { QuadPart = 0; }
	LargeInteger(INT rhs) { QuadPart = rhs; }
	LargeInteger(LONG rhs) { QuadPart = rhs; }
	LargeInteger(UINT rhs) { QuadPart = rhs; }
	LargeInteger(ULONG rhs) { QuadPart = rhs; }
	LargeInteger(INT64 rhs) { QuadPart = rhs; }
	LargeInteger(const LARGE_INTEGER& rhs) { QuadPart = rhs.QuadPart; }
	LargeInteger(const LargeInteger& rhs) { QuadPart = rhs.QuadPart; }
	explicit LargeInteger(const ULargeInteger& rhs) { QuadPart = rhs.QuadPart; }

	const ThisClass& winx_call operator=(const LARGE_INTEGER& rhs)
	{
		QuadPart = rhs.QuadPart;
		return *this;
	}
	
	const ThisClass& winx_call operator=(const LargeInteger& rhs)
	{
		QuadPart = rhs.QuadPart;
		return *this;
	}

	operator LARGE_INTEGER&()
	{
		return *(LARGE_INTEGER*)this;
	}

	operator const LARGE_INTEGER&() const
	{
		return *(const LARGE_INTEGER*)this;
	}

public:
	const ThisClass& winx_call operator+=(INT32 rhs)
	{
		QuadPart += rhs;
		return *this;
	}
	const ThisClass& winx_call operator+=(UINT32 rhs)
	{
		QuadPart += rhs;
		return *this;
	}
	const ThisClass& winx_call operator-=(INT32 rhs)
	{
		QuadPart -= rhs;
		return *this;
	}
	const ThisClass& winx_call operator-=(UINT32 rhs)
	{
		QuadPart -= rhs;
		return *this;
	}
	const ThisClass& winx_call operator++()
	{
		++QuadPart;
		return *this;
	}
	const ThisClass& winx_call operator--()
	{
		--QuadPart;
		return *this;
	}
	ThisClass winx_call operator++(int)
	{
		ThisClass tmp = *this;
		operator++();
		return tmp;
	}
	ThisClass winx_call operator--(int)
	{
		ThisClass tmp = *this;
		operator--();
		return tmp;
	}
	
	ThisClass winx_call operator+(INT32 rhs)
	{
		ThisClass tmp = *this;
		return tmp += rhs;
	}
	ThisClass winx_call operator-(INT32 rhs)
	{
		ThisClass tmp = *this;
		return tmp -= rhs;
	}
	
	bool winx_call operator==(const ThisClass& rhs) const
	{
		return QuadPart == rhs.QuadPart;
	}
	bool winx_call operator<(const ThisClass& rhs) const
	{
		return QuadPart < rhs.QuadPart;
	}
	bool winx_call operator<=(const ThisClass& rhs) const
	{
		return QuadPart <= rhs.QuadPart;
	}
	bool winx_call operator!=(const ThisClass& rhs) const
	{
		return QuadPart != rhs.QuadPart;
	}
	bool winx_call operator>(const ThisClass& rhs) const
	{
		return QuadPart > rhs.QuadPart;
	}
	bool winx_call operator>=(const ThisClass& rhs) const
	{
		return QuadPart >= rhs.QuadPart;
	}	
};

// -------------------------------------------------------------------------

inline LargeInteger winx_call operator-(const ULargeInteger& a, const ULargeInteger& b)
{
	INT64 result = a.QuadPart - b.QuadPart;
	return result;
}

inline LargeInteger winx_call operator/(const LargeInteger& a, const LargeInteger& b)
{
	INT64 result = a.QuadPart / b.QuadPart;
	return result;
}

inline LargeInteger winx_call operator*(const LargeInteger& a, const LargeInteger& b)
{
	INT64 result = a.QuadPart * b.QuadPart;
	return result;
}

inline LargeInteger winx_call operator*(const LargeInteger& a, const INT b)
{
	INT64 result = a.QuadPart * b;
	return result;
}

inline LargeInteger winx_call operator*(const INT a, const LargeInteger& b)
{
	INT64 result = a * b.QuadPart;
	return result;
}

inline ULargeInteger winx_call operator/(const ULargeInteger& a, const ULargeInteger& b)
{
	UINT64 result = a.QuadPart / b.QuadPart;
	return result;
}

inline ULargeInteger winx_call operator*(const ULargeInteger& a, const ULargeInteger& b)
{
	UINT64 result = a.QuadPart * b.QuadPart;
	return result;
}

inline ULargeInteger::ULargeInteger(const LargeInteger& rhs)
{
	QuadPart = rhs.QuadPart;
}

inline const ULargeInteger& winx_call ULargeInteger::operator=(const LargeInteger& rhs)
{
	QuadPart = rhs.QuadPart;
}

// -------------------------------------------------------------------------
// $Log: LargeInteger.h,v $
// Revision 1.1  2006/11/30 03:19:17  xushiwei
// STL-Extension:
//  ULargeInteger, LargeInteger, ReadArchiveImpl, WriteArchiveImpl
//  StreamArchive(StreamWriteArchive, StreamReadArchive, MemStreamWriteArchive, MemStreamReadArchive)
//  StdioArchive(StdioWriteArchive, StdioReadArchive)
//

NS_STDEXT_END

#endif /* STDEXT_LARGEINTEGER_H */
