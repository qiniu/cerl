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
#ifndef STDEXT_TEXT_ROPE_ROPE_H
#define STDEXT_TEXT_ROPE_ROPE_H

#ifndef __SGI_NUMERIC_H__
#include "../../sgi/numeric.h"
#endif

#ifndef __SGI_ALGORITHM_H__
#include "../../sgi/algorithm.h"
#endif

#ifndef __SGI_FUNCTIONAL_H__
#include "../../sgi/functional.h"
#endif

#ifndef STDEXT_TEXT_ROPE_ROPEREP_H
#include "RopeRep.h"
#endif

#ifndef __STL_TRY
#define __STL_TRY try
#define __STL_CATCH_ALL catch(...)
#define __STL_THROW(x) throw x
#define __STL_RETHROW throw
#define __STL_NOTHROW throw()
#define __STL_UNWIND(action) catch(...) { action; throw; }
#endif

/*
 * Copyright (c) 1997-1998
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

// Rope<_CharT,_Alloc> is a sequence of _CharT.
// Ropes appear to be mutable, but update operations
// really copy enough of the data structure to leave the original
// valid.  Thus ropes can be logically copied by just copying
// a pointer value.

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class _Rope_insert_char_consumer

template <class _CharT, class _Insert_ostream>
class _Rope_insert_char_consumer
{
private:
	_Insert_ostream& _M_o;

public:
	_Rope_insert_char_consumer(_Insert_ostream& __writer) 
	  : _M_o(__writer) {}

	bool winx_call operator()(const _CharT* __leaf, size_t __n)
	{
		_M_o.write(__leaf, __n);
		return true;
	}
};

// -------------------------------------------------------------------------
// class Rope

template<class _CharT, class _Alloc> class _Rope_char_ref_proxy;
template<class _CharT, class _Alloc> class _Rope_char_ptr_proxy;
template<class _CharT, class _Alloc> class _Rope_iterator;
template<class _CharT> class _Rope_const_iterator;

template <class _CharT, class _Alloc = DefaultAlloc>
class Rope {
    public:
        typedef _CharT value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef _CharT const_reference;
        typedef const _CharT* const_pointer;
        typedef _Rope_iterator<_CharT, _Alloc> iterator;
        typedef _Rope_const_iterator<_CharT> const_iterator;
        typedef _Rope_char_ref_proxy<_CharT,_Alloc> reference;
        typedef _Rope_char_ptr_proxy<_CharT,_Alloc> pointer;

        friend struct _Rope_RopeRep<_CharT>;
        friend struct _Rope_RopeSubstring<_CharT>;

		friend class _Rope_const_iterator<_CharT>;
		friend class _Rope_iterator<_CharT, _Alloc>;
        friend class _Rope_char_ptr_proxy<_CharT,_Alloc>;
        friend class _Rope_char_ref_proxy<_CharT,_Alloc>;

    protected:
        _Rope_RopeRep<_CharT>* _M_tree_ptr;
		_Alloc* _M_alloc;

        enum { _S_copy_max = 23 };
                // For strings shorter than _S_copy_max, we copy to
                // concatenate.

        typedef _Rope_RopeRep<_CharT> _RopeRep;
        typedef _Rope_RopeConcatenation<_CharT> _RopeConcatenation;
        typedef _Rope_RopeLeaf<_CharT> _RopeLeaf;
        typedef _Rope_RopeFunction<_CharT> _RopeFunction;
        typedef _Rope_RopeSubstring<_CharT> _RopeSubstring;

        // Retrieve a character at the indicated position.
        static _CharT winx_call _S_fetch(_RopeRep* __r, size_type __pos) {
			return _RopeRep::_S_fetch(__r, __pos);
		}

		typedef _Rope_RopeRep<_CharT>* _Self_destruct_ptr;

        // _Result is counted in refcount.
		static _RopeRep* winx_call _S_substring(_RopeRep* __base,
                                    size_t __start, size_t __endp1, _Alloc& __a);

        static _RopeRep* winx_call _S_concat_char_iter(_RopeRep* __r,
                                          const _CharT* __iter, size_t __slen, _Alloc& __a);
                // Concatenate Rope and char ptr, copying __s.
                // Should really take an arbitrary iterator.
                // Result is counted in refcount.
        static _RopeRep* winx_call _S_destr_concat_char_iter(_RopeRep* __r,
                                          const _CharT* __iter, size_t __slen, _Alloc& __a)
                // As above, but one reference to __r is about to be
                // destroyed.  Thus the pieces may be recycled if all
                // relevent reference counts are 1.
                { return _S_concat_char_iter(__r, __iter, __slen, __a); }

        static _RopeRep* winx_call _S_concat(_RopeRep* __left, _RopeRep* __right, _Alloc& __a);
                // General concatenation on _RopeRep.  _Result
                // has refcount of 1.  Adjusts argument refcounts.

		template <class _RopeCharConsumer>
        static bool winx_call _S_apply_to_pieces(
            _Alloc& __a, _RopeCharConsumer& __c, 
			const _RopeRep* __r, size_t __begin, size_t __end)
		{
			if (0 == __r)
				return true;
			switch(__r->_M_tag)
			{
			case _RopeRep::_S_concat:
				{
					_RopeConcatenation* __conc = (_RopeConcatenation*)__r;
					_RopeRep* __left =  __conc->_M_left;
					size_t __left_len = __left->_M_size;
					if (__begin < __left_len) {
						size_t __left_end = min(__left_len, __end);
						if (!_S_apply_to_pieces(__a, __c, __left, __begin, __left_end))
							return false;
					}
					if (__end > __left_len) {
						_RopeRep* __right =  __conc->_M_right;
						size_t __right_start = max(__left_len, __begin);
						if (!_S_apply_to_pieces(__a, __c, __right,
							__right_start - __left_len,
							__end - __left_len)) {
							return false;
						}
					}
				}
				return true;
			case _RopeRep::_S_leaf:
				{
					_RopeLeaf* __l = (_RopeLeaf*)__r;
					return __c(__l->_M_data + __begin, __end - __begin);
				}
			case _RopeRep::_S_function:
			case _RopeRep::_S_substringfn:
				{
					_RopeFunction* __f = (_RopeFunction*)__r;
					size_t __len = __end - __begin;
					bool __result;
					_CharT* __buffer = STD_ALLOC_ARRAY(__a, _CharT, __len);
					__STL_TRY {
						(*(__f->_M_fn))(__begin, __len, __buffer);
						__result = __c(__buffer, __len);
					}
					__STL_UNWIND(__result=false)
					return __result;
				}
			default:
				WINX_ASSERT(false); /*NOTREACHED*/
				return false;
			}
		}

   public:
		template <class _RopeCharConsumer>
		void winx_call apply_to_pieces(
			size_t __begin, size_t __end, _RopeCharConsumer& __c) const
		{
			_S_apply_to_pieces(*_M_alloc, __c, _M_tree_ptr, __begin, __end);
		}

		template <class _RopeCharConsumer>
		void winx_call apply_to_pieces(_RopeCharConsumer& __c) const
		{
			_S_apply_to_pieces(*_M_alloc, __c, _M_tree_ptr, 0, size());
		}

   public:
		template <class _OStream>
		_OStream& winx_call trace(_OStream& __os) const
		{
			_Rope_insert_char_consumer<_CharT, _OStream> __consumer(__os);
			apply_to_pieces(__consumer);
			return __os;
		}

   protected:
        // Allocate and construct a RopeLeaf using the supplied allocator
        // Takes ownership of s instead of copying.
        static _RopeLeaf* winx_call _S_new_RopeLeaf(const _CharT *__s,
                                          size_t __size, _Alloc& __a)
        {
			return STD_NEW(__a, _RopeLeaf)(__s, __size);
        }

        static _RopeConcatenation* winx_call _S_new_RopeConcatenation(
                        _RopeRep* __left, _RopeRep* __right,
                        _Alloc& __a)
        {
            return STD_NEW(__a, _RopeConcatenation)(__left, __right);
        }

        static _RopeFunction* winx_call _S_new_RopeFunction(CharProducer<_CharT>* __f,
                size_t __size, bool __d, _Alloc& __a)
        {
            return STD_NEW(__a, _RopeFunction)(__f, __size, __d);
        }

        static _RopeSubstring* winx_call _S_new_RopeSubstring(
                _Rope_RopeRep<_CharT>* __b, size_t __s,
                size_t __l, _Alloc& __a)
        {
            return STD_NEW(__a, _RopeSubstring)(__b, __s, __l);
        }

