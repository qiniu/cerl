/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Future.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Future.h 2525 2010-04-19 07:01:53Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FUTURE_H
#define ASYNC_FUTURE_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class Future
//
// http://en.wikipedia.org/wiki/Future_(programming)
// In computer science, future,
// promise, and delay refer to
// constructs used for
// synchronization in some
// concurrent programming languages.

class FutureParam;

class Future
{
private:
	int m_finished;
	Fiber m_fiberSwitchTo;

private:
	struct Param
	{
		Future* future;
		void* param;
	};

	friend class FutureParam;

	void cerl_call finish(Fiber self, IoService* service)
	{
		CERL_FIBER_ASSERT_SELF(self);
		m_finished = 1;
		service->exitFiberAndSwitch(self, m_fiberSwitchTo);
	}

public:
	Future(Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0)
		: m_finished(0)
	{
		CERL_FIBER_ASSERT_SELF(self);
		Param p = { this, startParam };
		IoService* service = CerlIoService(self);
		m_fiberSwitchTo = service->self;
		service->startFiber(self, lpStartAddress, &p, dwStackSize);
	}

	int cerl_call isFinished() const
	{
		return m_finished;
	}

	void cerl_call wait(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);
		if (!m_finished)
		{
			m_fiberSwitchTo = self;
			yield(self);
			CERL_ASSERT(m_finished);
		}
	}
};

// -------------------------------------------------------------------------
// class FutureParam

class FutureParam
{
public:
	Fiber const self;
	IoService* const service;
	void* const val;

private:
	Future* future;

#define CerlFutureParamData_ ((Future::Param*)CerlFiberParamData_->startParam)

public:
	FutureParam(LPVOID lpParam)
		: self(CerlFiberParamData_->self),
		  service(CerlFiberParamData_->service),
		  val(CerlFutureParamData_->param),
		  future(CerlFutureParamData_->future)
	{
	}
	~FutureParam() {
		future->finish(self, service);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_FUTURE_H */
