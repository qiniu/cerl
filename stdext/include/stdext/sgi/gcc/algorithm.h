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
// Module: sgi/gcc/algorithm.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: algorithm.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __SGI_GCC_ALGORITHM_H__
#define __SGI_GCC_ALGORITHM_H__

#if !defined(X_STL_GCC)
#error "only for compiling sgi-stl for gcc"
#endif

// -------------------------------------------------------------------------

#if !defined(_ALGORITHM)
#include <algorithm>
#endif

#ifndef _EXT_ALGORITHM
#include <ext/algorithm>
#endif

#ifndef _EXT_MEMORY
#include <ext/memory>
#endif

#ifndef __SGI_STL_ALGORITHM
	#define __SGI_STL_ALGORITHM
#endif

#ifndef __SGI_STL_INTERNAL_ALGO_H
	#define __SGI_STL_INTERNAL_ALGO_H
#endif

#ifndef __SGI_STL_INTERNAL_ALGOBASE_H
	#define __SGI_STL_INTERNAL_ALGOBASE_H
#endif

#ifndef __SGI_STL_ALGO_H
	#define __SGI_STL_ALGO_H
#endif

#ifndef __SGI_STL_ALGOBASE_H
	#define __SGI_STL_ALGOBASE_H
#endif

namespace __STD
{
	// Names from <stl_algo.h>
	using std::for_each; 
	using std::find; 
	using std::find_if; 
	using std::adjacent_find; 
	using std::count; 
	using std::count_if; 
	using std::search; 
	using std::search_n; 
	using std::swap_ranges; 
	using std::transform; 
	using std::replace; 
	using std::replace_if; 
	using std::replace_copy; 
	using std::replace_copy_if; 
	using std::generate; 
	using std::generate_n; 
	using std::remove; 
	using std::remove_if; 
	using std::remove_copy; 
	using std::remove_copy_if; 
	using std::unique; 
	using std::unique_copy; 
	using std::reverse; 
	using std::reverse_copy; 
	using std::rotate; 
	using std::rotate_copy; 
	using std::random_shuffle; 
//	using std::random_sample; 
//	using std::random_sample_n; 
	using std::partition; 
	using std::stable_partition; 
	using std::sort; 
	using std::stable_sort; 
	using std::partial_sort; 
	using std::partial_sort_copy; 
	using std::nth_element; 
	using std::lower_bound; 
	using std::upper_bound; 
	using std::equal_range; 
	using std::binary_search; 
	using std::merge; 
	using std::inplace_merge; 
	using std::includes; 
	using std::set_union; 
	using std::set_intersection; 
	using std::set_difference; 
	using std::set_symmetric_difference; 
	using std::min_element; 
	using std::max_element; 
	using std::next_permutation; 
	using std::prev_permutation; 
	using std::find_first_of; 
	using std::find_end; 
//	using std::is_sorted; 
//	using std::is_heap; 

	// Names from stl_heap.h
	using std::push_heap;
	using std::pop_heap;
	using std::make_heap;
	using std::sort_heap;

	// Names from <stl_numeric.h>
//	using std::accumulate; 
//	using std::inner_product; 
//	using std::partial_sum; 
//	using std::adjacent_difference; 
//	using std::power; 
//	using std::iota; 
}

// -------------------------------------------------------------------------
// $Log: algorithm.h,v $

#endif /* __SGI_GCC_ALGORITHM_H__ */
