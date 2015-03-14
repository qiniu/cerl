/*
@encoding utf8
@ns cerl
@*/

/**	BufferReader类提供了一个文件缓冲区来加速文件的读取速度。
	其支持的操作包括：文件中单个字符的快速读取、字符串快速读取等操作。
@*/
template <class FileT>
class BufferReader
{
	//@members
	enum endch;			// 表示结束字符
	typedef size_type;	// 表示缓冲区的大小
	typedef char_type;	// 表示以char为结束
	typedef uchar_type;	// 表示以unsigned char为结束
	typedef int_type;
	typedef range_type; // 表示缓冲区的大小
	//@end

	/**	初始化文件读取缓冲区。
	@arg [in] file
		要读取的文件句柄。
	@arg [in] buf
		读入文件的缓冲区。
	@arg [in] cb
		文件缓冲区大小。
	@*/
	BufferReader(FileT& file, char_type* buf, size_type cb);
	
	/** 获取缓冲区的地址。
	@return
		返回缓冲区地址。
	@*/
	const void* cerl_call get_buffer();
	
	/** 获取进行读操作的文件。
	@return
		返回一个文件句柄。
	@*/
	FileT& cerl_call get_file();
	
	/** 清空缓冲区
		把缓冲区内数据失效
	@*/
	void cerl_call clear();

	/**	读出文件缓冲区中存储的文件数据。
	@return
		返回文件缓冲区中存储的文件数据字符串。
	@*/
	range_type cerl_call get_some();

	/**	将文件缓冲区的指针回退指定长度。
	@arg [in] count
		指针回退的长度。
	@remark
		缓冲区指针回退的距离是有限度的，不能使指针落在缓冲区之外。
	@*/
	void cerl_call unget_some(size_type count);

	/**	获取文件缓冲区的大小。
	@return
		返回文件缓冲区的大小。
	@*/
	size_type cerl_call max_bytes();

	/**	从文件缓冲区指针开始的位置计算，读取指定字符长度的文件数据。
		其中，获取的字符长度不能超过缓冲区的尺寸。
	@arg [in] cb
		要获取文件数据的字符数。
	@return
		如果peek_message执行成功，将返回要读取数据的起始地址；
		如果peek_message执行失败，则返回NULL。
	@remark
		如果获取的字符长度超过缓冲区定义的大小，peek_message会执行失败（返回NULL），
		且此后该 BufferReader 对象状态未定义，不可继续调用其任何函数。
	@*/
	const char* cerl_call peek_bytes(size_type cb);
	
	/**	从文件缓冲区指针开始的位置计算，读取指定字符长度的文件数据。
		同时，文件缓冲区指针向后移动指定的字符长度。
		其中，获取的字符长度不能超过缓冲区的尺寸。
	@arg [in] cb
		要获取文件数据的字符数。
	@return
		如果get_message执行成功，将返回要读取数据的起始地址；
		如果get_message执行失败，则返回NULL。
	@remark
		如果获取的字符长度超过缓冲区定义的大小，get_message会执行失败，程序将退出。
	@*/
	const char* cerl_call get_bytes(size_type cb);
	
	/**	取出文件缓冲区中要读出的下一个字符。读出字符的同时，文件缓冲区指针不向后移动。
	@return
		文件缓冲区中要读出的下一个字符。
	@*/
	int_type cerl_call peek();

	/**	取出文件缓冲区中要读出的下一个字符，读出字符的同时，文件缓冲区指针向后移动一个字符。
	@return
		文件缓冲区中要读出的下一个字符。如果读到文件尾部，则返回文件结束符。
	@*/
	int_type cerl_call get();

	/**	跳过缓冲区中的若干字节的数据。
	@arg [in] cb
		跳过的字节数。
	@return
		返回缓冲区跳过的字节数。
	@*/
	size_type cerl_call skip(size_type cb);

	/**	将文件缓冲区数据中一定长度的内容读入指定的缓冲区。
	@arg [out] buf
		要读入文件的缓冲区地址。
	@arg [in] cb
		要读入的文件长度。
	@return
		实际读取文件的长度。
	@*/
	size_type cerl_call get(char_type* buf, size_type cb);
};

// =========================================================================
// class BufferedWriter

/** BufferedWriter 类提供了一个缓冲区来加速文件的写入速度。
	其支持的操作包括：文件中单个字符的快速读取、字符串快速写入等操作。
@*/
template <class FileT>
class BufferedWriter
{
	/**	初始化文件写入缓冲区。
	@arg [in] file
		要写入数据的文件句柄。
	@arg [in] buf
		写入文件的缓冲区。
	@arg [in] cb
		文件缓冲区大小。
	@*/
	BufferedWriter(FileT& file, char_type* buf, size_type cb);

	/** 获取写入缓冲区的地址。
	@return
		返回缓冲区地址。
	@*/
	const void* cerl_call get_buffer();

	/** 获取进行写入操作的文件。
	@return
		返回一个文件句柄。
	@*/
	FileT& cerl_call get_file();
	
	/** 将文件缓冲区中的数据立即写入文件。
	@return
		@val true
			将缓冲区的数据成功的写入了文件。
		@val false
			缓冲区的数据写入文件失败。
	@*/
	bool cerl_call flush();
	
	/** 清空缓冲区
		把缓冲区内数据失效
	@*/
	void cerl_call clear();
	
	range_type cerl_call put_some(const size_type countMin = 1);

	/** 向文件缓冲区写入一个字符。
	@arg [in] ch
		向文件缓冲区写入的字符。
	@return
		@var 1
			成功写入一个字符。
		@var 0
			写入失败。
	@*/
	size_type cerl_call put(char_type ch);

	/** 向文件缓冲区写入一个字符串。
	@arg [in] buf
		字符串存放地址。
	@arg [in] cb
		要写入的字符串的长度。
	@return
		返回成功写入的字符串长度。
	@*/
	size_type cerl_call put(const char_type* buf, size_type cb);

};