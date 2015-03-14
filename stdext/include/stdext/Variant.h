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
// Module: stdext/Variant.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2008-6-14 9:41:58
// 
// $Id: Variant.h 642 2008-06-12 03:25:29Z xushiweizh@gmail.com $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_VARIANT_H
#define STDEXT_VARIANT_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_MEMORY_H
#include "Memory.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class Variant

template <template <class ValueT> class TypeTraitsT>
class Variant
{
public:
	typedef typename TypeTraitsT<void>::type_id type_id;

private:
	void* m_ptr;
	type_id m_vt;
	
#define WINX_VARIANT_CAST_TO_TYPE_(ValueT) \
	(sizeof(ValueT) <= sizeof(m_ptr) ? (ValueT*)&m_ptr : (ValueT*)m_ptr)
	
public:
	Variant() {
		m_vt = TypeTraitsT<void>::type();
	}

	template <class AllocT, class ValueT>
	Variant(AllocT& alloc, const ValueT& val) {
		assign(alloc, val);
	}
	
	template <class AllocT, class ValueT>
	void winx_call assign(AllocT& alloc, const ValueT& val) {
		m_vt = TypeTraitsT<ValueT>::type();
		if (sizeof(ValueT) <= sizeof(m_ptr)) {
			new(&m_ptr) ValueT(val);
		}
		else {
			m_ptr = STD_NEW(alloc, ValueT)(val);
		}
	}
	
	void winx_call swap(Variant& o) {
		swap((void*)this, (void*)&o, sizeof(o));
	}

	bool winx_call empty() const {
		return m_vt != TypeTraitsT<void>::type();
	}
	
	type_id winx_call type() const {
		return m_vt;
	}
	
	template <class ValueT>
	bool winx_call is() const {
		return m_vt == TypeTraitsT<ValueT>::type();
	}
	
	template <class ValueT>
	ValueT& winx_call get() {
		WINX_ASSERT(m_vt == TypeTraitsT<ValueT>::type());
		return *WINX_VARIANT_CAST_TO_TYPE_(ValueT);
	}

	template <class ValueT>
	const ValueT& winx_call get() const {
		WINX_ASSERT(m_vt == TypeTraitsT<ValueT>::type());
		return *WINX_VARIANT_CAST_TO_TYPE_(ValueT);
	}
};

// -------------------------------------------------------------------------
// class Any

#if defined(_RTTI) || defined(__RTTI__)

template <class ValueT>
struct AnyTypeTraits
{
	typedef std::type_info type_id;
	
	__forceinline static type_id winx_call type() {
		return typeid(ValueT);
	}
};

typedef Variant<AnyTypeTraits> Any;

#endif

// -------------------------------------------------------------------------

#define WINX_VARIANT_TYPES_BEGIN(ns, typeId, vtNone)						\
namespace ns {																\
																			\
	typedef typeId TypeId_;													\
																			\
	template <class Ty>														\
	struct TypeTraits_ {													\
	};																		\
																			\
	template <>																\
	struct TypeTraits_<void> {												\
		typedef TypeId_ type_id;											\
		static TypeId_ TPL_CALL type() {									\
			return vtNone;													\
		}																	\
	};

#define WINX_VARIANT_TYPE(Type, vt)											\
	template <>																\
	struct TypeTraits_<Type> {												\
		static TypeId_ TPL_CALL type() {									\
			return vt;														\
		}																	\
	};

#define WINX_VARIANT_TYPES_END()											\
	typedef NS_STDEXT::Variant<TypeTraits_> Variant;						\
}

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_VARIANT_H */

