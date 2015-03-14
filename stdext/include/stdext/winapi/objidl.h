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
// Module: stdext/winapi/objidl.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-26 0:45:33
// 
// $Id: objidl.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_WINAPI_OBJIDL_H
#define STDEXT_WINAPI_OBJIDL_H

#ifndef STDEXT_WINAPI_WINDEF_H
#include "windef.h"
#endif

// =========================================================================

MIDL_INTERFACE_("00000000-0000-0000-C000-000000000046", IUnknown)
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
    
    virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;
    
    virtual ULONG STDMETHODCALLTYPE Release( void) = 0;
};

typedef IUnknown* LPUNKNOWN;

// -------------------------------------------------------------------------

MIDL_INTERFACE_("00000001-0000-0000-C000-000000000046", IClassFactory)
	: public IUnknown
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
			/* [unique][in] */ IUnknown __RPC_FAR *pUnkOuter,
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer( 
			/* [in] */ BOOL fLock) = 0;
};

// -------------------------------------------------------------------------

MIDL_INTERFACE_("00000002-0000-0000-C000-000000000046", IMalloc)
	: public IUnknown
{
public:
    virtual void __RPC_FAR *STDMETHODCALLTYPE Alloc( 
        /* [in] */ ULONG cb) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE Realloc( 
        /* [in] */ void __RPC_FAR *pv,
        /* [in] */ ULONG cb) = 0;
    
    virtual void STDMETHODCALLTYPE Free( 
        /* [in] */ void __RPC_FAR *pv) = 0;
    
    virtual ULONG STDMETHODCALLTYPE GetSize( 
        /* [in] */ void __RPC_FAR *pv) = 0;
    
    virtual int STDMETHODCALLTYPE DidAlloc( 
        void __RPC_FAR *pv) = 0;
    
    virtual void STDMETHODCALLTYPE HeapMinimize( void) = 0;
    
};

typedef /* [unique] */ IMalloc __RPC_FAR *LPMALLOC;

