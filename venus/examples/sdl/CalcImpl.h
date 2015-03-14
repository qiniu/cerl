/*
	Description: 
	Author:			SDL Compiler
*/

#ifndef USERFUNC_CALC_IMPL_H
#define USERFUNC_CALC_IMPL_H

#ifndef SDL_USERFUNC_BASE_H
#include "sdl_userfunc_base.h"
#endif

#ifndef VENUS_SERVER_H
#include <venus/Server.h>
#endif

namespace userfunc {

class CalcImpl : public cerl::Server, public CalcBase 
{
public:
	explicit CalcImpl()
	{
	}

	void cerl_call square(
		cerl::Caller* caller,
		_SquareResult& result,
		const cerl::UInt32 number)
	{
		result._code = code_ok;
		result.str = number * number;
	}

	void cerl_call add(
		cerl::Caller* caller,
		_AddResult& result,
		const NUM32 number1,
		const NUM32 number2)
	{
		result.value = number1 + number2;
		result._code = code_ok;
	}

	void cerl_call dec(
		cerl::Caller* caller,
		_DecResult& result,
		const cerl::UInt32 number1,
		const cerl::UInt32 number2,
		const cerl::UInt32 number3)
	{
		result._code = code_ok;
		result.value = number1 - number2 - number3;
	}

	void cerl_call helloworld1(
		cerl::Caller* caller,
		_Helloworld1Result& result)
	{
		result._code = code_ok;
		result.value = 1;
		printf("\nhelloworld1 is called\n");
	}

	void cerl_call helloworld2(
		cerl::Caller* caller,
		_Helloworld2Result& result)
	{
		result._code = code_ok;
		printf("\nhelloworld2 is called\n");
	}

	void cerl_call square2(
		cerl::Caller* caller,
		_Square2Result& result,
		const NUM number)
	{
		if ((number < 100) && (number > 0))
		{
			result._code = code_ok;
			result.str = number * number;
		}
		else if (number >= 100)
		{
			result._code = code_error;
			result.error.reason = code_notimpl;
		}
		else if (number <= 0)
		{
			result._code = code_error;
			result.error.reason = code_confliction;
		}
	}

	/*[async]*/
	void cerl_call stop(cerl::Caller* caller)
	{
		printf("\nasync_stop is called\n");
	}

	/*[async]*/
	void cerl_call async1(
		cerl::Caller* caller,
		const cerl::UInt32 number1)
	{
		printf("\nasync1(%d) is called\n", number1);
	}

	/*[async]*/
	void cerl_call async2(
		cerl::Caller* caller,
		const cerl::UInt32 number1,
		const cerl::UInt32 number2)
	{
		printf("\nasync2(%d, %d) is called\n", number1, number2);
	}

	void cerl_call _run(cerl::Fiber self, SOCKET socketListen);
};

} //namespace

#include "CalcStub.h"

#endif /* USERFUNC_CALC_IMPL_H */ 
