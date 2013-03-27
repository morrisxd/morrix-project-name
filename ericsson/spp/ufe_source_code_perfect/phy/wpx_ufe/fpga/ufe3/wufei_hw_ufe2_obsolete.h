/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_ufe2_obsolete.h
 *
 * Purpose: the ufe feature definitions needed for the code compelation
 *
 ****************************************************************************/
#ifndef WUFEI_HW_UFE2_OBSOLETE_H
#define WUFEI_HW_UFE2_OBSOLETE_H


/*
  if UPI do NOT set the dum bit for phy number 127
  uncommnet the define
  #define WUFEI_UPI_DUM_BIT_NOT_SET 1
 */
#define WUFEI_N_MAX_CAD_E3          WUFEI_MIN(3,WUFE_MAX_N_CAD_LINE_DS3)
#define WUFEI_N_MAX_CAD_DS3         WUFEI_MIN(3,WUFE_MAX_N_CAD_LINE_DS3)/*END OF CHECK*/

#define WUFE_MAX_SLOTS_PER_CAD  WUFE_N_SLOTS_E1

/************************************************/
/* Global System Configuration registers */
/************************************************/
#define WUFEI_GEN_CFG_ID_REL_REG_BASE    (0x003 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_GEN_CFG_SYS_REG_BASE       (0x004 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_GEN_CFG_ID_SERIAL_REG_BASE (0x005 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_GEN_CFG_CLK_RECVR_REG_BASE (0x008 * WUFEI_ADDR_ABS_TO_DRV)

/************************************/
/* the CAD  Interrupt address       */
/************************************/
#define WUFEI_CAD_EVENT_BASE (0x0280 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_CAD_EVENT ((WUFEI_hw_cad_events *) (WUFEI_CAD_EVENT_BASE))
/************************************************/
/* the General Timestamp configuration register */
/************************************************/
#define WUFEI_TIMESTAMP_GENERAL_CFG_REG_BASE (0x470 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_TIMESTAMP_GENERAL_CFG_REG ((WP_U16 *)(WUFEI_TIMESTAMP_GENERAL_CFG_REG_BASE))

/************************************/
/* the general info register        */
/************************************/
#define WUFEI_SYS_CONFIG_ID_REG (0x0004 * WUFEI_ADDR_ABS_TO_DRV)

/************************************/
/* the RESEST UFE2 core  register   */
/************************************/
#define WUFEI_UFE2_RESET_REG (0x0010 * WUFEI_ADDR_ABS_TO_DRV)

#define WUFEI_UFE2_RESET_S (15)
#define WUFEI_UFE2_RESET_W (1)
#define WUFEI_UFE2_RESET_F(v)  WUFEI_FIELD(v, WUFEI_UFE2_RESET_S)
#define WUFEI_UFE2_RESET_V(f) WUFEI_VALUE(f, WUFEI_UFE2_RESET_S, WUFEI_UFE2_RESET_W)
#define WUFEI_UFE2_RESET_M WUFEI_MASK(WUFEI_UFE2_RESET_S, WUFEI_UFE2_RESET_W)

/************************************************/
/* Global System Configuration registers */
/************************************************/
#define WUFEI_GEN_CFG_ID_REL_REG_BASE    (0x003 * WUFEI_ADDR_ABS_TO_DRV)

#define WUFEI_GEN_CFG_SYS_REG_BASE       (0x004 * WUFEI_ADDR_ABS_TO_DRV)

#define WUFEI_GEN_CFG_SYS_REG_HM_S (15)
#define WUFEI_GEN_CFG_SYS_REG_HM_W (1)
#define WUFEI_GEN_CFG_SYS_REG_HM_V(f) \
   WUFEI_VALUE(f, WUFEI_GEN_CFG_SYS_REG_HM_S, WUFEI_GEN_CFG_SYS_REG_HM_W)
#define WUFEI_GEN_CFG_SYS_REG_HM_FULL      0
#define WUFEI_GEN_CFG_SYS_REG_HM_CORE_ONLY 1

#define WUFEI_GEN_CFG_SYS_REG_LIFM_S (8)
#define WUFEI_GEN_CFG_SYS_REG_LIFM_W (4)
#define WUFEI_GEN_CFG_SYS_REG_LIFM_V(f) \
   WUFEI_VALUE(f, WUFEI_GEN_CFG_SYS_REG_LIFM_S, WUFEI_GEN_CFG_SYS_REG_LIFM_W)

#define WUFEI_GEN_CFG_SYS_REG_LIFM_SBI 1
#define WUFEI_GEN_CFG_SYS_REG_LIFM_CAD 2

#define WUFEI_GEN_CFG_SYS_REG_CT_S (7)
#define WUFEI_GEN_CFG_SYS_REG_CT_W (1)
#define WUFEI_GEN_CFG_SYS_REG_CT_V(f) \
   WUFEI_VALUE(f, WUFEI_GEN_CFG_SYS_REG_CT_S, WUFEI_GEN_CFG_SYS_REG_CT_W)

#define WUFEI_GEN_CFG_CLK_RECVR_REG_CT_S (0)
#define WUFEI_GEN_CFG_CLK_RECVR_REG_CT_W (7)
#define WUFEI_GEN_CFG_CLK_RECVR_REG_CT_V(f) \
   WUFEI_VALUE(f, WUFEI_GEN_CFG_CLK_RECVR_REG_CT_S, WUFEI_GEN_CFG_CLK_RECVR_REG_CT_W)


#endif /*WUFEI_HW_CONFIG_H*/
