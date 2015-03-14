/*
	Description: 
	Author:			SDL Compiler
*/

#ifndef RECURSIVECALL_CALCB_IMPL_H
#define RECURSIVECALL_CALCB_IMPL_H

#ifndef SDL_RECURSIVECALL_BASE_H
#include "sdl_recursivecall_base.h"
#endif

#ifndef SDL_RECURSIVECALL_H
#include "sdl_recursivecall.h"
#endif

#ifndef VENUS_SERVER_H
#include <venus/Server.h>
#endif

namespace recursivecall {

class CalcBImpl : public cerl::Server, public CalcBBase 
{
public:
	void cerl_call onStartup(cerl::Fiber fiber)
	{
		cerl::NInformation ni(":8633");
		cerl::NInformation nodeSelf(":8634");
		CalcA a = cerl::Connection(ni);
		CalcABase::_AddResult result;
		cerl::ScopedAlloc alloc(CerlBlockPool(fiber));
		a.add(alloc, result, nodeSelf, 3, 2);
		if (code_ok == result)
			printf("Get Add Result:%d\n", result.value);
	}

	void cerl_call minus(
		cerl::Caller* caller,
		_MinusResult& result,
		const NUM32 number1,
		const NUM32 number2)
	{
		result._code = code_ok;
		result.value = number1 - number2;
	}

	void cerl_call _run(cerl::Fiber self, SOCKET socketListen);
};

} //namespace

#include "CalcBStub.h"

#endif /* RECURSIVECALL_CALCB_IMPL_H */ 
