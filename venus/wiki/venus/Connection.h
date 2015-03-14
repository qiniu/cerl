/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// struct ConnectOptions

/** 连接选项集。
@*/
class ConnectOptions
{
	size_t msgBufSize;
	size_t recvStackSize;
	size_t sendStackSize;

	/** 初始化所有连接选项为默认配置。
	@*/
	ConnectOptions();
};

// -------------------------------------------------------------------------
// class Connection

/** 连接。在客户端向服务器发送connect请求后，会得到一个连接（Connection），用以向服务器发送请求。
@see cerl::connect, cerl::Server
@*/
class Connection
{
	/** 初始化一个空连接。
	@*/
	Connection();
	
	/** 初始化一个连接。
	@arg [in] conn
		一个连接对象的引用。
	@*/
	Connection(const Connection& conn);
	
	/** 初始化一个连接。
	@arg [in] self
		发起连接请求的纤程。
	@arg [in] socket
		建立连接的socket。
	@arg [in] options
		连接选项。
	@*/
	Connection(Fiber self, SOCKET socket, const ConnectOptions& options);
		
	/** 判断该Connection对象是否是有效的连接。
	@return
		@val true
			连接有效。
		@val false
			连接无效。
	@*/
	bool cerl_call good() const;

	/** 释放连接。
	@*/
	void cerl_call release();
	
	/** 发送一个无回复的请求（我们称之为cast请求或异步请求）。
	@arg [in] self
		调用者纤程。
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] args
		请求的参数。这假设 ArgsT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void NS_CERL_IO::put(ArchiveT& ar, const ArgsT& val);
		[[/code]]
	@*/
	template <class ArgsT>
	void cerl_call cast(Fiber self, FID fid, const ArgsT& args);
	
	/** 发送一个无回复的请求（我们称之为cast请求或异步请求）。
	@arg [in] self
		调用者纤程。
	@arg [in] fid
		请求的功能ID（Function ID）。
	@*/
	void cerl_call cast(Fiber self, FID fid);

	/** 发起一个带有超时的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] args
		请求的参数。这假设 ArgsT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void NS_CERL_IO::put(ArchiveT& ar, const ArgsT& val);
		[[/code]]
	@arg [in] timeout
		等待请求的超时时间（in ms）。
		在timeout时间内如果服务器还没有返回应答，那么结束等待并将result设为cerl::code_timeout_error。
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ResultT, class ArgsT>
	void cerl_call call(Fiber self, ResultT& result, FID fid, const ArgsT& args, Timeout timeout);

	/** 发起一个不携带参数的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] timeout
		等待请求的超时时间（in ms）。
		在timeout时间内如果服务器还没有返回应答，那么结束等待并将result设为cerl::code_timeout_error。
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ResultT>
	void cerl_call call(Fiber self, ResultT& result, FID fid, Timeout timeout);

	/** 发起一个同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] args
		请求的参数。这假设 ArgsT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void NS_CERL_IO::put(ArchiveT& ar, const ArgsT& val);
		[[/code]]
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ResultT, class ArgsT>
	void cerl_call call(Fiber self, ResultT& result, FID fid, const ArgsT& args);

	/** 发起一个不携带参数的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ResultT>
	void cerl_call call(Fiber self, ResultT& result, FID fid);

	/** 发送一个带有 Dump 调试信息的无回复的请求（我们称之为cast请求或异步请求）。
		其中，这个请求不携带参数。
	@arg [in] server
		一个指向服务器的指针。
	@arg [in] self
		调用者纤程。
	@arg [in] fid
		请求的功能ID（Function ID）。
	@*/
	template <class ServerT>
	void cerl_call dbg_cast0(ServerT* server, Fiber self, FID fid);

	/** 发送一个带有 Dump 调试信息的无回复的请求（我们称之为cast请求或异步请求）。
	@arg [in] server
		一个指向服务器的指针。
	@arg [in] self
		调用者纤程。
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] args
		请求的参数。这假设 ArgsT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void NS_CERL_IO::put(ArchiveT& ar, const ArgsT& val);
		[[/code]]
	@*/
	template <class ServerT, class ArgsT>
	void cerl_call dbg_cast(ServerT* server, Fiber self, FID fid, const ArgsT& args);

