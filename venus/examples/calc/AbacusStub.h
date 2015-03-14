/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/exmaples/calc/AbacusStub.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: AbacusStub.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef CALC_ABACUSSTUB_H
#define CALC_ABACUSSTUB_H

#ifndef CALC_ABACUSIMPL_H
#include "AbacusImpl.h"
#endif

namespace calc {

// =========================================================================
// class AbacusStub
	
class AbacusStub : public AbacusImpl
{
public:

	template <class ArchiveT>
	void cerl_call _process(cerl::Caller* _caller, ArchiveT& _ar)
	{
		cerl::FID _fid = _caller->getFid();
		if (is_async_call(_fid))
		{
			_handle_cast(_caller, _fid, _ar);
		}
		else
		{
			_handle_call(_caller, _fid, _ar);
		}
	}
	
	template <class ArchiveT>
	void cerl_call _handle_cast(cerl::Caller* _caller, cerl::FID _fid, ArchiveT& _ar)
	{
		cerl::skipMailBody(_ar);
	}

	template <class ArchiveT>
	void cerl_call _handle_call(cerl::Caller* _caller, cerl::FID _fid, ArchiveT& _ar)
	{
		switch (_fid)
		{
		case code_add:
			{
				_AddArgs _args;
				const bool _fOk = cerl::getMailBody(_ar, _args);
				
				_AddResult _result = code_error;
				if (_fOk)
				{
					CERL_DUMP_RECV_REQUEST(_caller->getSelf(), this, _fid, _args);
					add(_caller, _result, _args.a, _args.b);
				}

				_caller->dbg_reply(this, _result);
			}
			break;
		default:
			{
				cerl::handle_call(this, _caller, _fid, _ar);
			}
		}
	}
};

inline HRESULT cerl_call AbacusImpl::_run(cerl::Fiber self, SOCKET socketListen)
{
	return cerl::ServerRunner<AbacusStub>::run(self, static_cast<AbacusStub*>(this), socketListen);
}
	
}

#endif /* CALC_ABACUSSTUB_H */
