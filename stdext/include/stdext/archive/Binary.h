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
// Module: stdext/archive/Binary.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: Binary.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_BINARY_H
#define STDEXT_ARCHIVE_BINARY_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_TEXT_BASICSTRING_H
#include "../text/BasicString.h"
#endif

#ifndef NS_STDEXT_IO_BINARY
#define NS_STDEXT_IO_BINARY			NS_STDEXT::io
#define NS_STDEXT_IO_BINARY_BEGIN	NS_STDEXT_BEGIN namespace io {
#define NS_STDEXT_IO_BINARY_END		} NS_STDEXT_END
#endif

NS_STDEXT_IO_BINARY_BEGIN

// -------------------------------------------------------------------------
// ==== BinaryWriter ====

template <class WriteArchiveT>
inline void winx_call put16i(
	IN WriteArchiveT& ar, IN UINT16 val) throw(IoException)
{
	_WinxByteSwap16(val);
	ar.put( (const char*)&val, sizeof(val) );
}

template <class WriteArchiveT>
inline void winx_call put32i(
	IN WriteArchiveT& ar, IN UINT32 val) throw(IoException)
{
	_WinxByteSwap32(val);
	ar.put( (const char*)&val, sizeof(val) );
}

template <class WriteArchiveT>
inline void winx_call put64i(
	IN WriteArchiveT& ar, IN INT64 val) throw(IoException)
{
	_WinxByteSwap64(val);
	ar.put( (const char*)&val, sizeof(val) );
}

template <class WriteArchiveT>
inline void winx_call put64i(
	IN WriteArchiveT& ar, IN UINT64 val) throw(IoException)
{
	_WinxByteSwap64(val);
	ar.put( (const char*)&val, sizeof(val) );
}

template <class WriteArchiveT, class StrucType>
inline void winx_call put_struct(
	IN WriteArchiveT& ar, IN const StrucType& struc) throw(IoException)
{
	_WinxByteSwapStruct(struc);
	ar.put( (const char*)&struc, sizeof(struc) );
}

#if defined(WINX_BYTESWAP)

template <class WriteArchiveT>
inline void winx_call put16i(
	IN WriteArchiveT& ar, IN const UINT16 warray[], IN size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put16i(ar, warray[i]);
}

template <class WriteArchiveT>
inline void winx_call put16i(
	IN WriteArchiveT& ar, IN const INT16 warray[], IN size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put16i(ar, warray[i]);
}

template <class WriteArchiveT>
inline void winx_call put32i(
	IN WriteArchiveT& ar, IN const UINT32 dwarray[], IN size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put32i(ar, dwarray[i]);
}

template <class WriteArchiveT>
inline void winx_call put32i(
	IN WriteArchiveT& ar, IN const INT32 dwarray[], IN size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put32i(ar, dwarray[i]);
}

template <class WriteArchiveT>
inline void winx_call put64i(
	IN WriteArchiveT& ar, IN const UINT64 dwarray[], IN size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put64i(ar, dwarray[i]);
}

template <class WriteArchiveT>
inline void winx_call put64i(
	IN WriteArchiveT& ar, IN const INT64 dwarray[], IN size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put64i(ar, dwarray[i]);
}

template <class WriteArchiveT, class StrucType>
inline void winx_call put_struct(
	WriteArchiveT& ar, const StrucType* array, size_t count) throw(IoException)
{
	for (size_t i = 0; i < count; ++i)
		put_struct(ar, array[i]);
}

#else

template <class WriteArchiveT>
inline void winx_call put16i(
	WriteArchiveT& ar, IN const UINT16 warray[], IN size_t count) throw(IoException)
{
	ar.put( (const char*)warray, sizeof(UINT16)*count );
}

template <class WriteArchiveT>
inline void winx_call put16i(
	IN WriteArchiveT& ar, IN const INT16 warray[], IN size_t count) throw(IoException)
{
	ar.put( (const char*)warray, sizeof(INT16)*count );
}

template <class WriteArchiveT>
inline void winx_call put32i(
	IN WriteArchiveT& ar, IN const UINT32 dwarray[], IN size_t count) throw(IoException)
{
	ar.put( (const char*)dwarray, sizeof(UINT32)*count );
}

template <class WriteArchiveT>
inline void winx_call put32i(
	IN WriteArchiveT& ar, IN const INT32 dwarray[], IN size_t count) throw(IoException)
{
	ar.put( (const char*)dwarray, sizeof(INT32)*count );
}

template <class WriteArchiveT>
inline void winx_call put64i(
	IN WriteArchiveT& ar, IN const UINT64 dwarray[], IN size_t count) throw(IoException)
{
	ar.put( (const char*)dwarray, sizeof(UINT64)*count );
}

