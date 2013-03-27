/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hw_cad.h
 *
 * Purpose: the UFE Line from CAD type macro and structures
 *
 ****************************************************************************/
#ifndef WUFEI_HW_CAD_H
#define WUFEI_HW_CAD_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif

#define WUFEI_HwCadTdmConfig(system_registry,line_registry,hw_cad_tdm_ram_id) WUFE_ERR_NOT_IMPLEMENTED
#define WUFEI_HwCadModeSet(ufeid, cad_id, value)
#define WUFEI_HwCadModeGet(ufeid, cad_id) 0
#define WUFEI_HwCadIdGet(ufeid, cad_id) 0

typedef struct
{/*HW configuration*/
      WUFEI_reg_space mode;   /* CAD mode register */
      WUFEI_reg_space id;     /* CAD id register */
      WUFEI_reg_space reserved[2];
}WUFEI_hw_cad_block;

typedef struct
{
      WUFEI_reg_space rx_int[4];
      WUFEI_reg_space tx_int[4];
/*    rx_int[0]: Cad 0-7 Ingress (RX) interrupt register
      rx_int[1]: Cad 8-15 Ingress (RX) interrupt register
      rx_int[2]: Cad 16-23 Ingress (RX) interrupt register
      rx_int[3]: Cad 24-31 Ingress (RX) interrupt register
      tx_int[0]: Cad 0-7 Egress (TX) interrupt register
      tx_int[1]: Cad 8-15 Egress (TX) interrupt register
      tx_int[2]: Cad 16-23 Egress (TX) interrupt register
      tx_int[3]: Cad 24-31 Egress (TX) interrupt register */
}WUFEI_hw_cad_events;

#define WUFEI_CAD_INT_NUM 4;

typedef struct
{
      WUFEI_hw_cad_events cad_events;
      WUFEI_reg_space reserved1[120]; /* Reserved for CAD future enhancements */
      WUFEI_hw_cad_block cad[WUFE_MAX_N_CAD/2];
}WUFEI_hw_cad_core;

typedef struct
{
      WUFEI_reg_space reserved1[17]; /* Reserved for CAD future enhancements */
      WUFEI_reg_space global_event_reg; /* Global CAD interrupt register */
      WUFEI_reg_space global_event_mask_reg; /* Global CAD mask register */
      WUFEI_reg_space reserved2[237]; /* Reserved for CAD future enhancements */
      WUFEI_hw_cad_core core[WUFE_MAX_N_CORE];
}WUFEI_hw_cad;

typedef enum
{/*HW configuration do not change.*/
   WUFEI_CAD_TRANSFER_T1         = 1,
   WUFEI_CAD_TRANSFER_NOT_T1_MODE= 0,
   WUFEI_CAD_TRANSFER_E1         = WUFEI_CAD_TRANSFER_NOT_T1_MODE,
   WUFEI_CAD_TRANSFER_T3         = WUFEI_CAD_TRANSFER_NOT_T1_MODE,
   WUFEI_CAD_TRANSFER_E3         = WUFEI_CAD_TRANSFER_NOT_T1_MODE
}WUFEI_cad_transfer;

typedef enum
{/*HW configuration do not change.*/
   WUFEI_CAD_TRANSFER_NATIVE  = 0,
   WUFEI_CAD_TRANSFER_HMVIP_4 = 1
}WUFEI_cad_multi;

typedef enum
{/*HW configuration do not change.*/
   WUFEI_SLOTED   = 0,
   WUFEI_UNSLOTED = 1
}WUFEI_un_sloted;

typedef enum
{
   WUFEI_CAD_RX  = 0,
   WUFEI_CAD_TX  = 1
}WUFEI_cad_direction;

#define WUFEI_EMPTY_TDM_CAD_ENTRY (0xffffffff)

/******************************************
 *   CAD global interrupt register        *
 *****************************************/
#define WUFEI_CAD_GLOBAL_INT_RX_CORE0_S (0)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE0_W (1)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE0_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_RX_CORE0_S)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE0_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_RX_CORE0_S, WUFEI_CAD_GLOBAL_INT_RX_CORE0_W)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE0_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_RX_CORE0_S, WUFEI_CAD_GLOBAL_INT_RX_CORE0_W)

