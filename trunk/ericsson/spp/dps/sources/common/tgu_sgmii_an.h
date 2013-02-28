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

#define SGMII_TGU_AN_WINNET_0_TSTAT_REG_OFFSET        0x14204
#define SGMII_TGU_AN_WINNET_0_TANL_REG_OFFSET         0x14214
#define SGMII_TGU_AN_WINNET_0_PHY_0_MAC1_REG_OFFSET   0x14080
#define SGMII_TGU_AN_WINNET_0_WNC_2_REG_OFFSET        0x14004

#define SGMII_TGU_AN_NEXT_ENET_IN_WINNET_GROUP_TSTAT_OFFSET 0x40
#define SGMII_TGU_AN_NEXT_ENET_IN_WINNET_GROUP_TANL_OFFSET  0x40
#define SGMII_TGU_AN_NEXT_ENET_IN_WINNET_GROUP_MAC1_OFFSET  0x10
#define SGMII_TGU_AN_NEXT_WINNET_OFFSET                     0x2000

#define SGMII_TGU_AN_WNC2_GMII_MODE 0
#define SGMII_TGU_AN_WNC2_MII_MODE  1

#define SGMII_TGU_AN_TANL_SGMII_DUPLEX_HALF 0
#define SGMII_TGU_AN_TANL_SGMII_DUPLEX_FULL 1

#define SGMII_TGU_AN_TANL_SGMII_SPEED_1000  2

#define SGMII_TGU_AN_REG_SIZE 4

struct S_TanlHwReg
{
      _bit                   : 16; //reserved
      
      _bit LinkStatus        : 1;
      _bit AutoNegAck        : 1;
      _bit                   : 1;
      _bit DuplexMode        : 1;
      _bit Speed             : 2;
      _bit                   : 2;
      _bit                   : 8;
};

struct S_Wnc2HwReg
{
      _bit                   : 2;
      _bit BigFifoInterface0 : 1;
      _bit MiiModeGroup1     : 1;
      _bit MiiModeGroup0     : 1;
      _bit                   : 3;
      
      _bit                   : 2;
      _bit UserInfo          : 14;
      
      _bit RgmiiIOType1        : 1;
      _bit RgmiiIOType0        : 1;
      _bit InterfaceTypeGroup1 : 3;
      _bit InterfaceTypeGroup0 : 3;
};

struct S_Mac1HwReg
{
      _bit SoftReset        : 1;
      _bit SimulationReset  : 1;
      _bit                  : 6;
      
      _bit                  : 2;
      _bit ResetRxFifo      : 1;
      _bit ResetTxFifo      : 1;
      _bit ResetRxMac       : 1;
      _bit ResetTxMac       : 1;
      _bit ResetRxFunc      : 1;
      _bit ResetTxFunc      : 1;
      
      _bit                  : 7;
      _bit Loopback         : 1;
      
      _bit                  : 2;
      _bit RxFlow           : 1;
      _bit TxFlow           : 1;
      _bit SyncRxEnable     : 1;
      _bit RxEnable         : 1;
      _bit SyncTxEnable     : 1;
      _bit TxEnable         : 1;
};

struct S_TstatHwReg
{
      _bit                  : 26;
      _bit AutoNegComplete  : 1;
      _bit                  : 5;
};

struct S_Tcr2Reg
{
      _bit UD1              : 8;
      _bit UD2              : 2;
      _bit CyclicCounter    : 6;
      _bit DivFactor        : 16;
};

#endif //_TGU_SGMII_AN_H_
