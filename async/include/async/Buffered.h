/* -------------------------------------------------------------------------
// CERL2: C++ Erlang Server Model 2.0
// 
// Module: async/Buffered.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Buffered.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef ASYNC_BUFFERED_H
#define ASYNC_BUFFERED_H

#ifndef ASYNC_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../../../stdext/include/stdext/text/BasicString.h"
#endif

NS_CERL_BEGIN

// =========================================================================
// class BufferedReader

template <class FileT>
class BufferedReader
{
public:
	enum { endch = -1 };

	typedef size_t size_type;
	typedef int int_type;
	typedef char char_type;
	typedef unsigned char uchar_type;
	typedef NS_STDEXT::Range<const char_type*> range_type;

private:
	char_type* m_cur;
	char_type* m_buf;
	char_type* m_end;
	size_type m_size;
	FileT& m_file;

public:
	BufferedReader(FileT& file, char_type* buf, size_type cb)
		: m_cur(buf), m_buf(buf), m_end(buf), m_size(cb), m_file(file)
	{
	}

	const void* cerl_call get_buffer() const
	{
		return m_buf;
	}

	FileT& cerl_call get_file() const
	{
		return m_file;
	}

	void cerl_call clear()
	{
		m_cur = m_end = m_buf;
	}

public:
	range_type cerl_call get_some()
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedReader::get_some - fileobject broken!\n"));

		if (m_cur == m_end)
		{
			m_cur = m_end = m_buf + m_file.read_some(m_buf, m_size);
			return range_type(m_buf, m_end);
		}
		else
		{
			char_type* cur = m_cur;
			m_cur = m_end;
			return range_type(cur, m_end);
		}
	}

	void cerl_call unget_some(size_type count)
	{
		CERL_ASSERT(m_cur == m_end);
		CERL_ASSERT((size_type)(m_end - m_buf) >= count);

		m_cur -= count;
	}

public:
	size_type cerl_call max_bytes() const
	{
		return m_size;
	}

	const char_type* cerl_call peek_bytes(size_type cb)
	{
		CERL_ASSERT(cb <= max_bytes());
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedReader::peek_bytes - fileobject broken!\n"));

		size_type cbReaded = m_end - m_cur;
		if (cbReaded >= cb)
			return m_cur;

		if (cb > m_size)
			return NULL;

		memcpy(m_buf, m_cur, cbReaded);
		for (;;)
		{
			const size_type readed = m_file.read_some(m_buf + cbReaded, m_size - cbReaded);
			if (readed == 0)
				return NULL;
			if ((cbReaded += readed) >= cb)
				break;
		}
		m_cur = m_buf;
		m_end = m_buf + cbReaded;
		return m_buf;
	}

	const char_type* cerl_call get_bytes(size_type cb)
	{
		const char_type* msg = peek_bytes(cb);
		m_cur += cb;
		return msg;
	}

public:
	int_type cerl_call peek()
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedReader::peek - fileobject broken!\n"));

		if (m_cur != m_end)
			return (uchar_type)*m_cur;

		m_cur = m_buf;
		m_end = m_buf + m_file.read_some(m_buf, m_size);
		if (m_cur != m_end)
			return (uchar_type)*m_cur;
		return endch;				
	}

	int_type cerl_call get()
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedReader::get - fileobject broken!\n"));

		if (m_cur != m_end)
			return (uchar_type)*m_cur++;

		m_cur = m_buf;
		m_end = m_buf + m_file.read_some(m_buf, m_size);
		if (m_cur != m_end)
			return (uchar_type)*m_cur++;
		return endch;
	}

public:
	size_type cerl_call skip(size_type cb)
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedReader::skip - fileobject broken!\n"));
		
		if ((size_type)(m_end - m_cur) >= cb)
		{
			m_cur += cb;
			return cb;
		}
		return cb - do_skip_(cb);
	}

	size_type cerl_call get(char_type* buf, size_type cb)
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedReader::get - fileobject broken!\n"));

		if ((size_type)(m_end - m_cur) >= cb)
		{
			memcpy(buf, m_cur, cb);
			m_cur += cb;
			return cb;
		}

		return cb - do_get_(buf, cb);
	}

private:
	size_type cerl_call do_skip_(size_type cb)
	{
		const size_type rest = m_end - m_cur;
		if (rest)
		{
			cb -= rest;
			m_cur = m_end;
		}

		while (cb >= m_size)
		{
			const size_type readed = m_file.read_some(m_buf, m_size);
			if (readed == 0) // read fail
				return cb;
			cb -= readed;
		}

		for (;;)
		{
			const size_type readed = m_file.read_some(m_buf, m_size);
			if (readed >= cb) // read done
			{
				m_cur = m_buf + cb;
				m_end = m_buf + readed;
				return 0;
			}

			if (readed == 0) // read fail
			{
				return cb;
			}

			cb -= readed;
		}
	}

	size_type cerl_call do_get_(char_type* buf, size_type cb)
	{
		const size_type rest = m_end - m_cur;
		if (rest)
		{
			memcpy(buf, m_cur, rest);
			buf += rest;
			cb -= rest;
			m_cur = m_end;
		}

		while (cb >= m_size)
		{
			const size_type readed = m_file.read_some(buf, cb);
			if (readed == cb) // read done
				return 0;
			if (readed == 0) // read fail
				return cb;
			buf += readed;
			cb -= readed;
		}

		for (;;)
		{
			const size_type readed = m_file.read_some(m_buf, m_size);
			if (readed >= cb) // read done
			{
				memcpy(buf, m_buf, cb);
				m_cur = m_buf + cb;
				m_end = m_buf + readed;
				return 0;
			}

			if (readed == 0) // read fail
				return cb;

			memcpy(buf, m_buf, readed);
			buf += readed;
 			cb -= readed;
		}
	}
};

// -------------------------------------------------------------------------
// skip_eol

template <class ReadArchiveT>
inline size_t winx_call skip_eol(ReadArchiveT& ar) // '\r', '\n', '\r\n'
{
	typedef typename ReadArchiveT::int_type int_type;

	const int_type ch = ar.peek();
	if (ch == '\r')
	{
		ar.get();
		if (ar.peek() == '\n') {
			ar.get();
			return 2;
		}
		return 1;
	}
	else if (ch == '\n')
	{
		ar.get();
		return 1;
	}
	return 0;
}

// -------------------------------------------------------------------------
// append_line/fast_getline

template <class ArchiveT, class StringT>
inline bool /* need_skip_eol */ cerl_call append_line(ArchiveT& ar, StringT& line)
{
	typedef NS_STDEXT::CharType::IsEOL is_eol;
	typedef typename ArchiveT::range_type range_type;
	typedef typename range_type::const_iterator iterator;
	for (;;)
	{
		range_type const rg = ar.get_some();
		iterator const it = std::find_if(rg.begin(), rg.end(), is_eol());
		line.insert(line.end(), rg.begin(), it);
		if (it != rg.end())
		{
			ar.unget_some(rg.end() - it);
			return true;
		}
		if (rg.empty()) // is eof
		{
			return false;
		}
	}
}

