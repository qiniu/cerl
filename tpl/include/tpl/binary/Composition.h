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
// Module: tpl/binary/Composition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Composition.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_COMPOSITION_H
#define TPL_BINARY_COMPOSITION_H

#ifndef TPL_BINARY_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_BINARY_REF_H
#include "Ref.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function c_array

// Usage: c_array<Type>(n)
// Examples:
//	 c_array<UINT16>(10)
//	 c_array<UINT16>(ref(count))

template <class DimT>
class SizeTDim
{
private:
	DimT m_count;

public:
	SizeTDim(DimT count_) : m_count(count_) {}

	size_t TPL_CALL calc_size() const {
		return m_count;
	}

	size_t TPL_CALL size() const {
		return m_count;
	}
};

template <class TypeT, class DimT = SizeTDim<size_t> >
class CArr : public DimT
{
public:
	template <class T1>
	CArr(const T1& count_) : DimT(count_) {
	};

	enum { character = 0 };

	typedef ExplicitConvertible convertible_type;
	typedef CArr<TypeT> assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		TPL_REQUIRE(sizeof(typename SourceT::char_type) == 1, CharRequire_);
		return ar.try_skip(sizeof(TypeT) * DimT::calc_size());
	}
};

template <class TypeT>
__forceinline
Rule<CArr<TypeT> > TPL_CALL c_array(const size_t dim_) {
	return Rule<CArr<TypeT> >(dim_);
}

template <class TypeT, class DimT>
__forceinline
Rule<CArr<TypeT, SizeTDim<const DimT&> > > TPL_CALL c_array(const Var<DimT>& dim_) {
	return Rule<CArr<TypeT, SizeTDim<const DimT&> > >(dim_.val);
}

// =========================================================================
// class CArrEq

// Usage: c_array<TypeT>(const_array_var, array_size)

class CArrEq // todo
{
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_COMPOSITION_H */
