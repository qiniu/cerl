/*
@encoding utf8
@ns cerl
@*/

// -------------------------------------------------------------------------
// class BlockPool

/** BlockPool为上层其他内存分配器提供了底层机制支持，内部维护大块预开辟内存，有效优化频繁小内存的分配释放
	BlockPool的用法为， 在每线程中定义一个实例，然后将其用于其他上层分配器的下层
	BlockPool并不是直接提供接口给外部调用的，只有提供给各个Alloc实现的纯粹方法
@see ScopedAlloc, Pool, Pools
@*/
class BlockPool
{
	//@members
	enum Padding;
	enum BlockSize;
	//@end

	/** BlockPool初始化。
	@arg [in] cbFreeLimit
		为了加速内存的申请和释放，BlockPool释放内存时并不真正释放，而是缓存到一个FreeList中。这里cbFreeLimit是设置FreeList的内存上限。在达到该上限时，BlockPool 不再继续缓存更多的内存。
	@*/
	BlockPool(int cbFreeLimit = INT_MAX);

	/** 使用BlockPool进行内存分配。
	@arg [in] cb
		分配内存的大小。
	@*/
	void* cerl_call allocate(size_t cb);

	/** 释放由BlockPool分配出的内存块。
	@arg [in] p
		指向要释放内存块的指针。
	@remark
		将从BlockPool分配的内存，归还给BlockPool
	@*/
	void cerl_call deallocate(void* p);

	/** 获取指定内存空间的大小。
	@arg [in] p
		指向存储空间的指针。
	@return
		返回存储空间的大小。
	@*/
	size_t cerl_call alloc_size(void* p);

	/** 清空内部所有内存块，回到初始化状态
	@*/
	void cerl_call clear();
};

// -------------------------------------------------------------------------
// class ScopedAlloc

/** ScopedAlloc提供了通过内存池来分配和释放内存的方法，大块内存直接调用系统分配方式
@see BlockPool
@*/
class ScopedAlloc
{
	//@members
	enum MemBlockSize;
	typedef size_type;
	typedef alloc_type;
	//@end

	/** ScopedAlloc初始化。
	@arg [in] alloc
		BlockPool对象。
	@*/
	ScopedAlloc(BlockPool& alloc);

	/** 取得内部Pool实例
	@return
		当前Alloc使用的内部Pool实例
	@see BlockPool
	@*/
	BlockPool& cerl_call get_alloc() const;

	/** 与另一个ScopedAlloc对象做交换。
	@arg [in] o
		ScopedAlloc对象。
	@remark TODO
	@*/
	void cerl_call swap(ScopedAlloc& o);

	/** 释放ScopedAlloc分配的所有内存。
	@*/
	void cerl_call clear();

	/** 申请size为 cb 大小的内存块
	@*/
	void* cerl_call allocate(size_type cb);

	/** 调整已有内存块的大小
	@*/
	void* cerl_call reallocate(void* p, size_type oldSize, size_type newSize);

	/** 空函数， 为代码兼容性考虑
	@remark 用于模板实现的其他库中调用此方法
	@*/
	void cerl_call deallocate(void* p, size_type cb);

	/** 空函数， 为代码兼容性考虑
	@remark 用于模板实现的其他库中调用此方法
	@*/
	void cerl_call deallocate(void* p);
	
	/** 分配一块指定了析构时需要调用方法的内存空间，但尚未加入析构链表。
		稍后通过manage方法加入析构链表的内存将会在clear的时候调用指定的destroy方法。
		
	@remark	对比自动加入析构队列的方法 allocate(size_t cb, destructor_t fn)
		考虑到在目前的应用场景中(宏包装方式)， allocate(size_t cb, destructor_t fn)来new出一个对象的时候，
		存在一种情况，即内存分配成功， 但new对象失败，此时资源的清理不彻底。
		为解决这个问题， 使用umanaged_alloc, manage配套分步使用的方式
		
	@arg [in] cb
		要分配的内存大小。
	@arg [in] fn
		一个函数指针，如果是一个加入到m_destroyChain链表的内存单元，
		在clear的时候将会调用这个函数。
	@return
		返回一个指针，指向分配好的内存。
	@see manage(void* p, destructor_t fn)
	@*/
	void* cerl_call unmanaged_alloc(size_type cb, destructor_t fn);