template <class ArchiveT, class StringT, class BufferT>
inline bool /* need_skip_eol */ cerl_call fast_getline(ArchiveT& ar, StringT& line, BufferT& buffer)
{
	typedef NS_STDEXT::CharType::IsEOL is_eol;
	typedef typename ArchiveT::range_type range_type;
	typedef typename range_type::const_iterator iterator;

	range_type const rg = ar.get_some();
	iterator it = std::find_if(rg.begin(), rg.end(), is_eol());
	if (it != rg.end())
	{
		line = StringT(rg.begin(), it);
		ar.unget_some(rg.end() - it);
		return true;
	}
	else if (rg.empty()) // is eof
	{
		line = StringT();
		return false;
	}

	buffer.assign(rg.begin(), rg.end());
	bool const need_skip_eol = append_line(ar, buffer);
	line = StringT( std::iterToPointer(buffer.begin()), std::iterToPointer(buffer.end()) );
	return need_skip_eol;
}

// -------------------------------------------------------------------------
// getline

template <class ArchiveT, class StringT>
inline size_t cerl_call getline(ArchiveT& ar, StringT& line)
{
	line.clear();
	if (append_line(ar, line))
		return line.size() + skip_eol(ar);
	else
		return line.size();
}

// =========================================================================
// class BufferedWriter

