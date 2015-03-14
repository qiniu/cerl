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
// Module: tpl/Phoenix.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Phoenix.h 554 2008-05-25 07:50:50Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BOOST_PHOENIX_H
#define TPL_BOOST_PHOENIX_H

#ifndef BOOST_SPIRIT_PHOENIX_HPP
#include <boost/spirit/phoenix.hpp>
#endif

#ifndef TPL_REGEX_ASSIGN_H
#include "../regex/Assign.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Exec

struct TplVoid {
};

template <class OperaT, class ValueT = TplVoid>
class Exec
{
private:
	OperaT m_opera;

public:
	typedef ValueT value_type;
	typedef Action<Exec> action_type;

public:
	Exec(const OperaT& opera) : m_opera(opera) {
	}

	void TPL_CALL operator()(const value_type& val) const {
		m_opera(val);
	}
};

template <class OperaT>
class Exec<OperaT, TplVoid>
{
private:
	OperaT m_opera;

public:
	typedef SimpleAction<Exec> action_type;

public:
	Exec(const OperaT& opera) : m_opera(opera) {
	}

	void TPL_CALL operator()() const {
		m_opera();
	}
};

// -------------------------------------------------------------------------
// class Phoenix

template <class ValueT = TplVoid>
class Phoenix
{
public:
	template <class OperaT>
	typename Exec<OperaT, ValueT>::action_type TPL_CALL operator[](const OperaT& op) const {
		return typename Exec<OperaT, ValueT>::action_type(op);
	}
};

// -------------------------------------------------------------------------
// TPL_LAMBDA_LOCAL

template <class ValueT>
class PhoenixLocal_
{
public:
	typedef phoenix::actor<phoenix::variable<ValueT> > const Ref;
};

#define TPL_PHOENIX_LOCAL(ValueT, variable)										\
	ValueT _tpl_##variable = ValueT();											\
	tpl::PhoenixLocal_<ValueT>::Ref variable(_tpl_##variable)

template <class ValueT>
__forceinline Action<Assign<ValueT> > TPL_CALL assign(
	const phoenix::actor<phoenix::variable<ValueT> >& var_)
{
	ValueT& result = var_();
	return Action<Assign<ValueT> >(result);
}

template <class ValueT>
__forceinline Action<Assign<ValueT> > TPL_CALL assign(
	phoenix::actor<phoenix::variable<ValueT> >& var_)
{
	ValueT& result = var_();
	return Action<Assign<ValueT> >(result);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEXP_H */
