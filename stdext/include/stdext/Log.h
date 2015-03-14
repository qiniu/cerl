/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/Log.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 17:57:28
// 
// $Id: Log.h,v 1.7 2007/01/10 09:32:25 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_LOG_H
#define STDEXT_LOG_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_STORAGE_H
#include "Storage.h"
#endif

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDARG
#include <stdarg.h>
#endif

#ifndef STD_STRING_H
#include "../std/string.h"
#endif

NS_STDEXT_BEGIN

// =========================================================================
// __STD_LOG_PRINT

#define __STD_LOG_FMT_PRINT_EX(DataT, vData)								\
	Log& winx_call print(DataT v, const WCHAR* fmt) {						\
		return trace(fmt, vData);											\
	}																		\
	Log& winx_call print(DataT v, const char* fmt) {						\
		return trace(fmt, vData);											\
	}

#define __STD_LOG_FMT_PRINT(DataT)											\
	__STD_LOG_FMT_PRINT_EX(DataT, v)

#define __STD_LOG_PRINT_CHAR(DataT)											\
	Log& winx_call print(DataT v) {											\
		return repeat(v);													\
	}																		\
	__STD_LOG_FMT_PRINT(DataT)

#define __STD_LOG_PRINT_INT(SIntT, UIntT)									\
	Log& winx_call print(SIntT v) {         								\
		char buf[32];														\
		return printString(_ltoa(v, buf, 10));								\
	}																		\
	Log& winx_call print(UIntT v) {								            \
		char buf[32];														\
		return printString(_ultoa(v, buf, 10));								\
	}																		\
	__STD_LOG_FMT_PRINT(SIntT)										        \
	__STD_LOG_FMT_PRINT(UIntT)

#define __STD_LOG_PRINT_STRING(DataT)										\
	Log& winx_call print(DataT v) {											\
		return printString(v);												\
	}																		\
	__STD_LOG_FMT_PRINT(DataT)

#define __STD_LOG_PRINT_STRING_CLS()										\
	template <class _E, class _Tr, class _Alloc>							\
	Log& winx_call print(const basic_string<_E, _Tr, _Alloc>& v) {			\
		return printString(v);												\
	}																		\
	template <class _E, class _Tr, class _Alloc, class _E2>					\
	Log& winx_call print(													\
			const basic_string<_E, _Tr, _Alloc>&v, const _E2* fmt) {		\
		return trace(fmt, v.c_str());										\
	}

#define __STD_LOG_PRINT_INT64()	        									\
	Log& winx_call print(__int64 v) {									    \
		char buf[32];														\
		return printString(_i64toa(v, buf, 10));							\
	}																		\
	Log& winx_call print(__uint64 v) {		    					        \
		char buf[32];														\
		return printString(_ui64toa(v, buf, 10));							\
	}																		\
	__STD_LOG_FMT_PRINT(__int64)    										\
	__STD_LOG_FMT_PRINT(__uint64)

#define __STD_LOG_PRINT_FLOAT(DataT, n)										\
	Log& winx_call print(DataT v, int digits = n) {							\
		char buf[64];														\
		return printString(_gcvt(v, digits, buf));							\
	}																		\
	__STD_LOG_FMT_PRINT(DataT)

#define __STD_LOG_PRINT_CTYPES()											\
	__STD_LOG_PRINT_CHAR(char)												\
	__STD_LOG_PRINT_CHAR(signed char)										\
	__STD_LOG_PRINT_CHAR(unsigned char)										\
	__STD_LOG_PRINT_STRING(const char*)										\
	__STD_LOG_PRINT_STRING(const WCHAR*)									\
	__STD_LOG_PRINT_STRING_CLS()											\
	__STD_LOG_PRINT_INT(signed int, unsigned int)							\
	__STD_LOG_PRINT_INT(signed short, unsigned short)						\
	__STD_LOG_PRINT_INT(signed long, unsigned long)						    \
	__STD_LOG_PRINT_INT64()		        									\
	__STD_LOG_PRINT_FLOAT(float, 12)										\
	__STD_LOG_PRINT_FLOAT(double, 12)

// =========================================================================
// class Log

#if defined(_MSC_VER)
#pragma warning(disable:4996) // XXX  was declared deprecated
#endif

template <class Storage>
class Log
{
protected:
	Storage m_stg;
	
public:
	Log() {}

	template <class ArgT>
	Log(ArgT arg) : m_stg(arg) {}

