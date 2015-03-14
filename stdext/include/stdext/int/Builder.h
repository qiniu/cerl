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
// Module: stdext/int/Builder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Builder.h 2004-7-9 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_INT_BUILDER_H
#define STDEXT_INT_BUILDER_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct UINT_BUILDER

#pragma pack(1)

template <class UIntType>
struct UINT_BUILDER
{
#if defined(WINX_BYTESWAP)
	UIntType HighPart;
	UIntType LowPart;
#else
	UIntType LowPart;
	UIntType HighPart;
#endif
};

#pragma pack()

// -------------------------------------------------------------------------
// class UIntUtils

template <class UIntType>
class UIntUtils
{
public:
	typedef UINT32 CellUIntType;
	typedef CellUIntType CarryUIntType;
	typedef CellUIntType BorrowUIntType;

	typedef typename UIntType::BaseUIntType BaseUIntType;
	typedef typename UIntType::ParentUIntType ParentUIntType;

private:	
	typedef UIntUtils<BaseUIntType> BaseUtils;
	typedef UIntUtils<CellUIntType> CellUtils;

	enum { bits = BaseUtils::bits * 2 };
	enum { cell_bits = CellUtils::bits };
	enum { cell_count = bits / cell_bits };

#if defined(WINX_BYTESWAP)
	enum { cell_begin = 0, cell_end = cell_count, cell_step = 1 };
#else
	enum { cell_begin = cell_count-1, cell_end = -1, cell_step = -1 };
#endif

public:
	enum { radix10_count = BaseUtils::radix10_count * 2 };

public:
	static bool winx_call testCast(const ParentUIntType& rhs)
	{
		return !rhs.HighPart;
	}

	static bool winx_call cast(UIntType& lhs, const ParentUIntType& rhs)
	{
		lhs = rhs.LowPart;
		return !rhs.HighPart;
	}
	
	static CarryUIntType winx_call plus(UIntType& lhs, const UIntType& rhs)
	{
		CarryUIntType carryLow = BaseUtils::plus(lhs.LowPart, rhs.LowPart);
		CarryUIntType carryHigh = BaseUtils::plus(lhs.HighPart, carryLow);
		return carryHigh + BaseUtils::plus(lhs.HighPart, rhs.HighPart);
	}
	
	static CarryUIntType winx_call plus(UIntType& lhs, CarryUIntType rhs)
	{
		CarryUIntType carryLow = BaseUtils::plus(lhs.LowPart, rhs);
		return BaseUtils::plus(lhs.HighPart, carryLow);
	}

	static BorrowUIntType winx_call minus(UIntType& lhs, const UIntType& rhs)
	{
		BorrowUIntType borrowLow = BaseUtils::minus(lhs.LowPart, rhs.LowPart);
		BorrowUIntType borrowHigh = BaseUtils::minus(lhs.HighPart, borrowLow);
		return borrowHigh + BaseUtils::minus(lhs.HighPart, rhs.HighPart);
	}
	
	static BorrowUIntType winx_call minus(UIntType& lhs, BorrowUIntType rhs)
	{
		BorrowUIntType borrowLow = BaseUtils::minus(lhs.LowPart, rhs);
		return BaseUtils::minus(lhs.HighPart, borrowLow);
	}

	static void winx_call multiply(ParentUIntType& result, const UIntType& lhs, const UIntType& rhs)
	{
		BaseUtils::multiply(result.LowPart, lhs.LowPart, rhs.LowPart);
		BaseUtils::multiply(result.HighPart, lhs.HighPart, rhs.HighPart);

		UIntType resultMid1, resultMid2;
		BaseUtils::multiply(resultMid1, lhs.HighPart, rhs.LowPart);
		BaseUtils::multiply(resultMid2, lhs.LowPart, rhs.HighPart);
		
		CarryUIntType carryLow = BaseUtils::plus(result.LowPart.HighPart, resultMid1.LowPart);
		carryLow += BaseUtils::plus(result.LowPart.HighPart, resultMid2.LowPart);
		
		CarryUIntType carryHigh = BaseUtils::plus(result.HighPart.LowPart, carryLow);
		carryHigh += BaseUtils::plus(result.HighPart.LowPart, resultMid1.HighPart);
		carryHigh += BaseUtils::plus(result.HighPart.LowPart, resultMid2.HighPart);

		BaseUtils::plus(result.HighPart.HighPart, carryHigh);
	}

