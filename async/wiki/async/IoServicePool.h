/*
@encoding utf8
@ns cerl
@*/

// -----------------------------------------------------------------------
// class MultiIoService

/** MultiIoService 类，如使用本类来实例化 IoServicePool，则在该 Pool 中将有多个
	IoService，并且多个 IoService 共用一个 Session。
@*/
class MultiIoService
{
	/** 初始化，开始时线程数为0。
	@*/
	MultiIoService();
	
	/** 启动 Session。
	@remark
		加入 IoServicePool 以后，以前 startFiber 的时候现在要改成 startSession，
		典型情况为一个服务器接收到连接请求以后，以前是调用 startFiber 去处理请
		求，现在要改成 startSession。
	@arg SessionT
		模板参数，表示 Session 类，该类必须重载 IoService::Task 的run(IoService*, DWORD) 成员函数。
	@arg self
		当前控制权纤程。
	@arg session
		一个const SessionT& 对象，这里是 const SessionT& 原因是，Session 对象是跨线程传递的，
		到另一个线程时，该对象会被复制。
	@arg dwStackSize
		创建 Fiber 时的栈大小，缺省为0。
	@*/
	template <class SessionT>
	void cerl_call startSession(Fiber self, const SessionT& session, size_t dwStackSize = 0);
	
	/** 启动多个线程，每个线程对应一个 IoService。
	@arg nThread
		线程数目。
	@*/
	void cerl_call start(size_t nThread = 2);

	/** MultiIoService 的退出。
		退出所有 IoService。
	@*/
	void cerl_call quit();

	/** 退出所有线程的 IoService。
	@*/
	void cerl_call run();

	/** 对应于 IoService 的run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0),
		与 IoService 的不同在于 IoService 是在一个线程中创建纤程并循环等待纤程消息队列中消息，而本函数是把创建
		纤程的请求扔到 start() 中创建的线程对于的 IoService 队列中。
	@arg [in] lpStartAddress
		纤程的回调函数地址。
	@arg [in] startParam
		纤程携带的参数。
	@arg [in] dwStackSize
		设置纤程的栈大小，如果设置为0，采用默认大小。
	@*/
	void cerl_call run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0);
};

// -------------------------------------------------------------------------
// class IoServicePool

typedef  IoServicePool;

// -------------------------------------------------------------------------
// class IoServicePoolInit

/** 对 IoServicePool 的封装，只需要定义一个 IoServicePoolInit 对象就自动
	初始化全局的 IoServicePool 对象。
@*/
class IoServicePoolInit
{
	/** 构造函数。
	@arg threadCount
		需要在 IoServicePool 里面启动的线程数，每个线程中有一个 IoService 实例。
	@*/
	IoServicePoolInit(size_t threadCount = 2);
};

// -------------------------------------------------------------------------
