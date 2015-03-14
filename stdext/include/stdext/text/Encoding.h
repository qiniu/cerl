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
// Module: stdext/text/Encoding.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Encoding.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_ENCODING_H
#define STDEXT_TEXT_ENCODING_H

#ifndef STDEXT_TEXT_STRINGALGO_H
#include "StringAlgo.h"
#endif

#ifndef STD_MAP_H
#include "../../std/map.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class EncodingSet

namespace detail {

typedef String Encoding;

struct ICaseLess
{
	bool winx_call operator()(const Encoding& a, const Encoding& b) const {
		return a.icompare(b) < 0;
	}
};

typedef std::map<Encoding, int, ICaseLess> BaseEncoding2CPMap;

class Encoding2CPMap : public BaseEncoding2CPMap
{
public:
	struct Pair
	{
		const char* encoding;
		int cp;
	};

	Encoding2CPMap()
	{
		const Pair pairs[] =
		{
			{ "gbk", cp_gbk },
			{ "gb2312", cp_gbk },
			{ "utf-8", cp_utf8 },
			{ "utf8", cp_utf8 },
			{ "utf-7", cp_utf7 },
			{ "utf7", cp_utf7 },
			{ "big5", cp_big5 },
		};
		for (size_t i = 0; i < countof(pairs); ++i)
		{
			insert(value_type(pairs[i].encoding, pairs[i].cp));
		}
	}
};

} // namespace detail

// -------------------------------------------------------------------------
// function getEncoding

inline codepage_t winx_call getEncoding(const String& encoding, codepage_t cpError = cp_unknown)
{
	using namespace detail;

	static const Encoding2CPMap g_map = Encoding2CPMap();

	Encoding2CPMap::const_iterator it = g_map.find(encoding);
	if (it != g_map.end())
		return (codepage_t)(*it).second;
	else
		return cpError;
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_ENCODING_H */
