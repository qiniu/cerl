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
// Module: stdext/text/Rope.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Rope.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_ROPE_H
#define STDEXT_TEXT_ROPE_H

#ifndef STDEXT_HASH_H
#include "../Hash.h"
#endif

#ifdef STD_UNITTEST
#include "../HashMap.h"
#endif

#if defined(USE_SGISTL_ROPE) && !defined(NO_SGISTL)

#ifndef STDEXT_TEXT_SGI_ROPE_H
#include "sgi/Rope.h"
#endif

#else

#ifndef STDEXT_TEXT_ROPE_SEQUENCEBUFFER_H
#include "rope/SequenceBuffer.h"
#endif

#ifndef STDEXT_TEXT_ROPE_ROPE_H
#include "rope/Rope.h"
#endif

#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "BasicString.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// Rope::operator+ & Rope::operator+=

template <class _E, class _AllocT, class _T2> __forceinline
Rope<_E,_AllocT> operator+(Rope<_E,_AllocT> __left, const _T2& __right)
{
	return __left.append(__right);
}

template <class _E, class _AllocT, class _T2> __forceinline
Rope<_E,_AllocT>& operator+=(Rope<_E,_AllocT>& __left, const _T2& __right)
{
	return __left.append(__right);
}

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator==(const Rope<_E, _AllocT>& a, const _T2& b)
{return a.compare(b) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const BasicString<_E>& a, const Rope<_E, _AllocT>& b)
{return b.compare(a) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const BasicString<_E>& a, Rope<_E, _AllocT>& b)
{return b.compare(a) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const _E* a, const Rope<_E, _AllocT>& b)
{return b.compare(a) == 0; }

template <class _E, class _AllocT> __forceinline
bool winx_call operator==(const _E* a, Rope<_E, _AllocT>& b)
{return b.compare(a) == 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator!=(const Rope<_E, _AllocT>& a, const _T2& b)
{return a.compare(b) != 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator<(const Rope<_E, _AllocT>& a, const _T2& b)
{return a.compare(b) < 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator>(const Rope<_E, _AllocT>& a, const _T2& b)
{return a.compare(b) > 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator<=(const Rope<_E, _AllocT>& a, const _T2& b)
{return a.compare(b) <= 0; }

// -------------------------------------------------------------------------

template <class _E, class _AllocT, class _T2> __forceinline
bool winx_call operator>=(const Rope<_E, _AllocT>& a, const _T2& b)
{return a.compare(b) >= 0; }

// -------------------------------------------------------------------------
// Hash of Rope class

template <>
class Hash< Rope<char> >
{
public:
	size_t winx_call operator()(const Rope<char>& __str) const
	{
		size_t __size = __str.size();
		if (0 == __size) return 0;
		return 13*__str[0] + 5*__str[__size - 1] + __size;
	}
};

template <>
class Hash< Rope<WCHAR> >
{
public:
	size_t winx_call operator()(const Rope<WCHAR>& __str) const
	{
		size_t __size = __str.size();
		if (0 == __size) return 0;
		return 13*__str[0] + 5*__str[__size - 1] + __size;
	}
};

NS_STDEXT_END

// -------------------------------------------------------------------------
// class TestRope

#if defined(STD_UNITTEST)

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

template <class LogT>
class TestRope : public TestCase
{
	WINX_TEST_SUITE(TestRope);
		WINX_TEST(testBasic);
		WINX_TEST(testSequenceBuffer);
		WINX_TEST(testHash);
		WINX_TEST(testIterator);
		WINX_TEST(testFind);
	WINX_TEST_SUITE_END();

public:
	void testFind(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedPools alloc(recycle);

		NS_STDEXT::Rope<char> a(alloc);
		a.push_back('b');
		a.push_front('a');
		AssertExp(a == "ab");
		AssertExp(a.find("b") == 1);
	}

	void testIterator(LogT& log) // slowly (not recommended)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedPools alloc(recycle);

		NS_STDEXT::Rope<char> a(alloc, "Hello");
		NS_STDEXT::Rope<char> b(alloc, "abc");

		NS_STDEXT::Rope<char>::iterator it = a.mutable_begin();
		*it = 'e';
		*(it+1) = 'f';
		*(it+2) = 'g';
		AssertExp(a == "efglo");
		
		NS_STDEXT::Rope<char>::const_iterator it2 = it;
		AssertExp(*it == 'e');
		AssertExp(*it2 == 'e');
		AssertExp(*(it2+1) == 'f');
		
		std::copy(b.begin(), b.end(), it);
		AssertExp(a == "abclo");

		NS_STDEXT::Rope<char>::const_iterator it3;
		it3 = it;
		AssertExp(*it3 == 'a');

		char buffer[256];
		*std::copy(it, a.mutable_end(), buffer) = '\0';
		AssertExp(strcmp(buffer, "abclo") == 0);
	}

	void testHash(LogT& log)
	{
		typedef NS_STDEXT::Rope<char> KeyT;

		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedPools alloc(recycle);

		NS_STDEXT::HashMap<KeyT, int> cont(alloc);
		
		KeyT a(alloc, "Hello");
		cont[a] = 1;

		KeyT b(alloc, "Hello");
		AssertExp(cont.find(b) != cont.end());
		AssertExp(cont[b] == 1);
	}

	void testSequenceBuffer(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedPools alloc(recycle);
		char buffer[256];

		NS_STDEXT::Rope<char> a(alloc);
		{
			NS_STDEXT::SequenceBuffer<NS_STDEXT::Rope<char>, 8> input(a);
			input.append("Hello");
			input.append(", ");
			input.append("world!");
			input.append(1, '!');
			input.append(4, '?');
		}
		*a.copy(buffer) = '\0';
		AssertExp(a == "Hello, world!!????");
		{
			NS_STDEXT::SequenceBuffer<NS_STDEXT::Rope<char>, 16> input(a);
			input.append(2, ' ');
			input.append("You");
			input.append("'re");
			input.append(" welcome!");
		}
		*a.copy(buffer) = '\0';
		AssertExp(a == "Hello, world!!????  You're welcome!");
	}

	void testBasic(LogT& log)
	{
		NS_STDEXT::BlockPool recycle;
		NS_STDEXT::ScopedPools alloc(recycle);

		NS_STDEXT::Rope<char> a(alloc);
		a.append(20, 'a');
		a.append("bcdefg");

		NS_STDEXT::Rope<char> b(a);
		b.erase(b.size() - 7, 7);
		b.append("abcdefg");
		AssertExp(a.compare(b) == 0);

		NS_STDEXT::Rope<char> c(alloc);
		c = a;
		AssertExp(c == b);

		c = "abc";
		c.mutable_reference_at(0) = 'y';
		AssertExp(c == "ybc");

		log.printObj(c).newline();

#if defined(_OSTREAM_)
		using namespace std;
		c.trace(log);
		c.trace(cout) << "\n";
#endif
	}
};

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

#endif // defined(STD_UNITTEST)

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_ROPE_H */
