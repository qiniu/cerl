/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Direct.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Direct.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_DIRECT_H
#define ASYNC_DIRECT_H

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef ASYNC_TASKPOOL_H
#include "TaskPool.h"
#endif

#if defined(X_OS_WINDOWS)

#ifndef _INC_DIRECT
#include <direct.h>
#endif

#else

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef _mkdir
#define _mkdir(dir)	::mkdir(dir, 0755)
#endif

#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// function mkdir

inline int cerl_call mkdir(const char* dir)
{
	if (_mkdir(dir) == 0)
		return 0;
	else
		return errno;
}

inline int cerl_call mkdir(Fiber self, const char* dir)
{
	if (_mkdir(dir) == 0)
		return 0;
	else
		return errno;
}

// -------------------------------------------------------------------------
// class DirentExamples

template <class Unused>
class DirentExamples
{
public:
	static int cerl_call mkdir_byTaskRunner(cerl::Fiber self, const char* dir)
	{
		cerl::TaskRunner runner(self);
		return cerl::mkdir(dir);
	}

public:
	struct MkdirParam_
	{
		const char* dir;
		int result;
	};

	static void cerl_callback do_mkdir_(void* param)
	{
		MkdirParam_& p = *(MkdirParam_*)param;
		p.result = cerl::mkdir(p.dir);
	}

	static int cerl_call mkdir_byRunTask(cerl::Fiber self, const char* dir)
	{
		MkdirParam_ p = { dir, 0 };
		cerl::runTask(self, do_mkdir_, &p);
		return p.result;
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_DIRECT_H */
