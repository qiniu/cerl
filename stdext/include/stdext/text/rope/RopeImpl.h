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
// Module: stdext/text/rope/Rope.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: Rope.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_ROPE_ROPEIMPL_H
#define STDEXT_TEXT_ROPE_ROPEIMPL_H

/*
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

// -------------------------------------------------------------------------
// stdext::identity_element

NS_STDEXT_BEGIN

// Some helpers, so we can use power on ropes.
// See below for why this isn't local to the implementation.

template<class _CharT, class _Alloc>
struct _Rope_Concat_fn : public binary_function<
	Rope<_CharT,_Alloc>, Rope<_CharT,_Alloc>, Rope<_CharT,_Alloc> >
{
	_Alloc& m_alloc;
	_Rope_Concat_fn(_Alloc& alloc) : m_alloc(alloc) {}

	Rope<_CharT,_Alloc> winx_call operator()(
		Rope<_CharT,_Alloc> __x, const Rope<_CharT,_Alloc>& __y) {
		return __x.append(__y);
	}
};

NS_STDEXT_END

namespace __STD {

template <class _CharT, class _Alloc>
inline std::Rope<_CharT,_Alloc>
	winx_call identity_element(const std::_Rope_Concat_fn<_CharT, _Alloc>& r)
{
    return std::Rope<_CharT,_Alloc>(r.m_alloc);
}

} // namespace __STD

// -------------------------------------------------------------------------

NS_STDEXT_BEGIN

using __STD::identity_element;

// Concatenate a C string onto a leaf Rope by copying the Rope data.
// Used for short ropes.
template <class _CharT, class _Alloc>
inline typename Rope<_CharT,_Alloc>::_RopeLeaf*
winx_call Rope<_CharT,_Alloc>::_S_leaf_concat_char_iter
		(_RopeLeaf* __r, const _CharT* __iter, size_t __len, _Alloc& __a)
{
    size_t __old_len = __r->_M_size;
    _CharT* __new_data = 
		STD_ALLOC_ARRAY(__a, _CharT, __old_len + __len);
    _RopeLeaf* __result;
    
    stdext::uninitialized_copy_n(__r->_M_data, __old_len, __new_data);
    stdext::uninitialized_copy_n(__iter, __len, __new_data + __old_len);
	__result = _S_new_RopeLeaf(__new_data, __old_len + __len, __a);
    return __result;
}

// Assumes left and right are not 0.
// Does not increment (nor decrement on exception) child reference counts.
// Result has ref count 1.
template <class _CharT, class _Alloc>
inline typename Rope<_CharT,_Alloc>::_RopeRep*
winx_call Rope<_CharT,_Alloc>::_S_tree_concat (_RopeRep* __left, _RopeRep* __right, _Alloc& __a)
{
    _RopeConcatenation* __result =
      _S_new_RopeConcatenation(__left, __right, __a);
    size_t __depth = __result->_M_depth;
    
    if (__depth > 20 && (__result->_M_size < 1000 ||
			 __depth > _RopeRep::_S_max_rope_depth)) {
        _RopeRep* __balanced;
      
	   __balanced = _S_balance(__result, __a);
		return __balanced;
    } else {
		return __result;
    }
}

template <class _CharT, class _Alloc>
inline typename Rope<_CharT,_Alloc>::_RopeRep* 
winx_call Rope<_CharT,_Alloc>::_S_concat_char_iter
		(_RopeRep* __r, const _CharT*__s, size_t __slen, _Alloc& __a)
{
    _RopeRep* __result;
    if (0 == __slen) {
	return __r;
    }
    if (0 == __r)
		return __STL_ROPE_FROM_UNOWNED_CHAR_PTR(__s, __slen, __a);
    if (_RopeRep::_S_leaf == __r->_M_tag && __r->_M_size + __slen <= _S_copy_max) {
		__result = _S_leaf_concat_char_iter((_RopeLeaf*)__r, __s, __slen, __a);
		return __result;
    }
    if (_RopeRep::_S_concat == __r->_M_tag &&
		_RopeRep::_S_leaf == ((_RopeConcatenation*)__r)->_M_right->_M_tag) {
		_RopeLeaf* __right = 
			(_RopeLeaf* )(((_RopeConcatenation* )__r)->_M_right);
		if (__right->_M_size + __slen <= _S_copy_max) {
			_RopeRep* __left = ((_RopeConcatenation*)__r)->_M_left;
			_RopeRep* __nright = 
				_S_leaf_concat_char_iter((_RopeLeaf*)__right, __s, __slen, __a);
			__result = _S_tree_concat(__left, __nright, __a);
			return __result;
		}
    }
    _RopeRep* __nright =
      __STL_ROPE_FROM_UNOWNED_CHAR_PTR(__s, __slen, __a);
      __result = _S_tree_concat(__r, __nright, __a);
    return __result;
}

template <class _CharT, class _Alloc>
inline typename Rope<_CharT,_Alloc>::_RopeRep*
winx_call Rope<_CharT,_Alloc>::_S_concat(_RopeRep* __left, _RopeRep* __right, _Alloc& __a)
{
    if (0 == __left) {
	return __right;
    }
    if (0 == __right) {
	return __left;
    }
    if (_RopeRep::_S_leaf == __right->_M_tag) {
	if (_RopeRep::_S_leaf == __left->_M_tag) {
	  if (__right->_M_size + __left->_M_size <= _S_copy_max) {
	    return _S_leaf_concat_char_iter((_RopeLeaf*)__left,
					 ((_RopeLeaf*)__right)->_M_data,
					 __right->_M_size, __a);
	  }
	} else if (_RopeRep::_S_concat == __left->_M_tag
		   && _RopeRep::_S_leaf ==
		      ((_RopeConcatenation*)__left)->_M_right->_M_tag) {
	  _RopeLeaf* __leftright =
		    (_RopeLeaf*)(((_RopeConcatenation*)__left)->_M_right); 
	  if (__leftright->_M_size + __right->_M_size <= _S_copy_max) {
	    _RopeRep* __leftleft = ((_RopeConcatenation*)__left)->_M_left;
	    _RopeRep* __rest = _S_leaf_concat_char_iter(__leftright,
					   ((_RopeLeaf*)__right)->_M_data,
					   __right->_M_size, __a);
	      return _S_tree_concat(__leftleft, __rest, __a);
	  }
	}
    }
	return _S_tree_concat(__left, __right, __a);
}

template <class _CharT, class _Alloc>
inline typename Rope<_CharT,_Alloc>::_RopeRep*
winx_call Rope<_CharT,_Alloc>::_S_substring(_RopeRep* __base, 
                               size_t __start, size_t __endp1, _Alloc& __a)
{
    if (0 == __base) return 0;
    size_t __len = __base->_M_size;
    size_t __adj_endp1;
    const size_t __lazy_threshold = 128;
    
    if (__endp1 >= __len) {
	if (0 == __start) {
	    return __base;
	} else {
	    __adj_endp1 = __len;
	}
    } else {
	__adj_endp1 = __endp1;
    }
    switch(__base->_M_tag) {
	case _RopeRep::_S_concat:
	    {
		_RopeConcatenation* __c = (_RopeConcatenation*)__base;
		_RopeRep* __left = __c->_M_left;
		_RopeRep* __right = __c->_M_right;
		size_t __left_len = __left->_M_size;
		_RopeRep* __result;

		if (__adj_endp1 <= __left_len) {
		    return _S_substring(__left, __start, __endp1, __a);
		} else if (__start >= __left_len) {
		    return _S_substring(__right, __start - __left_len,
				  __adj_endp1 - __left_len, __a);
		}
		_Self_destruct_ptr __left_result(
		  _S_substring(__left, __start, __left_len, __a));
		_Self_destruct_ptr __right_result(
		  _S_substring(__right, 0, __endp1 - __left_len, __a));
		__result = _S_concat(__left_result, __right_result, __a);
		return __result;
	    }
	case _RopeRep::_S_leaf:
	    {
		_RopeLeaf* __l = (_RopeLeaf*)__base;
		_RopeLeaf* __result;
		size_t __result_len;
		if (__start >= __adj_endp1) return 0;
		__result_len = __adj_endp1 - __start;
		if (__result_len > __lazy_threshold) goto lazy;
		const _CharT* __section = __l->_M_data + __start;
		__result = _S_new_RopeLeaf(__section, __result_len, __a);
		return __result;
	    }
	case _RopeRep::_S_substringfn:
	    // Avoid introducing multiple layers of substring nodes.
	    {
		_RopeSubstring* __old = (_RopeSubstring*)__base;
		size_t __result_len;
		if (__start >= __adj_endp1) return 0;
		__result_len = __adj_endp1 - __start;
		if (__result_len > __lazy_threshold) {
		    _RopeSubstring* __result =
			_S_new_RopeSubstring(__old->_M_base,
					  __start + __old->_M_start,
					  __adj_endp1 - __start,
					  __a);
		    return __result;

		} // *** else fall through: ***
	    }
	case _RopeRep::_S_function:
	    {
		_RopeFunction* __f = (_RopeFunction*)__base;
		_CharT* __section;
		size_t __result_len;
		if (__start >= __adj_endp1) return 0;
		__result_len = __adj_endp1 - __start;

		if (__result_len > __lazy_threshold) goto lazy;
		__section = STD_ALLOC_ARRAY(__a, _CharT, __result_len);
		  (*(__f->_M_fn))(__start, __result_len, __section);
		return _S_new_RopeLeaf(__section, __result_len, __a);
	    }
    }
    /*NOTREACHED*/
    WINX_ASSERT(false);
  lazy:
    {
	// Create substring node.
	return _S_new_RopeSubstring(__base, __start, __adj_endp1 - __start, __a);
    }
}

