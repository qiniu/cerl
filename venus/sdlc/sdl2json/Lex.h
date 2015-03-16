/* -------------------------------------------------------------------------
// CERL: C++ Erlang Server Model
// 
// Module: cerl/Lex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Lex.h 2419 2010-04-08 14:00:42Z scm $
// -----------------------------------------------------------------------*/
#ifndef CERL_LEX_H
#define CERL_LEX_H

#ifndef TPL_C_LEX_H
#include <tpl/c/Lex.h>
#endif

#ifndef TPL_REGEXP_H
#include <tpl/RegExp.h>
#endif

#ifndef TR
#define TR	TPL_INFO("TRACE")
#endif

// =========================================================================

using namespace tpl;

typedef impl::Result dom;

extern dom::Document doc;
	extern dom::Mark tagModule;
	extern dom::NodeMark tagSentences;
		extern dom::NodeMark tagCodedef;
			extern dom::Mark tagName;
			extern dom::Mark tagValue;
		extern dom::NodeMark tagTypedef;
			extern dom::Mark tagName;
			extern dom::NodeMark tagType;
		extern dom::NodeMark tagServer;
			extern dom::NodeMark tagSentences;
				extern dom::NodeMark tagConstructor;
					extern dom::NodeMark tagArgs;
				extern dom::NodeMark tagCodedef;
				extern dom::NodeMark tagTypedef;
				extern dom::NodeMark tagFunction;
					extern dom::Mark tagId;
					extern dom::Mark tagAsync;
					extern dom::Mark tagName;
					extern dom::NodeMark tagArgs;
					extern dom::NodeMark tagType;

extern dom::NodeMark tagType;
	extern dom::NodeMark tagNamedType;
		extern dom::Mark tagName;
	extern dom::NodeMark tagStruct;
		extern dom::NodeMark tagVars;
			extern dom::NodeMark tagType;
			extern dom::Mark tagName;
	extern dom::NodeMark tagCodedType;
		extern dom::NodeMark tagItems;
			extern dom::Mark tagCode;
			extern dom::NodeMark tagVars;
				extern dom::NodeMark tagType;
				extern dom::Mark tagName;
	extern dom::NodeMark tagArray;
		extern dom::Mark tagSize;

extern impl::Allocator alloc;
extern impl::MarkedGrammar rType;
extern NS_STDEXT::String serverName;

// -------------------------------------------------------------------------
// common

#define sdl_keyword(kw)				gr(c_symbol()/eq(kw))
#define sdl_keyword2(kw, tag)		gr(c_symbol()/eq(kw)/tag)

#define sdl_symbol_l				(lower() + *c_symbol_next_char())
#define sdl_symbol_u				(upper() + *c_symbol_next_char())

#define sdl_code					gr(sdl_symbol_l/tagCode)
#define sdl_func_name				gr(sdl_symbol_l/tagName)
#define sdl_var_name				gr(sdl_symbol_l/tagName)
#define sdl_type_name				gr(sdl_symbol_u/tagName)
#define sdl_type_name2(var)			gr(sdl_symbol_u/assign(var)/tagName)

// -------------------------------------------------------------------------
// module

#define sdl_module					(sdl_keyword("module") + c_symbol()/tagModule + ';')

// -------------------------------------------------------------------------
// type

//
// var
#define sdl_var						(ref(rType) + sdl_var_name)

//
// coded type
#define sdl_coded_struct			('{' + sdl_code + *(',' + sdl_var/tagVars) + '}')
#define sdl_coded_type_item			(sdl_code | sdl_coded_struct)
#define sdl_coded_type				((sdl_coded_type_item/tagItems % '|')/tagCodedType)

//
// named type
#define sdl_named_type				(sdl_type_name/tagNamedType)

//
// struct
#define sdl_struct					(('{' + sdl_var/tagVars % ','  + '}')/tagStruct)

//
// type
#define sdl_array					(('[' + !gr(c_integer()/tagSize) + ']')/tagArray)
#define sdl_type					(((sdl_coded_type | sdl_named_type | sdl_struct) + !sdl_array)/tagType)

// -------------------------------------------------------------------------
// codedef

#define sdl_codedef					((sdl_keyword("code") + sdl_var_name + '=' + c_integer()/tagValue + ';')/tagCodedef)

// -------------------------------------------------------------------------
// typedef

#define sdl_typedef					((sdl_keyword("type") + sdl_type_name + '=' + ref(rType) + ';')/tagTypedef)

// -------------------------------------------------------------------------
// function

#define sdl_id						(gr("id") + '=' + c_integer()/tagId)
#define sdl_func_attrs				('[' + sdl_id + !(',' + sdl_keyword2("async", tagAsync)) + ']')
#define sdl_function_arg			sdl_var
#define sdl_function_head			(sdl_func_name + '(' + !(sdl_function_arg/tagArgs % ',') + ')')
#define sdl_ret_type				(sdl_coded_type/tagType)
#define sdl_function				((sdl_func_attrs + sdl_function_head + !("->" + sdl_ret_type) + ';')/tagFunction)

// -------------------------------------------------------------------------
// server

#define sdl_constructor_impl		(gr(sdl_symbol_u/eq(serverName)) + '(' + !(sdl_function_arg/tagArgs % ',') + ')')
#define sdl_constructor				((sdl_constructor_impl + ';')/tagConstructor)
#define sdl_server_sentence			(sdl_function | sdl_typedef | sdl_codedef | sdl_constructor | ';')
#define sdl_server_body				(*(sdl_server_sentence/tagSentences))
#define sdl_server					((sdl_keyword("server") + sdl_type_name2(serverName) + '{' + sdl_server_body + '}')/tagServer)

// -------------------------------------------------------------------------
// sentence

#define sdl_gbl_sentence			(sdl_codedef | sdl_typedef | sdl_server | ';')

// -------------------------------------------------------------------------
// doc

#define sdl_doc						(cpp_skip_[ sdl_module + *(sdl_gbl_sentence/tagSentences) ]/doc)

// =========================================================================
// $Log: $

#endif /* CERL_LEX_H */
