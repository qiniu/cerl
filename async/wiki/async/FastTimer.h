/*
@encoding utf8
@ns cerl
@*/

// =========================================================================
// class FastTimer

/** typedef FastTimeMutex
@*/
typedef NS_STDEXT::Mutex FastTimerMutex;

/** 定时器需要实现的接口类。
@remark
	在定时到达的时候会自动执行 onTimer 函数。
@see cerl::FastTimerQueue	
@*/
class FastTimer 
{
	/**	类初始化。
	@*/
	FastTimer();

	/**	检查 Timer 是否停止。
	@return 返回 bool 值。
		@val true 
			表示停止。
		@val false 
			表示非停止。
	@*/
	bool cerl_call stopped() const;

	/** 把 Timer 停止。
	@*/
	void cerl_call stop();

	/** Timer 到期时执行的函数，实际定时器对象通过重载实现。
	@*/
	void cerl_call onTimer(void* caller);
};

// =========================================================================
// class FastTimerQueue

/** 定时器队列。
@remark
	定时器的实现队列，使用本类时，首先把不同的定时器 (timer) 绑定到队列中，
	每过 precision 的时间之后运行一次队列的 tick() 函数来激活到期的定时器。
@example
	[[code type="c++"]]
		class MyTimer: public FastTimer{
			void cerl_call onTimer(void* caller)
			{
				...
			}
		};
		FastTimer* myTimer = new MyTimer;
		FastTimeQueue* timerQueue;
		timerQueue->start(myTimer, timeout);
		FastTimeQueue::duration_type precision = timerQueue->getPrecision();
		for(;;)
		{
			timerQueue->tick();
			Sleep(precision);
		}
	[[/code]]
	其中 timeout 为要定时的时长。
@*/
class FastTimerQueue
{
	/**	初始化队列。
	@arg [in] precision。
		精度，每个 precision 时间周期会有一次 tick() 的执行。
	@arg [in] slot_count
		队列中 slot 数目。
	@*/
	explicit FastTimerQueue(duration_type precision = 500, size_type slot_count = 31);

	/** 析构函数。
	@*/
	~FastTimerQueue();

	/** 返回精度值。
	@return
		返回精度值。
	@*/
	duration_type cerl_call getPrecision() const;

	/** 把一个定时器(timer)加入定时器队列。
	@arg timer
		要加入的 FastTimer 对象。
	@arg timeout
		timeout 的时间。
	@*/
	void cerl_call start(FastTimer* timer, duration_type timeout);

	/** 重新开始计时。
	@arg timer
		要重新开始的 FastTimer 对象。
	@arg timeout
		timeout 的时间。
	@*/
	void cerl_call restart(FastTimer* timer, duration_type timeout);
	
	/** 每个 precision 执行一次。
	@remark
		如果到时就触发 slot 中 timer 的 onTimer 函数
	@*/
	void cerl_call tick();
	
};
