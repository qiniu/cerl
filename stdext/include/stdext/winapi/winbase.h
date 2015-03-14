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
// Module: stdext/winapi/winbase.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:08:57
// 
// $Id: winbase.h,v 1.6 2007/01/10 09:38:10 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_WINBASE_H
#define STDEXT_WINAPI_WINBASE_H

// =========================================================================

#if defined(_WIN32) || defined(_WIN64)

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef _WINBASE_
#include <winbase.h>
#endif

#ifndef _BASETSD_H_
#include <basetsd.h>
#endif

#if !defined(_W64)
	#define WINX_DETAIL_WINSDK_VC6
	#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
	#define _W64 __w64
	#else
	#define _W64
	#endif
#endif

#if defined(WINX_DETAIL_WINSDK_VC6)
	#if defined(_WIN64)
		typedef __int64 LONG_PTR, *PLONG_PTR;
		typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
	#else
		typedef _W64 long LONG_PTR, *PLONG_PTR;
		typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;
	#endif
#endif

#ifndef STDEXT_WINAPI_ATOMIC_WINDOWS_H
#include "atomic/windows.h"
#endif

#else

#define STD_NO_WINSDK

#ifndef STDEXT_WINAPI_WTYPES_H
#include "wtypes.h"
#endif

#ifndef STDEXT_WINAPI_POSIX_PTHREAD_H
#include "posix/pthread.h"
#endif

#ifndef STDEXT_WINAPI_POSIX_TIME_H
#include "posix/time.h"
#endif

#ifndef STDEXT_WINAPI_ATOMIC_ATOMIC_BUILTINS_H
#include "atomic/atomic_builtins.h"
#endif

#endif

// =========================================================================

#ifdef STD_NO_WINSDK

// -------------------------------------------------------------------------
// NotImplementFeature

#if !defined(STD_NO_MSVCRT)
	#ifndef _INC_CRTDBG
	#include <crtdbg.h>
	#endif
#else
	#ifndef STDEXT_MSVCRT_CRTDBG_H
	#include "../msvcrt/crtdbg.h"
	#endif
#endif

inline void NotImplementFeature(const char* szFeature)
{
	_RPT1(_CRT_ERROR, "NotImplementFeature - %s\n", szFeature);
	throw szFeature;
}

// -------------------------------------------------------------------------

#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)

// -------------------------------------------------------------------------

#define RtlEqualMemory(Destination,Source,Length) (!memcmp((Destination),(Source),(Length)))
#define RtlMoveMemory(Destination,Source,Length) memmove((Destination),(Source),(Length))
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define RtlFillMemory(Destination,Length,Fill) memset((Destination),(Fill),(Length))
#define RtlZeroMemory(Destination,Length) memset((Destination),0,(Length))

#define MoveMemory RtlMoveMemory
#define CopyMemory RtlCopyMemory
#define FillMemory RtlFillMemory
#define ZeroMemory RtlZeroMemory

// -------------------------------------------------------------------------

#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
#define FORMAT_MESSAGE_FROM_STRING     0x00000400
#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

inline DWORD WINAPI GetLastError()
{
	NotImplementFeature("GetLastError");
	return 0;
}

inline VOID WINAPI SetLastError(DWORD dwErrCode)
{
	NotImplementFeature("SetLastError");
}

inline DWORD WINAPI FormatMessageA(
    DWORD dwFlags,
    LPCVOID lpSource,
    DWORD dwMessageId,
    DWORD dwLanguageId,
    LPSTR lpBuffer,
    DWORD nSize,
    va_list *Arguments)
{
	NotImplementFeature("FormatMessageA");
	return 0;
}

inline DWORD WINAPI FormatMessageW(
    DWORD dwFlags,
    LPCVOID lpSource,
    DWORD dwMessageId,
    DWORD dwLanguageId,
    LPWSTR lpBuffer,
    DWORD nSize,
    va_list *Arguments)
{
	NotImplementFeature("FormatMessageW");
	return 0;
}

#ifdef UNICODE
#define FormatMessage  FormatMessageW
#else
#define FormatMessage  FormatMessageA
#endif // !UNICODE

