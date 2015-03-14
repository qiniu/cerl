/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// macro CerlIoService

/** 获得纤程的 FiberData 指针。不建议直接使用该宏。
@arg fiber
	一个\<cerl::Fiber>对象句柄。
@return
	返回纤程的 FiberData 指针。
@*/
#define CerlFiberData(fiber)

/** 获得纤程的序号。内部使用，不建议直接使用该宏。
@arg fiber
	一个\<cerl::Fiber>对象句柄。
@return
	返回该 Fiber 的序号。
@*/
#define CerlFiberSerialNo(fiber)

/** 获得纤程的 IoService 对象。
@arg fiber
	一个\<Fibers>的句柄。
@return
	返回 Fiber 的 \<IoService> 对象。
@*/
#define CerlIoService(fiber)

/** 获取纤程的 IoService 对象。
@arg lpParam
	\<Fibers> 参数指针。
@return
	返回 Fiber 的 \<IoService> 对象。
@*/
#define CerlFiberParamService(lpParam)

/** 获得为 Fiber 分配内存的内存分配器。
@arg fiber
	一个 \<Fibers> 的句柄。
@return
	返回 Fiber 使用的内存分配器。
@*/
#define CerlFiberAllocator(fiber)

/** 获取纤程所使用的内存池集合。
@arg fiber
	一个 \<Fibers> 的句柄。
@return
	返回该纤程所使用的内存池集合。
@see Pools
@*/
#define CerlMemPools(fiber)

/** 获取 Fiber 的 Log。
@arg fiber
	一个 \<Fibers> 的句柄。
@return
	返回该纤程的 Log。
@*/
#define CerlDefaultLog(fiber)

/** 获取纤程的 Timeout 值。
@arg fiber
	一个 \<Fibers> 的句柄。
@return
	返回该纤程的 Timeout 值。
@*/
#define CerlIoTimeout(fiber)


// -------------------------------------------------------------------------
// class IoService

/** IoService是为实现I/O操作而设计的，实现的主要功能包括：I/O管理、Task管理和Fiber调度
	其中：Task，任务，是用来处理计算消耗型任务或者不可间断性I/O事件（如DB操作），
	IoService提供的Task管理的工作就是负责向TaskPool投递任务消息。
	使用Task的目的就是为了提高请求的吞吐量，减少请求的阻塞。
	Fiber，纤程，用来处理I/O操作（包括文件I/O和socket I/O），
	IoService提供的Fiber调度就是执行纤程执行权的切换。
	纤程拥有三个状态：运行状态、等待状态、阻塞状态。
	运行状态：该纤程目前正持有执行权。
	等待状态：该纤程执行的消息已经被post到IoService的消息队列中，即将调度执行。
	阻塞状态：该纤程执行了yield，将执行权移交，等待唤醒。
	使用Fiber的目的是为了通过纤程执行权的切换确保CPU的效率。
	此外，IoService还维护了I/O完成消息队列，存放I/O完成消息，并协助纤程的调度。
@see cerl::TaskPool, cerl::Fibers, cerl::IoCompletionPort
@*/
class IoService
{
	BlockPool recycle;
	Pools pools;
	Fiber self; // 主Fiber句柄

	/** IoService初始化。
	@remark
		分配并初始化与 Fiber 相关的对象。
	@*/
	IoService();

	/** 构造一条消息。
	@arg [in] fiber
		消息中指定的要通过调度获得执行权的纤程。
	@return
		返回一个打包的消息。
	@remark
	   messageOfSchedule在异步I/O操作的过程中，被IO操作的发起者调用，构造IO完成消息。
		异步操作的流程中涉及到messageOfSchedule的部分：
		使用messageOfSchedule构造一条消息==>执行异步操作==>异步操作完成时post这条消息。
		因此，messageOfSchedule其实是WorkFiber提前构造好的，表示异步操作已经完成的消息，
		但这条消息将在异步操作完成后才真正进行投递。之所以事先构造任务完成的消息的原因是：
		避免跨线程的内存分配与释放，可以使用单线程的内存池；避免malloc的重入问题。
	@*/
	ScheduleMessage cerl_call messageOfSchedule(Fiber fiber);

