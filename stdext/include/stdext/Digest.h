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
// Module: stdext/Digest.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:48:49
// 
// $Id: Digest.h,v 1.3 2007/01/10 09:35:16 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DIGEST_H
#define STDEXT_DIGEST_H

#ifndef STDEXT_DIGEST_MD4_H
#include "digest/MD4.h"
#endif

#ifndef STDEXT_DIGEST_MD5_H
#include "digest/MD5.h"
#endif

#ifndef STDEXT_DIGEST_SHA1_H
#include "digest/SHA1.h"
#endif

#ifndef STDEXT_TEXT_BINARY_H
#include "text/Binary.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// md5str, md4str, sha1str

inline const char* winx_call md5str(char digest[33], const void* buf, size_t bytes)
{
	BYTE v[16];
	md5(buf, bytes, v);
	bin2hex(digest, v, 16);
	digest[32] = '\0';
	return digest;
}

inline const char* winx_call md4str(char digest[33], const void* buf, size_t bytes)
{
	BYTE v[16];
	md4(buf, bytes, v);
	bin2hex(digest, v, 16);
	digest[32] = '\0';
	return digest;
}

inline const char* winx_call sha1str(char digest[41], const void* buf, size_t bytes)
{
	BYTE v[20];
	sha1(buf, bytes, v);
	bin2hex(digest, v, 20);
	digest[40] = '\0';
	return digest;
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_DIGEST_H */
