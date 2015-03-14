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
// Module: tpl/emu/Instruction.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Instruction.h 588 2008-05-28 07:22:04Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_EMU_INSTRUCTION_H
#define TPL_EMU_INSTRUCTION_H

#ifndef TPL_EMU_FUNCTION_H
#include "Function.h"
#endif

// -------------------------------------------------------------------------

#if defined(TPL_EMU_DEBUG) || defined(_DEBUG)

#if !defined(_IOSTREAM_) && !defined(_GLIBCXX_IOSTREAM) && !defined(_IOSTREAM)
#include <iostream>
#endif

#define TPL_EMU_INSTR_DEBUG(msg)				\
	do {										\
		if (ExecuteContextT::debug)				\
			std::cout << msg << std::endl;		\
	} while (0)

#define TPL_EMU_INSTR_OP_NAME(Op)				\
	NS_TPL_EMU::crackOpName(typeid(Op).name())

#else

#define TPL_EMU_INSTR_DEBUG(msg)
#define TPL_EMU_INSTR_OP_NAME(Op)	"op"

#endif // !defined(TPL_EMU_DEBUG)

// =========================================================================

NS_TPL_EMU_BEGIN

// crackOpName

#define TPL_EMU_INSTR_OP_(op, name)					\
	if (strstr(name_, #name) != NULL)				\
		return #op;

inline const char* TPL_CALL crackOpName(const char* name_)
{
	TPL_EMU_INSTR_OP_(assign, Assign)
	TPL_EMU_INSTR_OP_(add, plus)
	TPL_EMU_INSTR_OP_(sub, minus)
	TPL_EMU_INSTR_OP_(mul, multiplies)
	TPL_EMU_INSTR_OP_(div, divides)
	TPL_EMU_INSTR_OP_(mod, modulus)
	
	TPL_EMU_INSTR_OP_(neg, negate)
	
	TPL_EMU_INSTR_OP_(ne, not_equal_to)
	TPL_EMU_INSTR_OP_(eq, equal_to)
	TPL_EMU_INSTR_OP_(ge, greater_equal)
	TPL_EMU_INSTR_OP_(gt, greater)
	TPL_EMU_INSTR_OP_(le, less_equal)
	TPL_EMU_INSTR_OP_(lt, less)
	
	return name_;
}

// =========================================================================
// function ref_to_variant/variant_to_ref

template <class ValT>
struct VariantTraits
{
public:
	static ValT TPL_CALL cast(size_t size_) {
		ValT val_;
		*(size_t*)&val_ = size_;
		return val_;
	}
	static size_t TPL_CALL cast(const ValT& val_) {
		return *(size_t*)&val_;
	}
};

template <>
struct VariantTraits<double> {
	typedef double ValT;
	static ValT TPL_CALL cast(size_t size_) {
		return size_;
	}
	static size_t TPL_CALL cast(const ValT& val_) {
		return (size_t)val_;
	}
};

template <>
struct VariantTraits<size_t> {
	static size_t TPL_CALL cast(size_t size_) {
		return size_;
	}
};

template <class ValT>
__forceinline ValT TPL_CALL ref_to_variant(ValT& ref_) {
	return VariantTraits<ValT>::cast((size_t)&ref_);
}

template <class ValT>
__forceinline ValT& TPL_CALL variant_to_ref(const ValT& val_) {
	return *(ValT*)VariantTraits<ValT>::cast(val_);
}

// =========================================================================
// Instruction

union Operand
{
	size_t val;
	ptrdiff_t ival;
	const void* ptr;
};

template <class StackT, class ExecuteContextT>
class Instruction
{
private:
	typedef void (*Op)(Operand para, StackT& stk, ExecuteContextT& context);

public:
	const Op op;
	Operand para;

public:
	Instruction(Op op_) : op(op_) {}
	Instruction(Op op_, size_t val_) : op(op_)	 	{ para.val = val_; }
	Instruction(Op op_, ptrdiff_t val_) : op(op_) 	{ para.ival = val_; }
	Instruction(Op op_, const void* ptr_) : op(op_) { para.ptr = ptr_; }

	__forceinline void TPL_CALL operator()(StackT& stk, ExecuteContextT& context) const {
		op(para, stk, context);
	}
};

// =========================================================================
// class Nop

// Usage: nop

template <class StackT, class ExecuteContextT>
class Nop
{
public:
	static void op(Operand, StackT&, ExecuteContextT&) {
		TPL_EMU_INSTR_DEBUG("nop");
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// =========================================================================
// class Push/Pop/Repush(PopAndPush)

// Usage: push <val>

template <class StackT, class ExecuteContextT>
class Push
{
private:
	typedef typename StackT::value_type ValT;

public:
	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		if (sizeof(ValT) > sizeof(void*)) {
			TPL_EMU_INSTR_DEBUG("push " << *(const ValT*)para.ptr);
			stk.push_back(*(const ValT*)para.ptr);
		}
		else {
			TPL_EMU_INSTR_DEBUG("push " << *(const ValT*)&para.ptr);
			stk.push_back(*(const ValT*)&para.ptr);
		}
	}

	template <class AllocT>
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(AllocT& alloc, const ValT& val) {
		if (sizeof(ValT) > sizeof(void*)) {
			return Instruction<StackT, ExecuteContextT>(op, TPL_NEW(alloc, ValT)(val));
		}
		else {
			void* p;
			return Instruction<StackT, ExecuteContextT>(op, *(void**)new(&p) ValT(val));
		}
	}
};

// Usage: pop <count>

template <class StackT, class ExecuteContextT>
class Pop
{
public:
	static void op1(Operand para, StackT& stk, ExecuteContextT&) {
		TPL_EMU_INSTR_DEBUG("pop");
		stk.pop_back();
	}

	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		TPL_EMU_INSTR_DEBUG("pop " << para.val);
		stk.resize(stk.size() - para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op1);
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n);
	}
};

template <class StackT, class ExecuteContextT>
class Repush // PopAndPush
{
private:
	typedef typename StackT::value_type ValT;

public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		if (sizeof(ValT) > sizeof(void*)) {
			TPL_EMU_INSTR_DEBUG("repush " << *(const ValT*)para.ptr);
			stk.back() = *(const ValT*)para.ptr;
		}
		else {
			TPL_EMU_INSTR_DEBUG("repush " << *(const ValT*)&para.ptr);
			stk.back() = *(const ValT*)&para.ptr;
		}
	}
	
	template <class AllocT>
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(AllocT& alloc, const ValT& val) {
		if (sizeof(ValT) > sizeof(void*)) {
			return Instruction<StackT, ExecuteContextT>(op, TPL_NEW(alloc, ValT)(val));
		}
		else {
			void* p;
			return Instruction<StackT, ExecuteContextT>(op, *(void**)new(&p) ValT(val));
		}
	}
};

