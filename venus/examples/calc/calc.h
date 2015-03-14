/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/exmaples/calc/calc.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: calc.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef CALC_H
#define CALC_H

#ifndef CALC_BASE_H
#include "calc_base.h"
#endif

#ifndef VENUS_CONNECTION_H
#include <venus/Connection.h>
#endif

#pragma pack(1)

namespace calc {
	
// =========================================================================
// class Abacus
	
class Abacus : public AbacusBase, public cerl::Connection 
{
public:
	Abacus(const cerl::Connection& conn)
		: cerl::Connection(conn)
	{
	}
	
	void cerl_call add(
		cerl::ScopedAlloc& _alloc,
		_AddResult& _result,
		cerl::UInt32 a,
		cerl::UInt32 b
		)
	{
		_AddArgs _args = {a, b};
		cerl::Connection::dbg_call(this, _alloc, _result, (cerl::FID)code_add, _args);
	}
};

}

#pragma pack()

#endif /* CALC_H */
