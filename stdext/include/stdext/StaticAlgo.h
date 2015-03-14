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
// Module: stdext/StaticAlgo.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: StaticAlgo.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_STATICALGO_H
#define STDEXT_STATICALGO_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct StaticLog2

template <int V>
struct StaticLog2
{
	enum { value = 1 + StaticLog2<V/2>::value };
};

template <>
struct StaticLog2<1>
{

	enum { value = 0 };
};

template <>
struct StaticLog2<0>
{
	// value is undefined!
};

// -------------------------------------------------------------------------
// struct EqualSelect

template <int A, int B, int EqVal = 1, int NotEqVal = 0>
struct EqualSelect
{
	enum { value = NotEqVal };
};

template <int A, int EqVal, int NotEqVal>
struct EqualSelect<A, A, EqVal, NotEqVal>
{
	enum { value = EqVal };
};

// -------------------------------------------------------------------------
// struct MasksTraits

template <int nMasks>
struct MasksTraits
{
	enum { Head = nMasks & ~(nMasks - 1) };
	enum { Tail = nMasks & (nMasks - 1) };
	enum { Count = 1 + MasksTraits<Tail>::Count };
};

template <>
struct MasksTraits<0>
{
	enum { Count = 0 };
};

// -------------------------------------------------------------------------
// struct MakeMasks

#define WINX_INDEX_NULL				(-1)
#define WINX_INDEX_TO_MASK_(iBit)	IndexToMask_<iBit>::value

template <int iBit>
struct IndexToMask_
{
	enum { value = 1 << iBit };
};

template <>
struct IndexToMask_<WINX_INDEX_NULL>
{
	enum { value = 0 };
};

template <
	int M0 = WINX_INDEX_NULL,
	int M1 = WINX_INDEX_NULL,
	int M2 = WINX_INDEX_NULL,
	int M3 = WINX_INDEX_NULL,
	int M4 = WINX_INDEX_NULL,
	int M5 = WINX_INDEX_NULL,
	int M6 = WINX_INDEX_NULL,
	int M7 = WINX_INDEX_NULL,
	int M8 = WINX_INDEX_NULL,
	int M9 = WINX_INDEX_NULL
	>
struct MakeMasks
{
	enum { value = 
		WINX_INDEX_TO_MASK_(M0) | WINX_INDEX_TO_MASK_(M1) |
		WINX_INDEX_TO_MASK_(M2) | WINX_INDEX_TO_MASK_(M3) |
		WINX_INDEX_TO_MASK_(M4) | WINX_INDEX_TO_MASK_(M5) |
		WINX_INDEX_TO_MASK_(M6) | WINX_INDEX_TO_MASK_(M7) |
		WINX_INDEX_TO_MASK_(M8) | WINX_INDEX_TO_MASK_(M9)
		}; 
};

// -------------------------------------------------------------------------
// struct MasksIndexOfMask

template <int iMask, int iHead, int nMasks>
struct MasksIndexOfMask_
{
	enum { Head_ = MasksTraits<nMasks>::Head };
	enum { Tail_ = MasksTraits<nMasks>::Tail };
	
	enum { value = 1 + MasksIndexOfMask_<iMask, Head_, Tail_>::value };
};

template <int iMask, int nMasks>
struct MasksIndexOfMask_<iMask, iMask, nMasks>
{
	enum { value = 0 };
};

template <int iMask, int nMasks>
struct MasksIndexOfMask_<iMask, 0, nMasks>
{
	// value is undefined!
};

template <int iMask, int nMasks>
struct MasksIndexOfMask
{
	enum { Head_ = MasksTraits<nMasks>::Head };
	enum { Tail_ = MasksTraits<nMasks>::Tail };
	
	enum { value = MasksIndexOfMask_<iMask, Head_, Tail_>::value };
};

// -------------------------------------------------------------------------
// struct MasksIndexOfBit

