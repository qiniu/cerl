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
// Module: stdext/mmap/MapFile.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MMAP_MAPFILE_H
#define STDEXT_MMAP_MAPFILE_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

// -------------------------------------------------------------------------
// class MapFile

#if defined(X_OS_WINDOWS)

#ifndef STDEXT_MAPFILE_WINFILEMAPPING_H
#include "../mapfile/WinFileMapping.h"
#endif

#else

#ifndef STDEXT_MAPFILE_MMAP_H
#include "../mapfile/MMap.h"
#endif

#endif

NS_STDEXT_BEGIN

typedef MapFile<MappingReadWrite> MapFileRW;
typedef MapFile<MappingReadOnly> MapFileRO;

NS_STDEXT_END

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_MMAP_MAPFILE_H */
