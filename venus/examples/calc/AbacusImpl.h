/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/exmaples/calc/AbacusImpl.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: AbacusImpl.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef CALC_ABACUSIMPL_H
#define CALC_ABACUSIMPL_H

#ifndef CALC_BASE_H
#include "calc_base.h"
#endif

#ifndef VENUS_SERVER_H
#include <venus/Server.h>
#endif

namespace calc {

// =========================================================================
// class AbacusImpl
	
class AbacusImpl : public cerl::Server, public AbacusBase
{
public:
	enum { MultiThreads = TRUE };

public:
	~AbacusImpl()
	{
		printf("Server terminated: ~AbacusImpl\n");
	}

    void cerl_call add(cerl::Caller* _caller, _AddResult& result, int a, int b)
	{
		result = code_ok;
		result.sum = a + b;
		CERL_TR("Server", ("INFO: Result is %d\n", result.sum));
	}

public:
	HRESULT cerl_call _run(cerl::Fiber self, SOCKET socketListen);
};

}

#include "AbacusStub.h"

#endif /* CALC_ABACUSIMPL_H */
