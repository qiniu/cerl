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
// Module: tpl/regex/grammar/Concretion.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Concretion.h 1326 2009-11-19 12:58:52Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_GRAMMAR_CONCRETION_H
#define TPL_REGEX_GRAMMAR_CONCRETION_H

#ifndef TPL_REGEX_CONCRETION_H
#include "../Concretion.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class GrBind -- Grammar + Skipper => Rule

template <class GrammarT, class SkipperT, int fSkipTail = TRUE>
class GrBind
{
public:
	const GrammarT m_gr;
	const SkipperT m_skipper;

public:
	GrBind(const GrammarT& gr_, const SkipperT& skipper_)
		: m_gr(gr_), m_skipper(skipper_) {}

public:
	enum { character = GrammarT::character | SkipperT::character };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;
	typedef const Grammar<GrammarT>& grammar_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		if (m_gr.match(ar, context, m_skipper)) {
			if (fSkipTail)
				m_skipper.match(ar, context);
			return true;
		}
		return false;
	}
};

template <class SkipperT, class GrammarT>
struct IndexOpTraits<Rule<SkipperT>, Grammar<GrammarT> >
{
	typedef Rule<GrBind<GrammarT, SkipperT> > result_type;

	static result_type TPL_CALL call(const Rule<SkipperT>& y, const Grammar<GrammarT>& x) {
		return result_type(x, y);
	}
};

template <class SkipperT, class GrammarT>
inline Rule<GrBind<GrammarT, SkipperT, FALSE> > const
TPL_CALL operator*(const Rule<SkipperT>& y, const Grammar<GrammarT>& x) {
	return Rule<GrBind<GrammarT, SkipperT, FALSE> >(x, y);
}

#define TPL_SKIPPER_BIND_(GrammarT)		\
template <class SkipperT>				\
struct IndexOpTraits<Rule<SkipperT>, GrammarT> {			\
	typedef Rule<GrBind<GrammarT, SkipperT> > result_type;	\
	static result_type TPL_CALL call(const Rule<SkipperT>& y, const GrammarT& x) {	\
		return result_type(x, y);		\
	}									\
};

// =========================================================================
// class SkipperTraits

template <class SkipperT, class SourceT, class ContextT>
class SkipperTraits
{
private:
	typedef Concretion<CHARACTER_SKIPPER, SourceT, ContextT, false> ConcretionT;
	typedef typename ConcretionT::template Impl<SkipperT> ConcretionImplT;
	
public:
	typedef ConcretionT concreation_type;

	static concreation_type TPL_CALL concretion(const SkipperT& skipper_) {
		const ConcretionImplT* impl_ = (const ConcretionImplT*)&skipper_;
		return ConcretionT(impl_, ConcretionImplT::match);
	}
};

template <class SourceT, class ContextT>
class SkipperTraits<Concretion<CHARACTER_SKIPPER, SourceT, ContextT, false>, SourceT, ContextT>
{
private:
	typedef Concretion<CHARACTER_SKIPPER, SourceT, ContextT, false> ConcretionT;
	
public:
	typedef const ConcretionT& concreation_type;

	static concreation_type TPL_CALL concretion(const ConcretionT& skipper_) {
		return skipper_;
	}
};

// -------------------------------------------------------------------------
// class GConcretion

template <int uCharacter, class SourceT, class ContextT, bool bManaged = false>
class GConcretion
{
private:
	typedef Concretion<CHARACTER_SKIPPER, SourceT, ContextT, false> ConcretionT;
	typedef bool TPL_CALL _FN_match(const void* pThis, SourceT& ar, ContextT& context, const ConcretionT& skipper_);
	typedef _FN_match* FN_match;

	const void* m_this;
	FN_match m_fn;

public:
	GConcretion() : m_this(NULL) {}

	template <class GrammarT>
	GConcretion(const Grammar<GrammarT>& x) {
		assign(tpl_allocator(), x);
	}

	template <class AllocT, class GrammarT>
	GConcretion(AllocT& alloc, const Grammar<GrammarT>& x) {
		assign(alloc, x);
	}

public:
	enum { character = uCharacter };
	
	typedef TagAssigNone assig_tag;

	template <class SkipperT>
	bool TPL_CALL match(SourceT& ar, ContextT& context, const SkipperT& skipper_) const {
		TPL_ASSERT(m_this);
		typedef SkipperTraits<SkipperT, SourceT, ContextT> Tr_;
		return m_fn(m_this, ar, context, Tr_::concretion(skipper_));
	}

private:
	template <class GrammarT>
	class Impl
	{
	private:
		GrammarT m_x;

	public:
		Impl(const Grammar<GrammarT>& x)
			: m_x(static_cast<const GrammarT&>(x)) {
		}

		enum { character = GrammarT::character };

		static bool TPL_CALL match(const void* pThis, SourceT& ar, ContextT& context, const ConcretionT& skipper_) {
			const GrammarT& x = ((const Impl*)pThis)->m_x;
			return x.match(ar, context, skipper_);
		}
	};

public:
	template <class AllocT, class GrammarT>
	void TPL_CALL assign(AllocT& alloc, const Grammar<GrammarT>& x)
	{
		typedef Impl<GrammarT> Imp;
		TPL_ASSERT((Imp::character & ~character) == 0);

		if (bManaged)
			m_this = TPL_NEW(alloc, Imp)(x);
		else
			m_this = TPL_UNMANAGED_NEW(alloc, Imp)(x);

		m_fn = Imp::match;
	}

	template <class GrammarT>
	void TPL_CALL operator=(const Grammar<GrammarT>& x)
	{
		assign(tpl_allocator(), x);
	}

public:
	class Var : public Grammar<GRef<GConcretion> >
	{
	private:
		GConcretion m_x;

	public:
		Var() : Grammar<GRef<GConcretion> >(m_x) {
		}

		template <class AllocT, class GrammarT>
		void TPL_CALL assign(AllocT& alloc, const Grammar<GrammarT>& x) {
			m_x.assign(alloc, x);
		}

		template <class GrammarT>
		void TPL_CALL operator=(const Grammar<GrammarT>& x) {
			m_x = x;
		}
	};
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_GRAMMAR_CONCRETION_H */
