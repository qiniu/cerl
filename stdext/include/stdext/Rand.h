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
// Module: stdext/Rand.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Rand.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_RAND_H
#define STDEXT_RAND_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#if defined(X_CC_VC_NET)
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max
#elif defined(min)
#undef min
#undef max
#endif

// -------------------------------------------------------------------------
// class BasicRand

NS_STDEXT_BEGIN

__forceinline int winx_call getTimeSeed()
{
	LARGE_INTEGER tick;
	QueryPerformanceCounter(&tick);
	return tick.LowPart;
}

template <class RandGenerator> 
class BasicRand
{
private:
	RandGenerator m_gen;

public:
	typedef size_t size_type;
	typedef typename RandGenerator::result_type value_type;

public:
	BasicRand()	{ seed(); }
	BasicRand(int nSeed)
	{
		seed(nSeed);
	}

	void winx_call seed()
	{
		m_gen.seed(getTimeSeed());
	}

	void winx_call seed(IN int nSeed)
	{
		m_gen.seed(nSeed);
	}

public:
	//
	// Integer
	//

	value_type winx_call begin() const
	{
		return m_gen.min();
	}

	value_type winx_call end() const
	{
		return m_gen.max() + 1;
	}

	value_type winx_call size() const
	{
		return m_gen.max() + 1 - m_gen.min();
	}

	value_type winx_call rand() // [begin(), end())
	{
		return m_gen();
	}

	value_type winx_call rand(value_type nMax) // [0, nMax)
	{
		return m_gen() % nMax;
	}

	value_type winx_call rand(value_type nValMin, value_type nValMax) // [nValMin, nValMax)
	{
		WINX_ASSERT(nValMax > nValMin);

		return m_gen() % (nValMax - nValMin) + nValMin;
	}

public:
	//
	// Float
	//

	double winx_call frand() // [0, 1.0)
	{
		value_type maxVal = size() / 2;

		return (double)rand(maxVal) / maxVal;
	}

	double winx_call frand(double dblMin, double dblMax) // [dblMin, dblMax)
	{
		WINX_ASSERT(dblMin < dblMax);

		return (dblMax - dblMin) * frand() + dblMin;
	}

public:
	//
	// Integer Array
	//

	template <class ValType>
	void winx_call rand(
		OUT ValType szValArray[],
		IN const size_type nCount,
		IN const ValType szValset[],
		IN const size_type szValsetCount
		)
	{
		WINX_ASSERT(szValsetCount > 0);

		for (size_type i = 0; i < nCount; ++i)
		{
			szValArray[i] = szValset[m_gen() % szValsetCount];
		}
	}

	template <class IntType>
	void winx_call rand(
		OUT IntType szValArray[],
		IN const size_type nCount,
		IN const value_type nValMin,
		IN const value_type nValMax
		)
	{
		WINX_ASSERT(nValMax > nValMin);
		
		for (size_type i = 0; i < nCount; ++i)
		{
			szValArray[i] = rand(nValMin, nValMax);
		}
	}

	//----> return: nSrcLen * nRepeat
	template <class ValueType>
	size_type winx_call rep_rand(
		OUT ValueType* szValArray, // size: nSrcLen * nRepeat
		IN const ValueType szSrcToRepeat[],
		IN const size_type nSrcLen,
		IN const size_type nRepeat
		)
	{
		WINX_ASSERT(nRepeat >= 1 && nSrcLen > 0);
		
		std::copy(szSrcToRepeat, szSrcToRepeat + nSrcLen, szValArray);
		
		size_type nDestLen = nSrcLen;
		for (int i = 1; i < nRepeat; ++i)
		{
			int iInsert = rand(nDestLen + 1);
			std::copy_backward(
				szValArray + iInsert,
				szValArray + nDestLen,
				szValArray + nDestLen + nSrcLen
				);
			std::copy(
				szSrcToRepeat,
				szSrcToRepeat + nSrcLen,
				szValArray + iInsert
				);
			nDestLen += nSrcLen;
		}
		
		WINX_ASSERT(nDestLen == nSrcLen * nRepeat);
		return nDestLen;
	}

public:
	//
	// Float Array
	//

	void winx_call frand(
		OUT double dblValArray[],
		IN const size_type nCount)
	{
		for (size_type i = 0; i < nCount; ++i)
		{
			dblValArray[i] = frand();
		}
	}

	void winx_call frand(
		OUT double dblValArray[],
		IN const size_type nCount,
		IN const double dblValMin,
		IN const double dblValMax
		)
	{
		WINX_ASSERT(dblValMax > dblValMin);
		
		for (size_type i = 0; i < nCount; ++i)
		{
			dblValArray[i] = frand(dblValMin, dblValMax);
		}
	}

public:
	//
	// String
	//

	template <class CharType>
	void winx_call srand(
		OUT CharType szBuf[],
		IN const size_type nBufSize,
		IN const value_type nValMin,
		IN const value_type nValMax
		)
	{
		if (nBufSize <= 0)
			return;

		rand(szBuf, nBufSize-1, nValMin, nValMax);
		szBuf[nBufSize-1] = '\0';
	}

