/*****************************************************************************
 * (C) Copyright Wintegra 2000-2006.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************
/****************************************************************************
*
* File: example.h
*
* Description: Contains the defenitions, structures and declarations used
*              throughout the project
*
****************************************************************************/

#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

/*preprocessor definitions*/
#define SERIAL_LAST 0x080
#define SIZE_OF_DATA 64
#define NULL 0
#define CTR_SIZE 4
#define NUM_OF_KEYS 1
#define SIZE_OF_KEY 4
#define mask 0x000001ff

/*Structures*/

struct S_SerialStatusReg2
{
      _bit   SC                : 8 ;// Serial Count ID
      _bit                     : 1 ;//
      _bit   SA                : 1 ;// Service Address
      _bit   NoData            : 1 ;//
      _bit   S0                : 5 ;// Serial Order
      _bit   PN                : 8 ;// Phy Number note in EMP PN come from Port Descriptor 
      _bit   OCHL              : 8 ;// Over Size Cell Header Length.
};

struct Mode_Register
{
  _bit      : 24 ;
  _bit map  : 8  ;
};

struct Mode_Register2
   {
  _bit      : 8 ;
  _bit map  : 8  ;
  _bit      : 16 ;
};

struct Cell 
{
  _bit header  : 32;
  _bit         : 504;
  
};

struct LabelTag
{
      _bit   Offset            : 16 ;// Table offset (e.g ATM channel number)
      _bit   TagLabel          : 8  ;// Specific Tag Label
      _bit                     : 4  ;// Reserved
      _bit   LabelMode         : 1  ;// Tag mode is Label
      _bit                     : 1  ;// Resrved
      _bit                     : 1  ;// Resrved
      _bit                     : 1  ;// Resrved
};


/*Function declarations*/
func F_Receive();
func F_Transmit();





















#endif //__EXAMPLE_H__