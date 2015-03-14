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
// Module: stdext/Queue.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-20 15:29:13
// 
// $Id: Queue.h,v 1.2 2007/01/10 09:33:50 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_QUEUE_H
#define STDEXT_QUEUE_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class LimitedQueue

template <class Type>
class LimitedQueue
{
private:
	Type* m_data;
	size_t m_i;
	size_t m_size;
	size_t const nElement;

	LimitedQueue(const LimitedQueue&);
	void operator=(const LimitedQueue&);

public:
	typedef Type value_type;
	typedef size_t size_type;

	typedef Type& reference;
	typedef const Type& const_reference;

public:
	LimitedQueue(Type data[], size_t n)
		: m_data(data), nElement(n),
		  m_i(0), m_size(0)
	{
	}

public:
	void winx_call clear()	{ m_size = 0; }

	size_type winx_call capacity() const	{ return nElement; }
	size_type winx_call size() const		{ return m_size; }
	bool winx_call empty() const			{ return m_size == 0; }
	bool winx_call full() const				{ return m_size >= nElement; }

	reference winx_call front()				{ return m_data[m_i]; }
	const_reference winx_call front() const	{ return m_data[m_i]; }

	reference winx_call back()				{ return m_data[(m_i + m_size - 1) % nElement]; }
	const_reference winx_call back() const	{ return m_data[(m_i + m_size - 1) % nElement]; }

	void winx_call push(const_reference val)
	{
		WINX_ASSERT(!full());
		
		m_data[(m_i + m_size) % nElement] = val;
		++m_size;
	}

	void winx_call pop()
	{
		WINX_ASSERT(!empty());

		m_i = (m_i + 1) % nElement;
		--m_size;
	}

	void winx_call copy(const LimitedQueue& from)
	{
		m_size = from.m_size;
		m_i = from.m_i;
		std::copy(from.m_data, from.m_data + nElement, m_data);
	}
};

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_QUEUE_H */
