/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/FastTimer.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: FastTimer.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_FASTTIMER_H
#define ASYNC_FASTTIMER_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef ASYNC_LIST_H
#include "List.h"
#endif

#ifndef ASYNC_LOCKFREE_POOL_H
#include "lockfree/Pool.h"
#endif

#ifndef STDEXT_THREAD_MUTEX_H
#include "../../../stdext/include/stdext/thread/Mutex.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class FastTimer

typedef NS_STDEXT::Mutex FastTimerMutex;

class FastTimerQueue;
class FastTimer : private DListNode
{
public:
    typedef unsigned size_type;
    typedef unsigned duration_type;

private:
    friend class FastTimerQueue;

    FastTimerMutex* volatile m_mutex;
    size_type m_cycle;
    
    PoolMT* m_pool;
    LONG m_ref;
    
protected:
    FastTimer()
        : m_mutex(NULL), m_ref(1), m_pool(NULL)
    {
    }

public:
    bool cerl_call stopped() const
    {
        return m_mutex == NULL;
    }

    void cerl_call stop()
    {
        FastTimerMutex* mutex = m_mutex;
        if (mutex)
        {
            FastTimerMutex::scoped_lock lock(*mutex);
            DListNode::erase();
            release();
            m_mutex = NULL;
        }
        release();
    }
    
    virtual void cerl_call onTimer(void* caller) = 0;

    template <class TimerT>
    static void cerl_call create(TimerT** pp, PoolMT* pool)
    {
        TimerT* o = new(pool->allocate()) TimerT;
        o->m_pool = pool;
        *pp = o;
    }
    
private:
    void cerl_call acquire()
    {
        InterlockedIncrement(&m_ref);
    }
    
    void cerl_call release()
    {
        if (InterlockedDecrement(&m_ref) == 0)
        {
            CERL_ASSERT(stopped());
            m_pool->deallocate(this);
            // NOTE: ~Timer 析构没有调用，我们假设不会有什么析构需求
        }
    }
};

// =========================================================================
// class FastTimerQueue

class FastTimerQueue
{
public:
    typedef FastTimer::size_type size_type;
    typedef FastTimer::duration_type duration_type;

private:
    struct FastTimerSlot
    {
        DList nodes;
        FastTimerMutex mutex;
    };

    FastTimerSlot* m_slots;
    size_type m_slot_count;
    size_type m_curr;
    duration_type m_precision; // 最小精度(in ms)，建议不要低于10ms

public:
    explicit FastTimerQueue(duration_type precision = 500, size_type slot_count = 31)
        : m_slot_count(slot_count),
          m_curr(0),
          m_precision(precision)
    {
        CERL_ASSERT(precision >= 10);
        CERL_ASSERT(slot_count >= 1);

        m_slots = new FastTimerSlot[slot_count];
    }

    ~FastTimerQueue()
    {
        delete[] m_slots;
    }

    duration_type cerl_call getPrecision() const
    {
        return m_precision;
    }

    void cerl_call start(FastTimer* timer, duration_type timeout)
    {
        CERL_ASSERT(timer->stopped());

        const size_type ticks = timeout / m_precision;
        if (ticks == 0)
        {
            timer->onTimer(this);
            return;
        }

        FastTimerSlot& slot = m_slots[(ticks + m_curr) % m_slot_count];
        timer->m_cycle = ticks / m_slot_count;
        timer->m_mutex = &slot.mutex;
        ++timer->m_ref;
        
        FastTimerMutex::scoped_lock lock(slot.mutex);
        slot.nodes.push_back(timer);
    }

    void cerl_call restart(FastTimer* timer, duration_type timeout)
    {
        timer->stop();
        start(timer, timeout);
    }

    void cerl_call tick()
    {
        DList expiredTimers;

        {
            FastTimerSlot& slot = m_slots[m_curr++ % m_slot_count];
            FastTimerMutex::scoped_lock lock(slot.mutex);
            for (FastTimer* timer = (FastTimer*)slot.nodes.first(); !slot.nodes.done(timer); )
            {
                if (timer->m_cycle == 0)
                {
                    FastTimer* nextTimer = (FastTimer*)timer->next();
                    timer->erase();
                    //timer->acquire();
                    timer->m_mutex = NULL;
                    expiredTimers.push_back(timer);
                    timer = nextTimer;
                }
                else
                {
                    --timer->m_cycle;
                    timer = (FastTimer*)timer->next();
                }
            }
        }

        for (FastTimer* timer = (FastTimer*)expiredTimers.first(); !expiredTimers.done(timer); )
        {
            FastTimer* nextTimer = (FastTimer*)timer->next();
            timer->onTimer(this); // NOTE: 将 onTimer 从上面挪出来，是因为 onTimer 有可能阻塞（如 IoService 消息队列满）
            timer->release();
            timer = nextTimer;
        }
    }
};

// =========================================================================

NS_CERL_END

#endif /* ASYNC_FASTTIMER_H */
