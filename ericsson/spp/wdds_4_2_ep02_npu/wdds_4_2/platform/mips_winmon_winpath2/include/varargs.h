/*************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#ifndef _VARARGS_
#define _VARARGS_

#ifdef __PPC__
#include "stdarg.h"
#else

#ifndef __va_list__
#define __va_list__
typedef char *va_list;
#endif
#define va_dcl int va_alist;

#ifdef __mips64
#ifndef va_start
#define va_start(list,start) list=(WP_CHAR *)&start+sizeof(WP_S64)
#endif
#ifndef va_arg
#define va_arg(list,mode) ((mode *)(list += sizeof(WP_S64)))[-2]
#endif
#else
#ifndef va_start
#define va_start(list,start) list=(WP_CHAR *)&start+sizeof(WP_S32)
#endif
#ifndef va_arg
#define va_arg(list,mode) ((mode *)(list += sizeof(mode)))[-1]
#endif
#endif
#ifndef va_end
#define va_end(list)
#endif
#endif

#endif /* _VARARGS_ */
