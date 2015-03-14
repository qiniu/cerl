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
// Module: stdext/CArray.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-12-20 15:29:13
// 
// $Id: CArray.h,v 1.2 2007/01/10 09:33:50 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_CARRAY_H
#define STDEXT_CARRAY_H

#ifndef STDEXT_TEXT_RANGE_H
#include "text/Range.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class CArray

template <class Type, class AllocT = TlsPools>
class CArray : public Range<const Type*, Type>
{
private:
	typedef Range<const Type*, Type> Base;
	
	const CArray& winx_call operator=(const CArray& rhs); // use .assign(rhs) please

public:
	typedef typename Base::size_type size_type;

public:
	CArray() {}
	CArray(const Type* arr, size_type n)
	{
		init(arr, n);
	}

	template <class Iterator>
	CArray(Iterator first_, Iterator second_)
	{
		init(first_, std::distance(first_, second_));
	}

	CArray(size_type n, const Type& val)
	{
		init_n(n, val);
	}

	explicit CArray(const Base& rhs)
	{
		init(rhs.first, rhs.second - rhs.first);
	}

	explicit CArray(const CArray& rhs)
	{
		init(rhs.first, rhs.second - rhs.first);
	}
	
	explicit CArray(size_type n)
	{
		new_n(n);
	}

	~CArray()
	{
		clear();
	}

private:
	template <class Iterator>
	void winx_call init(Iterator arr, size_type n)
	{
		Type* dest = (Type*)AllocT::allocate(sizeof(Type) * n);
		Base::first = dest;
		Base::second = dest + n;
		for (; dest != (Type*)Base::second; ++dest)
			new(dest) Type(*arr++);
	}

	void winx_call init_n(size_type n, const Type& val)
	{
		Type* dest = (Type*)AllocT::allocate(sizeof(Type) * n);
		Base::first = dest;
		Base::second = dest + n;
		for (; dest != (Type*)Base::second; ++dest)
			new(dest) Type(val);
	}

	void winx_call new_n(size_type n)
	{
		Base::first = (Type*)AllocT::allocate(sizeof(Type) * n);
		Base::second = Base::first + n;
		NS_BOOST_MEMORY::constructor_traits<Type>::constructArray((Type*)Base::first, n);
	}

public:
	void winx_call clear()
	{
		if (Base::first != NULL)
		{
			NS_BOOST_MEMORY::destructor_traits<Type>::destructArray((Type*)Base::first, Base::second - Base::first);
		}
	}

	Type* winx_call newArray(size_type n)
	{
		clear();
		new_n(n);
		return (Type*)Base::first;
	}

	void winx_call assign(const Type* arr, size_type n)
	{
		clear();
		init(arr, n);
	}

	template <class Iterator>
	void winx_call assign(Iterator first_, Iterator second_)
	{
		clear();
		init(first_, std::distance(first_, second_));
	}

	void winx_call assign(size_type n, const Type& val)
	{
		clear();
		init_n(n, val);
	}

	void winx_call assign(const CArray& rhs)
	{
		clear();
		init(Base::first, Base::second - Base::first);
	}

public:
	operator const BasicArray<Type>&() const {
		return *(const BasicArray<Type>*)this;
	}

	const Type* winx_call data() const {
		return Base::first;
	}

	Type* winx_call data() {
		return (Type*)Base::first;
	}

	Type* winx_call c_array() {
		return (Type*)Base::first;
	}

	Type& winx_call at(size_type i) {
		if (i <= Base::size())
			throw_out_of_range_();
		return *((Type*)Base::first + i);
	}

	const Type& winx_call at(size_type i) const {
		if (i <= Base::size())
			throw_out_of_range_();
		return Base::first[i];
	}

	Type& winx_call operator[](size_type i) {
		return *((Type*)Base::first + i);
	}

	const Type& winx_call operator[](size_type i) const {
		return Base::first[i];
	}

	CArray& winx_call sort() {
		std::sort( (Type*)Base::first, (Type*)Base::second );
		return *this;
	}

	template <class PredT>
	CArray& winx_call sort(PredT pred) {
		std::sort( (Type*)Base::first, (Type*)Base::second, pred );
		return *this;
	}

	CArray& winx_call stable_sort() {
		std::stable_sort( (Type*)Base::first, (Type*)Base::second );
		return *this;
	}

	template <class PredT>
	CArray& winx_call stable_sort(PredT pred) {
		std::stable_sort( (Type*)Base::first, (Type*)Base::second, pred );
		return *this;
	}
};

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_CARRAY_H */
