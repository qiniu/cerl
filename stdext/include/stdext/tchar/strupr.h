/***
*strupr.c - routine to map lower-case characters in a string to upper-case
*
*   Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*   Converts all the lower case characters in string to upper case
*   in place.
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*char *_strupr(string) - map lower-case characters in a string to upper-case
*
*Purpose:
*   _strupr() converts lower-case characters in a null-terminated string
*   to their upper-case equivalents.  Conversion is done in place and
*   characters other than lower-case letters are not modified.
*
*   In the C locale, this function modifies only 7-bit ASCII characters
*   in the range 0x61 through 0x7A ('a' through 'z').
*
*   If the locale is not the 'C' locale, MapStringW() is used to do
*   the work.  Assumes enough space in the string to hold result.
*
*Entry:
*   char *string - string to change to upper case
*
*Exit:
*   input string address
*
*Exceptions:
*   The original string is returned unchanged on any error.
*
*******************************************************************************/

__template_xchar
xchar* __xdecl __xcharfunc(strupr)(xchar* string)
{
    xchar * cp;

    for (cp=string; *cp; ++cp)
    {
        if ('a' <= *cp && *cp <= 'z')
            *cp += 'A' - 'a';
    }
    return(string);
}