// =========================================================================
// class Add/Sub

// Usage: add <value>
// Example: add 2	; x + 2
// Example: sub 2	; x - 2

template <class StackT, class ExecuteContextT>
class Add
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		stk.back() += para.ival;
		TPL_EMU_INSTR_DEBUG("add " << para.ival << "\t\t; return: " << stk.back());
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t val) {
		return Instruction<StackT, ExecuteContextT>(op, val);
	}
};

// =========================================================================
// class Inc/Dec

// Usage: inc <value>
// Example: inc 2	; x += 2
// Example: dec 2	; x -= 2

template <class StackT, class ExecuteContextT>
class Inc
{
private:
	typedef typename StackT::value_type ValT;

public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		const ValT val = (variant_to_ref(stk.back()) += para.ival);
		stk.back() = val;
		TPL_EMU_INSTR_DEBUG("inc " << para.ival << "\t\t; return: " << val);
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(ptrdiff_t val) {
		return Instruction<StackT, ExecuteContextT>(op, val);
	}
};

// =========================================================================
// class Arity

// Usage: arity <count>

template <class StackT, class ExecuteContextT>
class Arity
{
private:
	typedef VariantTraits<typename StackT::value_type> Tr_;
	
public:
	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		TPL_EMU_INSTR_DEBUG("arity " << para.val);
		stk.push_back(Tr_::cast(para.val));
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n);
	}
};

// =========================================================================
// class CallerFrame

// Function Stack Frame
//	 arg1
//	 arg2
//	 ...
//	 argn
//	 [arity]
//	 callerframe
//	 ------------ <frame>
//	 [local_var1]
//	 [local_var2]
//	 ...
//	 [local_varn]
//	 return_val

class CallerFrame
{
public:
	enum { SIZE = 2 };
	
	enum { ARITY = -3 };
	enum { RETURN_IP = -2 };
	enum { BP = -1 };
	
public:
	template <class StackT, class ExecuteContextT>
	static void TPL_CALL call(StackT& stk, ExecuteContextT& context, size_t offset) {
		typedef VariantTraits<typename StackT::value_type> Tr_;
		stk.push_back(Tr_::cast(context.position()));
		stk.push_back(Tr_::cast(context.frame()));
		context.frame(stk.size());
		context.position(offset);
	}

