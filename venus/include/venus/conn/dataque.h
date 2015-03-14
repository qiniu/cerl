/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/conn/dataqueue.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: dataque.h 2743 2010-05-05 08:21:38Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_CONN_DATAQUE_H
#define VENUS_CONN_DATAQUE_H

#ifndef VENUS_BASIC_H
#include "../Basic.h"
#endif

#ifndef VENUS_IO_H
#include "../Io.h"
#endif

#ifndef VENUS_MAIL_H
#include "../Mail.h"
#endif

#ifndef ASYNC_DATAQUEUE_H
#include "../../../../async/include/async/DataQueue.h"
#endif

#ifndef STDEXT_HASHMAP_H
#include "../../../../stdext/include/stdext/HashMap.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class ConnectionImpl

template <class FileObjectT>
class DataQueueWriter : public DataQueue<FileObjectT>
{
	typedef DataQueue<FileObjectT> Base;

public:
	DataQueueWriter(FileObjectT& file, Pool& pool)
		: Base(file, pool)
	{
	}

	size_t cerl_call put(char c)
	{
		return Base::put(&c, 1);
	}

	using Base::put;
};

class ConnectionImpl
{
protected:
	typedef MailReader<SocketFile> Reader;
	typedef DataQueueWriter<SocketFileObject> Writer;
	
	typedef bool (cerl_callback *GetProc)(Reader& ar, void* param);
	
	enum { InvalidMailNo = -1 };
	enum { MailNoMask = 0x7FFFFFFF };
	enum { SlotCount = 64 };

	struct Slot
	{
		FiberMutex mutex;
		FiberMutex mailMutex;
		GetProc proc;
		void* param;
		MailNo mailno;
	};

	NInformation const host;
	SocketFileObject file;
	Writer writer;
	Pool& msgBufPool;
	UINT32 currMailNo;
	Slot slots[SlotCount];
	BOOL* pfConnectionOk;
	bool autoReconnect;

public:
	ConnectionImpl(SOCKET socket, const NInformation& hostArg, Pool& msgBufPoolArg)
		: host(hostArg), writer(file, msgBufPoolArg), msgBufPool(msgBufPoolArg),
	      currMailNo(0), pfConnectionOk(NULL), autoReconnect(true)
	{
		file.open_handle(socket);

		Fiber self = getCurrentFiber();
		writer.start(self, VENUS_CONNECTION_STACKSIZE);
		file.set_read_timeout(CERL_INFINITE);
		file.set_write_timeout(CERL_INFINITE);
		CerlIoService(self)->startFiber(self, ReceiverMain, this, VENUS_CONNECTION_STACKSIZE);
	}

	~ConnectionImpl()
	{
		if (pfConnectionOk)
			*pfConnectionOk = FALSE; // 通知 Receiver Fiber Connection 已经关闭
	}

	void cerl_call set_auto_reconnect(bool b)
	{
		autoReconnect = b;
	}

	const NInformation& cerl_call get_host() const
	{
		return host;
	}

	void cerl_call close()
	{
		file.close();
	}

	bool cerl_call good() const
	{
		return file.good();
	}

private:
	template <class ArgsT>
	bool cerl_call send(Fiber self, MailNo mailno, FID fid, const ArgsT& args)
	{
		CERL_ASSERT(good());

		CERL_VLOG("Connection", ("INFO: Connection::send - mail no: %d\n", mailno));
		MailHeader header = { sizeof(MailHeader), 0, VENUS_MAIL_MAGIC_CODE, mailno, fid };
		NS_STDEXT_IO_BINARY::put_struct(writer, header);
		NS_CERL::putMailBody(writer, args);
		writer.operate(self);
		CERL_VLOG("Connection", ("INFO: Connection::send - data sending finished.\n"));
		return true;
	}

	template <class ResultT>
	struct Traits
	{
		static bool cerl_callback getMailBody(Reader& ar, void* param)
		{
			return cerl::getMailBody(ar, *(ResultT*)param);
		}
	};

public:
	void cerl_call set_timeout(Timeout to)
	{
		file.set_read_timeout(to);
		file.set_write_timeout(to);
	}

