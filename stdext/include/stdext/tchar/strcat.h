/***
*strcat.c - contains strcat() and strcpy()
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       strcat() appends one xchar string onto another.
*       strcpy() copies one xchar string into another.
*
*       strcat() concatenates (appends) a copy of the source string to the
*       end of the destination string, returning the destination string.
*       Strings are wide-character strings.
*
*       strcpy() copies the source string to the spot pointed to be
*       the destination string, returning the destination string.
*       Strings are wide-character strings.
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *strcat(dst, src) - concatenate (append) one xchar string to another
*
*Purpose:
*       Concatenates src onto the end of dest.  Assumes enough
*       space in dest.
*
*Entry:
*       xchar *dst - xchar string to which "src" is to be appended
*       const xchar *src - xchar string to be appended to the end of "dst"
*
*Exit:
*       The address of "dst"
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
xchar * __xdecl __xcharfunc(strcat)(xchar * dst, const xchar* src)
{
        xchar * cp = dst;

        while( *cp )
                cp++;                   /* find end of dst */

        while( *cp++ = *src++ ) ;       /* Copy src to end of dst */

        return( dst );                  /* return dst */

}

/***
*xchar *strcpy(dst, src) - copy one xchar string over another
*
*Purpose:
*       Copies the xchar string src into the spot specified by
*       dest; assumes enough room.
*
*Entry:
*       xchar * dst - xchar string over which "src" is to be copied
*       const xchar * src - xchar string to be copied over "dst"
*
*Exit:
*       The address of "dst"
*
*Exceptions:
*******************************************************************************/

__template_xchar
xchar * __xdecl __xcharfunc(strcpy)(xchar* dst, const xchar * src)
{
        xchar* cp = dst;

        while( *cp++ = *src++ )
                ;               /* Copy src over dst */

        return( dst );
}

