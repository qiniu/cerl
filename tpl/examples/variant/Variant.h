#include <stdext/Variant.h>
#include <stdext/String.h>	// std::String

class Procedure
{
private:
	size_t m_proc;
};

// class test::Variant

enum TypeList {
	vtNone		= 0,		// void
	vtInt		= 0x0001,	// int
	vtDouble	= 0x0002,	// Double
	vtBool		= 0x0004,	// bool
	vtString	= 0x0008,	// std::String
	vtProc		= 0x0010,	// Procedure
};

WINX_VARIANT_TYPES_BEGIN(test, TypeList, vtNone)
	WINX_VARIANT_TYPE(int, vtInt)
	WINX_VARIANT_TYPE(double, vtDouble)
WINX_VARIANT_TYPES_END()

typedef test::Variant Variant;

inline double TPL_CALL to_double(const Variant& a)
{
	switch (a.type())
	{
	case vtDouble:	return a.get<double>();
	case vtInt:		return a.get<int>();
	default:		throw std::bad_cast();
	}
	return 0;
}

namespace std {

template <class OStreamT>
inline OStreamT& TPL_CALL operator<<(OStreamT& os, const test::Variant& a)
{
	switch (a.type())
	{
	case vtDouble:	return os << a.get<double>();
	case vtInt:		return os << a.get<int>();
	default:		return os << "(unassigned)";
	}
}

inline bool operator<(const test::Variant& a, const test::Variant& b)
{
	return to_double(a) < to_double(b);
}

}

template <class AllocT>
inline Variant TPL_CALL ext_multiplies(
	AllocT& alloc, const Variant& a, const Variant& b)
{
	if (a.type() == vtInt && b.type() == vtInt) {
		return Variant(alloc, a.get<int>() * b.get<int>());
	}
	else
		return Variant(alloc, to_double(a) * to_double(b));
}

template <class AllocT>
inline Variant TPL_CALL ext_divides(
	AllocT& alloc, const Variant& a, const Variant& b)
{
	if (a.type() == vtInt && b.type() == vtInt)
		return Variant(alloc, a.get<int>() / b.get<int>());
	else
		return Variant(alloc, to_double(a) / to_double(b));
}

template <class AllocT>
inline Variant TPL_CALL ext_assign(
	AllocT& alloc, Variant& a, const Variant& b)
{
	if (a.type() != vtNone)
		throw std::logic_error("variable is already assigned!");
	return a = b;
}