	/** 向消息队列post一个消息，该消息用于唤醒某个处于阻塞状态的纤程。
	@arg [in] fiber
		被唤醒的纤程。
	@remark
		scheduleFiber的应用场景：
		如果某个纤程曾经使用yield函数使自己阻塞，当前拥有执行权的纤程或线程如果发现
		被阻塞纤程的条件已经满足，那么可以使用scheduleFiber将阻塞的纤程唤醒。
	@see cerl::yield, cerl::schedule
	@*/
	void scheduleFiber(Fiber fiber);
	
	/** 向消息队列 post 一个任务，该任务由用户自定义的 run() 函数处理。
	@arg [in] task
		用户自定义的任务，该任务必须派生自 IoService::Task 类并继承实现 run() 函数。
	@arg [in] uToken
		默认值为0。
	@remark
	@*/
	void cerl_mtcall postTask(Task* task, DWORD uToken = 0);
	
	/** 向消息队列post一个退出IoService::run循环的消息。
	@remark
		quit消息一旦被执行，IoService::run循环将会立即结束，即使消息队列中还有其他的消息没有处理。
	@*/
	void quit();
	
	/** 唤醒一个纤程去执行。
	@arg [in] sn
		纤程的序号。
	@arg [in] fiberToWakeup
		需求唤醒来执行的纤程。
	@remark
		resumeFiber 确保 fiberToWakeup 序号和 sn 一致，否则不唤醒纤程。
	@*/
	void cerl_call resumeFiber(FiberSerialNo sn, Fiber fiberToWakeup);
	
	/**	根据输入的参数创建一个新的工作者纤程，将当前拥有执行权的纤程放入队列中等待下一次调度，
		并把执行权切换到新创建的纤程。
	@arg [in] self
		当前拥有执行权的纤程。
	@arg [in] lpStartAddress
		新创建纤程的回调函数的地址。
	@arg [in] startParam
		新创建纤程携带的参数。
	@arg [in] dwStackSize
		为新创建纤程设置栈，如果设置为0，采用默认大小。
	@return
		返回一个指向新创建纤程的指针。
	@remark
		startFiber的应用场景：
		如果纤程要对某个特定事件的发生进行处理，可以调度startFiber构造一个纤程来处理事件，
		典型情况就是网络服务器在Accept一个连接请求后，调用startFiber来处理这个网络连接。
	@see cerl::startFiber
	@*/
	Fiber cerl_call startFiber(Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0);

	/** 向消息队列post一个销毁当前的纤程的消息，之后把执行权切换到指定的纤程。
	@arg [in] self
		当前拥有执行权的纤程。
	@arg [in] fiberSwitchTo
		要通过调度获得执行权的纤程。
	@remark
		exitFiberAndSwitch的应用场景：
		典型的情况就是一个纤程等待另外一个纤程执行完毕之后才能够继续执行，
		当被等待的纤程执行完毕后，调用此函数将会转移执行权到等待的纤程。
	@*/
	void cerl_call exitFiberAndSwitch(Fiber self, Fiber fiberSwitchTo);

	/**	向消息队列post一个销毁当前的纤程的消息，并把执行权切换到schedule fiber。
	@arg [in] self
		当前拥有执行权的fiber。
	@remark
		调用 exitFiberAndSwitch 函数，切换到当前 IoService 所持有的纤程。
	@*/
	void cerl_call exitFiber(Fiber self);
	
	/**	当前纤程放弃执行权进入阻塞状态。
	@arg [in] self
		当前拥有执行权的纤程。
	@remark
		yield应用场景：
		当纤程必须等待某条件的满足才能继续执行时，需要执行yield交出执行权，进入阻塞状态。
		典型的情况就是当前纤程在发送异步IO操作请求后，调用yield等待IO操作的完成。
	@see cerl::yield, cerl::schedule
	@*/
	void cerl_call yield(Fiber self);

	/** run函数主要负责IoService的调度。她在获得执行权后将从IoService消息队列中取出消息，
		如果没有消息则等待；如果有消息要处理，则根据消息的不同类型来处理消息，
		在处理完消息后继续从消息队列取消息并调度执行；直到\<quit>函数被调用时run函数才退出。
	@remark
		运行run函数将启动IoService的管理和调度功能，即IoService开始工作。
		需要注意的是，run函数不能单独使用，在run调用之前需要事先构造好纤程并向消息队列post消息，
		否则run函数将会一直从空队列中取消息。用户通常可以使用带有参数的run来启动IoService。
	@see run(lpStartAddress,startParam,dwStackSize)
	@*/
	void run();

