/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_core_int.h
 *
 * Purpose: the UFE Glue logic function declaration
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CORE_INT_H
#define WUFEI_HW_CORE_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

#define WUFEI_UfeCoreBaseAddrGet(ufeid, coreid) ufeid

WP_U32 WUFEI_UfeLiuBaseAddrGet(WP_U32 ufeid);

WP_U32 WUFEI_UfeClkRecBaseAddrGet(WP_U32 ufeid);

WP_U32 WUFEI_UfeCoreHwModelGet(WP_U32 ufeid);

WUFE_status WUFEI_UfeCoreHwConfigDelete(WUFEI_system_hw_config *hw_config);

WP_U32 WUFEI_UfeCoreLiuModeGet(WP_U32 ufeid);
#define WUFEI_UfeCoreBandwidthGet(ufeid) 0

WUFE_status WUFEI_UfeCoreRegistrySetup(WUFEI_system_registry* system_registry);

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

#endif /* WUFEI_HW_CORE_INT_H*/
