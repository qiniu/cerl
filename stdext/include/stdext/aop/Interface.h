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
// Module: stdext/aop/Interface.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// Description: AOP - Aspect Oriented Programming
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_AOP_INTERFACE_H
#define STDEXT_AOP_INTERFACE_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// interface IConnection

interface IConnection
{
	virtual void __stdcall disconnect() = 0;
};

// -------------------------------------------------------------------------
// class Connection - IConnection* HandleClass

class Connection
{
private:
	Connection(const Connection&);
	void operator=(const Connection&);

private:
	IConnection* m_conn;

public:
	Connection(IConnection* conn = NULL) : m_conn(conn) {}
	~Connection() {
		if (m_conn)
			m_conn->disconnect();
	}

	void winx_call operator=(IConnection* conn)
	{
		WINX_ASSERT(m_conn == NULL);
		m_conn = conn;
	}

	void winx_call disconnect()
	{
		if (m_conn)
		{
			m_conn->disconnect();
			m_conn = NULL;
		}
	}
};

typedef Connection HConnection;

// -------------------------------------------------------------------------
// interface Event

class FakeTarget
{
};

typedef void* FakeMethod;

interface IEvent
{
	virtual IConnection* __stdcall _addListener(FakeTarget* target, FakeMethod method) = 0;
};

template <class EventTag>
interface SomeEvent : IEvent
{
	template <class Target, class ParametersList>
	IConnection* __stdcall addListener(
		Target* target,
		void (__stdcall Target::*method)(ParametersList)
		);
};

#define DEFINE_EVENT(Event, ParametersList)									\
interface Event	: std::IEvent												\
{																			\
	template <class Target, class TargetBase>								\
	__forceinline std::IConnection* winx_call addListener(					\
		Target* target,														\
		void (__stdcall TargetBase::*method) ParametersList)				\
	{																		\
		return _addListener(												\
			(std::FakeTarget*)static_cast<TargetBase*>(target),				\
			*(void**)&method);												\
	}																		\
}

// -------------------------------------------------------------------------
// interface IEventContainer

typedef IID EventID;
typedef const EventID& EventIDRef;

interface IEventContainer
{
	virtual HRESULT __stdcall findEvent(EventIDRef name, void** event) = 0;
};

// -------------------------------------------------------------------------
// EID, DEFINE_EID

#define EID(Event)	WINX_UUID(Event)

#define DEFINE_EID(Event, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)	\
	WINX_DEFINE_IID(Event, sz, x, s1, s2, c1, c2, c3, c4, c5, c6, c7, c8)

// -------------------------------------------------------------------------
// $Log: $

NS_STDEXT_END

#endif /* STDEXT_AOP_INTERFACE_H */
