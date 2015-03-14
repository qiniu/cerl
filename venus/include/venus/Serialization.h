/* -------------------------------------------------------------------------
// Venus: A High Performance async server framework
// 
// Module: venus/Serialization.h
// Creator: Xihe Yu
// Email: krzycube@gmail.com
// Date: 2009-12-08 19:41:58
// 
// $Id: Serialization.h 2732 2010-04-29 09:01:30Z lijie $
// -----------------------------------------------------------------------*/
#ifndef VENUS_SERIALIZATION_H
#define VENUS_SERIALIZATION_H

#ifndef VENUS_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_H
#include "../../../stdext/include/stdext/Archive.h"
#endif

#ifndef NS_CERL_IO
#define NS_CERL_IO			NS_CERL::io
#define NS_CERL_IO_BEGIN	NS_CERL_BEGIN namespace io {
#define NS_CERL_IO_END		} NS_CERL_END
#endif

NS_CERL_IO_BEGIN

// =========================================================================
// type id

template <class Type>
struct TypeIdTraits {
};

#define CERL_DEFINE_TYPEID(Type, typeId)									\
NS_CERL_IO_BEGIN															\
	template <>																\
	struct TypeIdTraits<Type> {												\
		enum { id = typeId };												\
	};																		\
NS_CERL_IO_END

#define CERL_TYPEID(Type)													\
	((UInt16)NS_CERL_IO::TypeIdTraits<Type>::id)

// =========================================================================
// io type

enum IoType
{
	vtInvalid		= -1,
	vtNull			= 0x0000,
	vtI1			= 0x1000, // char, bool, INT8, UINT8
	vtI2			= 0x2000, // INT16, UINT16
	vtI4			= 0x3000, // INT32, UINT32
	vtI8			= 0x4000, // INT64, UINT64, NInformation
	vtR8			= 0x5000, // double
	vtR4			= 0x6000, // float
	vtUserType		= 0x7000,
	vtReserved0		= 0x8000,
	vtReserved1		= 0x9000,
	vtReserved2		= 0xA000,
	vtReserved3		= 0xB000,
	vtReserved4		= 0xC000,
	vtReserved5		= 0xD000,
	vtReserved6		= 0xE000,
	vtReserved7		= 0xF000,
	vtArrayMask		= 0x0800,
};

enum IoTypeMask
{
	ioTypeMask	= 0xf800,
	ioSizeMask	= 0x07ff,
};

template <class Type>
struct IoTypeTraits_
{
	enum { vt = vtUserType };
};

#define CERL_DEFINE_IOTYPE(Type, vtType)									\
	template <>																\
	struct IoTypeTraits_<Type> {											\
		enum { vt = vtType };												\
	}

CERL_DEFINE_IOTYPE(bool, vtI1);
CERL_DEFINE_IOTYPE(char, vtI1);
CERL_DEFINE_IOTYPE(Byte, vtI1);
CERL_DEFINE_IOTYPE(Int16, vtI2);
CERL_DEFINE_IOTYPE(UInt16, vtI2);
CERL_DEFINE_IOTYPE(Int32, vtI4);
CERL_DEFINE_IOTYPE(UInt32, vtI4);
CERL_DEFINE_IOTYPE(Int64, vtI8);
CERL_DEFINE_IOTYPE(UInt64, vtI8);
CERL_DEFINE_IOTYPE(NInformation, vtI8);

template <class Type>
struct IoTypeTraits : public IoTypeTraits_<Type>
{
	enum { vt = IoTypeTraits_<Type>::vt };
	enum { vtArray = vt | vtArrayMask };

	template <class OutputStreamT>
	static __forceinline void cerl_call putType(OutputStreamT& os) {
		NS_STDEXT_IO_BINARY::put16i(os, (UInt16)vt);
	}

	template <class OutputStreamT>
	static __forceinline void cerl_call putUserType(OutputStreamT& os, UInt16 typeId) {
		CERL_ASSERT(typeId <= ioSizeMask);
		NS_STDEXT_IO_BINARY::put16i(os, (UInt16)(vtUserType | (ioSizeMask & typeId)));
	}

