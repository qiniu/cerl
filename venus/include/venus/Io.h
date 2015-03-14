/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Io.h
// Creator: duanxuetao
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Io.h 2732 2010-04-29 09:01:30Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_IO_H
#define VENUS_IO_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

#ifndef VENUS_DEBUG_H
#include "Debug.h"
#endif

#ifndef VENUS_SERIALIZATION_H
#include "Serialization.h"
#endif

#ifndef VENUS_DUMP_H
#include "Dump.h"
#endif

#ifndef VENUS_COPY_H
#include "Copy.h"
#endif

#ifndef ASYNC_BUFFERED_H
#include "../../../async/include/async/Buffered.h"
#endif

NS_CERL_BEGIN

// -------------------------------------------------------------------------
// class MailReader

template <class FileT>
class MailReader : public BufferedReader<FileT>
{
private:
	typedef BufferedReader<FileT> Base;
	
	ScopedAlloc* m_alloc;
	
public:
	MailReader(FileT& file, typename Base::char_type* buf, typename Base::size_type cb)
		: Base(file, buf, cb), m_alloc(NULL)
	{
	}

	void setAlloc(ScopedAlloc* alloc)
	{
		m_alloc = alloc;
	}

	ScopedAlloc& getAlloc()
	{
		CERL_ASSERT(m_alloc);
		return *m_alloc;
	}
};

// -------------------------------------------------------------------------

NS_CERL_END

#ifndef VENUS_IO_INL
#include "Io.inl"
#endif

#endif