	Timeout cerl_call get_timeout() const
	{
		return file.get_write_timeout();
	}

public:
	template <class ArgsT>
	bool cerl_call cast(FID fid, const ArgsT& args)
	{
		if (this == NULL || !file.good())
		{
			CERL_WARN("Connection", ("WARN: Connection::cast - invalid socket!\n"));
			return false;
		}

		MailNo const mailno = (++currMailNo & MailNoMask);
		Fiber self = getCurrentFiber();
		return send(self, mailno, fid, args);
	}

	template <class ResultT, class ArgsT>
	bool cerl_call call(ScopedAlloc& alloc, ResultT& result, FID fid, const ArgsT& args)
	{
		if (this == NULL || !file.good())
		{
			CERL_WARN("Connection", ("WARN: Connection::call - invalid socket!\n"));
			result = code_socket_error;
			return false;
		}

		MailNo const mailno = (++currMailNo & MailNoMask);
		Slot& slot = slots[mailno % SlotCount];

		Fiber self = getCurrentFiber();
		FiberMutex::scoped_lock lock(self, slot.mutex);

		slot.proc = Traits<ResultT>::getMailBody;
		slot.param = &result;
		slot.mailno = mailno;

		send(self, mailno, fid, args);

		yield(self);

		return true;
	}

private:
	void cerl_callback receiver_(Fiber self)
	{
		BOOL fConnectionOk = TRUE;
		pfConnectionOk = &fConnectionOk;

		SocketFile pfile(file);

		Reader reader(pfile, (char*)msgBufPool.allocate(), msgBufPool.alloc_size());
	
		MailHeader header;

		CERL_VLOG("Connection", ("INFO: Connection::receiver - receiver begin loop.\n"));
		for(;;)
		{
			CERL_VLOG("Connection", ("INFO: Connection::receiver - get header.\n"));
			if (!NS_STDEXT_IO_BINARY::get_struct(reader, header)
				|| VENUS_MAIL_MAGIC_CODE != header.magic 
				|| sizeof(MailHeader) != header.cbHeader)
			{
				CERL_WARN("Connection", ("WARN: Connection::receiver - read mail failed: mailno: %d, slot no: %d.\n", header.mailno, header.mailno % SlotCount));
				file.close();
				break;
			}
			Slot& slot = slots[header.mailno % SlotCount];
			{
				//@@bugfix: 这里有 (header.mailno & MailNoMask) == slot.mailno 的原因：
				//	因为后面紧跟的schedule函数会破坏FiberMutex的逻辑。
				//  所以要把schedule从FiberMutex限制的代码中抽出。
				//
				{
					FiberMutex::scoped_lock lock(self, slot.mailMutex);
					if ((header.mailno & MailNoMask) == slot.mailno)
					{
						CERL_VLOG("Connection", ("INFO: Connection::receiver - do unserialization. \n"));
						if (!slot.proc(reader, slot.param))
						{
							CERL_WARN("Connection", ("WARN: Connection::receiver - read mail failed!\n"));
							file.close();
							break;
						}
					}
					else
					{
						CERL_WARN("Connection", ("DROP: mail\n"));
						skipMailBody(reader);
					}
				}
				if ((header.mailno & MailNoMask) == slot.mailno)
				{
					CERL_VLOG("Connection", ("INFO: Connection::receiver - schedule to curr fiber. \n"));
					CerlIoService(self)->scheduleFiber(self);
					switchToFiber(self, slot.mutex.getCurrentFiber());
				}
			}
		}

		// Socket 已经关闭，现在我们进行一些清理操作：

		CERL_VLOG("Connection", ("INFO: Connection::receiver cleanup ...\n"));
		if (fConnectionOk)
		{
			pfConnectionOk = NULL;
			for (size_t i = 0; i < SlotCount; ++i)
			{
				Slot& slot = slots[i];
				if (slot.mutex.getCurrentFiber())
				{
					*(Code*)slot.param = code_socket_error;
					CerlIoService(self)->scheduleFiber(self);
					switchToFiber(self, slot.mutex.getCurrentFiber());
				}
			}
		}
	}
	
	static void cerl_callback ReceiverMain(LPVOID lpParam)
	{
		FiberParam p(lpParam);
		((ConnectionImpl*)p.val)->receiver_(p.self);
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#endif
