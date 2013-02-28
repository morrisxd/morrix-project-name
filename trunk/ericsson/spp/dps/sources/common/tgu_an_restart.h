/*****************************************************************************
 * (C) Copyright Wintegra 2000-2002.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************
/****************************************************************************
*
* File: tgu_sgmii_an.h
*
* Description: This file holds the defines and structs definitions of TGU that handles 
*              AutoNegotiation For SGMII devices.
*              
*
* Module Originator: Elad Jacob
* Creation Date: August 9, 2011
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
#ifndef _TGU_SGMII_AN_H_
#define _TGU_SGMII_AN_H_

#define SGMII_TGU_WINNET_0_TANDBG_REG_OFFSET       0x1423c
#define SGMII_TGU_WINNET_0_TCNTR_REG_OFFSET        0x14200

#define SGMII_TGU_NEXT_ENET_IN_WINNET_GROUP_OFFSET 0x40
#define SGMII_TGU_NEXT_WINNET_OFFSET               0x2000

#define SGMII_TGU_AN_TCNTR_SGMII_DUPLEX_HALF 0
#define SGMII_TGU_AN_TCNTR_SGMII_DUPLEX_FULL 1

#define SGMII_TGU_REG_SIZE 4

#define SGMII_TGU_CLEAR_INDEX_FROM_PORT_MASK(PortMask, PortIndex) (PortMask &= ~(0x80000000 >> PortIndex))
#define SGMII_TGU_TANDBG_STUCK_BIT_MASK 0x00000180
#define SGMII_TGU_1000BaseX_DIV_FACTOR  7
#define SGMII_TGU_SW_COUNTER_INIT_VAL   1
#define SGMII_TGU_SW_COUNTER_OFFSET     0
register B_TGU_AN_RESTART_SW_COUNTER       = b423;

struct S_TandbgHwReg
{
      _bit                   : 16; //reserved
      
      _bit                   : 7; //reserved
      _bit AnDisableLinkOk   : 1;

      _bit LinkOk            : 1;
      _bit IdleDetect        : 1;
      _bit NextPageWait      : 1;
      _bit CompleteAck       : 1;
      _bit AckDetect         : 1;
      _bit AbilityDetect     : 1;
      _bit AnRestart         : 1;
      _bit AnEnable          : 1;
};


struct S_TcntrHwReg
{
      _bit                  : 16;
      _bit PhyReset         : 1;
      _bit                  : 1;
      _bit Speed1           : 1;
      _bit AnEnable         : 1;
      _bit                  : 2;
      _bit RestartAn        : 1;
      _bit DuplexMode       : 1;
      _bit                  : 1;
      _bit Speed0           : 1;
      _bit                  : 6;
};

struct S_Tcr2Reg
{
      _bit UD1              : 8;
      _bit UD2              : 2;
      _bit CyclicCounter    : 6;
      _bit DivFactor        : 16;
};
   
#endif //_TGU_SGMII_AN_H_
