#include <async/lockfree/Pool.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <pthread.h>
#include <list>

using namespace std;

pthread_t ntid[10];
pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER;
list<void *> lv;

void
printids(const char *s)
{
    pid_t      pid;
    pthread_t  tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
           (unsigned int)tid, (unsigned int)tid);
    
}

struct Para
{
    cerl::PoolMT* poolmt;
    void* obj;
    int num;
};


void *
thr_fn(void *arg)
{
    Para* para = (Para*)arg;
    printf("#threads: %d, the deallocated addr: %p\n", para->num, para->obj);
    para->poolmt->deallocate(para->obj);
    printids("new thread: ");
    return((void *)0);
}

void *
thr_fn2(void *arg)
{
    cerl::PoolMT* poolmt = (cerl::PoolMT*)arg;
    printids("new thread: ");
    void *p = NULL;
    pthread_mutex_lock(&mutex);
    if (!lv.empty())
    {
        if (rand() % 2)
        {
            p = lv.front();
            lv.pop_front();
        }
        else
        {
            p = lv.back();
            lv.pop_back();
        }
    }
    pthread_mutex_unlock(&mutex);
    if (p) poolmt->deallocate(p);
    //printids("new thread: ");
    return((void *)0);
}

int main()
{
    cerl::Pools pools;
    cerl::PoolMT poolmt(pools, 64);
    pthread_t ntid;
    for(int i = 0; i < 100; ++i)
    {
        if (rand() % 2)
        {
            void* p = poolmt.allocate();
            pthread_mutex_lock(&mutex);
            lv.push_back(p);
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            int err = pthread_create(&ntid, NULL, thr_fn2, (void*)&poolmt);
            if (err != 0)
            {
                printf("can't create thread: %s\n", strerror(err));
            }
        }
    }
    /*
      while(true)
      {
      void* p[10];
      Para para[10];
      for(int i = 0; i < 10; ++i)
      {
      p[i] = poolmt.allocate();
      printf("allocate addr: %p\n", p[i]);
      para[i].poolmt = &poolmt;
      para[i].obj = p[i];
      para[i].num = i;
      }
      int err[10];
      for(int i = 0; i < 10; ++i)
      {
      err[i] = pthread_create(&ntid[i], NULL, thr_fn, (void*)&para[i]);
      if (err[i] != 0)
      {
      printf("can't create thread: %s\n", strerror(err[i]));
      }
      }
      //printids("main thread:");
      break;
      }
      for(int i = 0; i < 10; ++i)
      {
      pthread_join(ntid[i], NULL);
      }
    */
    sleep(5);

  	return 0;
}

