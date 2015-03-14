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
// Module: stdext/filebuf/MapFileBuf.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-31 10:11:31
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_MAPFILEBUF_H
#define STDEXT_FILEBUF_MAPFILEBUF_H

#ifndef STDEXT_MMAP_MAPFILE_H
#include "../mmap/MapFile.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class MapFileBuf

class MapFileBuf
{
private:
	const char* m_data;
	size_t m_fileSize;
	MapFileRO m_mapfile;
	
public:
	MapFileBuf() : m_data(NULL) {}

	MapFileBuf(LPCSTR file) : m_data(NULL) {
		read(file);
	}

	MapFileBuf(LPCWSTR file) : m_data(NULL) {
		read(file);
	}
	
	~MapFileBuf() {
		if (m_data) {
			MapFileRO::unmap((void*)m_data, m_fileSize);
			m_mapfile.close();
		}
	}

public:
	HRESULT winx_call read(LPCSTR file)
	{
		if (good())
			return E_ACCESSDENIED;
		
		MapFileRO::pos_type fileSize;
		HRESULT hr = m_mapfile.open(file, &fileSize);
		if (hr != S_OK)
			return hr;

		if (fileSize > 0xFFFFFFFF)
			return E_OUTOFMEMORY;
		
		m_fileSize = (DWORD)fileSize;
		m_data = (const char*)m_mapfile.map(0, m_fileSize);
		if (m_data)
			return S_OK;
		
		m_mapfile.close();
		return E_FAIL;
	}

	HRESULT winx_call read(LPCWSTR file)
	{
		WINX_USES_CONVERSION;
		return read(WINX_W2CA(file));
	}

public:
	void winx_call clear() {
		if (m_data) {
			MapFileRO::unmap((void*)m_data, m_fileSize);
			m_data = NULL;
			m_mapfile.close();
		}
	}

	const char* winx_call data() const {
		return m_data;
	}

	const char* winx_call begin() const {
		return m_data;
	}

	const char* winx_call end() const {
		return m_data + m_fileSize;
	}

	bool winx_call good() const {
		return m_data != NULL;
	}

	const size_t winx_call size() const {
		return m_fileSize;
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_MAPFILEBUF_H */
