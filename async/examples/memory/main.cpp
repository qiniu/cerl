#define WIN32_LEAN_AND_MEAN 
#include <async/Memory.h>

inline void testLog2()
{
	using namespace NS_STDEXT::detail;

	for (;;)
	{
		int v;
		scanf("%d", &v);
		if (v == 0)
			break;
		v = log2(v);
		printf("%d - %d\n", v, 1<<v);
	}
}

inline void testPools()
{
	enum { MAX_BYTES = cerl::Pools::MAX_BYTES };

	cerl::Pools pools;
	
	WINX_ASSERT(pools.has_pool(MAX_BYTES-1));
	WINX_ASSERT(!pools.has_pool(MAX_BYTES));

	cerl::Pool& pool1 = pools.get_pool(MAX_BYTES);
	WINX_ASSERT(pool1.alloc_size() == MAX_BYTES);

	void* p1 = pool1.allocate();
	pool1.deallocate(p1);

	cerl::Pool& pool2 = pools.get_pool(MAX_BYTES/2);
	CERL_ASSERT(&pool1 == &pool2);

	void* p2 = pool2.allocate();
	CERL_ASSERT(p1 == p2);

	{
		void* p1 = pools.get_pool(128).allocate();
		void* p2 = pools.get_pool(129).allocate();
		void* p3 = pools.get_pool(252).allocate();
		void* p4 = pools.get_pool(253).allocate();
	}
}

int main()
{
	testPools();
	return 0;
}
