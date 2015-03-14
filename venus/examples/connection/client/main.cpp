//test program of Connection class

//#define CERL_VERBOSE
#include <stdio.h>
#include <async/Application.h>
#include "../../../include/venus/Connection.h"


void cerl_callback fooClient(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	
	cerl::Connection	conn;
   	conn = cerl::connect("localhost", 5039);
	if(!conn.good())
	{
		CERL_WARN("client", ("Connection test client: connection fail to establish\n"));
	}
	CERL_VLOG("client", ("Connection established\n"));
	
	int	ret;
	
	for(int i=0; ; ++i)
	{
		//cerl::MailNo mn = conn.send(p.self, (cerl::FID)7, i);
		printf("data sent: %d\n", i);
		cerl::ScopedAlloc alloc(CerlBlockPool(p.self));
		conn.call(alloc, ret, (cerl::FID)0, i);
		if(!conn.good())
		{
			CERL_WARN("client", ("connection closed.\n"));
			break;
		}
		printf("data received: %d\n", ret);
	}
}

int main()
{
	cerl::Application	app;
	app.run(fooClient, NULL, 16000);
	return 0;
}
