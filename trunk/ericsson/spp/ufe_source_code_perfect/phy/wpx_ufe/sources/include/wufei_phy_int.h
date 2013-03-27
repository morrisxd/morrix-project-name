/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_emphy.h
 *
 * Purpose: the UFE EMPHY macro and structures
 *
 ****************************************************************************/

#ifndef WUFEI_PHY_INT_H
#define WUFEI_PHY_INT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_PHY_H
#include "wufei_hw_phy.h"
#endif

enum phy_state_mod
{
   WUFEI_PHY_EMPTY   = 0, /* get this value with memset to 0 */
   WUFEI_PHY_DISABLE = 1,
   WUFEI_PHY_ENABLE  = 2
   /*NOTE : do not change.
      many functions use empty as 0 and disable as 1.
   */
};

typedef enum
{
   WUFEI_PHY_CLK_REC_MASTER_NA = 0x00,
   WUFEI_PHY_CLK_REC_MASTER_TX = 0x01,
   WUFEI_PHY_CLK_REC_SLAVE_TX  = 0x02,
   WUFEI_PHY_CLK_REC_SLAVE_RX  = 0x04,
   WUFEI_PHY_CLK_REC_SLAVE_FULL_DUPLEX = (WUFEI_PHY_CLK_REC_SLAVE_TX |WUFEI_PHY_CLK_REC_SLAVE_RX)
}WUFEI_clk_rec_master;

WUFE_status WUFEI_PhySystemSetup(WUFEI_system_registry * system_registry);
WUFE_status WUFEI_PhySystemDelete(WUFEI_system_registry* system_registry);

WUFE_status  WUFEI_WriteRspt(WP_U32 wufe_id,
                             WP_U32 core_id,
                             WUFEI_rspt  rspt,
                             WUFEI_rspt *rspt_ptr);

WUFE_status  WUFEI_WriteTspt(WP_U32 wufe_id,
                             WP_U32 core_id,
                             WUFEI_tspt tspt,
                             WUFEI_tspt *tspt_ptr);

WUFE_status  WUFEI_ReadTspt(WP_U32 wufe_id,
                            WP_U32 core_id,
                            WUFEI_tspt *tspt,
                            WUFEI_tspt *tspt_ptr);

WUFE_status WUFEI_PhySystemTdmSlotsSetup(WUFEI_system_registry *system_registry,
                                         WP_U32 core_id);


WUFE_status WUFEI_PhyCheckIdleState(WP_U32 rx_enb,
                                    WP_U32 tx_enb,
                                    WUFEI_rspt *rspt,
                                    WUFEI_tspt *tspt,
                                    WP_U32 phy_id,
                                    WP_U32 wufe_id,
                                    WP_U32 core_id,
                                    WUFEI_system_registry *system_registry,
                                    WP_U32 inconsist_flag);

WUFE_status WUFEI_PhyDataUnitSizeGet(WUFEI_system_registry *system_registry,
                                     WP_U16 phy_id,
                                     WP_U32 *data_unit_size,  WP_U32 direction);


WUFE_status WUFEI_PhyRegistryGet(WUFEI_system_registry *system_registry,
                                 WP_U16 phy_id,
                                 WUFEI_phy_registry **phy_registry);

WUFE_status WUFEI_PhyNumSlotsGet(WUFEI_phy_registry *phy_registry,
                                 WP_U32 *n_slots, WP_U32 direction);


WUFE_status WUFEI_PhySetRxTxEnb(WP_U32 direction,WP_U32 *tx_enb,WP_U32 *rx_enb);

WUFE_status WUFEI_PhyClockRecTSUpdate(WP_U32 wufe_id,
                                      WP_U32 core_id,
                                      WP_U16 phy_id,
                                      WP_U16 clk_rec_if,
                                      WP_U32 direction,
                                      WP_U8 enable);

#define WUFEI_PhyLineIdGet(phy_registry) phy_registry->line_id
#define WUFEI_PhyClockRecStatusGet(phy_registry) phy_registry->master
#define WUFEI_PhyClockRecStatusSetRxSlave(phy_registry) \
   do { \
      phy_registry->master |= WUFEI_PHY_CLK_REC_SLAVE_RX;\
   } while (0)
#define WUFEI_PhyClockRecStatusSetTxSlave(phy_registry) \
   do { \
      phy_registry->master &= ~WUFEI_PHY_CLK_REC_MASTER_TX;\
      phy_registry->master |= WUFEI_PHY_CLK_REC_SLAVE_TX;\
   } while (0)
#define WUFEI_PhyClockRecStatusSetTxMaster(phy_registry) \
   do { \
      phy_registry->master &= ~WUFEI_PHY_CLK_REC_SLAVE_TX;\
      phy_registry->master |= WUFEI_PHY_CLK_REC_MASTER_TX;\
   } while (0)
#define WUFEI_PhyClockRecStatusSetRxNone(phy_registry) \
   do { \
      phy_registry->master &= ~WUFEI_PHY_CLK_REC_SLAVE_RX;     \
   } while (0)
#define WUFEI_PhyClockRecStatusSetTxNone(phy_registry) \
   do { \
      phy_registry->master &= ~WUFEI_PHY_CLK_REC_MASTER_TX;     \
      phy_registry->master &= ~WUFEI_PHY_CLK_REC_SLAVE_TX;     \
   } while (0)

#define WUFEI_PhyIdGet(phy_registry) phy_registry->phy_id
#define WUFEI_PhyRxActiveGet(phy_registry) phy_registry->rx_active
#define WUFEI_PhyTxActiveGet(phy_registry) phy_registry->tx_active

#endif /*  WUFEI_PHY_INT_H*/