	template <class StackT, class ExecuteContextT>
	static void TPL_CALL ret(StackT& stk, ExecuteContextT& context, size_t n) {
		typedef VariantTraits<typename StackT::value_type> Tr_;
		const typename StackT::value_type val = stk.top();
		size_t frame_ = context.frame();
		context.frame(Tr_::cast(stk[frame_ + BP]));
		context.position(Tr_::cast(stk[frame_ + RETURN_IP]));
		stk.resize(frame_ - n);
		stk.push_back(val);
	}

public:
	template <class StackT, class ExecuteContextT>
	size_t TPL_CALL arity(StackT& stk, ExecuteContextT& context) const {
		typedef VariantTraits<typename StackT::value_type> Tr_;
		const size_t arity_idx_ = context.frame() + ARITY;
		return Tr_::cast(stk[arity_idx_]);
	}

	template <class StackT, class ExecuteContextT>
	typename StackT::value_type TPL_CALL vargs(StackT& stk, ExecuteContextT& context) const {
		typedef VariantTraits<typename StackT::value_type> Tr_;
		const size_t arity_idx_ = context.frame() + ARITY;
		const size_t arity_ = Tr_::cast(stk[arity_idx_]);
		return typename StackT::value_type::array(stk.begin() + (arity_idx_ - arity_), arity_);
	}
};

// =========================================================================
// class Call

// Usage: call <position>
//
// Example1:
//	push 3.0
//	push 2.0
//	call offset pow
// Meanings:
//	pow(3.0, 2.0)
//
// Example2:
//	push 3.0
//	push 2.0
//	push 1.0
//	arity 3
//	call offset max
// Meanings:
//	max(3.0, 2.0, 1.0)

template <class StackT, class ExecuteContextT>
class Call
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		TPL_EMU_INSTR_DEBUG("call " << para.val);
		CallerFrame::call(stk, context, para.val);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t offset) {
		return Instruction<StackT, ExecuteContextT>(op, offset);
	}
};

// =========================================================================
// class Ret/RetN

// Usage: ret <n>
//
// Example1:
//	 proc my_pow
//		push_arg -2		; push arg1
//		push_arg -1		; push arg2
//		pow				; call pow instruction 
//		ret 2			; return
//	end proc
//
// Example2:
//	 proc my_max
//		push_vargs		; push arg[]
//		max_n			; call max_n instruction
//		ret				; return
//	 end proc

template <class StackT, class ExecuteContextT>
class RetN
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		CallerFrame::ret(stk, context, para.val);
		TPL_EMU_INSTR_DEBUG("ret " << para.val - CallerFrame::SIZE << "\t\t; return: " << stk.back());
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(size_t n) {
		return Instruction<StackT, ExecuteContextT>(op, n + CallerFrame::SIZE);
	}
};

