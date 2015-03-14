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
// Module: tpl/binary/Customization.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Customization.h 625 2008-06-10 14:16:54Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_BINARY_CUSTOMIZATION_H
#define TPL_BINARY_CUSTOMIZATION_H

#ifndef TPL_REGEX_CUSTOMIZATION_H
#include "../regex/Customization.h"
#endif

NS_TPL_BEGIN

template <class Policy = policy::Default, bool bHasDocument = true>
class BinaryImpl : public Customization<Policy, bHasDocument>
{
private:
	typedef Customization<Policy, bHasDocument> Base;

public:
	typedef typename Base::Iterator Iterator;

	template <class TypeT>
	class Array : public tpl::Arr<TypeT, Iterator>
	{
	public:
		Array() {}
		Array(Iterator it_, size_t count_) : tpl::Arr<TypeT, Iterator>(it_, count_) {}
	};

public:
	template <class RegExT>
	static inline bool TPL_CALL match(Iterator pos, size_t cb, const tpl::Rule<RegExT>& rule_)
	{
		typedef ArchiveIteratorTraits<Iterator> Tr_;
		typedef typename Tr_::value_type ValueT;
		TPL_REQUIRE(sizeof(ValueT) == 1, CharRequire_);

		Iterator pos2 = pos;
		std::advance(pos2, cb);
		Source source(pos, pos2);
		Context context;
		return rule_.match(source, context);
	}

	template <class RegExT>
	static inline bool TPL_CALL match(const void* pos, size_t cb, const tpl::Rule<RegExT>& rule_) {
		return match((const char*)pos, cb, rule_);
	}
};

// -------------------------------------------------------------------------
// SimpleBinaryImplementation

typedef BinaryImpl<policy::Default, false> SimpleBinaryImpl;
typedef SimpleBinaryImpl SimpleBinaryImplementation;

// -------------------------------------------------------------------------
// BinaryImplementation

typedef BinaryImpl<> BinaryImplementation;

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_BINARY_CUSTOMIZATION_H */
