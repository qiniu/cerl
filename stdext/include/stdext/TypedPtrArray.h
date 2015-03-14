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
// Module: stdext/TypedPtrArray.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: TypedPtrArray.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TYPEDPTRARRAY_H
#define STDEXT_TYPEDPTRARRAY_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STD_VECTOR_H
#include "../std/vector.h"
#endif

#ifndef STD_ALGORITHM_H
#include "../std/algorithm.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class TypedPtrArray

#define STDEXT_PTRARRAY_USING__												\
public:																		\
	using _Base::size;														\
	using _Base::resize;													\
	using _Base::empty;														\
	using _Base::reserve;													\
	using _Base::pop_back;													\
	using _Base::clear

template <class _PtrType>
class TypedPtrArray : private std::vector<void*>
{
private:
	typedef std::vector<void*> _Base;
	typedef TypedPtrArray<_PtrType> _Myt;
	STDEXT_PTRARRAY_USING__;

private:
	TypedPtrArray(const TypedPtrArray&);
	void operator=(const TypedPtrArray&);

public:
	typedef _Base::size_type size_type;
	typedef _PtrType value_type;
	
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef pointer iterator;
	typedef const_pointer const_iterator;

	TypedPtrArray() {}

	VOID winx_call swap(_Myt& o)
	{
		_Base::swap((_Base&)o);
	}

	void winx_call binary_insert(value_type val)
	{
		_Base::insert(
			std::lower_bound(_Base::begin(), _Base::end(), (void*)val), val);
	}
	
	const_iterator winx_call binary_find(value_type val) const
	{
		_Base::iterator itEnd = _Base::end();
		_Base::iterator it = std::lower_bound(_Base::begin(), itEnd, (void*)val);
		if (it != itEnd && (*it) == val)
			return it;
		else
			return itEnd;
	}

