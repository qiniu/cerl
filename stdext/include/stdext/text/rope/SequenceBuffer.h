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
// Module: stdext/text/rope/SequenceBuffer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: SequenceBuffer.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_ROPE_SEQUENCEBUFFER_H
#define STDEXT_TEXT_ROPE_SEQUENCEBUFFER_H

#ifndef STDEXT_TEXT_RANGE_H
#include "../Range.h"
#endif

/*
 * Copyright (c) 1997-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class SequenceBuffer

// Sequence buffers:
//
// Sequence must provide an append operation that appends an
// array to the sequence.  Sequence buffers are useful only if
// appending an entire array is cheaper than appending element by element.
// This is true for many string representations.
// This should  perhaps inherit from ostream<sequence::value_type>
// and be implemented correspondingly, so that they can be used
// for formatted.  For the sake of portability, we don't do this yet.
//
// sequence buffers behave a little like basic_ostringstream<sequence::value_type>
// and a little like containers.

template <class _Sequence, size_t _Buf_sz = 512
	#if defined(__sgi) && !defined(__GNUC__)
	#define __SB_TYPEDEF_WORKAROUND
		, class _V = typename _Sequence::value_type
	#endif
	>
// The 3rd parameter works around a common compiler bug.
class SequenceBuffer
{
public:
	#ifndef __SB_TYPEDEF_WORKAROUND
		typedef typename _Sequence::value_type value_type;
	#else
		typedef _V value_type;
	#endif

private:
	SequenceBuffer(const SequenceBuffer&);
	void operator=(const SequenceBuffer&);

protected:
	_Sequence* _M_prefix;
	value_type _M_buffer[_Buf_sz];
	size_t     _M_buf_count;

public:
	SequenceBuffer() : _M_prefix(0), _M_buf_count(0) {}
	SequenceBuffer(_Sequence& __s) : _M_prefix(&__s), _M_buf_count(0) {}
	~SequenceBuffer() { flush(); }
	
	void winx_call flush() {
		_M_prefix->append(_M_buffer, _M_buffer + _M_buf_count);
		_M_buf_count = 0;
	}
	
	void winx_call push_back(value_type __x)
	{
		if (_M_buf_count < _Buf_sz) {
			_M_buffer[_M_buf_count] = __x;
			++_M_buf_count;
		} else {
			flush();
			_M_buffer[0] = __x;
			_M_buf_count = 1;
		}
	}

	SequenceBuffer& winx_call append(const value_type* __s, size_t __len)
	{
		if (__len + _M_buf_count <= _Buf_sz) {
			size_t __i = _M_buf_count;
			size_t __j = 0;
			for (; __j < __len; __i++, __j++) {
				_M_buffer[__i] = __s[__j];
			}
			_M_buf_count += __len;
		} else if (0 == _M_buf_count) {
			_M_prefix->append(__s, __s + __len);
		} else {
			flush();
			append(__s, __len);
		}
		return *this;
	}
	SequenceBuffer& winx_call append(const BasicString<value_type>& s)
	{
		return append(s.data(), s.size());
	}
	SequenceBuffer& winx_call append(size_t __len, const value_type& __val)
	{
		if (__len + _M_buf_count <= _Buf_sz) {
			std::fill_n(_M_buffer + _M_buf_count, __len, __val);
			_M_buf_count += __len;
		} else if (0 == _M_buf_count) {
			_M_prefix->append(__len, __val);
		} else {
			flush();
			append(__len, __val);
		}
		return *this;
	}
	
	SequenceBuffer& winx_call write(const value_type* __s, size_t __len)
	{
		return append(__s, __len);
	}
	
	SequenceBuffer& winx_call put(value_type __x)
	{
		push_back(__x);
		return *this;
	}
};

// -------------------------------------------------------------------------
// $Log: SequenceBuffer.h,v $

NS_STDEXT_END

#endif /* STDEXT_TEXT_ROPE_SEQUENCEBUFFER_H */