MIDL_INTERFACE_("0000001d-0000-0000-C000-000000000046", IMallocSpy)
	: public IUnknown
{
public:
    virtual ULONG STDMETHODCALLTYPE PreAlloc( 
        /* [in] */ ULONG cbRequest) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PostAlloc( 
        /* [in] */ void __RPC_FAR *pActual) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PreFree( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void STDMETHODCALLTYPE PostFree( 
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual ULONG STDMETHODCALLTYPE PreRealloc( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ ULONG cbRequest,
        /* [out] */ void __RPC_FAR *__RPC_FAR *ppNewRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PostRealloc( 
        /* [in] */ void __RPC_FAR *pActual,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PreGetSize( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual ULONG STDMETHODCALLTYPE PostGetSize( 
        /* [in] */ ULONG cbActual,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual void __RPC_FAR *STDMETHODCALLTYPE PreDidAlloc( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed) = 0;
    
    virtual int STDMETHODCALLTYPE PostDidAlloc( 
        /* [in] */ void __RPC_FAR *pRequest,
        /* [in] */ BOOL fSpyed,
        /* [in] */ int fActual) = 0;
    
    virtual void STDMETHODCALLTYPE PreHeapMinimize( void) = 0;
    
    virtual void STDMETHODCALLTYPE PostHeapMinimize( void) = 0;
    
};

typedef /* [unique] */ IMallocSpy __RPC_FAR *LPMALLOCSPY;

// -------------------------------------------------------------------------

#pragma pack(8)

typedef struct  tagSTATSTG
{
    LPOLESTR pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
}	STATSTG;

#pragma pack()

typedef /* [wire_marshal] */ OLECHAR __RPC_FAR *__RPC_FAR *SNB;

MIDL_INTERFACE_("0000000a-0000-0000-C000-000000000046", ILockBytes)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE ReadAt( 
        /* [in] */ ULARGE_INTEGER ulOffset,
        /* [length_is][size_is][out] */ void __RPC_FAR *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG __RPC_FAR *pcbRead) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE WriteAt( 
        /* [in] */ ULARGE_INTEGER ulOffset,
        /* [size_is][in] */ const void __RPC_FAR *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG __RPC_FAR *pcbWritten) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Flush( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetSize( 
        /* [in] */ ULARGE_INTEGER cb) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE LockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Stat( 
        /* [out] */ STATSTG __RPC_FAR *pstatstg,
        /* [in] */ DWORD grfStatFlag) = 0;
    
};

typedef /* [unique] */ ILockBytes __RPC_FAR *LPLOCKBYTES;


MIDL_INTERFACE_("0c733a30-2a1c-11ce-ade5-00aa0044773d", ISequentialStream)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
        /* [length_is][size_is][out] */ void __RPC_FAR *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG __RPC_FAR *pcbRead) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
        /* [size_is][in] */ const void __RPC_FAR *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG __RPC_FAR *pcbWritten) = 0;
    
};

typedef 
enum tagSTREAM_SEEK
    {	STREAM_SEEK_SET	= 0,
	STREAM_SEEK_CUR	= 1,
	STREAM_SEEK_END	= 2
    }	STREAM_SEEK;

typedef 
enum tagSTATFLAG
    {	STATFLAG_DEFAULT	= 0,
	STATFLAG_NONAME	= 1,
	STATFLAG_NOOPEN	= 2
    }	STATFLAG;

MIDL_INTERFACE_("0000000c-0000-0000-C000-000000000046", IStream)
	: public ISequentialStream
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
        /* [in] */ LARGE_INTEGER dlibMove,
        /* [in] */ DWORD dwOrigin,
        /* [out] */ ULARGE_INTEGER __RPC_FAR *plibNewPosition) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetSize( 
        /* [in] */ ULARGE_INTEGER libNewSize) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
        /* [unique][in] */ IStream __RPC_FAR *pstm,
        /* [in] */ ULARGE_INTEGER cb,
        /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbRead,
        /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbWritten) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Commit( 
        /* [in] */ DWORD grfCommitFlags) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Revert( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE LockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Stat( 
        /* [out] */ STATSTG __RPC_FAR *pstatstg,
        /* [in] */ DWORD grfStatFlag) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) = 0;
};

typedef IStream* LPSTREAM;

MIDL_INTERFACE_("0000000d-0000-0000-C000-000000000046", IEnumSTATSTG)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG celt,
        /* [length_is][size_is][out] */ STATSTG __RPC_FAR *rgelt,
        /* [out] */ ULONG __RPC_FAR *pceltFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG celt) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ IEnumSTATSTG __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
};

