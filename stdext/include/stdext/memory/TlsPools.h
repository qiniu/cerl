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
// Module: stdext/memory/TlsPools.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:46:39
// 
// $Id: TlsPools.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MEMORY_TLSPOOLS_H
#define STDEXT_MEMORY_TLSPOOLS_H

#ifndef STDEXT_BOOST_MEMORY_H
#include "../boost/Memory.h"
#endif

#ifndef STDEXT_THREAD_TLS_H
#include "../thread/TLS.h"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class tls_pools

template <class PolicyT>
class tls_pools_alloc
{
private:
	static NS_STDEXT::TlsKey g_tls;
	
public:
	typedef pools_alloc<PolicyT> pools_type;

	static void BOOST_MEMORY_CALL put(pools_type& r)
	{
		static int g_result = g_tls.create();
		(void)g_result;
		g_tls.put(&r);
	}

	static pools_type& BOOST_MEMORY_CALL get()
	{
		pools_type* po = (pools_type*)g_tls.get();
		BOOST_MEMORY_ASSERT(po!= NULL);
		return *po;
	}

	static void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		pools_type* po = (pools_type*)g_tls.get();
		BOOST_MEMORY_ASSERT(po != NULL);
		return po->allocate(cb);
	}

	static void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		pools_type* po = (pools_type*)g_tls.get();
		BOOST_MEMORY_ASSERT(po != NULL);
		po->deallocate(p, cb);
	}

	static size_t BOOST_MEMORY_CALL alloc_size(void* p, size_t cb)
	{
		pools_type* po = (pools_type*)g_tls.get();
		BOOST_MEMORY_ASSERT(po != NULL);
		return po->alloc_size(p, cb);
	}
};

template <class PolicyT>
NS_STDEXT::TlsKey tls_pools_alloc<PolicyT>::g_tls;

typedef tls_pools_alloc<NS_BOOST_MEMORY_POLICY::stdlib> tls_pools;

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* STDEXT_MEMORY_TLSPOOLS_H */
