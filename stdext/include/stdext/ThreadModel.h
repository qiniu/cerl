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
// Module: stdext/ThreadModel.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-9-3 2:18:33
// 
// $Id: ThreadModel.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_THREADMODEL_H
#define STDEXT_THREADMODEL_H

#ifndef STDEXT_THREAD_REFCOUNT_H
#include "thread/RefCount.h"
#endif

#ifndef STDEXT_THREAD_MUTEX_H
#include "thread/Mutex.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class CriticalSectionST, CriticalSectionMT

typedef FakeMutex CriticalSectionST;
typedef Mutex CriticalSectionMT;

// -------------------------------------------------------------------------
// class MultiThreadModel

class MT
{
public:
	typedef RefCountMT RefCount;
	typedef CriticalSectionMT CriticalSection;

public:
	typedef CriticalSection CS;
	typedef CS::scoped_lock CSLock;
};

typedef MT MultiThreadModel;

// -------------------------------------------------------------------------
// class SingleThreadModel

class ST
{
public:
	typedef RefCountST RefCount;
	typedef CriticalSectionST CriticalSection;

public:
	typedef CriticalSection CS;
	typedef CS::scoped_lock CSLock;
};

typedef ST SingleThreadModel;

// -------------------------------------------------------------------------
// class DefaultThreadModel

#if defined(_MT)
typedef MultiThreadModel DefaultThreadModel;
#else
typedef SingleThreadModel DefaultThreadModel;
#endif

// -------------------------------------------------------------------------
//	$Log: $

NS_STDEXT_END

#endif /* STDEXT_THREADMODEL_H */