	template <class ArgT1, class ArgT2>
	Log(ArgT1 arg1, ArgT2 arg2) : m_stg(arg1, arg2) {}

public:
	Storage& winx_call storage() {
		return m_stg;
	}
	const Storage& winx_call storage() const {
		return m_stg;
	}

public:
	int winx_call good() const
	{
		return m_stg.good();
	}

	template <class ArgT>
	void winx_call open(ArgT stg)
	{
		WINX_ASSERT(!m_stg.good());
		m_stg.open(stg);
	}

	void winx_call close()
	{
		m_stg.close();
	}

	void winx_call flush()
	{
		if (m_stg)
			m_stg.flush();
	}

	Log& winx_call step(char ch = '.')
	{
		if (m_stg)
			m_stg.put(ch);
		return *this;
	}

	Log& winx_call put(size_t count, char ch)
	{
		if (m_stg)
			m_stg.put(count, ch);
		return *this;
	}

	Log& winx_call put(char ch)
	{
		if (m_stg)
			m_stg.put(ch);
		return *this;
	}

	Log& winx_call write(const char* s, size_t count)
	{
		if (m_stg)
			m_stg.put(s, count);
		return *this;
	}

	Log& winx_call repeat(char ch)
	{
		if (m_stg)
			m_stg.put(ch);
		return *this;
	}

	Log& winx_call repeat(BYTE ch)
	{
		if (m_stg)
			m_stg.put(ch);
		return *this;
	}

	Log& winx_call repeat(char ch, size_t count)
	{
		if (m_stg)
			m_stg.put(count, ch);
		return *this;
	}
	
	Log& winx_call repeat(WCHAR ch)
	{
		if (m_stg)
			m_stg.putw(ch);
		return *this;
	}

	Log& winx_call repeat(WCHAR ch, size_t count)
	{
		if (m_stg)
			m_stg.putw(count, ch);
		return *this;
	}

	Log& winx_call newline()
	{
		if (m_stg)
			m_stg.put('\n');
		return *this;
	}

	Log& winx_call newline(size_t count)
	{
		if (m_stg)
			m_stg.put(count, '\n');
		return *this;
	}

	template <class CharT>
	Log& winx_call vtrace(const CharT* fmt, va_list args)
	{
		if (m_stg)
			m_stg.putv(fmt, args);
		return *this;
	}

	template <class CharT>
	Log& winx_call trace(const CharT* fmt, ...)
	{
		if (m_stg)
		{
			va_list args;
			va_start(args, fmt);
			m_stg.putv(fmt, args);
			va_end(args);
		}
		return *this;
	}
	
private:
	template <class _It>
	Log& winx_call __printString(_It first, _It last, char)
	{
		if (m_stg)
			m_stg.put(iterToPointer(first), last - first);
		return *this;
	}
	
	template <class _It>
	Log& winx_call __printString(_It first, _It last, unsigned char)
	{
		if (m_stg)
			m_stg.put((char*)iterToPointer(first), last-first);
		return *this;
	}

	template <class _It>
	Log& winx_call __printString(_It first, _It last, WCHAR)
	{
		std::basic_string<WCHAR> s(first, last);
		return trace("%S", s.c_str());
	}

public:
	template <class _It>
	Log& winx_call printString(_It first, _It last)
	{
		return __printString(first, last, *first);
	}

	template <class _Tr, class _Alloc>
	Log& winx_call printString(const basic_string<char, _Tr, _Alloc>& s)
	{
		if (m_stg)
			m_stg.put(s.c_str(), s.size());
		return *this;
	}

	Log& winx_call printString(const char* s)
	{
		if (m_stg)
			m_stg.put(s, strlen(s));
		return *this;
	}
	
	template <class _Tr, class _Alloc>
	Log& winx_call printString(const basic_string<WCHAR, _Tr, _Alloc>& s)
	{
		return trace("%S", s.c_str());
	}

	Log& winx_call printString(const WCHAR* s)
	{
		return trace("%S", s);
	}

public:
	Log& winx_call print(const RECT& rc)
	{
		return trace("(%d, %d) - (%d, %d)", rc.left, rc.top, rc.right, rc.bottom);
	}

	Log& winx_call print(const POINT& pt)
	{
		return trace("(%d, %d)", pt.x, pt.y);
	}

	Log& winx_call print(const SIZE& sz)
	{
		return trace("(%d, %d)", sz.cx, sz.cy);
	}

public:
	template <class DataT>
	Log& winx_call printObj(const DataT& v)
	{
		v.trace(*this);
		return *this;
	}
	
