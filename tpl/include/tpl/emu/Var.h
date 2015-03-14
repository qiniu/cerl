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
// Module: tpl/emu/Var.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Var.h 588 2008-05-28 07:22:04Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_VAR_H
#define TPL_EMU_VAR_H

#ifndef TPL_EMU_INSTRUCTION_H
#include "Instruction.h"
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// class Local

// Usage: local <count>

template <class StackT, class ExecuteContextT>
class Local
{
public:
	static void op0(Operand, StackT& stk, ExecuteContextT&) {
		TPL_EMU_INSTR_DEBUG("local");
		stk.push_back(typename StackT::value_type());
	}

	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		TPL_EMU_INSTR_DEBUG("local " << para.val);
		stk.resize(stk.size() + para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op0);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n);
	}
};

// =========================================================================
// class PushVar

// Usage: push_var <delta> ; here <delta> can be -n ~ -1, 0 ~ m
//	 arg1 = -n
//	 arg2 = -(n-1)
//	 ...
//	 argn = -1
//	 local_var1 = 0
//	 local_var2 = 1,
//	 ...
//	 local_varm = m

template <class StackT, class ExecuteContextT>
class PushVar
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"push_var " << para.ival <<
			"\t; push: " << stk[context.frame() + para.ival] );
		stk.push_back(stk[context.frame() + para.ival]);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class LeaVar

// Usage: lea_var <delta>	; here <delta> can be -n ~ -1, 0 ~ m
//	 arg1 = -n
//	 arg2 = -(n-1)
//	 ...
//	 argn = -1
//	 local_var1 = 0
//	 local_var2 = 1,
//	 ...
//	 local_varm = m

template <class StackT, class ExecuteContextT>
class LeaVar
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"lea_var " << para.ival <<
			"\t; value: " << stk[context.frame() + para.ival] );
		stk.push_back(
			ref_to_variant(stk[context.frame() + para.ival])
			);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class AssignVar

// Usage: assgin_var <delta>	; here <delta> can be -n ~ -1, 0 ~ m

template <class StackT, class ExecuteContextT>
class AssignVar
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"assgin_var " << para.ival <<
			"\t; value: " << stk[context.frame() + para.ival] << " => " << stk.back() );
		stk[context.frame() + para.ival] = stk.back();
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class Variable

#define TPL_EMU_UNDEFINED_VAR	(0x7fffffff)

class Variable
{
private:
	ptrdiff_t m_var;
	Variable* m_prev;

public:	
	class Context
	{
	private:
		union {
			Variable* m_head; // for defining procedure arglist
			ptrdiff_t m_var;  // for defining local variables
		};
		friend class Variable;

	public:
		Context() {
			m_var = 0;
		}
		
		void TPL_CALL begin_arglist() {
			m_head = NULL;
		}
	
		void TPL_CALL end_arglist() {
			ptrdiff_t var_ = CallerFrame::ARITY;
			for (Variable* p = m_head; p; p = p->m_prev) {
				p->m_var = var_;
				--var_;
			}
			m_var = 0;
		}
		
		void TPL_CALL arglist(Variable& a) {
			a.m_var = CallerFrame::ARITY;
		}

		void TPL_CALL arglist(Variable& a, Variable& a2) {
			a2.m_var = CallerFrame::ARITY;
			a.m_var = CallerFrame::ARITY - 1;
		}

		void TPL_CALL arglist(Variable& a, Variable& a2, Variable& a3) {
			a3.m_var = CallerFrame::ARITY;
			a2.m_var = CallerFrame::ARITY - 1;
			a.m_var = CallerFrame::ARITY - 2;
		}

		void TPL_CALL arglist(Variable& a, Variable& a2, Variable& a3, Variable& a4) {
			a4.m_var = CallerFrame::ARITY;
			a3.m_var = CallerFrame::ARITY - 1;
			a2.m_var = CallerFrame::ARITY - 2;
			a.m_var = CallerFrame::ARITY - 3;
		}

		void TPL_CALL arglist(Variable& a, Variable& a2, Variable& a3, Variable& a4, Variable& a5) {
			a5.m_var = CallerFrame::ARITY;
			a4.m_var = CallerFrame::ARITY - 1;
			a3.m_var = CallerFrame::ARITY - 2;
			a2.m_var = CallerFrame::ARITY - 3;
			a.m_var = CallerFrame::ARITY - 4;
		}

		void TPL_CALL arglist(Variable& a, Variable& a2, Variable& a3, Variable& a4, Variable& a5, Variable& a6) {
			a6.m_var = CallerFrame::ARITY;
			a5.m_var = CallerFrame::ARITY - 1;
			a4.m_var = CallerFrame::ARITY - 2;
			a3.m_var = CallerFrame::ARITY - 3;
			a2.m_var = CallerFrame::ARITY - 4;
			a.m_var = CallerFrame::ARITY - 5;
		}
	};
	
	friend class Context;

public:
	Variable() : m_prev(NULL), m_var(TPL_EMU_UNDEFINED_VAR) {}

