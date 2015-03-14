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
// Module: stdext/memory/RecycleBuffer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:46:39
// 
// $Id: RecycleBuffer.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MEMORY_RECYCLEBUFFER_H
#define STDEXT_MEMORY_RECYCLEBUFFER_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class RecycleBuffer

template <class _Ty, int _RoundSize = 64>
class RecycleBuffer
{
private:
	_Ty* m_buf;
	size_t m_size;
	
public:
	typedef _Ty* pointer;

public:
	RecycleBuffer() : m_size(0), m_buf(NULL) {}
	explicit RecycleBuffer(size_t nInitial)
		: m_size(nInitial)
	{
		m_buf = (_Ty*)malloc(sizeof(_Ty)*m_size);
	}
	~RecycleBuffer()
	{
		if (m_buf)
			free(m_buf);
	}
	
	pointer winx_call data() const {
		return m_buf;
	}
	
	pointer winx_call begin() const {
		return m_buf;
	}
	
	pointer winx_call reserve(size_t nSize)
	{
		if (nSize > m_size) {
			free(m_buf);
			m_size = ROUND(nSize, _RoundSize);
			m_buf = (_Ty*)malloc(sizeof(_Ty)*m_size);
		}
		return m_buf;
	}
};

// -------------------------------------------------------------------------
// $Log: RecycleBuffer.h,v $

NS_STDEXT_END

#endif /* STDEXT_MEMORY_RECYCLEBUFFER_H */
