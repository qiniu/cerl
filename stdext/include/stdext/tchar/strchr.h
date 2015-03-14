/***
*wcschr.c - search a xchar string for a given xchar character
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcschr() - search a xchar string for a xchar character
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *wcschr(string, c) - search a string for a xchar character
*
*Purpose:
*       Searches a xchar string for a given xchar character,
*       which may be the null character L'\0'.
*
*Entry:
*       xchar *string - xchar string to search in
*       xchar c - xchar character to search for
*
*Exit:
*       returns pointer to the first occurence of c in string
*       returns NULL if c does not occur in string
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
xchar * __xdecl __xcharfunc(strchr)(
        const xchar * string,
        xchar ch
        )
{
        while (*string && *string != ch)
                string++;

        if (*string == ch)
                return (xchar*)string;
        return(NULL);
}

