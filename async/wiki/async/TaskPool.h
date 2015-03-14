/*
@encoding utf8
@ns cerl
@*/

/** TaskStub描述了一个任务，用来处理计算消耗型任务或者不可间断性I/O事件（如DB操作），
	该任务将由TaskPool来处理。
@*/
struct TaskStub
{
	//@members
	void* reserved;		//保留字段
	void* fiberMain;	//存放TaskThread的主纤程，确保在执行任务代码后能够切换回TaskThread。
	IoCompletionPort::Message wakeup;//存放WorkFiber，确保在TaskThread完成任务后能够切换回WorkFiber。
	//@end
};

// -------------------------------------------------------------------------
// class TaskPool

/** TaskPool类提供了添加、处理任务的方法。TaskPool通过一个任务队列和一组线程来完成任务：
	这些线程将从TaskPool的任务队列取出任务并执行，如果任务队列中没有任务可以处理
	或者所有的任务都已经处理完毕，这些线程将等待，直到有新的任务加入到任务队列。
@*/
class TaskPool
{
	/**	启动TaskPool。
	@arg [in] iocp
		I/O完成端口。
	@arg [in] threadCount
		TaskPool处理任务的线程数量。能够创建的线程数量由CPU的资源来决定。
	@remark
		在 \<cerl::IoService> 初始化时启动 TaskPool。
	@*/
	void cerl_call start(IoCompletionPort* iocp, size_t threadCount = 2);

	/**	向TaskPool的任务队列中push一个任务。
	@arg [in] task
		要加入任务队列中的任务。
	@*/
	void cerl_call addTask(TaskStub* task);
};