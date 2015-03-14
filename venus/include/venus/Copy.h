/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Copy.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Copy.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef VENUS_COPY_H
#define VENUS_COPY_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

#ifndef NS_CERL_IO
#define NS_CERL_IO			NS_CERL::io
#define NS_CERL_IO_BEGIN	NS_CERL_BEGIN namespace io {
#define NS_CERL_IO_END		} NS_CERL_END
#endif

NS_CERL_IO_BEGIN

// =========================================================================
// copy Basic Types

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, bool& dest, bool src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, char& dest, char src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, Byte& dest, Byte src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, Int16& dest, Int16 src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, UInt16& dest, UInt16 src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, Int32& dest, Int32 src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, UInt32& dest, UInt32 src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, Int64& dest, const Int64& src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, UInt64& dest, const UInt64& src)
{
	dest = src;
}

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, NInformation& dest, const NInformation& src)
{
	dest = src;
}

template <class AllocT>
inline void winx_call copy(AllocT& alloc, NoArgs& dest, const NoArgs& src)
{
}

// =========================================================================
// PodTraits

struct TrueType { enum { value = TRUE }; };
struct FalseType { enum { value = FALSE }; };

typedef TrueType true_type;
typedef FalseType false_type;

template <class Type>
struct PodTraits { typedef void type; };

#define CERL_PODTYPE(Type, f)	\
	template <> struct PodTraits< Type > { typedef f##_type type; }

CERL_PODTYPE(char, true);
CERL_PODTYPE(bool, true);
CERL_PODTYPE(Byte, true);
CERL_PODTYPE(Int16, true);
CERL_PODTYPE(UInt16, true);
CERL_PODTYPE(Int32, true);
CERL_PODTYPE(UInt32, true);
CERL_PODTYPE(Int64, true);
CERL_PODTYPE(UInt64, true);
CERL_PODTYPE(NInformation, true);

CERL_PODTYPE(CharRange, false);
CERL_PODTYPE(BoolRange, false);
CERL_PODTYPE(ByteRange, false);
CERL_PODTYPE(Int16Range, false);
CERL_PODTYPE(UInt16Range, false);
CERL_PODTYPE(Int32Range, false);
CERL_PODTYPE(UInt32Range, false);
CERL_PODTYPE(Int64Range, false);
CERL_PODTYPE(UInt64Range, false);
CERL_PODTYPE(NInformationRange, false);

// =========================================================================
// copy Array Types

template <class AllocT>
inline void cerl_call copy(AllocT& alloc, String& dest, const String& src)
{
	dest.assign(alloc, src);
}

//
// compile error (gcc) fixed: move code to <venus/Io.inl>
//
template <class AllocT, class Type>
void cerl_call copy(
	AllocT& alloc, NS_STDEXT::Range<const Type*, Type>& dest,
			 const NS_STDEXT::Range<const Type*, Type>& src);

template <class AllocT, class Type, size_t m_size>
void cerl_call copy(
	AllocT& alloc, Array<Type, m_size>& dest,
			 const Array<Type, m_size>& src);

// =========================================================================

NS_CERL_IO_END

#endif /* VENUS_COPY_H */