#define __STL_ROPE_FROM_UNOWNED_CHAR_PTR(__s, __size, __a)	\
	_S_RopeLeaf_from_unowned_char_ptr(__s, __size, __a)     

		static _RopeLeaf* winx_call _S_RopeLeaf_from_unowned_char_ptr(const _CharT *__s,
                       size_t __size, _Alloc& __a)
        {
            if (0 == __size) return 0;
            _CharT* __buf = STD_ALLOC_ARRAY(__a, _CharT, __size);
            stdext::uninitialized_copy_n(__s, __size, __buf);
            return _S_new_RopeLeaf(__buf, __size, __a);
        }
            

        // Concatenation of nonempty strings.
        // Always builds a concatenation node.
        // Rebalances if the result is too deep.
        // Result has refcount 1.
        // Does not increment left and right ref counts even though
        // they are referenced.
        static _RopeRep*
        winx_call _S_tree_concat(_RopeRep* __left, _RopeRep* __right, _Alloc& __a);

        // Concatenation helper functions
        static _RopeLeaf*
        winx_call _S_leaf_concat_char_iter(_RopeLeaf* __r,
                                 const _CharT* __iter, size_t __slen, _Alloc& __a);
                // Concatenate by copying leaf.
                // should take an arbitrary iterator
                // result has refcount 1.

        private:

        Rope(_Alloc& __a, _RopeRep* __t)
          : _M_tree_ptr(__t), _M_alloc(&__a) {}


        // Copy __r to the _CharT buffer.
        // Returns __buffer + __r->_M_size.
        // Assumes that buffer is uninitialized.
        static _CharT* winx_call _S_flatten(_RopeRep* __r, _CharT* __buffer);

        // Again, with explicit starting position and length.
        // Assumes that buffer is uninitialized.
        static _CharT* winx_call _S_flatten(_RopeRep* __r,
                                  size_t __start, size_t __len,
                                  _CharT* __buffer, _Alloc& __a);

        static const unsigned long 
          _S_min_len[_RopeRep::_S_max_rope_depth + 1];

        static bool winx_call _S_is_balanced(_RopeRep* __r)
                { return (__r->_M_size >= _S_min_len[__r->_M_depth]); }

        static bool winx_call _S_is_almost_balanced(_RopeRep* __r)
                { return (__r->_M_depth == 0 ||
                          __r->_M_size >= _S_min_len[__r->_M_depth - 1]); }

        static bool winx_call _S_is_roughly_balanced(_RopeRep* __r)
                { return (__r->_M_depth <= 1 ||
                          __r->_M_size >= _S_min_len[__r->_M_depth - 2]); }

        // Assumes the result is not empty.
        static _RopeRep* winx_call _S_concat_and_set_balanced(_RopeRep* __left,
                                                     _RopeRep* __right, _Alloc& __a)
        {
            _RopeRep* __result = _S_concat(__left, __right, __a);
            if (_S_is_balanced(__result)) __result->_M_is_balanced = true;
            return __result;
        }

        // The basic rebalancing operation.  Logically copies the
        // Rope.  The result has refcount of 1.  The client will
        // usually decrement the reference count of __r.
        // The result is within height 2 of balanced by the above
        // definition.
        static _RopeRep* winx_call _S_balance(_RopeRep* __r, _Alloc& __a);

        // Add all unbalanced subtrees to the forest of balanceed trees.
        // Used only by balance.
        static void winx_call _S_add_to_forest(_RopeRep*__r, _RopeRep** __forest, _Alloc& __a);
        
        // Add __r to forest, assuming __r is already balanced.
        static void winx_call _S_add_leaf_to_forest(_RopeRep* __r, _RopeRep** __forest, _Alloc& __a);

        // Print to stdout, exposing structure
        static void winx_call _S_dump(_RopeRep* __r, int __indent = 0);

        // Return -1, 0, or 1 if __x < __y, __x == __y, or __x > __y resp.
        static int winx_call _S_compare(const _RopeRep* __x, const _RopeRep* __y);

   public:
        bool winx_call empty() const { return 0 == _M_tree_ptr; }

        // Comparison member function.  This is public only for those
        // clients that need a ternary comparison.  Others
        // should use the comparison operators below.
        int winx_call compare(const Rope& __y) const {
            return _S_compare(_M_tree_ptr, __y._M_tree_ptr);
        }

		int winx_call compare(const BasicString<_CharT>& s) const { //@@todo
			const Rope b(*_M_alloc, s.begin(), s.end());
			return compare(b);
		}

        Rope(_Alloc& __a, const _CharT* __s)
        	: _M_tree_ptr(__STL_ROPE_FROM_UNOWNED_CHAR_PTR(__s, std::length(__s), __a)),
          	  _M_alloc(&__a)
        { }

        Rope(_Alloc& __a, const _CharT* __s, size_t __len)
        	: _M_tree_ptr(__STL_ROPE_FROM_UNOWNED_CHAR_PTR(__s, __len, __a)),
          	  _M_alloc(&__a)
        { }

        // Should perhaps be templatized with respect to the iterator type
        // and use Sequence_buffer.  (It should perhaps use sequence_buffer
        // even now.)
        Rope(_Alloc& __a, const _CharT *__s, const _CharT *__e)
        	: _M_tree_ptr(__STL_ROPE_FROM_UNOWNED_CHAR_PTR(__s, __e - __s, __a)),
          	  _M_alloc(&__a)
        { }

        Rope(_Alloc& __a, const const_iterator& __s, const const_iterator& __e)
        	: _M_tree_ptr(_S_substring(__s._M_root, __s._M_current_pos,
                             __e._M_current_pos)),
          	  _M_alloc(&__a)
        { }

        Rope(_Alloc& __a, const iterator& __s, const iterator& __e)
        	: _M_tree_ptr(_S_substring(__s._M_root, __s._M_current_pos, __e._M_current_pos)),
          	  _M_alloc(&__a)
        { }

        Rope(_Alloc& __a, _CharT __c)
        	: _M_alloc(&__a)
        {
            _CharT* __buf = STD_ALLOC_ARRAY(__a, _CharT, 1);
            construct(__buf, __c);
			_M_tree_ptr = _S_new_RopeLeaf(__buf, 1, __a);
        }

        Rope(_Alloc& __a, size_t __n, _CharT __c);

        Rope(_Alloc& __a)
			: _M_tree_ptr(0), _M_alloc(&__a) {}

        // Construct a Rope from a function that can compute its members
        Rope(_Alloc& __a, CharProducer<_CharT> *__fn, size_t __len, bool __delete_fn)
        	: _M_alloc(&__a)
        {
            _M_tree_ptr = (0 == __len) ?
               0 : _S_new_RopeFunction(__fn, __len, __delete_fn, __a);
        }

		Rope& winx_call operator=(const BasicString<_CharT>& s)
		{
			Rope b(*_M_alloc, s.begin(), s.end());
			return operator=(b);
		}

        void winx_call clear()
        {
            _M_tree_ptr = 0;
        }

        void winx_call push_back(_CharT __x)
        {
            //_RopeRep* __old = _M_tree_ptr;
            _M_tree_ptr = _S_destr_concat_char_iter(_M_tree_ptr, &__x, 1, *_M_alloc);
        }

        void winx_call pop_back()
        {
            //_RopeRep* __old = _M_tree_ptr;
            _M_tree_ptr = 
              _S_substring(_M_tree_ptr, 0, _M_tree_ptr->_M_size - 1);
        }

        _CharT winx_call back() const
        {
            return _S_fetch(_M_tree_ptr, _M_tree_ptr->_M_size - 1);
        }

        void winx_call push_front(_CharT __x)
        {
            //_RopeRep* __old = _M_tree_ptr;
            _RopeRep* __left =
				__STL_ROPE_FROM_UNOWNED_CHAR_PTR(&__x, 1, *_M_alloc);
			_M_tree_ptr = _S_concat(__left, _M_tree_ptr, *_M_alloc);
        }

        void winx_call pop_front()
        {
            _RopeRep* __old = _M_tree_ptr;
            _M_tree_ptr = _S_substring(_M_tree_ptr, 1, _M_tree_ptr->_M_size);
        }

        _CharT winx_call front() const
        {
            return _S_fetch(_M_tree_ptr, 0);
        }

        void winx_call balance()
        {
            _RopeRep* __old = _M_tree_ptr;
            _M_tree_ptr = _S_balance(_M_tree_ptr, *_M_alloc);
        }

        _CharT* winx_call copy(_CharT* __buffer) const {
			std::destroyArray(__buffer, size());
            return _S_flatten(_M_tree_ptr, __buffer);
        }

        // This is the copy function from the standard, but
        // with the arguments reordered to make it consistent with the
        // rest of the interface.
        // Note that this guaranteed not to compile if the draft standard
        // order is assumed.
        _CharT* winx_call copy(size_type __pos, size_type __n, _CharT* __buffer) const 
        {
            size_t __size = size();
            size_t __len = (__pos + __n > __size? __size - __pos : __n);

            std::destroyArray(__buffer, __len);
            return _S_flatten(_M_tree_ptr, __pos, __len, __buffer, *_M_alloc);
        }

        // Print to stdout, exposing structure.  May be useful for
        // performance debugging.
        void winx_call dump() {
            _S_dump(_M_tree_ptr);
        }

        _CharT winx_call operator[] (size_type __pos) const {
            return _S_fetch(_M_tree_ptr, __pos);
        }

        _CharT winx_call at(size_type __pos) const {
           // if (__pos >= size()) throw out_of_range;  // XXX
           return (*this)[__pos];
        }

        const_iterator winx_call begin() const {
            return(const_iterator(_M_tree_ptr, 0));
        }

        // An easy way to get a const iterator from a non-const container.
        const_iterator winx_call const_begin() const {
            return(const_iterator(_M_tree_ptr, 0));
        }

        const_iterator winx_call end() const {
            return(const_iterator(_M_tree_ptr, size()));
        }

        const_iterator winx_call const_end() const {
            return(const_iterator(_M_tree_ptr, size()));
        }

        size_type winx_call size() const { 
            return(0 == _M_tree_ptr? 0 : _M_tree_ptr->_M_size);
        }

        size_type winx_call length() const {
            return size();
        }

        size_type winx_call max_size() const {
            return _S_min_len[_RopeRep::_S_max_rope_depth-1] - 1;
            //  Guarantees that the result can be sufficirntly
            //  balanced.  Longer ropes will probably still work,
            //  but it's harder to make guarantees.
        }

