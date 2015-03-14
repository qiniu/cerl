/***
*wcsdup.c - duplicate a wide-character string in malloc'd memory
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wcsdup() - grab new memory, and duplicate the string into it
*       (wide-character).
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*xchar *_wcsdup(string) - duplicate string into malloc'd memory
*
*Purpose:
*       Allocates enough storage via malloc() for a copy of the
*       string, copies the string into the new memory, and returns
*       a pointer to it (wide-character).
*
*Entry:
*       xchar *string - string to copy into new memory
*
*Exit:
*       returns a pointer to the newly allocated storage with the
*       string in it.
*
*       returns NULL if enough memory could not be allocated, or
*       string was NULL.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

#if defined(X_CC_VC_NET)
#pragma push_macro("strdup")
#undef strdup
#endif

__template_xchar
xchar * __xdecl __xcharfunc(strdup)(
        const xchar * string
        )
{
        xchar *memory;

        if (!string)
                return(NULL);

        if (memory = (xchar *) malloc((__xcharfunc(strlen)(string)+1) * sizeof(xchar)))
                return __xcharfunc(strcpy)(memory,string);

        return(NULL);
}

#if defined(X_CC_VC_NET)
#pragma pop_macro("strdup")
#endif