	static CellUIntType winx_call divide(UIntType& lhs, CellUIntType rhs)
	{
		WINX_STATIC_ASSERT(
			sizeof(UINT_BUILDER<BaseUIntType>) == bits/8
			);

		typedef CellUtils::PARENT CellParent;
		CellParent dividend = rhs;
		CellParent divisor = 0;
		CellUIntType* dest = (CellUIntType*)&lhs;

		for (int i = cell_begin; i != cell_end; i += cell_step)
		{
			divisor = (divisor << cell_bits) | dest[i];
			dest[i] = (CellUIntType)(divisor / dividend);
			divisor %= dividend;
		}
		return (CellUIntType)divisor;
	}
};

// -------------------------------------------------------------------------
// class UIntBuilder

template <class UIntType>
class UIntBuilder : public UINT_BUILDER<UIntType>
{
private:
	typedef UIntBuilder<UIntType> Myt_;
	typedef UIntUtils<Myt_> Utils;

public:
	typedef UIntType BaseUIntType;
	typedef UIntBuilder<Myt_> ParentUIntType;
	typedef typename Utils::CellUIntType CellUIntType;
	typedef INT32 CellIntType;

	UIntBuilder() {}
	
	UIntBuilder(CellIntType nVal)
	{
		LowPart = nVal;
		HighPart = (nVal >= 0 ? (CellIntType)0 : (CellIntType)-1);
	}
	
	UIntBuilder(CellUIntType nVal)
	{
		LowPart = nVal;
		HighPart = 0;
	}
	
public:
	const UIntBuilder winx_call operator~() const
	{
		UIntBuilder ret;
		ret.LowPart = ~LowPart;
		ret.HighPart = ~HighPart;
		return ret;
	}
	
	const UIntBuilder winx_call operator-() const
	{
		UIntBuilder ret;
		ret.LowPart = ~LowPart;
		ret.HighPart = ~HighPart;
		Utils::plus(ret, (UINT)1);
		return ret;
	}
	
	const UIntBuilder& winx_call operator+() const
	{
		return *this;
	}

	bool winx_call operator!() const
	{
		return !LowPart && !HighPart;
	}
	
	const UIntBuilder& winx_call operator++()
	{
		Utils::plus(*this, (CellUIntType)1);
		return *this;
	}
	
	const UIntBuilder winx_call operator++(int)
	{
		UIntBuilder ret = *this;
		operator++();
		return ret;
	}

	const UIntBuilder& winx_call operator--()
	{
		Utils::minus(*this, (CellUIntType)1);
		return *this;
	}
	
	const UIntBuilder winx_call operator--(int)
	{
		UIntBuilder ret = *this;
		operator--();
		return ret;
	}
	
public:
	const UIntBuilder& winx_call operator+=(const UIntBuilder& rhs)
	{
		Utils::plus(*this, rhs);
		return *this;
	}
	
	const UIntBuilder& winx_call operator-=(const UIntBuilder& rhs)
	{
		Utils::minus(*this, rhs);
		return *this;
	}
	
	const UIntBuilder& winx_call operator*=(const UIntBuilder& rhs)
	{
		ParentUIntType result;
		Utils::multiply(result, *this, rhs);
		return *this = result.LowPart;
	}

	const UIntBuilder& winx_call operator/=(const UIntBuilder& rhs);
	const UIntBuilder& winx_call operator%=(const UIntBuilder& rhs);
	
	const UIntBuilder& winx_call operator^=(const UIntBuilder& rhs)
	{
		LowPart ^= rhs.LowPart;
		HighPart ^= rhs.HighPart;
		return *this;
	}
	
	const UIntBuilder& winx_call operator&=(const UIntBuilder& rhs)
	{
		LowPart &= rhs.LowPart;
		HighPart &= rhs.HighPart;
		return *this;
	}
	
	const UIntBuilder& winx_call operator|=(const UIntBuilder& rhs)
	{
		LowPart |= rhs.LowPart;
		HighPart |= rhs.HighPart;
		return *this;
	}
	
	const UIntBuilder& winx_call operator>>=(UINT nShift);
	const UIntBuilder& winx_call operator<<=(UINT nShift);
	
	friend const UIntBuilder winx_call operator+(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		UIntBuilder result = lhs;
		Utils::plus(result, rhs);
		return result;
	}
	
	friend const UIntBuilder winx_call operator-(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		UIntBuilder result = lhs;
		Utils::minus(result, rhs);
		return result;
	}
	
	friend const UIntBuilder winx_call operator*(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		ParentUIntType result;
		Utils::multiply(result, lhs, rhs);
		return result.LowPart;
	}
	
	friend const UIntBuilder winx_call operator/(const UIntBuilder& lhs, const UIntBuilder& rhs);
	friend const UIntBuilder winx_call operator%(const UIntBuilder& lhs, const UIntBuilder& rhs);

	friend const UIntBuilder winx_call operator^(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		UIntBuilder result;
		result.LowPart = lhs.LowPart ^ rhs.LowPart;
		result.HighPart = lhs.HighPart ^ rhs.HighPart;
		return result;
	}
	
