/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class ApplicationInit

/** ApplicationInit 类包装了 FiberInit、SocketInit、AioInit、TaskPoolInit 和 TimerInit，
	完成 Fiber、Socket、Aio、TaskPool 和 Timer 的初始化操作。
@see cerl::FiberInit, cerl::SocketInit, cerl::AioInit, cerl::TaskPoolInit, cerl::TimerInit
@*/
class ApplicationInit
{
};

// -------------------------------------------------------------------------
// class Application

/** Application 类包装了 IoService 类，并完成 IoService 所需的初始化操作。zz
@see cerl::ApplicationInit, cerl::IoService
@*/
class Application : public ApplicationInit, public IoService
{
	/** Application初始化。
	@arg [in] threadCount
		taskpool中要开启的线程数。
	@arg [in] timerPrecision
		定时器精度（in ms）。
	@arg [in] timerSlots
		定时器队列的插槽（Slot）数。
		一般以 (timerPrecision * timerSlots) 值略大于常规的 timeout 值为宜。
	@*/
	Application(size_t threadCount = 2, size_t timerPrecision = 500, size_t timerSlots = 31);
};

// -------------------------------------------------------------------------
// class Shell

/** 该类提供了使用 Shell 的方法。用户在使用 Shell 时需要编写 Shell 的处理函数代码。
@remark
	用户在需要使用 Shell 的时候，也可以直接使用\<startShell>函数。
@see startShell
@*/
class Shell
{

/** 启动一个 Shell。
@arg fnShell
	Shell 处理函数代码。
@arg startParam
	Shell 参数。
@arg dwStackSize
	Shell 使用的堆栈大小。
@*/
	void cerl_call start(FiberCallback fnShell, void* startParam = NULL, size_t dwStackSize = 0);
};

/** 启动一个 Shell。用户需要编写 Shell 的处理函数代码。
@arg fnShell
	Shell 处理函数代码。
@arg startParam
	Shell 参数。
@arg dwStackSize
	Shell 使用的堆栈大小。
@see Shell
@*/
void cerl_call startShell(FiberCallback fnShell, void* startParam = NULL, size_t dwStackSize = 0);