template<class _CharT>
class _Rope_flatten_char_consumer {
    private:
	_CharT* _M_buf_ptr;
    public:

	_Rope_flatten_char_consumer(_CharT* __buffer) {
	    _M_buf_ptr = __buffer;
	};
	bool winx_call operator() (const _CharT* __leaf, size_t __n) {
	    stdext::uninitialized_copy_n(__leaf, __n, _M_buf_ptr);
	    _M_buf_ptr += __n;
	    return true;
	}
};
	    
template<class _CharT>
class _Rope_find_char_char_consumer {
    private:
	_CharT _M_pattern;
    public:
	size_t _M_count;  // Number of nonmatching characters
	_Rope_find_char_char_consumer(_CharT __p) 
	  : _M_pattern(__p), _M_count(0) {}
	bool winx_call operator() (const _CharT* __leaf, size_t __n) {
	    size_t __i;
	    for (__i = 0; __i < __n; __i++) {
		if (__leaf[__i] == _M_pattern) {
		    _M_count += __i; return false;
		}
	    }
	    _M_count += __n; return true;
	}
};

template <class _CharT, class _Alloc>
inline _CharT* winx_call Rope<_CharT,_Alloc>::_S_flatten(_RopeRep* __r,
				 size_t __start, size_t __len,
				 _CharT* __buffer, _Alloc& __a)
{
    _Rope_flatten_char_consumer<_CharT> __c(__buffer);
    _S_apply_to_pieces(__a, __c, __r, __start, __start + __len);
    return(__buffer + __len);
}

