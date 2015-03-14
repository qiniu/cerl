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
// Module: stdext/msvcrt/crtdbg.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-1-14 15:24:12
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MSVCRT_CRTDBG_H
#define STDEXT_MSVCRT_CRTDBG_H

#if !defined(STD_NO_MSVCRT)
#error "Don't include <stdext/msvcrt/crtdbg.h>"
#endif

#ifndef _STDARG_H
#include <stdarg.h>
#endif

// -------------------------------------------------------------------------
// ==== _vsnprintf, _assert ==> required by crtdbg.h ====

//
// stdio.h
//
#ifdef WINX_GCC
#ifndef _STDIO_H
#include <stdio.h>
#endif
#define _vsnprintf(buf, n, fmt, arglist) vsnprintf(buf, n, fmt, arglist)
#endif

//
// assert.h
//
#ifdef WINX_GCC
#ifndef _ASSERT_H
#include <assert.h>
#endif
#define _assert(exp, file, line) __assert_fail(exp, (const char*)(file), line, __ASSERT_FUNCTION)
#endif

// -------------------------------------------------------------------------

#if defined(_DEBUG)

inline int _ExCrtDbgBreak(
    int nRptType, const char * szFile, int nLine, const char * szModule,
    const char * szFormat, ...)
{
	va_list arglist;
	va_start(arglist, szFormat);
	char buff[2048];
	_vsnprintf(buff, 2048, szFormat, arglist);
	_assert(buff, (void*)szFile, nLine);
	va_end(arglist);
	return 1;
}

#endif

/****************************************************************************
 *
 * Debug Reporting
 *
 ***************************************************************************/

typedef void *_HFILE; /* file handle pointer */

#define _CRT_WARN           0
#define _CRT_ERROR          1
#define _CRT_ASSERT         2
#define _CRT_ERRCNT         3

#define _CRTDBG_MODE_FILE      0x1
#define _CRTDBG_MODE_DEBUG     0x2
#define _CRTDBG_MODE_WNDW      0x4
#define _CRTDBG_REPORT_MODE    -1

#define _CRTDBG_INVALID_HFILE ((_HFILE)-1)
#define _CRTDBG_HFILE_ERROR   ((_HFILE)-2)
#define _CRTDBG_FILE_STDOUT   ((_HFILE)-4)
#define _CRTDBG_FILE_STDERR   ((_HFILE)-5)
#define _CRTDBG_REPORT_FILE   ((_HFILE)-6)

typedef int (__cdecl * _CRT_REPORT_HOOK)(int, char *, int *);

 /****************************************************************************.
 *
 * Client-defined allocation hook
 *
 ***************************************************************************/

#define _HOOK_ALLOC     1
#define _HOOK_REALLOC   2
#define _HOOK_FREE      3

typedef int (__cdecl * _CRT_ALLOC_HOOK)(int, void *, size_t, int, long, const unsigned char *, int);

/*
 * Bit values for _crtDbgFlag flag:
 *
 * These bitflags control debug heap behavior.
 */

#define _CRTDBG_ALLOC_MEM_DF        0x01  /* Turn on debug allocation */
#define _CRTDBG_DELAY_FREE_MEM_DF   0x02  /* Don't actually free memory */
#define _CRTDBG_CHECK_ALWAYS_DF     0x04  /* Check heap every alloc/dealloc */
#define _CRTDBG_RESERVED_DF         0x08  /* Reserved - do not use */
#define _CRTDBG_CHECK_CRT_DF        0x10  /* Leak check/diff CRT blocks */
#define _CRTDBG_LEAK_CHECK_DF       0x20  /* Leak check at program exit */

#define _CRTDBG_REPORT_FLAG         -1    /* Query bitflag status */

#define _BLOCK_TYPE(block)          (block & 0xFFFF)
#define _BLOCK_SUBTYPE(block)       (block >> 16 & 0xFFFF)

/* Memory block identification */
#define _FREE_BLOCK      0
#define _NORMAL_BLOCK    1
#define _CRT_BLOCK       2
#define _IGNORE_BLOCK    3
#define _CLIENT_BLOCK    4
#define _MAX_BLOCKS      5

typedef void (__cdecl * _CRT_DUMP_CLIENT)(void *, size_t);

typedef struct _CrtMemState
{
        struct _CrtMemBlockHeader * pBlockHeader;
        unsigned long lCounts[_MAX_BLOCKS];
        unsigned long lSizes[_MAX_BLOCKS];
        unsigned long lHighWaterCount;
        unsigned long lTotalCount;
} _CrtMemState;

#ifndef _DEBUG

#define _ASSERT(expr) ((void)0)
#define _ASSERTE(expr) ((void)0)

#define _RPT0(rptno, msg)
#define _RPT1(rptno, msg, arg1)
#define _RPT2(rptno, msg, arg1, arg2)
#define _RPT3(rptno, msg, arg1, arg2, arg3)
#define _RPT4(rptno, msg, arg1, arg2, arg3, arg4)


