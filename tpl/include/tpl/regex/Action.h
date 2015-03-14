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
// Module: tpl/regex/Action.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Action.h 1364 2009-11-24 06:05:49Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_ACTION_H
#define TPL_REGEX_ACTION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef TPL_REGEX_RULETRAITS_H
#include "RuleTraits.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class Act0

template <class RegExT, class ActionT>
class Act0 : public RegExT
{
public:
	const ActionT m_action;

public:
	Act0() : RegExT(), m_action() {}
	Act0(const RegExT& x, const ActionT& act)
		: RegExT(x), m_action(act) {}

public:
	enum { character = RegExT::character };

	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		if (RegExT::match(ar, context)) {
			m_action();
			return true;
		}
		return false;
	}
};

// -------------------------------------------------------------------------
// operator+

// Usage: SimpleAction1 + SimpleAction2

template <class T1, class T2>
class AndSAct
{
public:
	const T1 m_x;
	const T2 m_y;

public:
	AndSAct() : m_x(), m_y() {}
	AndSAct(const T1& x, const T2& y) : m_x(x), m_y(y) {}

public:
	void TPL_CALL operator()() const {
		m_x();
		m_y();
	}
};

template <class T1, class T2>
inline SimpleAction<AndSAct<T1, T2> > const
TPL_CALL operator+(const SimpleAction<T1>& x, const SimpleAction<T2>& y) {
	return SimpleAction<AndSAct<T1, T2> >(x, y);
}

// =========================================================================
// Action

// TPL_ASSIG

template <class AssigTag>
struct AssigTraits {
};

#define TPL_ASSIG_(AssigTag, Assig)											\
	template <>																\
	struct AssigTraits<AssigTag> {											\
		typedef Assig assig_type;											\
	};

#define TPL_ASSIG(AssigTag, Assig)											\
namespace tpl {																\
	TPL_ASSIG_(AssigTag, Assig)												\
}

#define TPL_ASSIG_DEFVALTYPE(ValueType)										\
	template <class Iterator>												\
	struct Default {														\
		typedef ValueType value_type;										\
	};

#define TPL_ASSIG_PREPARE(AssigTag, ValueT)									\
	typedef typename SourceT::iterator iterator;							\
	typedef SelectValueType<ValueT, NS_STDEXT::Range<iterator> > SelectT_;	\
	typedef typename SelectT_::value_type value_type;						\
	typedef typename SelectAssig<AssigTag, value_type>::assig_type assig_type;
	
#define TPL_ASSIG_PREPARE1(AssigTag)										\
	typedef typename SourceT::iterator iterator;							\
	typedef typename AssigTraits<AssigTag>::assig_type assig_type;			\
	typedef typename assig_type::template Default<iterator> DefaultT_;		\
	typedef typename DefaultT_::value_type value_type;

#define TPL_ASSIG_GET(pos, pos2, pParam)									\
	assig_type::template get<value_type>(pos, pos2, pParam)

template <class AssigTag, class ValueT>
struct SelectAssig {
	typedef typename AssigTraits<AssigTag>::assig_type assig_type;
};

// class Act

template <class RegExT, class ActionT>
class Act : public RegExT
{
public:
	const ActionT m_action;

public:
	Act() : RegExT(), m_action() {}
	Act(const RegExT& x, const ActionT& act)
		: RegExT(x), m_action(act) {}

public:
	enum { character = RegExT::character };
	
	typedef typename RegExT::convertible_type convertible_type;
	typedef typename RegExT::assig_tag assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		TPL_ASSIG_PREPARE(assig_tag, typename ActionT::value_type)
		
		const iterator pos = ar.position();
		if (RegExT::match(ar, context)) {
			const iterator pos2 = ar.position();
			const value_type val(TPL_ASSIG_GET(pos, pos2, static_cast<const RegExT*>(this)));
			m_action(val);
			return true;
		}
		return false;
	}
};

// =========================================================================
// function info

// Usage: Rule/info("...")

inline void TPL_CALL trace_info_(const char* prompt_, const std::string& val) {
	printf(prompt_, val.c_str());
}

inline void TPL_CALL trace_info_(const wchar_t* prompt_, const std::wstring& val) {
	wprintf(prompt_, val.c_str());
}

template <class CharT>
class Info
{
private:
	const CharT* m_prompt;

public:
	Info(const CharT* prompt_) : m_prompt(prompt_) {
	}

public:
	typedef std::basic_string<CharT> value_type;

	void TPL_CALL operator()(const value_type& val) const {
		trace_info_(m_prompt, val);
	}
};

template <class CharT>
inline Action<Info<CharT> > const TPL_CALL info(const CharT* prompt_) {
	return Action<Info<CharT> >(prompt_);
}

// =========================================================================
// function error

// Usage: Rule/error("...")

inline void TPL_CALL trace_error_(const char* prompt_, const std::string& val) {
	fprintf(stderr, prompt_, val.c_str());
}

inline void TPL_CALL trace_error_(const wchar_t* prompt_, const std::wstring& val) {
	fwprintf(stderr, prompt_, val.c_str());
}

template <class CharT>
class Error
{
private:
	const CharT* m_prompt;

public:
	Error(const CharT* prompt_) : m_prompt(prompt_) {
	}

public:
	typedef std::basic_string<CharT> value_type;

	void TPL_CALL operator()(const value_type& val) const {
		trace_error_(m_prompt, val);
	}
};

template <class CharT>
inline Action<Error<CharT> > const TPL_CALL error(const CharT* prompt_) {
	return Action<Error<CharT> >(prompt_);
}

// =========================================================================
// debug: TPL_INFO/TPL_ERROR

enum NullAction { null_action = 0 };

template <class T1> __forceinline
const T1& TPL_CALL operator/(const T1& sth, NullAction na) {
	return sth;
};

#if defined(_DEBUG)
#define TPL_INFO(prompt_)	tpl::info(prompt_ " : %s\n")
#define TPL_ERROR(prompt_)	tpl::error(prompt_ " : %s\n")
#else
#define TPL_INFO(prompt_)	tpl::null_action
#define TPL_ERROR(prompt_)	tpl::null_action
#endif

// =========================================================================
// function println

template <class PredT>
inline void print_ln_(const NS_STDEXT::Range<const char*>& s, const PredT& pred_) {
	if (pred_(s)) {
		fwrite(s.begin(), s.size(), 1, stdout);
		putc('\n', stdout);
	}
}

template <class PredT>
class PrintLn
{
private:
	const PredT m_pred;
	
public:
	PrintLn(const PredT& pred_) : m_pred(pred_) {}

public:
	typedef DefaultType value_type;

	template <class ValueT>
	void TPL_CALL operator()(const ValueT& s) const {
		print_ln_(s, m_pred);
	}
};

template <class PredT>
inline Action<PrintLn<PredT> > const TPL_CALL println(const PredT& pred_) {
	return Action<PrintLn<PredT> >(pred_);
}

// =========================================================================

NS_TPL_END

#endif /* TPL_REGEX_ACTION_H */