	/** 分配一块内存空间，分配方法和\<allocate>一致。
	@arg [in] cb
		要分配的内存大小。
	@arg [in] fnZero
		该参数没有被使用。
	@return
		返回一个指针，指向分配好的内存。
	@*/
	void* cerl_call unmanaged_alloc(size_type cb, int fnZero);

	/** 将使用unmanaged_alloc分配的内存空间加入到m_destroyChain。
		unmanaged_alloc + manage 相当于allocate(size_t cb, destructor_t fn)。
	@arg [in] p
		指向由unmanaged_alloc分配的内存空间的指针。
	@arg [in] fn
		一个函数指针，如果是一个加入到m_destroyChain链表的内存单元，
		在clear的时候将会调用这个函数。
	@see unmanaged_alloc(size_type cb, destructor_t fn)
	@*/
	void cerl_call manage(void* p, destructor_t fn);

	/** 空函数， 为代码兼容性考虑
	@remark 用于模板实现的其他库中调用此方法
	@*/
	void cerl_call manage(void* p, int fnZero);
	
	/** 空函数， 为代码兼容性考虑
	@remark 用于模板实现的其他库中调用此方法
	@*/
	template <class Type>
	void cerl_call destroy(Type* obj);

	/** 空函数， 为代码兼容性考虑
	@remark 用于模板实现的其他库中调用此方法
	@*/
	template <class Type>
	void cerl_call destroyArray(Type* array, size_type count);
};

// -------------------------------------------------------------------------
// class ScopedPool

/** ScopedPool提供了通过内存池来分配内存的方法
@see SystemAlloc, BlockPool
@*/
class ScopedPool
{
	//@members
	typedef alloc_type;
	typedef size_type;
	enum MemBlockSize;
	//@end

	/** 初始化ScopedPool。
	@arg [in] alloc
		BlockPool对象。
	@arg [in] cbElem
		每一次内存分配请求的内存单元大小。 < 16KB
	@*/
	ScopedPool(BlockPool& alloc, size_type cbElem);

	/** 取得分配单元的size
	@return
		返回当前ScopedPool实例分配单元的size。
	@*/
	size_type cerl_call alloc_size() const;
	
	/** 获取BlockPool对象
	@*/
	BlockPool& cerl_call get_alloc() const;

	/** 释放ScopedPool分配的所有内存。
	@*/
	void cerl_call clear();

	/** 分配一块ScopedPool实例中分配单元size的内存
	@return
		返回一个指向分配内存的指针。
	@*/
	void* cerl_call allocate();
	
	/** 处理一次内存分配请求
	@arg [in] cb
		请求分配的内存size
	@return
		返回分配的内存地址
	@remark
		ScopedPool只支持分配固定大小的内存，如果参数cb和初始化参数cbElem不同，
		仍将按照cbElem的大小来分配。因此参数cb的大小不能超过cbElem。
	@*/
	void* cerl_call allocate(size_type cb);

	/** 释放由一块由ScopedPool分配出的内存空间。
	@arg [in] p
		待释放的内存地址
	@remark
		ScopedPool释放出的内存将归还给ScopedPool的空闲chunk列表。
	@*/
	void cerl_call deallocate(void* const p);

	/** 释放由一块由ScopedPool分配出的内存空间。
	@arg [in] p
		待释放的内存地址
	@arg [in] cb
		释放内存空间的大小。输入的参数cb应和一次请求分配内存空间的大小一致。
		如果参数cb和初始化参数cbElem不同，释放的内存空间大小仍是cbElem。
	@remark
		ScopedPool释放出的内存将归还给ScopedPool的空闲chunk列表。
	@*/
	void cerl_call deallocate(void* const p, size_type cb);
};

// -------------------------------------------------------------------------
// class Pools

/** 内存池(Pools)集合。
@see Pool, BlockPool
@*/
class Pools
{
	//@members
	typedef size_type;
	typedef Pool pool_type;

	enum MAX_BYTES;
	//@end

	/** Pools初始化。
	@*/
	Pools();

	/** 根据用户的要求在不同的内存池分配内存。
	@arg [in] cb
		要求分配内存的size。
	@return
		返回一个指针，指向分配的内存空间。
	@*/
	Pool& cerl_call get_pool(size_type cb);
	
	/** 判断是否超过ScopedPools的MAX_BYTES。
	@arg [in] cb
		指定的大小。
	@return
		@val True
			没有超过MAX_BYTES。
		@val Flase
			超过了MAX_BYTES。
	@*/
	bool cerl_call has_pool(size_type cb) const;
};