#define _RPTF0(rptno, msg)
#define _RPTF1(rptno, msg, arg1)
#define _RPTF2(rptno, msg, arg1, arg2)
#define _RPTF3(rptno, msg, arg1, arg2, arg3)
#define _RPTF4(rptno, msg, arg1, arg2, arg3, arg4)

#define _malloc_dbg(s, t, f, l)         malloc(s)
#define _calloc_dbg(c, s, t, f, l)      calloc(c, s)
#define _realloc_dbg(p, s, t, f, l)     realloc(p, s)
#define _expand_dbg(p, s, t, f, l)      _expand(p, s)
#define _free_dbg(p, t)                 free(p)
#define _msize_dbg(p, t)                _msize(p)

#define _CrtSetReportHook(f)                ((void)0)
#define _CrtSetReportMode(t, f)             ((int)0)
#define _CrtSetReportFile(t, f)             ((void)0)

#define _CrtDbgBreak()                      ((void)0)
#define _CrtSetBreakAlloc(a)                ((long)0)
#define _CrtSetAllocHook(f)                 ((void)0)

#define _CrtCheckMemory()                   ((int)1)
#define _CrtSetDbgFlag(f)
#define _CrtDoForAllClientObjects(f, c)     ((void)0)
#define _CrtIsValidPointer(p, n, r)         ((int)1)
#define _CrtIsValidHeapPointer(p)           ((int)1)
#define _CrtIsMemoryBlock(p, t, r, f, l)    ((int)1)

#define _CrtSetDumpClient(f)                ((void)0)

#define _CrtMemCheckpoint(s)                ((void)0)
#define _CrtMemDifference(s1, s2, s3)       ((int)0)
#define _CrtMemDumpAllObjectsSince(s)       ((void)0)
#define _CrtMemDumpStatistics(s)            ((void)0)
#define _CrtDumpMemoryLeaks()               ((int)0)

#else   /* _DEBUG */

#define _ASSERT(expr) assert(expr)
#define _ASSERTE(expr) assert(expr)

#define _RPT0(rptno, msg)					_ExCrtDbgBreak(rptno, __FILE__, __LINE__, NULL, msg)
#define _RPT1(rptno, msg, arg1)				_ExCrtDbgBreak(rptno, __FILE__, __LINE__, NULL, msg, arg1)
#define _RPT2(rptno, msg, arg1, arg2)		_ExCrtDbgBreak(rptno, __FILE__, __LINE__, NULL, msg, arg1, arg2)
#define _RPT3(rptno, msg, arg1, arg2, arg3)	_ExCrtDbgBreak(rptno, __FILE__, __LINE__, NULL, msg, arg1, arg2, arg3)
#define _RPT4(rptno, msg, arg1, arg2, arg3, arg4)	_ExCrtDbgBreak(rptno, __FILE__, __LINE__, NULL, msg, arg1, arg2, arg3, arg4)

#define _RPTF0(rptno, msg)					_RPT0(rptno, msg)
#define _RPTF1(rptno, msg, arg1)			_RPT1(rptno, msg, arg1)
#define _RPTF2(rptno, msg, arg1, arg2)		_RPT2(rptno, msg, arg1, arg2)
#define _RPTF3(rptno, msg, arg1, arg2, arg3)	_RPT3(rptno, msg, arg1, arg2, arg3)
#define _RPTF4(rptno, msg, arg1, arg2, arg3, arg4)	_RPT4(rptno, msg, arg1, arg2, arg3, arg4)

#define _malloc_dbg(s, t, f, l)         malloc(s)
#define _calloc_dbg(c, s, t, f, l)      calloc(c, s)
#define _realloc_dbg(p, s, t, f, l)     realloc(p, s)
#define _expand_dbg(p, s, t, f, l)      _expand(p, s)
#define _free_dbg(p, t)                 free(p)
#define _msize_dbg(p, t)                _msize(p)

#define _CrtSetReportHook(f)                ((void)0)
#define _CrtSetReportMode(t, f)             ((int)0)
#define _CrtSetReportFile(t, f)             ((void)0)

#define _CrtDbgBreak()						_assert("_CrtDbgBreak", __FILE__, __LINE__)
#define _CrtSetBreakAlloc(a)                
#define _CrtSetAllocHook(f)                 

#define _CrtCheckMemory()                   ((int)1)
#define _CrtSetDbgFlag(f)					
#define _CrtDoForAllClientObjects(f, c)     ((void)0)
#define _CrtIsValidPointer(p, n, r)         ((int)1)
#define _CrtIsValidHeapPointer(p)           ((int)1)
#define _CrtIsMemoryBlock(p, t, r, f, l)    ((int)1)

#define _CrtSetDumpClient(f)                ((void)0)

#define _CrtMemCheckpoint(s)                ((void)0)
#define _CrtMemDifference(s1, s2, s3)       ((int)0)
#define _CrtMemDumpAllObjectsSince(s)       ((void)0)
#define _CrtMemDumpStatistics(s)            ((void)0)
#define _CrtDumpMemoryLeaks()               ((int)0)

#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_MSVCRT_CRTDBG_H */
