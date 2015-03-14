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
// Module: tpl/regex/Concretion.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Concretion.h 1134 2009-05-18 09:12:33Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_CONCRETION_H
#define TPL_REGEX_CONCRETION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function tpl_allocator

template <class Unused>
struct SingtonAllocatorT {
	static DefaultAllocator g_inst;
};

template <class Unused>
DefaultAllocator SingtonAllocatorT<Unused>::g_inst;

typedef SingtonAllocatorT<void> SingtonAllocator;

#define tpl_allocator()		NS_TPL::SingtonAllocator::g_inst

// =========================================================================
// class Concretion

template <int uCharacter, class SourceT, class ContextT, bool bManaged = false>
class Concretion
{
private:
	typedef bool TPL_CALL _FN_match(const void* pThis, SourceT& ar, ContextT& context);
	typedef _FN_match* FN_match;

	const void* m_this;
	FN_match m_fn;

public:
	Concretion() : m_this(NULL) {}
	
	Concretion(const void* this_, FN_match fn) 
		: m_this(this_), m_fn(fn) {
	}

	template <class AllocT>
	Concretion(AllocT& alloc, const Concretion& x) {
		new(this) Concretion(x);
	}

	template <class AllocT, class RegExT>
	Concretion(AllocT& alloc, const Rule<RegExT>& x) {
		assign(alloc, x);
	}

	template <class RegExT>
	Concretion(const Rule<RegExT>& x) {
		assign(tpl_allocator(), x);
	}

public:
	enum { character = uCharacter };

	typedef TagAssigNone assig_tag;
	typedef ExplicitConvertible convertible_type;
	typedef const Concretion& concreation_type;

	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		TPL_ASSERT(m_this);
		return m_fn(m_this, ar, context);
	}

	concreation_type TPL_CALL concretion() const {
		return *this;
	}

public:
	template <class RegExT>
	class Impl
	{
	private:
		RegExT m_x;

	public:
		Impl(const Rule<RegExT>& x)
			: m_x(static_cast<const RegExT&>(x)) {
		}

		enum { characterImpl = RegExT::character };

		typedef typename RegExT::assig_tag assig_tag;
		typedef typename RegExT::convertible_type convertible_type;

		static bool TPL_CALL match(const void* pThis, SourceT& ar, ContextT& context) {
			const RegExT& x = ((const Impl*)pThis)->m_x;
			return x.match(ar, context);
		}

		TPL_REQUIRE((characterImpl & ~character) == 0, CharacterRequired_);
	};

public:
	template <class AllocT, class RegExT>
	void TPL_CALL assign(AllocT& alloc, const Rule<RegExT>& x)
	{
		typedef Impl<RegExT> Imp;
		if (bManaged)
			m_this = TPL_NEW(alloc, Imp)(x);
		else
			m_this = TPL_UNMANAGED_NEW(alloc, Imp)(x);
	
		m_fn = Imp::match;
	}

	void TPL_CALL assign(const void* this_, FN_match fn)
	{
		m_this = this_;
		m_fn = fn;
	}

	template <class RegExT>
	void TPL_CALL operator=(const Rule<RegExT>& x)
	{
		assign(tpl_allocator(), x);
	}

public:
	class Var : public Rule<Ref<Concretion> >
	{
	private:
		Concretion m_x;

	public:
		Var() : Rule<Ref<Concretion> >(m_x) {
		}

		template <class AllocT, class RegExT>
		void TPL_CALL assign(AllocT& alloc, const Rule<RegExT>& x) {
			m_x.assign(alloc, x);
		}

		template <class RegExT>
		void TPL_CALL operator=(const Rule<RegExT>& x) {
			m_x = x;
		}
	};
};

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_CONCRETION_H */
