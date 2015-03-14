/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/TaskPool.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: TaskPool.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_TASKPOOL_H
#define ASYNC_TASKPOOL_H

// -------------------------------------------------------------------------

#ifndef ASYNC_TASK_GENTASKPOOL_H
#include "task/GenTaskPool.h"
#endif

#ifndef ASYNC_TASK_SHAREFIBERTASKPOOL_H
#include "task/ShareFiberTaskPool.h"
#endif

#if defined(X_OS_LINUX)
#  ifndef ASYNC_TASK_LINUX_H
#    include "task/linux.h"
#  endif
#endif

/*
#ifndef ASYNC_TASK_FIBERTASKPOOL_H
#include "task/FiberTaskPool.h"
#endif
*/

// -------------------------------------------------------------------------
// CERL_RUNONCE

NS_CERL_BEGIN

namespace detail {

template <class ObjectT>
class RunOnce : public ObjectT
{
public:
	bool toRunning;

public:
	RunOnce() : toRunning(true) {}
};

} // namespace detail

NS_CERL_END

#define CERL_RUNONCE(ObjectT)												\
	CERL_FOR(cerl::detail::RunOnce<ObjectT> cerl_runner_; cerl_runner_.toRunning; cerl_runner_.toRunning = false)

// -------------------------------------------------------------------------
// cerl_runtask

#define cerl_runtask		CERL_RUNONCE(cerl::TaskRunner)

// -------------------------------------------------------------------------

#endif /* ASYNC_TASKPOOL_H */