// -------------------------------------------------------------------------

inline VOID WINAPI OutputDebugStringA(LPCSTR lpOutputString)
{
	NotImplementFeature("OutputDebugStringA");
}

inline VOID WINAPI OutputDebugStringW(LPCWSTR lpOutputString)
{
	NotImplementFeature("OutputDebugStringW");
}

#ifdef UNICODE
#define OutputDebugString  OutputDebugStringW
#else
#define OutputDebugString  OutputDebugStringA
#endif // !UNICODE

// -------------------------------------------------------------------------

inline VOID WINAPI GetSystemTime(
    LPSYSTEMTIME lpSystemTime)
{
	NotImplementFeature("GetSystemTime");
}

// -------------------------------------------------------------------------

inline DWORD WINAPI GetModuleFileNameA(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize)
{
	NotImplementFeature("GetModuleFileName");
	return 0;
}

inline DWORD WINAPI GetModuleFileNameW(
    HMODULE hModule,
    LPWSTR lpFilename,
    DWORD nSize)
{
	NotImplementFeature("GetModuleFileName");
	return 0;
}

#ifdef UNICODE
#define GetModuleFileName  GetModuleFileNameW
#else
#define GetModuleFileName  GetModuleFileNameA
#endif // !UNICODE

// -------------------------------------------------------------------------

#define HEAP_NO_SERIALIZE               0x00000001      
#define HEAP_GROWABLE                   0x00000002      
#define HEAP_GENERATE_EXCEPTIONS        0x00000004      
#define HEAP_ZERO_MEMORY                0x00000008      
#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010      
#define HEAP_TAIL_CHECKING_ENABLED      0x00000020      
#define HEAP_FREE_CHECKING_ENABLED      0x00000040      
#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080      
#define HEAP_CREATE_ALIGN_16            0x00010000      
#define HEAP_CREATE_ENABLE_TRACING      0x00020000      
#define HEAP_MAXIMUM_TAG                0x0FFF              
#define HEAP_PSEUDO_TAG_FLAG            0x8000              
#define HEAP_TAG_SHIFT                  18                  
#define HEAP_MAKE_TAG_FLAGS( b, o ) ((DWORD)((b) + ((o) << 18)))  

inline HANDLE WINAPI HeapCreate(
	DWORD flOptions,
    DWORD dwInitialSize,
    DWORD dwMaximumSize)
{
	NotImplementFeature("HeapCreate");
	return NULL;
}

inline BOOL WINAPI HeapDestroy(
    HANDLE hHeap)
{
	NotImplementFeature("HeapDestroy");
	return TRUE;
}

inline LPVOID WINAPI HeapAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
    DWORD dwBytes)
{
	NotImplementFeature("HeapAlloc");
	return NULL;
}

inline LPVOID WINAPI HeapReAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
    LPVOID lpMem,
    DWORD dwBytes)
{
	NotImplementFeature("HeapReAlloc");
	return NULL;
}

inline BOOL WINAPI HeapFree(
    HANDLE hHeap,
    DWORD dwFlags,
    LPVOID lpMem)
{
	NotImplementFeature("HeapFree");
	return TRUE;
}

inline DWORD WINAPI HeapSize(
    HANDLE hHeap,
    DWORD dwFlags,
    LPCVOID lpMem)
{
	NotImplementFeature("HeapSize");
	return 0;
}

inline HANDLE WINAPI GetProcessHeap()
{
	NotImplementFeature("GetProcessHeap");
	return NULL;
}

// -------------------------------------------------------------------------

inline HLOCAL WINAPI LocalAlloc(
    UINT uFlags,
    UINT uBytes)
{
	NotImplementFeature("LocalAlloc");
	return NULL;
}

inline HLOCAL WINAPI LocalReAlloc(
    HLOCAL hMem,
    UINT uBytes,
    UINT uFlags)
{
	NotImplementFeature("LocalReAlloc");
	return NULL;
}

inline LPVOID WINAPI LocalLock(
    HLOCAL hMem)
{
	NotImplementFeature("LocalLock");
	return NULL;
}

