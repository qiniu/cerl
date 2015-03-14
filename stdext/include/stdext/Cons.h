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
// Module: stdext/Cons.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-6-14 9:41:58
// 
// $Id: Cons.h 642 2008-06-12 03:25:29Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_CONS_H
#define STDEXT_CONS_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class ConsUtils

template <class Type>
class ConsUtils
{
public:
	class Node
	{
	public:
		Type const value;
		const Node* const tail;

	public:
		Node(Type const& val_, const Node* tail_)
			: value(val_), tail(tail_) {
		}

		const Type& winx_call hd() const {
			return value;
		}

		const Node* winx_call tl() const {
			return tail;
		}
	};

public:
	typedef const Type& reference;
	typedef const Node* cons;

public:
	template <class CondT>
	static cons winx_call find(cons n, CondT cond)
	{
		for (; n; n = n->tail)
		{
			reference val = n->value;
			if (cond(val))
				return n;
		}
		return NULL;
	}

	template <class CondT>
	static size_t winx_call count_if(cons n, CondT cond)
	{
		size_t len = 0;
		for (; n; n = n->tail)
		{
			reference val = n->value;
			if (cond(val))
				++len;
		}
		return len;
	}

	static size_t winx_call length(cons n)
	{
		size_t len = 0;
		for (; n; n = n->tail)
			++len;
		return len;
	}
};

// -------------------------------------------------------------------------
// class Cons

template <class Type, bool bManaged = true>
class Cons : public ConsUtils<Type>
{
private:
	typedef ConsUtils<Type> Base;
	typedef typename Base::Node Node;

public:
	typedef typename Base::reference reference;
	typedef typename Base::cons cons;

public:
	template <class AllocT>
	static cons winx_call make(AllocT& alloc, const Type& val, cons tail)
	{
		if (bManaged)
			return STD_NEW(alloc, Node)(val, tail);
		else
			return STD_UNMANAGED_NEW(alloc, Node)(val, tail);
	}

	template <class AllocT>
	static cons winx_call reverse(AllocT& alloc, cons n)
	{
		cons lst = NULL;
		for (; n; n = n->tail)
		{
			lst = make(alloc, n->value, lst);
		}
		return lst;
	}

	template <class AllocT, class CondT>
	static cons winx_call select(AllocT& alloc, cons n, CondT cond)
	{
		cons lstRet = NULL;
		cons* ptail = &lstRet;
		for (; n; n = n->tail)
		{
			reference val = n->value;
			if (cond(val))
			{
				cons node = make(alloc, val, NULL);
				*ptail = node;
				ptail = (cons*)&node->tail;
			}
		}
		return lstRet;
	}
};

// -------------------------------------------------------------------------
// class ConsList

template <class Type, bool bManaged = true>
class ConsList
{
private:
	typedef NS_STDEXT::Cons<Type, bManaged> Cons;

public:
	typedef typename Cons::reference reference;
	typedef typename Cons::cons cons;

private:
	cons m_hd;

public:
	ConsList() : m_hd(NULL) {}
	ConsList(cons hd_) : m_hd(hd_) {}

	operator cons() const {
		return m_hd;
	}

	cons winx_call data() const {
		return m_hd;
	}

	cons winx_call operator=(cons hd_) {
		return m_hd = hd_;
	}

	bool winx_call empty() const {
		return m_hd == NULL;
	}

	size_t winx_call length() const {
		return Cons::length(m_hd);
	}

	template <class CondT>
	size_t winx_call count_if(CondT cond) const {
		return Cons::count_if(m_hd, cond);
	}

	reference winx_call front() const {
		return m_hd->value;
	}

	template <class AllocT>
	void winx_call push_front(AllocT& alloc, reference v) {
		m_hd = Cons::make(alloc, v, m_hd);
	}

	void winx_call pop_front() {
		m_hd = m_hd->tail;
	}

	template <class AllocT>
	void winx_call reverse(AllocT& alloc) {
		m_hd = Cons::reverse(alloc, m_hd);
	}

	template <class AllocT, class CondT>
	cons winx_call select(AllocT& alloc, CondT cond) const {
		return Cons::select(alloc, m_hd, cond);
	}

	template <class CondT>
	cons TPL_CALL find(CondT cond) const {
		return Cons::find(m_hd, cond);
	}
};

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_CONS_H */

