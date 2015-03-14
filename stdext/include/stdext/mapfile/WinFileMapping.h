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
// Module: stdext/mapfile/WinFileMapping.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MAPFILE_WINFILEMAPPING_H
#define STDEXT_MAPFILE_WINFILEMAPPING_H

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct MappingReadWrite

struct MappingReadWrite
{
	enum { PageProtect = PAGE_READWRITE };
	enum { PageAccess = FILE_MAP_READ|FILE_MAP_WRITE };

	enum { FileDesiredAccess = GENERIC_WRITE|GENERIC_READ };
	enum { FileShareMode = FILE_SHARE_READ };
	enum { FileCreationDisposition = OPEN_ALWAYS };
	enum { FileFlagsAndAttributes = FILE_FLAG_WRITE_THROUGH|FILE_ATTRIBUTE_NORMAL };

	enum { GetSizeOnOpen = FALSE };
};

// -------------------------------------------------------------------------
// struct MappingReadOnly

struct MappingReadOnly
{
	enum { PageProtect = PAGE_READONLY };
	enum { PageAccess = FILE_MAP_READ };
	
	enum { FileDesiredAccess = GENERIC_READ };
	enum { FileShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE };
	enum { FileCreationDisposition = OPEN_EXISTING };
	enum { FileFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN };

	enum { GetSizeOnOpen = TRUE };
};

// -------------------------------------------------------------------------
// class MapFile

template <class Config>
class MapFile
{
private:
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
	
public:
	typedef size_t size_type;
	typedef UINT64 pos_type;
	
public:
	MapFile() : m_hFile(INVALID_HANDLE_VALUE) {
	}
	
	MapFile(LPCSTR szFile, pos_type* offset = NULL) : m_hFile(INVALID_HANDLE_VALUE) {
		open(szFile, offset);
	}
	
	~MapFile() {
		if (m_hFile != INVALID_HANDLE_VALUE) {
			if (m_hFileMapping)
				CloseHandle(m_hFileMapping);
			CloseHandle(m_hFile);
		}
	}
	
	HRESULT winx_call open(LPCSTR szFile, pos_type* offset = NULL)
	{
		if (good())
			return E_ACCESSDENIED;
		
		m_hFile = CreateFileA(
			szFile,
			Config::FileDesiredAccess,
			Config::FileShareMode,
			NULL,
			Config::FileCreationDisposition,
			Config::FileFlagsAndAttributes,
			NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(GetLastError());
		
		if (Config::GetSizeOnOpen)
		{
			DWORD dwFileSizeHigh = 0;
			DWORD dwFileSize = ::GetFileSize(m_hFile, &dwFileSizeHigh);
			m_hFileMapping = ::CreateFileMapping(
				m_hFile, NULL, Config::PageProtect,
				dwFileSizeHigh,
				dwFileSize,
				NULL);
			if (offset)
				*offset = ((UINT64)dwFileSizeHigh << 32) | dwFileSize;
		}
		else {
			m_hFileMapping = NULL;
		}

		return S_OK;
	}
	
	void winx_call close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE) {
			if (m_hFileMapping) {
				CloseHandle(m_hFileMapping);
				m_hFileMapping = NULL;
			}
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}
	
	bool winx_call good() const {
		return m_hFile != INVALID_HANDLE_VALUE;
	}
	
	HRESULT winx_call resize(pos_type newSize)
	{
		if (m_hFileMapping)
			WINX_VERIFY(CloseHandle(m_hFileMapping));

		m_hFileMapping = CreateFileMapping(
			m_hFile, NULL, Config::PageProtect,
			(DWORD)(newSize >> 32),
			(DWORD)newSize,
			NULL);
		return m_hFileMapping ? S_OK : E_FAIL;
	}
	
	pos_type winx_call size() const {
		DWORD dwFileSizeHigh = 0;
		DWORD dwFileSize = ::GetFileSize(m_hFile, &dwFileSizeHigh);
		return ((UINT64)dwFileSizeHigh << 32) | dwFileSize;
	}
	
	void* winx_call map(pos_type offset, size_type cbSize, void* pvBaseAddress = NULL)
	{
		return ::MapViewOfFileEx(
			m_hFileMapping, Config::PageAccess,
			(DWORD)(offset >> 32), (DWORD)offset,
			cbSize, pvBaseAddress
			);
	}

	static void winx_call unmap(void* p, size_t cb) {
		UnmapViewOfFile(p);
	}

	static HRESULT winx_call flush(void* p, size_t cb) {
		return FlushViewOfFile(p, cb) ? S_OK : E_FAIL;
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MAPFILE_WINFILEMAPPING_H */