MIDL_INTERFACE_("0000000b-0000-0000-C000-000000000046", IStorage)
	: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE CreateStream( 
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsName,
        /* [in] */ DWORD grfMode,
        /* [in] */ DWORD reserved1,
        /* [in] */ DWORD reserved2,
        /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE OpenStream( 
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsName,
        /* [unique][in] */ void __RPC_FAR *reserved1,
        /* [in] */ DWORD grfMode,
        /* [in] */ DWORD reserved2,
        /* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CreateStorage( 
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsName,
        /* [in] */ DWORD grfMode,
        /* [in] */ DWORD reserved1,
        /* [in] */ DWORD reserved2,
        /* [out] */ IStorage __RPC_FAR *__RPC_FAR *ppstg) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE OpenStorage( 
        /* [string][unique][in] */ const OLECHAR __RPC_FAR *pwcsName,
        /* [unique][in] */ IStorage __RPC_FAR *pstgPriority,
        /* [in] */ DWORD grfMode,
        /* [unique][in] */ SNB snbExclude,
        /* [in] */ DWORD reserved,
        /* [out] */ IStorage __RPC_FAR *__RPC_FAR *ppstg) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CopyTo( 
        /* [in] */ DWORD ciidExclude,
        /* [size_is][unique][in] */ const IID __RPC_FAR *rgiidExclude,
        /* [unique][in] */ SNB snbExclude,
        /* [unique][in] */ IStorage __RPC_FAR *pstgDest) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE MoveElementTo( 
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsName,
        /* [unique][in] */ IStorage __RPC_FAR *pstgDest,
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsNewName,
        /* [in] */ DWORD grfFlags) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Commit( 
        /* [in] */ DWORD grfCommitFlags) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Revert( void) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE EnumElements( 
        /* [in] */ DWORD reserved1,
        /* [size_is][unique][in] */ void __RPC_FAR *reserved2,
        /* [in] */ DWORD reserved3,
        /* [out] */ IEnumSTATSTG __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DestroyElement( 
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsName) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE RenameElement( 
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsOldName,
        /* [string][in] */ const OLECHAR __RPC_FAR *pwcsNewName) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetElementTimes( 
        /* [string][unique][in] */ const OLECHAR __RPC_FAR *pwcsName,
        /* [unique][in] */ const FILETIME __RPC_FAR *pctime,
        /* [unique][in] */ const FILETIME __RPC_FAR *patime,
        /* [unique][in] */ const FILETIME __RPC_FAR *pmtime) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetClass( 
        /* [in] */ REFCLSID clsid) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetStateBits( 
        /* [in] */ DWORD grfStateBits,
        /* [in] */ DWORD grfMask) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Stat( 
        /* [out] */ STATSTG __RPC_FAR *pstatstg,
        /* [in] */ DWORD grfStatFlag) = 0;
    
};

// =========================================================================

interface IPersist;
interface IPersistStream;

MIDL_INTERFACE_("0000010c-0000-0000-C000-000000000046", IPersist)
	: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetClassID( 
        /* [out] */ CLSID __RPC_FAR *pClassID) = 0;
    
};

MIDL_INTERFACE_("00000109-0000-0000-C000-000000000046", IPersistStream)
	: public IPersist
{
public:
    virtual HRESULT STDMETHODCALLTYPE IsDirty( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Load( 
        /* [unique][in] */ IStream __RPC_FAR *pStm) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Save( 
        /* [unique][in] */ IStream __RPC_FAR *pStm,
        /* [in] */ BOOL fClearDirty) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
        /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbSize) = 0;
    
};

typedef /* [unique] */ IPersist __RPC_FAR *LPPERSIST;
typedef /* [unique] */ IPersistStream __RPC_FAR *LPPERSISTSTREAM;

// -------------------------------------------------------------------------

interface IEnumString;

MIDL_INTERFACE_("00000101-0000-0000-C000-000000000046", IEnumString)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG celt,
        /* [length_is][size_is][out] */ LPOLESTR __RPC_FAR *rgelt,
        /* [out] */ ULONG __RPC_FAR *pceltFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG celt) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ IEnumString __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
};

typedef /* [unique] */ IEnumString __RPC_FAR *LPENUMSTRING;

// =========================================================================

interface IMoniker;

interface IBindCtx;
interface IEnumMoniker;

MIDL_INTERFACE_("0000000f-0000-0000-C000-000000000046", IMoniker)
	: public IPersistStream
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE BindToObject( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkToLeft,
        /* [in] */ REFIID riidResult,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvResult) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE BindToStorage( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkToLeft,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObj) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reduce( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [in] */ DWORD dwReduceHowFar,
        /* [unique][out][in] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkToLeft,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkReduced) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE ComposeWith( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkRight,
        /* [in] */ BOOL fOnlyIfNotGeneric,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkComposite) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Enum( 
        /* [in] */ BOOL fForward,
        /* [out] */ IEnumMoniker __RPC_FAR *__RPC_FAR *ppenumMoniker) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE IsEqual( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkOtherMoniker) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Hash( 
        /* [out] */ DWORD __RPC_FAR *pdwHash) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE IsRunning( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkToLeft,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkNewlyRunning) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetTimeOfLastChange( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkToLeft,
        /* [out] */ FILETIME __RPC_FAR *pFileTime) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Inverse( 
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmk) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CommonPrefixWith( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkOther,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkPrefix) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE RelativePathTo( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkOther,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkRelPath) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetDisplayName( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkToLeft,
        /* [out] */ LPOLESTR __RPC_FAR *ppszDisplayName) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE ParseDisplayName( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkToLeft,
        /* [in] */ LPOLESTR pszDisplayName,
        /* [out] */ ULONG __RPC_FAR *pchEaten,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkOut) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE IsSystemMoniker( 
        /* [out] */ DWORD __RPC_FAR *pdwMksys) = 0;
    
};

