/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fiber/current/detail.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: detail.h 2705 2010-04-28 09:18:07Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FIBER_CURRENT_DETAIL_H
#define ASYNC_FIBER_CURRENT_DETAIL_H

#ifndef STDEXT_THREAD_TLS_H
#include "../../../../../stdext/include/stdext/thread/TLS.h"
#endif

// -------------------------------------------------------------------------
// class CurrentFiberImplMT
// class CurrentFiberImplST

namespace cerl {
namespace detail {

//
// class CurrentFiberImplMT
//

template <bool bMultiThread>
class CurrentFiberImplMT
{
private:
	static NS_STDEXT::TlsKey g_currFiber;

public:
	static void cerl_call init() {
		g_currFiber.create();
	}

	static void cerl_call term() {
		g_currFiber.clear();
	}

	static void cerl_call put(Fiber fiber) {
		g_currFiber.put(fiber);
	}

	static Fiber cerl_call get() {
		return (Fiber)g_currFiber.get();
	}
};

template <bool bMultiThread>
NS_STDEXT::TlsKey CurrentFiberImplMT<bMultiThread>::g_currFiber;

//
// class CurrentFiberImplST
//

template <bool bSingleThread>
class CurrentFiberImplST
{
private:
	static Fiber g_currFiber;

public:
	static void cerl_call init() {}
	static void cerl_call term() {}

	static void cerl_call put(Fiber fiber) {
		g_currFiber = fiber;
	}

	static Fiber cerl_call get() {
		return g_currFiber;
	}
};

template <bool bSingleThread>
Fiber CurrentFiberImplST<bSingleThread>::g_currFiber;

} // namespace detail
} // namespace cerl

// -------------------------------------------------------------------------
// class CurrentFiberInit

NS_CERL_BEGIN

typedef detail::CurrentFiberImplMT<true> CurrentFiberImpl;

class CurrentFiberInit
{
public:
	CurrentFiberInit() { CurrentFiberImpl::init(); }
	~CurrentFiberInit() { CurrentFiberImpl::term(); }
};

inline Fiber cerl_call getCurrentFiber()
{
	return CurrentFiberImpl::get();
}

inline PVOID cerl_call getFiberData()
{
	return *(PVOID*)CurrentFiberImpl::get();
}

NS_CERL_END

// -------------------------------------------------------------------------

#endif /* ASYNC_FIBER_CURRENT_DETAIL_H */
