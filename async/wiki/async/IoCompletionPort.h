/*
@encoding utf8
@ns cerl
@*/

/**	I/O完成端口提供了一个I/O完成消息队列，以及向该消息队列发送消息和取出消息的方法。
@remark
	使用场景，在IoService对象的 scheduleFiber，postUserMsg，quit等成员函数被调用时，往I/O完成端口的
	消息队列中加入消息。而在IoService 对象的 run 成员函数中读出队列中的消息并处理。
@see cerl::IoService
@*/
class IoCompletionPort
{
	/** I/O完成端口初始化。
	@arg [in] pools
		使用Pools为消息分配内存。
	@remark
		I/O完成端口将在\<IoService>初始化时被调用。
	@*/
	IoCompletionPort(Pools& pools);

	/** 向I/O完成端口传送的消息，定义了包括IO完成的字节数、I/O操作的key及overlapped指针。
	@*/
	class Message{};

	/** 向IoService的消息队列发送一个\<Message>。
	@arg [in] bytes
		I/O完成字节数。
	@arg [in] key
		I/O完成端口key。
	@arg [in] overlapped
		overlapped数据结构指针，通常是一个纤程。
	@*/
	void cerl_call post(DWORD bytes, ULONG_PTR key, LPOVERLAPPED overlapped);

	/** 向IoService的消息队列发送一个\<Message>。
	@arg [in] msg
		指向\<Message>的指针。
	@*/
	void cerl_call post(const Message& msg);
};