template <class StackT, class ExecuteContextT>
class Ret
{
public:
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		const size_t n = CallerFrame::arity(stk, context) + (CallerFrame::SIZE + 1);
		CallerFrame::ret(stk, context, n);
		TPL_EMU_INSTR_DEBUG("ret\t\t; return: " << stk.back());
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// =========================================================================
// class OpInstr/ExtOpInstr

// Usage:
//		add		; OpInstr<std::plus, StackT, ContextT>::instr()
//		sub		; OpInstr<std::minus, StackT, ContextT>::instr()
//		mul		; OpInstr<std::multiplies, StackT, ContextT>::instr()
//		div		; OpInstr<std::divides, StackT, ContextT>::instr()

template <template <class Type> class Op_, class StackT, class ExecuteContextT>
class OpInstr
{
private:
	typedef typename StackT::value_type Ty;
	enum { n_arity = ArityTraits<Op_, Ty>::value };

public:
	static void op(Operand, StackT& stk, ExecuteContextT&) {
		Function<Op_<Ty>, n_arity> fn_;
		fn_(stk);
		TPL_EMU_INSTR_DEBUG(TPL_EMU_INSTR_OP_NAME(Op_<Ty>) << "\t\t; return: " << stk.back());
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

template <template <class Type> class Op_, class StackT, class ExecuteContextT>
class ExtOpInstr
{
private:
	typedef typename StackT::value_type Ty;
	enum { n_arity = ArityTraits<Op_, Ty>::value };

public:
	static void op(Operand, StackT& stk, ExecuteContextT& context) {
		Function<Op_<Ty>, n_arity> fn_;
		fn_(context.get_alloc(), stk);
		TPL_EMU_INSTR_DEBUG(TPL_EMU_INSTR_OP_NAME(Op_<Ty>) << "\t\t; return: " << stk.back());
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// =========================================================================
// class FnInstr/ExtFnInstr

// Usage:
//		pow		; FnInstr<2, StackT, ContextT>::instr(pow)
//		sin		; FnInstr<1, StackT, ContextT>::instr(sin)
//		cos		; FnInstr<1, StackT, ContextT>::instr(cos)

template <int nArity, class StackT, class ExecuteContextT>
class FnInstr
{
private:
	typedef typename StackT::value_type Ty;
	
public:
	typedef typename OpTraits<nArity, Ty>::op_type op_type;

	static void op(Operand para, StackT& stk, ExecuteContextT&) {
		Function<op_type, nArity> fn_((op_type)para.ptr);
		fn_(stk);
		TPL_EMU_INSTR_DEBUG(
			"func " << para.ptr << "\t; argc: " << nArity <<
			", return: " << stk.back());
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(op_type fn) {
		return Instruction<StackT, ExecuteContextT>(op, (const void*)fn);
	}
};

template <int nArity, class StackT, class ExecuteContextT>
class ExtFnInstr
{
private:
	typedef typename StackT::value_type Ty;
	typedef typename ExecuteContextT::alloc_type AllocT;
	
public:
	typedef typename ExtOpTraits<nArity, Ty, AllocT>::op_type op_type;

	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		Function<op_type, nArity> fn_((op_type)para.ptr);
		fn_(context.get_alloc(), stk);
		TPL_EMU_INSTR_DEBUG(
			"func " << para.ptr << "\t; argc: " << nArity <<
			", return: " << stk.back());
	}

	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(op_type fn) {
		return Instruction<StackT, ExecuteContextT>(op, (const void*)fn);
	}
};

// =========================================================================
// class VargsFnInstr/ExtVargsFnInstr

template <class Iterator>
struct Array_ {
	Iterator data;
	size_t n;
};

template <class Iterator>
inline Array_<Iterator> TPL_CALL array_(Iterator data, size_t n) {
	Array_<Iterator> a = { data, n };
	return a;
}

template <class OStreamT, class Iterator>
OStreamT& operator<<(OStreamT& os, Array_<Iterator> array_) {
	os << "[ ";
	for (size_t i = 0; i < array_.n; ++i)
		os << *array_.data++ << ' ';
	os << "]";
	return os;
}

template <class IntT, class StackT, class ExecuteContextT>
class VargsFnInstr
{
private:
	typedef typename StackT::value_type Ty;
	typedef typename VargsOpTraits<Ty, IntT>::op_type Op;

public:
	typedef Op op_type;

	static void op(Operand para, StackT& stk, ExecuteContextT&)
	{
		typedef VariantTraits<Ty> Tr_;
		const IntT count = (IntT)Tr_::cast(stk.back());
		stk.pop_back();
		
		Ty* args = (Ty*)alloca(sizeof(Ty) * count);	
		for (IntT i = count; i--;) {		
			args[i] = stk.back();
			stk.pop_back();
		}
		
		stk.push_back(
			((op_type)para.ptr)(static_cast<const Ty*>(args), count)
			);
		TPL_EMU_INSTR_DEBUG(
			"func " << para.ptr << "\t; vargs: " 
				<< array_(args, count) << ", return: " << stk.back());
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(op_type fn) {
		return Instruction<StackT, ExecuteContextT>(op, (const void*)fn);
	}
};

template <class IntT, class StackT, class ExecuteContextT>
class ExtVargsFnInstr
{
private:
	typedef typename StackT::value_type Ty;
	typedef typename ExecuteContextT::alloc_type AllocT;
	typedef typename ExtVargsOpTraits<Ty, IntT, AllocT>::op_type Op;

public:
	typedef Op op_type;

	static void op(Operand para, StackT& stk, ExecuteContextT& context)
	{
		typedef VariantTraits<Ty> Tr_;
		const IntT count = Tr_::cast(stk.back());
		stk.pop_back();
		
		Ty* args = (Ty*)alloca(sizeof(Ty) * count);	
		for (IntT i = count; i--;) {		
			args[i] = stk.back();
			stk.pop_back();
		}
		
		TPL_EMU_INSTR_DEBUG("func " << para.ptr << "\t; vargs: " << array_(args, count));
		stk.push_back(
			((op_type)para.ptr)(context.get_alloc(), static_cast<const Ty*>(args), count)
			);
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr(op_type fn) {
		return Instruction<StackT, ExecuteContextT>(op, (const void*)fn);
	}
};

// =========================================================================
// class PushVArgs

// Usage: push_vargs

template <class StackT, class ExecuteContextT>
class PushVArgs
{
public:	
	static void op(Operand para, StackT& stk, ExecuteContextT& context) {
		stk.push_back(CallerFrame::vargs(stk, context));
	}
	
	static Instruction<StackT, ExecuteContextT> TPL_CALL instr() {
		return Instruction<StackT, ExecuteContextT>(op);
	}
};

// =========================================================================
// $Log: $

NS_TPL_EMU_END

#endif /* TPL_EMU_INSTRUCTION_H */

