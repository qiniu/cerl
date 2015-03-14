/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class QuitEvent

/** 退出事件类。
@remark
	一个事件（Event）包含两个角色：事件源（Source）和事件监听者（Listener）。当事件
	源（Source）发出（fire）一个事件后，所有的事件监听者（Listener）可以捕获该事件来
	做一些事情。对于退出事件（QuitEvent）类，它是指在事件源（Source）对象退出这样一个
	时机，监听者（Lisnener）可以获得机会（onQuit）进行处理。
@see cerl::IoService, cerl::CloseObjectOnQuit, cerl::SetFlagOnQuit
@*/
class QuitEvent
{
	/** 事件监听者（Listener）需要实现的接口。
	@*/
	interface Listener
	{
		/** 在事件源（Source）退出时机调用的函数。监听者通过重载该方法实现自己期望的行为。
		@*/
		void cerl_call onQuit();
	};
	
	/** 绑定事件监听者（Listener）到该事件。
	@arg [in] listener
		待绑定的监听者对象。
	@*/
	void cerl_call sink(Listener* listener);

	/** 发出事件。该函数由事件源（Source）调用。
	@*/
	void cerl_call fire();
};

// -------------------------------------------------------------------------
// class CloseObjectOnQuit

/** 退出事件的监听者，它在接收到退出事件时关闭一个对象。
@arg ObjectT
	对象类型。该类型必须实现 close() 方法。
@example
	[[code type="c++"]]
		IoService* service;
		ListenSocket s;
		...
		CloseObjectOnQuit<SocketFile> cooq(service, s);
		while (s.good())
		{
			s.accept();
			...
		}
	[[/code]]
	在该例子中，service是事件源（Source），cooq对象是事件监听者，它在构造过程中
	绑定到 service 的退出事件。在其他纤程执行 service->quit() 时，cooq 对象收到
	service 的退出事件后调用 s.close() 关闭该对象，从而打破 while(s.good()) 这个
	循环。
@see cerl::QuitEvent, cerl::IoService
@*/
template <class ObjectT>
class CloseObjectOnQuit : public QuitEvent::Listener
{

	/** 类的初始化。
	@arg [in] o
		需要在收到退出事件时关闭（close）的对象。
	@*/
	CloseObjectOnQuit(ObjectT& o);

	/** 初始化并绑定（sink）到事件源（Source）。
	@arg SourceT
		事件源（Source）类。该类必须有public成员：
			QuitEvent quitEvent;
	@arg [in] source
		事件源（Source）对象。
	@arg [in] o
		需要在收到退出事件时关闭（close）的对象。
	@*/
	template <class SourceT>
	CloseObjectOnQuit(SourceT* source, ObjectT& o);
	
	/** 绑定（sink）到事件源（Source）。
	@arg SourceT
		事件源（Source）类。该类必须有public成员：
			QuitEvent quitEvent;
	@arg [in] source
		事件源（Source）对象。
	@*/
	template <class SourceT>
	void cerl_call sink(SourceT* source);

	/** 在事件源（Source）退出时被自动调用。其行为是调用 o.close() 关闭对象。
	@*/
	void cerl_call onQuit();
};

// -------------------------------------------------------------------------
// class SetFlagOnQuit

/** 退出事件的监听者，它在接收到退出事件时设置一个 flag 为 true。
@example
	[[code type="c++"]]
		IoService* service;
		bool fQuit = false;
		...
		SetFlagOnQuit sfoq(service, fQuit);
		while (!fQuit)
		{
			...
		}
	[[/code]]
	在该例子中，service是事件源（Source），sfoq对象是事件监听者，它在构造过程中
	绑定到 service 的退出事件。在其他纤程执行 service->quit() 时，sfoq 对象收到
	service 的退出事件后将fQuit设置为true，从而打破 while(!fQuit) 这个循环。
@see cerl::QuitEvent, cerl::IoService
@*/
class SetFlagOnQuit : public QuitEvent::Listener
{

	/** 类的初始化。
	@arg [in] f
		需要在事件退出时设置为 true 的 bool 对象。
	@*/
	SetFlagOnQuit(bool& f);

	/** 初始化并绑定（sink）到事件源（Source）。
	@arg SourceT
		事件源（Source）类。该类必须有public成员：
			QuitEvent quitEvent;
	@arg [in] source
		事件源（Source）对象。
	@arg [in] f
		需要设置的bool变量对象。
	@*/
	template <class SourceT>
	SetFlagOnQuit(SourceT* source, bool& f);

	/** 绑定（sink）到事件源（Source）。
	@arg SourceT
		事件源（Source）类。该类必须有public成员：
			QuitEvent quitEvent;
	@arg [in] source
		事件源（Source）对象。
	@*/
	template <class SourceT>
	void cerl_call sink(SourceT* source);
	
	/** 在事件源（Source）退出时被自动调用。其行为是调用设置绑定时传入的bool值f为true。
	@*/
	void cerl_call onQuit();
};