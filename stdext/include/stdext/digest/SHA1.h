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
// Module: stdext/digest/SHA1.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: SHA1.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_DIGEST_SHA1_H
#define STDEXT_DIGEST_SHA1_H

#ifndef STDEXT_BASIC_H
#include "../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

namespace detail {

#include "sha1/sha1.h"
#include "sha1/sha1.inl"

} // namespace detail

class SHA1Checksum
{
private:
	detail::SHA1 m_impl;

public:
	void winx_call Update(const BYTE* buf, size_t bytes) {
		m_impl.Input(buf, (UINT)bytes);
	}
	
	void winx_call Reset()
	{
		m_impl.Reset();
	}

#if defined(WINX_BYTESWAP)
	void winx_call Final(BYTE checksum[20])
	{
		m_impl.Result( (detail::UINT4*)checksum );
	}
#else
	void winx_call Final(BYTE* checksum)
	{
		detail::UINT4 key[5];
		m_impl.Result( key );
		for (size_t i = 0; i < 5; ++i)
		{
			*checksum++ = (BYTE)(key[i] >> 24);
			*checksum++ = (BYTE)(key[i] >> 16);
			*checksum++ = (BYTE)(key[i] >> 8);
			*checksum++ = (BYTE)key[i];
		}
	}
#endif
};

// -------------------------------------------------------------------------
// function sha1

inline void winx_call sha1(const void* buf, size_t bytes, BYTE checksum[20])
{
	SHA1Checksum chksum;
	chksum.Update( (const BYTE*)buf, (UINT)bytes );
	chksum.Final( checksum );
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_DIGEST_SHA1_H */