	template <class DataT, class ArgT1>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1)
	{
		v.trace(*this, arg1);
		return *this;
	}

	template <class DataT, class ArgT1, class ArgT2>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1, ArgT2 arg2)
	{
		v.trace(*this, arg1, arg2);
		return *this;
	}

	template <class DataT, class ArgT1, class ArgT2, class ArgT3>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1, ArgT2 arg2, ArgT3 arg3)
	{
		v.trace(*this, arg1, arg2, arg3);
		return *this;
	}

public:
	template <class DataIt>
	Log& winx_call printArray(
		DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }",
		const char* dataSep = ", ")
	{
		if (m_stg)
		{
			m_stg.put(bracketL, strlen(bracketL));
			if (count)
			{
				for (size_t i = 0;;)
				{
					print(*array++);
					if (++i >= count)
						break;
					m_stg.put(dataSep, strlen(dataSep));
				}
			}
			m_stg.put(bracketR, strlen(bracketR));
		}
		return *this;
	}

	template <class ArgT, class DataIt>
	Log& winx_call printArray(
		ArgT fmt, DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }",
		const char* dataSep = ", ")
	{
		if (m_stg)
		{
			m_stg.put(bracketL, strlen(bracketL));
			if (count)
			{
				for (size_t i = 0;;)
				{
					print(*array++, fmt);
					if (++i >= count)
						break;
					m_stg.put(dataSep, strlen(dataSep));
				}
			}
			m_stg.put(bracketR, strlen(bracketR));
		}
		return *this;
	}

public:
	template <class DataIt>
	Log& winx_call printObjArray(
		DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }",
		const char* dataSep = ", ")
	{
		if (m_stg)
		{
			m_stg.put(bracketL, strlen(bracketL));
			if (count)
			{
				for (size_t i = 0;;)
				{
					(*array++).trace(*this);
					if (++i >= count)
						break;
					m_stg.put(dataSep, strlen(dataSep));
				}
			}
			m_stg.put(bracketR, strlen(bracketR));
		}
		return *this;
	}

	template <class ArgT, class DataIt>
	Log& winx_call printObjArray(
		ArgT fmt, DataIt array, size_t count,
		const char* bracketL = "{ ", const char* bracketR = " }",
		const char* dataSep = ", ")
	{
		if (m_stg)
		{
			m_stg.put(bracketL, strlen(bracketL));
			if (count)
			{
				for (size_t i = 0;;)
				{
					(*array++).trace(*this, fmt);
					if (++i >= count)
						break;
					m_stg.put(dataSep, strlen(dataSep));
				}
			}
			m_stg.put(bracketR, strlen(bracketR));
		}
		return *this;
	}

public:
	template <class CharT, class VectorT1, class VectorT2>
	Log& winx_call traceDim(const CharT* fmt, const VectorT1& x, const VectorT2& y)
	{
		size_t count = x.size();
		if (y.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				trace(fmt, x[i], y[i]);
		}
		return *this;
	}
	
	template <class CharT, class VectorT1, class VectorT2, class VectorT3>
	Log& winx_call traceDim(const CharT* fmt, const VectorT1& x, const VectorT2& y, const VectorT3& z)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				trace(fmt, x[i], y[i], z[i]);
		}
		return *this;
	}

	template <class CharT, class VectorT1, class VectorT2, class VectorT3, class VectorT4>
	Log& winx_call traceDim(const CharT* fmt, const VectorT1& x, const VectorT2& y, const VectorT3& z, const VectorT4& u)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count || u.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				trace(fmt, x[i], y[i], z[i], u[i]);
		}
		return *this;
	}

public:
	template <class VectorT1, class VectorT2>
	Log& winx_call printDim(const VectorT1& x, const VectorT2& y)
	{
		size_t count = x.size();
		if (y.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				print("\t[ ").
				print(x[i]).print(", ").
				print(y[i]).print(" ]\n");
		}
		return *this;
	}

	template <class VectorT1, class VectorT2, class VectorT3>
	Log& winx_call printDim(const VectorT1& x, const VectorT2& y, const VectorT3& z)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				print("\t[ ").
				print(x[i]).print(", ").
				print(y[i]).print(", ").
				print(z[i]).print(" ]\n");
		}
		return *this;
	}

	template <class VectorT1, class VectorT2, class VectorT3, class VectorT4>
	Log& winx_call printDim(const VectorT1& x, const VectorT2& y, const VectorT3& z, const VectorT4& u)
	{
		size_t count = x.size();
		if (y.size() < count || z.size() < count || u.size() < count)
		{
			print("\t[ Exception: Size of vector doesn't match! ]\n");
		}
		else
		{
			for (size_t i = 0; i < count; ++i)
				print("\t[ ").
				print(x[i]).print(", ").
				print(y[i]).print(", ").
				print(z[i]).print(", ").
				print(u[i]).print(" ]\n");
		}
		return *this;
	}

