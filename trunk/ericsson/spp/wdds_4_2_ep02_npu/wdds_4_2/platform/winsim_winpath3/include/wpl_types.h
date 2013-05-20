/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * Module:
 * Purpose:
 *
 * This module contains platform-specific data types.
 ****************************************************************************/

#ifndef WPL_TYPES_H
#define WPL_TYPES_H

#include <sys/types.h>
#include <stddef.h>

/*****************************************************************************
 * Definition of basic types for this platform.
 ****************************************************************************/
#ifdef WIN32
typedef double WP_U64;
typedef __int64 WP_S64;
#else
typedef unsigned long long WP_U64;
typedef signed long long WP_S64;
#endif
typedef unsigned int WP_U32;
typedef unsigned short WP_U16;
typedef unsigned char WP_U8;
typedef int WP_S32;
typedef short WP_S16;
typedef signed char WP_S8;
typedef char WP_CHAR;
typedef int WP_SEM_ID;
typedef int WP_THREAD_ID;

typedef volatile WP_U8 WP_VU8;
typedef volatile WP_U16 WP_VU16;
typedef volatile WP_U32 WP_VU32;
typedef volatile WP_S8 WP_VS8;
typedef volatile WP_S16 WP_VS16;
typedef volatile WP_S32 WP_VS32;

typedef enum
{
   WP_TRUE = 1,
   WP_FALSE = 0
}
WP_boolean;

typedef struct
{
   WP_U32 sec;
   WP_U32 usec;      
} WP_time;


/****************************************************************************
 * Undefine StdLib functions in order to avoid using redefining macros
 * instead of real function calls. (Some compilers do that)
 * *************************************************************************/
#ifdef memset
#undef memset
#endif

#ifdef strncpy
#undef strncpy
#endif

#ifdef strcmp
#undef strcmp
#endif

#ifdef strcpy
#undef strcpy
#endif

#ifdef strcat
#undef strcat
#endif

#endif
