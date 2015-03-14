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
// Module: stdext/kmp/TestFinder.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-2 15:53:51
// 
// $Id: TestFinder.h,v 1.2 2006/12/03 07:52:27 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_KMP_TESTFINDER_H
#define STDEXT_KMP_TESTFINDER_H

#ifndef STDEXT_KMP_REFFINDER_H
#include "RefFinder.h"
#endif

#ifndef STD_DEQUE_H
#include "../../std/deque.h"
#endif

#ifndef _FSTREAM_
#include <fstream>
#endif

// -------------------------------------------------------------------------

#if defined(STD_UNITTEST)

template <class LogT>
class TestFinder : public TestCase
{
public:
	WINX_TEST_SUITE(TestFinder);
		WINX_TEST(testSearchInCStr);
		WINX_TEST(testSearchInArchive);
		WINX_TEST(testSearchInDeque);
		WINX_TEST(testSearchInFStream);
	WINX_TEST_SUITE_END();

public:
	void testSearchInArchive(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedAlloc alloc(recycle);

		std::string line;
		NS_STDEXT::StdioReader ar(__FILE__);

		NS_STDEXT::kmp::Finder<char> finder("NS_STDEXT::kmp::Finder<char>");
		HRESULT hr = finder.next(ar);
		AssertExp(hr == S_OK);

		ar.getline(line);
		log.trace("\nline =%s\n", line.c_str());
	}

	void testSearchInFStream(LogT& log)
	{
		std::string line;
		std::ifstream is(__FILE__);
		
		NS_STDEXT::kmp::Finder<char> finder("std::ifstream");
		HRESULT hr = finder.istreamNext(is);
		AssertExp(hr == S_OK);

		std::getline(is, line);
		log.trace("\nline =%s\n", line.c_str());		
	}

	void testSearchInCStr(LogT& log)
	{
		const char* p;
		const char dest[] = "1234ababcde";
		
		NS_STDEXT::kmp::Finder<char> finder("abc");
		HRESULT hr = finder.cstrNext(dest, &p);
		AssertExp(hr == S_OK);
		AssertExp(strcmp(p, "de") == 0);
	}

	void testSearchInDeque(LogT& log)
	{
		typedef std::deque<char> Container;

		const char destBuf[] = "1234ababcde";
		Container::iterator itFind;
		Container dest(sizeof(destBuf));
		std::copy(destBuf, destBuf+sizeof(destBuf), dest.begin());

		NS_STDEXT::kmp::Finder<char> finder("abc");
		HRESULT hr = finder.iteratorNext(dest.begin(), dest.end(), &itFind);
		AssertExp(hr == S_OK);
		AssertExp(dest.end() - itFind == 3);
	}
};

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------
// $Log: TestFinder.h,v $

#endif /* STDEXT_KMP_TESTFINDER_H */