	template <class InputStreamT>
	static __forceinline bool cerl_call getType(InputStreamT& is) {
		UInt16 vtType;
		const bool fOk = NS_STDEXT_IO_BINARY::get16i(is, vtType) && vt == vtType;
		return fOk;
	}

	template <class InputStreamT>
	static __forceinline bool cerl_call getType(InputStreamT& is, UInt16& vtType) {
		return NS_STDEXT_IO_BINARY::get16i(is, vtType);
	}

	template <class InputStreamT>
	static __forceinline bool cerl_call getUserType(InputStreamT& is, UInt16 typeId) {
		CERL_ASSERT(typeId <= ioSizeMask);
		UInt16 vtType;
		return NS_STDEXT_IO_BINARY::get16i(is, vtType) && (vtUserType | (ioSizeMask & typeId)) == vtType;
	}

	template <class OutputStreamT>
	static __forceinline void cerl_call putArraySize(OutputStreamT& os, UInt32 count)
	{
		if (ioSizeMask > count)
		{
			NS_STDEXT_IO_BINARY::put16i(os, (UInt16)(vtArray | count));
		}
		else
		{
			NS_STDEXT_IO_BINARY::put16i(os, (UInt16)(vtArray | ioSizeMask));
			NS_STDEXT_IO_BINARY::put32i(os, count);
		}
	}

	template <class InputStreamT>
	static __forceinline bool cerl_call getArraySize(InputStreamT& is, UInt32& count)
	{
		UInt16 vtType;
		if (!NS_STDEXT_IO_BINARY::get16i(is, vtType))
			return false;
		if (vtType == (vtArray | ioSizeMask))
			return NS_STDEXT_IO_BINARY::get32i(is, count);

		count = (UInt32)(ioSizeMask & vtType);
		return true;
	}
};

template <class InputStreamT>
__forceinline bool cerl_call getArraySize(InputStreamT& is, UInt16 vtType, UInt32& count)
{
	if ((vtType & ioSizeMask) == ioSizeMask)
		return NS_STDEXT_IO_BINARY::get32i(is, count);

	count = (UInt32)(ioSizeMask & vtType);
	return true;
}

// =========================================================================
// put element

template <class Type>
struct Putter
{
};

/*
template <>
struct Putter<Type>
{
	template <class OutputStreamT>
	static __forceinline void cerl_call call(OutputStreamT& os, const Type& val) {
		// ...
	}
};
*/
#define CERL_IO_BEGIN_PUTTER(Type) 														\
		template <> 																	\
		struct Putter<Type> 															\
		{																				\
			template <class OutputStreamT>												\
			static __forceinline void cerl_call call(OutputStreamT& os, const Type& val) {
	
#define CERL_IO_END_PUTTER()	}};



template <class Type>
struct Getter
{
};

/*
template <>
struct Getter<Type>
{
	template <class InputStreamT>
	static __forceinline bool cerl_call call(InputStreamT& os, Type& val) {
		// ...
	}
};
*/
#define CERL_IO_BEGIN_GETTER(Type) 													\
	template <>																		\
	struct Getter<Type>																\
	{																				\
		template <class InputStreamT>												\
		static __forceinline bool cerl_call call(InputStreamT& is, Type& val) {

#define CERL_IO_END_GETTER()	}};



template <class OutputStreamT, class Type>
__forceinline void cerl_call put(OutputStreamT& os, const Type& val) {
	Putter<Type>::call(os, val);
}

template <class InputStreamT, class Type>
__forceinline bool cerl_call get(InputStreamT& is, Type& val) {
	return Getter<Type>::call(is, val);
}


CERL_IO_BEGIN_PUTTER(char)
	IoTypeTraits<char>::putType(os);
	os.put((const char*)&val, 1);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Byte)
	IoTypeTraits<Byte>::putType(os);
	os.put((const char*)&val, 1);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Bool)
	IoTypeTraits<Bool>::putType(os);
	os.put((const char*)&val, 1);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Int16)
	IoTypeTraits<Int16>::putType(os);
	NS_STDEXT_IO_BINARY::put16i(os, val);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(UInt16)
	IoTypeTraits<UInt16>::putType(os);
	NS_STDEXT_IO_BINARY::put16i(os, val);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Int32)
	IoTypeTraits<Int32>::putType(os);
	NS_STDEXT_IO_BINARY::put32i(os, val);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(UInt32)
	IoTypeTraits<UInt32>::putType(os);
	NS_STDEXT_IO_BINARY::put32i(os, val);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Int64)
	IoTypeTraits<Int64>::putType(os);
	NS_STDEXT_IO_BINARY::put64i(os, val);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(UInt64)
	IoTypeTraits<UInt64>::putType(os);
	NS_STDEXT_IO_BINARY::put64i(os, val);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(NInformation)
	IoTypeTraits<NInformation>::putType(os);
	NS_STDEXT_IO_BINARY::put64i(os, val.val);
