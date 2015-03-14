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
// Module: tpl/emu/Label.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Label.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_LABEL_H
#define TPL_EMU_LABEL_H

#ifndef STDEXT_ARRAY_H
#include "../../../../stdext/include/stdext/Array.h"
#endif

#ifndef TPL_EMU_VAR_H
#include "Var.h"
#endif

NS_TPL_EMU_BEGIN

// =========================================================================
// class Jmp

// Usage: jmp <position>

template <class StackT, class ExecuteContextT>
class Jmp
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG("jmp " << para.val);
		context.position(para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t offset) {
		return Instruction<StackT, ExecuteContextT>(op, offset);
	}
};

// =========================================================================
// class JmpIfFalse

// Usage: jz <position>

template <class StackT, class ExecuteContextT>
class JmpIfFalse
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		const typename StackT::value_type val = stk.back();
		stk.pop_back();
		TPL_EMU_INSTR_DEBUG("jz " << para.val << "\t\t; condition: " << val);
		if (!val)
			context.position(para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t offset) {
		return Instruction<StackT, ExecuteContextT>(op, offset);
	}
};

// =========================================================================
// class Label

#define TPL_EMU_DYNAMIC_LABEL	((size_t)-1)
#define TPL_EMU_UNDEFINED_LABEL	((size_t)-1)

template <size_t n = TPL_EMU_DYNAMIC_LABEL>
class Label
{
private:
	typedef size_t* Reference;
	typedef std::Array<Reference, n> References;
	
	size_t m_label;
	References m_refs;

public:
	Label()
		: m_label(TPL_EMU_UNDEFINED_LABEL) {
	}
	
	operator size_t() const {
		TPL_ASSERT(defined());
		return m_label;
	}
	
	bool TPL_CALL defined() const {
		return m_label != TPL_EMU_UNDEFINED_LABEL;
	}
	
	template <class CodeT>
	void TPL_CALL define(const CodeT& code) {
		TPL_ASSERT(!defined());
		m_label = code.size();
		for (size_t i = m_refs.size(); i--; )
			*m_refs[i] = m_label;
	}
	
	template <class CodeT>
	void TPL_CALL refer(CodeT& code) {
		if (m_label == TPL_EMU_UNDEFINED_LABEL) {
			m_refs.push_back(&code.back().para.val);
		}
		else {
			code.back().para.val = m_label;
		}
	}
};

template <>
class Label<0>
{
private:
	size_t m_label;

public:
	Label()
		: m_label(TPL_EMU_UNDEFINED_LABEL) {
	}
	
	operator size_t() const {
		TPL_ASSERT(defined());
		return m_label;
	}
	
	bool TPL_CALL defined() const {
		return m_label != TPL_EMU_UNDEFINED_LABEL;
	}
	
	template <class CodeT>
	void TPL_CALL define(const CodeT& code) {
		TPL_ASSERT(!defined());
		m_label = code.size();
	}

	template <class CodeT>
	void TPL_CALL refer(CodeT& code) {
		TPL_ASSERT(defined());
		code.back().para.val = m_label;
	}
};

template <>
class Label<TPL_EMU_DYNAMIC_LABEL>
{
private:
	struct Reference {
		size_t* offset;
		Reference* prev;
	};

	size_t m_label;
	Reference* m_refs;
	
public:
	Label()
		: m_label(TPL_EMU_UNDEFINED_LABEL), m_refs(NULL) {
	}
	
	operator size_t() const {
		TPL_ASSERT(defined());
		return m_label;
	}
	
	bool TPL_CALL defined() const {
		return m_label != TPL_EMU_UNDEFINED_LABEL;
	}
	
	template <class CodeT>
	void TPL_CALL define(const CodeT& code) {
		TPL_ASSERT(!defined());
		m_label = code.size();
		for (Reference* ref_ = m_refs; ref_; ref_ = ref_->prev)
			*ref_->offset = m_label;
	}
	
