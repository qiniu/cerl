/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fileio/aio.h
// Creator: mohuafeng
// Email: longshanks@gmail.com
// Date: 2009-11-11 9:41:58
// 
// $Id: aio.h 619 2009-11-11 9:41:58 mohuafeng$
// -----------------------------------------------------------------------*/

#ifndef ASYNC_FILEIO_AIO_H
#define ASYNC_FILEIO_AIO_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#include <aio.h>
#include <fcntl.h>

NS_CERL_BEGIN


#define		SIG_ASYNC_AIO	(SIGRTMIN+13)

// -------------------------------------------------------------------------
// class FileOverlapped32/FileOverlapped64

class FileObject;

#pragma pack(1)

template <class OffsetT>
class FileOverlappedImpl
{
private:
	FileOverlappedImpl(const FileOverlappedImpl&);
	void operator=(const FileOverlappedImpl&);

	friend class FileObject;

public:
	typedef OffsetT offset_type;

public:
	FileOverlappedImpl(Fiber self)
		: m_offset(0), m_result(0)
	{
		init(self);
	}

	void cerl_call setFiber(Fiber fiber) {
		m_fiber = fiber;
	}

	Fiber cerl_call getFiber() const {
		return m_fiber;
	}

	void cerl_call move(offset_type delta) {
		m_offset += delta;
	}

	void cerl_call seek(offset_type offset) {
		m_offset = offset;
	}

	offset_type cerl_call tell() const {
		return m_offset;
	}

private:
	void init(Fiber self)
	{
		m_fiber = self;

		memset(&m_acb, 0, sizeof(m_acb));
		m_acb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
		m_acb.aio_sigevent.sigev_signo = SIG_ASYNC_AIO;
		m_acb.aio_sigevent.sigev_value.sival_ptr = this;
	}

private:
	Fiber				m_fiber;
	offset_type			m_offset;
	size_t				m_result;
	aiocb				m_acb;
	IoService::ScheduleMessage m_wakeup;
};

#pragma pack()

typedef FileOverlappedImpl<INT32> FileOverlapped32;
typedef FileOverlappedImpl<INT64> FileOverlapped64;

typedef FileOverlapped32 FileOverlapped;

// -------------------------------------------------------------------------
// class BaseFile, File

class FileObject
{
public:
	FileObject()
		: m_hFile(INVALID_HANDLE_VALUE)
	{
	}

public:
	typedef HANDLE handle_type;

	bool cerl_call good() const
	{
		return m_hFile != INVALID_HANDLE_VALUE;
	}

	HANDLE cerl_call detach()
	{
		HANDLE hFileOld = m_hFile;
		m_hFile = INVALID_HANDLE_VALUE;
		return hFileOld;
	}

	HRESULT cerl_call open_handle(HANDLE hFile)
	{
		return open_handle(getCurrentFiber(), hFile);
	}

	HRESULT cerl_call open_handle(Fiber self, HANDLE hFile)
	{
		CERL_ASSERT(!good());
		if (hFile == INVALID_HANDLE_VALUE)
			return E_ACCESSDENIED;

		m_hFile = hFile;

		return S_OK;
	}

	HRESULT cerl_call open_to_write(const char* file)
	{
		return open_to_write(getCurrentFiber(), file);
	}

	HRESULT cerl_call open_to_write(Fiber self, const char* file)
	{
		return open_handle(self, (HANDLE)::open(file, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP));
	}

	HRESULT cerl_call open_to_read(const char* file)
	{
		return open_to_read(getCurrentFiber(), file);
	}

	HRESULT cerl_call open_to_read(Fiber self, const char* file)
	{
		return open_handle(self, (HANDLE)::open(file, O_RDONLY));
	}

	void cerl_call close()
	{
		::close((int)(size_t)m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

public:
	template <class OverlappedT>
	ptrdiff_t cerl_call read_some(OverlappedT& o, void* buf, size_t cb)
	{
		CERL_VLOG("File", ("INFO: FileObject::read_some - o.fiber: %p\n", o.m_fiber));
		CERL_FIBER_ASSERT_SELF(o.m_fiber); 

		SetAioBuf(o.m_acb, (int)(size_t)m_hFile, buf, o.m_offset, cb);
		o.m_wakeup = CerlIoService(o.m_fiber)->messageOfSchedule(o.m_fiber);

		if (aio_read(&o.m_acb) < 0)
			return 0;

		yield(o.m_fiber);
		o.m_offset += o.m_result;
		return o.m_result;
	}

	template <class OverlappedT>
	ptrdiff_t cerl_call write_some(OverlappedT& o, const void* buf, size_t cb)
	{
		CERL_VLOG("File", ("INFO: FileObject::write_some - o.fiber: %p\n", o.m_fiber));
		CERL_FIBER_ASSERT_SELF(o.m_fiber);

		SetAioBuf(o.m_acb, (int)(size_t)m_hFile, (void*)buf, o.m_offset, cb);
		o.m_wakeup = CerlIoService(o.m_fiber)->messageOfSchedule(o.m_fiber);

		//CERL_TRACE("   before aio write.\n");
		if (aio_write(&o.m_acb) < 0)
		{
			perror("     aio error:");
			return 0;
		}

		//CERL_TRACE("   before yield.\n");
		yield(o.m_fiber);
		o.m_offset += o.m_result;
		return o.m_result;
	}

	static void SetAioBuf(aiocb& acb, int hFile, void* buf, size_t offset, size_t cb)
	{
		acb.aio_fildes = hFile;
		acb.aio_buf = buf;
		acb.aio_nbytes = cb;
		acb.aio_offset = offset;
	}

	static void AioCallback(int event, siginfo_t* info, void*)
	{
		CERL_VLOG("File", ("  start aio callback.\n"));
		FileOverlapped* data = (FileOverlapped*)(info->si_value.sival_ptr);

		if(aio_error(&data->m_acb) == 0)
			data->m_result = aio_return(&data->m_acb);
		else
			data->m_result = 0;

		data->m_wakeup.postMT();
	} 

protected:
	HANDLE		m_hFile;
}; 


inline void cerl_call AioInitialize()
{
	struct sigaction sigact;
	memset(&sigact, 0, sizeof(sigact));
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO | SA_RESTART;
	sigact.sa_sigaction = FileObject::AioCallback;
	if(sigaction(SIG_ASYNC_AIO, &sigact, NULL) == -1)
	{
		perror("sigaction error: ");
	}
}

inline void cerl_call AioUninitialize()
{
}

// -------------------------------------------------------------------------
// class AioInit

class AioInit
{
public:
	AioInit() { AioInitialize(); }
	~AioInit() { AioUninitialize(); }
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif // ASYNC_FILEIO_AIO_H