template <class _CharT, class _Alloc>
inline size_t winx_call Rope<_CharT,_Alloc>::find(_CharT __pattern, size_t __start) const
{
    _Rope_find_char_char_consumer<_CharT> __c(__pattern);
    _S_apply_to_pieces(*_M_alloc, __c, _M_tree_ptr, __start, size());
    size_type __result_pos = __start + __c._M_count;
#   ifndef __STL_OLD_ROPE_SEMANTICS
	if (__result_pos == size()) __result_pos = npos;
#   endif
    return __result_pos;
}

template <class _CharT, class _Alloc>
inline _CharT* winx_call Rope<_CharT,_Alloc>::_S_flatten(_RopeRep* __r, _CharT* __buffer)
{
    if (0 == __r) return __buffer;
    switch(__r->_M_tag) {
	case _RopeRep::_S_concat:
	    {
		_RopeConcatenation* __c = (_RopeConcatenation*)__r;
		_RopeRep* __left = __c->_M_left;
		_RopeRep* __right = __c->_M_right;
		_CharT* __rest = _S_flatten(__left, __buffer);
		return _S_flatten(__right, __rest);
	    }
	case _RopeRep::_S_leaf:
	    {
		_RopeLeaf* __l = (_RopeLeaf*)__r;
		return stdext::copy_n(__l->_M_data, __l->_M_size, __buffer).second;
	    }
	case _RopeRep::_S_function:
	case _RopeRep::_S_substringfn:
	    // We dont yet do anything with substring nodes.
	    // This needs to be fixed before ropefiles will work well.
	    {
		_RopeFunction* __f = (_RopeFunction*)__r;
		(*(__f->_M_fn))(0, __f->_M_size, __buffer);
		return __buffer + __f->_M_size;
	    }
	default:
	    WINX_ASSERT(false);
	    /*NOTREACHED*/
	    return 0;
    }
}


