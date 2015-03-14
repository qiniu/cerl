/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Io.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Io.h 2785 2010-05-06 08:39:39Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_IO_H
#define ASYNC_IO_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef ASYNC_ERROR_H
#include "Error.h"
#endif

#ifndef ASYNC_IOSERVICE_H
#include "IoService.h"
#endif

#ifndef ASYNC_MUTEX_H
#include "Mutex.h"
#endif

// -------------------------------------------------------------------------

#if defined(X_OS_LINUX)
#include "fileio/libaio.h"
#include "socketio/epoll.h"
#elif defined(X_OS_WINDOWS)
#include "fileio/windows.h"
#include "socketio/windows.h"
#else
#error "error: Unsupport OS platform!!!"
#endif

#include "fileio/syncio.h"
#include "socketio/sync.h"

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class File, SocketFile

template <class FileObjectT, class OverlappedT = typename FileObjectT::overlapped_type>
class FileImpl : public OverlappedT
{
public:
	typedef FileObjectT file_object_type;

	FileObjectT& resource;

public:
	explicit FileImpl(FileObjectT& o)
		: OverlappedT(getCurrentFiber()), resource(o)
	{
	}

	explicit FileImpl(Fiber self, FileObjectT& o)
		: OverlappedT(self), resource(o)
	{
	}

	bool cerl_call good() const
	{
		return resource.good();
	}

	size_t cerl_call read_some(void* buf, size_t cb)
	{
		return resource.read_some(*(OverlappedT*)this, buf, cb);
	}

	size_t cerl_call write_some(const void* buf, size_t cb)
	{
		return resource.write_some(*(OverlappedT*)this, buf, cb);
	}

	size_t cerl_call read(void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(OverlappedT::getFiber());
		size_t cbTotalReaded = 0;
		for (;;)
		{
			const size_t cbReaded = resource.read_some(*(OverlappedT*)this, buf, cb);
			cbTotalReaded += cbReaded;
			if (cbReaded == cb || cbReaded == 0)
				return cbTotalReaded;
			buf = (char*)buf + cbReaded;
			cb -= cbReaded;
		}
	}

	size_t cerl_call write(const void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(OverlappedT::getFiber());
		size_t cbTotalWritten = 0;
		for (;;)
		{
			const size_t cbWritten = resource.write_some(*(OverlappedT*)this, buf, cb);
			CERL_ASSERT(cb >= cbWritten);

			cbTotalWritten += cbWritten;
			if (cbWritten == cb || cbWritten == 0)
				return cbTotalWritten;
			buf = (char*)buf + cbWritten;
			cb -= cbWritten;
		}
	}
};

typedef FileImpl<FileObject, FileOverlapped32> File32;
typedef FileImpl<FileObject, FileOverlapped64> File64;
typedef FileImpl<SocketFileObject, SocketOverlapped> SocketFile;

typedef File32 File;

// -------------------------------------------------------------------------

NS_CERL_END

#ifndef ASYNC_IOSERVICE_INL
#include "IoService.inl"
#endif

#endif /* ASYNC_IO_H */
