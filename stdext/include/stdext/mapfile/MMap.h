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
// Module: stdext/mapfile/MMap.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MAPFILE_MMAP_H
#define STDEXT_MAPFILE_MMAP_H

#ifndef _SYS_MMAN_H
#include <sys/mman.h>
#endif

#ifndef	_FCNTL_H
#include <fcntl.h>
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// struct MappingReadWrite

struct MappingReadWrite
{
	enum { PageProtect = PROT_READ | PROT_WRITE };
	enum { PageAccess = MAP_SHARED };

	enum { FileDesiredAccess = O_RDWR };
	enum { FileShareMode = 0 };
	enum { FileCreationDisposition = O_CREAT|O_TRUNC };
	enum { FileFlagsAndAttributes = S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH };
	
	enum { FlushMode = MS_ASYNC };
	enum { GetSizeOnOpen = FALSE };
};

// -------------------------------------------------------------------------
// struct MappingReadOnly

struct MappingReadOnly
{
	enum { PageProtect = PROT_READ };
	enum { PageAccess = MAP_SHARED };

	enum { FileDesiredAccess = O_RDONLY };
	enum { FileShareMode = 0 };
	enum { FileCreationDisposition = 0 };
	enum { FileFlagsAndAttributes = 0 };

	enum { GetSizeOnOpen = TRUE };
};

// -------------------------------------------------------------------------
// class MapFile

template <class Config>
class MapFile
{
private:
	int m_fd;
	
public:
	typedef size_t size_type;
	typedef off_t pos_type;
	
public:
	MapFile() : m_fd(-1) {
	}
	
	MapFile(LPCSTR szFile, pos_type* offset = NULL) : m_fd(-1) {
		open(szFile, offset);
	}
	
	~MapFile() {
		if (m_fd >= 0)
			::close(m_fd);
	}
	
	HRESULT winx_call open(LPCSTR szFile, pos_type* offset = NULL)
	{
		if (good())
			return E_ACCESSDENIED;
			
		m_fd = ::open(
			szFile,
			Config::FileDesiredAccess | Config::FileShareMode | Config::FileCreationDisposition,
			Config::FileFlagsAndAttributes);
		if (m_fd < 0)
			return E_ACCESSDENIED;
			
		if (Config::GetSizeOnOpen) {
			if (offset)
				*offset = size();
		}
		return S_OK;
	}
	
	void winx_call close() {
		if (m_fd >= 0) {
			::close(m_fd);
			m_fd = -1;
		}
	}
	
	bool winx_call good() const {
		return m_fd >= 0;
	}
	
	HRESULT winx_call resize(pos_type newSize) {
		return ::ftruncate(m_fd, newSize);
	}
	
	pos_type winx_call size() const {
		struct stat fi;
		::fstat(m_fd, &fi);
		return fi.st_size;
	}
	
	void* winx_call map(pos_type offset, size_type cbSize, void* pvBaseAddress = NULL)
	{
		return mmap(
			pvBaseAddress, cbSize,
			Config::PageProtect, Config::PageAccess,
			m_fd, offset);
	}

	static void winx_call unmap(void* p, size_t cb) {
		munmap(p, cb);
	}

	static HRESULT winx_call flush(void* p, size_t cb) {
		return msync(p, cb, Config::FlushMode);
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MAPFILE_MMAP_H */
