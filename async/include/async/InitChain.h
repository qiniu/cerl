/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/InitChain.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: InitChain.h 2577 2010-04-20 07:38:10Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_INITCHAIN_H
#define ASYNC_INITCHAIN_H

#ifndef ASYNC_LIST_H
#include "List.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class InitChain

template <class Unused>
class InitChain
{
private:
	static Queue g_init;
	static Stack g_term;

public:
	typedef void (cerl_callback *FInit)(void* param);
	typedef void (cerl_callback *FTerm)(void* param);
	
	struct InitNode : public Queue::Node
	{
		FInit fn;
	};

	struct TermNode : public Stack::Node
	{
		FTerm fn;
	};

	class Node
	{
	private:
		InitNode initNode;
		TermNode termNode;
		
	public:
		Node(FInit fnInit = NULL, FTerm fnTerm = NULL)
		{
			if (fnInit) {
				initNode.fn = fnInit;
				g_init.push(&initNode);
			}
			if (fnTerm) {
				termNode.fn = fnTerm;
				g_term.push(&termNode);
			}
		}
	};

	static void cerl_call init(void* param)
	{
		for (InitNode* node = (InitNode*)g_init.top(); node; node = (InitNode*)node->prev)
		{
			node->fn(param);
		}
	}

	static void cerl_call term(void* param)
	{
		for (TermNode* node = (TermNode*)g_term.top(); node; node = (TermNode*)node->prev)
		{
			node->fn(param);
		}
	}
};

template <class Unused>
Queue InitChain<Unused>::g_init;

template <class Unused>
Stack InitChain<Unused>::g_term;

// =========================================================================

NS_CERL_END

#endif
