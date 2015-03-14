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
// Module: stdext/archive/Posix.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: Posix.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_POSIX_H
#define STDEXT_ARCHIVE_POSIX_H

#ifndef STDEXT_ARCHIVE_WRITEARCHIVE_H
#include "WriteArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_READARCHIVE_H
#include "ReadArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_POSIX_CONFIG_H
#include "posix/Config.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class PosixAdapter

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

#if defined(_MSC_VER)
	typedef ptrdiff_t ssize_t;
	#ifndef STDIN_FILENO
		#define STDIN_FILENO	0
	#endif
#endif

class PosixAdapter
{
public:
	enum { endch = -1 };
	enum { nullfd = -1 };

	typedef char			char_type;
	typedef unsigned char	uchar_type;
	typedef int				int_type;
	
	typedef size_t			size_type;
	
	typedef __off_t			pos_type;
	typedef __off_t			off_type;

private:
	typedef char_type _E;
	typedef int _Handle;

	enum { writeMode = O_TRUNC|O_CREAT|O_BINARY|O_WRONLY };
	enum { appendMode = O_BINARY|O_RDWR|O_APPEND };
	enum { readMode = O_BINARY|O_RDONLY };
	enum { CMASK = 0644 }; // wrr

protected:
	_Handle m_fd;
	
public:
	PosixAdapter() : m_fd(nullfd) {}
	~PosixAdapter()
	{
		if (m_fd != nullfd)
		{
			::close(m_fd);
			m_fd = nullfd;
		}
	}
	
	void winx_call attach(_Handle fd)
	{
		m_fd = fd;
	}

	int winx_call detach()
	{
		_Handle fd = m_fd;
		m_fd = nullfd;
		return fd;
	}

	void winx_call open_handle(_Handle fd)
	{
		WINX_ASSERT(m_fd == nullfd);
		m_fd = fd;
	}

	HRESULT winx_call open_to_write(LPCWSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_USES_CONVERSION;
		m_fd = ::open(WINX_W2CA(szFile), writeMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_write(LPCSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		m_fd = ::open(szFile, writeMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_append(LPCWSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_USES_CONVERSION;
		m_fd = ::open(WINX_W2CA(szFile), appendMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_append(LPCSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		m_fd = ::open(szFile, appendMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCWSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		WINX_USES_CONVERSION;
		m_fd = ::open(WINX_W2CA(szFile), readMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCSTR szFile)
	{
		WINX_ASSERT(m_fd == nullfd);
		m_fd = ::open(szFile, readMode, CMASK);
		return good() ? S_OK : E_ACCESSDENIED;
	}

	void winx_call seek(const off_type& pos, int dir = SEEK_SET)
	{
		::lseek(m_fd, pos, dir);
	}
	
	pos_type winx_call tell() const
	{
		return ::tell(m_fd);
	}

	pos_type winx_call size() const
	{
		return _filelength(m_fd);
	}

	size_type winx_call pget(_E* buf, size_type cch, const pos_type& off)
	{
		seek(off);
		return get(buf, cch);
	}
	
	size_type winx_call get(_E* buf, size_type cch)
	{
#if (0) // for stream device
		size_type total = 0;
		ssize_t n;
		while (cch && (n = ::read(m_fd, buf, cch)) > 0)
		{
			buf += n;
			total += n;
			cch -= n;
		}
		return total;
#else
		ssize_t n = ::read(m_fd, buf, (unsigned int)cch);
		return (n > 0 ? (size_type)n : 0);
#endif
	}

	size_type winx_call put(const _E* s, size_type cch)
	{
		return ::write(m_fd, s, (unsigned int)cch);
	}

	void winx_call flush()
	{
		::_commit(m_fd);
	}

	void winx_call close()
	{
		if (m_fd != nullfd)
		{
			::close(m_fd);
			m_fd = nullfd;
		}
	}

	operator _Handle() const
	{
		return m_fd;
	}

	bool winx_call operator!() const
	{
		return m_fd == nullfd;
	}

	bool winx_call good() const
	{
		return m_fd != nullfd;
	}

	bool winx_call bad() const
	{
		return m_fd == nullfd;
	}
};

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

// -------------------------------------------------------------------------

typedef WriteArchive<int, PosixAdapter> PosixWriteArchive;
typedef PosixWriteArchive PosixWriter;

typedef ReadArchive<int, PosixAdapter> PosixReadArchive;
typedef PosixReadArchive PosixReader;

NS_STDEXT_END

// -------------------------------------------------------------------------
// $Log: Posix.h,v $

#endif /* STDEXT_ARCHIVE_POSIX_H */

