/* COPYRIGHT-ENEA-SRC-R2 *
**************************************************************************
* Copyright (C) 2006-2009 by Enea Software AB.
* All rights reserved.
*
* This Software is furnished under a software license agreement and
* may be used only in accordance with the terms of such agreement.
* Any other use or reproduction is prohibited. No title to and
* ownership of the Software is hereby transferred.
*
* PROPRIETARY NOTICE
* This Software consists of confidential information.
* Trade secret law and copyright law protect this Software.
* The above notice of copyright on this Software does not indicate
* any actual or intended publication of such Software.
**************************************************************************
* COPYRIGHT-END
*/

/**
 * @file wddi_log.h
 *
 * @brief The DMP system log interface header file.
 *
 * @long Contains the declarations of the SWU's global identifiers such as
 *       signal structs, constants, types, variables and functions.
 */

#ifndef _WDDI_LOG_H
#define _WDDI_LOG_H

/*----------------------------  Include files  ------------------------------*/
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <syslog.h>

/*----------------------------  Constants  ----------------------------------*/

/* Maximum size of the log message string. */
#define MAX_LOG_STR_LEN 256
#define FILE_NOPATH (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*----------------------------  Macros  -------------------------------------*/

/**
 * @macro INFONL
 *
 * @brief  Writes an informational message string (with the lowest priority
 *         set) to the system log. The preprocessor adds file and line to the
 *         beginning of the supplied string.
 *         This macro prepends the string with a newline, which might be
 *         useful to group certain strings together.
 *
 * @input  msg   The message format string.
 * @input  args  Variable-length argument list.
 *
 * @return
 */
#define WDDI_INFONL(...)                                                \
    do                                                                  \
    {                                                                   \
        (void) wddi_log_msg(LOG_INFO, FILE_NOPATH, __LINE__,"\n" __VA_ARGS__); \
    } while (0)


/**
 * @macro INFO
 *
 * @brief  Writes an informational message string (with the lowest priority
 *         set) to the system log. The preprocessor adds file and line to the
 *         beginning of the supplied string.
 *
 * @input  msg   The message format string.
 * @input  args  Variable-length argument list.
 *
 * @return
 */
#define WDDI_INFO(...)                                                  \
    do                                                                  \
    {                                                                   \
        if(g_logLevel < 2)                                              \
            (void) wddi_log_msg(LOG_INFO, FILE_NOPATH, __LINE__, __VA_ARGS__); \
    } while (0)

/**
 * @macro WARNING
 *
 * @brief  Writes an informational message string (with the lowest priority
 *         set) to the system log. The preprocessor adds file and line to the
 *         beginning of the supplied string.
 *
 * @input  msg   The message format string.
 * @input  args  Variable-length argument list.
 *
 * @return
 */
#define WDDI_WARNING(...)                                               \
    do                                                                  \
    {                                                                   \
        if(g_logLevel < 3)                                              \
            (void) wddi_log_msg(LOG_WARNING, FILE_NOPATH, __LINE__,  __VA_ARGS__); \
    } while (0)

/**
 * @macro WARNINGNL
 *
 * @brief  Writes an informational message string (with the lowest priority
 *         set) to the system log. The preprocessor adds file and line to the
 *         beginning of the supplied string.
 *
 * @input  msg   The message format string.
 * @input  args  Variable-length argument list.
 *
 * @return
 */
#define WDDI_WARNINGNL(...)                                             \
    do                                                                  \
    {                                                                   \
        (void) wddi_log_msg(LOG_WARNING, FILE_NOPATH, __LINE__, "\n" __VA_ARGS__); \
    } while (0)


/**
 * @macro PRINT
 *
 * @brief  Writes an informational message string (with the lowest priority
 *         set) to the system log.
 *
 * @input  args  Variable-length argument list.
 *
 * @return
 */
#define WDDI_PRINT(...)                                                 \
    do                                                                  \
    {                                                                   \
        if(g_logLevel == 0)                                             \
            (void) wddi_log_msg(LOG_DEBUG, FILE_NOPATH, __LINE__, __VA_ARGS__); \
    } while (0)

//#define  WDDI_STRING(buffer, length)

#define  WDDI_STRING(buffer, length)                                    \
    do                                                                  \
    {   if(g_logLevel == 0)                                             \
        {                                                               \
            char string[512]={0};                                       \
            char *pstr = string;                                        \
            int count;                                                  \
            int len;                                                    \
            len = length > 128 ? 128: length ;                          \
            for (count= 0; count < len ;count++)                        \
            {                                                           \
                pstr +=sprintf(pstr, "%2.2x", (unsigned char)buffer[count]); \
            }                                                           \
            *pstr = '\n';                                               \
            WDDI_PRINT(string);                                         \
        }                                                               \
    } while(0)

/**
 * @macro ERR
 *
 * @brief  Writes an error message string (with the highest priority set) to
 *         the system log. The preprocessor adds file and line to the beginning
 *         of the supplied string.
 *
 * @input  args  Variable-length argument list.
 *
 * @return
 */
#define WDDI_ERR(...)                                                   \
        do                                                              \
        {                                                               \
            (void) wddi_log_msg(LOG_EMERG, FILE_NOPATH, __LINE__, "\nFATAL_ERROR"); \
            (void) wddi_log_msg(LOG_EMERG, FILE_NOPATH, __LINE__, __VA_ARGS__); \
            if (errno)                                                  \
            {                                                           \
                (void) wddi_log_msg(LOG_EMERG, FILE_NOPATH, __LINE__, "errno = %d:%s", errno, strerror(errno)); \
            }                                                           \
            (void) wddi_log_msg(LOG_EMERG, FILE_NOPATH, __LINE__, "Terminating.\n"); \
            exit(0);                                                    \
        } while (0)
/*abort replace by exit(1);*/
/*----------------------------  Structs and typedefs  -----------------------*/

/*----------------------------  Declaration of Global Variables  ------------*/

/*----------------------------  Declaration of Global Functions  ------------*/

extern int
wddi_log_open(const char *ident);

extern int
wddi_log_close(void);

extern int
wddi_log_set_mask(char *loglevel);

extern unsigned int
wddi_log_get_mask(char *loglevel, unsigned int max_buff_size);

extern int
wddi_log_msg(unsigned int priority, char *file, unsigned int line, char *log_msg, ...);

#endif /* _WDDI_LOG_H */
