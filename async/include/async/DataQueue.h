/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/DataQueue.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: DataQueue.h 2495 2010-04-16 10:28:47Z xushiwei $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_DATAQUEUE_H
#define ASYNC_DATAQUEUE_H

#ifndef ASYNC_LIST_H
#include "List.h"
#endif

#ifndef ASYNC_IO_H
#include "Io.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class DataQueue

#pragma pack(1)

template <class FileObjectT>
class DataQueue
{
public:
	typedef size_t size_type;

private:
	struct Block : ListNode
	{
		size_type bytes;
	};

	Block* m_product;

	Queue m_mq;
	FileObjectT& m_fileobj;
	Pool& m_pool;
	size_type const m_blkSize;

	Fiber m_consume;
	BOOL m_waiting;

public:
	DataQueue(FileObjectT& file, Pool& pool)
		: m_fileobj(file), m_pool(pool), m_blkSize(pool.alloc_size())
	{
		m_product = (Block*)pool.allocate();
		m_product->bytes = sizeof(Block);
		m_mq.push(m_product);
		m_consume = NULL;
		m_waiting = FALSE;
	}

	~DataQueue()
	{
		while (!m_mq.empty())
			m_pool.deallocate(m_mq.pop());
	}

private:
	void cerl_call do_put_(const char* buf, size_type bytes)
	{
		const size_type blkRest = m_blkSize - m_product->bytes;
		memcpy((char*)m_product + m_product->bytes, buf, blkRest);
		m_product->bytes = m_blkSize;
		buf += blkRest;
		bytes -= blkRest;

		const size_type blkFree = m_blkSize - sizeof(Block);
		while (bytes >= blkFree)
		{
			Block* blk = (Block*)m_pool.allocate();
			memcpy((char*)blk + sizeof(Block), buf, blkFree);
			blk->bytes = m_blkSize;
			m_mq.push(blk);
			buf += blkFree;
			bytes -= blkFree;
		}

		m_product = (Block*)m_pool.allocate();
		memcpy((char*)m_product + sizeof(Block), buf, bytes);
		m_product->bytes = sizeof(Block) + bytes;
		m_mq.push(m_product);
	}

public:
	size_type cerl_call put(const char* buf, size_type bytes)
	{
		CERL_VLOG("DataQueue", ("INFO: DataQueue::put - data size: %d, block bytes: %d.\n", bytes, m_product->bytes));
		if (m_product->bytes + bytes < m_blkSize)
		{
			memcpy((char*)m_product + m_product->bytes, buf, bytes);
			m_product->bytes += bytes;
			CERL_VLOG("DataQueue", ("INFO: DataQueue::put - less than block size. prod bytes: %d\n", m_product->bytes));
		}
		else
		{
			do_put_(buf, bytes);
			CERL_VLOG("DataQueue", ("INFO: DataQueue::put - greater than block size, do put.\n"));
		}
		return bytes;
	}

	bool cerl_call operate(Fiber self)
	{
		CERL_FIBER_ASSERT_SELF(self);

		if (m_consume == NULL)
		{
			CERL_WARN("DataQueue", ("WARN: DataQueue::operate failed - consume service closed.\n"));
			return false;
		}
		if (m_waiting)
		{
			CerlIoService(self)->scheduleFiber(self);
			switchToFiber(self, m_consume);
		}
		return true;
	}

	bool cerl_call put_message(Fiber self, const char* msgBuf, size_type msgSize)
	{
		put(msgBuf, msgSize);
		return operate(self);
	}

public:
	void cerl_call start(Fiber self, size_type dwStackSize = 0)
	{
		CERL_FIBER_ASSERT_SELF(self);
		CERL_ASSERT(m_consume == NULL);

		m_consume = CerlIoService(self)->startFiber(self, consume, this, dwStackSize);
	}

private:
	void cerl_call do_consume(Fiber self)
	{
		typedef FileImpl<FileObjectT> ProxyFileT;
		
		CERL_FIBER_ASSERT_SELF(self);

		ProxyFileT file(self, m_fileobj);
		CERL_VLOG("DataQueue", ("INFO: DataQueue::do_consume - begin loop\n"));
		for (;;)
		{
			Block* p = (Block*)m_mq.pop();
			CERL_VLOG("DataQueue", ("INFO: DataQueue::do_consume - got block: size: %d\n", p->bytes));
			size_type off = sizeof(Block);
			for (;;)
			{
				while (off < p->bytes)
				{
					CERL_VLOG("DataQueue", ("INFO: DataQueue::do_consume - write to socket one block.\n"));
					const size_type written = file.write_some((char*)p + off, p->bytes - off);
					CERL_VLOG("DataQueue", ("INFO: DataQueue::do_consume - one block socket writing finished.\n"));
					if (written == 0)
					{
						m_consume = NULL;
						m_fileobj.close();
						return;
					}
					off += written;
				}
				if (off == m_blkSize)
				{
					CERL_ASSERT(!m_mq.empty());
					m_pool.deallocate(p);
					break;
				}
				m_waiting = TRUE;
				CERL_VLOG("DataQueue", ("INFO: DataQueue::do_consume - socket writing finished. yield\n", p->bytes));
				yield(self);
				CERL_VLOG("DataQueue", ("INFO: DataQueue::do_consume - continue socket writing.\n", p->bytes));
				m_waiting = FALSE;
			}
		}
	}

	static void cerl_callback consume(LPVOID lpParam)
	{
		FiberParam p(lpParam);
		((DataQueue*)p.val)->do_consume(p.self);
	}
};

#pragma pack(1)

// -------------------------------------------------------------------------

NS_CERL_END

#endif /* ASYNC_DATAQUEUE_H */