// This needs work for _CharT != char
template <class _CharT, class _Alloc>
inline void winx_call Rope<_CharT,_Alloc>::_S_dump(_RopeRep* __r, int __indent)
{
    for (int __i = 0; __i < __indent; __i++) putchar(' ');
    if (0 == __r) {
	printf("NULLn"); return;
    }
    if (_RopeRep::_S_concat == __r->_M_tag) {
	_RopeConcatenation* __c = (_RopeConcatenation*)__r;
	_RopeRep* __left = __c->_M_left;
	_RopeRep* __right = __c->_M_right;

	printf("Concatenation %p (depth = %d, len = %ld, %s balanced)n",
	    __r, __r->_M_depth, __r->_M_size, __r->_M_is_balanced? "" : "not");
	_S_dump(__left, __indent + 2);
	_S_dump(__right, __indent + 2);
	return;
    } else {
	char* __kind;

	switch (__r->_M_tag) {
	    case _RopeRep::_S_leaf:
		__kind = "Leaf";
		break;
	    case _RopeRep::_S_function:
		__kind = "Function";
		break;
	    case _RopeRep::_S_substringfn:
		__kind = "Function representing substring";
		break;
	    default:
		__kind = "(corrupted kind field!)";
	}
	  printf("%s %p (depth = %d, len = %ld) ",
		 __kind, __r, __r->_M_depth, __r->_M_size);
	if (_S_is_one_byte_char_type((_CharT*)0)) {
	    const int __max_len = 40;
	    _Self_destruct_ptr __prefix(_S_substring(__r, 0, __max_len));
	    _CharT __buffer[__max_len + 1];
	    bool __too_big = __r->_M_size > __prefix->_M_size;

	    _S_flatten(__prefix, __buffer);
	    __buffer[__prefix->_M_size] = _CharT(); 
	    printf("%s%sn", 
	           (char*)__buffer, __too_big? "...n" : "n");
	} else {
	    printf("n");
	}
    }
}

template <class _CharT, class _Alloc>
const unsigned long
Rope<_CharT,_Alloc>::_S_min_len[
  _Rope_RopeRep<_CharT>::_S_max_rope_depth + 1] = {
/* 0 */1, /* 1 */2, /* 2 */3, /* 3 */5, /* 4 */8, /* 5 */13, /* 6 */21,
/* 7 */34, /* 8 */55, /* 9 */89, /* 10 */144, /* 11 */233, /* 12 */377,
/* 13 */610, /* 14 */987, /* 15 */1597, /* 16 */2584, /* 17 */4181,
/* 18 */6765, /* 19 */10946, /* 20 */17711, /* 21 */28657, /* 22 */46368,
/* 23 */75025, /* 24 */121393, /* 25 */196418, /* 26 */317811,
/* 27 */514229, /* 28 */832040, /* 29 */1346269, /* 30 */2178309,
/* 31 */3524578, /* 32 */5702887, /* 33 */9227465, /* 34 */14930352,
/* 35 */24157817, /* 36 */39088169, /* 37 */63245986, /* 38 */102334155,
/* 39 */165580141, /* 40 */267914296, /* 41 */433494437,
/* 42 */701408733, /* 43 */1134903170, /* 44 */1836311903,
/* 45 */2971215073u };
// These are Fibonacci numbers < 2**32.

