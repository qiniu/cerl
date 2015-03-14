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
// Module: stdext/text/rope/CharProxy.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: CharProxy.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_ROPE_CHARPROXY_H
#define STDEXT_TEXT_ROPE_CHARPROXY_H

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class _Rope_char_ref_proxy

// Dereferencing a nonconst iterator has to return something
// that behaves almost like a reference.  It's not possible to
// return an actual reference since assignment requires extra
// work.  And we would get into the same problems as with the
// CD2 version of basic_string.
template<class _CharT, class _Alloc>
class _Rope_char_ref_proxy
{
    friend class _Rope_iterator<_CharT, _Alloc>;
    friend class _Rope_char_ptr_proxy<_CharT,_Alloc>;
    friend class Rope<_CharT,_Alloc>;

    typedef _Rope_RopeRep<_CharT>* _Self_destruct_ptr;
    typedef _Rope_RopeRep<_CharT> _RopeRep;
    typedef Rope<_CharT,_Alloc> _My_rope;

    size_t _M_pos;
    _CharT _M_current;
    bool _M_current_valid;
    _My_rope* _M_root;     // The whole Rope.

public:
	_Rope_char_ref_proxy(_My_rope* __r, size_t __p)
      :  _M_pos(__p), _M_current_valid(false), _M_root(__r) {}
    _Rope_char_ref_proxy(const _Rope_char_ref_proxy& __x)
      : _M_pos(__x._M_pos), _M_current_valid(false), _M_root(__x._M_root) {}
        // Don't preserve cache if the reference can outlive the
        // expression.  We claim that's not possible without calling
        // a copy constructor or generating reference to a proxy
        // reference.  We declare the latter to have undefined semantics.
    _Rope_char_ref_proxy(_My_rope* __r, size_t __p, _CharT __c)
      : _M_pos(__p), _M_current(__c), _M_current_valid(true), _M_root(__r) {}
    
	inline operator _CharT () const;
	inline _Alloc& get_allocator() const {
		return *_M_root->_M_alloc;
	}

	_Rope_char_ptr_proxy<_CharT,_Alloc> operator& () const;
    _Rope_char_ref_proxy& operator= (_CharT __c);
    _Rope_char_ref_proxy& operator= (const _Rope_char_ref_proxy& __c) {
        return operator=((_CharT)__c); 
    }
};

// There is no really acceptable way to handle this.  The default
// definition of swap doesn't work for proxy references.
// It can't really be made to work, even with ugly hacks, since
// the only unusual operation it uses is the copy constructor, which
// is needed for other purposes.  We provide a macro for
// full specializations, and instantiate the most common case.
template<class _CharT, class __Alloc>
inline void winx_call swap(_Rope_char_ref_proxy<_CharT, __Alloc> __a,
                 _Rope_char_ref_proxy<_CharT, __Alloc> __b) {
    _CharT __tmp = __a;
    __a = __b;
    __b = __tmp;
}

// -------------------------------------------------------------------------
// class _Rope_char_ptr_proxy

template<class _CharT, class _Alloc>
class _Rope_char_ptr_proxy {
    // XXX this class should be rewritten.
    friend class _Rope_char_ref_proxy<_CharT,_Alloc>;
    size_t _M_pos;
    Rope<_CharT,_Alloc>* _M_root;     // The whole Rope.
  public:
    _Rope_char_ptr_proxy(const _Rope_char_ref_proxy<_CharT,_Alloc>& __x) 
      : _M_pos(__x._M_pos), _M_root(__x._M_root) {}
    _Rope_char_ptr_proxy(const _Rope_char_ptr_proxy& __x)
      : _M_pos(__x._M_pos), _M_root(__x._M_root) {}
    _Rope_char_ptr_proxy() {}
    _Rope_char_ptr_proxy(_CharT* __x) : _M_root(0), _M_pos(0) {
        WINX_ASSERT(0 == __x);
    }
    _Rope_char_ptr_proxy& 
    operator= (const _Rope_char_ptr_proxy& __x) {
        _M_pos = __x._M_pos;
        _M_root = __x._M_root;
        return *this;
    }
    _Rope_char_ref_proxy<_CharT,_Alloc> operator*() const {
        return _Rope_char_ref_proxy<_CharT,_Alloc>(_M_root, _M_pos);
    }
};

// -------------------------------------------------------------------------
// class _Rope_char_ref_proxy

// Assignment to reference proxies.
template <class _CharT, class _Alloc>
_Rope_char_ref_proxy<_CharT, _Alloc>&
_Rope_char_ref_proxy<_CharT, _Alloc>::operator= (_CharT __c) {
	_Alloc& __a = get_allocator();
    _RopeRep* __old = _M_root->_M_tree_ptr;
    _Self_destruct_ptr __left(
      _My_rope::_S_substring(__old, 0, _M_pos, __a));
    _Self_destruct_ptr __right(
      _My_rope::_S_substring(__old, _M_pos+1, __old->_M_size, __a));
    _Self_destruct_ptr __result_left(
      _My_rope::_S_destr_concat_char_iter(__left, &__c, 1, __a));

    _RopeRep* __result =
		_My_rope::_S_concat(__result_left, __right, __a);
    _M_root->_M_tree_ptr = __result;
    return *this;
}

template <class _CharT, class _Alloc>
inline _Rope_char_ref_proxy<_CharT, _Alloc>::operator _CharT () const
{
    if (_M_current_valid) {
		return _M_current;
    } else {
        return _My_rope::_S_fetch(_M_root->_M_tree_ptr, _M_pos);
    }
}

template <class _CharT, class _Alloc>
inline _Rope_char_ptr_proxy<_CharT, _Alloc>
_Rope_char_ref_proxy<_CharT, _Alloc>::operator& () const {
    return _Rope_char_ptr_proxy<_CharT, _Alloc>(*this);
}

// -------------------------------------------------------------------------
// class _Rope_char_ptr_proxy

template <class _CharT, class _Alloc>
inline bool operator== (const _Rope_char_ptr_proxy<_CharT,_Alloc>& __x,
                        const _Rope_char_ptr_proxy<_CharT,_Alloc>& __y) {
        return (__x._M_pos == __y._M_pos && __x._M_root == __y._M_root);
}

template <class _CharT, class _Alloc>
inline bool operator!= (const _Rope_char_ptr_proxy<_CharT,_Alloc>& __x,
                        const _Rope_char_ptr_proxy<_CharT,_Alloc>& __y) {
  return !(__x == __y);
}

// -------------------------------------------------------------------------
// $Log: CharProxy.h,v $

NS_STDEXT_END

#endif /* STDEXT_TEXT_ROPE_CHARPROXY_H */
