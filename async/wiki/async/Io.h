/*
@encoding utf8
@ns cerl
@*/

/** Aio的初始化，由AioInit的构造函数调用。
@see cerl::AioInit
@*/
void cerl_call AioInitialize();

/** 结束Aio，由AioInit的析构函数调用。
@see cerl::AioInit
@*/
void cerl_call AioUninitialize();

/** 为使用异步操作文件对象提供了初始化方法以及在异步IO操作后的清理工作。
@remark
	对文件对象的异步IO操作是在Async库底层实现的，用户使用的是封装好的同步IO操作方法，
	因此将感觉不到异步操作过程的存在。
@see cerl::AioInitialize,cerl::AioUninitialize
@*/
class AioInit{};

/** FileOverlapped提供了FileIO操作的上下文，FileObject在读写文件时必须指定FileOverlapped。
@see cerl::FileObject
@*/
class FileOverlapped
{
	//@members
	typedef offset_type;	//文件偏移量类型
	//@end

	/** 初始化FileOverlapped。
	@arg [in] fiber
		在初始化中设定对该文件对象进行操作的纤程。
	@*/
	FileOverlapped(Fiber fiber);

	/** 把对文件对象进行操作的纤程设置为当前纤程。
	@arg [in] self
		当前拥有执行权的纤程。
	@*/
	void cerl_call setFiber(Fiber self);

	/** 将当前文件位置向后移动指定的距离。其中，文件位置是距文件起始处的字节偏移量。
	@arg [in] delta
		指定以字节为单位的移动距离。
	@remark
		move的应用场景：
		一个典型的应用就是在异步读写文件的read_some和write_some函数中使用。
		由于 read_some 和 write_some 不保证一次完成读写任务，
		因此 read_some 和 write_some 在执行一次读写任务后
		就需要调用 move 函数移动文件指针到下次读写的位置。
	@see read_some, write_some
	@*/
	void cerl_call move(offset_type delta);

	/** 将文件位置移动到指定位置。其中，文件位置是距文件起始处的字节偏移量。
	@arg [in] offset
		文件要移动到的位置。
	@*/
	void cerl_call seek(offset_type offset);

	/** 返回当前文件位置。其中，文件位置是距文件起始处的字节偏移量。
	@return
		当前文件的位置。
	@*/
	offset_type cerl_call tell() const;
};

/** FileObject提供了对文件对象的操作方法，这包括：打开、关闭、读、写文件I/O。
	FileObject可以认为是不带上下文的File，因此在读写操作时必须指定FileOverlapped。
@see cerl::FileOverlapped
@*/
class FileObject
{
	//@members
	typedef handle_type;	//句柄类型
	//@end

	/**	初始化文件句柄。
	@*/
	FileObject();

	/** 判断是否已经打开了文件句柄。
	@return
		@val True
			文件句柄已经打开，返回True。
		@val False
			文件句柄还没有打开，返回False。
	@remark
		如果需要在进行文件操作前确认文件是否已经成功打开，则调用 good 。
	@*/
	bool cerl_call good() const;
	
	/** 将文件句柄设为无效。
	@return
		返回文件在detach前的句柄。
	@*/
	HANDLE cerl_call detach();

	/** 设置要进行I/O操作的文件句柄。
	@arg [in] self
		对文件对象进行操作的纤程。
	@arg [in] hFile
		文件句柄。
	@return
		@val S_OK
			open_handle成功。
		@val E_ACCESSDENIED
			open_handle失败。
	@*/
	HRESULT cerl_call open_handle(Fiber self, HANDLE hFile);

	/** 以可读写方式打开文件。
	@arg [in] self
		对文件对象进行操作的纤程。
	@arg [in] file
		指向文件名的指针。
	@return
		@val S_OK
			打开文件成功。
		@val E_ACCESSDENIED
			打开文件失败。
	@*/
	HRESULT cerl_call open_to_write(Fiber self, const char* file);

