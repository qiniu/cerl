/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fileio/syncio.h
// Creator: mohuafeng
// Email: longshanks@gmail.com
// Date: 2009-11-11 9:41:58
// 
// $Id: aio.h 619 2009-11-11 9:41:58 mohuafeng$
// -----------------------------------------------------------------------*/

#ifndef ASYNC_FILEIO_SYNCIO_H
#define ASYNC_FILEIO_SYNCIO_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#ifndef STDEXT_ARCHIVE_POSIX_CONFIG_H
#include "../../../../stdext/include/stdext/archive/posix/Config.h"
#endif

NS_CERL_BEGIN


// -------------------------------------------------------------------------
// class SyncFile

class SyncFile
{
public:
	SyncFile()
		: m_hFile(INVALID_HANDLE_VALUE)
	{
	}

public:
	typedef HANDLE handle_type;

	bool cerl_call good() const
	{
		return m_hFile != INVALID_HANDLE_VALUE;
	}

	HANDLE cerl_call detach()
	{
		HANDLE hFileOld = m_hFile;
		m_hFile = INVALID_HANDLE_VALUE;
		return hFileOld;
	}

	HRESULT cerl_call open_handle(HANDLE hFile)
	{
		CERL_ASSERT(!good());
		if (hFile == INVALID_HANDLE_VALUE)
			return E_ACCESSDENIED;

		m_hFile = hFile;

		return S_OK;
	}

	HRESULT cerl_call open_to_write(const char* file)
	{
		return open_handle((HANDLE)::open(file, O_RDWR|O_CREAT));
	}

	HRESULT cerl_call open_to_read(const char* file)
	{
		return open_handle((HANDLE)::open(file, O_RDONLY));
	}

	void cerl_call close()
	{
		::close((int)(size_t)m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

public:
	ptrdiff_t cerl_call read_some(void* buf, size_t cb)
	{
		int bytes = ::read((int)(size_t)m_hFile, buf, cb);
		return (bytes > 0) ? bytes : 0;
	}

	ptrdiff_t cerl_call write_some(const void* buf, size_t cb)
	{
		int bytes = ::write((int)(size_t)m_hFile, buf, cb);
		return (bytes > 0) ? bytes : 0;
	}

	ptrdiff_t cerl_call read(void* buf, size_t cb)
	{
		return read_some(buf, cb);
	}

	ptrdiff_t cerl_call write(const void* buf, size_t cb)
	{
		return write_some(buf, cb);
	}

protected:
	HANDLE		m_hFile;
}; 

NS_CERL_END

#endif // ASYNC_FILEIO_SYNCIO_H

