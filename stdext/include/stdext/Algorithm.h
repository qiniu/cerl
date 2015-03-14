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
// Module: stdext/Algorithm.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-30 16:43:10
// 
// $Id: Algorithm.h,v 1.1 2006/11/30 08:45:42 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ALGORITHM_H
#define STDEXT_ALGORITHM_H

#ifndef STDEXT_BASIC_H
#include "Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// algorithm - set_intersection_do

/*
@fn std::set_intersection_do(first1,last1,first2,last2,op)
@brief
	Unites all of the elements that belong to both sorted source ranges and
	do some operation on the united elements.
@arg first1
	An input iterator addressing the position of the first element in the first
	of two sorted source ranges to be united and sorted into a single range 
	representing the intersection of the two source ranges. 
@arg last1
	An input iterator addressing the position one past the last element in the 
	first of two sorted source ranges to be united and sorted into a single range
	representing the intersection of the two source ranges. 
@arg first2
	An input iterator addressing the position of the first element in second of 
	two consecutive sorted source ranges to be united and sorted into a single 
	range representing the intersection of the two source ranges. 
@arg last2
	An input iterator addressing the position one past the last element in second 
	of two consecutive sorted source ranges to be united and sorted into a single
	range representing the intersection of the two source ranges.
@arg op
	The operation which will be dealed on the united elements.
@return
	The operation.
@remark
	The sorted source ranges referenced must be valid; all pointers must be 
	dereferenceable and within each sequence the last position must be reachable 
	from the first by incrementation. 

	The sorted source ranges must each be arranged as a precondition to the application
	of the merge algorithm in accordance with the same ordering as is to be used by 
	the algorithm to sort the combined ranges.

	Being differ from std::set_intersection, the element type of the two sorted source 
	ranges can be different.

	The value types of the input iterators need be less-than comparable to be ordered,
	so that, given two elements, it may be determined either that they are equivalent 
	(in the sense that neither is less than the other) or that one is less than the 
	other. This results in an ordering between the nonequivalent elements. When there
	are equivalent elements in both source ranges, the elements in the first range 
	precede the elements from the second source range in the destination range. If the
	source ranges contain duplicates of an element, then the destination range will 
	contain the maximum number of those elements that occur in both source ranges.

	The complexity of the algorithm is linear with at most 2*((last1每first1)每(last2每first2))每1
	comparisons for nonempty source ranges.
@*/
template <class InputIterT1, class InputIterT2, class OperatorT>
inline OperatorT& winx_call set_intersection_do(
	InputIterT1 first1, InputIterT1 last1,
	InputIterT2 first2, InputIterT2 last2, OperatorT& op)
{
	for (; first1 != last1 && first2 != last2; )
	{
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else
			op(*first1++, *first2++);
	}
	return op;
}

template <class InputIterT1, class InputIterT2, class OperatorT>
inline OperatorT& winx_call set_difference_do(
	InputIterT1 first1, InputIterT1 last1,
	InputIterT2 first2, InputIterT2 last2, OperatorT& op)
{
	for (; first1 != last1 && first2 != last2; )
	{
		if (*first1 < *first2)
			op(*first1++);
		else if (*first2 < *first1)
			++first2;
		else
			++first1, ++first2;
	}
	std::for_each(first1, last1, op);
	return op;
}

template <class InputIterT1, class InputIterT2, class OperatorT>
inline OperatorT& winx_call set_union_do(
	InputIterT1 first1, InputIterT1 last1,
	InputIterT2 first2, InputIterT2 last2, OperatorT& op)
{
	for (; first1 != last1 && first2 != last2; )
	{
		if (*first1 < *first2)
			op.set1(*first1++);
		else if (*first2 < *first1)
			op.set2(*first2++);
		else
			op.set12(*first1++, *first2++);
	}
	for (; first1 != last1; )
	{
		op.set1(*first1++);
	}
	for (; first2 != last2; )
	{
		op.set2(*first2++);
	}
	return op;
}

