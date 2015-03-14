#ifndef ASYNC_FILEIO_LIBAIO_H
#define ASYNC_FILEIO_LIBAIO_H

#include "aio.h"
#include "aio_syscall.h"

namespace cerl
{

namespace detail
{
	typedef void(*AioCallback)(struct iocb*);

	struct AioContext
	{
		AioCallback callback;
		cerl::Fiber fiber;
		ssize_t result;
	};

	static inline
	void aioCallback(struct iocb* cb)
	{
		AioContext* ctx = (AioContext*)cb->data;
		cerl::IoService::ScheduleMessage msg = CerlIoService(ctx->fiber)->messageOfSchedule(ctx->fiber);
		msg.postMT();
	}
}

// -------------------------------------------------------------------------
// class LibAioInit

template <int N>
class LibAioInit_
{
public:
	static io_context_t g_ctx;

public:
	LibAioInit_()
	{
		if (!g_ctx)
		{
			int r = io_setup(1024, &g_ctx);
			assert(r == 0);
			CERL_LOG("LibAioInit", ("Context setup.\n"));
		}
		CERL_LOG("LibAioInit", ("Context need not setup.\n"));

		HTHREAD hThread = CreateThread(ThreadMain, NULL);
        CloseThread(hThread);
	}

	~LibAioInit_()
	{
		int r = io_destroy(g_ctx);
		assert(r == 0);

		g_ctx = NULL;
	}

private:

	static THREADRET WINAPI ThreadMain(LPVOID lpThreadParameter)
	{
		return (THREADRET)runAio();
	}

	static int runAio();
};

typedef LibAioInit_<0> LibAioInit;

template <int N>
io_context_t LibAioInit_<N>::g_ctx = NULL;

template <int N>
int LibAioInit_<N>::runAio()
{
	assert(g_ctx);

	// run event loop
	const int EVENT_COUNT = 64;
	struct io_event events[EVENT_COUNT];
	while (g_ctx)
	{
		int r = io_getevents(g_ctx, 1, EVENT_COUNT, events, NULL);

		if (r == -1)
		{
			if (errno == EINTR)
				continue;
			break;
		}

		for (int i=0; i<r; ++i)
		{
			const io_event& event = events[i];
			if (!event.data)
				break;

			detail::AioContext* ctx = (detail::AioContext*)event.obj->data;
			ctx->result = event.res;
			(*ctx->callback)(event.obj);
		}
	}

	return 0;
}

// -------------------------------------------------------------------------

static inline
ssize_t aio_pread(int fd, void *buf, size_t count, off_t offset)
{
	assert(LibAioInit::g_ctx && "libaio not initialized");

	detail::AioContext ctx;
	ctx.callback = detail::aioCallback;
	ctx.fiber = cerl::getCurrentFiber();

	struct iocb cb;
	io_prep_pread(&cb, fd, buf, count, offset);
	cb.data = &ctx;
	struct iocb* cbs[1] = {&cb};

	const int ret = io_submit(LibAioInit::g_ctx, 1, (struct iocb**)cbs);
	if (1 != ret)
	{
		CERL_WARN("Aio", ("submit aio read request fail: %s\n", strerror(errno)));
		return -1;
	}

	yield(ctx.fiber);

	if (ctx.result < 0)
	{
		errno = -ctx.result;
		ctx.result = -1;
	}
	return ctx.result;
}

inline
ssize_t aio_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
	assert(LibAioInit::g_ctx && "libaio not initialized");

	detail::AioContext ctx;
	ctx.callback = detail::aioCallback;
	ctx.fiber = cerl::getCurrentFiber();

	struct iocb cb;
	io_prep_pwrite(&cb, fd, (void*)buf, count, offset);
	cb.data = &ctx;
	struct iocb* cbs[1] = {&cb};

	const int ret = io_submit(LibAioInit::g_ctx, 1, (struct iocb**)cbs);
	if (1 != ret)
	{
		CERL_WARN("Aio", ("submit aio write request fail: %d:%s, ctx: %d\n", ret, strerror(-ret), LibAioInit::g_ctx));
		return -1;
	}

	yield(ctx.fiber);

	if (ctx.result < 0)
	{
		errno = -ctx.result;
		ctx.result = -1;
	}
	return ctx.result;
}

inline
ssize_t aio_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	assert(LibAioInit::g_ctx && "libaio not initialized");

	detail::AioContext ctx;
	ctx.callback = detail::aioCallback;
	ctx.fiber = cerl::getCurrentFiber();

	struct iocb cb;
	io_prep_preadv(&cb, fd, iov, iovcnt, offset);
	cb.data = &ctx;
	struct iocb* cbs[1] = {&cb};

	const int ret = io_submit(LibAioInit::g_ctx, 1, (struct iocb**)cbs);
	if (1 != ret)
	{
		CERL_WARN("Aio", ("submit aio readv request fail: %d:%s\n", ret, strerror(-ret)));
		return -1;
	}

	yield(ctx.fiber);

	if (ctx.result < 0)
	{
		errno = -ctx.result;
		ctx.result = -1;
	}
	return ctx.result;
}

static inline
ssize_t aio_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	assert(LibAioInit::g_ctx && "libaio not initialized");

	detail::AioContext ctx;
	ctx.callback = detail::aioCallback;
	ctx.fiber = cerl::getCurrentFiber();

	struct iocb cb;
	io_prep_pwritev(&cb, fd, iov, iovcnt, offset);
	cb.data = &ctx;
	struct iocb* cbs[1] = {&cb};

	const int ret = io_submit(LibAioInit::g_ctx, 1, (struct iocb**)cbs);
	if (1 != ret)
	{
		CERL_WARN("Aio", ("submit aio writev request fail: %d, errno: %d:%s\n", ret, errno, strerror(-ret)));
		return -1;
	}

	yield(ctx.fiber);

	if (ctx.result < 0)
	{
		errno = -ctx.result;
		ctx.result = -1;
	}
	return ctx.result;
}

}

#endif /* ASYNC_FILEIO_LIBAIO_H */

