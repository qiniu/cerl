/***
*wcscspn.c - find length of initial substring of wide characters
*        not in a control string
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcscspn()- finds the length of the initial substring of
*       a string consisting entirely of characters not in a control string
*       (wide-character strings).
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*size_t wcscspn(string, control) - search for init substring w/o control wchars
*
*Purpose:
*       returns the index of the first character in string that belongs
*       to the set of characters specified by control.  This is equivalent
*       to the length of the length of the initial substring of string
*       composed entirely of characters not in control.  Null chars not
*       considered (wide-character strings).
*
*Entry:
*       xchar *string - string to search
*       xchar *control - set of characters not allowed in init substring
*
*Exit:
*       returns the index of the first xchar in string
*       that is in the set of characters specified by control.
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
size_t __xdecl __xcharfunc(strcspn)(
        const xchar * string,
        const xchar * control
        )
{
        xchar *str = (xchar *) string;
        xchar *wcset;

        /* 1st char in control string stops search */
        while (*str) {
            for (wcset = (xchar *)control; *wcset; wcset++) {
                if (*wcset == *str) {
                    return str - string;
                }
            }
            str++;
        }
        return str - string;
}

