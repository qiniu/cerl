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
// Module: stdext/filebuf/Stdio.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-25 15:33:27
// 
// $Id: Stdio.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_FILEBUF_STDIO_H
#define STDEXT_FILEBUF_STDIO_H

#ifndef STDEXT_MEMORY_H
#include "../Memory.h"
#endif

#ifndef STDEXT_STRING_H
#include "../String.h"
#endif

#ifndef STDEXT_FILEBUF_AUTOBUFFER_H
#include "AutoBuffer.h"
#endif

NS_STDEXT_BEGIN

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

// -------------------------------------------------------------------------
// function readFile

#ifndef WINX_FILEBUF_SEGMENT_BYTES
#define WINX_FILEBUF_SEGMENT_BYTES	4096
#endif

template <class AllocT, class ContainerT>
inline void winx_call readFile(
	AllocT& alloc, ContainerT& file, FILE* fp, size_t cbSeg = WINX_FILEBUF_SEGMENT_BYTES)
{
	size_t cb;
	char* buf;
	do
	{
		buf = (char*)alloc.allocate(cbSeg);
		cb = ::fread(buf, 1, cbSeg, fp);
		file.push_back(String(buf, buf+cb));
	}
	while (cb == cbSeg);
}

// -------------------------------------------------------------------------
// class FILEFileBuf

template <class AllocT>
class FILEFileBufT : public AutoBufferT<AllocT>
{
private:
	typedef AutoBufferT<AllocT> Base;
	
public:
	FILEFileBufT() {}
	
	template <class InputFileT>
	explicit FILEFileBufT(const InputFileT& file) {
		read(file);
	}

	HRESULT winx_call readSeq(FILE* fp, size_t cbSeg = WINX_FILEBUF_SEGMENT_BYTES)
	{
		typedef std::vector<String> ContainerT;
		
		AutoAlloc alloc;
		ContainerT file;
		readFile(alloc, file, fp, cbSeg);
		
		const size_t lFileSize = (file.size() - 1) * cbSeg + file.back().size();
		char* buf = Base::allocate(lFileSize);
		for(ContainerT::iterator it = file.begin(); it != file.end(); ++it) {
			const String& i = *it;
			buf = std::copy(i.begin(), i.end(), buf);
		}
		return S_OK;
	}
	
	HRESULT winx_call readRand(FILE* fp)
	{
		WINX_ASSERT(!Base::good());
		
		::fseek(fp, 0, SEEK_END);
		long lFileSize = ::ftell(fp);
		if (lFileSize < 0)
			return STG_E_READFAULT;

		::fseek(fp, 0, SEEK_SET);
		if (lFileSize != (long)::fread(Base::allocate(lFileSize), 1, lFileSize, fp))
			return STG_E_READFAULT;

		return S_OK;
	}

	HRESULT winx_call read(LPCSTR file)
	{
		FILE* fp = ::fopen(file, "rb");
		if (fp == NULL)
			return STG_E_ACCESSDENIED;
		
		HRESULT hr = readRand(fp);
		::fclose(fp);
		return hr;
	}
	
	HRESULT winx_call read(LPCWSTR file)
	{
		WINX_USES_CONVERSION;
		return read(WINX_W2CA(file));
	}
};

typedef FILEFileBufT<DefaultStaticAlloc> FILEFileBuf;
typedef FILEFileBuf StdioFileBuf;

// -------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

NS_STDEXT_END

#endif /* STDEXT_FILEBUF_STDIO_H */

