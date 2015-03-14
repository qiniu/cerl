/***
*wcsspn.c - find length of initial substring of chars from a control string
*       (wide-character strings)
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcsspn() - finds the length of the initial substring of
*       a string consisting entirely of characters from a control string
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
*int wcsspn(string, control) - find init substring of control chars
*
*Purpose:
*       Finds the index of the first character in string that does belong
*       to the set of characters specified by control.  This is
*       equivalent to the length of the initial substring of string that
*       consists entirely of characters from control.  The L'\0' character
*       that terminates control is not considered in the matching process
*       (wide-character strings).
*
*Entry:
*       xchar *string - string to search
*       xchar *control - string containing characters not to search for
*
*Exit:
*       returns index of first xchar in string not in control
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
size_t __xdecl __xcharfunc(strspn)(
        const xchar * string,
        const xchar * control
        )
{
        xchar *str = (xchar *) string;
        xchar *ctl;

        /* 1st char not in control string stops search */
        while (*str) {
            for (ctl = (xchar *)control; *ctl != *str; ctl++) {
                if (*ctl == (xchar)0) {
                    /*
                     * reached end of control string without finding a match
                     */
                    return str - string;
                }
            }
            str++;
        }
        /*
         * The whole string consisted of characters from control
         */
        return str - string;
}

