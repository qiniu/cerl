/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class FiberEvent

/** FiberEvent用于纤程之间的同步操作。
@remark
	FiberEvent应用场景：
	* 某个worker Fiber在某个时刻，希望同时执行若干job（job1 ～ jobN）。
	* 该worker Fiber并不总是等待这些job全部完成才继续向执行，而是在满足某个条件时就结束等待。
@*/
class FiberEvent
{
	/** 初始化 FiberEvent
	@*/
	FiberEvent();
	
	/** 获取执行同步操作的work Fiber。
	@return
		返回等待在 Event 发生的 Fiber 对象。
	@*/
	Fiber cerl_call getCurrentFiber() const;

	/** 在出现异常的时候使用reset，可以确保wait和timed_wait能够执行。
	@*/
	void cerl_call reset();

	/** FiberEvent的同步原语。
	@arg [in] self
		当前拥有执行权的work fiber。
	@remark
		支持超时的FiberEvent同步原语请参考\<FiberEvent::timed_wait>。
	@see FiberEvent::timed_wait
	@*/
	void cerl_call wait(Fiber self);

	/** FiberEvent的同步原语，该同步原语支持超时。
	@arg [in] self
		当前拥有执行权的work fiber。
	@arg [in] timeout
		超时时间。
	@see FiberEvent::wait
	@*/
	bool cerl_call timed_wait(Fiber self, Timeout timeout);

	/** Notifier 为使用 FiberEvent 进行同步操作的work fiber提供了唤醒的方法。
	@*/
	class Notifier
	{
		/** Notifier的初始化。
		@arg [in] ev
			需要把 Notifier 对象关联到的 FiberEvent 对象。
		@remark
			把 Notifier 对象关联到事件(FiberEvent)上。
		@*/
		Notifier(FiberEvent* ev);
		
		/** 测试FiberEvent是否有效。
		@return
			@val true
				FiberEvent有效。
			@val false
				FiberEvent无效。
		@remark
			job fiber对其参数对象的任何操作，都需要在判断 ev.good() 成立后才可以访问。
		@*/
		bool cerl_call good() const;

		/** 当同步条件满足时调用notify，将执行权切换到work fiber执行。
		@arg [in] self
			当前拥有执行权的job fiber。
		@remark
			唤醒正在等待事件发生的纤程。
		@*/
		void cerl_call notify(Fiber self);
	};
};

// -------------------------------------------------------------------------
