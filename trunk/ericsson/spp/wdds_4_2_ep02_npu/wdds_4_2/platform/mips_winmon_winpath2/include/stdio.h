/*************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#ifndef _STDIO_
#define _STDIO_

#include <varargs.h>
#include <winmon.h>

extern int printf(const char *, ...);
extern int sprintf(char *, const char *, ...);
extern int vsprintf(char *buf, char *fmt, va_list ap);

extern int unformat(int (*)(int), char *,va_list);
extern int sscanf(char *s, char *fmt, ...);

extern char *index(char *s, int  c);
extern char *gets(char *s);

#define putchar(c) WinMon_putchar(c)
#define puts(s)    WinMon_puts(s)
#define getchar()  WinMon_getchar()

#endif /* _STDIO_ */