	friend const UIntBuilder winx_call operator&(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		UIntBuilder result;
		result.LowPart = lhs.LowPart & rhs.LowPart;
		result.HighPart = lhs.HighPart & rhs.HighPart;
		return result;
	}
	
	friend const UIntBuilder winx_call operator|(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		UIntBuilder result;
		result.LowPart = lhs.LowPart | rhs.LowPart;
		result.HighPart = lhs.HighPart | rhs.HighPart;
		return result;
	}
	
	const UIntBuilder winx_call operator<<(UINT nShift) const
	{
		UIntBuilder tmp = *this;
		return tmp <<= nShift;
	}
	
	const UIntBuilder winx_call operator<<(int nShift) const
	{
		UIntBuilder tmp = *this;
		return tmp <<= (UINT)nShift;
	}
	
	const UIntBuilder winx_call operator>>(UINT nShift) const
	{
		UIntBuilder tmp = *this;
		return tmp >>= nShift;
	}
	
	const UIntBuilder winx_call operator>>(int nShift) const
	{
		UIntBuilder tmp = *this;
		return tmp >>= (UINT)nShift;
	}
	
	friend bool winx_call operator==(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return (lhs.LowPart == rhs.LowPart) && (lhs.HighPart == rhs.HighPart);
	}
	
	friend bool winx_call operator!=(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return (lhs.LowPart != rhs.LowPart) || (lhs.HighPart != rhs.HighPart);
	}
	
	friend bool winx_call operator<(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return (lhs.HighPart < rhs.HighPart) ||
			(lhs.HighPart == rhs.HighPart && lhs.LowPart < rhs.LowPart);
	}
	
	friend bool winx_call operator>(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return (rhs.HighPart < lhs.HighPart) ||
			(rhs.HighPart == lhs.HighPart && rhs.LowPart < lhs.LowPart);
	}
	
	friend bool winx_call operator<=(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return (lhs.HighPart < rhs.HighPart) ||
			(lhs.HighPart == rhs.HighPart && lhs.LowPart <= rhs.LowPart);
	}
	
	friend bool winx_call operator>=(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return (rhs.HighPart < lhs.HighPart) ||
			(rhs.HighPart == lhs.HighPart && rhs.LowPart <= lhs.LowPart);
	}
};

// -------------------------------------------------------------------------
// class UIntBuilder<UINT32>

template <>
class UIntBuilder<UINT32> : public UINT_BUILDER<UINT32>
{
private:
	typedef UINT32 UIntType;
	typedef UIntBuilder<UIntType> Myt_;
	typedef UIntUtils<Myt_> Utils;

public:
	typedef UIntType BaseUIntType;
	typedef UIntBuilder<Myt_> ParentUIntType;
	typedef typename Utils::CellUIntType CellUIntType;
	typedef INT32 CellIntType;

public:
	UIntBuilder() {}
	
	UIntBuilder(CellIntType nVal)
	{
		*(UINT64*)this = nVal;
	}
	
	UIntBuilder(CellUIntType nVal)
	{
		*(UINT64*)this = nVal;
	}
	
	UIntBuilder(INT64 nVal)
	{
		*(UINT64*)this = nVal;
	}
	
	UIntBuilder(UINT64 nVal)
	{
		*(UINT64*)this = nVal;
	}
	
public:
	const UIntBuilder winx_call operator~() const
	{
		return ~ *(UINT64*)this;
	}
	
	const UIntBuilder winx_call operator-() const
	{
		return - *(INT64*)this;
	}
	
	const UIntBuilder& winx_call operator+() const
	{
		return *this;
	}

	bool winx_call operator!() const
	{
		return ! *(UINT64*)this;
	}
	
	const UIntBuilder& winx_call operator++()
	{
		++ *(UINT64*)this;
		return *this;
	}
	
	const UIntBuilder winx_call operator++(int)
	{
		UIntBuilder ret = *this;
		++ *(UINT64*)this;
		return ret;
	}

	const UIntBuilder& winx_call operator--()
	{
		-- *(UINT64*)this;
		return *this;
	}
	