typedef /* [unique] */ IMoniker __RPC_FAR *LPMONIKER;

// -------------------------------------------------------------------------

interface IEnumMoniker;

MIDL_INTERFACE_("00000102-0000-0000-C000-000000000046", IEnumMoniker)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG celt,
        /* [length_is][size_is][out] */ IMoniker __RPC_FAR *__RPC_FAR *rgelt,
        /* [out] */ ULONG __RPC_FAR *pceltFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG celt) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ IEnumMoniker __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
};

typedef /* [unique] */ IEnumMoniker __RPC_FAR *LPENUMMONIKER;

// -------------------------------------------------------------------------

interface IRunningObjectTable;

interface IMoniker;
interface IEnumMoniker;

MIDL_INTERFACE_("00000010-0000-0000-C000-000000000046", IRunningObjectTable)
	: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Register( 
        /* [in] */ DWORD grfFlags,
        /* [unique][in] */ IUnknown __RPC_FAR *punkObject,
        /* [unique][in] */ IMoniker __RPC_FAR *pmkObjectName,
        /* [out] */ DWORD __RPC_FAR *pdwRegister) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Revoke( 
        /* [in] */ DWORD dwRegister) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE IsRunning( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkObjectName) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetObject( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkObjectName,
        /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunkObject) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE NoteChangeTime( 
        /* [in] */ DWORD dwRegister,
        /* [in] */ FILETIME __RPC_FAR *pfiletime) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetTimeOfLastChange( 
        /* [unique][in] */ IMoniker __RPC_FAR *pmkObjectName,
        /* [out] */ FILETIME __RPC_FAR *pfiletime) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EnumRunning( 
        /* [out] */ IEnumMoniker __RPC_FAR *__RPC_FAR *ppenumMoniker) = 0;
    
};

typedef /* [unique] */ IRunningObjectTable __RPC_FAR *LPRUNNINGOBJECTTABLE;

// -------------------------------------------------------------------------

interface IBindCtx;

#pragma pack(8)

typedef struct  tagBIND_OPTS
{
    DWORD cbStruct;
    DWORD grfFlags;
    DWORD grfMode;
    DWORD dwTickCountDeadline;
}	BIND_OPTS;

#pragma pack()

typedef struct tagBIND_OPTS __RPC_FAR *LPBIND_OPTS;

interface IRunningObjectTable;

MIDL_INTERFACE_("0000000e-0000-0000-C000-000000000046", IBindCtx)
	: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE RegisterObjectBound( 
        /* [unique][in] */ IUnknown __RPC_FAR *punk) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE RevokeObjectBound( 
        /* [unique][in] */ IUnknown __RPC_FAR *punk) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE ReleaseBoundObjects( void) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetBindOptions( 
        /* [in] */ BIND_OPTS __RPC_FAR *pbindopts) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetBindOptions( 
        /* [out][in] */ BIND_OPTS __RPC_FAR *pbindopts) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetRunningObjectTable( 
        /* [out] */ IRunningObjectTable __RPC_FAR *__RPC_FAR *pprot) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE RegisterObjectParam( 
        /* [in] */ LPOLESTR pszKey,
        /* [unique][in] */ IUnknown __RPC_FAR *punk) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetObjectParam( 
        /* [in] */ LPOLESTR pszKey,
        /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppunk) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EnumObjectParam( 
        /* [out] */ IEnumString __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE RevokeObjectParam( 
        /* [in] */ LPOLESTR pszKey) = 0;
    
};

