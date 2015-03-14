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
// Module: tpl/emu/Code.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Code.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_CODE_H
#define TPL_EMU_CODE_H

#ifndef TPL_EMU_FUNCTION_H
#include "Function.h"
#endif

#ifndef TPL_EMU_INSTRUCTION_H
#include "Instruction.h"
#endif

#ifndef TPL_EMU_LABEL_H
#include "Label.h"
#endif

#ifndef TPL_EMU_VAR_H
#include "Var.h"
#endif

#ifndef STDEXT_DEQUE_H
#include "../../../../stdext/include/stdext/Deque.h"
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// class Stack

template <class ValT>
class Stack : public std::deque<ValT>
{
private:
	typedef std::deque<ValT> Base;
	
public:
	__forceinline const ValT& TPL_CALL top() const {
		return Base::back();
	}
	
	__forceinline void TPL_CALL push(const ValT& val) {
		Base::push_back(val);
	}

	__forceinline void TPL_CALL pop() {
		Base::pop_back();
	}
};

// =========================================================================
// class Context: Execute Context

class ContextBase
{
private:
	size_t m_ip;
	size_t m_frame;

public:
	ContextBase(size_t ip)
		: m_ip(ip), m_frame(0) {
	}

public:
	size_t TPL_CALL frame() const {
		return m_frame;
	}

	void TPL_CALL frame(size_t frame_) {
		m_frame = frame_;
	}
	
public:
	size_t TPL_CALL position() const {
		return m_ip;
	}
	
	void TPL_CALL position(size_t ip) {
		m_ip = ip;
	}

	template <class CodeT>
	const typename CodeT::instruction_type& TPL_CALL next(const CodeT& code) {
		return code[m_ip++];
	}
};

template <class AllocT, bool bDebug = false>
class Context : public ContextBase
{
private:
	AllocT& m_alloc;

public:
	enum { debug = bDebug };

	typedef AllocT alloc_type;
	
	Context(AllocT& alloc, size_t ip)
		: m_alloc(alloc), ContextBase(ip) {
	}
	
	AllocT& TPL_CALL get_alloc() const {
		return m_alloc;
	}
};

template <>
class Context<void, true> : public ContextBase
{
public:
	enum { debug = TRUE };

	Context(size_t ip)
		: ContextBase(ip) {
	}
};

template <>
class Context<void, false> : public ContextBase
{
public:
	enum { debug = FALSE };
	
	Context(size_t ip)
		: ContextBase(ip) {
	}
};

// =========================================================================
// class CodeContext

class CodeContext : public Variable::Context
{
};

// =========================================================================
// class CodeGenerator

template <class GenT>
class CodeGenerator : public GenT
{
public:
	CodeGenerator() : GenT() {} 

	template <class T1>
	CodeGenerator(const T1& x) : GenT(x) {} 

	template <class T1>
	CodeGenerator(T1& x) : GenT(x) {} 

	template <class T1, class T2>
	CodeGenerator(T1& x, T2& y) : GenT(x, y) {} 

// concept:
//
//	template <class CodeT>
//	void TPL_CALL generate(CodeT& code) const;
};

// =========================================================================
// class InstrCode/ExtInstrCode

template <class ValT, class InstructionT>
class InstrCode // InstrCode: push, repush, etc
{
private:
	const ValT m_val;

public:
	InstrCode(const ValT& val) : m_val(val) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		code.push_back(InstructionT::instr(code.get_alloc(), m_val));
	}
};

template <class ValT, class InstructionT, class ValT2>
class ExtInstrCode
{
private:
	const ValT m_val;

public:
	ExtInstrCode(const ValT& val) : m_val(val) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		typename CodeT::alloc_type& alloc = code.get_alloc();
		code.push_back(InstructionT::instr(alloc, ValT2(alloc, m_val)));
	}
};

template <class ValT, class InstructionT>
class ExtInstrCode<ValT, InstructionT, ValT>
{
	// error!
};

// =========================================================================
// class Code

template <class ValT, class ExecuteContextT, class AllocT = DefaultAllocator>
class Code : public NS_STDEXT::Deque<Instruction<Stack<ValT>, ExecuteContextT>, AllocT>
{
private:
	typedef NS_STDEXT::Deque<Instruction<Stack<ValT>, ExecuteContextT>, AllocT> Base;
	
	Code(const Code&);
	void operator=(const Code&);

	CodeContext m_context;

public:
	typedef AllocT alloc_type;
	typedef Stack<ValT> stack_type;
	typedef CodeContext code_context;
	typedef ExecuteContextT execute_context;
	typedef Instruction<stack_type, execute_context> instruction_type;

	explicit Code(AllocT& alloc)
		: Base(alloc) {
	}

	CodeContext& TPL_CALL get_context() {
		return m_context;
	}

public:
	// Instruction:
	
	Code& TPL_CALL operator,(const instruction_type& instr_) {
		Base::push_back(instr_);
		return *this;
	}
	
	Code& TPL_CALL operator<<(const instruction_type& instr_) {
		Base::push_back(instr_);
		return *this;
	}

	// CodeGenerator:
	
	template <class GenT>
	Code& TPL_CALL operator,(const CodeGenerator<GenT>& gen_) {
		gen_.generate(*this);
		return *this;
	}

	template <class GenT>
	Code& TPL_CALL operator<<(const CodeGenerator<GenT>& gen_) {
		gen_.generate(*this);
		return *this;
	}

public:
	// Execute:
	
	void TPL_CALL exec(size_t ipFrom, size_t ipTo, stack_type& stk) const
	{
		execute_context context(ipFrom);
		while (context.position() != ipTo) {
			const instruction_type& instr_ = context.next(*this);
			instr_(stk, context);
		}
	}

	template <class AllocT2>
	void TPL_CALL exec(AllocT2& alloc, size_t ipFrom, size_t ipTo, stack_type& stk) const
	{
		execute_context context(alloc, ipFrom);
		while (context.position() != ipTo) {
			const instruction_type& instr_ = context.next(*this);
			instr_(stk, context);
		}
	}
};

// =========================================================================
// Imposible:

template <class StackT, class ExecuteContextT, class AnyT>
void TPL_CALL operator,(const Instruction<StackT, ExecuteContextT>& a, const AnyT& b);

template <class GenT, class AnyT>
void TPL_CALL operator,(const CodeGenerator<GenT>& a, const AnyT& b);

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_CODE_H */

