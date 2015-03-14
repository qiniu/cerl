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
// Module: stdext/msvcrt/specstrings.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-7 15:25:27
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_MSVCRT_SPECSTRINGS_H
#define STDEXT_MSVCRT_SPECSTRINGS_H

// -------------------------------------------------------------------------

#if !defined(__specstrings)

#define __null
#define __notnull
#define __maybenull
#define __readonly
#define __notreadonly
#define __maybereadonly
#define __valid
#define __notvalid
#define __maybevalid
#define __readableTo(extent)
#define __elem_readableTo(size)
#define __byte_readableTo(size)
#define __writableTo(size)
#define __elem_writableTo(size)
#define __byte_writableTo(size)
#define __deref
#define __pre
#define __post
#define __precond(expr)
#define __postcond(expr)
#define __exceptthat
#define __inner_success(expr)
#define __inner_checkReturn
#define __inner_typefix(ctype)
#define __inner_override
#define __inner_callback
#define __inner_blocksOn(resource)
#define __inner_fallthrough_dec
#define __inner_fallthrough
#define __refparam
#define __inner_control_entrypoint(category)
#define __inner_data_entrypoint(category)

#define __ecount(size)
#define __bcount(size)
#define __in
#define __in_ecount(size)
#define __in_bcount(size)
#define __out
#define __out_z
#define __out_ecount(size)
#define __out_bcount(size)
#define __out_ecount_part(size,length)
#define __out_bcount_part(size,length)
#define __out_ecount_full(size)
#define __out_bcount_full(size)
#define __inout
#define __inout_ecount(size)
#define __inout_bcount(size)
#define __inout_ecount_part(size,length)
#define __inout_bcount_part(size,length)
#define __inout_ecount_full(size)
#define __inout_bcount_full(size)

#define __ecount_opt(size)
#define __bcount_opt(size)
#define __in_opt
#define __in_ecount_opt(size)
#define __in_bcount_opt(size)
#define __out_opt
#define __out_ecount_opt(size)
#define __out_bcount_opt(size)
#define __out_ecount_part_opt(size,length)
#define __out_bcount_part_opt(size,length)
#define __out_ecount_full_opt(size)
#define __out_bcount_full_opt(size)
#define __inout_opt
#define __inout_ecount_opt(size)
#define __inout_bcount_opt(size)
#define __inout_ecount_part_opt(size,length)
#define __inout_bcount_part_opt(size,length)
#define __inout_ecount_full_opt(size)
#define __inout_bcount_full_opt(size)

#define __deref_ecount(size)
#define __deref_bcount(size)
#define __deref_in
#define __deref_in_ecount(size)
#define __deref_in_bcount(size)
#define __deref_out
#define __deref_out_ecount(size)
#define __deref_out_bcount(size)
#define __deref_out_ecount_part(size,length)
#define __deref_out_bcount_part(size,length)      
#define __deref_out_ecount_full(size)             
#define __deref_out_bcount_full(size)             
#define __deref_inout                             
#define __deref_inout_ecount(size)                
#define __deref_inout_bcount(size)                
#define __deref_inout_ecount_part(size,length)    
#define __deref_inout_bcount_part(size,length)    
#define __deref_inout_ecount_full(size)           
#define __deref_inout_bcount_full(size)           

#define __deref_ecount_opt(size)                  
#define __deref_bcount_opt(size)                  
#define __deref_in_opt                            
#define __deref_in_ecount_opt(size)               
#define __deref_in_bcount_opt(size)               
#define __deref_out_opt                           
#define __deref_out_ecount_opt(size)              
#define __deref_out_bcount_opt(size)              
#define __deref_out_ecount_part_opt(size,length)  
#define __deref_out_bcount_part_opt(size,length)  
#define __deref_out_ecount_full_opt(size)         
#define __deref_out_bcount_full_opt(size)         
#define __deref_inout_opt                         
#define __deref_inout_ecount_opt(size)            
#define __deref_inout_bcount_opt(size)            
#define __deref_inout_ecount_part_opt(size,length)
#define __deref_inout_bcount_part_opt(size,length)
#define __deref_inout_ecount_full_opt(size)
#define __deref_inout_bcount_full_opt(size)

#define __deref_opt_ecount(size)                    
#define __deref_opt_bcount(size)                         
#define __deref_opt_in                                   
#define __deref_opt_in_ecount(size)                      
#define __deref_opt_in_bcount(size)                      
#define __deref_opt_out                                  
#define __deref_opt_out_ecount(size)                     
#define __deref_opt_out_bcount(size)                     
#define __deref_opt_out_ecount_part(size,length)         
#define __deref_opt_out_bcount_part(size,length)         
#define __deref_opt_out_ecount_full(size)                
#define __deref_opt_out_bcount_full(size)                
#define __deref_opt_inout                                
#define __deref_opt_inout_ecount(size)                   
#define __deref_opt_inout_bcount(size)                   
#define __deref_opt_inout_ecount_part(size,length)       
#define __deref_opt_inout_bcount_part(size,length)       
#define __deref_opt_inout_ecount_full(size)              
#define __deref_opt_inout_bcount_full(size)              

#define __deref_opt_ecount_opt(size)                     
#define __deref_opt_bcount_opt(size)                     
#define __deref_opt_in_opt
#define __deref_opt_in_ecount_opt(size)
#define __deref_opt_in_bcount_opt(size)
#define __deref_opt_out_opt
#define __deref_opt_out_ecount_opt(size)
#define __deref_opt_out_bcount_opt(size)
#define __deref_opt_out_ecount_part_opt(size,length)
#define __deref_opt_out_bcount_part_opt(size,length)
#define __deref_opt_out_ecount_full_opt(size)
#define __deref_opt_out_bcount_full_opt(size)
#define __deref_opt_inout_opt
#define __deref_opt_inout_ecount_opt(size)
#define __deref_opt_inout_bcount_opt(size)
#define __deref_opt_inout_ecount_part_opt(size,length)
#define __deref_opt_inout_bcount_part_opt(size,length)
#define __deref_opt_inout_ecount_full_opt(size)
#define __deref_opt_inout_bcount_full_opt(size)

#endif // !defined(__specstrings)

// -------------------------------------------------------------------------

#ifndef __in_z
#define __in_z
#endif

#ifndef __in_z_opt
#define __in_z_opt
#endif

#ifndef __out_ecount_part_z
#define __out_ecount_part_z(size,length)
#endif

#ifndef __out_ecount_part_z_opt
#define __out_ecount_part_z_opt(size,length)
#endif

// -------------------------------------------------------------------------
// $Log: $

#endif /* STDEXT_MSVCRT_SPECSTRINGS_H */
