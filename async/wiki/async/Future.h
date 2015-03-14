/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class Future

/** Future是在纤程的基础上增加了同步原语。
@see cerl::Fibers
@*/
class Future
{
	/**	初始化Future。初始化工作包括根据输入的参数创建纤程，并调度执行。
		此纤程具有同步原语wait，因此被称为Future。
	@arg [in] self
		当前拥有执行权的纤程。
	@arg [in] lpStartAddress
		新创建纤程的回调函数。
	@arg [in] startParam
		新创建纤程携带的参数。
	@arg [in] dwStackSize
		设置新创建纤程的栈大小。如果为0，那么将使用默认大小。
	@*/
	Future(Fiber self, FiberCallback lpStartAddress, void* startParam = NULL, size_t dwStackSize = 0);

	/**	查询Future是否完成。
	@return
		@val 1
			Future已经结束。
		@val 0
			Future没有完成。
	@*/
	int cerl_call isFinished() const;

	/** Future的同步原语。当前拥有执行权的纤程可以使用wait来等待Future的完成。
	@arg self
		当前拥有执行权的纤程。
	@remark
		如果Future已经完成，则纤程继续执行；
		否则，纤程将进入阻塞状态，直到Future完成时才被唤醒。
	@*/
	void cerl_call wait(Fiber self);
};

// -------------------------------------------------------------------------
// class FutureParam

/** FutureParam类描述了Future及其携带的参数，在Future的回调函数中使用。
@*/
class FutureParam
{
	//@members
	Fiber const fiber;
	void* const val;
	IoService* const service;
	//@end

	/** 该函数负责FutureParam初始化：设定Future及Future的参数。
	@arg [in] lpParam
		Future回调函数携带的参数。
	@remark
		FutureParam构造函数完成后，可以为Future回调函数提供以下帮助：
		1. 为Future回调函数提供参数。
		2. 为Future回调函数提供当前Future。
	@*/
	FutureParam(LPVOID lpParam);

	/** FutureParam的析构函数负责Future的退出清理工作：将执行权由当前纤程切换至另一个纤程，并将Future销毁。
	@*/
	~FutureParam();
};

// -------------------------------------------------------------------------
