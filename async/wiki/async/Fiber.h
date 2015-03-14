/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// type LPFIBER_START_ROUTINE

/** todo
@*/
typedef LPFIBER_START_ROUTINE;

// -------------------------------------------------------------------------
// macro CERL_FIBER_ASSERT_SELF

/** 断言一个Fiber句柄是否是CurrentFiber。
@arg self
	要断言的Fiber句柄。
@remark
	需要注意的是，在cerl/async library中，GetCurrentFiber等与CurrentFiber相关的API是不推荐使用的。
	但是我们的库会利用GetCurrentFiber来对用户代码的正确性进行诊断（只在Debug版本下生效）。
@*/
#define CERL_FIBER_ASSERT_SELF(self)

// -------------------------------------------------------------------------
// function switchToFiber

/** 把执行权从当前纤程转换到指定的纤程。
@arg [in] self
	当前占有执行权的纤程。
@arg [in] switchTo
	要通过切换获得执行权的纤程。
@remark
	switchToFiber调用时表示当前执行的纤程要主动放弃执行权，switchToFiber在以下场景中使用：
	1. 当需要等待某个条件发生的时候需要放弃执行权，将自己阻塞。即yield。
		yield就是通过switchToFiber来实现的，switchTo参数需要填写为schedule fiber。
	2. 如果一个纤程占用的CPU时间过长，可以通过switchToFiber将执行权转交给其他纤程。
		此时，通常需要和ScheduleFiber函数一起使用，确保该纤程可以再次被执行。
@*/
void cerl_call switchToFiber(Fiber self, Fiber switchTo);

// -------------------------------------------------------------------------
// class Fibers

/** Fibers 提供了纤程创建和销毁的方法。
	纤程是比线程粒度更小的运行单位，在线程空间中创建、运行。与线程不同，纤程需要手工切换。
	纤程拥有三个状态：运行状态、等待状态、阻塞状态。
	运行状态：该纤程目前正持有执行权。
	等待状态：该纤程执行的消息已经被post到IoService的调度队列中，即将调度执行。
	阻塞状态：该纤程执行了yield，将执行权移交，等待唤醒。
@*/
class Fibers
{
	/** 纤程的初始化，为创建纤程做准备。
	@arg [in] pools
		使用Pools为纤程分配内存空间。
	@*/
	Fibers(Pools& pools);

	/**	创建一个纤程，这个工作包括了设置栈大小，设置执行函数的起始地址，设置纤程的参数。
		若要销毁这个纤程，需要调用destroy函数。
	@arg [in] lpStartAddress
		指向新创建的纤程执行函数地址。
	@arg [in] lpParameter
		向纤程传递的参数。
	@arg [in] dwStackSize
		设置栈的大小。如果为0，那么将使用默认大小（Windows平台为1MB，Linux平台为4KB）。
	@return
		如果创建纤程成功，则将返回纤程的地址；
		如果创建纤程失败，则将返回NULL。
	@remark
		create只是创建纤程，并不会调度这个纤程。如果需执行，则要调用SwitchToFiber。
		create和SwitchToFiber被一起封装在IoService::startFiber中。
		因此，当用户需要创建一个纤程，并要求立即执行时，可以调用IoService::startFiber，
		或者选择创建一个Future。此外，一个线程可以开启的纤程数量由可用的虚拟内存来决定。
	@see destroy, cerl::startFiber
	@*/
	Fiber cerl_call create(
		LPFIBER_START_ROUTINE lpStartAddress,
		LPVOID lpParameter = NULL, size_t dwStackSize = 0);

	/**	销毁一个纤程。
	@arg [in] lpFiber
		一个指针，指向要销毁的纤程。
	@remark
		destroy将会销毁所有与纤程相关的数据。
	@*/
	void cerl_call destroy(Fiber lpFiber);
};

// -------------------------------------------------------------------------
// class ThreadToFiber

/**	ThreadToFiber类完成的工作包括：把当前的线程转换为纤程，以及把纤程转换为线程。
@remark
	在线程中如果要使用纤程来执行操作，必须要把当前线程转换为纤程。
@*/
class ThreadToFiber
{
	/**	把当前的线程转换为纤程。
	@arg [in] lpParameter
		向纤程传送的参数。
	@return
		如果成功，返回指向纤程的指针；
		如果失败，则将返回NULL。
	@remark
		convert在IoService的构造函数中被调用。
	@see cerl::IoService
	@*/
	Fiber cerl_call convert(LPVOID lpParameter = NULL);

	/**	把当前纤程转换为线程。
	@*/
	void cerl_call unconvert();
};

// -------------------------------------------------------------------------
// class FiberInit

/** FiberInit提供了纤程子系统的初始化以及在纤程子系统在程序结束时的清理工作。
@*/
class FiberInit
{
	/** 初始化纤程子系统。
	@*/
	FiberInit();

	/** 纤程子系统在程序结束时的清理工作。
	@*/
	~FiberInit();
};

// -------------------------------------------------------------------------