template <class WriteArchiveT>
inline void winx_call put64i(
	IN WriteArchiveT& ar, IN const INT64 dwarray[], IN size_t count) throw(IoException)
{
	ar.put( (const char*)dwarray, sizeof(INT64)*count );
}

template <class WriteArchiveT, class StrucType>
inline void winx_call put_struct(
	WriteArchiveT& ar, const StrucType* array, size_t count) throw(IoException)
{
	ar.put( (const char*)array, sizeof(StrucType)*count );
}

#endif // !defined(WINX_BYTESWAP)

// -------------------------------------------------------------------------
// ==== BinaryReader ====

#if defined(WINX_BYTESWAP)

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, UINT16& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwap16(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, INT16& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwap16(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, UINT32& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwap32(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, INT32& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwap32(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, UINT64& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwap64(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, INT64& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwap64(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT, class StrucType>
inline bool winx_call get_struct(ReadArchiveT& ar, StrucType& val)
{
	if (ar.get((char*)&val, sizeof(val)) == sizeof(val))
	{
		_WinxByteSwapStruct(val);
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, UINT16 warray[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get16i(ar, warray[i]))
			return false;
	}
	return true;
}

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, INT16 warray[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get16i(ar, warray[i]))
			return false;
	}
	return true;
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, UINT32 dwarray[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get32i(ar, dwarray[i]))
			return false;
	}
	return true;
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, INT32 dwarray[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get32i(ar, dwarray[i]))
			return false;
	}
	return true;
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, UINT64 dwarray[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get64i(ar, dwarray[i]))
			return false;
	}
	return true;
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, INT64 dwarray[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get64i(ar, dwarray[i]))
			return false;
	}
	return true;
}

template <class ReadArchiveT, class StrucType>
inline bool winx_call get_struct(ReadArchiveT& ar, StrucType array[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (!get_struct(ar, array[i]))
			return false;
	}
	return true;
}

#else

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, UINT16& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, INT16& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, UINT32& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, INT32& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, UINT64& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, INT64& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT, class StrucType>
inline bool winx_call get_struct(ReadArchiveT& ar, StrucType& val)
{
	return ar.get((char*)&val, sizeof(val)) == sizeof(val);
}