	bool TPL_CALL defined() const {
		return m_var != TPL_EMU_UNDEFINED_VAR;
	}

	void TPL_CALL define_arg(Context& context) {
		TPL_ASSERT(!defined());
		m_prev = context.m_head;
		context.m_head = this;
	}
	
	void TPL_CALL define_var(Context& context) {
		TPL_ASSERT(!defined());
		m_var = context.m_var++;
	}
	
	ptrdiff_t TPL_CALL get() const {
		TPL_ASSERT(defined());
		return m_var;
	}
};

// =========================================================================
// class DefineArg/DefineVar

class DefineArg
{
private:
	Variable& m_var;
	
public:
	DefineArg(Variable& var_) : m_var(var_) {}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_var.define_arg(code.get_context());
	}
};

class DefineVar
{
private:
	Variable& m_var;
	
public:
	DefineVar(Variable& var_) : m_var(var_) {}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_var.define_var(code.get_context());
		typedef typename CodeT::stack_type StackT;
		typedef typename CodeT::execute_context ExecuteContextT;
		code.push_back(Local<StackT, ExecuteContextT>::instr());
	}
};

class Define2Var
{
private:
	Variable& m_var;
	Variable& m_var2;
	
public:
	Define2Var(Variable& var_, Variable& var2_)
		: m_var(var_), m_var2(var2_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		typename CodeT::code_context& context = code.get_context();
		m_var.define_var(context);
		m_var2.define_var(context);
		typedef typename CodeT::stack_type StackT;
		typedef typename CodeT::execute_context ExecuteContextT;
		code.push_back(Local<StackT, ExecuteContextT>::instr(2));
	}
};

class Define3Var
{
private:
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	
public:
	Define3Var(Variable& var_, Variable& var2_, Variable& var3_)
		: m_var(var_), m_var2(var2_), m_var3(var3_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		typename CodeT::code_context& context = code.get_context();
		m_var.define_var(context);
		m_var2.define_var(context);
		m_var3.define_var(context);
		typedef typename CodeT::stack_type StackT;
		typedef typename CodeT::execute_context ExecuteContextT;
		code.push_back(Local<StackT, ExecuteContextT>::instr(3));
	}
};

class Define4Var
{
private:
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	Variable& m_var4;
	
public:
	Define4Var(Variable& var_, Variable& var2_, Variable& var3_, Variable& var4_)
		: m_var(var_), m_var2(var2_), m_var3(var3_), m_var4(var4_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		typename CodeT::code_context& context = code.get_context();
		m_var.define_var(context);
		m_var2.define_var(context);
		m_var3.define_var(context);
		m_var4.define_var(context);
		typedef typename CodeT::stack_type StackT;
		typedef typename CodeT::execute_context ExecuteContextT;
		code.push_back(Local<StackT, ExecuteContextT>::instr(4));
	}
};

class Define5Var
{
private:
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	Variable& m_var4;
	Variable& m_var5;
	
public:
	Define5Var(Variable& var_, Variable& var2_, Variable& var3_, Variable& var4_, Variable& var5_)
		: m_var(var_), m_var2(var2_), m_var3(var3_), m_var4(var4_), m_var5(var5_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		typename CodeT::code_context& context = code.get_context();
		m_var.define_var(context);
		m_var2.define_var(context);
		m_var3.define_var(context);
		m_var4.define_var(context);
		m_var5.define_var(context);
		typedef typename CodeT::stack_type StackT;
		typedef typename CodeT::execute_context ExecuteContextT;
		code.push_back(Local<StackT, ExecuteContextT>::instr(5));
	}
};

class Define6Var
{
private:
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	Variable& m_var4;
	Variable& m_var5;
	Variable& m_var6;
	
public:
	Define6Var(Variable& var_, Variable& var2_, Variable& var3_, Variable& var4_, Variable& var5_, Variable& var6_)
		: m_var(var_), m_var2(var2_), m_var3(var3_), m_var4(var4_), m_var5(var5_), m_var6(var6_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		typename CodeT::code_context& context = code.get_context();
		m_var.define_var(context);
		m_var2.define_var(context);
		m_var3.define_var(context);
		m_var4.define_var(context);
		m_var5.define_var(context);
		m_var6.define_var(context);
		typedef typename CodeT::stack_type StackT;
		typedef typename CodeT::execute_context ExecuteContextT;
		code.push_back(Local<StackT, ExecuteContextT>::instr(6));
	}
};

// =========================================================================
// class VarInit

class VarInit
{
private:
	Variable& m_var;
	
public:
	VarInit(Variable& var_) : m_var(var_) {}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_var.define_var(code.get_context());
	}
};

// =========================================================================
// class VarOp

template <class InstructionT>
class VarOp // push(var), assign(var), lea(var)
{
private:
	typedef Variable VarT;
	
	const VarT& m_var;

public:
	VarOp(const VarT& var_) : m_var(var_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		code.push_back(InstructionT::instr(m_var.get()));
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_VAR_H */

