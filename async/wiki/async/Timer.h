//@encoding utf8
//@ns cerl

/**	返回一个全局的TimerQueue。使用的时候只需要往这个队列里面加入(start) Timer对象。
@remark
	直接使用不用加锁，因为在TimerQueue内部有锁。
@*/
#define CerlTimerQueue();

/** 定时器服务的实现类。
@*/
typedef TimerService;

/** Timer服务初始化类。
@remark
	只需要定义一个 TimerInit 对象即可使用定时器队列。
@example
	[[code type="c++"]]
	TimerInit timeInit；
	...
	CerlTimerQueue()->start(timer);
	...
	[[/code]]
	这里timer是一个用户自定义的派生自Timer的对象。
@see cerl::FastTimer, cerl::FastTimeQueue。
@*/
class TimerInit
{
	TimerInit(size_t timerPrecision = 500, size_t timerSlots = 31);
};

/** 带有 Timer 的 yield，如果纤程 yield 时间超过 Timer 的超时时间，纤程将被唤醒。
@arg self
	当前执行的纤程。
@arg timeout
	设置 Timer 超时时间。
@return
	@val true
		还未到 timeout 就 resume 返回 true。
	@val false
		timeout 之后 resume 返回 false。
@*/
bool cerl_call timed_yield(Fiber self, Timeout timeout);

/** 纤程休眠一段时间。
@arg [in] self
	当前执行纤程。
@arg [in] ms
	纤程需要休眠的时间（in ms）。
@*/
void cerl_call sleep(Fiber self, Timeout ms);

/** 纤程休眠。
@remark
	纤程休眠，但马上可以被切换回来继续执行。
@arg [in] self
	当前执行纤程。
@*/
void cerl_call sleep(Fiber self);

/** 心跳，每过一个心跳时间将会触发 ServerT 的一次 onHeartbeat 心跳操作。
@arg ServerT
	模板参数，执行心跳的类。
@arg self
	当前执行的纤程。
@arg server
	执行心跳的对象。
@arg timeout
	心跳时间。
@*/
template <class ServerT>
void cerl_call heartbeat(Fiber self, ServerT* server, Timeout timeout);

