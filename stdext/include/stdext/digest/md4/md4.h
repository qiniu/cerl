///////////////////////////////////////////////////////////////////////////////
/* PROTOTYPES should be set to one if and only if the compiler supports
     function argument prototyping.
   The following makes PROTOTYPES default to 0 if it has not already
     been defined with C compiler flags.
 */
#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
   If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
     returns an empty list.
 */

#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

/* MD4.H - header file for MD4C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD4 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD4 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
 */

/* MD4 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD4_CTX;

/*
inline void MD4Init PROTO_LIST ((MD4_CTX *));
inline void MD4Update PROTO_LIST
  ((MD4_CTX *, unsigned char *, unsigned int));
inline void MD4Final PROTO_LIST ((unsigned char [16], MD4_CTX *));
*/

/* MD4C.C - RSA Data Security, Inc., MD4 message-digest algorithm
 */

/* Copyright (C) 1990-2, RSA Data Security, Inc. All rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD4 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD4 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
 */

/* Constants for MD4Transform routine.
 */
#define S11 3
#define S12 7
#define S13 11
#define S14 19
#define S21 3
#define S22 5
#define S23 9
#define S24 13
#define S31 3
#define S32 9
#define S33 11
#define S34 15

/* inline void Encode PROTO_LIST
  ((unsigned char *, UINT4 *, unsigned int)); */
/* Encodes input (UINT4) into output (unsigned char). Assumes len is
     a multiple of 4.
 */
inline void Encode (unsigned char *output, UINT4 *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
    output[j] = (unsigned char)(input[i] & 0xff);
    output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
    output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
    output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
  }
}

/* inline void Decode PROTO_LIST
  ((UINT4 *, unsigned char *, unsigned int)); */
/* Decodes input (unsigned char) into output (UINT4). Assumes len is
     a multiple of 4. */
inline void Decode (UINT4 * output, const unsigned char *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
    output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |
      (((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);
}

// inline void MD4_memcpy PROTO_LIST ((POINTER, POINTER, unsigned int));
/* Note: Replace "for loop" with standard memcpy if possible.
 */
inline void MD4_memcpy (POINTER output, POINTER input, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len; i++)
	output[i] = input[i];
}

// inline void MD4_memset PROTO_LIST ((POINTER, int, unsigned int));
/* Note: Replace "for loop" with standard memset if possible.
 */
inline void MD4_memset (POINTER output, int value, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
    ((char *)output)[i] = (char)value;
}

/* F, G and H are basic MD4 functions.
 */
#define MD4_F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define MD4_G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define MD4_H(x, y, z) ((x) ^ (y) ^ (z))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG and HH are transformations for rounds 1, 2 and 3 */
/* Rotation is separate from addition to prevent recomputation */

#define MD4_FF(a, b, c, d, x, s) { \
    (a) += MD4_F ((b), (c), (d)) + (x); \
    (a) = ROTATE_LEFT ((a), (s)); \
  }
#define MD4_GG(a, b, c, d, x, s) { \
    (a) += MD4_G ((b), (c), (d)) + (x) + (UINT4)0x5a827999; \
    (a) = ROTATE_LEFT ((a), (s)); \
  }
#define MD4_HH(a, b, c, d, x, s) { \
    (a) += MD4_H ((b), (c), (d)) + (x) + (UINT4)0x6ed9eba1; \
    (a) = ROTATE_LEFT ((a), (s)); \
  }

// inline void MD4Transform PROTO_LIST ((UINT4 [4], unsigned char [64]));
/* MD4 basic transformation. Transforms state based on block.
 */