	//-----> strlen(szBuf) ¡Ê [nStrLenMin, nBufSize)
	template <class CharType>
	size_type winx_call srand(
		OUT CharType szBuf[],
		IN const size_type nStrLenMin,
		IN const size_type nBufSize,
		IN const value_type nValMin,
		IN const value_type nValMax
		)
	{
		WINX_ASSERT(nStrLenMin < nBufSize);
		WINX_ASSERT(nValMax > nValMin);

		size_type nSize = rand(nStrLenMin, nBufSize);
		WINX_ASSERT(nSize < nBufSize);
		
		srand(szBuf, nSize+1, nValMin, nValMax);
		return nSize;
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class SimpleRand, MinstdRand, Mt19937Rand

#if !defined(_INC_STDLIB) && !defined(_STDLIB_H)
#include <stdlib.h>
#endif

NS_STDEXT_BEGIN

class SimpleRandGen
{
public:
	typedef int result_type;

	static void winx_call seed(int nSeed) { ::srand(nSeed); }

	static result_type winx_call max() { return RAND_MAX; }
	static result_type winx_call min() { return 0; }

	result_type winx_call operator()() const // [min(), max()]
	{
		return ::rand();
	}
};

typedef BasicRand<SimpleRandGen> SimpleRand;

#ifdef BOOST_RANDOM_LINEAR_CONGRUENTIAL_HPP
typedef BasicRand<boost::minstd_rand> MinstdRand;
#endif

#ifdef BOOST_RANDOM_MERSENNE_TWISTER_HPP
typedef BasicRand<boost::mt19937> Mt19937Rand;
#endif

NS_STDEXT_END

#if defined(X_CC_VC_NET)
#pragma pop_macro("min")
#pragma pop_macro("max")
#endif

// -------------------------------------------------------------------------
// class RandGen/Rand

NS_STDEXT_BEGIN

#if defined(BOOST_RANDOM_LINEAR_CONGRUENTIAL_HPP)
typedef boost::minstd_rand RandGen;
typedef MinstdRand Rand;
#elif defined(BOOST_RANDOM_MERSENNE_TWISTER_HPP)
typedef boost::mt19937 RandGen;
typedef Mt19937Rand Rand;
#else
typedef SimpleRandGen RandGen;
typedef SimpleRand Rand;
#endif

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestRand

#if defined(STD_UNITTEST)

NS_STDEXT_BEGIN

template <class LogT>
class TestRand : public TestCase
{
	WINX_TEST_SUITE(TestRand);
		WINX_TEST(testInteger);
		WINX_TEST(testString);
		WINX_TEST(testFloat);
	WINX_TEST_SUITE_END();

private:
	NS_STDEXT::Rand rnd;

public:
	void testInteger(LogT& log)
	{
		NS_STDEXT::Rand::value_type v1 = rnd.rand();
		log.print(v1).newline();
		AssertExp(v1 >= rnd.begin() && v1 < rnd.end());

		NS_STDEXT::Rand::value_type v2 = rnd.rand(2);
		log.print(v2).newline();
		AssertExp(v2 == 0 || v2 == 1);

		NS_STDEXT::Rand::value_type v3 = rnd.rand(30, 100);
		log.print(v3).newline();
		AssertExp(v3 >= 30 && v3 < 100);

		NS_STDEXT::Rand::value_type v4[100];
		rnd.rand(v4, countof(v4), 30, 100);
		log.printArray("%d", v4, countof(v4)).newline();
		log.print(*std::max_element(v4, v4+countof(v4))).newline();
		for (int i = 0; i < countof(v4); ++i)
			AssertExp(v4[i] >= 30 && v4[i] < 100);

		NS_STDEXT::Rand::value_type vals[] = { 1, 3, 5, 7, 9 };
		NS_STDEXT::Rand::value_type v5[100];
		rnd.rand(v5, countof(v5), vals, countof(vals));
		log.printArray("%d", v5, countof(v5)).newline();
		for (int j = 0; j < countof(v5); ++j)
			AssertExp(std::find(vals, vals+countof(vals), v5[j]) != vals+countof(vals));
	}

	void testString(LogT& log)
	{
		char s1[64];
		rnd.srand(s1, countof(s1), 'A', 'z');
		log.print(s1).newline();
		AssertExp(strlen(s1) == countof(s1)-1);

		for (int i = 0; i < 16; ++i)
		{
			rnd.srand(s1, 6, countof(s1), 'A', 'z');
			log.print(s1).newline();
			AssertExp(strlen(s1) >= 6);
		}
	}

	void testFloat(LogT& log)
	{
		double v1 = rnd.frand();
		log.print(v1).newline();
		AssertExp(v1 >= 0 && v1 < 1);

		double v2 = rnd.frand(1.6, 3.2);
		log.print(v2).newline();
		AssertExp(v2 >= 1.6 && v2 < 3.2);

		double v3[200];
		rnd.frand(v3, countof(v3));
		log.printArray("%lf", v3, countof(v3)).newline();
		log.print(*std::min_element(v3, v3+countof(v3)), "%lf").newline();
		log.print(*std::max_element(v3, v3+countof(v3)), "%lf").newline();
	}
};

NS_STDEXT_END

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: Rand.h,v $

#endif /* STDEXT_RAND_H */
