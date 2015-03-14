/***
*wcslen.c - contains wcslen() routine
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       wcslen returns the length of a null-terminated wide-character string,
*       not including the null xchar itself.
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*wcslen - return the length of a null-terminated wide-character string
*
*Purpose:
*       Finds the length in xchar's of the given string, not including
*       the final null xchar (wide-characters).
*
*Entry:
*       const xchar * wcs - string whose length is to be computed
*
*Exit:
*       length of the string "wcs", exclusive of the final null xchar
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
size_t __xdecl __xcharfunc(strlen)(
        const xchar * wcs
        )
{
		const xchar *eos = wcs;

		while( *eos++ ) ;

		return( (size_t)(eos - wcs - 1) );
}

