/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_system.h
 *
 * Purpose: the UFE system internal structures.
 *
 ****************************************************************************/

#ifndef WUFEI_SYSTEM_H
#define WUFEI_SYSTEM_H

#define WUFE_SYS_EMPHY_ENTRY_SIZE 2
#define WUFE_SYS_EMPHY_ENTRY_TX_SIZE 4

#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif
#ifndef WUFEI_REGISTRY_H
#include "wufei_registry.h"
#endif

enum system_state_mode
{
   WUFEI_SYSTEM_DISABLE = 0,
   WUFEI_SYSTEM_ENABLE  = 1
};

/************************************/
/* the FIFO manager address         */
/* the Rx address is UFE base +0x10 */
/* the Rx address is UFE base +0x20 */
/************************************/
enum WUFE_fifo_manager_set
{
   WUFEI_RX = 0,
   WUFEI_TX
};

#define WUFEI_SystemHwConfigReleaseIdGet(system_registry) system_registry->hw_config.release_id
#define WUFEI_SystemHwConfigSecondaryReleaseIdGet(system_registry)\
        system_registry->hw_config.secondary_release_id
#define WUFEI_SystemHwConfigEmphyModeGet(system_registry)\
        system_registry->hw_config.emphy_setup_mode
#define WUFEI_SystemHwConfigCoreEnableGet(system_registry, coreid)\
        system_registry->hw_config.core_enabled[coreid]
#define WUFEI_SystemHwConfigZbtSizeGet(system_registry, coreid)\
        system_registry->hw_config.zbt_size[coreid]
#define WUFEI_SystemHwConfigZbtInternalGet(system_registry, coreid)\
        system_registry->hw_config.zbt_internal[coreid]

#define WUFEI_SystemClkRecHwConfigRxAdapUnitsGet(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_rec_rx.n_units
#define WUFEI_SystemClkRecHwConfigTxAdapUnitsGet(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_rec_tx.n_units
#define WUFEI_SystemClkRecHwConfigRxDiffUnitsGet(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_rec_rx.n_ext_diff_units
#define WUFEI_SystemClkRecHwConfigTxDiffUnitsGet(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_rec_tx.n_ext_diff_units

#define WUFEI_SystemClkRecHwConfigTxEn(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_rec_tx.mode
#define WUFEI_SystemClkRecHwConfigRxEn(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_rec_rx.mode

#define WUFEI_SystemClkRecHwConfigCtEn(system_registry) \
   ((system_registry->hw_config.clk_rec_hw_config == 0) ? WP_FALSE : \
    (system_registry->hw_config.clk_rec_hw_config)->clk_translator.mode)
#define WUFEI_SystemClkRecHwConfigCtAdapUnitsGet(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_translator.n_units;
#define WUFEI_SystemClkRecHwConfigCtDiffUnitsGet(system_registry) \
       (system_registry->hw_config.clk_rec_hw_config)->clk_translator.n_ext_diff_units;

WP_U32 WUFEI_SystemHwConfigLiuSbi(WUFEI_system_registry *system_registry);
WP_U32 WUFEI_SystemHwConfigLiuCad(WUFEI_system_registry *system_registry);
WP_U32 WUFEI_SystemHwConfigFullModel(WUFEI_system_registry *system_registry);
WP_U32 WUFEI_SystemHwConfigAtmMachineValid(WUFEI_system_registry *system_registry,
                                           WP_U32 coreid);
WP_U32 WUFEI_SystemHwConfigHdlcMachineValid(WUFEI_system_registry *system_registry,
                                            WP_U32 coreid);
WP_U32 WUFEI_SystemHwConfigTransMachineValid(WUFEI_system_registry *system_registry,
                                             WP_U32 coreid);
WP_U32 WUFEI_SystemHwConfigPosMachineValid(WUFEI_system_registry *system_registry,
                                           WP_U32 coreid);
WUFE_status WUFEI_SystemDelete(WP_U32 ufeid);
WUFE_status WUFEI_SystemDisable(WP_U32 wufe_id, WP_U32 property, WP_U32 direction);

#endif /*  WUFEI_SYSTEM_H*/
