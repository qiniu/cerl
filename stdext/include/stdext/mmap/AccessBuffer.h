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
// Module: stdext/mmap/AccessBuffer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_ACCESSBUFFER_H
#define STDEXT_MMAP_ACCESSBUFFER_H

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class SegmentAllocBuffer - as a simplest allocator

//
// ***NOTE***
// 	SegmentSize = (1 << SegBits) is maximum bytes that user can allocate.
//

#pragma pack(1)

struct SegmentAllocInfo
{
	DWORD m_iBasePage;	// index number of current block for allocating
	DWORD m_cbFree;		// free bytes of current block
};

#pragma pack()

template <class Base, int SegBits = 16>
class SegmentAllocBuffer : public Base, private SegmentAllocInfo
{
private:
	char* m_pView;

	SegmentAllocBuffer(const SegmentAllocBuffer&);
	void operator=(const SegmentAllocBuffer&);

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::pos_type pos_type;

public:
	enum { AllocationGranularityBits = Base::AllocationGranularityBits };
	enum { SegmentBits = MAX(SegBits, AllocationGranularityBits) };
	enum { SegmentSize = 1 << SegmentBits };
	enum { SegmentSizeMask = SegmentSize-1 };

private:
	enum { AlignBits_ = SegmentBits - AllocationGranularityBits };
	enum { AlignSize_ = 1 << AlignBits_ };
	
	typedef typename Base::Utils Utils;

public:
	SegmentAllocBuffer() : m_pView(NULL) { m_cbFree = 0; }

	template <class T1>
	SegmentAllocBuffer(T1& arg1)
		: m_pView(NULL), Base(arg1) { m_cbFree = 0; }
	
	template <class T1>
	SegmentAllocBuffer(const T1& arg1)
		: m_pView(NULL), Base(arg1) { m_cbFree = 0; }

	~SegmentAllocBuffer() {
		if (m_pView)
			Utils::unmap(m_pView, SegmentSize);
	}

	void winx_call close()
	{
		if (m_pView)
		{
			Utils::unmap(m_pView, SegmentSize);
			m_pView = NULL;
			m_cbFree = 0;
			Base::close();
		}
	}

	void winx_call flush()
	{
		if (m_pView)
			WINX_VERIFY(Utils::flush(m_pView, SegmentSize));
	}

	HRESULT winx_call initBuffer(DWORD iBasePage = 0)
	{
		WINX_ASSERT(m_pView == NULL && m_cbFree == 0);

		m_pView = Base::accessSegment(iBasePage, AlignSize_);
		if (m_pView != NULL)
		{
			m_cbFree = SegmentSize;
			m_iBasePage = iBasePage;
			return S_OK;
		}
		return E_ACCESSDENIED;
	}

	HRESULT winx_call initBuffer(const SegmentAllocInfo& info)
	{
		WINX_ASSERT(m_pView == NULL && m_cbFree == 0);

		m_pView = Base::accessSegment(info.m_iBasePage, AlignSize_);
		if (m_pView != NULL)
		{
			*static_cast<SegmentAllocInfo*>(this) = info;
			return S_OK;
		}
		return E_ACCESSDENIED;
	}

	void winx_call getAllocInfo(SegmentAllocInfo& info) const
	{
		info = *static_cast<const SegmentAllocInfo*>(this);
	}

	char* winx_call allocData(size_type cbSize, pos_type& fcPos)
	{
		WINX_ASSERT((int)SegmentBits >= (int)AllocationGranularityBits);
		WINX_ASSERT(cbSize <= SegmentSize);

		if (cbSize <= m_cbFree)
		{
Proc:		char* pData = m_pView + (SegmentSize - m_cbFree);
			fcPos = ((pos_type)m_iBasePage << AllocationGranularityBits) + (SegmentSize - m_cbFree);
			m_cbFree -= cbSize;
			return pData;
		}
		if (cbSize <= SegmentSize)
		{
			if (m_pView)
				Utils::unmap(m_pView, SegmentSize);

			m_pView = Base::allocSegment(AlignSize_, m_iBasePage);
			WINX_ASSERT(m_pView);
			
			if (m_pView != NULL) {
				m_cbFree = SegmentSize;
				goto Proc;
			}
			m_cbFree = 0;
		}
		return NULL;
	}
};

