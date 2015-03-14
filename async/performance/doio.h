#ifndef CERL_TEST_DOIO_H
#define CERL_TEST_DOIO_H

#if defined(X_OS_LINUX)
#include <unistd.h>
#endif

inline bool do_send(SOCKET sd, const char* buf, int cb)
{
	//printf("send %d bytes\n", cb);	
	while (cb)
	{
		int cbDone = send(sd, buf, cb, 0);
		if (cbDone <= 0)
			return false;
		buf += cbDone;
		cb -= cbDone;
	}
	return true;
}

inline bool do_recv(SOCKET sd, char* buf, int cb)
{
	//printf("send %d bytes\n", cb);	
	while (cb)
	{
		int cbDone = recv(sd, buf, cb, 0);
		if (cbDone <= 0)
			return false;
		buf += cbDone;
		cb -= cbDone;
	}
	return true;
}


#endif