/*
@fn std::set_intersection_do(first1,last1,first2,last2,op,pred)
@brief
	Unites all of the elements that belong to both sorted source ranges and
	do some operation on the united elements.
@arg first1
	An input iterator addressing the position of the first element in the first
	of two sorted source ranges to be united and sorted into a single range 
	representing the intersection of the two source ranges. 
@arg last1
	An input iterator addressing the position one past the last element in the 
	first of two sorted source ranges to be united and sorted into a single range
	representing the intersection of the two source ranges. 
@arg first2
	An input iterator addressing the position of the first element in second of 
	two consecutive sorted source ranges to be united and sorted into a single 
	range representing the intersection of the two source ranges. 
@arg last2
	An input iterator addressing the position one past the last element in second 
	of two consecutive sorted source ranges to be united and sorted into a single
	range representing the intersection of the two source ranges.
@arg op
	The operation which will be dealed on the united elements.
@arg pred
	User-defined predicate function object that defines the sense in which one 
	element is greater than another. The binary predicate takes two arguments and
	should return <b>true</b> when the first element is less than the second element 
	and <b>false</b> otherwise. 
@return
	The operation.
@remark
	The sorted source ranges referenced must be valid; all pointers must be 
	dereferenceable and within each sequence the last position must be reachable 
	from the first by incrementation. 

	The sorted source ranges must each be arranged as a precondition to the application
	of the merge algorithm in accordance with the same ordering as is to be used by 
	the algorithm to sort the combined ranges.

	Being differ from std::set_intersection, the element type of the two sorted source 
	ranges can be different.

	The value types of the input iterators need be less-than comparable to be ordered,
	so that, given two elements, it may be determined either that they are equivalent 
	(in the sense that neither is less than the other) or that one is less than the 
	other. This results in an ordering between the nonequivalent elements. When there
	are equivalent elements in both source ranges, the elements in the first range 
	precede the elements from the second source range in the destination range. If the
	source ranges contain duplicates of an element, then the destination range will 
	contain the maximum number of those elements that occur in both source ranges.

	The complexity of the algorithm is linear with at most 2*((last1每first1)每(last2每first2))每1
	comparisons for nonempty source ranges.
@*/
template<class InputIterT1, class InputIterT2, class OperatorT, class PredT>
inline OperatorT& winx_call set_intersection_do(
	InputIterT1 first1, InputIterT1 last1,
	InputIterT2 first2, InputIterT2 last2, OperatorT& op, PredT pred)
{
	for (; first1 != last1 && first2 != last2; )
	{
		if (pred(*first1, *first2))
			++first1;
		else if (pred(*first2, *first1))
			++first2;
		else
			op(*first1++, *first2++);
	}
	return op;
}

template<class InputIterT1, class InputIterT2, class OperatorT, class PredT>
inline OperatorT& winx_call set_difference_do(
	InputIterT1 first1, InputIterT1 last1,
	InputIterT2 first2, InputIterT2 last2, OperatorT& op, PredT pred)
{
	for (; first1 != last1 && first2 != last2; )
	{
		if (pred(*first1, *first2))
			op(*first1++);
		else if (pred(*first2, *first1))
			++first2;
		else
			++first1, ++first2;
	}
	std::for_each(first1, last1, op);
	return op;
}

/*
@fn std::set_intersection_do(cont1,cont2,op)
@brief
	Unites all of the elements that belong to both sorted source containers and
	do some operation on the united elements.
@arg cont1
	An sorted container representing the first of two sorted source ranges to be 
	united and sorted into a single range.
@arg cont2
	An sorted container representing the second of two sorted source ranges to be 
	united and sorted into a single range.
@arg op
	The operation which will be dealed on the united elements.
@return
	The operation.
@*/
template <class ContainerT1, class ContainerT2, class OperatorT>
inline OperatorT& winx_call set_intersection_do(
	const ContainerT1& cont1, const ContainerT2& cont2, OperatorT& op)
{
	return set_intersection_do(
		cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), op);
}

/*
@fn std::set_intersection_do(cont1,cont2,op,pred)
@brief
	Unites all of the elements that belong to both sorted source containers and
	do some operation on the united elements.
@arg cont1
	An sorted container representing the first of two sorted source ranges to be 
	united and sorted into a single range.
@arg cont2
	An sorted container representing the second of two sorted source ranges to be 
	united and sorted into a single range.
@arg op
	The operation which will be dealed on the united elements.
@arg pred
	User-defined predicate function object that defines the sense in which one 
	element is greater than another. The binary predicate takes two arguments and
	should return <b>true</b> when the first element is less than the second element 
	and <b>false</b> otherwise. 
@return
	The operation.
@*/
template <class ContainerT1, class ContainerT2, class OperatorT, class PredT>
inline OperatorT& winx_call set_intersection_do(
	const ContainerT1& cont1, const ContainerT2& cont2, OperatorT& op, PredT pred)
{
	return set_intersection_do(
		cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), op, pred);
}

// -------------------------------------------------------------------------
// algorithm - set_intersect_count

class IntersectionCount_
{
private:
	unsigned m_count;

public:
	IntersectionCount_() : m_count(0) {}
	operator unsigned() const {	return m_count; }

	template <class ItemT1, class ItemT2>
	void winx_call operator()(const ItemT1& i1, const ItemT1& i2) {
		++m_count;
	}
};

