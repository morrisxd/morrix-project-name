/*************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/


#ifndef _LIMITS_H_
#define _LIMITS_H_
 /*
  * Sizes of integral types
  */

#if !defined(_CHAR_IS_SIGNED) && !defined(_CHAR_IS_UNSIGNED)
                                        /* By default chars are signed */
#define _CHAR_IS_SIGNED
#endif

#define CHAR_BIT        8               /* max # of bits in a "char" */
#define SCHAR_MIN       (-128)          /* min value of a "signed char" */
#define SCHAR_MAX       127             /* max value of a "signed char" */
#define UCHAR_MAX       255             /* max value of an "unsigned char" */

#define MB_LEN_MAX      5

#if defined(_CHAR_IS_SIGNED)
#define CHAR_MIN        SCHAR_MIN       /* min value of a "char" */
#define CHAR_MAX        SCHAR_MAX       /* max value of a "char" */
#elif defined(_CHAR_IS_UNSIGNED)
#define CHAR_MIN        0               /* min value of a "char" */
#define CHAR_MAX        UCHAR_MAX       /* max value of a "char" */
#else
#error "chars are signed or unsigned"
#endif

#define SHRT_MIN        (-32768)        /* min value of a "short int" */
#define SHRT_MAX        32767           /* max value of a "short int" */
#define USHRT_MAX       65535           /* max value of "unsigned short int" */
#define INT_MIN         (-2147483647-1) /* min value of an "int" */
#define INT_MAX         2147483647      /* max value of an "int" */
#define UINT_MAX        4294967295U     /* max value of an "unsigned int" */

#if defined(_LP64)
#define LONG_MIN        (-9223372036854775807L-1L)
                                        /* min value of a "long int" */
#define LONG_MAX        9223372036854775807L
                                        /* max value of a "long int" */
#define ULONG_MAX       18446744073709551615UL
                                        /* max value of "unsigned long int" */
#else   /* _ILP32 */
#define LONG_MIN        (-2147483647L-1L)
                                        /* min value of a "long int" */
#define LONG_MAX        2147483647L     /* max value of a "long int" */
#define ULONG_MAX       4294967295UL    /* max value of "unsigned long int" */
#endif

#define MAXUINT(w)      (\
                ((w) == sizeof(WP_U8))  ? 0xFFU :\
                ((w) == sizeof(WP_U16)) ? 0xFFFFU :\
                ((w) == sizeof(WP_U32)) ? 0xFFFFFFFFU : 0)

#define MAXINT(w)       (\
                ((w) == sizeof(WP_S8))  ? 0x7F :\
                ((w) == sizeof(WP_S16)) ? 0x7FFF :\
                ((w) == sizeof(WP_S32)) ? 0x7FFFFFFF : 0)

#endif