typedef /* [unique] */ IBindCtx __RPC_FAR *LPBC;
typedef /* [unique] */ IBindCtx __RPC_FAR *LPBINDCTX;

// -------------------------------------------------------------------------

interface IAdviseSink;

#pragma pack(8)

typedef /* [wire_marshal] */ WORD CLIPFORMAT;

typedef CLIPFORMAT __RPC_FAR *LPCLIPFORMAT;

typedef struct  tagDVTARGETDEVICE
{
    DWORD tdSize;
    WORD tdDriverNameOffset;
    WORD tdDeviceNameOffset;
    WORD tdPortNameOffset;
    WORD tdExtDevmodeOffset;
    /* [size_is] */ BYTE tdData[ 1 ];
}	DVTARGETDEVICE;

typedef struct  tagFORMATETC
{
    CLIPFORMAT cfFormat;
    /* [unique] */ DVTARGETDEVICE __RPC_FAR *ptd;
    DWORD dwAspect;
    LONG lindex;
    DWORD tymed;
}	FORMATETC;

typedef struct tagFORMATETC __RPC_FAR *LPFORMATETC;

typedef struct  tagSTGMEDIUM
{
    DWORD tymed;
    /* [switch_is][switch_type] */ union 
	{
        /* [case()] */ HBITMAP hBitmap;
        /* [case()] */ HMETAFILEPICT hMetaFilePict;
        /* [case()] */ HENHMETAFILE hEnhMetaFile;
        /* [case()] */ HGLOBAL hGlobal;
        /* [case()] */ LPOLESTR lpszFileName;
        /* [case()] */ IStream __RPC_FAR *pstm;
        /* [case()] */ IStorage __RPC_FAR *pstg;
        /* [default] */  /* Empty union arm */ 
	}	;
    /* [unique] */ IUnknown __RPC_FAR *pUnkForRelease;
}	uSTGMEDIUM;

typedef /* [wire_marshal] */ uSTGMEDIUM STGMEDIUM;

typedef STGMEDIUM __RPC_FAR *LPSTGMEDIUM;

#pragma pack()

interface IMoniker;

MIDL_INTERFACE_("0000010f-0000-0000-C000-000000000046", IAdviseSink)
	: public IUnknown
{
public:
    virtual /* [local] */ void STDMETHODCALLTYPE OnDataChange( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pFormatetc,
        /* [unique][in] */ STGMEDIUM __RPC_FAR *pStgmed) = 0;
    
    virtual /* [local] */ void STDMETHODCALLTYPE OnViewChange( 
        /* [in] */ DWORD dwAspect,
        /* [in] */ LONG lindex) = 0;
    
    virtual /* [local] */ void STDMETHODCALLTYPE OnRename( 
        /* [in] */ IMoniker __RPC_FAR *pmk) = 0;
    
    virtual /* [local] */ void STDMETHODCALLTYPE OnSave( void) = 0;
    
    virtual /* [local] */ void STDMETHODCALLTYPE OnClose( void) = 0;
    
};

// -------------------------------------------------------------------------

interface IEnumSTATDATA;

typedef /* [unique] */ IEnumSTATDATA __RPC_FAR *LPENUMSTATDATA;

typedef 
enum tagADVF
    {	ADVF_NODATA	= 1,
	ADVF_PRIMEFIRST	= 2,
	ADVF_ONLYONCE	= 4,
	ADVF_DATAONSTOP	= 64,
	ADVFCACHE_NOHANDLER	= 8,
	ADVFCACHE_FORCEBUILTIN	= 16,
	ADVFCACHE_ONSAVE	= 32
    }	ADVF;

#pragma pack(8)

typedef struct  tagSTATDATA
    {
    FORMATETC formatetc;
    DWORD advf;
    /* [unique] */ IAdviseSink __RPC_FAR *pAdvSink;
    DWORD dwConnection;
    }	STATDATA;

