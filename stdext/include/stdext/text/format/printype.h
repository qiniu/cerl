/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: stdext/text/format/printype.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 10:08:49
// 
// $Id: printype.h,v 1.4 2006/12/03 07:51:57 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_TEXT_FORMAT_PRINTYPE_H
#define STDEXT_TEXT_FORMAT_PRINTYPE_H

#ifndef STDEXT_BASIC_H
#include "../../Basic.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// STD_PRINTYPE_XXX

#define STD_PRINTYPE_FLAGS_SUB		0x01		/* [-] */
#define STD_PRINTYPE_FLAGS_ADD		0x02		/* [+] */
#define STD_PRINTYPE_FLAGS_SPACE	0x04		/* [ ] */
#define STD_PRINTYPE_FLAGS_SHARP	0x08		/* [#] */
#define STD_PRINTYPE_FLAGS_ZERO		0x10		/* [0] */
#define STD_PRINTYPE_FLAGS			(STD_PRINTYPE_FLAGS_SUB | STD_PRINTYPE_FLAGS_ADD | \
									 STD_PRINTYPE_FLAGS_SPACE | STD_PRINTYPE_FLAGS_SHARP | \
									 STD_PRINTYPE_FLAGS_ZERO)

#define STD_PRINTYPE_WIDTH_NUMBER	0x20		/* [0-9] */
#define STD_PRINTYPE_WIDTH_MUL		0x40		/* [*] */
#define STD_PRINTYPE_WIDTH			(STD_PRINTYPE_WIDTH_NUMBER | STD_PRINTYPE_WIDTH_MUL)

#define STD_PRINTYPE_LENGTH			0x80		/* [hlLqjzt] */

#define STD_PRINTYPE_I_DEC			0x100		/* [di] */
#define STD_PRINTYPE_I_UDEC			0x200		/* [u] */
#define STD_PRINTYPE_I_OCT			0x400		/* [o] */
#define STD_PRINTYPE_I_HEX_UPPER	0x800		/* [X] */
#define STD_PRINTYPE_I_HEX_LOWER	0x1000		/* [x] */
#define STD_PRINTYPE_F_SCIENTIFIC	0x2000		/* [Ee] */
#define STD_PRINTYPE_F_POINT		0x4000		/* [f] */
#define STD_PRINTYPE_F_G			0x8000		/* [Gg] */
#define STD_PRINTYPE_STRING			0x10000		/* [s] */
#define STD_PRINTYPE_PTR			0x20000		/* [p] */
#define STD_PRINTYPE_SIZE			0x40000		/* [n] */
#define STD_PRINTYPE_CHAR			0x80000		/* [c] */
#define STD_PRINTYPE_ANY			0x100000	/* [a] */
#define STD_PRINTYPE_RESERVED		0x80000000	/* Reserved */
#define STD_PRINTYPE_I_HEX			(STD_PRINTYPE_I_HEX_UPPER |	STD_PRINTYPE_I_HEX_LOWER)
#define STD_PRINTYPE_I				(STD_PRINTYPE_I_DEC | STD_PRINTYPE_I_UDEC | STD_PRINTYPE_I_OCT | STD_PRINTYPE_I_HEX)
#define STD_PRINTYPE_F				(STD_PRINTYPE_F_G | STD_PRINTYPE_F_SCIENTIFIC | STD_PRINTYPE_F_POINT)
#define STD_PRINTYPE_SPECIFIER		(STD_PRINTYPE_I | STD_PRINTYPE_F | STD_PRINTYPE_CHAR | STD_PRINTYPE_ANY | \
									 STD_PRINTYPE_STRING | STD_PRINTYPE_PTR | STD_PRINTYPE_SIZE)

#define STD_PRINTYPE_MAX_CHAR		128

// -------------------------------------------------------------------------

#define STD_PRINTYPE_IS_(mask, c)												\
	(STD_PRINTYPE_MAX_CHAR > (unsigned)(c) ? ((mask) & data[c]) : 0)

// -------------------------------------------------------------------------
// class PrinType

template <class Unused>
struct PrinTypeT
{
	static unsigned data[STD_PRINTYPE_MAX_CHAR];

	static unsigned winx_call is(unsigned typeMask, int c) {
		return STD_PRINTYPE_IS_(typeMask, c);
	}
};

