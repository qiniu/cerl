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
// Module: tpl/c/Comment.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: Comment.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef TPL_C_COMMENT_H
#define TPL_C_COMMENT_H

#ifndef TPL_C_LEX_H
#include "Lex.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class FilterCComment

#define TPL_CSKIP_M1_		( +ch('*') )
#define TPL_CSKIP_M2_		( +ch('-') )
#define TPL_CSKIP_M3_		( +ch('=') )
#define TPL_CSKIP_M4_		( +ch('/') )
#define TPL_CSKIP_M5_		( '!' )
#define TPL_CSKIP_MX_		( TPL_CSKIP_M1_ | TPL_CSKIP_M2_ | TPL_CSKIP_M3_ | TPL_CSKIP_M4_ | TPL_CSKIP_M5_ )
#define TPL_CSKIP_SP_		( non_eol_space() | peek<0x0a, 0x0d, -1>() )
#define TPL_CSKIP_M_		( skipws() + TPL_CSKIP_MX_ + TPL_CSKIP_SP_ )

template <class ArchiveT>
class FilterCComment
{
private:
	typedef typename ArchiveT::char_type CharT;

	ArchiveT& m_wr;

public:
	explicit FilterCComment(ArchiveT& wr)
		: m_wr(wr) {
	}

public:
	typedef NS_STDEXT::BasicString<CharT> value_type;

	void TPL_CALL operator()(const value_type& val) const
	{
		val >> *( !TPL_CSKIP_M_ + ((paragraph() + eol())/put(m_wr) | strict_eol()) );
	}
};

// -------------------------------------------------------------------------
// class CppCommentContent

#define TPL_C_COMMENT_CONTENT			( +("/*" + find("*/")/filter + "*/") )
#define TPL_CPPONLY_COMMENT_CONTENT		( +("//" + find_eol<true>()/filter) )
#define TPL_CPP_COMMENT_CONTENT			( TPL_CPPONLY_COMMENT_CONTENT | TPL_C_COMMENT_CONTENT )

template <class AllocT, class RegExT>
class CppCommentContent
{
private:
	Rule<RegExT> const m_rule;
	AllocT& m_alloc;

public:
	CppCommentContent(AllocT& alloc, const Rule<RegExT>& rule)
		: m_rule(rule), m_alloc(alloc)
	{
	}

public:
	enum { character = RegExT::character };

	typedef SelfConvertible convertible_type;
	typedef TagAssigNone assig_tag;

	template <class SourceT, class ContextT>
	bool TPL_CALL match(SourceT& ar, ContextT& context) const
	{
		using namespace NS_STDEXT;
		typedef typename SourceT::char_type char_type;
		typedef std::vector<char_type> content_type;
		typedef MemWriteArchive<content_type> content_writer;
		content_type text;
		content_writer wr(text);
		Action<FilterCComment<content_writer> > filter(wr);
		bool fOk = TPL_CPP_COMMENT_CONTENT.match(ar, context);
		if (fOk)
			NS_TPL::match(BasicString<char_type>(m_alloc, text), m_rule, context);
		return fOk;
	}

	TPL_SIMPLEST_GRAMMAR_();
};

template <class AllocT, class RegExT>
inline Rule<CppCommentContent<AllocT, RegExT> > const
TPL_CALL cpp_comment_content(AllocT& alloc, const Rule<RegExT>& rule) {
	return Rule<CppCommentContent<AllocT, RegExT> >(alloc, rule);
}

// -------------------------------------------------------------------------

NS_TPL_END

#endif /* TPL_C_COMMENT_H */