template <class _CharT, class _Alloc>
inline typename Rope<_CharT,_Alloc>::_RopeRep*
winx_call Rope<_CharT,_Alloc>::_S_balance(_RopeRep* __r, _Alloc& __a)
{
    _RopeRep* __forest[_RopeRep::_S_max_rope_depth + 1];
    _RopeRep* __result = 0;
    int __i;
    // Invariant:
    // The concatenation of forest in descending order is equal to __r.
    // __forest[__i]._M_size >= _S_min_len[__i]
    // __forest[__i]._M_depth = __i
    // References from forest are included in refcount.

    for (__i = 0; __i <= _RopeRep::_S_max_rope_depth; ++__i) 
      __forest[__i] = 0;
      _S_add_to_forest(__r, __forest, __a);
      for (__i = 0; __i <= _RopeRep::_S_max_rope_depth; ++__i) 
        if (0 != __forest[__i]) {
	  __result = _S_concat(__forest[__i], __result, __a);
      }
    if (__result->_M_depth > _RopeRep::_S_max_rope_depth) {
#     ifdef __STL_USE_EXCEPTIONS
	__STL_THROW(length_error("Rope too long"));
#     else
	abort();
#     endif
    }
    return(__result);
}


template <class _CharT, class _Alloc>
inline void winx_call Rope<_CharT,_Alloc>::_S_add_to_forest(
	_RopeRep* __r, _RopeRep** __forest, _Alloc& __a)
{
    if (__r->_M_is_balanced) {
	_S_add_leaf_to_forest(__r, __forest, __a);
	return;
    }
    WINX_ASSERT(__r->_M_tag == _RopeRep::_S_concat);
    {
	_RopeConcatenation* __c = (_RopeConcatenation*)__r;

	_S_add_to_forest(__c->_M_left, __forest, __a);
	_S_add_to_forest(__c->_M_right, __forest, __a);
    }
}


template <class _CharT, class _Alloc>
inline void winx_call Rope<_CharT,_Alloc>::_S_add_leaf_to_forest(
	_RopeRep* __r, _RopeRep** __forest, _Alloc& __a)
{
    _RopeRep* __insertee;   		// included in refcount
    _RopeRep* __too_tiny = 0;    	// included in refcount
    int __i;  				// forest[0..__i-1] is empty
    size_t __s = __r->_M_size;

    for (__i = 0; __s >= _S_min_len[__i+1]/* not this bucket */; ++__i) {
	if (0 != __forest[__i]) {
	    __too_tiny = _S_concat_and_set_balanced(__forest[__i], __too_tiny, __a);
	    __forest[__i] = 0;
	}
    }
    {
	__insertee = _S_concat_and_set_balanced(__too_tiny, __r, __a);
    }
    // Too_tiny dead, and no longer included in refcount.
    // Insertee is live and included.
    WINX_ASSERT(_S_is_almost_balanced(__insertee));
    WINX_ASSERT(__insertee->_M_depth <= __r->_M_depth + 1);
    for (;; ++__i) {
	if (0 != __forest[__i]) {
	    __insertee = _S_concat_and_set_balanced(__forest[__i], __insertee, __a);
	    __forest[__i] = 0;
	    WINX_ASSERT(_S_is_almost_balanced(__insertee));
	}
	WINX_ASSERT(_S_min_len[__i] <= __insertee->_M_size);
	WINX_ASSERT(__forest[__i] == 0);
	if (__i == _RopeRep::_S_max_rope_depth || 
	      __insertee->_M_size < _S_min_len[__i+1]) {
	    __forest[__i] = __insertee;
	    // refcount is OK since __insertee is now dead.
	    return;
	}
    }
}

