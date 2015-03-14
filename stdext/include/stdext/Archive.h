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
// Module: stdext/Archive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-25 9:47:19
// 
// $Id: Archive.h,v 1.2 2006/11/30 03:19:17 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_H
#define STDEXT_ARCHIVE_H

#if (0)
#define WINX_STREAM_ARCHIVE
#endif

// -------------------------------------------------------------------------

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "archive/Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_BINARY_H
#include "archive/Binary.h"
#endif

#ifndef STDEXT_ARCHIVE_TEXT_H
#include "archive/Text.h"
#endif

#ifndef STDEXT_ARCHIVE_WRITEARCHIVE_H
#include "archive/WriteArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_READARCHIVE_H
#include "archive/ReadArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_STDIO_H
#include "archive/Stdio.h"
#endif

#ifndef STDEXT_ARCHIVE_POSIX_H
#include "archive/Posix.h"
#endif

#ifndef STDEXT_ARCHIVE_MEMARCHIVE_H
#include "archive/MemArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_MMAPARCHIVE_H
#include "archive/MMapArchive.h"
#endif

#ifndef STDEXT_ARCHIVE_MEM_H
#include "archive/Mem.h"
#endif

#ifndef STDEXT_ARCHIVE_RECORD_H
#include "archive/Record.h"
#endif

#if defined(X_OS_WINDOWS)

#if defined(WINX_STREAM_ARCHIVE)
	#ifndef STDEXT_ARCHIVE_STREAM_H
	#include "archive/Stream.h"
	#endif
#endif // defined(WINX_STREAM_ARCHIVE)

#ifndef STDEXT_ARCHIVE_WINREG_H
#include "archive/WinReg.h"
#endif

#endif

// -------------------------------------------------------------------------
// class FileReader/FileWriter

NS_STDEXT_BEGIN

typedef PosixAdapter FileDirectReader;
typedef PosixReadArchive FileReadArchive;
typedef FileReadArchive FileReader;

typedef PosixAdapter FileDirectWriter;
typedef PosixWriteArchive FileWriteArchive;
typedef FileWriteArchive FileWriter;

NS_STDEXT_END

// -------------------------------------------------------------------------
// $Log: Archive.h,v $

#endif /* STDEXT_ARCHIVE_H */