	/** 以只读方式打开文件。
	@arg [in] self
		对文件对象进行操作的纤程。
	@arg [in] file
		指向文件名的指针。
	@return
		@val S_OK
			打开文件成功。
		@val E_ACCESSDENIED
			打开文件失败。
	@*/
	HRESULT cerl_call open_to_read(Fiber self, const char* file);

	/** 关闭已经打开的文件句柄，设置文件句柄为无效。
	@*/
	void cerl_call close();

	/** 从文件对象读出数据，保存在指定的缓冲区中。
	@arg [in] o
		用于保存文件对象相关参数的模板参数OverlappedT。
	@arg [out] buf
		存储读出数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功读出的字节数。
	@remark
		read_some成功读出的字节数可能小于指定的缓冲区大小，
		此时文件指针将向后移动已经读出的字节数的距离。
		如果要求读出的字节书和指定的缓冲区大小相同，请使用\<FileImpl::read>。
	@see FileImpl::read
	@*/
	template <class OverlappedT>
	size_t cerl_call read_some(OverlappedT& o, void* buf, size_t cb);

	/**	向文件对象写入数据，要写入的数据保存在指定的缓冲区中。
	@arg [in] o
		用于保存文件对象相关参数的模板参数OverlappedT。
	@arg [out] buf
		存储要写入数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功读出的字节数。
	@remark
	   read_some成功写入的字节数可能小于指定的缓冲区大小，
		此时文件指针将向后移动已经写入的字节数的距离。
		如果要求写入的字节书和指定的缓冲区大小相同，请使用\<FileImpl::write>。
	@see FileImpl::write
	@*/
	template <class OverlappedT>
	size_t cerl_call write_some(OverlappedT& o, const void* buf, size_t cb);
};

// -------------------------------------------------------------------------
// class SocketOverlapped

/** SocketOverlapped提供了Socket IO操作的上下文，SocketFileObject在读写Socket IO时必须指定SocketOverlapped。
@see cerl::SocketFileObject
@*/
class SocketOverlapped
{
	/**	初始化FileOverlapped。
	@arg [in] fiber
		对Socket对象进行操作的纤程。
	@*/
	SocketOverlapped(Fiber fiber);
	
	/**	把对Socket对象进行操作的纤程设置为当前纤程。
	@arg [in] self
		当前拥有执行权的纤程。
	@*/
	void cerl_call setFiber(Fiber self);
	
	/** 获得 SocketOverlapped 对象的纤程。
	@return 
		当前 SocketOverlapped 对象所持有的纤程对象。
	@*/
	Fiber cerl_call getFiber() const;
};

/**	SocketFileObject 提供了基于纤程的打开、关闭、读、写Socket I/O的方法。
	在对Socket文件对象进行读写操作时，必须指定一个SocketOverlapped。
@see cerl::SocketOverlapped
@*/
class SocketFileObject
{

	/**	初始化Socket文件对象。
	@remark
		缺省的 timeout 时间设置为 CERL_DEFAULT_TIMEOUT。
	@*/
	SocketFileObject();
	
	/** 析构函数，关闭 socket。
	@*/
	~SocketFileObject();
	
	/** 把已打开的 Socket 对象绑定在 SocketFileObject 对象上。
	@arg [in] self
		处理该 SOCKET 上事件的纤程。
	@arg [in] s
		已打开的 SOCKET 对象。
	@return
		@val S_OK 
			绑定成功。
		@val E_ACCESDENIED 
			未绑定成功。
	@*/
	HRESULT cerl_call open_handle(Fiber self, SOCKET s);
	
	/** 返回已绑定在 SocketFileObject 对象上的 Socket 对象。
	@return
		已绑定的 Socket 对象。
	@*/
	SOCKET cerl_call get_handle() const;
	
	/** 判断 SocketFileObject 对象中是否持有有效的 Socket 对象。
	@return bool
		@val true 
			持有有效 Socket 对象。
		@val false 
			未持有有效 Socket 对象。
	@*/
	bool cerl_call good() const;

