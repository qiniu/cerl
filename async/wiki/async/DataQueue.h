/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class DataQueue

/** DataQueue 类用于多个 Fiber 之间共享一个带缓存的写数据的通道。
@arg FileObjectT
	任何符合 concept FileObject（文件对象）的类。如：FileObject, SocketFileObject, etc。
@remark
	提供与 DataQueue 类似功能的类有 Shared，不过 Shared 并不带写缓存。DataQueue 与
	Shared 的共享机制也不同，Shared 通过在写操作前后加锁实现，而 DataQueue 是确保写入
	一个 Message 完整写入缓存后才进行真正的 IO 操作，从而避免了锁的使用。对于 Shared，
	写入消息的流程（以SharedFile为例）如下：
	[[code]]
		SharedFile shared;
		...
		{
			shared.acquire(self);
			shared.write(...);
			shared.write(...);
			shared.release();
		}
		// 或者：
		{
			SharedFile::scoped_lock lock(self, shared);
			shared.write(...);
			shared.write(...);
		}
	[[/code]]
	对于DataQueue，写入消息的流程是这样的：
	[[code]]
		DataQueue<FileObject> dq;
		...
		{
			dq.put(...);
			dq.put(...);
			dq.operate(self);
		}
	[[/code]]
@see cerl::Shared, cerl::FileObject, cerl::SocketFileObject
@*/
template <class FileObjectT>
class DataQueue
{
	/** 构造一个DataQueue。
	@arg [in] file
		要进行写操作的文件对象。注意 DataQueue 对该文件对象的写操作是独占的。也就是说，
		不允许任何其他人调用该文件对象的写操作(write_some)。所以一个直观的推论是，在同
		一个文件对象上不允许创建多个DataQueue（当然这只是限制之一而不是全部）。
	@arg [in] pool
		缓存的内存分配器。
	@*/
	DataQueue(FileObjectT& file, Pool& pool);

	/** 向 DataQueue 中写入一块数据。该操作只是将数据放入缓存，并不涉及 IO 操作。
	@arg [in] buf, bytes
		要写入的数据块。数据块大小是以 bytes（字节）为单位。
	@return
		成功写入的字节数。
	@remark
		由于 put 操作并不引起 IO，所以一块数据分为多次 put 操作写入是安全的，并不会出现被
		其他 Fiber 截断的情况。当然前提是这些 put 操作之间没有调用任何可能引起 Fiber 切换
		的操作。
	@*/
	size_type cerl_call put(const char* buf, size_type bytes);

	/** 向 DataQueue 发起进行 IO 操作的请求。该函数将执行权让给 DataQueue 的服务器纤程，由
		该服务器纤程执行 IO 操作。
	@arg [in] self
		发起该 IO 操作请求的Fiber对象。
	@return
		@val true
			成功。
		@val false
			IO 操作请求失败。DataQueue的服务器没有启动或者已经因为Socket连接关闭而被动结束。
			要启动DataQueue服务器，请调用 DataQueue::start 函数。
	@*/
	bool cerl_call operate(Fiber self);

	/** 向 DataQueue 中写入一条消息（Message）。写入的消息不会中途被其他Fiber的IO操作截断。
		该函数只是一个辅助函数。内部实际上就是一个 put + operate 操作。
	@arg [in] self
		写入该消息（Message）的Fiber对象。
	@arg [in] msgBuf, msgSize
		要写入的消息。
	@return
		@val true
			成功。
		@val false
			写操作失败。DataQueue的服务器没有启动或者已经因为Socket连接关闭而被动结束。
			要启动DataQueue服务器，请调用 DataQueue::start 函数。
	@remark
		一次 pug_message 写入的消息不会中途被其他Fiber的IO操作截断。但是需要注意的是，多
		次 pug_message 之间并无此保证。
	@see Shared::pug_message
	@*/
	bool cerl_call put_message(Fiber self, const char* msgBuf, size_t msgSize);
	
	/** 启动 DataQueue 服务器。
	@arg [in] self
		调用者。即启动 DataQueue 服务器的纤程。
	@arg [in] dwStackSize
		DataQueue 服务器纤程的栈大小。如果为0则表示取内部默认的栈大小。
	@*/
	void cerl_call start(Fiber self, size_t dwStackSize = 0);
};

// -------------------------------------------------------------------------
