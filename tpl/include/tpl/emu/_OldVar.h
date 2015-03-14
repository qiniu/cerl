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
// Module: tpl/emu/_OldVar.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: _OldVar.h 588 2008-05-28 07:22:04Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU__OLDVAR_H
#define TPL_EMU__OLDVAR_H

NS_TPL_EMU_BEGIN

// =========================================================================
// class PushArg/PushLocal/PushOperand

// Usage: push_arg <delta>	; here <delta> can be -n ~ -1
//	 arg1 = push_arg -n
//	 arg2 = push_arg -(n-1)
//	 ...
//	 argn = push_arg -1

template <class StackT, class ExecuteContextT>
class PushArg
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"push_arg " << para.ival + CallerFrame::SIZE <<
			"\t; push: " << stk[context.frame() + para.ival] );
		stk.push_back(stk[context.frame() + para.ival]);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta - CallerFrame::SIZE);
	}
};

// Usage: push_local <index> ; here <index> can be 0 ~ n-1
//	 local_var1 = push_local 0
//	 local_var2 = push_local 1
//	 ...
//	 local_varn = push_local n-1

template <class StackT, class ExecuteContextT>
class PushLocal
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"push_local " << para.val <<
			"\t; push: " << stk[context.frame() + para.val] );
		stk.push_back(stk[context.frame() + para.val]);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// Usage: push_operand <delta> ; here <delta> can be -n ~ -1
//	 oprand1 = push_operand -n
//	 oprand2 = push_operand -(n-1)
//	 ...
//	 oprandn = push_operand -1

template <class StackT, class ExecuteContextT>
class PushOperand
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"push_operand " << para.ival <<
			"\t; push: " << stk[context.size() + para.ival] );
		stk.push_back(stk[context.size() + para.ival]);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class LeaArg/LeaLocal

// Usage: lea_arg <delta>	; here <delta> can be -n ~ -1
//	 arg1 = lea_arg -n
//	 arg2 = lea_arg -(n-1)
//	 ...
//	 argn = lea_arg -1

template <class StackT, class ExecuteContextT>
class LeaArg
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"lea_arg " << para.ival + CallerFrame::SIZE <<
			"\t; value: " << stk[context.frame() + para.ival] );
		stk.push_back(
			ref_to_variant(stk[context.frame() + para.ival])
			);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta - CallerFrame::SIZE);
	}
};

// Usage: lea_local <index> ; here <index> can be 0 ~ n-1
//	 local_var1 = lea_local 0
//	 local_var2 = lea_local 1
//	 ...
//	 local_varn = lea_local n-1

template <class StackT, class ExecuteContextT>
class LeaLocal
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"lea_local " << para.val <<
			"\t; value: " << stk[context.frame() + para.val] );
		stk.push_back(
			ref_to_variant(stk[context.frame() + para.val])
			);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// class AssignArg/AssignLocal

// Usage: assgin_arg <delta>	; here <delta> can be -n ~ -1

template <class StackT, class ExecuteContextT>
class AssignArg
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"assgin_arg " << para.ival + CallerFrame::SIZE <<
			"\t; value: " << stk[context.frame() + para.ival] << " => " << stk.back() );
		stk[context.frame() + para.ival] = stk.back();
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta - CallerFrame::SIZE);
	}
};

// Usage: assign_local <index> ; here <index> can be 0 ~ n-1

template <class StackT, class ExecuteContextT>
class AssignLocal
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG(
			"assgin_local " << para.val <<
			"\t; value: " << stk[context.frame() + para.val] << " => " << stk.back() );
		stk[context.frame() + para.val] = stk.back();
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t delta) {
		return Instruction<StackT, ExecuteContextT>(op, delta);
	}
};

// =========================================================================
// old instructions

#define TPL_EMU_OLDVAR_INSTRS_() \
\
	static InstructionT TPL_CALL local(size_t n) { \
		return Local<StackT, ContextT>::instr(n); \
	} \
\
	static InstructionT TPL_CALL push_arg(ptrdiff_t delta) { \
		return PushArg<StackT, ContextT>::instr(delta); \
	} \
\
	static InstructionT TPL_CALL push_local(size_t delta) { \
		return PushLocal<StackT, ContextT>::instr(delta); \
	} \
\
	static InstructionT TPL_CALL push_operand(ptrdiff_t delta) { \
		return PushLocal<StackT, ContextT>::instr(delta); \
	}\
\
	static InstructionT TPL_CALL lea_arg(ptrdiff_t delta) { \
		return LeaArg<StackT, ContextT>::instr(delta); \
	} \
\
	static InstructionT TPL_CALL lea_local(size_t delta) { \
		return LeaLocal<StackT, ContextT>::instr(delta); \
	} \
\
	static InstructionT TPL_CALL assign_arg(ptrdiff_t delta) { \
		return AssignArg<StackT, ContextT>::instr(delta); \
	} \
\
	static InstructionT TPL_CALL assign_local(size_t delta) { \
		return AssignLocal<StackT, ContextT>::instr(delta); \
	}

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU__OLDVAR_H */

