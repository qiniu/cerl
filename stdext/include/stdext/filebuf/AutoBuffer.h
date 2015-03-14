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
// Module: stdext/filebuf/AutoBuffer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-25 15:33:27
// 
// $Id: AutoBuffer.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_AUTOBUFFER_H
#define STDEXT_FILEBUF_AUTOBUFFER_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class AutoBufferT

template <class AllocT>
class AutoBufferT
{
protected:
	char* m_data;
	size_t m_bytes;

public:
	typedef char* iterator;
	typedef const char* const_iterator;

public:
	AutoBufferT() : m_data(NULL), m_bytes(0) {}
	explicit AutoBufferT(size_t bytes) : m_data(NULL) {
		allocate(bytes);
	}
	~AutoBufferT() {
		if (m_data)
			AllocT::deallocate(m_data);
	}

	char* winx_call allocate(size_t bytes) {
		WINX_ASSERT(m_data == NULL);
		m_bytes = bytes;
		m_data = (char*)AllocT::allocate(bytes);
		return m_data;
	}

	void winx_call clear() {
		if (m_data) {
			AllocT::deallocate(m_data, m_bytes);
			m_data = NULL;
		}
	}

	char* winx_call data() {
		return m_data;
	}
	const char* winx_call data() const {
		return m_data;
	}

	char* winx_call begin() {
		return m_data;
	}
	const char* winx_call begin() const {
		return m_data;
	}
	
	char* winx_call end() {
		return m_data + m_bytes;
	}
	const char* winx_call end() const {
		return m_data + m_bytes;
	}

	int winx_call good() const {
		return m_data != NULL;
	}

	const size_t winx_call size() const {
		return m_bytes;
	}
};

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_AUTOBUFFER_H */