CERL_IO_END_PUTTER()

// -------------------------------------------------------------------------
// get element

CERL_IO_BEGIN_GETTER(char)
	return IoTypeTraits<char>::getType(is) && is.get(&val, 1);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Byte)
	return IoTypeTraits<Byte>::getType(is) && is.get((char*)&val, 1);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Bool)
	return IoTypeTraits<Bool>::getType(is) && is.get((char*)&val, 1);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Int16)
	return IoTypeTraits<Int16>::getType(is) && NS_STDEXT_IO_BINARY::get16i(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(UInt16)
	return IoTypeTraits<UInt16>::getType(is) && NS_STDEXT_IO_BINARY::get16i(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Int32)
	return IoTypeTraits<Int32>::getType(is) && NS_STDEXT_IO_BINARY::get32i(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(UInt32)
	return IoTypeTraits<UInt32>::getType(is) && NS_STDEXT_IO_BINARY::get32i(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Int64)
	return IoTypeTraits<Int64>::getType(is) && NS_STDEXT_IO_BINARY::get64i(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(UInt64)
	return IoTypeTraits<UInt64>::getType(is) && NS_STDEXT_IO_BINARY::get64i(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(NInformation)
	return IoTypeTraits<NInformation>::getType(is) && NS_STDEXT_IO_BINARY::get64i(is, val.val);
CERL_IO_END_GETTER()

// -------------------------------------------------------------------------
// put array

template <class OutputStreamT>
inline void cerl_call putString(OutputStreamT& os, const char* s, UInt32 count)
{
	IoTypeTraits<char>::putArraySize(os, count);
	os.put(s, count);
}

CERL_IO_BEGIN_PUTTER(std::string)
	putString(os, std::_ConvIt(val.begin()), val.length());
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(CharRange)
	putString(os, val.begin(), val.size());
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(ByteRange)
	putString(os, (const char*)val.begin(), val.size());
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(String)
	putString(os, val.begin(), val.size());
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(UInt16Range)
	const UInt32 count = val.size();
	IoTypeTraits<UInt16>::putArraySize(os, count);
	NS_STDEXT_IO_BINARY::put16i(os, val.begin(), count);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Int32Range)
	const UInt32 count = val.size();
	IoTypeTraits<Int32>::putArraySize(os, count);
	NS_STDEXT_IO_BINARY::put32i(os, val.begin(), count);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(UInt32Range)
	const UInt32 count = val.size();
	IoTypeTraits<UInt32>::putArraySize(os, count);
	NS_STDEXT_IO_BINARY::put32i(os, val.begin(), count);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(Int64Range)
	const UInt32 count = val.size();
	IoTypeTraits<Int64>::putArraySize(os, count);
	NS_STDEXT_IO_BINARY::put64i(os, val.begin(), count);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(UInt64Range)
	const UInt32 count = val.size();
	IoTypeTraits<UInt64>::putArraySize(os, count);
	NS_STDEXT_IO_BINARY::put64i(os, val.begin(), count);
CERL_IO_END_PUTTER()

CERL_IO_BEGIN_PUTTER(NInformationRange)
	const UInt32 count = val.size();
	IoTypeTraits<NInformation>::putArraySize(os, count);
	NS_STDEXT_IO_BINARY::put_struct(os, val.begin(), count);
CERL_IO_END_PUTTER()

template <class Type>
struct Putter<typename NS_CERL::Range<const Type*, Type> >
{
	template <class OutputStreamT>
	static __forceinline void cerl_call call(OutputStreamT& os, const typename NS_CERL::Range<const Type*, Type>& val)
	{
		CERL_ASSERT(IoTypeTraits<Type>::vt == (int)vtUserType);

		const UInt32 count = val.size();
		const Type* array = val.begin();
		IoTypeTraits<Type>::putArraySize(os, count);
		for (UInt32 i = 0; i < count; ++i)
		{
			put(os, array[i]);
		}
	}
};

template <class Type>
struct Putter<NS_CERL::BasicArray<Type> >
{
	template <class OutputStreamT>
	static __forceinline void cerl_call call(OutputStreamT& os, const NS_CERL::BasicArray<Type>& val)
	{
		Putter<NS_CERL::Range<const Type*, Type> >::call(os, val);
	}
};

template <class Type, size_t nElement>
struct Putter<NS_CERL::Array<Type, nElement> >
{
	template <class OutputStreamT>
	static __forceinline void cerl_call call(OutputStreamT& os, const NS_CERL::Array<Type, nElement>& val)
	{
		typedef NS_CERL::Range<const Type*, Type> TypeRangeT;
		const TypeRangeT t(val.begin(), val.end());
		return put(os, t);
	}
};

template <class CharT>
struct Putter<NS_CERL::BasicString<CharT> >
{
	template <class OutputStreamT>
	static __forceinline void cerl_call call(OutputStreamT& os, const NS_CERL::BasicString<CharT>& val)
	{
		put(os, (const NS_CERL::Range<const CharT*, CharT>&)val);
	}
};


// -------------------------------------------------------------------------
// get array

template <class AllocT, class RangeT>
__forceinline typename RangeT::value_type* cerl_call allocArray_(AllocT& alloc, RangeT& val, UInt32 count)
{
	typedef typename RangeT::value_type Type;

	Type* array = STD_NEW_ARRAY(alloc, Type, count);
	val.assign(array, array + count);
	return array;
}

template <class Type>
__forceinline Type* cerl_call allocString_(std::basic_string<Type>& val, UInt32 count)
{
	val.resize(count);
	return std::_ConvIt(val.begin());
}

template <class InputStreamT>
inline bool cerl_call getString(InputStreamT& is, CharRange& val)
{
	UInt32 count;
	return
		IoTypeTraits<char>::getArraySize(is, count) &&
		is.get(allocArray_(is.getAlloc(), val, count), count) == count;
}

CERL_IO_BEGIN_GETTER(std::string)
	UInt32 count;
	return
		IoTypeTraits<char>::getArraySize(is, count) &&
		is.get(allocString_(val, count), count) == count;
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(CharRange)
	return getString(is, val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(ByteRange)
	return getString(is, *(CharRange*)&val);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Int16Range)
	UInt32 count;
	return
		IoTypeTraits<Int16>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get16i(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(UInt16Range)
	UInt32 count;
	return
		IoTypeTraits<UInt16>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get16i(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Int32Range)
	UInt32 count;
	return
		IoTypeTraits<Int32>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get32i(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(UInt32Range)
	UInt32 count;
	return
		IoTypeTraits<UInt32>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get32i(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(Int64Range)
	UInt32 count;
	return
		IoTypeTraits<Int64>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get64i(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(UInt64Range)
	UInt32 count;
	return
		IoTypeTraits<UInt64>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get64i(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

CERL_IO_BEGIN_GETTER(NInformationRange)
	UInt32 count;
	return
		IoTypeTraits<NInformation>::getArraySize(is, count) &&
		NS_STDEXT_IO_BINARY::get_struct(is, allocArray_(is.getAlloc(), val, count), count);
CERL_IO_END_GETTER()

template <class Type>
struct Getter<NS_STDEXT::Range<const Type*, Type> >
{
	template <class InputStreamT>
	static __forceinline bool cerl_call call(InputStreamT& is, NS_STDEXT::Range<const Type*, Type>& val) {
		CERL_ASSERT(IoTypeTraits<Type>::vt == (int)vtUserType);

		UInt32 count;
		if (!IoTypeTraits<Type>::getArraySize(is, count)) {
			return false;
		}

		Type* array = allocArray_(is.getAlloc(), val, count);
		for (UInt32 i = 0; i < count; ++i)
		{
			if (!get(is, array[i]))
				return false;
		}
		return true;
	}
};

template <class T>
struct Getter<NS_CERL::BasicArray<T> >
{
	template <class InputStreamT>
	static __forceinline bool cerl_call call(InputStreamT& is, NS_CERL::BasicArray<T>& val) {
		return Getter<NS_CERL::Range<const T*, T> >::call(is, val);
	}
};

template <class Type, size_t nElement>
struct Getter<NS_CERL::Array<Type, nElement> >
{
	template <class InputStreamT>
	static __forceinline bool cerl_call call(InputStreamT& is, NS_CERL::Array<Type, nElement>& val) {
		NS_CERL::Range<const Type*, Type> t;
		const bool fOk = get(is, t) && t.size() == nElement;
		if (fOk)
			std::copy(t.begin(), t.end(), val.begin());
		return fOk;
	}
};

template <class CharT>
struct Getter<NS_CERL::BasicString<CharT> >
{
	template <class InputStreamT>
	static __forceinline bool cerl_call call(InputStreamT& is, NS_CERL::BasicString<CharT>& val) {
		return get(is, (NS_CERL::Range<const CharT*, CharT>&)val);
	}
};


// =========================================================================
// function put(noargs)

template <class OutputStreamT>
__forceinline void cerl_call put(OutputStreamT& os, const NoArgs& noargs)
{
}

// =========================================================================
// function put_vt_null, check_vt_null

template <class OutputStreamT>
__forceinline bool cerl_call put_vt_null(OutputStreamT& os)
{
	CERL_ASSERT(vtNull == 0);
	os.put('\0');
	return os.put('\0') == 1;
}

template <class InputStreamT>
__forceinline bool cerl_call check_vt_null(InputStreamT& is)
{
	CERL_ASSERT(vtNull == 0);
	return is.get() == '\0' && is.get() == '\0';
}

// =========================================================================
// function skip_one

template <class InputStreamT>
inline IoType cerl_call skip_one(InputStreamT& is)
{
	UInt16 vtType;
	if (!NS_STDEXT_IO_BINARY::get16i(is, vtType))
		return vtInvalid;

	static char g_typeinfo[] =
	{
		0,	// vtNull			= 0x0000,
		1,	// vtI1				= 0x1000, // char, bool, INT8, UINT8
		2,	// vtI2				= 0x2000, // INT16, UINT16
		4,	// vtI4				= 0x3000, // INT32, UINT32
		8,	// vtI8				= 0x4000, // INT64, UINT64, NInformation
		8,	// vtR8				= 0x5000, // double
		4,	// vtR4				= 0x6000, // float
		-1,	// vtUserType		= 0x7000,
		12,	// vtPInformation	= 0x8000, // PInformation
		-2,	// vtReserved1		= 0x9000,
		-2, // vtReserved2		= 0xA000,
		-2, // vtReserved3		= 0xB000,
		-2, // vtReserved4		= 0xC000,
		-2, // vtReserved5		= 0xD000,
		-2, // vtReserved6		= 0xE000,
		-2, // vtReserved7		= 0xF000,
	};

	const ptrdiff_t cb = g_typeinfo[vtType >> 12];
	CERL_ASSERT(cb != -2);

	if (cb == -2)
		return vtInvalid;

	if (vtArrayMask & vtType)
	{
		UInt32 count;
		if (!getArraySize(is, vtType, count))
			return vtInvalid;

		if (cb >= 0)
		{
			is.skip(cb * count);
		}
		else
		{
			CERL_ASSERT(cb == -1); // user type
			for (UInt32 i = 0; i < count; ++i)
			{
				if (skip_one(is) == vtInvalid)
					return vtInvalid;
			}
		}
	}
	else
	{
		if (cb >= 0)
		{
			is.skip(cb);
		}
		else
		{
			CERL_ASSERT(cb == -1); // user type
			while (skip_one(is) > vtNull);
		}
	}
	return (IoType)vtType;
}

// =========================================================================

NS_CERL_IO_END

/*
#ifndef VENUS_IO_INL
#include "Io.inl"
#endif
*/

#endif /* VENUS_SERIALIZATION_H */