#     if !defined(WINX_NO_PARTIAL_SPECIALIZATION)
        typedef reverse_iterator<const_iterator> const_reverse_iterator;
#     else
        typedef reverse_iterator<const_iterator, value_type, const_reference,
                                 difference_type>  const_reverse_iterator;
#     endif

        const_reverse_iterator winx_call rbegin() const {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator winx_call const_rbegin() const {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator winx_call rend() const {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator winx_call const_rend() const {
            return const_reverse_iterator(begin());
        }

        // The symmetric cases are intentionally omitted, since they're presumed
        // to be less common, and we don't handle them as well.

        // The following should really be templatized.
        // The first argument should be an input iterator or
        // forward iterator with value_type _CharT.
        Rope& winx_call append(const _CharT* __iter, size_t __n) {
            _RopeRep* __result = 
              _S_destr_concat_char_iter(_M_tree_ptr, __iter, __n, *_M_alloc);
            _M_tree_ptr = __result;
            return *this;
        }

        Rope& winx_call append(const _CharT* __c_string) {
            size_t __len = std::length(__c_string);
            append(__c_string, __len);
            return(*this);
        }

        Rope& winx_call append(const _CharT* __s, const _CharT* __e) {
            _RopeRep* __result =
                _S_destr_concat_char_iter(_M_tree_ptr, __s, __e - __s, *_M_alloc);
            _M_tree_ptr = __result;
            return *this;
        }

        Rope& winx_call append(const_iterator __s, const_iterator __e) {
            WINX_ASSERT(__s._M_root == __e._M_root);
            _Self_destruct_ptr __appendee(_S_substring(
              __s._M_root, __s._M_current_pos, __e._M_current_pos));
            _RopeRep* __result = 
              _S_concat(_M_tree_ptr, (_RopeRep*)__appendee, *_M_alloc);
            _M_tree_ptr = __result;
            return *this;
        }

        Rope& winx_call append(_CharT __c) {
            _RopeRep* __result = 
              _S_destr_concat_char_iter(_M_tree_ptr, &__c, 1);
            _M_tree_ptr = __result;
            return *this;
        }

        Rope& winx_call append(const Rope& __y) {
            _RopeRep* __result = _S_concat(_M_tree_ptr, __y._M_tree_ptr, *_M_alloc);
            _M_tree_ptr = __result;
            return *this;
        }

        Rope& winx_call append(size_t __n, _CharT __c) {
            Rope<_CharT,_Alloc> __last(*_M_alloc, __n, __c);
            return append(__last);
        }

        void winx_call swap(Rope& __b) {
			std::swap(_M_alloc, __b._M_alloc);
			std::swap(_M_tree_ptr, __b._M_tree_ptr);
        }

    protected:
        // Result is included in refcount.
        static _RopeRep* winx_call replace(_RopeRep* __old, size_t __pos1,
                                  size_t __pos2, _RopeRep* __r, _Alloc& __a) {
            if (0 == __old) { return __r; }
            _Self_destruct_ptr __left(
              _S_substring(__old, 0, __pos1, __a));
            _Self_destruct_ptr __right(
              _S_substring(__old, __pos2, __old->_M_size, __a));
            _RopeRep* __result;

            if (0 == __r) {
                __result = _S_concat(__left, __right, __a);
            } else {
                _Self_destruct_ptr __left_result(_S_concat(__left, __r, __a));
                __result = _S_concat(__left_result, __right, __a);
            }
            return __result;
        }

    public:
        void winx_call insert(size_t __p, const Rope& __r) {
            _RopeRep* __result = 
				replace(_M_tree_ptr, __p, __p, __r._M_tree_ptr);
            _M_tree_ptr = __result;
        }

        void winx_call insert(size_t __p, size_t __n, _CharT __c) {
            Rope<_CharT,_Alloc> __r(__n,__c);
            insert(__p, __r);
        }

        void winx_call insert(size_t __p, const _CharT* __i, size_t __n) {
            _Self_destruct_ptr __left(_S_substring(_M_tree_ptr, 0, __p));
            _Self_destruct_ptr __right(_S_substring(_M_tree_ptr, __p, size()));
            _Self_destruct_ptr __left_result(
              _S_concat_char_iter(__left, __i, __n));
                // _S_ destr_concat_char_iter should be safe here.
                // But as it stands it's probably not a win, since __left
                // is likely to have additional references.
            _RopeRep* __result = _S_concat(__left_result, __right, *_M_alloc);
            _M_tree_ptr = __result;
        }

        void winx_call insert(size_t __p, const _CharT* __c_string) {
            insert(__p, __c_string, std::length(__c_string));
        }

        void winx_call insert(size_t __p, _CharT __c) {
            insert(__p, &__c, 1);
        }

        void winx_call insert(size_t __p) {
            _CharT __c = _CharT();
            insert(__p, &__c, 1);
        }

        void winx_call insert(size_t __p, const _CharT* __i, const _CharT* __j) {
            Rope __r(__i, __j);
            insert(__p, __r);
        }

        void winx_call insert(size_t __p, const const_iterator& __i,
                              const const_iterator& __j) {
            Rope __r(__i, __j);
            insert(__p, __r);
        }

        void winx_call insert(size_t __p, const iterator& __i,
                              const iterator& __j) {
            Rope __r(__i, __j);
            insert(__p, __r);
        }

        // (position, length) versions of replace operations:

        void winx_call replace(size_t __p, size_t __n, const Rope& __r) {
            _RopeRep* __result = 
              replace(_M_tree_ptr, __p, __p + __n, __r._M_tree_ptr);
            _M_tree_ptr = __result;
        }

        void winx_call replace(size_t __p, size_t __n, 
                     const _CharT* __i, size_t __i_len) {
            Rope __r(__i, __i_len);
            replace(__p, __n, __r);
        }

        void winx_call replace(size_t __p, size_t __n, _CharT __c) {
            Rope __r(__c);
            replace(__p, __n, __r);
        }

        void winx_call replace(size_t __p, size_t __n, const _CharT* __c_string) {
            Rope __r(__c_string);
            replace(__p, __n, __r);
        }

        void winx_call replace(size_t __p, size_t __n, 
                     const _CharT* __i, const _CharT* __j) {
            Rope __r(__i, __j);
            replace(__p, __n, __r);
        }

        void winx_call replace(size_t __p, size_t __n,
                     const const_iterator& __i, const const_iterator& __j) {
            Rope __r(__i, __j);
            replace(__p, __n, __r);
        }

        void winx_call replace(size_t __p, size_t __n,
                     const iterator& __i, const iterator& __j) {
            Rope __r(__i, __j);
            replace(__p, __n, __r);
        }

        // Single character variants:
        void winx_call replace(size_t __p, _CharT __c) {
            iterator __i(this, __p);
            *__i = __c;
        }

        void winx_call replace(size_t __p, const Rope& __r) {
            replace(__p, 1, __r);
        }

        void winx_call replace(size_t __p, const _CharT* __i, size_t __i_len) {
            replace(__p, 1, __i, __i_len);
        }

        void winx_call replace(size_t __p, const _CharT* __c_string) {
            replace(__p, 1, __c_string);
        }

        void winx_call replace(size_t __p, const _CharT* __i, const _CharT* __j) {
            replace(__p, 1, __i, __j);
        }

        void winx_call replace(size_t __p, const const_iterator& __i,
                               const const_iterator& __j) {
            replace(__p, 1, __i, __j);
        }

        void winx_call replace(size_t __p, const iterator& __i,
                               const iterator& __j) {
            replace(__p, 1, __i, __j);
        }

        // Erase, (position, size) variant.
        void winx_call erase(size_t __p, size_t __n) {
            _RopeRep* __result = replace(_M_tree_ptr, __p, __p + __n, 0, *_M_alloc);
            _M_tree_ptr = __result;
        }

        // Erase, single character
        void winx_call erase(size_t __p) {
            erase(__p, __p + 1);
        }

        // Insert, iterator variants.  
        iterator winx_call insert(const iterator& __p, const Rope& __r)
                { insert(__p.index(), __r); return __p; }
        iterator winx_call insert(const iterator& __p, size_t __n, _CharT __c)
                { insert(__p.index(), __n, __c); return __p; }
        iterator winx_call insert(const iterator& __p, _CharT __c) 
                { insert(__p.index(), __c); return __p; }
        iterator winx_call insert(const iterator& __p ) 
                { insert(__p.index()); return __p; }
        iterator winx_call insert(const iterator& __p, const _CharT* c_string) 
                { insert(__p.index(), c_string); return __p; }
        iterator winx_call insert(const iterator& __p, const _CharT* __i, size_t __n)
                { insert(__p.index(), __i, __n); return __p; }
        iterator winx_call insert(const iterator& __p, const _CharT* __i, 
                        const _CharT* __j)
                { insert(__p.index(), __i, __j);  return __p; }
        iterator winx_call insert(const iterator& __p,
                        const const_iterator& __i, const const_iterator& __j)
                { insert(__p.index(), __i, __j); return __p; }
        iterator winx_call insert(const iterator& __p,
                        const iterator& __i, const iterator& __j)
                { insert(__p.index(), __i, __j); return __p; }

        // Replace, range variants.
        void winx_call replace(const iterator& __p, const iterator& __q,
                     const Rope& __r)
                { replace(__p.index(), __q.index() - __p.index(), __r); }
        void winx_call replace(const iterator& __p, const iterator& __q, _CharT __c)
                { replace(__p.index(), __q.index() - __p.index(), __c); }
        void winx_call replace(const iterator& __p, const iterator& __q,
                     const _CharT* __c_string)
                { replace(__p.index(), __q.index() - __p.index(), __c_string); }
        void winx_call replace(const iterator& __p, const iterator& __q,
                     const _CharT* __i, size_t __n)
                { replace(__p.index(), __q.index() - __p.index(), __i, __n); }
        void winx_call replace(const iterator& __p, const iterator& __q,
                     const _CharT* __i, const _CharT* __j)
                { replace(__p.index(), __q.index() - __p.index(), __i, __j); }
        void winx_call replace(const iterator& __p, const iterator& __q,
                     const const_iterator& __i, const const_iterator& __j)
                { replace(__p.index(), __q.index() - __p.index(), __i, __j); }
        void winx_call replace(const iterator& __p, const iterator& __q,
                     const iterator& __i, const iterator& __j)
                { replace(__p.index(), __q.index() - __p.index(), __i, __j); }

        // Replace, iterator variants.
        void winx_call replace(const iterator& __p, const Rope& __r)
                { replace(__p.index(), __r); }
        void winx_call replace(const iterator& __p, _CharT __c)
                { replace(__p.index(), __c); }
        void winx_call replace(const iterator& __p, const _CharT* __c_string)
                { replace(__p.index(), __c_string); }
        void winx_call replace(const iterator& __p, const _CharT* __i, size_t __n)
                { replace(__p.index(), __i, __n); }
        void winx_call replace(const iterator& __p, const _CharT* __i, const _CharT* __j)
                { replace(__p.index(), __i, __j); }
        void winx_call replace(const iterator& __p, const_iterator __i, 
                     const_iterator __j)
                { replace(__p.index(), __i, __j); }
        void winx_call replace(const iterator& __p, iterator __i, iterator __j)
                { replace(__p.index(), __i, __j); }

        // Iterator and range variants of erase
        iterator winx_call erase(const iterator& __p, const iterator& __q) {
            size_t __p_index = __p.index();
            erase(__p_index, __q.index() - __p_index);
            return iterator(this, __p_index);
        }
        iterator winx_call erase(const iterator& __p) {
            size_t __p_index = __p.index();
            erase(__p_index, 1);
            return iterator(this, __p_index);
        }

        Rope winx_call substr(size_t __start, size_t __len = 1) const {
            return Rope<_CharT,_Alloc>(
                        _S_substring(_M_tree_ptr, __start, __start + __len));
        }

        Rope winx_call substr(iterator __start, iterator __end) const {
            return Rope<_CharT,_Alloc>(
                _S_substring(_M_tree_ptr, __start.index(), __end.index()));
        }
        
        Rope winx_call substr(iterator __start) const {
            size_t __pos = __start.index();
            return Rope<_CharT,_Alloc>(
                        _S_substring(_M_tree_ptr, __pos, __pos + 1));
        }
        
        Rope winx_call substr(const_iterator __start, const_iterator __end) const {
            // This might eventually take advantage of the cache in the
            // iterator.
            return Rope<_CharT,_Alloc>(
              _S_substring(_M_tree_ptr, __start.index(), __end.index()));
        }

        Rope<_CharT,_Alloc> winx_call substr(const_iterator __start) {
            size_t __pos = __start.index();
            return Rope<_CharT,_Alloc>(
              _S_substring(_M_tree_ptr, __pos, __pos + 1));
        }

        static const size_type npos;

        size_type winx_call find(_CharT __c, size_type __pos = 0) const;
        size_type winx_call find(const _CharT* __s, size_type __pos = 0) const {
            size_type __result_pos;
            const_iterator __result = search(const_begin() + __pos, const_end(),
				__s, __s + std::length(__s));
            __result_pos = __result.index();
#           ifndef __STL_OLD_ROPE_SEMANTICS
                if (__result_pos == size()) __result_pos = npos;
#           endif
            return __result_pos;
        }

        iterator winx_call mutable_begin() {
            return(iterator(this, 0));
        }

        iterator winx_call mutable_end() {
            return(iterator(this, size()));
        }

#     if !defined(WINX_NO_PARTIAL_SPECIALIZATION)
        typedef reverse_iterator<iterator> reverse_iterator;
#     else
        typedef reverse_iterator<iterator, value_type, reference,
                                 difference_type>  reverse_iterator;
#     endif

        reverse_iterator winx_call mutable_rbegin() {
            return reverse_iterator(mutable_end());
        }

        reverse_iterator winx_call mutable_rend() {
            return reverse_iterator(mutable_begin());
        }

        reference winx_call mutable_reference_at(size_type __pos) {
            return reference(this, __pos);
        }

        void winx_call reserve(size_type __res_arg = 0) {}

        size_type winx_call capacity() const {
            return max_size();
        }
};

template <class _CharT, class _Alloc>
const typename Rope<_CharT, _Alloc>::size_type Rope<_CharT, _Alloc>::npos =
                        (size_type)(-1);

NS_STDEXT_END

#ifndef STDEXT_TEXT_ROPE_ROPEITER_H
#include "RopeIter.h"
#endif

#ifndef STDEXT_TEXT_ROPE_ROPEIMPL_H
#include "RopeImpl.h"
#endif

#ifndef STDEXT_TEXT_ROPE_CHARPROXY_H
#include "CharProxy.h"
#endif

// -------------------------------------------------------------------------
// $Log: Rope.h,v $

#endif /* STDEXT_TEXT_ROPE_ROPE_H */