public:
	void winx_call reportTestCaseError(
		const char* testClass,
		const char* testCase,
		const char* msg,
		const char* file, int line)
	{
		trace(
			"\n%s(%d):\n%s::%s()\n  ---> %s\n", 
			file, line, testClass, testCase, msg
			);
	}

	void winx_call reportError(
		const char* msg, const char* file, int line)
	{
		trace("\n%s(%d):\n  ---> %s\n", file, line, msg);
	}

	__STD_LOG_PRINT_CTYPES()
};

#if defined(_MSC_VER)
#pragma warning(default:4996) // XXX  was declared deprecated
#endif

// =========================================================================
// class OutputLog, ErrorLog, FileLog

class OutputLog : public Log<FILEStorage>
{
public:
	OutputLog() : Log<FILEStorage>(stdout) {}
};

class ErrorLog : public Log<FILEStorage>
{
public:
	ErrorLog() : Log<FILEStorage>(stderr) {}
};

class FileLog : public Log<FILEStorage>
{
private:
	FileLog(const FileLog&);
	void operator=(const FileLog&);

public:
	FileLog() {}

	template <class ArgT>
	FileLog(ArgT szFile) : Log<FILEStorage>(szFile, false) {}

	~FileLog() {
		m_stg.close();
	}

	template <class ArgT>
	void winx_call open(ArgT szFile) {
		m_stg.open(szFile, false);
	}
};

// =========================================================================
// class StringLog

class StringLog : public Log< StringStorage<std::string> >
{
public:
	template <class LogT>
	void winx_call trace(LogT& log) const
	{
		const std::string& str = storage();
		log.printString(str);
	}
};

// =========================================================================
// class MultiLog

template <UINT nStorage = 0, class StorageContainer = Array<ILogStorage*, nStorage> >
class MultiLog : public Log< MultiStorage<nStorage, StorageContainer> >
{
};

// =========================================================================
// class NullLog

class NullLog
{
public:
	typedef NullLog Log;

	void winx_call flush()							{}
	Log& winx_call step()							{ return *this; }
	Log& winx_call repeat(char ch)					{ return *this; }
	Log& winx_call repeat(char ch, size_t count)	{ return *this; }
	Log& winx_call repeat(WCHAR ch)					{ return *this; }
	Log& winx_call repeat(WCHAR ch, size_t count)	{ return *this; }
	Log& winx_call newline()						{ return *this; }

	Log& winx_call trace(const char* fmt, ...)		{ return *this; }
	Log& winx_call trace(const WCHAR* fmt, ...)		{ return *this; }

	Log& winx_call put(char ch)						{ return *this; }
	Log& winx_call put(size_t count, char ch)		{ return *this; }
	Log& winx_call write(const char*, size_t)		{ return *this; }

public:
	template <class DataT>
	Log& winx_call print(const DataT& v)
	{
		return *this;
	}

	template <class DataT, class ArgT1>
	Log& winx_call print(const DataT& v, ArgT1 arg1)
	{
		return *this;
	}
	
	template <class DataT, class ArgT1, class ArgT2>
	Log& winx_call print(const DataT& v, ArgT1 arg1, ArgT2 arg2)
	{
		return *this;
	}
	
	template <class DataT, class ArgT1, class ArgT2, class ArgT3>
	Log& winx_call print(const DataT& v, ArgT1 arg1, ArgT2 arg2, ArgT3 arg3)
	{
		return *this;
	}

public:
	template <class DataT>
	Log& winx_call printObj(const DataT& v)
	{
		return *this;
	}

	template <class DataT, class ArgT1>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1)
	{
		return *this;
	}
	
	template <class DataT, class ArgT1, class ArgT2>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1, ArgT2 arg2)
	{
		return *this;
	}
	
	template <class DataT, class ArgT1, class ArgT2, class ArgT3>
	Log& winx_call printObj(const DataT& v, ArgT1 arg1, ArgT2 arg2, ArgT3 arg3)
	{
		return *this;
	}