	/**	关闭已经打开的socket。
	@*/
	void cerl_call close();

	/**	关闭已经打开的socket，同时打印出警告信息。
	@arg [out] op
		指向警告信息字符串的指针。
	@*/
	void cerl_call dbg_close(const char* op);

	/** 设置读的超时时间。
	@arg to
		想要设置的超时时间。
	@*/
	void cerl_call set_read_timeout(Timeout to);

	/** 返回当前读超时时间。
	@return
		读超时时间。
	@*/
	Timeout cerl_call get_read_timeout() const;

	/** 设置写的超时时间。
	@arg to
		想要设置的超时时间。
	@*/
	void cerl_call set_write_timeout(Timeout to);
	
	/** 返回当前写超时时间。
	@return
		写超时时间。
	@*/
	Timeout cerl_call get_write_timeout() const;

	
	/**	从Socket文件对象读出数据，保存在指定的缓冲区中。
	@arg [in] o
		用于保存socket文件对象相关参数的 SocketOverlapped 对象。
	@arg [out] buf
		存储读出数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功读出的字节数。
	@remark
		read_some成功读出的字节数可能小于指定的缓冲区大小。
		如果要求读出的字节书和指定的缓冲区大小相同，可以使用\<FileImpl::read>来完成。
	@see FileImpl::read
	@*/
	size_t cerl_call read_some(SocketOverlapped& o, void* buf, size_t cb);

	/**	向Socket文件对象写入数据，要写入的数据保存在指定的缓冲区中。
	@arg [in] o
		用于保存socket文件对象相关参数的 SocketOverlapped 对象。
	@arg [in] buf
		存储要写入数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功写入的字节数。
	@remark
		write_some成功写入的字节数可能小于指定的缓冲区大小。
		如果要求读出的字节书和指定的缓冲区大小相同，可以使用FileImpl::write来完成。
	@see FileImpl::write
	@*/
	size_t cerl_call write_some(SocketOverlapped& o, const void* buf, size_t cb);
};

/**	ListenSocket提供了一个异步的accept方法。
@*/
class ListenSocket
{
	/**	初始化ListenSocket套接字。
	@*/
	ListenSocket();
	
	/** 析构函数，关闭 socket 。
	@*/
	~ListenSocket();
	
	/** 判断正在监听的 socket 是否有效。
	@return
		@val true
			有效 socket。
		@val false
			无效 socket。
	@*/
	bool cerl_call good();
	
	/** 关闭 socket。
	@remark
		关闭之前必须唤醒等待在接受队列上的所有纤程。
	@*/
	void cerl_call close();

	/**	设置要监听的Socket套接字。
	@arg [in] self
		当前对Socket文件对象进行操作的纤程。
	@arg [in] s
		一个Socket套接字。
	@return
		@val S_OK
			open_handle成功。
		@val E_ACCESSDENIED
			open_handle失败。
	@*/
	HRESULT cerl_call open_handle(Fiber self, SOCKET s);

	/**	接受请求的Socket连接。
	@arg [in] self
		当前的纤程。
	@return
		返回已经建立好连接的Socket。
	@*/
	SOCKET cerl_call accept(Fiber self);
};

/**	FileImpl继承自FileObjectT和OverlappedT，实现了对文件IO和对SocketIO的读写操作。
	由于Fileobject和SocketFileObject提供的read_some()/write_some()函数并不能保证一次IO操作即可取到
	或送出所需的所有数据。因此，FileImpl额外定义了read()/write()，通过反复读取/写入，确保所有数据
	操作可以在一次调用中完成。即，FileImpl通过封装异步IO操作提供了对文件对象的伪同步读写操作方法。
@*/
template <class FileObjectT, class OverlappedT>
class FileImpl : public FileObjectT, public OverlappedT
{
	/**	FileImpl初始化。
	@arg [in] self
		对文件对象进行操作的纤程。
	@*/
	FileImpl(Fiber self);

