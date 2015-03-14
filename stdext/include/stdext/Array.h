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
// Module: stdext/Array.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-20 15:29:13
// 
// $Id: Array.h,v 1.2 2007/01/10 09:33:50 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARRAY_H
#define STDEXT_ARRAY_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STD_FUNCTIONAL_H
#include "../std/functional.h"
#endif

#ifndef STD_ALGORITHM_H
#include "../std/algorithm.h"
#endif

#ifndef STD_STDEXCEPT_H
#include "../std/stdexcept.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class Array - see boost::array

template <class Type, size_t nElement>
class Array
{
private:
	Type m_data[nElement];
	size_t m_size;

	Array(const Array&);
	void operator=(const Array&);

public:
	typedef Type value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef Type& reference;
	typedef Type* iterator;

	typedef const Type& const_reference;
	typedef const Type* const_iterator;

public:
	Array() : m_size(0) {}

	void winx_call rangecheck(size_type i)
	{
		if (i >= size())
			throw std::range_error("Array<>: index out of range");
	}

	void winx_call clear()	{ m_size = 0; }
	void winx_call resize(size_type n)
	{
		WINX_ASSERT(n <= capacity());
		m_size = n;
	}

	size_type winx_call capacity() const	{ return nElement; }
	size_type winx_call size() const		{ return m_size; }
	bool winx_call empty() const			{ return m_size == 0; }
	bool winx_call full() const				{ return m_size >= nElement; }

    // direct access to data (read-only)
    const value_type* winx_call data() const { return m_data; }

	value_type* winx_call c_array()			{ return m_data; }

    iterator winx_call begin()				{ return m_data; }
    const_iterator winx_call begin() const	{ return m_data; }

    iterator winx_call end()				{ return m_data+m_size; }
    const_iterator winx_call end() const	{ return m_data+m_size; }

	// at() with range check
    reference winx_call at(size_type i)				 { rangecheck(i); return m_data[i]; }
    const_reference winx_call  at(size_type i) const { rangecheck(i); return m_data[i]; }

	reference winx_call front()				{ return (*begin()); }
	const_reference winx_call front() const	{ return (*begin()); }

	reference winx_call back()				{ return (*(end() - 1)); }
	const_reference winx_call back() const	{ return (*(end() - 1)); }

	reference winx_call operator[](size_type i)
	{
		WINX_ASSERT( i < size() && "out of range" );
		return m_data[i];
	}

	const_reference winx_call operator[](size_type i) const 
    {
		WINX_ASSERT( i < size() && "out of range" );
		return m_data[i];
    }

	void winx_call insert(iterator it, const_reference val)
	{
		WINX_ASSERT(it >= begin() && it <= end());
		WINX_ASSERT(!full());

		std::copy_backward(it, end(), end()+1);
		*it = val;
		++m_size;
	}

	void winx_call erase(iterator it)
	{
		WINX_ASSERT(it >= begin() && it <= end());
		
		std::copy(it+1, end(), it);
		--m_size;
	}

	void winx_call push_back(const_reference val)
	{
		WINX_ASSERT(!full());
		
		m_data[m_size++] = val;
	}

	void winx_call pop_back()
	{
		WINX_ASSERT(!empty());

		--m_size;
	}

	void winx_call copy(const Array& from)
	{
		m_size = from.m_size;
		std::copy(from.m_data, from.m_data + m_size, m_data);
	}
};

// -------------------------------------------------------------------------
// priority_push_back

template <class VectorT>
inline void winx_call priority_push_back(
	VectorT& cont, const typename VectorT::value_type& val, const size_t countLim)
{
	WINX_ASSRET(cont.capacity() >= countLim);

	if (cont.size() >= countLim)
	{
		if (cont.back() < val)
			return;
		cont.pop_back();
	}
	const typename VectorT::iterator it = std::lower_bound(cont.begin(), cont.end(), val);
	cont.insert(it, val);
}

template <class VectorT, class PredT>
inline void winx_call priority_push_back(
	VectorT& cont, const typename VectorT::value_type& val, const size_t countLim, PredT pred)
{
	WINX_ASSRET(cont.capacity() >= countLim);

	if (cont.size() >= countLim)
	{
		if (pred(cont.back(), val))
			return;
		cont.pop_back();
	}
	const typename VectorT::iterator it = std::lower_bound(cont.begin(), cont.end(), val, pred);
	cont.insert(it, val);
}

// -------------------------------------------------------------------------
// class PriorityArray

template <class Type, size_t nElement, class Pred = std::less<Type> >
class PriorityArray : public Array<Type, nElement>
{
private:
	Pred m_pred;

public:
	void winx_call push_back(const Type& val)
	{
		priority_push_back(*this, val, nElement, m_pred);
	}
};

// -------------------------------------------------------------------------
// class TestPriorityArray

template <class LogT>
class TestPriorityArray
{
	WINX_TEST_SUITE(TestPriorityArray);
		WINX_TEST(testLess);
		WINX_TEST(testGreater);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	void testLess(LogT& log)
	{
		const int data[] = { 3, 5, 4, 2, 5, 6, 7, 1, 3, 1 };
		const int dataSel[] = { 1, 1, 2 };
		
		PriorityArray<int, 3> pa;
		for (size_t i = 0; i < countof(data); ++i)
			pa.push_back(data[i]);

		AssertExp(pa.size() == 3);
		AssertEqBuf(pa.begin(), dataSel, 3);
	}

	void testGreater(LogT& log)
	{
		const int data[] = { 3, 5, 4, 2, 5, 6, 7, 1, 3, 1 };
		const int dataSel[] = { 7, 6, 5 };
		
		PriorityArray<int, 3, greater<int> > pa;
		for (size_t i = 0; i < countof(data); ++i)
			pa.push_back(data[i]);

		AssertExp(pa.size() == 3);
		AssertEqBuf(pa.begin(), dataSel, 3);
	}
};

// -------------------------------------------------------------------------
// $Log: Array.h,v $
// Revision 1.2  2007/01/10 09:33:50  xushiwei
// class PriorityArray
//
// Revision 1.1  2006/12/20 08:41:43  xushiwei
// STL-Extension: Container(Array), Log(MultiStorage, MultiLog)
//

NS_STDEXT_END

#endif /* STDEXT_ARRAY_H */