	template <class CodeT>
	void TPL_CALL refer(CodeT& code) {
		if (m_label == TPL_EMU_UNDEFINED_LABEL) {
			Reference* ref_ = TPL_ALLOC(code.get_alloc(), Reference);
			ref_->offset = &code.back().para.val;
			ref_->prev = m_refs;
			m_refs = ref_;
		}
		else {
			code.back().para.val = m_label;
		}
	}
};

// =========================================================================
// class Label(Define/Refer)

template <class LabelT>
class LabelDefine
{
private:
	LabelT& m_label;

public:	
	LabelDefine(LabelT& label_) : m_label(label_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_label.define(code);
	}
};

template <class LabelT, class InstructionT>
class LabelRefer
{
private:
	LabelT& m_label;

public:
	LabelRefer(LabelT& label_) : m_label(label_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		code.push_back(InstructionT::instr(0));
		m_label.refer(code);
	}
};

// =========================================================================
// class ProcDefine

template <class LabelT>
class ProcDefine
{
private:
	LabelT& m_proc;

public:	
	ProcDefine(LabelT& proc_) : m_proc(proc_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().begin_arglist();
	}
};

class EndArgList
{
public:
	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		code.get_context().end_arglist();
	}
};

// =========================================================================

template <class LabelT>
class Proc1Arg
{
private:
	LabelT& m_proc;
	Variable& m_var;
	
public:
	Proc1Arg(LabelT& proc_, Variable& var_)
		: m_proc(proc_), m_var(var_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().arglist(m_var);
	}
};

template <class LabelT>
class Proc2Arg
{
private:
	LabelT& m_proc;
	Variable& m_var;
	Variable& m_var2;
	
public:
	Proc2Arg(LabelT& proc_, Variable& var_, Variable& var2_)
		: m_proc(proc_), m_var(var_), m_var2(var2_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().arglist(m_var, m_var2);
	}
};

template <class LabelT>
class Proc3Arg
{
private:
	LabelT& m_proc;
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	
public:
	Proc3Arg(LabelT& proc_, Variable& var_, Variable& var2_, Variable& var3_)
		: m_proc(proc_), m_var(var_), m_var2(var2_), m_var3(var3_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().arglist(m_var, m_var2, m_var3);
	}
};

template <class LabelT>
class Proc4Arg
{
private:
	LabelT& m_proc;
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	Variable& m_var4;
	
public:
	Proc4Arg(LabelT& proc_, Variable& var_, Variable& var2_, Variable& var3_, Variable& var4_)
		: m_proc(proc_), m_var(var_), m_var2(var2_), m_var3(var3_), m_var4(var4_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().arglist(m_var, m_var2, m_var3, m_var4);
	}
};

template <class LabelT>
class Proc5Arg
{
private:
	LabelT& m_proc;
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	Variable& m_var4;
	Variable& m_var5;
	
public:
	Proc5Arg(LabelT& proc_, Variable& var_, Variable& var2_, Variable& var3_, Variable& var4_, Variable& var5_)
		: m_proc(proc_), m_var(var_), m_var2(var2_), m_var3(var3_), m_var4(var4_), m_var5(var5_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().arglist(m_var, m_var2, m_var3, m_var4, m_var5);
	}
};

template <class LabelT>
class Proc6Arg
{
private:
	LabelT& m_proc;
	Variable& m_var;
	Variable& m_var2;
	Variable& m_var3;
	Variable& m_var4;
	Variable& m_var5;
	Variable& m_var6;
	
public:
	Proc6Arg(LabelT& proc_, Variable& var_, Variable& var2_, Variable& var3_, Variable& var4_, Variable& var5_, Variable& var6_)
		: m_proc(proc_), m_var(var_), m_var2(var2_), m_var3(var3_), m_var4(var4_), m_var5(var5_), m_var6(var6_) {
	}

	template <class CodeT>
	void TPL_CALL generate(CodeT& code) const {
		m_proc.define(code);
		code.get_context().arglist(m_var, m_var2, m_var3, m_var4, m_var5, m_var6);
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_LABEL_H */

