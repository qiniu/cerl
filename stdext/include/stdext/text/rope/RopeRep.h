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
// Module: stdext/text/rope/RopeRep.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 18:56:07
// 
// $Id: RopeRep.h,v 1.1 2006/10/18 12:13:39 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_ROPE_ROPEREP_H
#define STDEXT_TEXT_ROPE_ROPEREP_H

#define GC_REGISTER_FINALIZER(a0, a1, a2, a3, a4) //@@todo

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------

// CharProducers are logically functions that generate a section of
// a string.  These can be convereted to ropes.  The resulting Rope
// invokes the CharProducer on demand.  This allows, for example,
// files to be viewed as ropes without reading the entire file.
template <class _CharT>
class CharProducer {
    public:
		virtual ~CharProducer() {}
        virtual void operator()(size_t __start_pos, size_t __len, 
                                _CharT* __buffer) = 0;
        // Buffer should really be an arbitrary output iterator.
        // That way we could flatten directly into an ostream, etc.
        // This is thoroughly impossible, since iterator types don't
        // have runtime descriptions.
};

template<class _CharT>
struct _Rope_RopeRep
{
    public:
    enum { _S_max_rope_depth = 45 };
    enum _Tag {_S_leaf, _S_concat, _S_substringfn, _S_function};
	size_t _M_size;
    _Tag _M_tag:8;
    bool _M_is_balanced:8;
    unsigned char _M_depth;

    _Rope_RopeRep(_Tag __t, int __d, bool __b, size_t __size)
        : _M_size(__size),
          _M_tag(__t), _M_is_balanced(__b), _M_depth(__d) {}

	static _CharT _S_fetch(_Rope_RopeRep* __r, size_t __i);
};

template<class _CharT>
struct _Rope_RopeLeaf : public _Rope_RopeRep<_CharT> {
  typedef _Rope_RopeRep<_CharT> _Base;
  public:
  	using _Base::_S_leaf;
  public:
    const _CharT* _M_data;		/* Not necessarily 0 terminated. */
                                /* The allocated size is         */
                                /* _S_rounded_up_size(size), except */
                                /* in the GC case, in which it   */
                                /* doesn't matter.               */
    _Rope_RopeLeaf(const _CharT* __d, size_t __size)
        : _Rope_RopeRep<_CharT>(_S_leaf, 0, true, __size),
          _M_data(__d)
    {
		WINX_ASSERT(__size > 0);
	}
};

template<class _CharT>
struct _Rope_RopeConcatenation : public _Rope_RopeRep<_CharT> {
  typedef _Rope_RopeRep<_CharT> _Base;
  public:
  	using _Base::_S_concat;
  public:
    _Rope_RopeRep<_CharT>* _M_left;
    _Rope_RopeRep<_CharT>* _M_right;
    _Rope_RopeConcatenation(_Rope_RopeRep<_CharT>* __l,
                             _Rope_RopeRep<_CharT>* __r)

      : _Rope_RopeRep<_CharT>(_S_concat,
                                     max(__l->_M_depth, __r->_M_depth) + 1,
                                     false,
                                     __l->_M_size + __r->_M_size),
        _M_left(__l), _M_right(__r)
      {}
};

