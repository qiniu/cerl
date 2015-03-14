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
// Module: tpl/binary/Storage.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Storage.h 618 2008-06-01 03:33:32Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_STORAGE_H
#define TPL_BINARY_STORAGE_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_BINARY_COMPTR_H
#include "ComPtr.h"
#endif

#if !defined( _OBJBASE_H_ )
#include <objbase.h>
#endif

NS_TPL_BEGIN

#define TPL_STGM_READONLY (STGM_SHARE_DENY_WRITE | STGM_READ)

// =========================================================================
// class Stream

template <class WStringArgT>
class Stream
{
private:
	WStringArgT m_stgName;

public:
	template <class ArgT>
	Stream(const ArgT& stgName)
		: m_stgName(stgName) {
	}

	typedef ComPtr<IStream*> result_type;

	template <class ContextT>
	bool TPL_CALL match(IStorage* stg, ContextT& context, result_type& strm) const {
		return S_OK == stg->OpenStream(
			m_stgName, NULL, TPL_STGM_READONLY, 0, &strm);
	}
};

template <class ArgT> __forceinline
StgRule<Stream<typename WStringTraits<ArgT>::value_type> >
TPL_CALL stream(const ArgT& stgName) {
	return StgRule<Stream<typename WStringTraits<ArgT>::value_type> >(stgName);
}

// =========================================================================
// class Storage

template <class WStringArgT>
class Storage
{
private:
	WStringArgT m_stgName;

public:
	template <class ArgT>
	Storage(const ArgT& stgName)
		: m_stgName(stgName) {
	}

	typedef ComPtr<IStorage*> result_type;

	template <class ContextT>
	bool TPL_CALL match(IStorage* stg, ContextT& context, result_type& strm) const {
		return S_OK == stg->OpenStorage(
			m_stgName, NULL, TPL_STGM_READONLY, 0, 0, &strm);
	}
};

template <class ArgT> __forceinline
StgRule<Storage<typename WStringTraits<ArgT>::value_type> >
TPL_CALL storage(const ArgT& stgName) {
	return StgRule<Storage<typename WStringTraits<ArgT>::value_type> >(stgName);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_STORAGE_H */