	size_type winx_call erase_unique(value_type val, size_type iEraseFrom = 0)
	{
		for (size_type i = iEraseFrom; i < _Base::size(); ++i)
		{
			if (_Base::at(i) == (void*)val)
			{
				_Base::erase(_Base::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	template <class _Ty, class _Equal>
	size_type winx_call erase_unique(_Ty val, _Equal eq, size_type iEraseFrom = 0)
	{
		for (size_type i = iEraseFrom; i < _Base::size(); ++i)
		{
			if (eq((value_type)_Base::at(i), val))
			{
				_Base::erase(_Base::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	bool winx_call pop_back(value_type* val)
	{
		if (_Base::empty())
			return false;
		else
		{
			*val = (value_type)_Base::back();
			_Base::pop_back();
			return true;
		}
	}

	void winx_call push_back(value_type val)
		{ _Base::push_back(val); }
	
	const_reference winx_call at(size_type i) const
		{ return (const_reference)_Base::at(i); }

	reference winx_call at(size_type i)
		{ return (reference)_Base::at(i); }

	reference winx_call front()
		{ return (reference)_Base::front(); }

	const_reference winx_call front() const
		{ return (const_reference)_Base::front(); }

	reference winx_call back()
		{ return (reference)_Base::back(); }

	const_reference winx_call back() const
		{ return (const_reference)_Base::back(); }

	iterator winx_call begin()
		{ return (iterator)iterToPointer(_Base::begin()); }

	iterator winx_call end()
		{ return (iterator)iterToPointer(_Base::end()); }

	const_iterator winx_call begin() const
		{ return (const_iterator)iterToPointer(_Base::begin()); }

	const_iterator winx_call end() const
		{ return (const_iterator)iterToPointer(_Base::end()); }

	void winx_call copy(const _Myt& from) {
		_Base::operator=(from);
	}
};

// -------------------------------------------------------------------------
// class InterfaceArray

#define STDEXT_INTERFACE_ARRAY_USING__										\
public:																		\
	using _Base::size;														\
	using _Base::empty;														\
	using _Base::reserve

template <class Interface>
class InterfaceArray : private std::vector<void*>
{
private:
	typedef std::vector<void*> _Base;
	typedef Interface* _PtrType;
	typedef InterfaceArray<Interface> _Myt;
	STDEXT_INTERFACE_ARRAY_USING__;

private:
	InterfaceArray(const InterfaceArray&);
	void operator=(const InterfaceArray&);

public:
	typedef _Base::size_type size_type;
	typedef _PtrType value_type;
	
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	
	typedef value_type& reference;
	typedef const value_type& const_reference;

	typedef pointer iterator;
	typedef const_pointer const_iterator;

	InterfaceArray() {}
	~InterfaceArray() { clear(); }

	void winx_call swap(const _Myt& o)
	{
		_Base::swap((_Base&)o);
	}

	void winx_call binary_insert(value_type val)
	{
		_Base::insert(
			std::lower_bound(_Base::begin(), _Base::end(), (void*)val), val);
		if (val)
			val->AddRef();
	}
	
	const_iterator winx_call binary_find(value_type val) const
	{
		_Base::const_iterator itEnd = _Base::end();
		_Base::const_iterator it = std::lower_bound(_Base::begin(), itEnd, (void*)val);
		if (it != itEnd && (*it) == val)
			return it;
		else
			return itEnd;
	}

	bool winx_call exist(value_type val) const
	{
		_Base::const_iterator itEnd = _Base::end();
		return std::find(_Base::begin(), itEnd, (void*)val) != itEnd;
	}

	const_iterator winx_call find(value_type val) const
	{
		return std::find(_Base::begin(), _Base::end(), (void*)val);
	}

	size_type winx_call insert_unique(value_type val)
	{
		_Base::iterator itEnd = _Base::end();
		_Base::iterator it = std::find(_Base::begin(), itEnd, (void*)val);
		if (it != itEnd)
			return 0;

		_Base::push_back(val);
		if (val)
			val->AddRef();
		return 1;
	}

	size_type winx_call erase_unique(value_type val)
	{
		_Base::iterator itEnd = _Base::end();
		_Base::iterator it = std::find(_Base::begin(), itEnd, (void*)val);
		if (it == itEnd)
			return 0;

		_Base::erase(it);
		if (val)
			val->Release();
		return 1;
	}
	
	size_type winx_call erase_unique(value_type val, size_type iEraseFrom)
	{
		for (size_type i = iEraseFrom; i < _Base::size(); ++i)
		{
			if (_Base::at(i) == (void*)val)
			{
				if (val)
					val->Release();
				_Base::erase(_Base::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	template <class _Ty, class _Equal>
	size_type winx_call erase_unique(
		_Ty val, _Equal eq, size_type iEraseFrom = 0, value_type* itemDetach = NULL)
	{
		for (size_type i = iEraseFrom; i < _Base::size(); ++i)
		{
			value_type item = (value_type)_Base::at(i);
			if (eq(item, val))
			{
				if (itemDetach)
					*itemDetach = item;
				else if (item)
					item->Release();
				_Base::erase(_Base::begin() + i);
				return 1;
			}
		}
		return 0;
	}

	bool winx_call pop_back(value_type* val)
	{
		if (_Base::empty())
			return false;
		else
		{
			*val = (value_type)_Base::back();
			_Base::pop_back();
			return true;
		}
	}
	
	void winx_call pop_back()
	{
		value_type val = (value_type)_Base::back();
		_Base::pop_back();
		if (val)
			val->Release();
	}

	void winx_call push_back(value_type val)
	{
		_Base::push_back(val);
		if (val)
			val->AddRef();
	}

	void winx_call attach_push_back(value_type val)
	{
		_Base::push_back(val);
	}
	
	void winx_call attach_insert(size_type pos, value_type val)
	{
		_Base::insert(_Base::begin() + pos, val);
	}

	void winx_call resize(size_type n)
	{
		for (size_type i = n; i < _Base::size(); ++i)
		{
			value_type item = (value_type)_Base::at(i);
			if (item)
				item->Release();
		}
		_Base::resize(n);
	}

	void winx_call clear()
	{
		for (_Base::iterator it = _Base::begin(); it != _Base::end(); ++it)
		{
			value_type item = (value_type)*it;
			if (item)
				item->Release();
		}
		_Base::clear();
	}

	template <class _Ty>
	long winx_call item(size_type i, _Ty* val)
	{
		if (i < _Base::size())
		{
			*val = (value_type)_Base::at(i);
			if (*val)
				(*val)->AddRef();
			return 0;
		}
		return 0x80000003L; // E_INVALIDARG - no need include <winerror.h>
	}

	const_reference winx_call at(size_type i) const
		{ return (const_reference)_Base::at(i); }

	reference winx_call at(size_type i)
		{ return (reference)_Base::at(i); }

	reference winx_call front()
		{ return (reference)_Base::front(); }

	const_reference winx_call front() const
		{ return (const_reference)_Base::front(); }

	reference winx_call back()
		{ return (reference)_Base::back(); }

	const_reference winx_call back() const
		{ return (const_reference)_Base::back(); }

	iterator winx_call begin()
		{ return (iterator)iterToPointer(_Base::begin()); }

	iterator winx_call end()
		{ return (iterator)iterToPointer(_Base::end()); }

	const_iterator winx_call begin() const
		{ return (const_iterator)iterToPointer(_Base::begin()); }

	const_iterator winx_call end() const
		{ return (const_iterator)iterToPointer(_Base::end()); }
};

// -------------------------------------------------------------------------
// $Log: TypedPtrArray.h,v $

NS_STDEXT_END

#endif /* STDEXT_TYPEDPTRARRAY_H */
