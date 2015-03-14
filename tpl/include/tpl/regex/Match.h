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
// Module: tpl/regex/Match.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Match.h 783 2008-07-01 10:38:02Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_MATCH_H
#define TPL_REGEX_MATCH_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

#ifndef TPL_REGEX_UCOMPOSITION_H
#include "UComposition.h"
#endif

#ifndef TPL_REGEX_OPERATOR_H
#include "Operator.h"
#endif

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

#ifndef TPL_REGEX_ASSIGN_H
#include "Assign.h"
#endif

#ifndef TPL_REGEX_APPEND_H
#include "Append.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

#ifndef TPL_REGEX_FIND_H
#include "Find.h"
#endif

#ifndef TPL_REGEX_CONDITION_H
#include "Condition.h"
#endif

#ifndef TPL_REGEX_CASE_H
#include "Case.h"
#endif

#ifndef TPL_REGEX_TRANSFORMATION_H
#include "Transformation.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_OPERATOR_H
#include "grammar/Operator.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_ACTION_H
#include "grammar/Action.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONDITION_H
#include "grammar/Condition.h"
#endif

#ifndef TPL_REGEX_GRAMMAR_CONCRETION_H
#include "grammar/Concretion.h"
#endif

#ifndef STDEXT_ARCHIVE_MEM_H
#include "../../../../stdext/include/stdext/archive/Mem.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// ArchiveTraits

template <class ContainerT>
struct ArchiveTraits_ {
	typedef typename ContainerT::const_iterator iterator;
	typedef std::MemReadArchive<iterator> type;
};

template <class CharT>
struct ArchiveTraits_<const CharT*> {
	typedef const CharT* iterator;
	typedef std::MemReadArchive<iterator> type;
};

template <class ContainerT>
struct ArchiveTraits
{
	typedef ArrayTypeTraits<ContainerT> Tr_;
	typedef ArchiveTraits_<typename Tr_::const_type> Tr2_;
	typedef typename Tr2_::type type;
};

// -------------------------------------------------------------------------
// class Context0

template <class Iterator>
class Context0
{
public:
	template <int category>
	class trans_type
	{
	private:
		Iterator vPos;

	public:
		template <class SourceT>
		trans_type(const SourceT& ar, const Context0&) {
			vPos = ar.position();
		}

		template <class SourceT>
		void TPL_CALL rollback(SourceT& ar) {
			ar.seek(vPos);
		}
	};
};

// -------------------------------------------------------------------------
// match, match_all

template <class ContainerT, class RegExT, class ContextT> inline 
bool TPL_CALL match(const ContainerT& src_, const Rule<RegExT>& rule_, ContextT& context)
{
	typedef typename ArchiveTraits<ContainerT>::type SourceT;
	SourceT source(src_);
	return rule_.match(source, context);
}

template <class ContainerT, class RegExT, class ContextT> inline 
bool TPL_CALL match_all(const ContainerT& src_, const Rule<RegExT>& rule_, ContextT& context)
{
	typedef typename ArchiveTraits<ContainerT>::type SourceT;
	SourceT source(src_);
	return rule_.match(source, context) && (source.get() == SourceT::endch);
}

// -------------------------------------------------------------------------
// operator>>

template <class ContainerT, class RegExT> inline 
bool TPL_CALL operator>>(const ContainerT& src_, const Rule<RegExT>& rule_)
{
	typedef typename ArchiveTraits<ContainerT>::type SourceT;
	typedef Context0<typename SourceT::iterator> ContextT;
	
	SourceT source(src_);
	ContextT context;
	return rule_.match(source, context);
}

// -------------------------------------------------------------------------
// operator==, operator!=

template <class ContainerT, class RegExT> inline 
bool TPL_CALL operator==(const ContainerT& src_, const Rule<RegExT>& rule_)
{
	typedef typename ArchiveTraits<ContainerT>::type SourceT;
	typedef Context0<typename SourceT::iterator> ContextT;
	
	SourceT source(src_);
	ContextT context;
	return rule_.match(source, context) && (source.get() == SourceT::endch);
}

template <class ContainerT, class RegExT> inline 
bool TPL_CALL operator!=(const ContainerT& src_, const Rule<RegExT>& rule_) {
	return !(src_ == rule_);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_MATCH_H */