	const UIntBuilder winx_call operator--(int)
	{
		UIntBuilder ret = *this;
		-- *(UINT64*)this;
		return ret;
	}
	
public:
	const UIntBuilder& winx_call operator+=(const UIntBuilder& rhs)
	{
		*(UINT64*)this += *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator-=(const UIntBuilder& rhs)
	{
		*(UINT64*)this -= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator*=(const UIntBuilder& rhs)
	{
		*(UINT64*)this *= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator/=(const UIntBuilder& rhs)
	{
		*(UINT64*)this /= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator%=(const UIntBuilder& rhs)
	{
		*(UINT64*)this %= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator^=(const UIntBuilder& rhs)
	{
		*(UINT64*)this ^= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator&=(const UIntBuilder& rhs)
	{
		*(UINT64*)this &= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator|=(const UIntBuilder& rhs)
	{
		*(UINT64*)this |= *(const UINT64*)&rhs;
		return *this;
	}
	
	const UIntBuilder& winx_call operator>>=(UINT nShift)
	{
		*(UINT64*)this >>= nShift;
		return *this;
	}
	
	const UIntBuilder& winx_call operator<<=(UINT nShift)
	{
		*(UINT64*)this <<= nShift;
		return *this;
	}
	
	friend const UIntBuilder winx_call operator+(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs + *(const UINT64*)&rhs;
	}
	
	friend const UIntBuilder winx_call operator-(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs - *(const UINT64*)&rhs;
	}
	
	friend const UIntBuilder winx_call operator*(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs * *(const UINT64*)&rhs;
	}
	
	friend const UIntBuilder winx_call operator/(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs / *(const UINT64*)&rhs;
	}
	
	friend const UIntBuilder winx_call operator%(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs % *(const UINT64*)&rhs;
	}

	friend const UIntBuilder winx_call operator^(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs ^ *(const UINT64*)&rhs;
	}
	
	friend const UIntBuilder winx_call operator&(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs & *(const UINT64*)&rhs;
	}
	
	friend const UIntBuilder winx_call operator|(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs | *(const UINT64*)&rhs;
	}
	
	const UIntBuilder winx_call operator<<(UINT nShift) const
	{
		return *(const UINT64*)this << nShift;
	}
	
	const UIntBuilder winx_call operator<<(int nShift) const
	{
		return *(const UINT64*)this << nShift;
	}
	
	const UIntBuilder winx_call operator>>(UINT nShift) const
	{
		return *(const UINT64*)this >> nShift;
	}
	
	const UIntBuilder winx_call operator>>(int nShift) const
	{
		return *(const UINT64*)this >> nShift;
	}
	
	friend bool winx_call operator==(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs == *(const UINT64*)&rhs;
	}
	
	friend bool winx_call operator!=(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs != *(const UINT64*)&rhs;
	}
	
	friend bool winx_call operator<(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs < *(const UINT64*)&rhs;
	}
	
	friend bool winx_call operator>(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs > *(const UINT64*)&rhs;
	}
	
	friend bool winx_call operator<=(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs <= *(const UINT64*)&rhs;
	}
	
	friend bool winx_call operator>=(const UIntBuilder& lhs, const UIntBuilder& rhs)
	{
		return *(const UINT64*)&lhs >= *(const UINT64*)&rhs;
	}
};

// -------------------------------------------------------------------------
// class UIntUtils<UINT32>

template <>
class UIntUtils<UINT32>
{
public:
	typedef UINT32 CellUIntType;
	typedef CellUIntType CarryUIntType;
	typedef CellUIntType BorrowUIntType;

	typedef UINT16 BaseUIntType;
	typedef UIntBuilder<SELF> ParentUIntType;

public:
	typedef UINT32 SELF;
	typedef UINT64 PARENT;

private:	
	enum { bits = sizeof(SELF) * 8 };
	enum { half_bits = bits / 2 };
	
public:
	enum { radix10_count = 10 };

public:
	static CarryUIntType winx_call plus(SELF& lhs, SELF rhs)
	{
		PARENT result = (PARENT)lhs + rhs;
		lhs = (SELF)result;
		return (CarryUIntType)(result >> bits);
	}
	
	static BorrowUIntType winx_call minus(SELF& lhs, SELF rhs)
	{
		PARENT result = (PARENT)lhs - rhs;
		lhs = (SELF)result;
		return (BorrowUIntType)(result >> (bits*2-1));
	}
	
	static void winx_call multiply(ParentUIntType& result, SELF lhs, SELF rhs)
	{
		PARENT result64 = (PARENT)lhs * (PARENT)rhs;
		result.LowPart = (SELF)result64;
		result.HighPart = (SELF)(result64 >> bits);
	}
};

// -------------------------------------------------------------------------
// UINT128, UINT256, etc

typedef UIntBuilder<UINT32> UINT64_;
typedef UIntBuilder<UINT64_> UINT128;
typedef UIntBuilder<UINT128> UINT256;
typedef UIntBuilder<UINT256> UINT512;
typedef UIntBuilder<UINT512> UINT1024;

// -------------------------------------------------------------------------
// $Log: Builder.h,v $

NS_STDEXT_END

#endif /* STDEXT_INT_BUILDER_H */

