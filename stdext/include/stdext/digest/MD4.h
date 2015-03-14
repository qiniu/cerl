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
// Module: stdext/digest/MD4.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: MD4.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DIGEST_MD4_H
#define STDEXT_DIGEST_MD4_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

namespace detail {

#include "md4/md4.h"

} // namespace detail

// -------------------------------------------------------------------------
// class MD4Checksum

class MD4Checksum
{
private:
	detail::MD4_CTX context;

public:
	MD4Checksum() {
		detail::MD4Init(&context);
	}

	void winx_call Update(const BYTE* buf, size_t bytes) {
		detail::MD4Update(&context, buf, (UINT)bytes);
	}

	void winx_call Final(BYTE checksum[16]) {
		detail::MD4Final(checksum, &context);
	}
};

// -------------------------------------------------------------------------
// function md4

inline void winx_call md4(const void* buf, size_t bytes, BYTE checksum[16])
{
	using namespace detail;

	MD4_CTX context;
	MD4Init(&context);
	MD4Update(&context, (const BYTE*)buf, (UINT)bytes);
	MD4Final(checksum, &context);
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_DIGEST_MD4_H */
