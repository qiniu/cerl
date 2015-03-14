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
// Module: stdext/text/Append.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Append.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_APPEND_H
#define STDEXT_TEXT_APPEND_H

#ifndef STDEXT_TEXT_STRINGBUILDER_H
#include "StringBuilder.h"
#endif

#ifndef STDEXT_ARCHIVE_WRITEARCHIVE_H
#include "../archive/WriteArchive.h"
#endif

#ifndef NS_STDEXT_TEXT
#define NS_STDEXT_TEXT				NS_STDEXT::ns_text
#define NS_STDEXT_TEXT_BEGIN		namespace NS_STDEXT { namespace ns_text {
#define NS_STDEXT_TEXT_END			} }
#endif

NS_STDEXT_TEXT_BEGIN

// -------------------------------------------------------------------------

template <class CharT, class AllocT>
inline int winx_call append(std::vector<CharT, AllocT>& dest, const CharT* val, const CharT* valEnd)
{
	dest.insert(dest.end(), val, valEnd);
	return 0;
}

template <class CharT, class AllocT>
inline int winx_call append(std::vector<CharT, AllocT>& dest, const size_t count, const CharT val)
{
	dest.insert(dest.end(), count, val);
	return 0;
}

template <class CharT, class AllocT>
inline int winx_call append(std::vector<CharT, AllocT>& dest, const CharT val)
{
	dest.push_back(val);
	return 0;
}

template <class CharT>
inline int winx_call append(NS_STDEXT::BasicStringBuilder<CharT>& dest, const CharT val)
{
	dest.push_back(val);
	return 0;
}

template <class CharT, class AllocT>
inline void winx_call clear(std::vector<CharT, AllocT>& dest)
{
	dest.clear();
}

// -------------------------------------------------------------------------

template <class CharT, class Tr, class AllocT>
inline int winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT* val)
{
	dest.append(val);
	return 0;
}

template <class CharT, class Tr, class AllocT>
inline int winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT* val, const CharT* valEnd)
{
	dest.append(val, valEnd);
	return 0;
}

template <class CharT, class Tr, class AllocT>
inline int winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const size_t count, const CharT val)
{
	dest.append(count, val);
	return 0;
}

template <class CharT, class Tr, class AllocT>
inline int winx_call append(std::basic_string<CharT, Tr, AllocT>& dest, const CharT val)
{
	dest.append(1, val);
	return 0;
}

template <class CharT, class Tr, class AllocT>
inline void winx_call clear(std::basic_string<CharT, Tr, AllocT>& dest)
{
	dest.erase();
}

// -------------------------------------------------------------------------

template<class CharT, class Tr>
inline int winx_call append(std::basic_ostream<CharT, Tr>& os, const CharT* val, const CharT* valEnd)
{
	os.write(val, valEnd - val);
	return os.rdstate();
}

template<class CharT, class Tr>
inline int winx_call append(std::basic_ostream<CharT, Tr>& os, size_t count, const CharT val)
{
	while (count--)
		os.put(val);
	return os.rdstate();
}

template<class CharT, class Tr>
inline int winx_call append(std::basic_ostream<CharT, Tr>& os, const CharT val)
{
	os.put(val);
	return os.rdstate();
}

template<class CharT, class Tr>
inline void winx_call clear(std::basic_ostream<CharT, Tr>& os)
{
}

// -------------------------------------------------------------------------

template <class Handle, class StreamHandle, class CacheT, class CharT>
inline int winx_call append(WriteArchive<Handle, StreamHandle, CacheT>& ar, const CharT* val, const CharT* valEnd)
{
	ar.put(val, valEnd - val);
	return 0;
}

template <class Handle, class StreamHandle, class CacheT, class CharT>
inline int winx_call append(WriteArchive<Handle, StreamHandle, CacheT>& ar, size_t count, const CharT val)
{
	while (count--)
		ar.put(val);
	return 0;
}

template <class Handle, class StreamHandle, class CacheT, class CharT>
inline int winx_call append(WriteArchive<Handle, StreamHandle, CacheT>& ar, const CharT val)
{
	ar.put(val);
	return 0;
}

template <class Handle, class StreamHandle, class CacheT>
inline void winx_call clear(WriteArchive<Handle, StreamHandle, CacheT>& ar)
{
}

// -------------------------------------------------------------------------

template <class CharT>
inline int winx_call append(FILE& fp, const CharT* val, const CharT* valEnd)
{
	fwrite(val, sizeof(CharT), valEnd - val, &fp);
	return 0;
}

inline int winx_call append(FILE& fp, size_t count, const char val)
{
	while (count--)
		putc(val, &fp);
	return 0;
}

inline int winx_call append(FILE& fp, const char val)
{
	putc(val, &fp);
	return 0;
}

inline void winx_call clear(FILE& dest)
{
}

// -------------------------------------------------------------------------

template <class ArchiveT>
class Archive2Appender : public ArchiveT
{
private:
	Archive2Appender();
	Archive2Appender(const Archive2Appender&);
	void operator=(const Archive2Appender&);
};

template <class ArchiveT, class CharT>
inline int winx_call append(Archive2Appender<ArchiveT>& app, const CharT* val, const CharT* valEnd)
{
	app.put(val, valEnd - val);
	return 0;
}

template <class ArchiveT, class CharT>
inline int winx_call append(Archive2Appender<ArchiveT>& app, size_t count, const CharT val)
{
	while (count--)
		app.put(val);
	return 0;
}

template <class ArchiveT, class CharT>
inline int winx_call append(Archive2Appender<ArchiveT>& app, const CharT val)
{
	app.put(val);
	return 0;
}

template <class ArchiveT>
inline void winx_call clear(Archive2Appender<ArchiveT>& app)
{
}

// -------------------------------------------------------------------------
// class File2Appender

template <class FileT>
class File2Appender : public FileT
{
private:
	File2Appender();
	File2Appender(const File2Appender&);
	void operator=(const File2Appender&);
};

template <class FileT, class CharT>
inline int winx_call append(File2Appender<FileT>& app, const CharT* val, const CharT* valEnd)
{
	return app.put(val, valEnd - val);
}

template <class FileT, class CharT>
inline int winx_call append(File2Appender<FileT>& app, size_t count, const CharT val)
{
	return app.put(count, val);
}

template <class FileT>
inline int winx_call append(File2Appender<FileT>& app, const char val)
{
	return app.put(val);
}

template <class FileT>
inline int winx_call append(File2Appender<FileT>& app, const char* val)
{
	return app.put(val);
}

template <class FileT>
inline void winx_call clear(File2Appender<FileT>& app)
{
}

// -------------------------------------------------------------------------

template <class StringT>
inline int winx_call append(StringT& dest, const String& val)
{
	return append(dest, val.begin(), val.end());
}

template <class StringT>
inline int winx_call append(StringT& dest, const WString& val)
{
	return append(dest, val.begin(), val.end());
}

// -------------------------------------------------------------------------

NS_STDEXT_TEXT_END

NS_STDEXT_BEGIN // function ar2appender

template <class ArchiveT>
__forceinline NS_STDEXT_TEXT::Archive2Appender<ArchiveT>& winx_call ar2appender(ArchiveT& ar)
{
	return *(NS_STDEXT_TEXT::Archive2Appender<ArchiveT>*)&ar;
}

template <class FileT>
__forceinline NS_STDEXT_TEXT::File2Appender<FileT>& winx_call appender(FileT& file)
{
	return *(NS_STDEXT_TEXT::File2Appender<FileT>*)&file;
}

NS_STDEXT_END

// -------------------------------------------------------------------------

#endif /* STDEXT_TEXT_APPEND_H */
