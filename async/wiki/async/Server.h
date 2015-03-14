/*
@encoding utf8
@ns cerl
@*/

// -----------------------------------------------------------------------
// class ServerSession

/** 本类定义了一个网络服务的Session。
	一个 Session 包括创建 Socket，等待连接，接受连接之后的处理，断开连接。
@*/
class ServerSession
{
	/** 启动 Session 的函数。
	@arg ServerT
		实际Server对象模板，具体的连接和数据处理工作由它来实现。
	@arg self
		当前纤程。
	@arg server
		ServerT 对象。
	@arg sdAccept
		accept()返回的连接好的 socket 对象。
	@*/
	template <class ServerT>
	void cerl_call main(Fiber self, ServerT* const server, SOCKET sdAccept);
};

// -----------------------------------------------------------------------
// class ServerRunner
/** 对 ServerT 和 SessionT 的封装。
	使用的时候只需要定义 ServerT，然后直接调用 run() 就可以。
@arg ServerT
	模板类参数，用来表示处理具体逻辑的 Server，必须提供一个bool型 MultiThreads 成员变量
	表示是否使用多线程模式。
@arg SessionT
	模板类参数，用来表示 Session 的实现，缺省就用 ServerSession。
@see ServerSession。
@*/
template <class ServerT, class SessionT = ServerSession>
class ServerRunner
{
	typedef ServerT server_type;	
	
	/** 提供运行一个 Server 的接口。
	@arg self
		当前纤程对象。
	@arg server
		具体实现业务逻辑的 server 对象。
	@arg socketListen
		正在监听的 socket 对象。
	@*/
	HRESULT cerl_call run(Fiber self, server_type* server, SOCKET socketListen);
};

// -----------------------------------------------------------------------