inline void MD4Transform (UINT4 state[4], const unsigned char block[64])
{
  UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  Decode (x, block, 64);

  /* Round 1 */
  MD4_FF (a, b, c, d, x[ 0], S11); /* 1 */
  MD4_FF (d, a, b, c, x[ 1], S12); /* 2 */
  MD4_FF (c, d, a, b, x[ 2], S13); /* 3 */
  MD4_FF (b, c, d, a, x[ 3], S14); /* 4 */
  MD4_FF (a, b, c, d, x[ 4], S11); /* 5 */
  MD4_FF (d, a, b, c, x[ 5], S12); /* 6 */
  MD4_FF (c, d, a, b, x[ 6], S13); /* 7 */
  MD4_FF (b, c, d, a, x[ 7], S14); /* 8 */
  MD4_FF (a, b, c, d, x[ 8], S11); /* 9 */
  MD4_FF (d, a, b, c, x[ 9], S12); /* 10 */
  MD4_FF (c, d, a, b, x[10], S13); /* 11 */
  MD4_FF (b, c, d, a, x[11], S14); /* 12 */
  MD4_FF (a, b, c, d, x[12], S11); /* 13 */
  MD4_FF (d, a, b, c, x[13], S12); /* 14 */
  MD4_FF (c, d, a, b, x[14], S13); /* 15 */
  MD4_FF (b, c, d, a, x[15], S14); /* 16 */

  /* Round 2 */
  MD4_GG (a, b, c, d, x[ 0], S21); /* 17 */
  MD4_GG (d, a, b, c, x[ 4], S22); /* 18 */
  MD4_GG (c, d, a, b, x[ 8], S23); /* 19 */
  MD4_GG (b, c, d, a, x[12], S24); /* 20 */
  MD4_GG (a, b, c, d, x[ 1], S21); /* 21 */
  MD4_GG (d, a, b, c, x[ 5], S22); /* 22 */
  MD4_GG (c, d, a, b, x[ 9], S23); /* 23 */
  MD4_GG (b, c, d, a, x[13], S24); /* 24 */
  MD4_GG (a, b, c, d, x[ 2], S21); /* 25 */
  MD4_GG (d, a, b, c, x[ 6], S22); /* 26 */
  MD4_GG (c, d, a, b, x[10], S23); /* 27 */
  MD4_GG (b, c, d, a, x[14], S24); /* 28 */
  MD4_GG (a, b, c, d, x[ 3], S21); /* 29 */
  MD4_GG (d, a, b, c, x[ 7], S22); /* 30 */
  MD4_GG (c, d, a, b, x[11], S23); /* 31 */
  MD4_GG (b, c, d, a, x[15], S24); /* 32 */

  /* Round 3 */
  MD4_HH (a, b, c, d, x[ 0], S31); /* 33 */
  MD4_HH (d, a, b, c, x[ 8], S32); /* 34 */
  MD4_HH (c, d, a, b, x[ 4], S33); /* 35 */
  MD4_HH (b, c, d, a, x[12], S34); /* 36 */
  MD4_HH (a, b, c, d, x[ 2], S31); /* 37 */
  MD4_HH (d, a, b, c, x[10], S32); /* 38 */
  MD4_HH (c, d, a, b, x[ 6], S33); /* 39 */
  MD4_HH (b, c, d, a, x[14], S34); /* 40 */
  MD4_HH (a, b, c, d, x[ 1], S31); /* 41 */
  MD4_HH (d, a, b, c, x[ 9], S32); /* 42 */
  MD4_HH (c, d, a, b, x[ 5], S33); /* 43 */
  MD4_HH (b, c, d, a, x[13], S34); /* 44 */
  MD4_HH (a, b, c, d, x[ 3], S31); /* 45 */
  MD4_HH (d, a, b, c, x[11], S32); /* 46 */
  MD4_HH (c, d, a, b, x[ 7], S33); /* 47 */
  MD4_HH (b, c, d, a, x[15], S34); /* 48 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information.
   */
  MD4_memset ((POINTER)x, 0, sizeof (x));
}

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* MD4 initialization. Begins an MD4 operation, writing a new context.
 */
inline void MD4Init (MD4_CTX *context)
{
  context->count[0] = context->count[1] = 0;

  /* Load magic initialization constants.
   */
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/* MD4 block update operation. Continues an MD4 message-digest
     operation, processing another message block, and updating the
     context.
 */
inline void MD4Update (MD4_CTX *context, const unsigned char *input, unsigned int inputLen)
{
  unsigned int i, index, partLen;

  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);
  /* Update number of bits */
  if ((context->count[0] += ((UINT4)inputLen << 3))
      < ((UINT4)inputLen << 3))
    context->count[1]++;
  context->count[1] += ((UINT4)inputLen >> 29);

  partLen = 64 - index;

  /* Transform as many times as possible.
   */
  if (inputLen >= partLen) {
    MD4_memcpy
      ((POINTER)&context->buffer[index], (POINTER)input, partLen);
    MD4Transform (context->state, context->buffer);

    for (i = partLen; i + 63 < inputLen; i += 64)
      MD4Transform (context->state, &input[i]);

    index = 0;
  }
  else
    i = 0;

  /* Buffer remaining input */
  MD4_memcpy
    ((POINTER)&context->buffer[index], (POINTER)&input[i],
     inputLen-i);
}

/* MD4 finalization. Ends an MD4 message-digest operation, writing the
     the message digest and zeroizing the context.
 */
/* digest: message digest */
/* context */
inline void MD4Final (unsigned char digest[16], MD4_CTX *context)
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* Pad out to 56 mod 64.
   */
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  MD4Update (context, PADDING, padLen);

  /* Append length (before padding) */
  MD4Update (context, bits, 8);
  /* Store state in digest */
  Encode (digest, context->state, 16);

  /* Zeroize sensitive information.
   */
  MD4_memset ((POINTER)context, 0, sizeof (*context));

}
