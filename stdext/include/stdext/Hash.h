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
// Module: stdext/Hash.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Hash.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_HASH_H
#define STDEXT_HASH_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_STRING_H
#include "String.h" // class String, StringEqualTo, StringLess
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// hashOfString

template <class Iterator>
inline size_t winx_call hashOfString(Iterator it, Iterator itEnd)
{
	unsigned long h = 0;
	for (; it != itEnd; ++it)
		h = 5 * h + *it;
	return (size_t)h;
}

template <class Iterator>
inline size_t winx_call hashOfString(Iterator s)
{
	unsigned long h = 0; 
	for ( ; *s; ++s)
		h = 5*h + *s;
	return (size_t)h;
}

// -------------------------------------------------------------------------
// class Hash

template <class ValueT>
class Hash {
};

template<> class Hash<char*>
{
public:
	size_t winx_call operator()(const char* s) const { return hashOfString(s); }
};

template<> class Hash<const char*>
{
public:
	size_t winx_call operator()(const char* s) const { return hashOfString(s); }
};

template<> class Hash<char>
{
public:
	size_t winx_call operator()(char x) const { return x; }
};

template<> class Hash<unsigned char>
{
public:
	size_t winx_call operator()(unsigned char x) const { return x; }
};

template<> class Hash<signed char>
{
public:
	size_t winx_call operator()(unsigned char x) const { return x; }
};

template<> class Hash<short>
{
public:
	size_t winx_call operator()(short x) const { return x; }
};

template<> class Hash<unsigned short>
{
public:
	size_t winx_call operator()(unsigned short x) const { return x; }
};

template<> class Hash<int>
{
public:
	size_t winx_call operator()(int x) const { return x; }
};

template<> class Hash<unsigned int>
{
public:
	size_t winx_call operator()(unsigned int x) const { return x; }
};

template<> class Hash<long>
{
public:
	size_t winx_call operator()(long x) const { return x; }
};

template<> class Hash<unsigned long>
{
public:
	size_t winx_call operator()(unsigned long x) const { return x; }
};

// -------------------------------------------------------------------------

#if defined(WINX_NO_PARTIAL_SPECIALIZATION)

template <>
class Hash<std::string>
{
public:
	size_t winx_call operator()(const std::string& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <>
class Hash<std::basic_string<wchar_t> >
{
public:
	size_t winx_call operator()(const std::basic_string<wchar_t>& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <>
class Hash<String>
{
public:
	size_t winx_call operator()(const String& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <>
class Hash<WString>
{
public:
	size_t winx_call operator()(const WString& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

#else

template <class CharT, class Tr, class Ax>
class Hash<std::basic_string<CharT, Tr, Ax> >
{
public:
	size_t winx_call operator()(const std::basic_string<CharT, Tr, Ax>& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

template <class CharT>
class Hash<BasicString<CharT> >
{
public:
	size_t winx_call operator()(const NS_STDEXT::BasicString<CharT>& o) const {
		return hashOfString(o.begin(), o.end());
	}
};

#endif // !defined(WINX_NO_PARTIAL_SPECIALIZATION)

// -------------------------------------------------------------------------
// class HashCompare

template <class KeyT>
class HashCompare
{
public:
	typedef Hash<KeyT> hasher;
	typedef std::equal_to<KeyT> key_equal;
	typedef std::less<KeyT> key_pred;
};

template <>
class HashCompare<const char*>
{
public:
	typedef Hash<const char*> hasher;
	typedef StringEqualTo<char> key_equal;
	typedef StringLess<char> key_pred;
};

template <>
class HashCompare<const wchar_t*>
{
public:
	typedef Hash<const wchar_t*> hasher;
	typedef StringEqualTo<wchar_t> key_equal;
	typedef StringLess<wchar_t> key_pred;
};

template <>
class HashCompare<char*> : public HashCompare<const char*> {
};

template <>
class HashCompare<wchar_t*> : public HashCompare<const wchar_t*> {
};

// -------------------------------------------------------------------------
// struct HashtableAllocTraits

#if defined(X_CC_VC6)
#define WINX_HASH_TYPENAME_
#else
#define WINX_HASH_TYPENAME_	typename
#endif

template <class AllocT>
struct HashtableAllocTraits
{
	typedef AllocT alloc_type;

	template <class HashtableT>
	static alloc_type& winx_call getAlloc(HashtableT* h, AllocT& alloc) {
		return alloc;
	}
};

template <>
struct HashtableAllocTraits<Pools>
{
	typedef Pools::pool_type alloc_type;

	template <class HashtableT>
	static alloc_type& winx_call getAlloc(HashtableT* h, Pools& alloc) {
		return alloc.get_pool(HashtableT::node_size());
	}
};

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_HASH_H */
