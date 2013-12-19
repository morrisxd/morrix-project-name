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
 * @file wddi_log.c
 *
 * @brief ADSP Platform.
 *
 * @long This file contains the DMP platform's system logging procedures.
 *
 */

/*----------------------------  Include files  ------------------------------*/

#include <syslog.h>
#include <stdarg.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#define  COMMON_MAX_STRING_LENGTH 16
/*----------------------------  Constants  ----------------------------------*/

/*
 * Priority Codes (defined in syslog.h)
 * It determines the importance of a message. The levels are ordered in
 * decreasing importance.
 * ----------------------------------------------------------------------------
 * LOG_EMERG                system is unusable
 * LOG_ALERT                action must be taken immediately
 * LOG_CRIT                 critical conditions
 * LOG_ERR                  error conditions
 * LOG_WARNING              warning conditions
 * LOG_NOTICE               normal but significant condition
 * LOG_INFO                 informational
 * LOG_DEBUG                debug-level messages
 */

/* Facility Codes (defined in syslog.h)
 * The facility code is used to specify what type of program that is logging
 * a certain message.
 * ----------------------------------------------------------------------------
 * LOG_LOCAL0               reserved for local use
 */

/*----------------------------  Macros  -------------------------------------*/

/*----------------------------  Structs and typedefs  -----------------------*/

/*----------------------------  Declaration of Local Functions  -------------*/

/*----------------------------  Definition of Global Variables  -------------*/

/*----------------------------  Definition of Local Variables  --------------*/

static const unsigned int facility = LOG_LOCAL0;  /* Default log facility. */
static char wddi_og_level[COMMON_MAX_STRING_LENGTH];

/*----------------------------  Function Definitions  -----------------------*/

/**
 * @function wddi_log_open
 *
 * @brief
 *
 * @long   Opens a connection to the DMP platform's system log.
 *         Linux syslog is used on a target system and during the debug phase
 *         either the specified file or stderr is used.
 *
 * @input  ident  An identity string, a file path or NULL.
 *
 * @return zero on success, or -1 if an error occurred.
 */
int
wddi_log_open(const char *ident)
{
    /* Opens a connection to the system logger for the calling program.
     * The ident string is prepended to every message, and is typically
     * set to the program name.
     */
    //return 0;
#if 1
    int logfd;
    FILE  * fp = fopen( "/var/log/spp_wddi.log","a+");
	if(fp == NULL)
	{
	    printf("wddi_log_open fail \n");
		exit(1);
	}
    logfd=fileno(fp);
    close(STDERR_FILENO);
    dup2( logfd, STDERR_FILENO);
	dup2( logfd, STDOUT_FILENO);
    close(logfd);
    /*Print to stderr as well*/
    // openlog(ident, LOG_PID | LOG_CONS | LOG_NDELAY | LOG_PERROR, facility);
    openlog(ident, LOG_PID, LOG_USER);
    return 0;
#endif
}


/**
 * @function wddi_log_close
 *
 * @brief
 *
 * @long   Closes the previously opened connection to the system log.
 *
 * @input  None.
 *
 * @return None.
 */
int
wddi_log_close(void)
{
    (void) closelog();
    return 0;
}


/**
 * @function wddi_log_get_mask
 *
 * @brief
 *
 * @long   Gets the name of the log level used to determine which message
 *         string that is logged or not.
 *
 * @input  loglevel String containing the log level name
 * @input  max_buff_size Maximum string size to be copied in the loglevel
 *         string
 *
 * @return None.
 */
int
wddi_log_get_mask(char *loglevel, unsigned int max_buff_size)
{
    strncpy(loglevel, wddi_og_level, max_buff_size);
    loglevel[max_buff_size - 1] = '\0';
    return 0;
}


/**
 * @function wddi_log_set_mask
 *
 * @brief
 *
 * @long   Sets the log priority bit mask used to determine which message
 *         string that is logged or not. To disable logging set mask to
 *         zero.
 *
 * @input  loglevel String containing the log level name
 *
 * @return None.
 */
int
wddi_log_set_mask(char *loglevel)
{
    unsigned int mask;

    /* Logging is enabled for the priorities that have the corresponding bit
     * set in mask.
     * The initial mask is such that logging is enabled for all priorities.
     */

    if (loglevel == NULL)
        return -1;

    if (strncmp(loglevel, "LOG_DEBUG", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_DEBUG;
    else if (strncmp(loglevel, "LOG_INFO", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_INFO;
    else if (strncmp(loglevel, "LOG_NOTICE", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_NOTICE;
    else if (strncmp(loglevel, "LOG_WARNING", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_WARNING;
    else if (strncmp(loglevel, "LOG_ERR", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_ERR;
    else if (strncmp(loglevel, "LOG_CRIT", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_CRIT;
    else if (strncmp(loglevel, "LOG_ALERT", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_ALERT;
    else if (strncmp(loglevel, "LOG_EMERG", COMMON_MAX_STRING_LENGTH) == 0)
        mask = LOG_EMERG;
    else
        return -1;

    (void) setlogmask(LOG_UPTO(mask));
    /* Save localy the DMP log level configured */
    strncpy(wddi_og_level, loglevel, COMMON_MAX_STRING_LENGTH);
    wddi_og_level[COMMON_MAX_STRING_LENGTH - 1] = '\0';

    return 0;
}


/**
 * @function wddi_log_msg
 *
 * @brief  Generates a log message.
 *
 * @long   Assembles a valid log entry string from the supplied message format
 *         string and its argument list and writes it, depending on the message
 *         string's log priority, into the available system log.
 *
 * @input  priority  The message string's log priority.
 * @input  log_msg   The message format string.
 * @input  file      File
 * @input  line      Line
 * @input  ...       Variable-length argument list.
 *
 * @return None.
 */
int
wddi_log_msg(const unsigned int priority, char *file, unsigned int line, char *log_msg, ...)
{

    va_list args;
    va_start(args, log_msg);       /*lint !e530 varargs */
    char log_entry_str[1024];
    int cnt = 0;
#if 0
    struct tm *p;
    char timestr[40];
    time_t timep;
    time(&timep);
    p= localtime(&timep);
    sprintf(timestr,"%4d-%02d-%02d %2.2d:%2.2d:%2.2d",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    /* Assemble the log entry string. */
    cnt = snprintf(log_entry_str, 1024, "%s %s:%4d ", timestr,file, line);
#endif
    cnt = snprintf(log_entry_str, 1024, "%s:%4d ",file, line);

    if(cnt < 0)
    {
        cnt = 0;
    }

    (void)vsnprintf(&(log_entry_str[cnt]), 1024 - cnt, log_msg, args);

    /* Write into the specified syslog. */
    syslog(facility | priority, log_entry_str);
    va_end(args);

    return 0;
}