	/**	和\<run>函数功能基本相同，不同的是：该函数会根据输入的参数来创建纤程，
		并将该纤程的调度消息post到消息队列中，这样，IoService在运行后就能立即调度执行该纤程。
	@brief
	@arg [in] lpStartAddress
		纤程的回调函数地址。
	@arg [in] startParam
		纤程携带的参数。
	@arg [in] dwStackSize
		设置纤程的栈大小，如果设置为0，采用默认大小。
	@see run()
	@*/
	void run(FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0);
	
	/** Session 接口，startSession 时需要传入 Session 模板对象。
	@*/
//	struct Session{
//		void cerl_call run(Fiber self) const;
//	};
	
	/** 启动一个 Session，在 Session 中只有一个 IoService 模式中调用 Session 对象的 run() 函数。
	@remark
		单 IoService 的情况下 startSession 的作用就是 startFiber。
	@arg self
		当前纤程对象。
	@arg session
		SessionT 模板类型对象。
	@arg dwStackSize
		创建 Fiber 时的栈大小，缺省为0。
	@*/
	template <class SessionT>
	void cerl_call startSession(Fiber self, const SessionT& session, size_t dwStackSize = 0);

	/** IoServicePool 初始化的时候会调用本函数，在Session中只有一个 IoService 模式中什么都不用做。
	@*/
	void cerl_call start(size_t nThread = 1);
};

/**	根据输入的参数创建一个新的工作者纤程，将当前拥有执行权的纤程放入队列中等待下一次调度，
	并把执行权切换到新创建的纤程。
@arg [in] self
	当前拥有执行权的纤程。
@arg [in] lpStartAddress
	新创建纤程的回调函数的地址。
@arg [in] startParam
	新创建纤程携带的参数。
@arg [in] dwStackSize
	为新创建纤程设置栈，如果设置为0，采用默认大小。
@return
	返回一个指向新创建纤程的指针。
@remark
	startFiber的应用场景：
	如果纤程要对某个特定事件的发生进行处理，可以调度startFiber构造一个纤程来处理事件，
	典型情况就是网络服务器在Accept一个连接请求后，调用startFiber来处理这个网络连接。
@*/
Fiber cerl_call startFiber(Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0);

/**	向消息队列post一个销毁纤程的消息，并把执行权切换到schedule fiber。
@arg [in] self
	一个\<Fiber>句柄。
@*/
void cerl_call exitFiber(Fiber self);

/**	当前纤程放弃执行权进入阻塞状态。
@arg [in] self
	当前拥有执行权的纤程。
@remark
	yield应用场景：
	当纤程必须等待某条件的满足才能继续执行时，需要执行yield交出执行权，进入阻塞状态。
	典型的情况就是当前纤程在发送异步IO操作请求后，调用yield等待IO操作的完成。
@see cerl::schedule
@*/
void cerl_call yield(Fiber self);

/** 当前纤程临时放弃执行权（但并没有进入阻塞状态），等待下一次调度。
@arg [in] self
	当前拥有执行权的纤程。
@remark
	schedule一般发生在一个纤程执行一个花费时间较长的运算时调用(占用过长的处理时间)，
	以主动出让执行权，等待下一次调度。schedule和yield都放弃执行权，但是两者不同之处在于
	schedule并不等待某种条件的满足，只要有机会它仍然参与调度。但是yield的纤程在等待的条
	件没有达成时，是不会参与纤程调度的。
@see cerl::yield
@*/
void cerl_call schedule(Fiber self);

// -------------------------------------------------------------------------
// class FiberParam

/** FiberParam描述了纤程及其携带的参数，FiberParam在纤程回调函数中使用，其主要工作包括：
	1.提供纤程携带的参数；2.提供当前纤程；3.在FiberParam析构的时候提供纤程的退出清理工作，
	将执行权由当前纤程切换至Schedule Fiber，并将纤程销毁。
@*/
class FiberParam
{
	//@members
	Fiber const self;
	void* const val;
	IoService* const service;
	//@end

	/** FiberParam初始化：设定纤程以及纤程携带的参数。
	@arg [in] lpParam
		纤程回调函数携带的参数。
	@*/
	FiberParam(LPVOID lpParam);
};