/*
@fn std::set_intersection_count(first1,last1,first2,last2)
@brief
	Calculates count of the elements that belong to both sorted source ranges.
@arg first1
	An input iterator addressing the position of the first element in the first
	of two sorted source ranges to be united and sorted into a single range 
	representing the intersection of the two source ranges. 
@arg last1
	An input iterator addressing the position one past the last element in the 
	first of two sorted source ranges to be united and sorted into a single range
	representing the intersection of the two source ranges. 
@arg first2
	An input iterator addressing the position of the first element in second of 
	two consecutive sorted source ranges to be united and sorted into a single 
	range representing the intersection of the two source ranges. 
@arg last2
	An input iterator addressing the position one past the last element in second 
	of two consecutive sorted source ranges to be united and sorted into a single
	range representing the intersection of the two source ranges.
@return
	The count of unite elements.
@see std::set_intersection_do
@*/
template <class InputIterT1, class InputIterT2>
inline unsigned winx_call set_intersection_count(
	InputIterT1 first1, InputIterT1 last1, InputIterT2 first2, InputIterT2 last2)
{
	return set_intersection_do(first1, last1, first2, last2, IntersectionCount_());
}

/*
@fn std::set_intersection_count(first1,last1,first2,last2,pred)
@brief
	Calculates count of the elements that belong to both sorted source ranges.
@arg first1
	An input iterator addressing the position of the first element in the first
	of two sorted source ranges to be united and sorted into a single range 
	representing the intersection of the two source ranges. 
@arg last1
	An input iterator addressing the position one past the last element in the 
	first of two sorted source ranges to be united and sorted into a single range
	representing the intersection of the two source ranges. 
@arg first2
	An input iterator addressing the position of the first element in second of 
	two consecutive sorted source ranges to be united and sorted into a single 
	range representing the intersection of the two source ranges. 
@arg last2
	An input iterator addressing the position one past the last element in second 
	of two consecutive sorted source ranges to be united and sorted into a single
	range representing the intersection of the two source ranges.
@arg pred
	User-defined predicate function object that defines the sense in which one 
	element is greater than another. The binary predicate takes two arguments and
	should return <b>true</b> when the first element is less than the second element 
	and <b>false</b> otherwise. 
@return
	The count of unite elements.
@see std::set_intersection_do
@*/
template <class InputIterT1, class InputIterT2, class PredT>
inline unsigned winx_call set_intersection_count(
	InputIterT1 first1, InputIterT1 last1, InputIterT2 first2, InputIterT2 last2, PredT pred)
{
	return set_intersection_do(first1, last1, first2, last2, IntersectionCount_(), pred);
}

/*
@fn std::set_intersection_count(cont1,cont2)
@brief
	Calculates count of the elements that belong to both sorted source ranges.
@arg cont1
	An sorted container representing the first of two sorted source ranges to be 
	united and sorted into a single range.
@arg cont2
	An sorted container representing the second of two sorted source ranges to be 
	united and sorted into a single range.
@return
	The count of unite elements.
@see std::set_intersection_do
@*/
template <class ContainerT1, class ContainerT2>
inline unsigned winx_call set_intersection_count(
	const ContainerT1& cont1, const ContainerT2& cont2)
{
	return set_intersection_do(
		cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), IntersectionCount_());
}

/*
@fn std::set_intersection_count(cont1,cont2,pred)
@brief
	Calculates count of the elements that belong to both sorted source ranges.
@arg cont1
	An sorted container representing the first of two sorted source ranges to be 
	united and sorted into a single range.
@arg cont2
	An sorted container representing the second of two sorted source ranges to be 
	united and sorted into a single range.
@arg pred
	User-defined predicate function object that defines the sense in which one 
	element is greater than another. The binary predicate takes two arguments and
	should return <b>true</b> when the first element is less than the second element 
	and <b>false</b> otherwise. 
@return
	The count of unite elements.
@see std::set_intersection_do
@*/
template <class ContainerT1, class ContainerT2, class PredT>
inline unsigned winx_call set_intersection_count(
	const ContainerT1& cont1, const ContainerT2& cont2, PredT pred)
{
	return set_intersection_do(
		cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), IntersectionCount_(), pred);
}

// -------------------------------------------------------------------------
// algorithm - purge

/*
@fn std::purge
@brief
	Clear all elements of an container and free the used memory.
@arg cont
	The container to purge/clear. It can be a std::vector, std::string, etc.
@remark
	This method purge/clear all elements of an container "physically", including to free
	the used memory. If you use vector::clear or basic_string::erase, They maybe just
	destruct elements in the container, doesn't free the used memory.
@*/
template <class ContainerT>
inline void winx_call purge(ContainerT& cont)
{
	ContainerT contNull;
	cont.swap(contNull);
}

// -------------------------------------------------------------------------
// $Log: Algorithm.h,v $

NS_STDEXT_END

#endif /* STDEXT_ALGORITHM_H */
