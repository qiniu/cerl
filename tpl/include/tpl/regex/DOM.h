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
// Module: tpl/regex/DOM.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: DOM.h 783 2008-07-01 10:38:02Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_DOM_H
#define TPL_REGEX_DOM_H

#ifndef TPL_REGEX_MATCH_H
#include "Match.h"
#endif

#ifndef TPL_REGEX_RESULT_MARK_H
#include "result/Mark.h"
#endif

#ifndef TPL_REGEX_RESULT_DOCUMENT_H
#include "result/Document.h"
#endif

#ifndef TPL_REGEX_RESULT_CONTEXT_H
#include "result/Context.h"
#endif

NS_TPL_BEGIN

typedef char DefaultTagChar;
typedef NS_STDEXT::Range<const char*> DefaultLeaf;

// -------------------------------------------------------------------------
// class DOM

template <
	class LeafT = DefaultLeaf,
	class AllocT = DefaultAllocator,
	class TagCharT = DefaultTagChar
	>
class DOM
{
public:
	// Allocator
	
	typedef AllocT Allocator;

public:
	// TagChar
	
	typedef TagCharT TagChar;

public:
	// Mark, LeafMark, NodeMark
	
	typedef tpl::Mark<LeafT, TagCharT> Mark, LeafMark;
	typedef tpl::Mark<TagNodeType, TagCharT> NodeMark;

public:
	// Leaf, Node, Document
	
	typedef LeafT Leaf;
	typedef tpl::Node<LeafT, TagCharT> Node;
	typedef tpl::Document<LeafT, AllocT, TagCharT> Document;
};

// -------------------------------------------------------------------------
// operator/

template <class RegExT, class LeafT, class AllocT, class TagCharT>
class DocumentedRule
{	
public:
	typedef Document<LeafT, AllocT, TagCharT> DocumentT;
	
	const RegExT& m_rule;
	DocumentT& m_doc;
	
public:
	DocumentedRule(const RegExT& rule_, DocumentT& doc_)
		: m_rule(rule_), m_doc(doc_) {}
};

template <class RegExT, class LeafT, class AllocT, class TagCharT>
inline const DocumentedRule<RegExT, LeafT, AllocT, TagCharT>
TPL_CALL operator/(const Rule<RegExT>& rule_, Document<LeafT, AllocT, TagCharT>& doc_) {
	return DocumentedRule<RegExT, LeafT, AllocT, TagCharT>(rule_, doc_);
}

// -------------------------------------------------------------------------
// operator>>

template <class ContainerT, class RegExT, class LeafT, class AllocT, class TagCharT>
inline bool TPL_CALL operator>>(
	const ContainerT& src_, const DocumentedRule<RegExT, LeafT, AllocT, TagCharT>& dr_)
{
	typedef typename ArchiveTraits<ContainerT>::type SourceT;
	typedef Document<LeafT, AllocT, TagCharT> DocumentT;
	typedef Context<typename SourceT::iterator, LeafT, AllocT, TagCharT> ContextT;
	
	SourceT source(src_);
	DocumentT& doc(dr_.m_doc);
	ContextT context(doc.get_alloc(), doc);
	return dr_.m_rule.match(source, context);
}

// -------------------------------------------------------------------------
// operator==, operator!=

template <class ContainerT, class RegExT, class LeafT, class AllocT, class TagCharT>
inline bool TPL_CALL operator==(
	const ContainerT& src_, const DocumentedRule<RegExT, LeafT, AllocT, TagCharT>& dr_)
{
	typedef typename ArchiveTraits<ContainerT>::type SourceT;
	typedef Document<LeafT, AllocT, TagCharT> DocumentT;
	typedef Context<typename SourceT::iterator, LeafT, AllocT, TagCharT> ContextT;
	
	SourceT source(src_);
	DocumentT& doc(dr_.m_doc);
	ContextT context(doc.get_alloc(), doc);
	return dr_.m_rule.match(source, context) && (source.get() == SourceT::endch);
}

template <class ContainerT, class RegExT, class LeafT, class AllocT, class TagCharT>
inline bool TPL_CALL operator!=(
	const ContainerT& src_, const DocumentedRule<RegExT, LeafT, AllocT, TagCharT>& dr_)
{
	return !(src_ == dr_);
}

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_DOM_H */

