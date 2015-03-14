/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/QuitEvent.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: QuitEvent.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_QUITEVENT_H
#define ASYNC_QUITEVENT_H

#ifndef ASYNC_LIST_H
#include "List.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class QuitEvent

class QuitEvent
{
private:
	DList listeners;

public:
	struct Listener : DListNode
	{
		~Listener() { erase(); }
		virtual void cerl_call onQuit() = 0;
	};

public:
	void cerl_call sink(Listener* listener)
	{
		listeners.push_back(listener);
	}

	void cerl_call fire()
	{
		for (Listener* p = (Listener*)listeners.first(); !listeners.done(p); p = (Listener*)p->next())
			p->onQuit();
		listeners.clear();
	}
};

// -------------------------------------------------------------------------
// class CloseObjectOnQuit

template <class ObjectT>
class CloseObjectOnQuit : public QuitEvent::Listener
{
private:
	ObjectT& obj;

public:
	CloseObjectOnQuit(ObjectT& o)
		: obj(o)
	{
	}

	template <class SourceT>
	CloseObjectOnQuit(SourceT* source, ObjectT& o)
		: obj(o)
	{
		source->quitEvent.sink(this);
	}

	template <class SourceT>
	void cerl_call sink(SourceT* source)
	{
		source->quitEvent.sink(this);
	}

	void cerl_call onQuit()
	{
		obj.close();
	}
};

// -------------------------------------------------------------------------
// class SetFlagOnQuit

class SetFlagOnQuit : public QuitEvent::Listener
{
private:
	bool& flag;

public:
	SetFlagOnQuit(bool& f)
		: flag(f)
	{
	}

	template <class SourceT>
	SetFlagOnQuit(SourceT* source, bool& f)
		: flag(f)
	{
		source->quitEvent.sink(this);
	}

	template <class SourceT>
	void cerl_call sink(SourceT* source)
	{
		source->quitEvent.sink(this);
	}

	void cerl_call onQuit()
	{
		flag = true;
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_QUITEVENT_H */