template <class FileT>
class BufferedWriter
{
public:
	enum { endch = -1 };

	typedef size_t size_type;
	typedef int int_type;
	typedef char char_type;
	typedef unsigned char uchar_type;
	typedef NS_STDEXT::Range<char_type*> range_type;

private:
	char_type* m_cur;
	char_type* m_buf;
	char_type* m_end;
	FileT& m_file;

public:
	BufferedWriter(FileT& file, char_type* buf, size_type cb)
		: m_cur(buf), m_buf(buf), m_end(buf + cb), m_file(file)
	{
	}

	const void* cerl_call get_buffer() const
	{
		return m_buf;
	}

	FileT& cerl_call get_file() const
	{
		return m_file;
	}
	
	bool cerl_call flush()
	{
		const size_type rest = m_cur - m_buf;
		if (rest)
		{
			m_cur = m_buf;
			return m_file.write(m_buf, rest) == rest;
		}
		return true;
	}

	void cerl_call clear()
	{
		m_cur = m_buf;
	}

public:
	range_type cerl_call put_some(const size_type countMin = 1)
	{
		CERL_TRACE_IF(!m_file.good(), ("ERROR: BufferedWriter::put_some - maybe file closed?\n"));

		if (m_cur + countMin > m_end)
		{
			const size_type written = m_file.write(m_buf, m_cur - m_buf);
			if (written != m_cur - m_buf) // failed
			{
				m_cur += written;
				return range_type();
			}
			m_cur = m_end;
			return range_type(m_buf, m_end);
		}
		else
		{
			char_type* cur = m_cur;
			m_cur = m_end;
			return range_type(cur, m_end);
		}
	}

	void cerl_call unput_some(size_type count)
	{
		CERL_ASSERT(m_cur == m_end);
		CERL_ASSERT((size_type)(m_end - m_buf) >= count);

		m_cur -= count;
	}

public:
	size_type cerl_call put(char_type ch)
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedWriter::put(char) - fileobject broken!\n"));

		if (m_cur == m_end)
		{
			const size_type written = m_file.write(m_buf, m_end - m_buf);
			if ((int)written != m_end - m_buf) // failed
			{
				m_cur = m_buf;
				return 0;
			}
			m_cur = m_buf;
		}
		*m_cur++ = ch;
		return 1;
	}

	size_type cerl_call put(const char_type* buf, size_type cb)
	{
		CERL_TRACE_IF(!m_file.good(), ("WARN: BufferedWriter::put(buf) - fileobject broken!\n"));

		if ((size_type)(m_end - m_cur) >= cb)
		{
			memcpy(m_cur, buf, cb);
			m_cur += cb;
			return cb;
		}
		return cb - do_put_(buf, cb);
	}

	size_type cerl_call put(const char_type* buf)
	{
		return put(buf, strlen(buf));
	}

private:
	size_type cerl_call do_put_(const char_type* buf, size_type cb)
	{
		const size_type rest = m_end - m_cur;
		memcpy(m_cur, buf, rest);
		buf += rest;
		cb -= rest;

		const size_type bufsize = m_end - m_buf;
		const size_type written = m_file.write(m_buf, bufsize);
		m_cur = m_buf;
		if (written != bufsize) // failed
		{
			return cb;
		}
		
		while (cb >= bufsize)
		{
			const size_type written = m_file.write_some(buf, cb);
			if (written == cb) // write done
				return 0;
			if (written == 0) // write fail
				return cb;
			buf += written;
			cb -= written;
		}
		
		memcpy(m_buf, buf, cb);
		m_cur = m_buf + cb;
		return 0;
	}
};

// =========================================================================

NS_CERL_END

#endif /* ASYNC_BUFFERED_H */
