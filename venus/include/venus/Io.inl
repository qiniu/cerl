/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Io.inl
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Io.inl 2732 2010-04-29 09:01:30Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_IO_INL
#define VENUS_IO_INL

#ifndef VENUS_IO_H
#include "Io.h"
#endif

NS_CERL_IO_BEGIN

// -------------------------------------------------------------------------
// move from <venus/Copy.h>

template <class AllocT, class Type>
inline void cerl_call copy_aux_(
	AllocT& alloc, NS_STDEXT::Range<const Type*, Type>& dest,
			 const NS_STDEXT::Range<const Type*, Type>& src, TrueType pod)
{
	dest = NS_STDEXT::BasicString<Type>(alloc, src);
}

template <class AllocT, class Type>
inline void cerl_call copy_aux_(
	AllocT& alloc, NS_STDEXT::Range<const Type*, Type>& dest,
			 const NS_STDEXT::Range<const Type*, Type>& src, FalseType pod)
{
	size_t const n = src.size();
	Type* const arr = STD_NEW_ARRAY(alloc, Type, n);
	const Type* const arr0 = src.data();
	for (size_t i = 0; i < n; ++i)
		copy(alloc, arr[i], arr0[i]);
	dest.assign(arr, arr + n);
}

template <class AllocT, class Type>
inline void cerl_call copy(
	AllocT& alloc, NS_STDEXT::Range<const Type*, Type>& dest,
			 const NS_STDEXT::Range<const Type*, Type>& src)
{
	copy_aux_(alloc, dest, src, PodTraits<Type>::type());	
}

template <class AllocT, class Type, size_t m_size>
inline void cerl_call copy_aux_(
	AllocT& alloc, Array<Type, m_size>& dest,
			 const Array<Type, m_size>& src, TrueType pod)
{
	dest = src;
}

template <class AllocT, class Type, size_t m_size>
inline void cerl_call copy_aux_(
	AllocT& alloc, Array<Type, m_size>& dest,
			 const Array<Type, m_size>& src, FalseType pod)
{
	for (size_t i = 0; i < m_size; ++i)
		copy(alloc, dest[i], src[i]);
}

template <class AllocT, class Type, size_t m_size>
inline void cerl_call copy(
	AllocT& alloc, Array<Type, m_size>& dest,
			 const Array<Type, m_size>& src)
{
	copy_aux_(alloc, dest, src, PodTraits<Type>::type());
}

// -------------------------------------------------------------------------
// move from <venus/Dump.h>

template <class LogT, class DataIt>
inline void winx_call dumpArray(
	LogT& log, DataIt first, size_t count,
	const char* bracketL, const char* bracketR, const char* dataSep)
{
	dump(log, bracketL);
	if (count)
	{
		for (size_t i = 0;;)
		{
			dump(log, *first++);
			if (++i >= count)
				break;
			dump(log, dataSep);
		}
	}
	dump(log, bracketR);
}

template <class LogT, class Type>
inline void cerl_call dump(LogT& log, const NS_STDEXT::Range<const Type*, Type>& val)
{
	dumpArray(log, val.begin(), val.size());
}

template <class LogT, class Type, size_t m_size>
inline void cerl_call dump(LogT& log, const Array<Type, m_size>& val)
{
	dumpArray(log, val.begin(), val.size());
}

// -------------------------------------------------------------------------

NS_CERL_IO_END

#endif
