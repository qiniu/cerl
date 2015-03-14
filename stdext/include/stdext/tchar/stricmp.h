/***
*wcsicmp.c - contains case-insensitive wide string comp routine _wcsicmp
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _wcsicmp()
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __xdecl
#endif

/***
*int _wcsicmp(dst, src) - compare wide-character strings, ignore case
*
*Purpose:
*       _wcsicmp perform a case-insensitive xchar string comparision.
*       _wcsicmp is independent of locale.
*
*Entry:
*       xchar *dst, *src - strings to compare
*
*Return:
*       <0 if dst < src
*        0 if dst = src
*       >0 if dst > src
*       This range of return values may differ from other *cmp|*coll functions.
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
int __xdecl __xcharfunc(stricmp)(
        const xchar * dst,
        const xchar * src
        )
{
        xchar f,l;
        
		do  {
            f = ((*dst <= 'Z') && (*dst >= 'A'))
                ? *dst + 'a' - 'A'
                : *dst;
            l = ((*src <= 'Z') && (*src >= 'A'))
                ? *src + 'a' - 'A'
                : *src;
            dst++;
            src++;
        } while ( (f) && (f == l) );

        return (int)(f - l);
}

