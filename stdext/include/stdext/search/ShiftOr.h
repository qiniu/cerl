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
// Module: stdext/search/ShiftOr.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-8-8 20:30:40
// 
// $Id: ShiftOr.h 2006/12/26 10:54:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_SEARCH_SHIFTOR_H
#define STDEXT_SEARCH_SHIFTOR_H

#ifndef STD_VECTOR_H
#include "../../std/vector.h"
#endif

#ifndef STDEXT_SEARCH_BASIC_H
#include "Basic.h"
#endif

#ifndef NS_SHIFTOR
#define NS_SHIFTOR			NS_STDEXT::shift_or
#define NS_SHIFTOR_BEGIN	NS_STDEXT_BEGIN namespace shift_or {
#define NS_SHIFTOR_END		} NS_STDEXT_END
#endif

NS_SHIFTOR_BEGIN

// -------------------------------------------------------------------------
// struct BitmapTraits

template <class UIntT>
struct BitmapTraits
{
	enum { MaxSize = sizeof(UIntT) * 8 };
	
	typedef UIntT TestParamType;

	static void winx_call initTrue(UIntT& bitmap, size_t n) {
		bitmap = ((UIntT)1 << n) - 1;
	}

	static TestParamType winx_call getTestParam(size_t n) {
		return (UIntT)1 << (n - 1);
	}
	
	static void winx_call clear(UIntT& bitmap, size_t i) {
		bitmap &= ~((UIntT)1 << i);
	}

	static void winx_call leftShift(UIntT& bitmap) {
		bitmap <<= 1;
	}
	
	static void winx_call binaryOr(UIntT& bitmap, const UIntT& mask) {
		bitmap |= mask;
	}
	
	static UIntT winx_call testTrue(const UIntT& bitmap, const TestParamType& mask) {
		return bitmap & mask;
	}
};

typedef std::vector<char> SimpleVariantBitmap;
typedef SimpleVariantBitmap VariantBitmap;

template <>
struct BitmapTraits<SimpleVariantBitmap>
{
	typedef SimpleVariantBitmap BitmapT;
	
	enum { MaxSize = -1 };
	
	typedef size_t TestParamType;
	
	static void winx_call initTrue(BitmapT& bitmap, size_t n) {
		bitmap.assign(n, 1);
	}
	
	static TestParamType winx_call getTestParam(size_t n) {
		return n - 1;
	}

	static void winx_call clear(BitmapT& bitmap, size_t i) {
		bitmap[i] = 0;
	}
	
	static void winx_call leftShift(BitmapT& bitmap) {
		bitmap.pop_back();
		bitmap.insert(bitmap.begin(), 0);
	}
	
	static void winx_call binaryOr(BitmapT& bitmap, const BitmapT& mask) {
		WINX_ASSERT(bitmap.size() == mask.size());
		BitmapT::const_iterator it2 = mask.begin();
		for (BitmapT::iterator it = bitmap.begin(); it != bitmap.end(); ++it) {
			*it |= *it2;
			++it2;
		}
	}
	
	static BitmapT::value_type winx_call testTrue(const BitmapT& bitmap, const TestParamType i) {
		return bitmap[i];
	}
};

// -------------------------------------------------------------------------
// class Finder

template <class CharT, class BitmapT = VariantBitmap, class Strategy = MatchCase<CharT> >
class Finder
{
public:
	typedef CharT char_type;
	typedef typename ArchiveCharTraits<CharT>::uchar_type uchar_type;
	typedef size_t size_type;

	enum { npos = -1 };

private:
	typedef BitmapTraits<BitmapT> Tr;
	
	enum { CharCount = 1 << (sizeof(CharT) * 8) };
	
	BitmapT m_bitmap[CharCount];
	size_t m_size;

	Finder(const Finder&);
	void operator=(const Finder&);

public:
	Finder()
		: m_size(0)
	{
	}
	
	Finder(const char_type* szPattern, size_type cchLen)
	{
		initPattern(szPattern, szPattern + cchLen);
	}

	Finder(const char_type* szPattern)
	{
		initPattern(szPattern);
	}

	template <class Tr2, class AllocT2>
	Finder(const std::basic_string<CharT, Tr2, AllocT2>& strPattern)
	{
		initPattern(strPattern.begin(), strPattern.end());
	}

