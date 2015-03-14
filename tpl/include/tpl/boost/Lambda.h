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
// Module: tpl/Lambda.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Lambda.h 554 2008-05-25 07:50:50Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BOOST_LAMBDA_H
#define TPL_BOOST_LAMBDA_H

#if defined(_MSC_VER)
#pragma warning(disable:4819)
#endif

#ifndef BOOST_LAMBDA_LAMBDA_HPP
#include <boost/lambda/lambda.hpp>
#endif

#ifndef TPL_REGEX_ASSIGN_H
#include "../regex/Assign.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class Lambda

template <class OperaT, class ValueT, int arity = boost::lambda::get_arity<OperaT>::value >
class LambdaExec_
{
};

template <class OperaT, class ValueT>
class LambdaExec_<OperaT, ValueT, 0x01>
{
private:
	OperaT m_opera;

public:
	typedef ValueT value_type;
	typedef Action<LambdaExec_> action_type;

public:
	LambdaExec_(const OperaT& opera) : m_opera(opera) {
	}

	void TPL_CALL operator()(const value_type& val) const {
		m_opera(val);
	}
};

template <class OperaT, class ValueT>
class LambdaExec_<OperaT, ValueT, 0x00>
{
private:
	OperaT m_opera;

public:
	typedef SimpleAction<LambdaExec_> action_type;

public:
	LambdaExec_(const OperaT& opera) : m_opera(opera) {
	}

	void TPL_CALL operator()() const {
		m_opera();
	}
};

template <class ValueT>
class Lambda
{
public:
	template <class OperaT>
	typename LambdaExec_<OperaT, ValueT>::action_type TPL_CALL operator[](const OperaT& op) const {
		return typename LambdaExec_<OperaT, ValueT>::action_type(op);
	}
};

// -------------------------------------------------------------------------
// TPL_LAMBDA_LOCAL

template <class ValueT>
class LambdaLocal_
{
public:
	typedef boost::lambda::lambda_functor<boost::lambda::identity<ValueT&> > const Ref;
};

#define TPL_LAMBDA_LOCAL(ValueT, variable)										\
	ValueT _tpl_##variable = ValueT();											\
	tpl::LambdaLocal_<ValueT>::Ref variable = boost::lambda::var(_tpl_##variable)

template <class ValueT>
__forceinline Action<Assign<ValueT> > TPL_CALL assign(
	const boost::lambda::lambda_functor<boost::lambda::identity<ValueT&> >& var_)
{
	ValueT& result = var_();
	return Action<Assign<ValueT> >(result);
}

template <class ValueT>
__forceinline Action<Assign<ValueT> > TPL_CALL assign(
	boost::lambda::lambda_functor<boost::lambda::identity<ValueT&> >& var_)
{
	ValueT& result = var_();
	return Action<Assign<ValueT> >(result);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEXP_H */