template <class Unused>
unsigned PrinTypeT<Unused>::data[STD_PRINTYPE_MAX_CHAR] =
{
	0,	//   [0]
	0,	//   [1]
	0,	//   [2]
	0,	//   [3]
	0,	//   [4]
	0,	//   [5]
	0,	//   [6]
	0,	//   [7]
	0,	//   [8]
	0,	//   [9]
	0,	//   [10]
	0,	//   [11]
	0,	//   [12]
	0,	//   [13]
	0,	//   [14]
	0,	//   [15]
	0,	//   [16]
	0,	//   [17]
	0,	//   [18]
	0,	//   [19]
	0,	//   [20]
	0,	//   [21]
	0,	//   [22]
	0,	//   [23]
	0,	//   [24]
	0,	//   [25]
	0,	//   [26]
	0,	//   [27]
	0,	//   [28]
	0,	//   [29]
	0,	//   [30]
	0,	//   [31]
	STD_PRINTYPE_FLAGS_SPACE,	//   [32]
	0,	// ! [33]
	0,	// " [34]
	STD_PRINTYPE_FLAGS_SHARP,	// # [35]
	0,	// $ [36]
	0,	// % [37]
	0,	// & [38]
	0,	// ' [39]
	0,	// ( [40]
	0,	// ) [41]
	STD_PRINTYPE_WIDTH_MUL,	// * [42]
	STD_PRINTYPE_FLAGS_ADD,	// + [43]
	0,	// , [44]
	STD_PRINTYPE_FLAGS_SUB,	// - [45]
	0,	// . [46]
	0,	// / [47]
	STD_PRINTYPE_FLAGS_ZERO | STD_PRINTYPE_WIDTH_NUMBER,	// 0 [48]
	STD_PRINTYPE_WIDTH_NUMBER,	// 1 [49]
	STD_PRINTYPE_WIDTH_NUMBER,	// 2 [50]
	STD_PRINTYPE_WIDTH_NUMBER,	// 3 [51]
	STD_PRINTYPE_WIDTH_NUMBER,	// 4 [52]
	STD_PRINTYPE_WIDTH_NUMBER,	// 5 [53]
	STD_PRINTYPE_WIDTH_NUMBER,	// 6 [54]
	STD_PRINTYPE_WIDTH_NUMBER,	// 7 [55]
	STD_PRINTYPE_WIDTH_NUMBER,	// 8 [56]
	STD_PRINTYPE_WIDTH_NUMBER,	// 9 [57]
	0,	// : [58]
	0,	// ; [59]
	0,	// < [60]
	0,	// = [61]
	0,	// > [62]
	0,	// ? [63]
	0,	// @ [64]
	0,	// A [65]
	0,	// B [66]
	0,	// C [67]
	0,	// D [68]
	STD_PRINTYPE_F_SCIENTIFIC,	// E [69]
	0,	// F [70]
	STD_PRINTYPE_F_G,	// G [71]
	0,	// H [72]
	0,	// I [73]
	0,	// J [74]
	0,	// K [75]
	STD_PRINTYPE_LENGTH,	// L [76]
	0,	// M [77]
	0,	// N [78]
	0,	// O [79]
	0,	// P [80]
	0,	// Q [81]
	0,	// R [82]
	0,	// S [83]
	0,	// T [84]
	0,	// U [85]
	0,	// V [86]
	0,	// W [87]
	STD_PRINTYPE_I_HEX_UPPER,	// X [88]
	0,	// Y [89]
	0,	// Z [90]
	0,	// [ [91]
	0,	// \ [92]
	0,	// ] [93]
	0,	// ^ [94]
	0,	// _ [95]
	0,	// ` [96]
	STD_PRINTYPE_ANY,	// a [97]
	0,	// b [98]
	STD_PRINTYPE_CHAR,	// c [99]
	STD_PRINTYPE_I_DEC,	// d [100]
	STD_PRINTYPE_F_SCIENTIFIC,	// e [101]
	STD_PRINTYPE_F_POINT,	// f [102]
	STD_PRINTYPE_F_G,	// g [103]
	STD_PRINTYPE_LENGTH,	// h [104]
	STD_PRINTYPE_I_DEC,	// i [105]
	STD_PRINTYPE_LENGTH,	// j [106]
	0,	// k [107]
	STD_PRINTYPE_LENGTH,	// l [108]
	0,	// m [109]
	STD_PRINTYPE_SIZE,	// n [110]
	STD_PRINTYPE_I_OCT,	// o [111]
	STD_PRINTYPE_PTR,	// p [112]
	STD_PRINTYPE_LENGTH,	// q [113]
	0,	// r [114]
	STD_PRINTYPE_STRING,	// s [115]
	STD_PRINTYPE_LENGTH,	// t [116]
	STD_PRINTYPE_I_UDEC,	// u [117]
	0,	// v [118]
	0,	// w [119]
	STD_PRINTYPE_I_HEX_LOWER,	// x [120]
	0,	// y [121]
	STD_PRINTYPE_LENGTH,	// z [122]
	0,	// { [123]
	0,	// | [124]
	0,	// } [125]
	0,	// ~ [126]
	0,	// del [127]
};

typedef PrinTypeT<void> PrinType;

// -------------------------------------------------------------------------

NS_STDEXT_END

#endif /* STDEXT_TEXT_FORMAT_PRINTYPE_H */
