/***
*wcsnicmp.c - compare n chars of wide-character strings, ignoring case
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wcsnicmp() - Compares at most n characters of two xchar
*       strings, without regard to case.
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*int _wcsnicmp(first, last, count) - compares count xchar of strings,
*       ignore case
*
*Purpose:
*       Compare the two strings for lexical order.  Stops the comparison
*       when the following occurs: (1) strings differ, (2) the end of the
*       strings is reached, or (3) count characters have been compared.
*       For the purposes of the comparison, upper case characters are
*       converted to lower case (wide-characters).
*
*Entry:
*       xchar *first, *last - strings to compare
*       size_t count - maximum number of characters to compare
*
*Exit:
*       -1 if first < last
*        0 if first == last
*        1 if first > last
*       This range of return values may differ from other *cmp|*coll functions.
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
int __xdecl __xcharfunc(strnicmp) (
        const xchar * first,
        const xchar * last,
        size_t count
        )
{
        xchar f,l;
        int result = 0;

        if ( count ) {
            do {
                f = ((*first <= L'Z') && (*first >= L'A'))
                ? *first - L'A' + L'a'
                : *first;
                l = ((*last <= L'Z') && (*last >= L'A'))
                ? *last - L'A' + L'a'
                : *last;
                first++;
                last++;
            } while ( (--count) && f && (f == l) );

            result = (int)(f - l);
        }
        return result;
}

