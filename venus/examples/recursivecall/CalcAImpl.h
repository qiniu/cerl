/*
	Description: 
	Author:			SDL Compiler
*/

#ifndef RECURSIVECALL_CALCA_IMPL_H
#define RECURSIVECALL_CALCA_IMPL_H

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

class CalcAImpl : public cerl::Server, public CalcABase 
{
public:
	explicit CalcAImpl()
	{
	}

	void cerl_call add(
		cerl::Caller* caller,
		_AddResult& result,
		const cerl::NInformation& node,
		const NUM32 number1,
		const NUM32 number2)
	{
		printf("Received a add request...\n");
		printf("Going to request minus from caller\n");
		CalcB b = cerl::Connection(node);
		CalcBBase::_MinusResult _mresult;
		b.minus(caller->getAlloc(), _mresult, number1, number2);
		if (code_ok == _mresult)
			printf("Get minus result:%d\n", _mresult.value);
		result._code = code_ok;
		result.value = number1 + number2;
	}

	void cerl_call _run(cerl::Fiber self, SOCKET socketListen);
};

} //namespace

#include "CalcAStub.h"

#endif /* RECURSIVECALL_CALCA_IMPL_H */ 
