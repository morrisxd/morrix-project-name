/*************************************************************
 * (C) Copyright 2003-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/

#ifndef _ERRNO_
#define _ERRNO_

/* NOTE: In general, error codes should be handled through the "syserror"
 * system -- see "syserror.h".
 * The errno.h include file is only included for std c compatability reasons.
 */


/*
 * Error codes for standard library functions,
 * - add new ones when required
 */

#define ERANGE  34 /* Result not representable  */

extern int errno;

#endif /* _ERRNO_ */
