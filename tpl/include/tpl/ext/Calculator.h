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
// Module: tpl/ext/Calculator.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Calculator.h 658 2008-06-14 10:12:31Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_EXT_CALCULATOR_H
#define TPL_EXT_CALCULATOR_H

#ifndef TPL_REGEXP_H
#include "../RegExp.h"
#endif

#ifndef TPL_EMU_FUNCTION_H
#include "../emu/Function.h"
#endif

#if !defined(_FUNCTIONAL_) && !defined(_FUNCTIONAL)
#include <functional>
#endif

NS_TPL_BEGIN

// =========================================================================
// class Function

template <class Op, class StackT, int nArity>
class Function : public emu::Function<Op, nArity>
{
private:
	typedef emu::Function<Op, nArity> Base;

	StackT& m_stk;

public:
	Function(StackT& stk) : m_stk(stk), Base() {}

	template <class T1>
	Function(StackT& stk, const T1& arg) : m_stk(stk), Base(arg) {}

public:
	void TPL_CALL operator()() const {
		Base::operator()(m_stk);
	}
};

// -------------------------------------------------------------------------
// function calc/1-ary

template <template <class Type> class Op_, class StackT>
struct CalcTraits_ {
	enum { n_arity = emu::ArityTraits<Op_>::value };
	typedef Op_<typename StackT::value_type> Op;
	typedef Function<Op, StackT, n_arity> action_type;
};

template <template <class Type> class Op_, class StackT> __forceinline
SimpleAction<typename CalcTraits_<Op_, StackT>::action_type> TPL_CALL calc(StackT& stk) {
	return SimpleAction<typename CalcTraits_<Op_, StackT>::action_type>(stk);	
}

// =========================================================================
// function check_arity

// Usage: check_arity<nArity>(nArgv)

inline void throw_arity_error() {
	throw std::invalid_argument("function arity is not consistent!");
}

template <int nArity>
class CheckArity
{
private:
	const int& m_nArgv;

public:
	CheckArity(const int& nArgv)
		: m_nArgv(nArgv) {}

public:
	void TPL_CALL operator()() const {
		if (m_nArgv != nArity)
			throw_arity_error();
	}
};

template <int nArity> __forceinline
SimpleAction<CheckArity<nArity> > TPL_CALL check_arity(const int& nArgv) {
	return SimpleAction<CheckArity<nArity> >(nArgv);	
}

// =========================================================================
// function calc/2-ary, calc/3-ary

template <int nArity, class StackT>
struct CalcTraits_N_ {
	typedef typename StackT::value_type Ty;
	typedef typename emu::OpTraits<nArity, Ty>::op_type Op;
	typedef Function<Op, StackT, nArity> action_type;
};

template <int nArity, class StackT>
struct CalcTraitsEx_ {
	typedef CalcTraits_N_<nArity, StackT> Tr_;
	typedef CheckArity<nArity> Action1;
	typedef typename Tr_::action_type Action2;
	typedef typename Tr_::Op Op;
	typedef AndSAct<Action1, Action2> action_type;
};

#define TPL_CALC_IMPL2_(nArity)															\
template <class StackT> __forceinline													\
SimpleAction<typename CalcTraits_N_<nArity, StackT>::action_type> TPL_CALL				\
calc(StackT& stk, typename CalcTraits_N_<nArity, StackT>::Op fn) {						\
	return SimpleAction<typename CalcTraits_N_<nArity, StackT>::action_type>(stk, fn);	\
}

#define TPL_CALC_IMPL3_(nArity)															\
template <class StackT> __forceinline													\
SimpleAction<typename CalcTraitsEx_<nArity, StackT>::action_type> TPL_CALL				\
calc(StackT& stk, typename CalcTraitsEx_<nArity, StackT>::Op fn, const int& nArgv) {	\
	return SimpleAction<typename CalcTraitsEx_<nArity, StackT>::Action1>(nArgv) +		\
			SimpleAction<typename CalcTraitsEx_<nArity, StackT>::Action2>(stk, fn);		\
}

#define TPL_CALC_IMPL_(nArity)															\
	TPL_CALC_IMPL2_(nArity)																\
	TPL_CALC_IMPL3_(nArity)

TPL_CALC_IMPL_(1)
TPL_CALC_IMPL_(2)
TPL_CALC_IMPL_(3)
TPL_CALC_IMPL_(4)
TPL_CALC_IMPL_(5)
TPL_CALC_IMPL_(6)

// =========================================================================
// class VargsFunction

template <class Op, class StackT>
class VargsFunction
{
private:
	typedef typename StackT::value_type value_type;

private:
	StackT& m_stk;
	const Op m_op;
	const int& m_nArgv;

public:
	VargsFunction(StackT& stk, const Op& op, const int& nArgv)
		: m_stk(stk), m_op(op), m_nArgv(nArgv) {}

public:
	void TPL_CALL operator()() const {
		const int count = m_nArgv;
		value_type* args = (value_type*)alloca(sizeof(value_type) * count);	
		for (int i = count; i--;) {		
			args[i] = m_stk.back();
			m_stk.pop_back();
		}
		m_stk.push_back(m_op(static_cast<const value_type*>(args), count));
	}
};

// -------------------------------------------------------------------------
// function calc/3-ary

template <class StackT, class IntT>
class CalcTraits_Varg_
{
public:
	typedef typename StackT::value_type Ty;
	typedef Ty (*Op)(const Ty args[], IntT count);
	typedef VargsFunction<Op, StackT> action_type;
};

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_Varg_<StackT, int>::action_type> TPL_CALL
calc(StackT& stk, typename CalcTraits_Varg_<StackT, int>::Op fn, const int& nArity) {
	return SimpleAction<typename CalcTraits_Varg_<StackT, int>::action_type>(stk, fn, nArity);
}

template <class StackT> __forceinline
SimpleAction<typename CalcTraits_Varg_<StackT, size_t>::action_type> TPL_CALL
calc(StackT& stk, typename CalcTraits_Varg_<StackT, size_t>::Op fn, const int& nArity) {
	return SimpleAction<typename CalcTraits_Varg_<StackT, size_t>::action_type>(stk, fn, nArity);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_EXT_CALCULATOR_H */

