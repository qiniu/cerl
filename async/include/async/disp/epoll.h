/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/disp/epoll.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: epoll.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_DISP_EPOLL_H
#define ASYNC_DISP_EPOLL_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#ifndef ASYNC_SOCKETIO_EPOLL_H
#include "../socketio/epoll.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// IoService::run()

inline void cerl_call IoService::run()
{
	enum { ASYNC_EVENTSIZE = 128 };

	epoll_event	epevts[ASYNC_EVENTSIZE];

	for (;;)
	{
		processMessageQueueST_();

		if (quitLockRef == QUIT_MASK)
			break;

		const int timeout = iocpST.size() ? 0 : -1;
		const int nevts = epoll_wait(iocpMT.m_epollfd, epevts, ASYNC_EVENTSIZE, timeout);
		if (nevts < 0)
		{
			if (errno == EINTR)
				continue;

			CERL_WARN("Io", ("WARN: IoService::run - epoll_wait failed: %d, ", errno));
			return;
		}

		for (int i = 0; i < nevts; ++i)
		{
			detail::SocketOpType* const op = (detail::SocketOpType*)epevts[i].data.ptr;
			CERL_VLOG("IoService", ("INFO: IoService::run - processing op code %d.\n", *op));

			if (*op == detail::sot_data)
			{
				((SocketFileObject*)op)->handle_event(epevts[i]);
			}
			else if (*op == detail::sot_iocp)
			{
				IoCompletionPort::Message msg;
				CERL_VLOG("IoService", ("INFO: IoService::run - processing iocp disp %d.\n", (int)msg.key));

				while (true)
				{
					int msgsz = recv(iocpMT.m_fdout, &msg, sizeof(msg), MSG_DONTWAIT);
					if (msgsz < 0 && errno == EINTR)
						continue;

					if (msgsz != sizeof(msg))
						break;

					processMessage_(msg.bytes, msg.key, msg.overlapped);
				}
			}
			else if (*op == detail::sot_accept)
			{
				((ListenSocket*)op)->handle_event(epevts[i]);
			}
			else if (*op == detail::sot_connect)
			{
				((detail::ConnectSocket*)op)->handle_event(epevts[i]);
			}
			else
			{
				CERL_WARN("IoService", ("INFO: IoService::run - bad epoll event data: %p, op code: %d\n", op, *op));
			}
		}
	}
}

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_DISP_EPOLL_H */
