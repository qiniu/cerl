/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// struct NInformation

/** 用于描述DataNode节点信息。
@*/
struct NInformation
{
	UINT32 ip;		//节点的ip地址
	UINT32 port;	//节点的端口地址

	/** 初始化节点信息。
	@*/
	NInformation();

	/** 初始化DataNode节点信息。
	@arg [in] ip
		节点的ip地址。
	@arg [in] port
		节点的端口地址。
	@*/
	NInformation(Address ip, Port port);

	/** 初始化DataNode节点信息。
	@arg [in] szHost
		节点的主机名称。其中，主机名称是char*类型。
	@arg [in] port
		节点的端口地址。
	@*/
	NInformation(const char* szHost, Port port);

	/** 初始化DataNode节点信息。
	@arg [in] host
		节点的主机名称。其中，主机名称是String类型。
	@arg [in] port
		节点的端口地址。
	@*/
	NInformation(const String& host, Port port);

	/** 初始化DataNode节点信息。
	@arg [in] szHostAndPort
		一个包含了ip和port的字符数组，其格式为ip:port。
	@*/
	NInformation(const char* szHostAndPort);

	/** 初始化DataNode节点信息。
	@arg [in] hostAndPort
		一个包含了ip和port的字符串，其格式为ip:port。
	@*/
	NInformation(const String& hostAndPort);

	/** 得到节点的ip地址。
	@return
		返回节点的ip地址。
	@*/
	Address cerl_call getAddress();
	
	/** 得到节点的端口地址。
	@return
		返回节点的端口地址。
	@*/
	Port cerl_call getPort();

	/** 检查节点是否可用。
	@return
		@val true
			节点可用。
		@val false
			节点不可用。
	@*/
	bool cerl_call good() const;

	/** 判断两个节点信息是否相同。
	@arg [in] b
		节点信息。
	@return
		@val true
			节点信息相同。
		@val false
			节点信息不同。
	@*/
	bool cerl_call operator==(const NInformation& b);

	/** 比较节点信息的大小。用于放入一个集合。如 std::set<cerl::NInformation>
	@arg [in] b
		节点信息。
	@return
		@val true
			节点信息的值小于b节点。
		@val false
			节点信息的值不小于b节点。
	@*/
	bool cerl_call operator<(const NInformation& b);
};

/** DataNode节点信息类。
@*/
typedef NInformation NInformationArg;

/**	建立一个socket套接字，并使用这个套接字与指定的DataNode节点建立连接。
@arg [in] ni
	连接的DataNode节点\<cerl::NInformation>。
@return
	@val 返回socket fd
		连接成功。
	@val INVALID_SOCKET
		连接失败。
@*/
SOCKET cerl_call connectSocket(const NInformation& ni);

/**	建立一个socket套接字，并使用这个套接字与指定的DataNode节点建立连接。
@arg [in] self
	当前拥有执行权的纤程。
@arg [in] ni
	连接的DataNode节点\<cerl::NInformation>。
@return
	@val 返回socket fd
		连接成功。
	@val INVALID_SOCKET
		连接失败。
@*/
SOCKET cerl_call connectSocket(Fiber self, const NInformation& ni);

// -------------------------------------------------------------------------
