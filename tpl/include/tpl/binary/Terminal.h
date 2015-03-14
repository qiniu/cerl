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
// Module: tpl/binary/Terminal.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Terminal.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_TERMINAL_H
#define TPL_BINARY_TERMINAL_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "../regex/Ref.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class CType

// Usage: c_type<TypeT>()
// Usage: c_struct<TypeT>()
// Examples:
//	 c_type<UINT32>()
//	 c_struct<MyStruct>()

template <class TypeT>
class CType
{
public:
	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef CType<TypeT> assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		TPL_REQUIRE(sizeof(typename SourceT::char_type) == 1, CharRequire_);
		return ar.try_skip(sizeof(TypeT));
	}
};

template <class TypeT>
__forceinline
Rule<CType<TypeT> > TPL_CALL c_type() {
	return Rule<CType<TypeT> >();
}

template <class TypeT>
__forceinline
Rule<CType<TypeT> > TPL_CALL c_struct() {
	return Rule<CType<TypeT> >();
}

// =========================================================================
// class CEq

// Usage: c_type<TypeT>(const_val)
// Usage: c_struct<TypeT>(const_val)
// Usage: c_eq(var)
// Examples:
//	 c_type<UINT32>(23243)
//	 c_struct<MyStruct>(a_const_struct_val)
//	 c_eq(a_var)

template <class TypeT>
class AssigCEq;

template <class TypeT>
class CEq
{
private:
	TypeT m_val;

	friend class AssigCEq<TypeT>;

public:
	template <class T1>
	CEq(const T1& val) : m_val(val) {}

public:
	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef CEq<TypeT> assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		TPL_REQUIRE(sizeof(typename SourceT::char_type) == 1, CharRequire_);
		TypeT val;
		if (!ar.try_get((char*)&val, sizeof(TypeT)))
			return false;
#if defined(TPL_BYTESWAP)
#error "todo"
#endif
		return val == m_val;
	}
};

template <class TypeT, class ValueT>
__forceinline
Rule<CEq<TypeT> > TPL_CALL c_type(const ValueT& val) {
	return Rule<CEq<TypeT> >(val);
}

template <class TypeT, class ValueT>
__forceinline
Rule<CEq<TypeT> > TPL_CALL c_struct(const ValueT& val) {
	return Rule<CEq<TypeT> >(val);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_TERMINAL_H */
