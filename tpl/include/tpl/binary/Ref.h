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
// Module: tpl/binary/Ref.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Ref.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_REF_H
#define TPL_BINARY_REF_H

#ifndef TPL_REGEX_REF_H
#include "../regex/Ref.h"
#endif

#ifndef TPL_BINARY_TERMINAL_H
#include "Terminal.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// ReferenceTratis

// Usage: Var<TypeT> var;
// Examples:
//	 Var<UINT32> a;
//	 Var<INT16> b;

template <>
struct ReferenceTratis<UINT8> {
	typedef CEq<UINT8> reference_type;
};

template <>
struct ReferenceTratis<UINT16> {
	typedef CEq<UINT16> reference_type;
};

template <>
struct ReferenceTratis<INT16> {
	typedef CEq<INT16> reference_type;
};

template <>
struct ReferenceTratis<UINT32> {
	typedef CEq<UINT32> reference_type;
};

template <>
struct ReferenceTratis<INT32> {
	typedef CEq<INT32> reference_type;
};

// =========================================================================
// class Struct

// Usage: Struct<TypeT> var;
// Examples:
//	 Struct<MyStruct> a;

template <class Type>
class Struct : public Rule<Deref<CEq<Type> > >
{
public:
	Type val;

public:
	Struct() : Rule<Deref<CEq<Type> > >(val) {}
};

template <class TypeT>
__forceinline
Rule<CEq<TypeT> > TPL_CALL c_eq(const Var<TypeT>& var_) {
	return Rule<CEq<TypeT> >(var_.val);
}

template <class TypeT>
__forceinline
Rule<CEq<TypeT> > TPL_CALL c_eq(const Struct<TypeT>& var_) {
	return Rule<CEq<TypeT> >(var_.val);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_REF_H */