inline HLOCAL WINAPI LocalHandle(
    LPCVOID pMem)
{
	NotImplementFeature("LocalHandle");
	return NULL;
}

inline BOOL WINAPI LocalUnlock(
    HLOCAL hMem)
{
	NotImplementFeature("LocalUnlock");
	return TRUE;
}

inline UINT WINAPI LocalSize(
    HLOCAL hMem)
{
	NotImplementFeature("LocalSize");
	return 0;
}

inline UINT WINAPI LocalFlags(
    HLOCAL hMem)
{
	NotImplementFeature("LocalFlags");
	return 0;
}

inline HLOCAL WINAPI LocalFree(
    HLOCAL hMem)
{
	NotImplementFeature("LocalFree");
	return NULL;
}

// -------------------------------------------------------------------------

inline HGLOBAL WINAPI GlobalAlloc(
    UINT uFlags,
    DWORD dwBytes)
{
	NotImplementFeature("GlobalAlloc");
	return NULL;
}

inline HGLOBAL WINAPI GlobalReAlloc(
    HGLOBAL hMem,
    DWORD dwBytes,
    UINT uFlags)
{
	NotImplementFeature("GlobalReAlloc");
	return NULL;
}

inline DWORD WINAPI GlobalSize(
    HGLOBAL hMem)
{
	NotImplementFeature("GlobalSize");
	return 0;
}

inline UINT WINAPI GlobalFlags(
	HGLOBAL hMem)
{
	NotImplementFeature("GlobalFlags");
	return 0;
}

inline LPVOID WINAPI GlobalLock(
    HGLOBAL hMem)
{
	NotImplementFeature("GlobalLock");
	return NULL;
}

inline BOOL WINAPI GlobalUnlock(
    HGLOBAL hMem)
{
	NotImplementFeature("GlobalUnlock");
	return TRUE;
}

inline HGLOBAL WINAPI GlobalFree(
    HGLOBAL hMem)
{
	NotImplementFeature("GlobalFree");
	return NULL;
}

// -------------------------------------------------------------------------

//
// The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
// devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
// constants *MUST* always be in sync.
// The values are redefined in devioctl.h because they must be available to
// both DOS and NT.
//

#define FILE_READ_DATA            ( 0x0001 )    // file & pipe
#define FILE_LIST_DIRECTORY       ( 0x0001 )    // directory

#define FILE_WRITE_DATA           ( 0x0002 )    // file & pipe
#define FILE_ADD_FILE             ( 0x0002 )    // directory

#define FILE_APPEND_DATA          ( 0x0004 )    // file
#define FILE_ADD_SUBDIRECTORY     ( 0x0004 )    // directory
#define FILE_CREATE_PIPE_INSTANCE ( 0x0004 )    // named pipe


#define FILE_READ_EA              ( 0x0008 )    // file & directory

#define FILE_WRITE_EA             ( 0x0010 )    // file & directory

#define FILE_EXECUTE              ( 0x0020 )    // file
#define FILE_TRAVERSE             ( 0x0020 )    // directory

#define FILE_DELETE_CHILD         ( 0x0040 )    // directory

#define FILE_READ_ATTRIBUTES      ( 0x0080 )    // all

#define FILE_WRITE_ATTRIBUTES     ( 0x0100 )    // all

#define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3FF)

#define FILE_GENERIC_READ         (STANDARD_RIGHTS_READ     |\
                                   FILE_READ_DATA           |\
                                   FILE_READ_ATTRIBUTES     |\
                                   FILE_READ_EA             |\
                                   SYNCHRONIZE)


#define FILE_GENERIC_WRITE        (STANDARD_RIGHTS_WRITE    |\
                                   FILE_WRITE_DATA          |\
                                   FILE_WRITE_ATTRIBUTES    |\
                                   FILE_WRITE_EA            |\
                                   FILE_APPEND_DATA         |\
                                   SYNCHRONIZE)