	/**	读出文件对象的数据，保存在指定的缓冲区中。
	@arg [out] buf
		存储读出数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功读出的字节数。
	@remark
		read_some成功读出的字节数可能小于指定的缓冲区大小。
	@*/
	size_t cerl_call read_some(void* buf, size_t cb);

	/**	向文件对象写入数据，要写入的数据保存在指定的缓冲区中。
	@arg [in] buf
		存储要写入数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功写入的字节数。
	@remark
		write_some成功写入的字节数可能小于指定的缓冲区大小。
	@*/
	size_t cerl_call write_some(const void* buf, size_t cb);

	/**	从文件对象读出数据，保存在指定的缓冲区中。
	@arg [out] buf
		存储读出数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功读出的字节数。
	@remark
		与read_some不同，该函数能够一次读出缓冲区大小的数据。
	@*/
	size_t cerl_call read(void* buf, size_t cb);

	/**	向文件对象写入数据，要写入的数据保存在指定的缓冲区中。
	@arg [in] buf
		存储要写入数据的缓冲区。
	@arg [in] cb
		缓冲区的大小。
	@return
		返回成功写入的字节数。
	@remark
		与write_some不同，该函数能够一次写入缓冲区大小的数据。
	@*/
	size_t cerl_call write(void* buf, size_t cb);
};

//@type File
//typedef FileImpl<FileObject, FileOverlapped> File;

//@type SocketFile
//typedef FileImpl<SocketFileObject, SocketOverlapped> SocketFile;

/** 创建并连接 socket。
@arg [in] self
	当前纤程对象。
@arg [in] si
	sockaddr_in 对象。
@return
	返回连接成功的 socket 对象。
@*/
SOCKET cerl_call connectSocket(Fiber self, const SockaddrIn& si);

/** 创建并连接 socket。
@arg [in] self
	当前纤程对象。
@arg [in] hostname
	服务器主机名。
@arg [in] port
	端口号。
@*/
SOCKET cerl_call connectSocket(Fiber self, const char* hostname, Port port);

/** Shared 类用于多个 Fiber 之间共享一个文件对象。
@arg ResourceT
	任何符合 concept FileObject（文件对象）的类。如：FileObject, SocketFileObject, etc。
@remark
	提供与 Shared 类似功能的类有 DataQueue，不过 DataQueue 带写缓存。Shared 与 DataQueue 
	的共享机制也不同，Shared 通过在写操作前后加锁实现， DataQueue 则是确保写入一个
	Message 完整写入缓存后才进行真正的 IO 操作，从而避免了锁的使用。
@*/
template <class ResourceT>
class Shared : public ResourceT
{
	//@type scoped_lock
	typedef ScopedLock<Shared> scoped_lock;

	/**	Shared初始化。
	@*/
	Shared();

	/**	向 Shared 中写入一条消息（Message），在put_message执行前必须执行acquire操作，
		以保证写入的消息不会中途被其他Fiber的IO操作截断。
	@arg [in] self
		当前纤程。
	@arg [in] msgBuf
		存放消息的缓冲区。
	@arg [in] msgSize
		消息的大小。
	@return
		@val True
			如果消息全部写入，返回True。
		@val False
			如果消息未能全部写入，返回False。
	@remark
		在 pug_message 写入的消息之前需要执行 acquire 操作;
		当不再需要 pug_message 写入消息时，需要 release 释放锁。
	@*/
	bool cerl_call put_message(Fiber self, const char* msgBuf, size_t msgSize);

	/**	加锁。
	@arg [in] self
		当前纤程。
	@remark
		acquire 需要和 release 一起使用，在两条操作之间可以使用 put_message 向共享资源发送消息。
	@see release
	@*/
	void cerl_call acquire(Fiber self);

	/**	释放锁。
	@remark
		release 需要和 acquire 一起使用，在两条操作之间可以使用 put_message 向共享资源发送消息。
	@see acquire
	@*/
	void cerl_call release();
};

//@type SharedFile
//typedef Shared<File> SharedFile;