	/** 发起一个带有 Dump 调试信息的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
		这个同步调用不携带参数。
	@arg [in] server
		一个指向服务器的指针。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ServerT, class ResultT>
	void cerl_call dbg_call0(ServerT* server, Fiber self, ResultT& result, FID fid);

	/** 发起一个带有 Dump 调试信息的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
	@arg [in] server
		一个指向服务器的指针。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] args
		请求的参数。这假设 ArgsT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void NS_CERL_IO::put(ArchiveT& ar, const ArgsT& val);
		[[/code]]
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ServerT, class ResultT, class ArgsT>
	void cerl_call dbg_call(ServerT* server, Fiber self, ResultT& result, FID fid, const ArgsT& args);

	/** 发起一个带有 Dump 调试信息的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
		这个同步调用带有超时，并且不携带参数。
	@arg [in] server
		一个指向服务器的指针。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] timeout
		等待请求的超时时间（in ms）。
		在timeout时间内如果服务器还没有返回应答，那么结束等待并将result设为cerl::code_timeout_error。
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ServerT, class ResultT>
	void cerl_call timed_dbg_call0(ServerT* server, Fiber self, ResultT& result, FID fid, Timeout timeout);

	/** 发起一个带有 Dump 调试信息的同步调用（即需要服务器返回应答的请求。我们也称之为同步请求）。
		这个同步调用带有超时。
	@arg [in] server
		一个指向服务器的指针。
	@arg [in] self
		调用者纤程。
	@arg [out] result
		返回服务器的应答。这假设 ResultT 类支持 serialize 函数:
		[[code type="c++"]]
			template <class ArchiveT>
			bool NS_CERL_IO::get(ArchiveT& ar, ResultT& val);
		[[/code]]
	@arg [in] fid
		请求的功能ID（Function ID）。
	@arg [in] args
		请求的参数。这假设 ArgsT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void NS_CERL_IO::put(ArchiveT& ar, const ArgsT& val);
		[[/code]]
	@arg [in] timeout
		等待请求的超时时间（in ms）。
		在timeout时间内如果服务器还没有返回应答，那么结束等待并将result设为cerl::code_timeout_error。
	@remark
		如果连接无效（Connection::good()返回false），result返回cerl::code_socket_error。
	@*/
	template <class ServerT, class ResultT, class ArgsT>
	void cerl_call timed_dbg_call(ServerT* server, Fiber self, ResultT& result, FID fid, const ArgsT& args, Timeout timeout);
};

/** 连接一个服务器。
@arg [in] self
	发起连接请求的纤程。
@arg [in] addr
	要连接的服务器地址（含端口）。
@arg [in] options
	连接选项。
@return
	返回一个连接对象。用户应调用 Connection::good() 以确定连接是否成功。
@*/
template <class AddressT>
Connection cerl_call connect(
	Fiber self, const AddressT& addr, const ConnectOptions& options);

/** 连接一个服务器。
@arg [in] self
	发起连接请求的纤程。
@arg [in] hostname, port
	要连接的服务器地址（含端口）。
@arg [in] options
	连接选项。
@return
	返回一个连接对象。用户应调用 Connection::good() 以确定连接是否成功。
@example
	[[code type="c++"]]
		//
		// Foo.h (rpc-call code generated by venusc)
		//
		
		class Foo : public cerl::Connection, public FooBase
		{
		public:
			void dosth(cerl::Fiber self, _DosthResult& result, Args args)
			{
				...
			}
		};
		
		//
		// client.cpp
		//
		#include "Foo.h"
		#include <venus/Application.h>
		
		void cerl_callback fooClient(LPVOID lpParam)
		{
			cerl::FiberParam p(lpParam);
			
			Foo foo = cerl::connect(p.self, "localhost", 8888);
			
			Foo::_DosthResult result;
			foo.dosth(p.self, result, args);
			
			...
		}

		int main()
		{
			cerl::Application app;
			app.run(fooClient);
			return 0;
		}
	[[/code]]
@*/
Connection cerl_call connect(
	Fiber self, const char* hostname, Port port, const ConnectOptions& options);
