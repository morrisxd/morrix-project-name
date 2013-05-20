/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
/*****************************************************************************
 *
 * Module: $RCSfile: sockint.h,v $
 * Purpose:
 *      This file implements the driver/simulator interface using
 *  sockets.
 *
 * Module Originator: Arnon Mordoh (Login Name: 'arnonm')
 * Creation Date: Oct 23, 2000
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
 ****************************************************************************/
#ifndef _SOCKINT_H_
#define _SOCKINT_H_

#define WINPATH_DEFAULT_SIMULATOR_PORT  1050
#define WINPATH_DEFAULT_VERILOG_PORT    1051

#define SOCKINT_OK       0
#define SOCKINT_ERROR    1
#define SOCKINT_TIMEOUT  2
#define SOCKINT_TERM     3

int InitSocketInterface(void);
int CleanUpSocket(int perm);
int HandleClientRequest(void);       /* Used by server to handle requests */

int WriteByte(unsigned int address, unsigned int value);
int WriteShort(unsigned int address, unsigned int value);
int WriteLong(unsigned int address, unsigned int value);
int WriteBlock(unsigned int address, unsigned char *pval, int size);
int WriteBlockSwap(unsigned int address, unsigned char *pval, int size);

int ReadByte(unsigned int address, unsigned int *pvalue);
int ReadShort(unsigned int address, unsigned int *pvalue);
int ReadLong(unsigned int address, unsigned int *pvalue);
int ReadBlock(unsigned int address, unsigned char *pval, int size);
int ReadBlockSwap(unsigned int address, unsigned char *pval, int size);

void SimReadBlock(unsigned int addr, unsigned char * buffer,
                  unsigned char size, int IsInternal);
void SimWriteBlock(unsigned int addr, unsigned char * buffer,
                   unsigned char size, int IsInternal);

int WriteCmdFile( char* file_buf, unsigned long file_size );

void WPI_ReadByte(unsigned address, unsigned char *pvalue);
void WPI_ReadShort(unsigned address, unsigned short *pvalue);
void WPI_ReadLong(unsigned int address, unsigned int *pvalue);
void WPI_WriteShort(unsigned int address, unsigned int value);
void WPI_WriteLong(unsigned int address, unsigned int value);
#endif
/*****************************************************************************
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 ****************************************************************************/
