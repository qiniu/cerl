/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Array.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Array.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef VENUS_ARRAY_H
#define VENUS_ARRAY_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class Array = boost::array

#pragma pack(1)

template <class Type, size_t m_size>
class Array
{
private:
	Type m_data[m_size];
	
public:
	typedef Type value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef Type& reference;
	typedef Type* iterator;

	typedef const Type& const_reference;
	typedef const Type* const_iterator;
	
public:
	static bool cerl_call empty() { return false; }
	static size_type cerl_call size() { return m_size; }
	static size_type cerl_call max_size() { return m_size; }
	
	static void cerl_call rangecheck(size_type i)
	{
		if (i >= size())
			throw std::range_error("Array<>: index out of range");
	}

	// assign one value to all elements
	void cerl_call assign(const value_type& value)
	{
		std::fill_n(begin(),size(),value);
	}

    const value_type* cerl_call data() const { return m_data; }
    value_type* cerl_call data() 			 { return m_data; }

	value_type* cerl_call c_array()			{ return m_data; }

    iterator cerl_call begin()				{ return m_data; }
    const_iterator cerl_call begin() const	{ return m_data; }

    iterator cerl_call end()				{ return m_data+m_size; }
    const_iterator cerl_call end() const	{ return m_data+m_size; }

	reference cerl_call operator[](size_type i)
	{
		CERL_ASSERT( i < size() && "out of range" );
		return m_data[i];
	}

	const_reference cerl_call operator[](size_type i) const 
    {
		CERL_ASSERT( i < size() && "out of range" );
		return m_data[i];
    }

	bool cerl_call operator==(const Array& b) const
	{
		for (size_t i = 0; i < m_size; ++i) {
			if (m_data[i] != b.m_data[i])
				return false;
		}
		return true;
	}

	bool cerl_call operator!=(const Array& b) const
	{
		return !(*this == b);
	}
};

#pragma pack()

// =========================================================================

NS_CERL_END

#endif /* VENUS_ARRAY_H */
