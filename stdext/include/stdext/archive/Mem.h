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
// Module: stdext/archive/Mem.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 21:07:06
// 
// $Id: Mem.h,v 1.3 2007/01/10 09:36:12 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_MEM_H
#define STDEXT_ARCHIVE_MEM_H

#ifndef STDEXT_ARCHIVE_MEMARCHIVE_H
#include "MemArchive.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class MemReader

typedef MemReadArchive<const char*> PointerReadArchive;
typedef PointerReadArchive MemReader;

// -------------------------------------------------------------------------
// class VectorReader/VectorWriter

typedef std::vector<char> CharVector;

typedef MemWriteArchive<CharVector> VectorWriteArchive;
typedef VectorWriteArchive VectorWriter;

typedef MemReadArchive<CharVector::const_iterator> VectorReadArchive;
typedef VectorReadArchive VectorReader;

// -------------------------------------------------------------------------
// class StringBuilderReader/StringBuilderWriter

typedef VectorWriter StringBuilderWriter;
typedef VectorReader StringBuilderReader;

// -------------------------------------------------------------------------
// class StlStringReader/StlStringWriter

typedef MemWriteArchive<std::string> StlStringWriteArchive;
typedef StlStringWriteArchive StlStringWriter;

typedef MemReadArchive<std::string::const_iterator> StlStringReadArchive;
typedef StlStringReadArchive StlStringReader;

// -------------------------------------------------------------------------
// class DequeReader/DequeWriter, TextPoolReader/TextPoolWriter

#ifdef STDEXT_DEQUE_H

typedef std::Deque<char> CharDeque;

typedef MemWriteArchive<CharDeque> DequeWriteArchive;
typedef DequeWriteArchive DequeWriter;

typedef MemReadArchive<CharDeque::const_iterator> DequeReadArchive;
typedef DequeReadArchive DequeReader;

typedef DequeWriter TextPoolWriter;
typedef DequeReader TextPoolReader;

#endif

NS_STDEXT_END

// -------------------------------------------------------------------------
// $Log: Mem.h,v $

#endif /* STDEXT_ARCHIVE_MEM_H */
