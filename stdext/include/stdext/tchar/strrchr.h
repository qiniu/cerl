/***
*wcsrchr.c - find last occurrence of xchar character in wide string
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcsrchr() - find the last occurrence of a given character
*       in a string (wide-characters).
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *wcsrchr(string, ch) - find last occurrence of ch in wide string
*
*Purpose:
*       Finds the last occurrence of ch in string.  The terminating
*       null character is used as part of the search (wide-characters).
*
*Entry:
*       xchar *string - string to search in
*       xchar ch - character to search for
*
*Exit:
*       returns a pointer to the last occurrence of ch in the given
*       string
*       returns NULL if ch does not occurr in the string
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
xchar * __xdecl __xcharfunc(strrchr)(
        const xchar * string,
        xchar ch
        )
{
        xchar *start = (xchar *)string;

        while (*string++)                       /* find end of string */
                ;
                                                /* search towards front */
        while (--string != start && *string != (xchar)ch)
                ;

        if (*string == (xchar)ch)             /* xchar found ? */
                return( (xchar *)string );

        return(NULL);
}

