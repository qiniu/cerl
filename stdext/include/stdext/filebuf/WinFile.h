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
// Module: stdext/filebuf/WinFile.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-25 15:33:27
// 
// $Id: WinFile.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_WINFILE_H
#define STDEXT_FILEBUF_WINFILE_H

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef STDEXT_FILEBUF_AUTOBUFFER_H
#include "AutoBuffer.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class WinFileBuf

template <class AllocT>
class WinFileBufT : public AutoBufferT<AllocT>
{
private:
	typedef AutoBufferT<AllocT> Base;
	
	HRESULT winx_call _read(HANDLE hFile)
	{
		WINX_ASSERT(!Base::good());
		
		DWORD dwFileSizeHigh = 0;
		DWORD dwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);
		if (dwFileSizeHigh > 0)
			return E_OUTOFMEMORY;
		
		::ReadFile(hFile, Base::allocate(dwFileSize), dwFileSize, &dwFileSizeHigh, NULL);
		return (dwFileSizeHigh == dwFileSize ? S_OK : STG_E_READFAULT);
	}

public:
	WinFileBufT() {}

	template <class InputFileT>
	explicit WinFileBufT(const InputFileT& file) {
		read(file);
	}

	HRESULT winx_call read(LPCSTR file)
	{
		HANDLE hFile = ::CreateFileA(
			file,
			GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(::GetLastError());
	
		HRESULT hr = _read(hFile);
		::CloseHandle(hFile);
		return hr;
	}

	HRESULT winx_call read(LPCWSTR file)
	{
		HANDLE hFile = ::CreateFileW(
			file,
			GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(::GetLastError());
		
		HRESULT hr = _read(hFile);
		::CloseHandle(hFile);
		return hr;
	}
};

typedef WinFileBufT<DefaultStaticAlloc> WinFileBuf;

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_WINFILE_H */

