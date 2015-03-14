/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/List.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: List.h 2577 2010-04-20 07:38:10Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_LIST_H
#define ASYNC_LIST_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// CERL_DBG_ERASE_LINK

#if defined(_DEBUG)
#define CERL_DBG_ERASE_LINK(p)	(p)->prev = NULL
#else
#define CERL_DBG_ERASE_LINK(p)	(void)NULL
#endif

// -------------------------------------------------------------------------
// struct ListNode

struct ListNode
{
	ListNode* prev;
};

// -------------------------------------------------------------------------
// function reverse

inline ListNode* cerl_call reverse(ListNode* lst, ListNode* head = NULL)
{
	while (lst)
	{
		ListNode* prev = (ListNode*)lst->prev;
		lst->prev = head;
		head = lst;
		lst = prev;
	}
	return head;
}

// -------------------------------------------------------------------------
// class List

class List
{
public:
	typedef ListNode Node;

private:
	Node* m_head;

public:
	List() : m_head(NULL)
	{
	}

	bool cerl_call empty() const
	{
		return m_head == NULL;
	}

	void cerl_call push(Node* val)
	{
		val->prev = m_head;
		m_head = val;
	}

	Node* cerl_call top()
	{
		return m_head;
	}

	Node* cerl_call pop()
	{
		Node* val = m_head;
		if (val)
		{
			m_head = val->prev;
			CERL_DBG_ERASE_LINK(val);
		}
		return val;
	}
};

typedef List Stack;

// -------------------------------------------------------------------------
// class Queue

class Queue
{
public:
	typedef ListNode Node;

private:
	Node* m_head;
	Node** m_tail;

public:
	Queue()
		: m_head(NULL), m_tail(&m_head)
	{
	}

	bool cerl_call empty() const
	{
		return m_head == NULL;
	}

	void cerl_call push(Node* val)
	{
		*m_tail = val;
		m_tail = &val->prev;
		*m_tail = NULL;
	}
	
	Node* cerl_call top()
	{
		return m_head;
	}

	Node* cerl_call pop()
	{
		Node* val = m_head;
		if (val)
		{
			m_head = val->prev;
			CERL_DBG_ERASE_LINK(val);
			if (m_head == NULL)
				m_tail = &m_head;
		}
		return val;
	}

	Node* cerl_call clear()
	{
		Node* head = m_head;
		m_head = NULL;
		m_tail = &m_head;
		return head;
	}
};

// =========================================================================
// class DListNode, DList

typedef NS_BOOST_MEMORY::dcl_list_node_base DListNode;
typedef NS_BOOST_MEMORY::dcl_list<DListNode> DList;

// =========================================================================

NS_CERL_END

#endif
