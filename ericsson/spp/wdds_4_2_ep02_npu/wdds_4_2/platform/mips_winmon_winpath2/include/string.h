/*************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#ifndef _STRING_
#define _STRING_

#include <stdlib.h>

extern char *strcpy(char *, const char *);
extern char *strncpy(char *, const char *, size_t);
extern char *strcat(char *, const char *);
extern char *strncat(char *, const char *, size_t);
extern char *strchr(const char *, int);
extern char *strstr(const char *, const char *);
extern char *strrchr(char *, int);
extern char *strpbrk(const char *, const char *);
extern char *strtok(char *, char *);
extern char *strxcpy(char *, char *, int);
extern char *strerror(int);

extern unsigned long strtoul(const char *,char **,int);
extern long strtol(const char *,char **,int);

extern int strcmp(const char *, const char *);
extern int strncmp(const char *, const char *, size_t);
extern int strlen(const char *);
extern int strspn(const char *, const char *);
extern int strcspn(const char *, const char *);
extern int strcasecmp(const char *, const char *);
extern void *memccpy(void *, const void *, int, size_t);
extern void *memchr(const void *, int, size_t);
extern void *memcpy(void *, const void *, size_t);
extern void *memset(void *, int, size_t);
extern void *memmove(void *, const void *, size_t);
extern int memcmp(const void *, const void *, size_t);

#endif /* _STRING_ */