// -------------------------------------------------------------------------
// class SegmentAccessBuffer - as the simplest way to access memory

//
// ***NOTE*** 
//	SegmentSize = (1<<SegBits) is maximum bytes that user can access.
//	And user can't access memory across	the boundary of Segments.
//

template <class Base, int SegBits = 16, BOOL ReadOnly = FALSE>
class SegmentAccessBuffer : public Base
{
private:
	char* m_pView;
	DWORD m_iSeg;

	SegmentAccessBuffer(const SegmentAccessBuffer&);
	void operator=(const SegmentAccessBuffer&);

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::pos_type pos_type;

public:
	enum { AllocationGranularityBits = Base::AllocationGranularityBits };
	enum { SegmentBits = MAX(SegBits, AllocationGranularityBits) };
	enum { SegmentSize = 1 << SegmentBits };
	enum { SegmentSizeMask = SegmentSize-1 };

private:
	enum { AlignBits_ = SegmentBits - AllocationGranularityBits };
	enum { AlignSize_ = 1 << AlignBits_ };

	typedef typename Base::Utils Utils;

public:
	SegmentAccessBuffer() : m_pView(NULL), m_iSeg((DWORD)-1) {}

	template <class T1>
	SegmentAccessBuffer(T1& arg1)
		: m_pView(NULL), m_iSeg((DWORD)-1), Base(arg1) {}
	
	template <class T1>
	SegmentAccessBuffer(const T1& arg1)
		: m_pView(NULL), m_iSeg((DWORD)-1), Base(arg1) {}

	~SegmentAccessBuffer() {
		if (m_pView)
			Utils::unmap(m_pView, SegmentSize);
	}
	
	char* winx_call getCurrentView() const {
		return m_pView;
	}
	
	DWORD winx_call getCurrentSeg() const {
		return m_iSeg;
	}

	void winx_call close()
	{
		if (m_pView)
		{
			Utils::unmap(m_pView, SegmentSize);
			m_pView = NULL;
			m_iSeg = ((DWORD)-1);
			Base::close();
		}
	}

	void winx_call flush()
	{
		if (m_pView)
			WINX_VERIFY(Utils::flush(m_pView, SegmentSize));
	}
	
private:
	char* winx_call _viewSegment(DWORD iSeg)
	{
		if (m_pView)
			Utils::unmap(m_pView, SegmentSize);

		if (ReadOnly)
			m_pView = Base::viewSegment(iSeg << AlignBits_, AlignSize_);
		else
			m_pView = Base::accessSegment(iSeg << AlignBits_, AlignSize_);

		WINX_ASSERT(m_pView);

		if (m_pView != NULL) {
			m_iSeg = iSeg;
			return m_pView;
		}
		else {
			m_iSeg = (DWORD)-1;
			return NULL;
		}
	}

	char* winx_call _view(DWORD iSeg, pos_type fc)
	{
		if (m_pView)
			Utils::unmap(m_pView, SegmentSize);

		if (ReadOnly)
			m_pView = Base::viewSegment(iSeg << AlignBits_, AlignSize_);
		else
			m_pView = Base::accessSegment(iSeg << AlignBits_, AlignSize_);

		WINX_ASSERT(m_pView);

		if (m_pView != NULL) {
			m_iSeg = iSeg;
			return m_pView + (SegmentSizeMask & (size_type)fc);
		}
		else {
			m_iSeg = (DWORD)-1;
			return NULL;
		}
	}
	
public:	
	char* winx_call viewSegment(DWORD iSeg)
	{
		return iSeg == m_iSeg ? m_pView : _viewSegment(iSeg);
	}

	char* winx_call view(pos_type fc)
	{
		DWORD iSeg = (DWORD)(fc >> SegmentBits);
		return iSeg == m_iSeg ? m_pView + (SegmentSizeMask & (size_type)fc) : _view(iSeg, fc);
	}
};

