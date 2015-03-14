/***
* itoa.c - convert integers/longs to ASCII string
*
*       Copyright (c) 1989-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The module has code to convert integers/longs to ASCII strings.  See
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

#ifndef _INC_LIMITS
#include <limits.h>
#endif

/***
*xchar *itoa, *ltoa, *ultoa(val, buf, radix) - convert binary int to ASCII
*       string
*
*Purpose:
*       Converts an int to a character string.
*
*Entry:
*       val - number to be converted (int, long or unsigned long)
*       int radix - base to convert into
*       xchar *buf - ptr to buffer to place result
*
*Exit:
*       fills in space pointed to by buf with string result
*       returns a pointer to this buffer
*
*Exceptions:
*
*******************************************************************************/

/* helper routine that does the main job. */

__template_xchar
void __xdecl __xcharfunc(__xtoa)(
        unsigned long val,
        xchar *buf,
        unsigned radix,
        int is_neg
        )
{
        xchar *p;                /* pointer to traverse string */
        xchar *firstdig;         /* pointer to first digit */
        xchar temp;              /* temp xchar */
        unsigned digval;        /* value of digit */

        p = buf;

        if (is_neg) {
            /* negative, so output '-' and negate */
            *p++ = '-';
            val = (unsigned long)(-(long)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (xchar) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (xchar) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

__template_xchar
xchar* __xdecl __xcharfunc(itoa)(
        int val,
        xchar *buf,
        int radix
        )
{
        if (radix == 10 && val < 0)
            __xcharfunc(__xtoa)((unsigned long)val, buf, radix, 1);
        else
            __xcharfunc(__xtoa)((unsigned long)(unsigned int)val, buf, radix, 0);
        return buf;
}

__template_xchar
xchar* __xdecl __xcharfunc(ltoa)(
        long val,
        xchar *buf,
        int radix
        )
{
        __xcharfunc(__xtoa)((unsigned long)val, buf, radix, (radix == 10 && val < 0));
        return buf;
}

__template_xchar
xchar * __xdecl __xcharfunc(ultoa)(
        unsigned long val,
        xchar *buf,
        int radix
        )
{
        __xcharfunc(__xtoa)(val, buf, radix, 0);
        return buf;
}

#ifndef _NO_INT64

__template_xchar
void __xdecl __xcharfunc(__x64toa) (   /* stdcall is faster and smaller... Might as well use it for the helper. */
        __uint64 val,
        xchar *buf,
        unsigned radix,
        int is_neg
        )
{
        xchar *p;                /* pointer to traverse string */
        xchar *firstdig;         /* pointer to first digit */
        xchar temp;              /* temp xchar */
        unsigned digval;        /* value of digit */

        p = buf;

        if ( is_neg )
        {
            *p++ = '-';         /* negative, so output '-' and negate */
            val = (unsigned __int64)(-(__int64)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (xchar) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (xchar) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
   and return pointer to buffer. */

__template_xchar
xchar* __xdecl __xcharfunc(i64toa) (
        __int64 val,
        xchar *buf,
        int radix
        )
{
        __xcharfunc(__x64toa)((unsigned __int64)val, buf, radix, (radix == 10 && val < 0));
        return buf;
}

__template_xchar
xchar* __xdecl __xcharfunc(ui64toa) (
        __uint64 val,
        xchar *buf,
        int radix
        )
{
        __xcharfunc(__x64toa)(val, buf, radix, 0);
        return buf;
}

#endif  /* _NO_INT64 */
