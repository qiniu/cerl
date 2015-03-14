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
// Module: tpl/regex/Customization.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Customization.h 766 2008-06-28 15:41:08Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CUSTOMIZATION_H
#define TPL_REGEX_CUSTOMIZATION_H

#ifndef TPL_REGEX_DOM_H
#include "DOM.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// DefaultSource, DefaultResult

typedef std::PointerReadArchive DefaultSource;
typedef tpl::DOM<> DefaultResult;
typedef void NullResult;

// -------------------------------------------------------------------------
// CustomizationTraits_

template <class SourceT, class ResultT>
class CustomizationTraits_
{
private:
	typedef typename ResultT::TagChar TagCharT;

public:
	typedef typename ResultT::Allocator Allocator;
	
	typedef typename ResultT::Mark Mark;
	typedef typename ResultT::LeafMark LeafMark;
	typedef typename ResultT::NodeMark NodeMark;
	
	typedef typename ResultT::Leaf Leaf;
	typedef typename ResultT::Node Node;
	typedef typename ResultT::Document Document;

	typedef tpl::Context<typename SourceT::iterator, Leaf, Allocator, TagCharT> Context;

	enum { characterMarked	= CHARACTER_MARKED };
};

template <class SourceT>
class CustomizationTraits_<SourceT, NullResult>
{
public:
	typedef DefaultAllocator Allocator;
	typedef DefaultLeaf Leaf;
	
	typedef void Mark;
	typedef void LeafMark;
	typedef void NodeMark;
	
	typedef void Node;
	typedef void Document;

	typedef Context0<typename SourceT::iterator> Context;

	enum { characterMarked	= 0 };
};

// -------------------------------------------------------------------------
// class Customization

template <class SourceT = DefaultSource, class ResultT = DefaultResult>
class Customization
{
private:
	typedef CustomizationTraits_<SourceT, ResultT> Tr_;

public:
	// Source, Result

	typedef SourceT Source;
	typedef ResultT Result;

public:
	// Result: Allocator, Mark, LeafMark, NodeMark, Leaf, Node, Document
	
	typedef typename Tr_::Allocator Allocator;
	
	typedef typename Tr_::Mark Mark;
	typedef typename Tr_::LeafMark LeafMark;
	typedef typename Tr_::NodeMark NodeMark;
	
	typedef typename Tr_::Leaf Leaf;
	typedef typename Tr_::Node Node;
	typedef typename Tr_::Document Document;

public:
	// Iterator

	typedef typename Source::iterator Iterator;

public:
	// Context

	typedef typename Tr_::Context Context;

public:
	// Rule

	template <int uCharacter>
	class RuleT : public tpl::Rule<Concretion<uCharacter, Source, Context, false> >
	{
	private:
		typedef tpl::Rule<Concretion<uCharacter, Source, Context, false> > Base;

	public:
		RuleT() {}

		template <class RegExT>
		RuleT(const RegExT& x)
			: Base(x) {}

		template <class AllocT, class RegExT>
		RuleT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef RuleT<0> Rule;
	typedef RuleT<Tr_::characterMarked> MarkedRule;

public:
	// ManagedRule

	template <int uCharacter>
	class ManagedRuleT : public tpl::Rule<Concretion<uCharacter, Source, Context, true> >
	{
	private:
		typedef tpl::Rule<Concretion<uCharacter, Source, Context, true> > Base;

	public:
		ManagedRuleT() {}

		template <class RegExT>
		ManagedRuleT(const RegExT& x)
			: Base(x) {}

		template <class AllocT, class RegExT>
		ManagedRuleT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef ManagedRuleT<0> ManagedRule;
	typedef ManagedRuleT<Tr_::characterMarked> ManagedMarkedRule;

public:
	// Rule helper functions:

	template <class RegExT>
	static bool TPL_CALL match(Iterator pos, Iterator pos2, const tpl::Rule<RegExT>& rule_)
	{
		Source source(pos, pos2);
		Context context;
		return rule_.match(source, context);
	}

	template <class ContainerT, class RegExT>
	static bool TPL_CALL match(const ContainerT& src, const tpl::Rule<RegExT>& rule_)
	{
		Source source(src);
		Context context;
		return rule_.match(source, context);
	}

public:
	// Grammar

	template <int uCharacter>
	class GrammarT : public tpl::Grammar<GConcretion<uCharacter, Source, Context, false> >
	{
	private:
		typedef tpl::Grammar<GConcretion<uCharacter, Source, Context, false> > Base;

	public:
		GrammarT() {}

		template <class RegExT>
		GrammarT(const RegExT& x)
			: Base(x) {}

		template <class AllocT, class RegExT>
		GrammarT(AllocT& alloc, const RegExT& x)
			: Base(alloc, x) {}
	};

	typedef GrammarT<0> Grammar;
	typedef GrammarT<Tr_::characterMarked> MarkedGrammar;

public:
	// Grammar helper functions:

	template <class RegExT, class SkipperT>
	static inline bool TPL_CALL match(
		Iterator pos, Iterator pos2, const tpl::Grammar<RegExT>& grammar_, 
		const tpl::Rule<SkipperT>& skipper_)
	{
		Source source(pos, pos2);
		Context context;
		return grammar_.match(source, context, skipper_);
	}

	template <class ContainerT, class RegExT, class SkipperT>
	static inline bool TPL_CALL match(
		const ContainerT& src, const tpl::Grammar<RegExT>& grammar_,
		const tpl::Rule<SkipperT>& skipper_)
	{
		Source source(src);
		Context context;
		return grammar_.match(source, context, skipper_);
	}
};

// -------------------------------------------------------------------------
// SimpleImplementation

typedef Customization<DefaultSource, NullResult> SimpleImplementation;

typedef SimpleImplementation simple;

// -------------------------------------------------------------------------
// DefaultImplementation

typedef Customization<> DefaultImplementation;

typedef DefaultImplementation impl;

// -------------------------------------------------------------------------
// Skipper Bind

TPL_SKIPPER_BIND_(simple::Grammar)

TPL_SKIPPER_BIND_(impl::Grammar)
TPL_SKIPPER_BIND_(impl::MarkedGrammar)

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CUSTOMIZATION_H */

