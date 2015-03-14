/*
@encoding utf8
@ns venus
@*/

/** 请求者， 一个到Server并发送一个或多个调用的连接
@*/
class Caller
{
	/** 发送一个回复。
	@arg [in] result
		要发送的结果。这假设 ResultT 类支持 serialize 函数: 
		[[code type="c++"]]
			template <class ArchiveT>
			void cerl::io::put(ArchiveT& ar, const ResultT& result);
		[[/code]]
	@*/
	template <class ResultT>
	bool reply(const FID fid, const MailNo mailno, const ResultT& response);
};

/** 服务器。
@see venus::Request, venus::Connection
@example
	[[code type="c++"]]
		//
		// server.cpp
		//

		class FooImpl : public Server<FooImpl>
		{
			void do_sth(Caller* caller, MailHeader* mailHeader, Result& result, Args args)
			{
				...
				// req->reply(FooImpl::code_do_sth, mailHeader->mailno, result); 
			}
		};

		void cerl_callback fooServer(LPVOID lpParam)
		{
			cerl::FiberParam p(lpParam);
			
			FooImpl foo;
			foo.run(p.self, 8888);
		}

		int main()
		{
			cerl::Application app;
			app.run(fooServer);
			return 0;
		}
	[[/code]]
@*/
template <class RequestHandlerT>
class Server
{
	/** 运行一个服务器。
	@arg [in] self
		要运行该服务器的纤程。
	@arg [in] addr
		要运行的服务器的地址（含端口）。如果服务器不想绑定ip，可只指定端口：
		[[code type="c++"]]
			run(self, 8888); // 监听8888端口
		[[/code]]
	@*/
	template <class AddressT>
	void run(Fiber self, const AddressT& addr);
};
