/***
*strstr.c - search for one string inside another
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines strstr() - search for one string inside another
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	template <class xchar> inline
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *strstr(string1, string2) - search for string2 in string1
*
*Purpose:
*       finds the first occurrence of string2 in string1
*
*Entry:
*       xchar *string1 - string to search in
*       xchar *string2 - string to search for
*
*Exit:
*       returns a pointer to the first occurrence of string2 in
*       string1, or NULL if string2 does not occur in string1
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
xchar * __xdecl strstr (
        const xchar * str1,
        const xchar * str2
        )
{
        xchar *cp = (xchar *) str1;
        xchar *s1, *s2;

        if ( !*str2 )
            return((xchar *)str1);

        while (*cp)
        {
                s1 = cp;
                s2 = (xchar *) str2;

                while ( *s1 && *s2 && !(*s1-*s2) )
                        s1++, s2++;

                if (!*s2)
                        return(cp);

                cp++;
        }

        return(NULL);

}
