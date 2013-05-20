/*************************************************************
 * (C) Copyright 2003-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ************************************************************/
#define zero            $0
#define AT              $1
#define v0              $2
#define v1              $3
#define a0              $4
#define a1              $5
#define a2              $6
#define a3              $7
#define t0              $8
#define t1              $9
#define t2              $10
#define t3              $11
#define t4              $12
#define t5              $13
#define t6              $14
#define t7              $15
#define s0              $16
#define s1              $17
#define s2              $18
#define s3              $19
#define s4              $20
#define s5              $21
#define s6              $22
#define s7              $23
#define t8              $24
#define t9              $25
#define k0              $26
#define k1              $27
#define gp              $28
#define sp              $29
#define s8              $30
#define fp              $30
#define ra              $31

      .text


/************************************************************************
 *
 *                          _start
 *  Description :
 *  -------------
 *
 *  Function linked to application start address
 *
 *  Initialises sp and gp registers and clears bss section
 *
 *  Return values :
 *  ---------------
 *
 *  None
 *
 ************************************************************************/

      .globl _start
      .ent   _start

_start:

      .set noreorder

      /* Setup sp and gp */
      la    gp, _gp
      la    sp, _sp

      /* Clear bss */

      la    t0, _fbss	/* First address */
      la    t1, _end	/* Last  address */
1:
      sw    zero, 0(t0)
      bne   t0, t1, 1b
      addiu t0, 4

      /* Get ready to jump to main */
      move  s0, ra
      la    t0, main
      /* Jump to main */
      jal   t0
      nop

      /* Return to monitor */
      jr    s0
      nop

      .set reorder

      .end   _start




/************************************************************************
 *
 *                          __main, _gccmain
 *  Description :
 *  -------------
 *
 *  Dummy functions called by main() function.
 *
 *  GNU-gcc 2.8.1 : main() calls __main
 *  GNU-gcc 2.9   : main() calls __gccmain
 *
 *  Return values :
 *  ---------------
 *
 *  None
 *
 ************************************************************************/

      .set noreorder

      .globl __main
      .ent   __main

__main:
      jr	ra
      nop

      .end   __main



      .globl __gccmain
      .ent   __gccmain

__gccmain:
      jr	ra
      nop

      .end   __gccmain
