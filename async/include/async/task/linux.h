#ifndef ASYNC_TASK_LINUX_H
#define ASYNC_TASK_LINUX_H

namespace cerl
{

inline int open_byRunTask(cerl::Fiber fiber, const char* path, int flags)
{
	int ret;
	{
		cerl::TaskRunner runner(fiber);
		ret = ::open(path, flags);
		CerlFiberErrno(fiber) = errno;
	}
	errno = CerlFiberErrno(fiber);
	return ret;
}

inline int open_byRunTask(cerl::Fiber fiber, const char* path, int flags, mode_t mode)
{
	int ret;
	{
		cerl::TaskRunner runner(fiber);
		ret = ::open(path, flags, mode);
		CerlFiberErrno(fiber) = errno;
	}
	errno = CerlFiberErrno(fiber);
	return ret;
}

inline int close_byRunTask(cerl::Fiber fiber, int fd)
{
	int ret;
	{
		cerl::TaskRunner runner(fiber);
		ret = ::close(fd);
		CerlFiberErrno(fiber) = errno;
	}
	errno = CerlFiberErrno(fiber);
	return ret;
}

inline int unlink_byRunTask(cerl::Fiber fiber, const char* path)
{
	int ret;
	{
		cerl::TaskRunner runner(fiber);
		ret = ::unlink(path);
		CerlFiberErrno(fiber) = errno;
	}
	errno = CerlFiberErrno(fiber);
	return ret;
}

inline int remove_byRunTask(cerl::Fiber fiber, const char* path)
{
	int ret;
	{
		cerl::TaskRunner runner(fiber);
		ret = ::remove(path);
		CerlFiberErrno(fiber) = errno;
	}
	errno = CerlFiberErrno(fiber);
	return ret;
}

} /* namespace cerl */

#endif /* ASYNC_TASK_LINUX_H */

