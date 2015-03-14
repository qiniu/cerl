/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/scopedbg/CheckException.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Contributor: sting.feng@gmail.com
// Date: 2007-2-4 15:00:59
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_SCOPEDBG_CHECKEXCEPTION_H
#define STDEXT_SCOPEDBG_CHECKEXCEPTION_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

#if !defined(_MSC_VER)
#error "Not support."
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class CheckException - 该类不允许继承

template <class Base>
class CheckException : public Base
{
private:
	// declared but not implemented, allocated on stack only
	static void* operator new(size_t);
	static void operator delete(void*);

	static bool winx_call _isNormalCall(PBYTE callAddr)
	{
		return callAddr[0] != 0xC3 && // is not 'ret'
			*((PWORD)callAddr-1) != 0xD0FF; // is not 'call eax'
	}

public:
	// disable global optimization and setup frame pointer to get return address
	#pragma optimize( "g" , off )
	#pragma optimize( "y" , on )

	~CheckException()
	{
		PBYTE callAddr;
		__asm
		{
#if !defined(_DEBUG) && defined(WINX_VC_NET_LE2003)
			mov eax, CheckException<Base>::~CheckException;
			cmp byte ptr [eax], 85; // is 'push ebp' ?
			je has_frame_pointers;
			mov eax, [esp+4];
			jmp set_call_addr;
	has_frame_pointers:
			mov eax, [ebp+4];
	set_call_addr:
			mov callAddr, eax;
#else
			mov eax, [ebp+4];
			mov callAddr, eax;
#endif
		}
		Base::onLeave( _isNormalCall(callAddr) ); // is normal call?
	}

	// use project default optimization settings
	#pragma optimize("", on)

public:
	CheckException() {}

	template <class ArgT1>
	CheckException(ArgT1 arg1) : Base(arg1) {}

	template <class ArgT1, class ArgT2>
	CheckException(ArgT1 arg1, ArgT2 arg2) : Base(arg1, arg2) {}

	template <class ArgT1, class ArgT2, class ArgT3>
	CheckException(ArgT1 arg1, ArgT2 arg2, ArgT3 arg3) : Base(arg1, arg2, arg3) {
	}

	template <class ArgT1, class ArgT2, class ArgT3, class ArgT4>
	CheckException(ArgT1 arg1, ArgT2 arg2, ArgT3 arg3, ArgT4 arg4) : Base(arg1, arg2, arg3, arg4) {
	}
};

// -------------------------------------------------------------------------
// class TestCheckException

template <class LogT>
class TestCheckException
{
	WINX_TEST_SUITE(TestCheckException);
		WINX_TEST(test);
	WINX_TEST_SUITE_END();

public:
	void setUp() {}
	void tearDown() {}

public:
	class SimpleCheck
	{
	private:
		LogT& log;
		bool isNormal;
	public:
		SimpleCheck(LogT* aLog, bool aIsNormal)
			: log(*aLog), isNormal(aIsNormal) {
		}
		void winx_call onLeave(bool aIsNormal)
		{
			if (aIsNormal)
				log.print("onNormalCall\n");
			else 
				log.print("onException\n");
			AssertExp(isNormal == aIsNormal);
		}
	};

	void test(LogT& log)
	{
		try {{
			CheckException<SimpleCheck> chkNormal(&log, true);
		}{
			CheckException<SimpleCheck> chkExcept(&log, false);
			throw 0;
		}}
		catch (...) {
		}
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_SCOPEDBG_CHECKEXCEPTION_H */
