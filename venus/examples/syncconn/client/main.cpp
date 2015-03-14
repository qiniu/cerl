//test program of Connection class

//#define CERL_VERBOSE
#include <stdio.h>
#include <async/Application.h>
#include "../sdl_testsvr_base.h"
#include "../../../include/venus/Connection.h"


void cerl_callback fooClient()
{
	int sock = cerl::connectSocket("localhost", 8888);
	if (-1 == sock)
	{
		CERL_WARN("client", ("connect failed: %s\n", strerror(errno)));
		return;
	}
	CERL_VLOG("client", ("Connection established\n"));

	cerl::Pools pools;
	cerl::BlockPool recycle;

	cerl::SyncConnection conn(sock, pools.get_pool(VENUS_CONNECTION_BUFSIZE));
	conn.set_timeout(10);
	
	for(int i=0; ; ++i)
	{
		//cerl::MailNo mn = conn.send(p.self, (cerl::FID)7, i);
		printf("data sent: %d\n", i);
		testsvr::MsgData args;
		args.assign('c');
		testsvr::TestSvrBase::_TestResult ret;
		cerl::ScopedAlloc alloc(recycle);
		conn.call(alloc, ret, (cerl::FID)1, args);
		if(!conn.good())
		{
			CERL_WARN("client", ("connection closed.\n"));
			break;
		}
		printf("data received: %d\n", (int)ret);
	}
}

int main()
{
	fooClient();
	return 0;
}
