/*
@encoding utf8
@ns cerl
@*/

/**	dbg_closesocket 将关闭一个指定的socket套接字。
@arg [in] op
	当定义了CERL_LOG的时候，将把op指针指向的字符串打印出来。
@arg [in] sd
	指定要关闭的socket套接字。
@*/
void cerl_call dbg_closesocket(const char* op, SOCKET sd);

/**	从socketaddr_in结构体中提取ip地址，并把ip地址转换为主机字节顺序。
@arg [in] addr
	指向socketaddr_in的指针。
@return
	返回值是主机字节顺序的ip地址。
@*/
Address cerl_call getAddress(sockaddr_in* addr);

/**	通过主机名称来得到ip地址，并把ip地址转换为主机字节顺序。其中，主机名称是char*类型。
@arg [in] hostname
	主机名称。 
@return
	返回值是主机字节顺序的ip地址。
@*/
Address cerl_call getAddress(const char* hostname);

/**	通过主机名称来得到ip地址，并把ip地址转换为主机字节顺序。其中，主机名称是String类型。
@arg [in] host
	主机名称。 
@return
	返回值是主机字节顺序的ip地址。
@*/
Address cerl_call getAddress(const String& host);

/**	建立一个socket套接字，进行套接字绑定，并对此socket进行监听。
@arg [in] si
	和socket套接字进行绑定的\<SockaddrIn>。
@arg [in] backlog
	挂起连接队列的最大长度。
@return
	@val 返回socket fd
		listenSocket成功。
	@val INVALID_SOCKET
		listenSocket失败。
@*/
SOCKET cerl_call listenSocket(const SockaddrIn& si, int backlog);

/**	建立一个socket套接字，并进行套接字绑定，并对此socket进行监听。
@arg [in] local
	一个包含ip地址字段和port端口号字段的模板参数。
@arg [in] bind_retry_times
	当绑定失败后，尝试重新绑定的次数。
@arg [in] backlog
	挂起连接队列的最大长度。
@return
	@val 返回socket fd
		listenSocket成功。
	@val INVALID_SOCKET
		listenSocket失败。
@*/
template <class HostT>
SOCKET cerl_call listenSocketEx(HostT& local, unsigned bind_retry_times, int backlog);

/**	建立一个socket套接字，并使用这个套接字和指定的SockaddrIn进行连接。
@arg [in] si
	进行连接的\<SockaddrIn>。
@return
	@val 返回socket fd
		connectSocket连接成功。
	@val INVALID_SOCKET
		connectSocket连接失败。
@*/
SOCKET cerl_call connectSocket(const SockaddrIn& si);

/**	建立一个socket套接字，并使用这个套接字和指定的SockaddrIn进行连接。
@arg [in] hostname
	主机名称，类型为const char*。 
@arg [in] port
	端口号。
@return
	@val 返回socket fd
		connectSocket连接成功。
	@val INVALID_SOCKET
		connectSocket连接失败。
@*/
SOCKET cerl_call connectSocket(const char* hostname, Port port);

/** SockaddrIn类继承自SOCKADDR_IN结构体，并且提供了Socket的创建、绑定和连接的功能。
@*/
struct SockaddrIn
{
	/** 构造SockaddrIn对象。ip被设为ANY，port设为指定的端口号。
	@arg [in] port
		端口号。
	@remark
		这个构造函数通常用户服务端listen时用。
		因为服务器往往不绑定具体ip（当然如果机器有多个网卡，有可能listen时指定ip也是有的）。
	@*/
	SockaddrIn(Port port);

	/** 构造SockaddrIn对象。
	@arg [in] ip
		ip地址。
	@arg [in] port
		端口号。
	@*/
	SockaddrIn(Address ip, Port port);

	/**	绑定指定的socket套接字。
	@arg [in] sd
		一个未进行过绑定的socket套接字。
	@return
		@val 0
			绑定成功。
		@val -1
			绑定失败。
	@*/
	int cerl_call bind(SOCKET sd);

	/**	连接一个指定的套接字。
	@arg [in] sd
		一个未进行过连接的socket套接字。
	@return
		@val 0
			绑定成功。
		@val -1
			绑定失败。
	@*/
	int cerl_call connect(SOCKET sd);
};

/** SocketAppInit类负责启动和清除一个socket。
@*/
class SocketAppInit
{};

/** 通过使用createsocket来建立一个socket。
@return
	如果成功，将返回一个socket套接字。
@*/
SOCKET cerl_call createSocket();

/**	closesocket将关闭一个指定的socket套接字。
@arg [in] sd
	要关闭socket套接字。
@return
	@val 0
		closesocket成功。
	@val -1
		closesocket失败。
@*/
int cerl_call closesocket(SOCKET sd);