	template <class Iterator>
	Finder(Iterator patternBegin, Iterator patternEnd)
	{
		initPattern(patternBegin, patternEnd);
	}

public:
	template <class Iterator>
	HRESULT winx_call initPattern(Iterator it, Iterator itEnd)
	{
		if (it == itEnd)
			return E_INVALIDARG;
		
		m_size = std::distance(it, itEnd);
		WINX_ASSERT(m_size <= (size_t)Tr::MaxSize);
		
		if (Tr::MaxSize >= 0) {
			if (m_size > (size_t)Tr::MaxSize)
				return E_INVALIDARG; // OUT_OF_RANGE
		}
		
		for (int i = 0; i < CharCount; ++i) {
			Tr::initTrue(m_bitmap[i], m_size);
		}
		
		size_t n = 0;
		for (; it != itEnd; ++it) {
			const size_t i = Strategy::transform((uchar_type)*it);
			Tr::clear(m_bitmap[i], n++);
		}
		return S_OK;
	}

	HRESULT winx_call initPattern(const char_type* szPattern)
	{
		if (szPattern == NULL)
			return E_INVALIDARG;
		return initPattern(szPattern, szPattern + std::char_traits<CharT>::length(szPattern));
	}

	template <class Tr2, class AllocT2>
	HRESULT winx_call initPattern(const std::basic_string<CharT, Tr2, AllocT2>& strPattern)
	{
		return initPattern(strPattern.begin(), strPattern.end());
	}

	bool winx_call good() const
	{
		return m_size != 0;
	}

	size_type winx_call size() const
	{
		return m_size;
	}

public:
	template <class ArchiveT>
	HRESULT winx_call next(ArchiveT& ar) const
	{
		if (m_size == 0)
			return E_ACCESSDENIED;

		BitmapT d;
		typename Tr::TestParamType para = Tr::getTestParam(m_size);
		Tr::initTrue(d, m_size);
		do
		{
			const typename ArchiveT::int_type ch_ = Strategy::get(ar);
			if (ch_ == ArchiveT::endch)
				return S_FALSE;

			Tr::leftShift(d);
			Tr::binaryOr(d, m_bitmap[ch_]);
		}
		while (Tr::testTrue(d, para));
		
		return S_OK;
	}

	template <class ArchiveT>
	HRESULT winx_call next(ArchiveT& ar, size_type limit) const
	{
		limited_archive<ArchiveT> arLim(ar, limit);
		return next(arLim, limit);
	};

	template <class istream_type>
	HRESULT winx_call istreamNext(istream_type& is) const
	{
		return next( *(istream2archive<istream_type>*)&is );
	}

	template <class istream_type>
	HRESULT winx_call istreamNext(istream_type& is, size_t limit) const
	{
		return next( *(istream2archive<istream_type>*)&is, limit );
	}

	template <class iterator_type>
	HRESULT winx_call iteratorNext(iterator_type it, iterator_type itEnd, iterator_type* pitFind) const
	{
		iterator2archive<iterator_type> is( it, itEnd );
		const HRESULT hr = next( is );
		*pitFind = is.tell();
		return hr;
	}

	HRESULT winx_call cstrNext(const char_type* text, const char_type** ppFind) const
	{
		cstr2archive<char_type> is( text );
		const HRESULT hr = next( is );
		*ppFind = is.tell();
		return hr;
	}
};

template <class CharT, class BitmapT = VariantBitmap>
class NoCaseFinder : public Finder<CharT, BitmapT, MatchNoCase<CharT> >
{
private:
	typedef Finder<CharT, BitmapT, MatchNoCase<CharT> > Base;

public:
    typedef typename Base::size_type size_type;
    typedef typename Base::char_type char_type;
    
	NoCaseFinder() : Base() {}
	
	NoCaseFinder(const char_type* szPattern, size_type cchLen) : Base(szPattern, cchLen) {}
	
	NoCaseFinder(const char_type* szPattern) : Base(szPattern) {}

	template <class Tr2, class AllocT2>
	NoCaseFinder(const std::basic_string<CharT, Tr2, AllocT2>& strPattern) : Base(strPattern) {}
};

// -------------------------------------------------------------------------
// $Log: ShiftOr.h,v $

NS_SHIFTOR_END

#endif /* STDEXT_SEARCH_SHIFTOR_H */
