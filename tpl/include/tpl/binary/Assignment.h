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
// Module: tpl/binary/Assignment.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Assignment.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_ASSIGNMENT_H
#define TPL_BINARY_ASSIGNMENT_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_BINARY_MATCHRESULT_H
#include "MatchResult.h"
#endif

#ifndef TPL_BINARY_TERMINAL_H
#include "Terminal.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class AssigCType

template <class TypeT>
class AssigCType
{
public:
	template <class TypeT2, class Iterator>
	static TypeT TPL_CALL get(Iterator first, Iterator last, const void* pExtra)
	{
		typedef ArchiveIteratorTraits<Iterator> Tr_;
		typedef typename Tr_::value_type value_type;
		TPL_REQUIRE(sizeof(value_type) == 1, CharRequire_);
		TPL_REQUIRE_CLASS(TypeT, TypeT2, EqClass_);
		return _get_value<TypeT>(first);
	}
};

template <class TypeT>
struct AssigTraits<CType<TypeT> > {
	typedef AssigCType<TypeT> assig_type;
};

// =========================================================================
// class AssigCEq

template <class TypeT>
class AssigCEq
{
public:
	template <class TypeT2, class Iterator>
	static TypeT TPL_CALL get(Iterator first, Iterator last, const CEq<TypeT>* rule_)
	{
		return rule_->m_val;
	}
};

template <class TypeT>
struct AssigTraits<CEq<TypeT> > {
	typedef AssigCEq<TypeT> assig_type;
};

// =========================================================================
// class AssigCArr

template <class TypeT>
class AssigCArr
{
public:
	template <class ArrT, class Iterator>
	static ArrT TPL_CALL get(Iterator first, Iterator last, const CArr<TypeT>* arr)
	{
		Arr<TypeT>& result
			typedef ArchiveIteratorTraits<Iterator> Tr_;
		typedef typename Tr_::value_type value_type;
		TPL_REQUIRE(sizeof(value_type) == 1, CharRequire_);
		TPL_REQUIRE_CLASS(Arr<TypeT>, ArrT, EqClass_);
		return ArrT(first, arr->size());
	}
};

template <class TypeT>
struct AssigTraits<CArr<TypeT> > {
	typedef AssigCArr<TypeT> assig_type;
};


// =========================================================================
// function c_type, c_array

template <class TypeT, class VarT>
__forceinline
Rule<Act<CArr<TypeT>, Assign<VarT> > >
TPL_CALL c_array(VarT& val, const size_t dim_) {
	return c_array<TypeT>(dim_) / assign(val);
}

template <class TypeT, class VarT, class DimT>
__forceinline
Rule<Act<CArr<TypeT, SizeTDim<const DimT&> >, Assign<VarT> > >
TPL_CALL c_array(VarT& val, const Var<DimT>& dim_) {
	return c_array<TypeT>(dim_) / assign(val);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_ASSIGNMENT_H */