template<class _CharT>
struct _Rope_RopeFunction : public _Rope_RopeRep<_CharT> {
  typedef _Rope_RopeRep<_CharT> _Base;
  public:
  	using _Base::_S_function;
  public:
    CharProducer<_CharT>* _M_fn;
      // In the GC case, we either register the Rope for
      // finalization, or not.  Thus the field is unnecessary;
      // the information is stored in the collector data structures.
      // We do need a finalization procedure to be invoked by the
      // collector.
      static void _S_fn_finalization_proc(void * __tree, void *) {
        delete ((_Rope_RopeFunction *)__tree) -> _M_fn;
      }
    _Rope_RopeFunction(CharProducer<_CharT>* __f, size_t __size, bool __d)
      : _Rope_RopeRep<_CharT>(_S_function, 0, true, __size)
      , _M_fn(__f)
    {
        WINX_ASSERT(__size > 0);
        if (__d) {
            GC_REGISTER_FINALIZER(
              this, _Rope_RopeFunction::_S_fn_finalization_proc, 0, 0, 0);
        }
    }
};
// Substring results are usually represented using just
// concatenation nodes.  But in the case of very long flat ropes
// or ropes with a functional representation that isn't practical.
// In that case, we represent the __result as a special case of
// RopeFunction, whose CharProducer points back to the Rope itself.
// In all cases except repeated substring operations and
// deallocation, we treat the __result as a RopeFunction.
template<class _CharT>
struct _Rope_RopeSubstring : public _Rope_RopeFunction<_CharT>,
                             public CharProducer<_CharT> {
  typedef _Rope_RopeFunction<_CharT> _Base;
  public:
  	using _Base::_S_function;
  	using _Base::_S_substringfn;
  	using _Base::_S_leaf;
  	using _Base::_M_tag;
	using _Base::_M_fn;
	using _Base::_M_size;
  public:
    // XXX this whole class should be rewritten.
    _Rope_RopeRep<_CharT>* _M_base;      // not 0
    size_t _M_start;
    virtual void operator()(size_t __start_pos, size_t __req_len,
                            _CharT* __buffer) {
        switch(_M_base->_M_tag) {
            case _S_function:
            case _S_substringfn:
              {
                CharProducer<_CharT>* __fn =
                        ((_Rope_RopeFunction<_CharT>*)_M_base)->_M_fn;
                WINX_ASSERT(__start_pos + __req_len <= _M_size);
                WINX_ASSERT(_M_start + _M_size <= _M_base->_M_size);
                (*__fn)(__start_pos + _M_start, __req_len, __buffer);
              }
              break;
            case _S_leaf:
              {
                const _CharT* __s =
                        ((_Rope_RopeLeaf<_CharT>*)_M_base)->_M_data;
                stdext::uninitialized_copy_n(__s + __start_pos + _M_start, __req_len,
                                     __buffer);
              }
              break;
            default:
              WINX_ASSERT(false);
        }
    }

    _Rope_RopeSubstring(_Rope_RopeRep<_CharT>* __b, size_t __s, size_t __l)
      : _Rope_RopeFunction<_CharT>(NULL, __l, false),
        CharProducer<_CharT>(),
        _M_base(__b),
        _M_start(__s)
    {
        WINX_ASSERT(__l > 0);
        WINX_ASSERT(__s + __l <= __b->_M_size);
        _M_tag = _S_substringfn;
		_M_fn = (CharProducer<_CharT>*)this;
    }
};

template <class _CharT>
_CharT _Rope_RopeRep<_CharT>::_S_fetch(_Rope_RopeRep<_CharT>* __r, size_t __i)
{
	typedef _Rope_RopeRep<_CharT> _RopeRep;
	typedef _Rope_RopeConcatenation<_CharT> _RopeConcatenation;
	typedef _Rope_RopeLeaf<_CharT> _RopeLeaf;
	typedef _Rope_RopeFunction<_CharT> _RopeFunction;
	typedef _Rope_RopeSubstring<_CharT> _RopeSubstring;
    WINX_ASSERT(__i < __r->_M_size);
    for(;;) {
      switch(__r->_M_tag) {
	case _RopeRep::_S_concat:
	    {
		_RopeConcatenation* __c = (_RopeConcatenation*)__r;
		_RopeRep* __left = __c->_M_left;
		size_t __left_len = __left->_M_size;

		if (__i >= __left_len) {
		    __i -= __left_len;
		    __r = __c->_M_right;
		} else {
		    __r = __left;
		}
	    }
	    break;
	case _RopeRep::_S_leaf:
	    {
		_RopeLeaf* __l = (_RopeLeaf*)__r;
		return __l->_M_data[__i];
	    }
	case _RopeRep::_S_function:
	case _RopeRep::_S_substringfn:
	    {
		_RopeFunction* __f = (_RopeFunction*)__r;
		_CharT __result;

		(*(__f->_M_fn))(__i, 1, &__result);
		return __result;
	    }
      }
    }
}

// -------------------------------------------------------------------------
// $Log: RopeRep.h,v $

NS_STDEXT_END

#endif /* STDEXT_TEXT_ROPE_ROPEREP_H */
