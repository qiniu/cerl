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
// Module: tpl/regex/Composition.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Composition.h 810 2008-07-07 20:45:03Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_COMPOSITION_H
#define TPL_REGEX_COMPOSITION_H

#ifndef TPL_REGEX_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "../../../../stdext/include/stdext/archive/Basic.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// function eps, nothing

// Usage: eps()	--- means: always be successful.
// Usage: nothing()	--- means: always be failed.

template <bool bOk>
class Null
{
public:
	enum { character = 0 };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return bOk;
	}

	TPL_SIMPLEST_GRAMMAR_();
};

class Eps : public Null<true> {};
class Nothing : public Null<false> {};

__forceinline Rule<Eps> TPL_CALL eps() {
	return Rule<Eps>();
}

__forceinline Rule<Nothing> TPL_CALL nothing() {
	return Rule<Nothing>();
}

// -------------------------------------------------------------------------
// function done

// Usage: done() --- means: seek to end-of-stream and return ok

class Done
{
public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		ar.seek_end();
		return true;
	}
};

__forceinline Rule<Done> TPL_CALL done() {
	return Rule<Done>();
}

// -------------------------------------------------------------------------
// function eos/eof

// Usage: eos() --- means: matching end-of-stream
// Usage: eof() --- same as: eos()

class Eos
{
public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return ar.peek() == SourceT::endch;
	}
};

typedef Eos Eof;

__forceinline Rule<Eos> TPL_CALL eos() {
	return Rule<Eos>();
}

__forceinline Rule<Eos> TPL_CALL eof() {
	return Rule<Eos>();
}

// -------------------------------------------------------------------------
// function not_eos/not_eof

// Usage: not_eos() --- means: true if current position is not end-of-stream
// Usage: not_eof() --- same as: not_eos()

class NotEos
{
public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return ar.peek() != SourceT::endch;
	}
};

typedef NotEos NotEof;

__forceinline Rule<NotEos> TPL_CALL not_eos() {
	return Rule<NotEos>();
}

__forceinline Rule<NotEos> TPL_CALL not_eof() {
	return Rule<NotEos>();
}

// =========================================================================
// class LenStr

template <class CharT, const CharT* m_s>
class LenStr
{
public:
	static const size_t m_len;
};

template <class CharT, const CharT* m_s>
const size_t LenStr<CharT, m_s>::m_len = std::char_traits<CharT>::length(m_s);

// -------------------------------------------------------------------------
// class Str

template <class Iterator, class SourceT, class ContextT>
inline bool TPL_CALL match_str(Iterator first, Iterator last, SourceT& ar, ContextT& context)
{
	typedef typename std::ArchiveIteratorTraits<Iterator>::value_type char_type;
	typedef typename std::ArchiveCharTraits<char_type>::uchar_type uchar_type;
	typename SourceT::iterator pos = ar.position();
	for (; first != last; ++first) {
		if (ar.get() != (uchar_type)*first) {
			ar.seek(pos);
			return false;
		}
	}
	return true;
}

template <class CharT, const CharT* m_s>
class BasicStr
{
private:
	typedef LenStr<CharT, m_s> LenStrT;

public:
	template <class Iterator>
	bool TPL_CALL operator()(Iterator first, Iterator last) const {
		size_t len = std::distance(first, last);
		return len == LenStrT::m_len && std::equal(m_s, m_s + len, first);
	}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_str(m_s, m_s + LenStrT::m_len, ar, context);
	}
};

template <const char* m_s>
class Str : public BasicStr<char, m_s> {
};

template <const wchar_t* m_s>
class WStr : public BasicStr<wchar_t, m_s> {
};

// =========================================================================
// class EqStr_

template <class Iterator>
class EqRange_
{
private:
	Iterator m_s;
	size_t m_len;

public:
	template <class StringT>
	EqRange_(const StringT& s) {
		m_s = s.begin();
		m_len = s.size();
	}

	EqRange_(Iterator s, size_t n) {
		m_s = s;
		m_len = n;
	}

public:
	template <class Iterator2>
	bool TPL_CALL operator()(Iterator2 first, Iterator2 last) const {
		size_t len = std::distance(first, last);
		return len == m_len && std::equal(m_s, m_s + len, first);
	}

public:
	enum { character = 0 };

	typedef AutoConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const {
		return match_str(m_s, m_s + m_len, ar, context);
	}
};

template <class CharT>
class EqStr_ : public EqRange_<const CharT*>
{
private:
	typedef EqRange_<const CharT*> Base;

public:
	EqStr_(const CharT* s, size_t n)
		: Base(s, n) {
	}

	EqStr_(const CharT* s)
		: Base(s,std::char_traits<CharT>::length(s)) {
	}
};

typedef EqStr_<char> EqStr;
typedef EqStr_<wchar_t> EqWStr;

// -------------------------------------------------------------------------
// function str

// Usage: str("abcd")			--- Work as a rule. Same as: str("abcd")
// Usage: str("abcd", 3)			--- Work as a rule. Same as: str("abcd")

template <class CharT>
__forceinline Rule<EqStr_<CharT> > TPL_CALL str(const CharT* s) {
	return Rule<EqStr_<CharT> >(s);
}

template <class CharT>
__forceinline Rule<EqStr_<CharT> > TPL_CALL str(const CharT* s, size_t n) {
	return Rule<EqStr_<CharT> >(s, n);
}

// =========================================================================
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_COMPOSITION_H */