template <int iBit, int nMasks>
struct MasksIndexOfBit
{
	enum { value = MasksIndexOfMask<(1 << iBit), nMasks>::value };
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestStaticAlgo

#if defined(STD_UNITTEST)

template <class LogT>
class TestStaticAlgo : public TestCase
{
	WINX_TEST_SUITE(TestStaticAlgo);
		WINX_TEST(testCount);
		WINX_TEST(testHeadTail);
		WINX_TEST(testLog2);
		WINX_TEST(testMakeMasks);
		WINX_TEST(testMasksIndex);
	WINX_TEST_SUITE_END();

public:
	void testLog2(LogT& log)
	{
		using namespace NS_STDEXT;

		AssertExp(StaticLog2<1>::value == 0);
		AssertExp(StaticLog2<2>::value == 1);
		AssertExp(StaticLog2<3>::value == 1);
		AssertExp(StaticLog2<4>::value == 2);
		AssertExp(StaticLog2<5>::value == 2);
		AssertExp(StaticLog2<6>::value == 2);
		AssertExp(StaticLog2<7>::value == 2);
		AssertExp(StaticLog2<8>::value == 3);
	}
	
	void testMakeMasks(LogT& log)
	{
		using namespace NS_STDEXT;

		AssertExp((MakeMasks<0, 1>::value) == 3);
		AssertExp((MakeMasks<0, 2>::value) == 5);
		AssertExp((MakeMasks<2, 4>::value) == 20);
		AssertExp((MakeMasks<0, 1, 2>::value) == 7);
	}
	
	void testMasksIndex(LogT& log)
	{
		using namespace NS_STDEXT;

		AssertExp((MasksIndexOfBit<1, MakeMasks<0, 1>::value>::value) == 1);
		AssertExp((MasksIndexOfBit<2, MakeMasks<2, 4>::value>::value) == 0);
		AssertExp((MasksIndexOfBit<4, MakeMasks<0, 1, 2, 4>::value>::value) == 3);
	}
	
	void testHeadTail(LogT& log)
	{
		using namespace NS_STDEXT;

		AssertExp(MasksTraits<1>::Head == 1);
		AssertExp(MasksTraits<1>::Tail == 0);

		AssertExp(MasksTraits<2>::Head == 2);
		AssertExp(MasksTraits<2>::Tail == 0);

		AssertExp(MasksTraits<4>::Head == 4);
		AssertExp(MasksTraits<4>::Tail == 0);
		
		AssertExp(MasksTraits<8>::Head == 8);
		AssertExp(MasksTraits<8>::Tail == 0);

		AssertExp(MasksTraits<16>::Head == 16);
		AssertExp(MasksTraits<16>::Tail == 0);

		AssertExp(MasksTraits<3>::Head == 1);
		AssertExp(MasksTraits<3>::Tail == 2);

		AssertExp(MasksTraits<5>::Head == 1);
		AssertExp(MasksTraits<5>::Tail == 4);

		AssertExp(MasksTraits<6>::Head == 2);
		AssertExp(MasksTraits<6>::Tail == 4);

		AssertExp(MasksTraits<7>::Head == 1);
		AssertExp(MasksTraits<7>::Tail == 6);

		AssertExp(MasksTraits<15>::Head == 1);
		AssertExp(MasksTraits<15>::Tail == 14);
	}
	
	void testCount(LogT& log)
	{
		using namespace NS_STDEXT;

		AssertExp(MasksTraits<1>::Count == 1);
		AssertExp(MasksTraits<2>::Count == 1);
		AssertExp(MasksTraits<4>::Count == 1);
		AssertExp(MasksTraits<8>::Count == 1);
		AssertExp(MasksTraits<16>::Count == 1);

		AssertExp(MasksTraits<3>::Count == 2);
		AssertExp(MasksTraits<5>::Count == 2);
		AssertExp(MasksTraits<6>::Count == 2);
		AssertExp(MasksTraits<9>::Count == 2);

		AssertExp(MasksTraits<7>::Count == 3);
		AssertExp(MasksTraits<15>::Count == 4);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: StaticAlgo.h,v $

#endif /* STDEXT_STATICALGO_H */