typedef STATDATA __RPC_FAR *LPSTATDATA;

#pragma pack()

MIDL_INTERFACE_("00000105-0000-0000-C000-000000000046", IEnumSTATDATA)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG celt,
        /* [length_is][size_is][out] */ STATDATA __RPC_FAR *rgelt,
        /* [out] */ ULONG __RPC_FAR *pceltFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG celt) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ IEnumSTATDATA __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
};

// -------------------------------------------------------------------------

interface IEnumFORMATETC;

MIDL_INTERFACE_("00000103-0000-0000-C000-000000000046", IEnumFORMATETC)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
        /* [in] */ ULONG celt,
        /* [length_is][size_is][out] */ FORMATETC __RPC_FAR *rgelt,
        /* [out] */ ULONG __RPC_FAR *pceltFetched) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Skip( 
        /* [in] */ ULONG celt) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenum) = 0;
    
};

// -------------------------------------------------------------------------

interface IDataObject;

MIDL_INTERFACE_("0000010e-0000-0000-C000-000000000046", IDataObject)
	: public IUnknown
{
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetData( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetcIn,
        /* [out] */ STGMEDIUM __RPC_FAR *pmedium) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetDataHere( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [out][in] */ STGMEDIUM __RPC_FAR *pmedium) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE QueryGetData( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetc) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatectIn,
        /* [out] */ FORMATETC __RPC_FAR *pformatetcOut) = 0;
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE SetData( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [unique][in] */ STGMEDIUM __RPC_FAR *pmedium,
        /* [in] */ BOOL fRelease) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EnumFormatEtc( 
        /* [in] */ DWORD dwDirection,
        /* [out] */ IEnumFORMATETC __RPC_FAR *__RPC_FAR *ppenumFormatEtc) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DAdvise( 
        /* [in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [in] */ DWORD advf,
        /* [unique][in] */ IAdviseSink __RPC_FAR *pAdvSink,
        /* [out] */ DWORD __RPC_FAR *pdwConnection) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DUnadvise( 
        /* [in] */ DWORD dwConnection) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EnumDAdvise( 
        /* [out] */ IEnumSTATDATA __RPC_FAR *__RPC_FAR *ppenumAdvise) = 0;
    
};

typedef /* [unique] */ IDataObject __RPC_FAR *LPDATAOBJECT;

// =========================================================================

interface IStdMarshalInfo;

typedef /* [unique] */ IStdMarshalInfo __RPC_FAR *LPSTDMARSHALINFO;

MIDL_INTERFACE_("00000018-0000-0000-C000-000000000046", IStdMarshalInfo)
	: public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetClassForHandler( 
        /* [in] */ DWORD dwDestContext,
        /* [unique][in] */ void __RPC_FAR *pvDestContext,
        /* [out] */ CLSID __RPC_FAR *pClsid) = 0;
    
};

// =========================================================================

interface IExternalConnection;

typedef /* [unique] */ IExternalConnection __RPC_FAR *LPEXTERNALCONNECTION;

typedef 
enum tagEXTCONN
    {	EXTCONN_STRONG	= 0x1,
	EXTCONN_WEAK	= 0x2,
	EXTCONN_CALLABLE	= 0x4
    }	EXTCONN;

MIDL_INTERFACE_("00000019-0000-0000-C000-000000000046", IExternalConnection)
	: public IUnknown
{
public:
    virtual DWORD STDMETHODCALLTYPE AddConnection( 
        /* [in] */ DWORD extconn,
        /* [in] */ DWORD reserved) = 0;
    
    virtual DWORD STDMETHODCALLTYPE ReleaseConnection( 
        /* [in] */ DWORD extconn,
        /* [in] */ DWORD reserved,
        /* [in] */ BOOL fLastReleaseCloses) = 0;
    
};

// =========================================================================
// $Log: objidl.h,v $
//

#endif /* STDEXT_WINAPI_OBJIDL_H */
