/* $Id: generic.h,v 1.1.1.1 2003/12/03 11:59:00 arnonm Exp $ */
/*****************************************************************************
 * (C) Copyright Wintegra 2000, All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/
/*****************************************************************************
 *
 * Module: $RCSfile: generic.h,v $
 * Purpose:
 *     Generic definitions and types.
 *
 * Module Originator: George Agasandian (Login Name: 'george') 
 * Creation Date: Thu Nov 02 11:51:14 2000
 *
 * Change History: 
 * +------------+--------------------+----------------------------------------
 * | Date       | By                 | Description
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 * |            |                    |
 * +------------+--------------------+----------------------------------------
 *
 *****************************************************************************/

#ifndef __GENERIC_H__
#define __GENERIC_H__

 
#define NL "\n"
 
typedef unsigned long  ULONG;
typedef unsigned int   UINT;
typedef unsigned char  UCHAR;
typedef unsigned short USHORT;

#ifndef WIN32
typedef long long          DLONG;
typedef unsigned long long UDLONG;
#else
#include "stdafx.h"
typedef hyper              DLONG;
typedef unsigned hyper     UDLONG;
#endif


typedef ULONG          Word;
typedef USHORT         HalfWord;
typedef UCHAR          Byte;

typedef ULONG          Address;
typedef Address        Addr;


/*
 * BOOLEAN TYPE AND VALUES
 */
typedef void       (*Bool)();
#define BOOL_TRUE  ((Bool)1)
#define BOOL_FALSE ((Bool)0)

/*
 * MACROS
 */
#ifndef min
#define min(x,y) ((x<y)?x:y)
#endif
#ifndef max
#define max(x,y) ((x>y)?x:y)
#endif
#define bit_set(n)    (1<<n)
#define bit_chk(data,bit) (data & bit_set(bit))

typedef union
{
      Word   W;
      UCHAR  B[4];
}
SwapWordUnit;

typedef union
{
    UCHAR  uc;
    UCHAR  ub:8;
}
SwapByteUnit;



/*
 * COLORS
 */
#define COLOR_FG_BLACK   "[30m"
#define COLOR_FG_RED     "[31m"
#define COLOR_FG_GREEN   "[32m"
#define COLOR_FG_YELLOW  "[33m"
#define COLOR_FG_BLUE    "[34m"
#define COLOR_FG_MAGENTA "[35m"
#define COLOR_FG_CYAN    "[36m"
#define COLOR_FG_WHITE   "[37m"
#define COLOR_BG_BLACK   "[40m"
#define COLOR_BG_RED     "[41m"
#define COLOR_BG_GREEN   "[42m"
#define COLOR_BG_YELLOW  "[43m"
#define COLOR_BG_BLUE    "[44m"
#define COLOR_BG_MAGENTA "[45m"
#define COLOR_BG_CYAN    "[46m"
#define COLOR_BG_WHITE   "[47m"
#define COLOR_NORMAL     "[0m"
#define COLOR_UNDERLINE  "[4m"
#define COLOR_BOLD       "[5m"
#define COLOR_INVERT     "[7m"



#endif /* __GENERIC_H__ */


/*****************************************************************************
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 *****************************************************************************/