template <class Owner, int SegBits = 16>
class SegmentViewBuffer : public SegmentAccessBuffer<Owner, SegBits, TRUE>
{
private:
	typedef SegmentAccessBuffer<Owner, SegBits, TRUE> Base;
	
public:
	SegmentViewBuffer() {}

	template <class T1>
	SegmentViewBuffer(T1& arg1)
		: Base(arg1) {}
	
	template <class T1>
	SegmentViewBuffer(const T1& arg1)
		: Base(arg1) {}
};

// -------------------------------------------------------------------------
// class AccessBuffer - as a general way to access memory

template <class Base, BOOL ReadOnly = FALSE>
class AccessBuffer : public Base
{
private:
	char* m_pView;
	DWORD m_nBasePage;
	DWORD m_nNextPage;

	AccessBuffer(const AccessBuffer&);
	void operator=(const AccessBuffer&);

	typedef typename Base::Utils Utils;

public:
	typedef typename Base::size_type size_type;
	typedef typename Base::pos_type pos_type;

public:
	enum { AllocationGranularity = Base::AllocationGranularity };
	enum { AllocationGranularityBits = Base::AllocationGranularityBits };
	enum { AllocationGranularityMask = Base::AllocationGranularityMask };

public:
	AccessBuffer() : m_pView(NULL), m_nBasePage(LONG_MAX), m_nNextPage(0) {}

	template <class T1>
	AccessBuffer(T1& arg1)
		: m_pView(NULL), m_nBasePage(LONG_MAX), m_nNextPage(0), Base(arg1) {}
	
	template <class T1>
	AccessBuffer(const T1& arg1)
		: m_pView(NULL), m_nBasePage(LONG_MAX), m_nNextPage(0), Base(arg1) {}

	~AccessBuffer()
	{
		if (m_pView) {
			Utils::unmap(
				m_pView,
				(m_nNextPage - m_nBasePage) << AllocationGranularityBits);
		}
	}
	
	void winx_call close()
	{
		if (m_pView)
		{
			WINX_VERIFY(
				Utils::unmap(
					m_pView,
					(m_nNextPage - m_nBasePage) << AllocationGranularityBits
					)
				);
			m_pView = NULL;
			m_nBasePage = LONG_MAX;
			m_nNextPage = 0;
			Base::close();
		}
	}

	void winx_call flush()
	{
		if (m_pView)
		{
			WINX_VERIFY(
				Utils::flush(
					m_pView,
					(m_nNextPage - m_nBasePage) << AllocationGranularityBits
					)
				);
		}
	}

	char* winx_call view(pos_type offset, size_type count)
	{
		DWORD nBasePage = (DWORD)(
			offset >> AllocationGranularityBits);
		
		DWORD nNextPage = (DWORD)(
			(offset + count + AllocationGranularityMask) >> AllocationGranularityBits);

		WINX_ASSERT(count > 0);
		WINX_ASSERT(nBasePage < nNextPage);

		if (m_nNextPage < nNextPage || m_nBasePage > nBasePage)
		{
			if (m_pView) {
				Utils::unmap(
					m_pView,
					(m_nNextPage - m_nBasePage) << AllocationGranularityBits);
			}

			if (ReadOnly)
				m_pView = Base::viewSegment(nBasePage, nNextPage - nBasePage);
			else
				m_pView = Base::accessSegment(nBasePage, nNextPage - nBasePage);

			WINX_ASSERT(m_pView);

			if (m_pView == NULL)
				return NULL;

			m_nBasePage = nBasePage;
			m_nNextPage = nNextPage;
		}

		UINT32 offsetInPage = (UINT32)
			(offset - ((pos_type)m_nBasePage << AllocationGranularityBits));

		return m_pView + offsetInPage;
	}
};

template <class Owner>
class ViewBuffer : public AccessBuffer<Owner, TRUE>
{
private:
	typedef AccessBuffer<Owner, TRUE> Base;
	
public:
	ViewBuffer() {}

	template <class T1>
	ViewBuffer(T1& arg1)
		: Base(arg1) {}
	
	template <class T1>
	ViewBuffer(const T1& arg1)
		: Base(arg1) {}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_MMAP_ACCESSBUFFER_H */
