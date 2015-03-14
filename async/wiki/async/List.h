/*
@encoding utf8
@ns cerl
@*/

#define CERL_DBG_ERASE_LINK(p)	

// -------------------------------------------------------------------------
// struct ListNode

/** 链表节点。
@*/
struct ListNode
{
	ListNode* prev;
};

// -------------------------------------------------------------------------
// function reverse

/** 倒置链表。
@arg [in] lst
	要倒置的链表头对象。
@arg [in] head
	要返回的链表头对象。
@return
	倒置以后的链表头对象。
@*/
inline ListNode* cerl_call reverse(ListNode* lst, ListNode* head = NULL);

// -------------------------------------------------------------------------
// class List

/** 链表类。
@*/

class List
{
public:
	typedef ListNode Node;

private:
	Node* m_head;

public:
	/** 链表初始化。
	@*/
	List() : m_head(NULL);
	
	/** 检查链表是否为空。
	@return
		@val true 链表空。
		@val false 链表非空。
	@*/
	bool cerl_call empty() const;

	/** 插入新节点到表头。
	@arg [in] val
		要插入的节点对象。
	@*/
	void cerl_call push(Node* val);

	/** 弹出链表头节点。
	@return 
		返回头节点对象。
	@*/
	Node* cerl_call pop();
};

/** 提供 Stack 的功能。
@*/
typedef List Stack;

// -------------------------------------------------------------------------
// class Queue

/** 队列类。
@*/
class Queue
{
public:
	typedef ListNode Node;

private:
	Node* m_head;
	Node** m_tail;

public:
	/** 队列初始化。
	@*/
	Queue()；

	/** 检查队列是否为空。
	@return
		@val true 队列空。
		@val false 队列非空。
	@*/
	bool cerl_call empty() const；
	
	/** 插入新节点到队列尾。
	@arg [in] val
		要插入的节点指针。
	@*/
	void cerl_call push(Node* val)；

	/** 弹出队列头节点。
	@return 
		返回头节点指针。
	@*/
	Node* cerl_call pop()；

	/** 清空队列。
	@return
		返回原始队列头指针。
	@*/
	Node* cerl_call clear()；
};

// =========================================================================
// class DListNode, DList

typedef NS_BOOST_MEMORY::dcl_list_node_base DListNode;
typedef NS_BOOST_MEMORY::dcl_list<DListNode> DList;

