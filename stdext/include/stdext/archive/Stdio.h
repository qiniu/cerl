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
// Module: stdext/archive/Stdio.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: Stdio.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_STDIO_H
#define STDEXT_ARCHIVE_STDIO_H

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
// class StdioAdapter

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

class StdioAdapter
{
public:
	enum { endch = -1 };
	enum { nullfd = 0 };

	typedef char			char_type;
	typedef unsigned char	uchar_type;
	typedef int				int_type;
	
	typedef size_t			size_type;
	
	typedef unsigned long	pos_type;
	typedef long			off_type;

private:
	typedef char_type _E;
	typedef FILE* _Handle;

protected:
	_Handle m_pFile;
	
public:
	StdioAdapter() : m_pFile(NULL) {}
	~StdioAdapter()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}
	
	void winx_call attach(_Handle pFile)
	{
		m_pFile = pFile;
	}

	_Handle winx_call detach()
	{
		_Handle pFile = m_pFile;
		m_pFile = NULL;
		return pFile;
	}

	void winx_call open_handle(_Handle pFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		WINX_ASSERT(tell() == 0);
		m_pFile = pFile;
	}

	HRESULT winx_call open_to_write(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		WINX_USES_CONVERSION;
		m_pFile = fopen(WINX_W2CA(szFile), "wb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_write(LPCSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		m_pFile = fopen(szFile, "wb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCWSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		WINX_USES_CONVERSION;
		m_pFile = fopen(WINX_W2CA(szFile), "rb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	HRESULT winx_call open_to_read(LPCSTR szFile)
	{
		WINX_ASSERT(m_pFile == NULL);
		m_pFile = fopen(szFile, "rb");
		return m_pFile ? S_OK : E_ACCESSDENIED;
	}

	void winx_call seek(const off_type& pos, int dir = SEEK_SET)
	{
		fseek(m_pFile, pos, dir);
	}
	
	pos_type winx_call tell() const
	{
		return ftell(m_pFile);
	}

	pos_type winx_call size() const
	{
		return _filelength(fileno(m_pFile));
	}
	
	size_type winx_call get(_E* buf, size_type cch)
	{	
		return fread(buf, sizeof(char_type), cch, m_pFile);
	}

	size_type winx_call put(const _E* s, size_type cch)
	{
		return fwrite(s, sizeof(char_type), cch, m_pFile);
	}

	void winx_call flush()
	{
		fflush(m_pFile);
	}

	void winx_call close()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}

	operator _Handle() const
	{
		return m_pFile;
	}

	bool winx_call operator!() const
	{
		return m_pFile == NULL;
	}

	bool winx_call good() const
	{
		return m_pFile != NULL;
	}

	bool winx_call bad() const
	{
		return m_pFile == NULL;
	}
};

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

// -------------------------------------------------------------------------

typedef WriteArchive<FILE*, StdioAdapter> StdioWriteArchive;
typedef StdioWriteArchive StdioWriter;

typedef ReadArchive<FILE*, StdioAdapter> StdioReadArchive;
typedef StdioReadArchive StdioReader;

NS_STDEXT_END

// -------------------------------------------------------------------------
// $Log: Stdio.h,v $

#endif /* STDEXT_ARCHIVE_STDIO_H */

