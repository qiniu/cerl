/*
@encoding utf8
@ns cerl
@*/

/**	消息队列。所谓消息队列，本质上就是一个多线程安全的、符合生产者-消费者模型的队列。
@*/
class MessageQueue
{
	/** MessageQueue消息队列中的一个节点类型。
	@*/
	typedef ListNode Node;
	
	/**	向消息队列push一个元素。
	@arg [in] val
		要push的对象。
	@remark
		注意该对象必须没有加入到队列。将同一个对象push两次到队列将发生不可预期的结果。
	@*/
	void cerl_call push(Node* val);

	/**	从消息队列pop一个元素。如果没有可以pop的元素，那么将等待。
	@return
		返回一个指针，该指针指向从消息队列中pop出的元素。
	@*/
	Node* cerl_call pop();
};

/** 纤程消息队列类。
remark
	本类的作用是
@*/
class FiberMessageQueue
{
	
	FiberMessageQueue();
	
	// push val;
	// if waiting list is not empty: wakeup a fiber;
	//
	/** 往队列加入消息。
	@remark
		如果有其他纤程正在等待中，则唤醒一个等待中的纤程。
	@arg [in] self
		当前纤程对象。
	@arg [in] val
		传入队列需要传递的信息对象。
	@*/
	void cerl_call push(Fiber self, Node* val);

	// while message queue is empty && m_fWorking: push self into waiting list;
	// return the top element if has, or return NULL;
	//
	/** 如果队列为空，则把当前纤程加入等待队列，并yield() 释放 cpu 。
	@remark
		当前纤程会在push()中被唤醒。
	@arg [in] self 
		当前纤程。
	@return
		消息队列传送的消息信息对象。
	@*/
	Node* cerl_call pop(Fiber self);

	// if message queue is empty: return NULL;
	// else: pop & return the top element;
	//
	/** 返回当前队列的头节点信息。
	@return
		@val non NULL 有效信息。
		@val NULL 当前队列空。
	@*/
	Node* cerl_call try_pop();
	
	// set m_fWorking = FALSE & wake up all fibers in waiting list.
	//
	/** 把消息队列工作状态设置成 FALSE，唤醒等待在队列上的纤程。
	@*/
	void cerl_call close();
};
