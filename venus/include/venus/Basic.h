/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Basic.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Basic.h 2743 2010-05-05 08:21:38Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_BASIC_H
#define VENUS_BASIC_H

#ifndef ASYNC_IO_H
#include "../../../async/include/async/Io.h"
#endif

#ifndef ASYNC_NINFORMATION_H
#include "../../../async/include/async/NInformation.h"
#endif

// -------------------------------------------------------------------------
// Basic Types

NS_CERL_BEGIN

typedef char Char;
typedef bool Bool;
typedef UINT8 Byte;

typedef char Int8;
typedef INT16 Int16;
typedef INT32 Int32;
typedef INT64 Int64;

typedef UINT8 UInt8;
typedef UINT16 UInt16;
typedef UINT32 UInt32;
typedef UINT64 UInt64;

typedef NS_STDEXT::String String;
typedef NS_STDEXT::CString CString;

using NS_STDEXT::Range;
using NS_STDEXT::BasicArray;
using NS_STDEXT::BasicString;

using NS_STDEXT::g_str;
using NS_STDEXT::arrayof;

// -------------------------------------------------------------------------
// NoArgs

struct NoArgs {};

#define cerl_noargs			cerl::NoArgs()

// -------------------------------------------------------------------------
// Array Types

#define CERL_RANGE_(Type)	NS_STDEXT::Range<const Type*, Type>

typedef CERL_RANGE_(char) CharRange;
typedef CERL_RANGE_(bool) BoolRange;
typedef CERL_RANGE_(Byte) ByteRange;
typedef CERL_RANGE_(Int16) Int16Range;
typedef CERL_RANGE_(UInt16) UInt16Range;
typedef CERL_RANGE_(Int32) Int32Range;
typedef CERL_RANGE_(UInt32) UInt32Range;
typedef CERL_RANGE_(Int64) Int64Range;
typedef CERL_RANGE_(UInt64) UInt64Range;
typedef CERL_RANGE_(NInformation) NInformationRange;

// -------------------------------------------------------------------------
// CERL_PID_NULL, CERL_PID_MAIN

typedef UINT32 PID; // process id

#ifndef CERL_PID_MAIN
#define CERL_PID_MAIN			((cerl::PID)0)
#endif

#ifndef CERL_PID_NULL
#define CERL_PID_NULL			((cerl::PID)-1)
#endif

// -------------------------------------------------------------------------
// enum FID

#ifndef CERL_FID_FAC_SYSTEM
#define CERL_FID_FAC_SYSTEM		0x10000000
#endif

#ifndef CERL_FID_ASYNC_MASK
#define CERL_FID_ASYNC_MASK		0x80000000
#endif

#ifndef CERL_FID_RET_MASK
#define CERL_FID_RET_MASK		0x40000000
#endif

#ifndef CERL_FID_MASKS
#define CERL_FID_MASKS			(CERL_FID_ASYNC_MASK | CERL_FID_RET_MASK)
#endif

#ifndef CERL_FID_QUIT
#define CERL_FID_QUIT			((cerl::FID)(CERL_FID_FAC_SYSTEM | CERL_FID_ASYNC_MASK | 0xffff))
#endif

#ifndef CERL_FID_QUIT_REQ
#define CERL_FID_QUIT_REQ		((cerl::FID)(CERL_FID_FAC_SYSTEM | 0xffff))
#endif

#ifndef CERL_FID_DEBUG
#define CERL_FID_DEBUG			((cerl::FID)(CERL_FID_FAC_SYSTEM | 0xfffe))
#endif

#ifndef CERL_FID_GETPROCESS
#define CERL_FID_GETPROCESS		((cerl::FID)(CERL_FID_FAC_SYSTEM | 0xfffd))
#endif

#ifndef CERL_FID_NULL
#define CERL_FID_NULL			((cerl::FID)-1)
#endif

enum FID { FID_MIN = -1, FID_MAX = 2049672950 }; // function id

__forceinline int cerl_call is_sync_call(FID fid) {
	return !(CERL_FID_MASKS & fid);
}

__forceinline int cerl_call is_async_call(FID fid) {
	return CERL_FID_ASYNC_MASK & fid;
}

__forceinline int cerl_call is_ret(FID fid) {
	return CERL_FID_RET_MASK & fid;
}

// -------------------------------------------------------------------------
// macro CERL_DEBUG

#if defined(_DEBUG) && !defined(CERL_DEBUG)
//#define CERL_DEBUG
#endif

// -------------------------------------------------------------------------
// enum Code

typedef UINT32 Code;

enum { code_false = 0x0000 };
enum { code_true = 0x0001 };
enum { code_ok = 0x0002 };
enum { code_error = 0xffff };

//
// system error (用户不得使用这些错误码)
//
enum { code_unknown_error = 0xfff6 }; // 未知错误
enum { code_exists_already = 0xfff7 }; // 已经存在
enum { code_undefined = 0xfff8 }; // 未定义
enum { code_socket_error = 0xfff9 }; // Socket错
enum { code_encounter_exception = 0xfffa }; // 发生异常
enum { code_input_error = 0xfffb }; // 输入参数非法
enum { code_unknown_fid = 0xfffc }; // 未知功能ID（Function ID）
enum { code_format_error = 0xfffd }; // 网络协议（Mail）格式错
enum { code_timeout_error = 0xfffe }; // 超时

// -------------------------------------------------------------------------

NS_CERL_END

#ifndef VENUS_ARRAY_H
#include "Array.h"
#endif

#endif /*	VENUS_BASIC_H	*/
