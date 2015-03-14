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
// Module: tpl/regex/result/Context.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: Context.h 794 2008-07-04 06:27:26Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_RESULT_CONTEXT_H
#define TPL_REGEX_RESULT_CONTEXT_H

#ifndef TPL_REGEX_MATCH_H
#include "../Match.h"
#endif

#ifndef TPL_REGEX_RESULT_MARK_H
#include "Mark.h"
#endif

#ifndef TPL_REGEX_RESULT_DOCUMENT_H
#include "Document.h"
#endif

NS_TPL_BEGIN

// -------------------------------------------------------------------------
// class ContextTransaction

template <int uCharacter, class Iterator, class Scope>
class ContextTransaction {
};

template <class Iterator, class Scope>
class ContextTransaction<0, Iterator, Scope>
{
private:
	Iterator vPos;

public:
	template <class SourceT, class ContextT>
	ContextTransaction(const SourceT& ar, const ContextT&) {
		vPos = ar.position();
	}

	template <class SourceT>
	void TPL_CALL rollback(SourceT& ar) {
		ar.seek(vPos);
	}
};

template <class Iterator, class Scope>
class ContextTransaction<CHARACTER_MARKED, Iterator, Scope>
{
private:
	Scope vScope;
	Iterator vPos;

public:
	template <class SourceT, class ContextT>
	ContextTransaction(const SourceT& ar, const ContextT& context) : vScope(context) {
		vPos = ar.position();
	}

	template <class SourceT>
	void TPL_CALL rollback(SourceT& ar) {
		ar.seek(vPos);
		vScope.rollback();
	}
};

// -------------------------------------------------------------------------
// class Context

template <class Iterator, class LeafT, class AllocT, class TagCharT = char>
class Context
{
private:
	typedef Node<LeafT, TagCharT> NodeT;
	typedef NS_STDEXT::ConsList<NodeT*, false> StackT;

	AllocT& m_alloc;
	StackT m_stk;

public:
	Context(AllocT& alloc, NodeT& doc)
		: m_alloc(alloc)
	{
		m_stk.push_front(alloc, &doc);
	}

private:
	typedef Mark<LeafT, TagCharT> LeafMarkT;
	typedef Mark<TagNodeType, TagCharT> NodeMarkT;

public:
	typedef AllocT alloc_type;

	alloc_type& TPL_CALL get_alloc() const {
		return m_alloc;
	}

	void TPL_CALL insertLeaf(const LeafMarkT& mark, const LeafT& val) {
		m_stk.front()->insertLeaf(m_alloc, mark, val);
	}

public:
	class ScopeTransaction
	{
	private:
		NodeT* vParent;
		NodeT vOld;

	public:
		ScopeTransaction(const Context& context) {
			StackT stk = context.m_stk;
			vParent = stk.front();
			vOld = *vParent;
		}

		void TPL_CALL rollback() {
			*vParent = vOld;
		}
	};

	typedef ScopeTransaction scope_trans_type;

public:
	template <int uCharacter>
	class trans_type : public ContextTransaction<uCharacter, Iterator, ScopeTransaction>
	{
	private:
		typedef ContextTransaction<uCharacter, Iterator, ScopeTransaction> Base;
		
	public:
		template <class SourceT>
		trans_type(const SourceT& ar, const Context& context)
			: Base(ar, context) {
		}
	};

public:
	class Scope
	{
	private:
		StackT& m_stk;
	
	public:
		Scope(Context& context, const NodeMarkT& mark)
			: m_stk(context.m_stk)
		{
			NodeT* v = m_stk.front()->insertNode(context.m_alloc, mark);
			m_stk.push_front(context.m_alloc, v);
		}
		~Scope() {
			m_stk.pop_front();
		}
	};
	friend class Scope;

	typedef Scope scope_type;
};

// -------------------------------------------------------------------------
// $Log: $

NS_TPL_END

#endif /* TPL_REGEX_RESULT_CONTEXT_H */
