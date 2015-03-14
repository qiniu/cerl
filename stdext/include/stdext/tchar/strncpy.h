/***
*wcsncpy.c - copy at most n characters of wide-character string
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcsncpy() - copy at most n characters of xchar string
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *wcsncpy(dest, source, count) - copy at most n wide characters
*
*Purpose:
*       Copies count characters from the source string to the
*       destination.  If count is less than the length of source,
*       NO NULL CHARACTER is put onto the end of the copied string.
*       If count is greater than the length of sources, dest is padded
*       with null characters to length count (wide-characters).
*
*
*Entry:
*       xchar *dest - pointer to destination
*       xchar *source - source string for copy
*       size_t count - max number of characters to copy
*
*Exit:
*       returns dest
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
xchar * __xdecl __xcharfunc(strncpy)(
        xchar * dest,
        const xchar * source,
        size_t count
        )
{
        xchar *start = dest;

        while (count && (*dest++ = *source++))    /* copy string */
                count--;

        if (count)                              /* pad out with zeroes */
                while (--count)
                        *dest++ = L'\0';

        return(start);
}

