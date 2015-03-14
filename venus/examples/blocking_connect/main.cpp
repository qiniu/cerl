//test program of Connection class

//#define CERL_VERBOSE
#include <stdio.h>
#include <sys/time.h>
#include <async/Application.h>
#include "venus/Connection.h"

struct Context
{
	cerl::Connections* conns;
	int count;
};

void fooClient(cerl::Fiber self, Context* ctx)
{
	cerl::Connection	conn;

	CERL_WARN("client", ("context: %p\n", ctx));
	int id = ctx->count;;
	CERL_WARN("client", ("start client: %d\n", id));

	// test unreachable address
	conn = cerl::Connection(*ctx->conns, cerl::NInformation("192.168.56.111", 5039));

	// test connect failed
	// conn = cerl::Connection(self, *ctx->conns, cerl::NInformation("127.0.0.1", 5039));

	timeval last;
	gettimeofday(&last, NULL);

	while (1)
	{
		int	ret;
		
		for(int i=0; ; ++i)
		{
			cerl::ScopedAlloc alloc(CerlBlockPool(self));
			if (!conn.call(alloc, ret, (cerl::FID)0, i))
			{
				timeval now;
				gettimeofday(&now, NULL);
				int timeTakes = (now.tv_sec - last.tv_sec) * 1000 + (now.tv_usec - last.tv_usec) / 1000;
				CERL_WARN("client", ("call failed. client: %d, time takes: %d\n", id, timeTakes));
				last = now;
				break;
			}
			printf("data received: %d\n", ret);
		}
	}
}
void cerl_callback fooClientProc(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	
	Context* ctx = (Context*)p.val;

	fooClient(p.self, ctx);
}

void cerl_callback mainProc(LPVOID lpParam)
{
	cerl::FiberParam p(lpParam);
	cerl::Connections conns(CerlMemPools(p.self));

	int count = (int)(size_t)p.val;

	for (int i=0; i<count; i++)
	{
		Context* ctx1 = new Context;
		ctx1->conns = &conns;
		ctx1->count = i;
		p.service->startFiber(p.self, fooClientProc, ctx1, 16384);
	}
	Context ctx;
	ctx.conns = &conns;
	ctx.count = count;
	fooClient(p.self, &ctx);
}

int main()
{
	cerl::TimerInit init(10);

	cerl::Application	app;
	app.run(mainProc, (void*)10, 16384);
	return 0;
}