#define FILE_GENERIC_EXECUTE      (STANDARD_RIGHTS_EXECUTE  |\
                                   FILE_READ_ATTRIBUTES     |\
                                   FILE_EXECUTE             |\
                                   SYNCHRONIZE)

#define FILE_SHARE_READ                 0x00000001  
#define FILE_SHARE_WRITE                0x00000002  
#define FILE_SHARE_DELETE               0x00000004  
#define FILE_ATTRIBUTE_READONLY             0x00000001  
#define FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define FILE_ATTRIBUTE_ENCRYPTED            0x00000040  
#define FILE_ATTRIBUTE_NORMAL               0x00000080  
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400  
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
#define FILE_ATTRIBUTE_OFFLINE              0x00001000  
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001   
#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002   
#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004   
#define FILE_NOTIFY_CHANGE_SIZE         0x00000008   
#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010   
#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020   
#define FILE_NOTIFY_CHANGE_CREATION     0x00000040   
#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100   
#define FILE_ACTION_ADDED                   0x00000001   
#define FILE_ACTION_REMOVED                 0x00000002   
#define FILE_ACTION_MODIFIED                0x00000003   
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004   
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005   
#define MAILSLOT_NO_MESSAGE             ((DWORD)-1) 
#define MAILSLOT_WAIT_FOREVER           ((DWORD)-1) 
#define FILE_CASE_SENSITIVE_SEARCH      0x00000001  
#define FILE_CASE_PRESERVED_NAMES       0x00000002  
#define FILE_UNICODE_ON_DISK            0x00000004  
#define FILE_PERSISTENT_ACLS            0x00000008  
#define FILE_FILE_COMPRESSION           0x00000010  
#define FILE_VOLUME_QUOTAS              0x00000020  
#define FILE_SUPPORTS_SPARSE_FILES      0x00000040  
#define FILE_SUPPORTS_REPARSE_POINTS    0x00000080  
#define FILE_SUPPORTS_REMOTE_STORAGE    0x00000100  
#define FILE_VOLUME_IS_COMPRESSED       0x00008000  
#define FILE_SUPPORTS_OBJECT_IDS        0x00010000  
#define FILE_SUPPORTS_ENCRYPTION        0x00020000  

#define SECTION_QUERY       0x0001
#define SECTION_MAP_WRITE   0x0002
#define SECTION_MAP_READ    0x0004
#define SECTION_MAP_EXECUTE 0x0008
#define SECTION_EXTEND_SIZE 0x0010

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
                            SECTION_MAP_WRITE |      \
                            SECTION_MAP_READ |       \
                            SECTION_MAP_EXECUTE |    \
                            SECTION_EXTEND_SIZE)

#define FILE_MAP_COPY       SECTION_QUERY
#define FILE_MAP_WRITE      SECTION_MAP_WRITE
#define FILE_MAP_READ       SECTION_MAP_READ
#define FILE_MAP_ALL_ACCESS SECTION_ALL_ACCESS

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

//
// File creation flags must start at the high end since they
// are combined with the attributes
//

#define FILE_FLAG_WRITE_THROUGH         0x80000000
#define FILE_FLAG_OVERLAPPED            0x40000000
#define FILE_FLAG_NO_BUFFERING          0x20000000
#define FILE_FLAG_RANDOM_ACCESS         0x10000000
#define FILE_FLAG_SEQUENTIAL_SCAN       0x08000000
#define FILE_FLAG_DELETE_ON_CLOSE       0x04000000
#define FILE_FLAG_BACKUP_SEMANTICS      0x02000000
#define FILE_FLAG_POSIX_SEMANTICS       0x01000000
#define FILE_FLAG_OPEN_REPARSE_POINT    0x00200000
#define FILE_FLAG_OPEN_NO_RECALL        0x00100000

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define PAGE_NOACCESS          0x01     
#define PAGE_READONLY          0x02     
#define PAGE_READWRITE         0x04     
#define PAGE_WRITECOPY         0x08     
#define PAGE_EXECUTE           0x10     
#define PAGE_EXECUTE_READ      0x20     
#define PAGE_EXECUTE_READWRITE 0x40     
#define PAGE_EXECUTE_WRITECOPY 0x80     
#define PAGE_GUARD            0x100     
#define PAGE_NOCACHE          0x200     
#define PAGE_WRITECOMBINE     0x400     
#define MEM_COMMIT           0x1000     
#define MEM_RESERVE          0x2000     
#define MEM_DECOMMIT         0x4000     
#define MEM_RELEASE          0x8000     
#define MEM_FREE            0x10000     
#define MEM_PRIVATE         0x20000     
#define MEM_MAPPED          0x40000     
#define MEM_RESET           0x80000     
#define MEM_TOP_DOWN       0x100000     
#define MEM_4MB_PAGES    0x80000000     
#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_VLM           0x2000000     
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     
#define MEM_IMAGE         SEC_IMAGE     