// The following could be implemented trivially using
// lexicographical_compare_3way.
// We do a little more work to avoid dealing with Rope iterators for
// flat strings.
template <class _CharT, class _Alloc>
inline int winx_call Rope<_CharT,_Alloc>::_S_compare (const _RopeRep* __left, 
                                 const _RopeRep* __right)
{
    size_t __left_len;
    size_t __right_len;

    if (0 == __right) return 0 != __left;
    if (0 == __left) return -1;
    __left_len = __left->_M_size;
    __right_len = __right->_M_size;
    if (_RopeRep::_S_leaf == __left->_M_tag) {
	_RopeLeaf* __l = (_RopeLeaf*) __left;
	if (_RopeRep::_S_leaf == __right->_M_tag) {
	    _RopeLeaf* __r = (_RopeLeaf*) __right;
	    return stdext::lexicographical_compare_3way(
			__l->_M_data, __l->_M_data + __left_len,
			__r->_M_data, __r->_M_data + __right_len);
	} else {
	    const_iterator __rstart(__right, 0);
	    const_iterator __rend(__right, __right_len);
	    return stdext::lexicographical_compare_3way(
			__l->_M_data, __l->_M_data + __left_len,
			__rstart, __rend);
	}
    } else {
	const_iterator __lstart(__left, 0);
	const_iterator __lend(__left, __left_len);
	if (_RopeRep::_S_leaf == __right->_M_tag) {
	    _RopeLeaf* __r = (_RopeLeaf*) __right;
	    return stdext::lexicographical_compare_3way(
				   __lstart, __lend,
				   __r->_M_data, __r->_M_data + __right_len);
	} else {
	    const_iterator __rstart(__right, 0);
	    const_iterator __rend(__right, __right_len);
	    return stdext::lexicographical_compare_3way(
				   __lstart, __lend,
				   __rstart, __rend);
	}
    }
}

template <class _CharT, class _Alloc>
inline Rope<_CharT, _Alloc>::Rope(_Alloc& __a, size_t __n, _CharT __c)
	: _M_alloc(&__a)
{
    Rope<_CharT,_Alloc> __result(__a);
    const size_t __exponentiate_threshold = 32;
    size_t __exponent;
    size_t __rest;
    _CharT* __rest_buffer;
    _RopeRep* __remainder;
    Rope<_CharT,_Alloc> __remainder_rope(__a);

    if (0 == __n) {
	  _M_tree_ptr = 0; 
      return;
    }
    
    __exponent = __n / __exponentiate_threshold;
    __rest = __n % __exponentiate_threshold;
    if (0 == __rest) {
		__remainder = 0;
    } else {
		__rest_buffer = STD_ALLOC_ARRAY(__a, _CharT, __rest);
		uninitialized_fill_n(__rest_buffer, __rest, __c);
	    __remainder = _S_new_RopeLeaf(__rest_buffer, __rest, __a);
    }
    __remainder_rope._M_tree_ptr = __remainder;
    if (__exponent != 0) {
	_CharT* __base_buffer = STD_ALLOC_ARRAY(__a, _CharT, __exponentiate_threshold);
	_RopeLeaf* __base_leaf;
	Rope __base_rope(__a);
	uninitialized_fill_n(__base_buffer, __exponentiate_threshold, __c);
    __base_leaf = _S_new_RopeLeaf(__base_buffer,
                                    __exponentiate_threshold, __a);
	__base_rope._M_tree_ptr = __base_leaf;
 	if (1 == __exponent) {
		__result = __base_rope;
	} else {
		__result = stdext::power(
			__base_rope, __exponent, _Rope_Concat_fn<_CharT,_Alloc>(__a));
	}
	if (0 != __remainder) {
	  __result.append(__remainder_rope);
	}
    } else {
	__result = __remainder_rope;
    }
    _M_tree_ptr = __result._M_tree_ptr;
}

NS_STDEXT_END

// -------------------------------------------------------------------------
// $Log: Rope.h,v $

#endif /* STDEXT_TEXT_ROPE_ROPEIMPL_H */
