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
// Module: tpl/binary/ComPtr.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: ComPtr.h 618 2008-06-01 03:33:32Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_COMPTR_H
#define TPL_BINARY_COMPTR_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"	// TPL_ASSERT, NS_TPL
#endif

#if !defined( _OBJBASE_H_ )
#include <objbase.h>
#endif

NS_TPL_BEGIN

// =========================================================================
// class ComPtr

template <class T>
class ComPtr
{
public:
	T* p;

public:
	ComPtr() {
		p = NULL;
	}
	ComPtr(T* lp) {
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
	~ComPtr()
	{
		if (p)
			p->Release();
	}

	operator T*() const {
		return p;
	}
	T& TPL_CALL operator*() const {
		return *p;
	}
	T** TPL_CALL operator&() {
		//The assert on operator& usually indicates a bug.  If this is really
		//what is needed, however, take the address of the p member explicitly.
		TPL_ASSERT(p==NULL);
		return &p;
	}
	T* TPL_CALL operator->() const {
		TPL_ASSERT(p!=NULL);
		return p;
	}

	bool TPL_CALL operator!() const {
		return (p == NULL);
	}
	bool TPL_CALL operator!=(T* pT) const {
		return !operator==(pT);
	}
	bool TPL_CALL operator==(T* pT) const {
		return p == pT;
	}

	// Release the interface and set to NULL
	void TPL_CALL clear() {
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	// Attach to an existing interface (does not AddRef)
	void TPL_CALL attach(T* p2) {
		if (p)
			p->Release();
		p = p2;
	}
	// Detach the interface (does not Release)
	T* TPL_CALL detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}

	HRESULT create(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		TPL_ASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
	HRESULT create(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = ::CLSIDFromProgID(szProgID, &clsid);
		TPL_ASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}

	template <class Q>
	HRESULT queryInterface(Q** pp) const
	{
		TPL_ASSERT(pp != NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_COMPTR_H */
