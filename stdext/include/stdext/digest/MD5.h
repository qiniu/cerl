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
// Module: stdext/digest/MD5.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: MD5.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DIGEST_MD5_H
#define STDEXT_DIGEST_MD5_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

namespace detail {

#include "md5/MD5Checksum.h"
#include "md5/MD5ChecksumDefines.h"
#include "md5/MD5ChecksumImpl.h"

} // namespace detail

typedef detail::CMD5Checksum MD5Checksum;

// -------------------------------------------------------------------------
// function md5

inline void winx_call md5(const void* buf, size_t bytes, BYTE checksum[16])
{
	MD5Checksum chksum;
	chksum.Update( (const BYTE*)buf, (UINT)bytes );
	chksum.Final( checksum );
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_DIGEST_MD5_H */