public:
	template <class _It>
	Log& winx_call printString(_It first, _It last)	{ return *this; }
	
	template <class _String>
	Log& winx_call printString(const _String& s)
	{
		return *this;
	}
	
public:
	template <class ArgT, class DataIt>
	Log& winx_call printArray(
		DataIt array, size_t count,
		const char* bracketL = NULL, const char* bracketR = NULL,
		const char* dataSep = NULL)
	{
		return *this;
	}

	template <class ArgT, class DataIt>
	Log& winx_call printArray(
		ArgT fmt, DataIt array, size_t count,
		const char* bracketL = NULL, const char* bracketR = NULL,
		const char* dataSep = NULL)
	{
		return *this;
	}
	
public:
	template <class ArgT, class DataIt>
	Log& winx_call printObjArray(
		DataIt array, size_t count,
		const char* bracketL = NULL, const char* bracketR = NULL,
		const char* dataSep = NULL)
	{
		return *this;
	}

	template <class ArgT, class DataIt>
	Log& winx_call printObjArray(
		ArgT fmt, DataIt array, size_t count,
		const char* bracketL = NULL, const char* bracketR = NULL,
		const char* dataSep = NULL)
	{
		return *this;
	}

public:
	void winx_call reportTestCaseError(
		const char* testClass,
		const char* testCase,
		const char* msg,
		const char* file, int line)
	{
	}
	
	void winx_call reportGuardError(
		const char* general,
		const int error,
		const char* detail,
		const char* file, int line)
	{
	}

	void winx_call reportError(
		const char* msg, const char* file, int line)
	{
	}
};

// =========================================================================
// class TestLog

#if defined(STD_UNITTEST)

template <class LogT>
class TestLog : public TestCase
{
	WINX_TEST_SUITE(TestLog);
		WINX_TEST(testLog);
		WINX_TEST(testStringLog);
		WINX_TEST(testMultiLog);
	WINX_TEST_SUITE_END();

public:
	struct Obj
	{
		int m_data;
		Obj(int data) : m_data(data) {}

		template <class LogT2>
		void trace(LogT2& log) const {
			log.trace("%d", m_data);
		}
		
		template <class LogT2>
		void trace(LogT2& log, const char* fmt) const {
			log.trace(fmt, m_data);
		}
	};

	void testLog(LogT& log)
	{
		Obj obj(0x4224);

		log.print("\n---> print int\n");
		log.print(12).newline();
		log.print((short)123).newline();
		
		log.print("\n---> print int64\n");
		log.print((INT64)-1).newline();
		log.print((UINT64)-1).newline();
		log.print((INT64)-1, "0x%I64X").newline();

		log.print("\n---> print float\n");
		log.print(12.2).newline();
		log.print(1.0/3).newline();
		log.print(1.0/3, 1).newline();
		log.print(1.0/3, "%.8f").newline();

		log.print("\n---> print char\n");
		log.print('c').newline();
		log.print('c', "%d").newline();
		
		log.print("\n---> print string\n");
		log.print("abc").newline();
		log.print(L"abc").newline();
		log.print("abc", "%8s").newline();
		
		log.print("\n---> print string class\n");
		std::string s = "class";
		log.print(s).newline();
		log.print(s, "%8s").newline();

		log.print("\n---> print object\n");
		log.printObj(obj, "0x%.4x").newline();
		log.printObj(obj).newline();
	}

	void testStringLog(LogT& log)
	{
		std::StringLog slog;
		slog.print("\n--> print to string log\n");
		slog.print(1.0/3).newline();
		slog.print(1.0/3, 1).newline();
		slog.newline();
		log.print(slog.storage());
	}

	void testMultiLog(LogT& log)
	{
		std::FILEStorageT<true> file("/__temp__.log");
		std::FILEStorageT<false> fout(stdout);
		std::MultiLog<2> mlog;
		mlog.storage().add(file);
		mlog.storage().add(fout);
		mlog.trace("Hello, MultiLog!!!\n");

		int sel = mlog.storage().select(1);
		mlog.trace("Message to file!!!\n");
		mlog.storage().select(sel);

		mlog.trace("Hello, MultiLog2!!!\n");

		int sel2 = mlog.storage().select(0);
		mlog.trace("Disabled message!!!\n");
		mlog.storage().select(sel2);
	}
};

#endif // defined(STD_UNITTEST)

// =========================================================================
// $Log: Log.h,v $

NS_STDEXT_END

#endif /* STDEXT_LOG_H */
