/***
*wcscmp.c - routine to compare two xchar strings (for equal, less, or greater)
*
*       Copyright (c) 1985-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compares two wide-character strings, determining their lexical order.
*
*******************************************************************************/

#ifndef __template_xchar
#define __template_xchar	// template <class xchar>
#endif

#ifndef __xdecl
#define __xdecl				// __cdecl
#endif

/***
*wcscmp - compare two xchar strings,
*        returning less than, equal to, or greater than
*
*Purpose:
*       wcscmp compares two wide-character strings and returns an integer
*       to indicate whether the first is less than the second, the two are
*       equal, or whether the first is greater than the second.
*
*       Comparison is done xchar by xchar on an UNSIGNED basis, which is to
*       say that Null xchar(0) is less than any other character.
*
*Entry:
*       const xchar * src - string for left-hand side of comparison
*       const xchar * dst - string for right-hand side of comparison
*
*Exit:
*       returns <0 if src <  dst
*       returns =0 if src == dst
*       returns >0 if src >  dst
*
*Exceptions:
*
*******************************************************************************/

__template_xchar
int __xdecl __xcharfunc(strcmp)(const xchar * src, const xchar * dst)
{
	int ret;
	while( !(ret = (int)*src - (int)*dst) && *dst )
		++src, ++dst;
	return ret;
}

__forceinline int __xdecl strcmp(const char* src, const char* dst)
{
	return ::strcmp(src, dst);
}

__forceinline int __xdecl strcmp(const wchar_t* src, const wchar_t* dst)
{
	return ::wcscmp(src, dst);
}
