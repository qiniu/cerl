/***
*wcslwr.c - routine to map upper-case characters in a xchar string
*       to lower-case
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts all the upper case characters in a xchar string
*       to lower case, in place.
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *_wcslwr(string) - map upper-case characters in a string to lower-case
*
*Purpose:
*       wcslwr converts upper-case characters in a null-terminated xchar
*       string to their lower-case equivalents.  The result may be longer or
*       shorter than the original string.  Assumes enough space in string
*       to hold the result.
*
*Entry:
*       xchar *wsrc - xchar string to change to lower case
*
*Exit:
*       input string address
*
*Exceptions:
*       on an error, the original string is unaltered
*
*******************************************************************************/

__template_xchar
xchar * __xdecl __xcharfunc(strlwr)(
        xchar * wsrc
        )
{
        xchar *p;             /* traverses string for C locale conversion */
        xchar *wdst = NULL;   /* wide version of string in alternate case */
        //int dstlen;         /* len of wdst string, wide chars, with null */

        for (p=wsrc; *p; p++)
        {
			if ( (*p >= 'A') && (*p <= 'Z') )
				*p = *p - 'A' + 'a';
		}
        return (wsrc);
}

