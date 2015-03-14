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
// Module: stdext/mmap/FileMapping.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_FILEMAPPING_H
#define STDEXT_MMAP_FILEMAPPING_H

#ifndef STDEXT_MMAP_MAPFILE_H
#include "MapFile.h"
#endif

#ifndef STDEXT_MMAP_ACCESSBUFFER_H
#include "AccessBuffer.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class HandleProxy

template <class Owner>
class HandleProxy
{
private:
	Owner* m_owner;

public:
	enum { AllocationGranularityBits = Owner::AllocationGranularityBits };
	enum { AllocationGranularity = Owner::AllocationGranularity };
	enum { AllocationGranularityMask = Owner::AllocationGranularityMask };

public:
	typedef typename Owner::size_type size_type;
	typedef typename Owner::pos_type pos_type;

	typedef typename Owner::Utils Utils;

public:
	HandleProxy(Owner& owner) : m_owner(&owner) {
	}

	void winx_call close() {
	}

	char* winx_call viewSegment(DWORD iBasePage, DWORD nPageCount) {
		return m_owner->viewSegment(iBasePage, nPageCount);
	}

	char* winx_call accessSegment(DWORD iBasePage, DWORD nPageCount) {
		return m_owner->accessSegment(iBasePage, nPageCount);
	}

	char* winx_call allocSegment(DWORD nPageCount, DWORD& iBasePage) {
		return m_owner->allocSegment(nPageCount, iBasePage);
	}
};

// -------------------------------------------------------------------------
// class FileMapping

template <class Config>
class FileMapping : public MapFile<Config>
{
private:
	typedef MapFile<Config> BaseClass;
	
	DWORD m_nTotalPage;

	FileMapping(const FileMapping&);
	void operator=(const FileMapping&);

private:
	enum { _nAGBits = 16 };
	enum { _nAllocationGranularityInvBits = sizeof(DWORD)*8 - _nAGBits };

public:
	enum { AllocationGranularityBits = _nAGBits };
	enum { AllocationGranularity = (1 << _nAGBits) };
	enum { AllocationGranularityMask = (AllocationGranularity - 1) };

public:
	typedef typename BaseClass::size_type size_type;
	typedef typename BaseClass::pos_type pos_type;

	typedef HandleProxy<FileMapping> Handle;
	typedef BaseClass Utils;

public:
	FileMapping() {}
	
	FileMapping(LPCSTR szFile, pos_type* offset = NULL) {
		open(szFile, offset);
	}
	
public:
	DWORD winx_call getTotalPages() const {
		return m_nTotalPage;
	}

	void winx_call close() {
		BaseClass::close();
		m_nTotalPage = 0;
	}

	HRESULT winx_call resize(pos_type cbSize) {
		m_nTotalPage = (DWORD)((cbSize + AllocationGranularityMask) >> AllocationGranularityBits);
		return BaseClass::resize(cbSize);
	}

	HRESULT winx_call open(LPCSTR szFile, pos_type* offset = NULL)
	{
		if (Config::GetSizeOnOpen) {
			pos_type cbSize;
			HRESULT hr = BaseClass::open(szFile, &cbSize);
			m_nTotalPage = (DWORD)((cbSize + AllocationGranularityMask) >> AllocationGranularityBits);
			if (offset)
				*offset = cbSize;
			return hr;
		}
		else {
			m_nTotalPage = 0;
			return BaseClass::open(szFile, NULL);
		}
	}

	char* winx_call viewSegment(DWORD iBasePage, DWORD nPageCount)
	{
		WINX_ASSERT(BaseClass::good());

		if (iBasePage + nPageCount > m_nTotalPage)
		{
			if (iBasePage >= m_nTotalPage)
				return NULL;
			else
				nPageCount = m_nTotalPage - iBasePage;
		}
		
		return (char*)BaseClass::map(
			(off_t)iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits);
	}

	char* winx_call accessSegment(DWORD iBasePage, DWORD nPageCount)
	{
		WINX_ASSERT(BaseClass::good());

		if (iBasePage + nPageCount > m_nTotalPage)
		{
			m_nTotalPage = iBasePage + nPageCount;
			BaseClass::resize((off_t)m_nTotalPage << AllocationGranularityBits);
		}
		
		return (char*)BaseClass::map(
			(off_t)iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits);
	}

	char* winx_call allocSegment(DWORD nPageCount, DWORD& iBasePage)
	{
		WINX_ASSERT(BaseClass::good());

		iBasePage = m_nTotalPage;
		
		m_nTotalPage += nPageCount;
		BaseClass::resize((off_t)m_nTotalPage << AllocationGranularityBits);

		return (char*)BaseClass::map(
			(off_t)iBasePage << AllocationGranularityBits,
			nPageCount << AllocationGranularityBits);
	}
};

typedef FileMapping<MappingReadWrite> FileMappingRW;
typedef FileMapping<MappingReadOnly> FileMappingRO;

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MMAP_FILEMAPPING_H */
