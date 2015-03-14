/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/fileio/windows.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: windows.h 2754 2010-05-05 10:25:27Z lijie $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FILEIO_WINDOWS_H
#define ASYNC_FILEIO_WINDOWS_H

#ifndef ASYNC_IOSERVICE_H
#include "../IoService.h"
#endif

#ifndef _INC_IO
#include <io.h>
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// AioInitialize/AioUninitialize

inline void cerl_call AioInitialize() {}
inline void cerl_call AioUninitialize() {}

class AioInit
{
};

// -------------------------------------------------------------------------
// class SocketOverlapped/FileOverlapped32/FileOverlapped64

#pragma pack(1)

class SocketOverlapped
{
private:
	SocketOverlapped(const SocketOverlapped&);
	void operator=(const SocketOverlapped&);

public:
	Fiber fiber;

public:
	explicit SocketOverlapped(Fiber fiber) {
		this->fiber = fiber;
	}

	void cerl_call setFiber(Fiber fiber) {
		this->fiber = fiber;
	}

	Fiber cerl_call getFiber() const {
		return fiber;
	}
};

template <class OffsetT>
class FileOverlappedImpl
{
private:
	FileOverlappedImpl(const FileOverlappedImpl&);
	void operator=(const FileOverlappedImpl&);

public:
	typedef OffsetT offset_type;

	Fiber fiber;
	offset_type offset;

public:
	explicit FileOverlappedImpl(Fiber fiber) {
		this->fiber = fiber;
		this->offset = 0;
	}

	void cerl_call setFiber(Fiber fiber) {
		this->fiber = fiber;
	}

	Fiber cerl_call getFiber() const {
		return fiber;
	}

	void cerl_call move(offset_type delta) {
		offset += delta;
	}

	void cerl_call seek(offset_type offset) {
		this->offset = offset;
	}

	offset_type cerl_call tell() const {
		return offset;
	}
};

typedef FileOverlappedImpl<UINT32> FileOverlapped32;
typedef FileOverlappedImpl<UINT64> FileOverlapped64;
typedef FileOverlapped32 FileOverlapped;

#pragma pack()

// -------------------------------------------------------------------------
// type OVERLAPPED_IO

#pragma pack(1)

namespace detail {

struct OVERLAPPED_IO : IoService::OVERLAPPED_IO
{
	template <class OverlappedT>
	void cerl_call init_(OverlappedT& o, size_t& ret)
	{
		ZeroMemory(&Overlapped, sizeof(Overlapped));
		fiber = o.fiber;
		sn = CerlFiberSerialNo(fiber);
		result = &ret;
	}

	void cerl_call init(SocketOverlapped& o, size_t& ret)
	{
		init_(o, ret);
	}

	void cerl_call init(FileOverlapped32& o, size_t& ret)
	{
		init_(o, ret);
		Overlapped.Offset = o.offset;
	}

	void cerl_call init(FileOverlapped64& o, size_t& ret)
	{
		init_(o, ret);
		Overlapped.Offset = (DWORD)o.offset;
		Overlapped.OffsetHigh = (DWORD)(o.offset >> 32);
	}
};

typedef IoService::OVERLAPPED_ACCEPT OVERLAPPED_ACCEPT;

} // namespace detail

#pragma pack()

// -------------------------------------------------------------------------
// class FileObject

class FileObject
{
protected:
	HANDLE hFile;

public:
	typedef HANDLE handle_type;
	typedef FileOverlapped overlapped_type;

public:
	FileObject()
		: hFile(INVALID_HANDLE_VALUE)
	{
	}
	~FileObject()
	{
		if (hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
	}

	bool cerl_call good() const
	{
		return hFile != INVALID_HANDLE_VALUE;
	}

	HANDLE cerl_call detach()
	{
		HANDLE hFileOld = hFile;
		hFile = INVALID_HANDLE_VALUE;
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

		CerlIoService(self)->iocpMT.bind(hFile, (ULONG_PTR)IoService::ClientIoReadWrite);
		this->hFile = hFile;
		return S_OK;
	}

	HRESULT cerl_call open_to_write(const char* file)
	{
		return open_to_write(getCurrentFiber(), file);
	}

	HRESULT cerl_call open_to_write(Fiber self, const char* file)
	{
		return open_handle(self, CreateFileA(
			file, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL));
	}

	HRESULT cerl_call open_to_read(const char* file)
	{
		return open_to_read(getCurrentFiber(), file);
	}

	HRESULT cerl_call open_to_read(Fiber self, const char* file)
	{
		return open_handle(self, CreateFileA(
			file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL));
	}

	void cerl_call close()
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}

	template <class OverlappedT>
	size_t cerl_call read_some(OverlappedT& o, void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.fiber);

		using namespace detail;

		size_t result = 0;
		IoService* const service = CerlIoService(o.fiber);
		OVERLAPPED_IO* const ovlp = (OVERLAPPED_IO*)service->overlappedPool.allocate();
		ovlp->init(o, result);

		if (!ReadFile(hFile, buf, cb, NULL, &ovlp->Overlapped))
		{
			const DWORD nRet = GetLastError();
			if (nRet != ERROR_IO_PENDING) {
				CERL_WARN("File", ("WARN: FileObject::read_some: ReadFile failed: %d\n", nRet));
				return 0;
			}
		}
		service->yield(o.fiber);
		o.move(result);
		return result;
	}

	template <class OverlappedT>
	size_t cerl_call write_some(OverlappedT& o, const void* buf, size_t cb)
	{
		CERL_FIBER_ASSERT_SELF(o.fiber);

		using namespace detail;

		size_t result = 0;
		IoService* const service = CerlIoService(o.fiber);
		OVERLAPPED_IO* const ovlp = (OVERLAPPED_IO*)service->overlappedPool.allocate();
		ovlp->init(o, result);

		if (!WriteFile(hFile, buf, cb, NULL, &ovlp->Overlapped))
		{
			const DWORD nRet = GetLastError();
			if (nRet != ERROR_IO_PENDING) {
				CERL_WARN("File", ("WARN: FileObject::write_some: WriteFile failed: %d\n", nRet));
				return 0;
			}
		}
		service->yield(o.fiber);
		o.move(result);
		return result;
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_FILEIO_WINDOWS_H */