#define WUFEI_CAD_GLOBAL_INT_TX_CORE0_S (1)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE0_W (1)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE0_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_TX_CORE0_S)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE0_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_TX_CORE0_S, WUFEI_CAD_GLOBAL_INT_TX_CORE0_W)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE0_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_TX_CORE0_S, WUFEI_CAD_GLOBAL_INT_TX_CORE0_W)

#define WUFEI_CAD_GLOBAL_INT_RX_CORE1_S (2)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE1_W (1)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE1_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_RX_CORE1_S)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE1_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_RX_CORE1_S, WUFEI_CAD_GLOBAL_INT_RX_CORE1_W)
#define WUFEI_CAD_GLOBAL_INT_RX_CORE1_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_RX_CORE1_S, WUFEI_CAD_GLOBAL_INT_RX_CORE1_W)

#define WUFEI_CAD_GLOBAL_INT_TX_CORE1_S (3)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE1_W (1)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE1_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_TX_CORE1_S)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE1_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_TX_CORE1_S, WUFEI_CAD_GLOBAL_INT_TX_CORE1_W)
#define WUFEI_CAD_GLOBAL_INT_TX_CORE1_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_TX_CORE1_S, WUFEI_CAD_GLOBAL_INT_TX_CORE1_W)

/******************************************
 *   CAD global interrupt MASK register   *
 *****************************************/
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_S (0)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_W (1)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_S)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_W)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE0_W)

#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_S (1)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_W (1)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_S)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_W)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE0_W)

#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_S (2)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_W (1)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_S)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_W)
#define WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_RX_CORE1_W)

#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_S (3)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_W (1)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_F(v) \
        WUFEI_FIELD(v, WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_S)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_W)
#define WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_M \
        WUFEI_MASK(WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_S, \
        WUFEI_CAD_GLOBAL_INT_MASK_TX_CORE1_W)


/*************************************
 *         CAD mode                  *
 ************************************/
#define WUFEI_CAD_RX_MODE_ENABLE_S (15)
#define WUFEI_CAD_RX_MODE_ENABLE_W (1)
#define WUFEI_CAD_RX_MODE_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_CAD_RX_MODE_ENABLE_S)
#define WUFEI_CAD_RX_MODE_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_RX_MODE_ENABLE_S, WUFEI_CAD_RX_MODE_ENABLE_W)
#define WUFEI_CAD_RX_MODE_ENABLE_M \
        WUFEI_MASK(WUFEI_CAD_RX_MODE_ENABLE_S, WUFEI_CAD_RX_MODE_ENABLE_W)

#define WUFEI_CAD_TX_MODE_ENABLE_S (14)
#define WUFEI_CAD_TX_MODE_ENABLE_W (1)
#define WUFEI_CAD_TX_MODE_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_CAD_TX_MODE_ENABLE_S)
#define WUFEI_CAD_TX_MODE_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_TX_MODE_ENABLE_S, WUFEI_CAD_TX_MODE_ENABLE_W)
#define WUFEI_CAD_TX_MODE_ENABLE_M \
        WUFEI_MASK(WUFEI_CAD_TX_MODE_ENABLE_S, WUFEI_CAD_TX_MODE_ENABLE_W)

#define WUFEI_CAD_MODE_MULTIFRAME_NUM_S (11)
#define WUFEI_CAD_MODE_MULTIFRAME_NUM_W (1)
#define WUFEI_CAD_MODE_MULTIFRAME_NUM_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_MULTIFRAME_NUM_S)
#define WUFEI_CAD_MODE_MULTIFRAME_NUM_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_MULTIFRAME_NUM_S, WUFEI_CAD_MODE_MULTIFRAME_NUM_W)
#define WUFEI_CAD_MODE_MULTIFRAME_NUM_M \
        WUFEI_MASK(WUFEI_CAD_MODE_MULTIFRAME_NUM_S, WUFEI_CAD_MODE_MULTIFRAME_NUM_W)

