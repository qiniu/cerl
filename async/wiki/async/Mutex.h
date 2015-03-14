/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class ScopedLock

/**	ScopedLock在Scope范围内使用ResourceT加锁，保证同时只能有一个纤程运行Scope内的代码。
@arg ResourceT
	能够提供锁的资源，如Shared，FiberMutex。
@remark
	对于 Shared，使用ScopedLock方法（以SharedFile为例）如下：
	[[code]]
	typedef ScopedLock<Shared> scoped_lock;
	SharedFile shared;
	{
		SharedFile::scoped_lock lock(self, shared);
		shared.write(...);
		shared.write(...);
	}
	[[/code]]
	此外，ScopedLock是异常安全的。
@*/
template <class ResourceT>
class ScopedLock
{
	/** ScopedLock在scope范围对ResourceT进行加锁操作。
	@*/
	ScopedLock(Fiber self, ResourceT& cs);

	/** ScopedLock在scope范围代码运行结束后，将通过析构函数来释放锁。
	@*/
	~ScopedLock();
};

// -------------------------------------------------------------------------
// class FiberOnce
/** FiberOnce 类提供对纤程的锁操作。
@remark
	如果取锁(acquire)成功则返回 true，否则返回 false。
@*/
class FiberOnce
{
	/** FiberOnce 初始化
	@*/
	FiberOnce();

	/** 返回当前持有锁的纤程。
	@return Fiber
		持有锁的纤程对象。
	@*/
	Fiber cerl_call getCurrentFiber() const;
	
	/** FiberOnce 取锁操作。
	@arg self
		需要取锁的纤程。
	@return bool
		@val true 成功获取锁。
		@val false 未获取锁。
	@remark
		未拿到锁就 yield，等待唤醒。
	@*/
	bool cerl_call acquire(Fiber self);

	/** FiberOnce 释放锁操作。
	@remark
		释放锁的同时唤醒等待队列上的所有纤程。
	@*/
	void cerl_call release();
};

// -------------------------------------------------------------------------
// class FiberMutex

//
// FiberMutex类提供对纤程的锁操作。
// @remark
//	如果当前锁被被别的纤程持有，则加入阻塞队列。
//
class FiberMutex
{
	/**	FiberMutex初始化。
	@*/
	FiberMutex();

	/**	FiberMutex加锁操作。
	@arg [in] self
		请求锁的纤程，即当前拥有执行权的纤程。
	@remark
		FiberMutex提供了一个锁等待队列，当FiberMutex已经将锁分配给某个纤程时，
		其他纤程如果acquire锁，则将被加入锁等待队列，并进入阻塞状态，直到锁被release。
		本函数返回时纤程 self 一定拿着锁。
	@see release
	@*/
	void cerl_call acquire(Fiber self);

	/** FiberMutex解锁操作。
	@remark
		执行release后，FiberMutex将释放锁，并将锁等待队列中的第一个纤程唤醒。
	@see acquire
	@*/
	void cerl_call release();

	//@type scoped_lock
	typedef ScopedLock<FiberMutex> scoped_lock;
};

// -------------------------------------------------------------------------
