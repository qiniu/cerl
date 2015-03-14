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
// Module: tpl/regex/Ref.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Ref.h 867 2008-07-29 10:40:46Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_REF_H
#define TPL_REGEX_REF_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_TERMINAL_H
#include "Terminal.h"
#endif

#ifndef TPL_REGEX_COMPOSITION_H
#include "Composition.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function ref(a_rule_var)

// Usage: ref(a_rule_var)	--- Work as a rule.
// Note: if two rules A, B circularly refer to each other (or a rule A refer to itself),
//	we need use ref() function to break the circuit.

template <class RegExT>
class Ref
{
private:
	const RegExT& m_x;

public:
	Ref(const RegExT& x) : m_x(x) {}

public:
	enum { character = RegExT::character };

	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return m_x.match(ar, context);
	}
};

template <class T1>
__forceinline Rule<Ref<T1> > const TPL_CALL ref(const Rule<T1>& x) {
	return Rule<Ref<T1> >(x);
}

// =========================================================================
// function ref(a_grammar_var)

// Usage: ref(a_grammar_var)	--- Work as a grammar.
// Note: if two rules A, B circularly refer to each other (or a grammar A refer to itself),
//	we need use ref() function to break the circuit.

template <class GrammarT>
class GRef
{
private:
	const GrammarT& m_x;

public:
	GRef(const GrammarT& x) : m_x(x) {}

public:
	enum { character = GrammarT::character };

	typedef typename GrammarT::assig_tag assig_tag;

	template <class SourceT, class ContextT, class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const {
		return m_x.match(ar, context, skipper_);
	}
};

template <class T1>
__forceinline Grammar<GRef<T1> > const TPL_CALL ref(const Grammar<T1>& x) {
	return Grammar<GRef<T1> >(x);
}

// =========================================================================
// Dereference for Rule

// RefT(Reference) concept:
//
// typedef typename RefT::value_type value_type;
// typedef typename RefT::rule_type rule_type;
// typedef typename RefT::dereference_type dereference_type;
//
// dereference_type TPL_CALL operator()() const;

template <class RefT>
class Deref : public RefT
{
private:
	typedef typename RefT::dereference_type dereference_type;
	typedef typename RefT::rule_type rule_type;

public:
	Deref(const RefT& ref_) : RefT(ref_) {}

public:
	enum { character = rule_type::character };

	typedef typename rule_type::convertible_type convertible_type;
	typedef typename rule_type::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		const dereference_type der(RefT::operator()());
		const rule_type x(der);
		return x.match(ar, context);
	}

	bool TPL_CALL operator()(int c) const {
		const dereference_type der(RefT::operator()());
		const rule_type x(der);
		return x(c);
	}
};

// =========================================================================
// function ref(a_ch_var)

template <class CharT>
struct CharTraits {
};

template <>
struct CharTraits<char> {
	typedef char char_type;
	typedef unsigned char uchar_type;
};

template <>
struct CharTraits<wchar_t> {
	typedef wchar_t char_type;
	typedef wchar_t uchar_type;
};

template <>
struct CharTraits<int> {
	typedef int char_type;
	typedef unsigned int uchar_type;
};

template <class CharT>
class RefCh
{
private:
	typedef CharTraits<CharT> Tr_;
	typedef typename Tr_::uchar_type UCharT;

	const CharT& m_c;

public:
	RefCh(const CharT& c) : m_c(c) {}

public:
	typedef CharT value_type;
	typedef int dereference_type;
	typedef Ch1 rule_type;

	dereference_type TPL_CALL operator()() const {
		return (UCharT)m_c;
	}
};

// =========================================================================
// function ref(a_str_var), ref(leaf)

template <class StringT, class Iterator = typename StringT::const_iterator>
class RefStr_
{
private:
	const StringT& m_s;

public:
	RefStr_(const StringT& s) : m_s(s) {}

public:
	typedef StringT value_type;
	typedef const StringT& dereference_type;
	typedef EqRange_<Iterator> rule_type;

	dereference_type TPL_CALL operator()() const {
		return m_s;
	}
};

template <class CharT>
class RefCStr : public RefStr_<const CharT*, const CharT*>
{
public:
	RefCStr(const CharT* const& s)
		: RefStr_<const CharT*, const CharT*>(s) {}
};

template <class CharT>
class RefStr : public RefStr_<std::basic_string<CharT> >
{
public:
	RefStr(const std::basic_string<CharT>& s)
		: RefStr_<std::basic_string<CharT> >(s) {}
};

template <class Iterator>
class RefLeaf : public RefStr_<NS_STDEXT::Range<Iterator> >
{
public:
	RefLeaf(const NS_STDEXT::Range<Iterator>& leaf)
		: RefStr_<NS_STDEXT::Range<Iterator> >(leaf) {}
};

// =========================================================================
// function ref(var)

template <class Type>
struct ReferenceTraits {
};

template <class CharT>
struct ReferenceTraits<const CharT*> {
	typedef RefCStr<CharT> reference_type;
};

template <class CharT>
struct ReferenceTraits<std::basic_string<CharT> > {
	typedef RefStr<CharT> reference_type;
};

template <class Iterator>
struct ReferenceTraits<NS_STDEXT::Range<Iterator> > {
	typedef RefLeaf<Iterator> reference_type;
};

template <>
struct ReferenceTraits<char> {
	typedef RefCh<char> reference_type;
};

template <>
struct ReferenceTraits<wchar_t> {
	typedef RefCh<wchar_t> reference_type;
};

template <class Type> __forceinline
Rule<Deref<typename ReferenceTraits<Type>::reference_type> > const TPL_CALL ref(const Type& var_) {
	return Rule<Deref<typename ReferenceTraits<Type>::reference_type> >(var_);
}

// =========================================================================
// class Var

template <class Type>
class Var : public Rule<Deref<typename ReferenceTraits<Type>::reference_type> >
{
public:
	Type val;

public:
	Var() : Rule<Deref<typename ReferenceTraits<Type>::reference_type> >(val) {}

	operator Type&() {
		return val;
	}
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_REF_H */