#define WUFEI_CAD_MODE_MULTI_FRAME_SYNC_S (10)
#define WUFEI_CAD_MODE_MULTI_FRAME_SYNC_W (1)
#define WUFEI_CAD_MODE_MULTI_FRAME_SYNC_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_MULTI_FRAME_SYNC_S)
#define WUFEI_CAD_MODE_MULTI_FRAME_SYNC_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_MULTI_FRAME_SYNC_S, WUFEI_CAD_MODE_MULTI_FRAME_SYNC_W)
#define WUFEI_CAD_MODE_MULTI_FRAME_SYNC_M \
        WUFEI_MASK(WUFEI_CAD_MODE_MULTI_FRAME_SYNC_S, WUFEI_CAD_MODE_MULTI_FRAME_SYNC_W)

#define WUFEI_CAD_MODE_UN_FRAME_MODE_S (9)
#define WUFEI_CAD_MODE_UN_FRAME_MODE_W (1)
#define WUFEI_CAD_MODE_UN_FRAME_MODE_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_UN_FRAME_MODE_S)
#define WUFEI_CAD_MODE_UN_FRAME_MODE_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_UN_FRAME_MODE_S, WUFEI_CAD_MODE_UN_FRAME_MODE_W)
#define WUFEI_CAD_MODE_UN_FRAME_MODE_M \
        WUFEI_MASK(WUFEI_CAD_MODE_UN_FRAME_MODE_S, WUFEI_CAD_MODE_UN_FRAME_MODE_W)

#define WUFEI_CAD_MODE_M_FRAME_SYNC_S (8)
#define WUFEI_CAD_MODE_M_FRAME_SYNC_W (1)
#define WUFEI_CAD_MODE_M_FRAME_SYNC_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_M_FRAME_SYNC_S)
#define WUFEI_CAD_MODE_M_FRAME_SYNC_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_M_FRAME_SYNC_S, WUFEI_CAD_MODE_M_FRAME_SYNC_W)
#define WUFEI_CAD_MODE_M_FRAME_SYNC_M \
        WUFEI_MASK(WUFEI_CAD_MODE_M_FRAME_SYNC_S, WUFEI_CAD_MODE_M_FRAME_SYNC_W)

#define WUFEI_CAD_MODE_UN_SLOT_S (7)
#define WUFEI_CAD_MODE_UN_SLOT_W (1)
#define WUFEI_CAD_MODE_UN_SLOT_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_UN_SLOT_S)
#define WUFEI_CAD_MODE_UN_SLOT_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_UN_SLOT_S, WUFEI_CAD_MODE_UN_SLOT_W)
#define WUFEI_CAD_MODE_UN_SLOT_M \
        WUFEI_MASK(WUFEI_CAD_MODE_UN_SLOT_S, WUFEI_CAD_MODE_UN_SLOT_W)

#define WUFEI_CAD_MODE_CAS_ENABLE_S (5)
#define WUFEI_CAD_MODE_CAS_ENABLE_W (1)
#define WUFEI_CAD_MODE_CAS_ENABLE_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_CAS_ENABLE_S)
#define WUFEI_CAD_MODE_CAS_ENABLE_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_CAS_ENABLE_S, WUFEI_CAD_MODE_CAS_ENABLE_W)
#define WUFEI_CAD_MODE_CAS_ENABLE_M \
        WUFEI_MASK(WUFEI_CAD_MODE_CAS_ENABLE_S, WUFEI_CAD_MODE_CAS_ENABLE_W)

#define WUFEI_CAD_MODE_CAS_ALIGN_S (4)
#define WUFEI_CAD_MODE_CAS_ALIGN_W (1)
#define WUFEI_CAD_MODE_CAS_ALIGN_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_CAS_ALIGN_S)
#define WUFEI_CAD_MODE_CAS_ALIGN_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_CAS_ALIGN_S, WUFEI_CAD_MODE_CAS_ALIGN_W)
#define WUFEI_CAD_MODE_CAS_ALIGN_M \
        WUFEI_MASK(WUFEI_CAD_MODE_CAS_ALIGN_S, WUFEI_CAD_MODE_CAS_ALIGN_W)

#define WUFEI_CAD_MODE_T1_S (3)
#define WUFEI_CAD_MODE_T1_W (1)
#define WUFEI_CAD_MODE_T1_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_T1_S)
#define WUFEI_CAD_MODE_T1_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_T1_S, WUFEI_CAD_MODE_T1_W)
#define WUFEI_CAD_MODE_T1_M \
        WUFEI_MASK(WUFEI_CAD_MODE_T1_S, WUFEI_CAD_MODE_T1_W)