//
//  File structures
//

typedef struct _OVERLAPPED {
    DWORD   Internal;
    DWORD   InternalHigh;
    DWORD   Offset;
    DWORD   OffsetHigh;
    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

inline HANDLE WINAPI CreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
	NotImplementFeature("CreateFile");
	return NULL;
}

inline HANDLE WINAPI CreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
	NotImplementFeature("CreateFile");
	return NULL;
}

#ifdef UNICODE
#define CreateFile  CreateFileW
#else
#define CreateFile  CreateFileA
#endif // !UNICODE

inline DWORD WINAPI GetFileSize(
    HANDLE hFile,
    LPDWORD lpFileSizeHigh)
{
	NotImplementFeature("GetFileSize");
	return 0;
}

inline BOOL WINAPI WriteFile(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped)
{
	NotImplementFeature("WriteFile");
	return TRUE;
}

inline BOOL WINAPI ReadFile(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped)
{
	NotImplementFeature("ReadFile");
	return TRUE;
}

inline BOOL WINAPI FlushFileBuffers(
    HANDLE hFile)
{
	NotImplementFeature("FlushFileBuffers");
	return TRUE;
}

inline HANDLE WINAPI CreateFileMappingA(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCSTR lpName)
{
	NotImplementFeature("CreateFileMapping");
	return NULL;
}

inline HANDLE WINAPI CreateFileMappingW(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName)
{
	NotImplementFeature("CreateFileMapping");
	return NULL;
}

#ifdef UNICODE
#define CreateFileMapping  CreateFileMappingW
#else
#define CreateFileMapping  CreateFileMappingA
#endif // !UNICODE

inline BOOL WINAPI FlushViewOfFile(
    LPCVOID lpBaseAddress,
    DWORD dwNumberOfBytesToFlush)
{
	NotImplementFeature("FlushViewOfFile");
	return TRUE;
}

inline LPVOID WINAPI MapViewOfFileEx(
    HANDLE hFileMappingObject,
    DWORD dwDesiredAccess,
    DWORD dwFileOffsetHigh,
    DWORD dwFileOffsetLow,
    DWORD dwNumberOfBytesToMap,
    LPVOID lpBaseAddress)
{
	NotImplementFeature("MapViewOfFileEx");
	return NULL;
}

inline BOOL WINAPI UnmapViewOfFile(
    LPCVOID lpBaseAddress)
{
	NotImplementFeature("UnmapViewOfFile");
	return TRUE;
}

// -------------------------------------------------------------------------

inline BOOL WINAPI CreateDirectoryA(
    LPCSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	NotImplementFeature("CreateDirectory");
	return TRUE;
}

inline BOOL WINAPI CreateDirectoryW(
    LPCWSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	NotImplementFeature("CreateDirectory");
	return TRUE;
}

#ifdef UNICODE
#define CreateDirectory  CreateDirectoryW
#else
#define CreateDirectory  CreateDirectoryA
#endif // !UNICODE

// -------------------------------------------------------------------------

inline BOOL WINAPI CloseHandle(
    HANDLE hObject)
{
	NotImplementFeature("CloseHandle");
	return TRUE;
}

#endif // STD_NO_WINSDK

// =========================================================================
// $Log: $

#endif /* STDEXT_WINAPI_WINBASE_H */