template <class ReadArchiveT>
inline bool winx_call peek16i(ReadArchiveT& ar, UINT16& val)
{
	const char* p = ar.peek(sizeof(val));
	if (p) {
		val = *(const UINT16*)p;
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call peek16i(ReadArchiveT& ar, INT16& val)
{
	const char* p = ar.peek(sizeof(val));
	if (p) {
		val = *(const INT16*)p;
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call peek32i(ReadArchiveT& ar, UINT32& val)
{
	const char* p = ar.peek(sizeof(val));
	if (p) {
		val = *(const UINT32*)p;
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call peek32i(ReadArchiveT& ar, INT32& val)
{
	const char* p = ar.peek(sizeof(val));
	if (p) {
		val = *(const INT32*)p;
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call peek64i(ReadArchiveT& ar, UINT64& val)
{
	const char* p = ar.peek(sizeof(val));
	if (p) {
		val = *(const UINT64*)p;
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call peek64i(ReadArchiveT& ar, INT64& val)
{
	const char* p = ar.peek(sizeof(val));
	if (p) {
		val = *(const INT64*)p;
		return true;
	}
	return false;
}

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, UINT16 warray[], size_t count)
{
	const size_t cbToRead = sizeof(UINT16) * count;
	return ar.get((char*)warray, cbToRead) == cbToRead;
}

template <class ReadArchiveT>
inline bool winx_call get16i(ReadArchiveT& ar, INT16 warray[], size_t count)
{
	const size_t cbToRead = sizeof(INT16) * count;
	return ar.get((char*)warray, cbToRead) == cbToRead;
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, UINT32 dwarray[], size_t count)
{
	const size_t cbToRead = sizeof(UINT32) * count;
	return ar.get((char*)dwarray, cbToRead) == cbToRead;
}

template <class ReadArchiveT>
inline bool winx_call get32i(ReadArchiveT& ar, INT32 dwarray[], size_t count)
{
	const size_t cbToRead = sizeof(INT32) * count;
	return ar.get((char*)dwarray, cbToRead) == cbToRead;
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, UINT64 dwarray[], size_t count)
{
	const size_t cbToRead = sizeof(UINT64) * count;
	return ar.get((char*)dwarray, cbToRead) == cbToRead;
}

template <class ReadArchiveT>
inline bool winx_call get64i(ReadArchiveT& ar, INT64 dwarray[], size_t count)
{
	const size_t cbToRead = sizeof(INT32) * count;
	return ar.get((char*)dwarray, cbToRead) == cbToRead;
}
template <class ReadArchiveT, class StrucType>
inline bool winx_call get_struct(ReadArchiveT& ar, StrucType array[], size_t count)
{
	const size_t cbToRead = sizeof(StrucType) * count;
	return ar.get((char*)array, cbToRead) == cbToRead;
}

#endif // !defined(WINX_BYTESWAP)

// -------------------------------------------------------------------------
// ==== Binary String Writer ====

//
// paddingIf(Fpad) => if Fpad -> 0:byte end.
//
// (Buf,Len):string0 => Len:byte test(Len < 254) Buf:char[Len];
// (Buf,Len):string0 => 254:byte Len:word Buf:char[Len];
// (Buf,Len):string0 => 255:byte Len:dword Buf:char[Len].
//
// string => (Buf,Len):string0 paddingIf(isEven(Len)).
//
template <class WriterArchiveT>
inline void winx_call puts(
	WriterArchiveT& ar, const char* lpBuf, size_t cch) throw(IoException)
{
	if (cch < 254) {
		ar.put( (UINT8)cch );
	}
	else if (cch < 65536) {
		ar.put( (UINT8)254 );
		put16i( ar, (UINT16)cch );
	}
	else {
		ar.put( (UINT8)255 );
		put32i( ar, (UINT32)cch );
	}
	ar.put( lpBuf, cch );
	if ( !(cch & 1) )
		ar.put( (UINT8)0 );
}

template <class WriterArchiveT>
inline void winx_call puts(
	WriterArchiveT& ar, const String& s) throw(IoException)
{
	puts(ar, s.data(), s.size());
}

//
// string => Len:word test(Len < 65535) Buf:word[Len];
// string => 65535:word Len:dword Buf:word[Len];
//
template <class WriterArchiveT>
inline void winx_call wputs(
	WriterArchiveT& ar, const UINT16* s, size_t cch) throw(IoException)
{
	if (cch < 65535) {
		put16i( ar, (UINT16)cch );
	}
	else {
		put16i( ar, (UINT16)65535 );
		put32i( ar, (UINT32)cch );
	}
	put16i( ar, s, cch );
}

template <class WriterArchiveT>
inline void winx_call wputs(
	WriterArchiveT& ar, const BasicString<UINT16>& s) throw(IoException)
{
	wputs(ar, s.data(), s.size());
}

// -------------------------------------------------------------------------
// ==== Binary String Reader ====

//
// get a binary string
//

template <class ReadArchiveT, class StringT>
inline bool winx_call gets(ReadArchiveT& ar, StringT& s)
{
	typedef typename ReadArchiveT::size_type size_type;
	
	bool hr;
	size_type cch = (size_type)ar.get();
	if (cch < 254) {
		/* nothing todo */
	}
	else if (cch == 254) {
		UINT16 cch2;
		if (!get16i(ar, cch2))
			return false;
		cch = cch2;
	}
	else if (cch == 255) {
		INT32 cch4;
		if (!get32i(ar, cch4) || cch4 < 0)
			return false;
		cch = cch4;
	}
	else {
		return false;
	}
	if (ar.get(std::resize(s, cch), cch) != cch)
		return false;
	if ( !(cch & 1) )
		ar.get(); // padding
	return true;
}

template <class ReadArchiveT, class AllocT>
inline bool winx_call gets(ReadArchiveT& ar, AllocT& alloc, BasicString<char>& s)
{
	OutputBasicString<char, AllocT> s1(alloc, s);
	return gets(ar, s1);
}

template <class ReadArchiveT, class StringT>
inline bool winx_call wgets(ReadArchiveT& ar, StringT& s)
{
	typedef typename ReadArchiveT::size_type size_type;

	size_type cch;
	UINT16 cch2;
	if (!get16i(ar, cch2)) {
		return false;
	}
	else if (cch2 == 65535) {
		INT32 cch4;
		if (!get32i(ar, cch4) || cch4 < 0)
			return false;
		cch = cch4;
	}
	else {
		cch = cch2;
	}
	return get16i(ar, std::resize(s, cch), cch);
}

template <class ReadArchiveT, class AllocT>
inline bool winx_call wgets(ReadArchiveT& ar, AllocT& alloc, BasicString<UINT16>& s)
{
	OutputBasicString<UINT16, AllocT> s1(alloc, s);
	return wgets(ar, s1);
}

// -------------------------------------------------------------------------
// $Log: Binary.h,v $

NS_STDEXT_IO_BINARY_END

#endif /* STDEXT_ARCHIVE_BINARY_H */