#define WUFEI_CAD_MODE_LINE_LB_S (2)
#define WUFEI_CAD_MODE_LINE_LB_W (1)
#define WUFEI_CAD_MODE_LINE_LB_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_LINE_LB_S)
#define WUFEI_CAD_MODE_LINE_LB_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_LINE_LB_S, WUFEI_CAD_MODE_LINE_LB_W)
#define WUFEI_CAD_MODE_LINE_LB_M \
        WUFEI_MASK(WUFEI_CAD_MODE_LINE_LB_S, WUFEI_CAD_MODE_LINE_LB_W)

#define WUFEI_CAD_MODE_SYS_LB_S (1)
#define WUFEI_CAD_MODE_SYS_LB_W (1)
#define WUFEI_CAD_MODE_SYS_LB_F(v) WUFEI_FIELD(v, WUFEI_CAD_MODE_SYS_LB_S)
#define WUFEI_CAD_MODE_SYS_LB_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_MODE_SYS_LB_S, WUFEI_CAD_MODE_SYS_LB_W)
#define WUFEI_CAD_MODE_SYS_LB_M \
        WUFEI_MASK(WUFEI_CAD_MODE_SYS_LB_S, WUFEI_CAD_MODE_SYS_LB_W)

/*************************************
 *         CAD ID                    *
 ************************************/

#define WUFEI_CAD_ID_E1_T1_S (0)
#define WUFEI_CAD_ID_E1_T1_W (7)
#define WUFEI_CAD_ID_E1_T1_F(v) WUFEI_FIELD(v, WUFEI_CAD_ID_E1_T1_S)
#define WUFEI_CAD_ID_E1_T1_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_ID_E1_T1_S, WUFEI_CAD_ID_E1_T1_W)
#define WUFEI_CAD_ID_E1_T1_M \
        WUFEI_MASK(WUFEI_CAD_RX_ID_E1_T1_S, WUFEI_CAD_ID_E1_T1_W)

#define WUFEI_CAD_ID_HMVIP_4_S (2)
#define WUFEI_CAD_ID_HMVIP_4_W (5)
#define WUFEI_CAD_ID_HMVIP_4_F(v) WUFEI_FIELD(v, WUFEI_CAD_ID_HMVIP_4_S)
#define WUFEI_CAD_ID_HMVIP_4_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_ID_HMVIP_4_S, WUFEI_CAD_ID_HMVIP_4_W)
#define WUFEI_CAD_ID_HMVIP_4_M \
        WUFEI_MASK(WUFEI_CAD_RX_ID_HMVIP_4_S, WUFEI_CAD_ID_HMVIP_4_W)

#define WUFEI_CAD_ID_E3_T3_S (5)
#define WUFEI_CAD_ID_E3_T3_W (2)
#define WUFEI_CAD_ID_E3_T3_F(v) WUFEI_FIELD(v, WUFEI_CAD_ID_E3_T3_S)
#define WUFEI_CAD_ID_E3_T3_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_ID_E3_T3_S, WUFEI_CAD_ID_E3_T3_W)
#define WUFEI_CAD_ID_E3_T3_M \
        WUFEI_MASK(WUFEI_CAD_RX_ID_E3_T3_S, WUFEI_CAD_ID_E3_T3_W)

#define WUFEI_CAD_ID_SPE_S (5)
#define WUFEI_CAD_ID_SPE_W (2)
#define WUFEI_CAD_ID_SPE_F(v) WUFEI_FIELD(v, WUFEI_CAD_ID_SPE_S)
#define WUFEI_CAD_ID_SPE_V(f) \
        WUFEI_VALUE(f, WUFEI_CAD_ID_SPE_S, WUFEI_CAD_ID_SPE_W)
#define WUFEI_CAD_ID_SPE_M \
        WUFEI_MASK(WUFEI_CAD_RX_ID_SPE_S, WUFEI_CAD_ID_SPE_W)

#endif /* WUFEI_HW_CAD_H*/
