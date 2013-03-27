/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_hardware_config.h
 *
 * Purpose: the user define global definitions for the UFE
 *
 ****************************************************************************/

#ifndef WUFEI_HW_CONFIG_H
#define WUFEI_HW_CONFIG_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_GEN_REGS_H
#include "wufei_hw_gen_regs.h"
#endif

#ifndef WUFE_USER_CONFIG_H
#include "wufe_user_config.h"
#endif

#ifndef WUFEI_HW_GLOBALS_H
#include "wufei_hw_globals.h"
#endif

#ifndef WUFEI_HW_EMPHY_H
#include "wufei_hw_emphy.h"
#endif

#define WUFEI_FPGA_TYPE  WUFEI_FPGA_TYPE_UFE4

/**************************************************
 * Address translation
 * Example for board not using 3 LSB of the bus
 *
 * DRV address is * 2 from the ABS
 *
 * HW  address is * 8 from the ABS
 *
 *************************************************/

#define WUFEI_ADDR_ABS_TO_HW  (1 << WUFEI_ADDRSHIFT)
#define WUFEI_ADDR_ABS_TO_DRV (2)
#define WUFEI_ADDR_DRV_TO_HW  ( WUFEI_ADDR_ABS_TO_HW /  WUFEI_ADDR_ABS_TO_DRV )

/* when the driver is connecting the indirect address space it ought to use following define*/
#define WUFEI_INDRCT_ADDR_ABS_TO_DRV (2)

#define WUFEI_INDIRECT_TIMEOUT 10000
#define WUFEI_INDIRECT_BUSY_WAIT 0x1
#define WUFEI_INDIRECT_RD_CMD 0x8000

#ifdef UFE_412_1344
 #define WUFEI_TDM_NUM_REGISTERS (0x540)
 #define WUFEI_RESERVED3_NUM_REGISTERS (0x2c0)
 #define WUFEI_MACHINE_NUM_REGISTERS (0x540)
 #define WUFEI_FIFO_MANAGER_NUM_REGISTERS (0x540)
 #define WUFEI_RESERVED1_NUM_REGISTERS (0x800)
#else
#define WUFEI_TDM_NUM_REGISTERS (0x800)
#define WUFEI_MACHINE_NUM_REGISTERS (0x800)
#define WUFEI_FIFO_MANAGER_NUM_REGISTERS (0x800)
#define WUFEI_RESERVED1_NUM_REGISTERS (0x800)
#endif

#define WUFEI_RESERVED2_NUM_REGISTERS (0x8000)
#define WUFEI_EMPHY_NUM_REGISTERS (0x2000)
#define WUFEI_GLOBAL_NUM_REGISTERS (0x2000)
#define WUFEI_BOARD_SUPPORT_NUM_REGISTERS (0x4000)
#define WUFEI_CLK_REC_NUM_REGISTERS (0x8000)

#ifdef UFE_412_1344
typedef struct
{
   WUFEI_reg_space tdm[WUFEI_TDM_NUM_REGISTERS];
   WUFEI_reg_space reserved1[WUFEI_RESERVED3_NUM_REGISTERS];
   WUFEI_reg_space machine[WUFEI_MACHINE_NUM_REGISTERS];
   WUFEI_reg_space reserved2[WUFEI_RESERVED3_NUM_REGISTERS];
   WUFEI_reg_space fm[WUFEI_FIFO_MANAGER_NUM_REGISTERS];
   WUFEI_reg_space reserved3[WUFEI_RESERVED3_NUM_REGISTERS];
   WUFEI_reg_space reserved[WUFEI_RESERVED1_NUM_REGISTERS];
}WUFEI_hw_cores;

#else
typedef struct
{
   WUFEI_reg_space tdm[WUFEI_TDM_NUM_REGISTERS];
   WUFEI_reg_space machine[WUFEI_MACHINE_NUM_REGISTERS];
   WUFEI_reg_space fm[WUFEI_FIFO_MANAGER_NUM_REGISTERS];
   WUFEI_reg_space reserved[WUFEI_RESERVED1_NUM_REGISTERS];
}WUFEI_hw_cores;
#endif

typedef struct
{
   WUFEI_hw_cores  core[WUFEI_MAX_N_CORE];
   WUFEI_reg_space emphy[WUFEI_EMPHY_NUM_REGISTERS];
   WUFEI_reg_space globals[WUFEI_GLOBAL_NUM_REGISTERS];
   WUFEI_reg_space clk_rec[WUFEI_CLK_REC_NUM_REGISTERS];
   WUFEI_reg_space reserved[WUFEI_RESERVED2_NUM_REGISTERS];
   WUFEI_reg_space bsp[WUFEI_BOARD_SUPPORT_NUM_REGISTERS];
}WUFEI_hw_ufe;

#define WUFEI_CAD_CORE_ID_GET(cad_id) WUFE_CORE_ID_0

/*sclk - user in TDM system loopback*/
#define WUFEI_SYS_TRIB_CLK_RATIO_T1 (0x6E)/*170MHz*/
#define WUFEI_SYS_TRIB_CLK_RATIO_E1 (0x53)/*170MHz*/

#define WUFEI_HW_MINIMUM_FIFO_MEM_SIZE  128
#define WUFEI_HW_INTERNAL_FIFO_MEM_SIZE 116

#define WUFEI_N_MAX_CAD_HMVIP_4_E1  WUFE_MAX_N_CAD_LINE_HMVIP
#define WUFEI_N_MAX_CAD_HMVIP_4_T1  WUFE_MAX_N_CAD_LINE_HMVIP
#define WUFE_MAX_HMVIP_INUSE        WUFE_HMVIP_NUM_4

/************************************/
/*     Memory address               */
/************************************/
/* Receive state parameter table*/
#define WUFEI_INDIRECT_MEM_ADD_RSPT_START     ( 0 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_MEM_ADD_RSPT_END       ( 0x001A40 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)

/* Transmit state parameter table*/
#define WUFEI_INDIRECT_MEM_ADD_TSPT_START     ( 0x010000 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_MEM_ADD_TSPT_END       ( 0x011A40 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)

/* Receive tdm table*/
#define WUFEI_INDIRECT_MEM_ADD_RTDM_START     ( 0x020000 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_MEM_ADD_RTDM_END       ( 0x024000 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)

/* Transmit tdm table*/
#define WUFEI_INDIRECT_MEM_ADD_TTDM_START     ( 0x028000 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_MEM_ADD_TTDM_END       ( 0x02C000 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)

/* External FIFO tables (ZBT)*/
#define WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_SIZE  ( 0x100000 ) /* 1 Mbyte ZBT*/
#define WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START ( 0x200000 * WUFEI_INDRCT_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_END   ( WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_START +\
                                                WUFEI_INDIRECT_MEM_ADD_EXT_FIFO_SIZE )

/* Receive SBI MODE RAM RX*/
#define WUFEI_INDIRECT_MEM_ADD_SBI_MODE_RX_START   ( 0x000000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_MODE_RX_END     ( 0x0000A8 )

/* Receive SBI State Machine RAM RX*/
#define WUFEI_INDIRECT_MEM_ADD_SBI_STATE_RX_START  ( 0x001000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_STATE_RX_END    ( 0x001150 )

/* Receive SBI ALM/V5 RAM RX */
#define WUFEI_INDIRECT_MEM_ADD_SBI_RX_ALM_V5_START ( 0x002000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_RX_ALM_V5_END   ( 0x0020A8 )

/* Transmit SBI MODE RAM TX*/
#define WUFEI_INDIRECT_MEM_ADD_SBI_MODE_TX_START   ( 0x003000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_MODE_TX_END     ( 0x0030A8 )

/* Transmit SBI State Machine RAM TX*/
#define WUFEI_INDIRECT_MEM_ADD_SBI_STATE_TX_START  ( 0x004000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_STATE_TX_END    ( 0x0040A8 )

/* Receive SBI CAS RAM RX*/
#define WUFEI_INDIRECT_MEM_ADD_SBI_CAS_RX_START   ( 0x005000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_CAS_RX_END     ( 0x005540 )

/* Transmit SBI CAS RAM TX*/
#define WUFEI_INDIRECT_MEM_ADD_SBI_CAS_TX_START   ( 0x006000 )
#define WUFEI_INDIRECT_MEM_ADD_SBI_CAS_TX_END     ( 0x006540 )

/* Clock Generator RX tributary programable registers RAM */
#define WUFEI_INDIRECT_MEM_ADD_CGEN_RX_RAM_START     ( 0x010000 )
#define WUFEI_INDIRECT_MEM_ADD_CGEN_RX_RAM_END       ( 0x010150 )

/* Clock Translator TX tributary programable registers RAM */
#define WUFEI_INDIRECT_MEM_ADD_CLK_TRANS_RAM_START   ( 0x011000 )
#define WUFEI_INDIRECT_MEM_ADD_CLK_TRANS_RAM_END     ( 0x011150 )

/* Clock Recovery Timestamp RX port RAM*/
#define WUFEI_INDIRECT_MEM_ADD_TS_RX_PORT_RAM_START  ( 0x000000 )
#define WUFEI_INDIRECT_MEM_ADD_TS_RX_PORT_RAM_END    ( 0x000100 )

/* Clock Recovery Timestamp RX mask RAM*/
#define WUFEI_INDIRECT_MEM_ADD_TS_RX_MASK_RAM_START  ( 0x000200 )
#define WUFEI_INDIRECT_MEM_ADD_TS_RX_MASK_RAM_END    ( 0x000300 )

/* the UFE buffers on the ZBT has the following offset */
#define WUFEI_UFE_DIRECT_MEM_SIZE (0xCFA)


/* Clock Recovery Tributery Mode RAM */
#define WUFEI_INDIRECT_MEM_ADD_TRIB_MODE_RAM_START      ( 0x000000 )
#define WUFEI_INDIRECT_MEM_ADD_TRIB_MODE_RAM_END        ( 0x000230 )

/* Clock Recovery Tributery stuffing indication RAM */
#define WUFEI_INDIRECT_MEM_ADD_TRIB_STF_IND_RAM_START   ( 0x000800 )
#define WUFEI_INDIRECT_MEM_ADD_TRIB_STF_IND_RAM_END     ( 0x000AA0 )

/* Clock Recovery Tributery GAP count RAM */
#define WUFEI_INDIRECT_MEM_ADD_TRIB_GAP_CNT_RAM_START   ( 0x001000 )
#define WUFEI_INDIRECT_MEM_ADD_TRIB_GAP_CNT_RAM_END     ( 0x001300 )

/* Clock Recovery Tributery GAP Fraction RAM */
#define WUFEI_INDIRECT_MEM_ADD_TRIB_GAP_FRAC_RAM_START  ( 0x001800 )
#define WUFEI_INDIRECT_MEM_ADD_TRIB_GAP_FRAC_RAM_END    ( 0x001E00 )

/* Clock Recovery Delta GAP RAM */
#define WUFEI_INDIRECT_MEM_ADD_DELTA_GAP_RAM_START      ( 0x002000 )
#define WUFEI_INDIRECT_MEM_ADD_DELTA_GAP_RAM_END        ( 0x002540 )


/* Fifo Manager Indirect Address Space*/
#define WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_CONF_START      ( 0x000000 )
#define WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_CONF_END        (WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_CONF_START + WUFEI_MAX_N_PHY_PER_CORE)

#define WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_CONF_START      ( 0x030000 )
#define WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_CONF_END        (WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_CONF_START + (WUFEI_MAX_N_PHY_PER_CORE *2))

#define WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_DDR_MEM_START   ( 0x010000 )
#define WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_DDR_MEM_END     (WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_DDR_MEM_START + (WUFEI_MAX_N_PHY_PER_CORE *2))

#define WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_DDR_MEM_START   ( 0x040000 )
#define WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_DDR_MEM_END     (WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_DDR_MEM_START + (WUFEI_MAX_N_PHY_PER_CORE *4))

#define WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_BURST_MEM_START ( 0x020000 )
#define WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_BURST_MEM_END   (WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_BURST_MEM_START + WUFEI_MAX_N_PHY_PER_CORE)

#define WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_BURST_MEM_START ( 0x050000 )
#define WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_BURST_MEM_END   (WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_BURST_MEM_START + WUFEI_MAX_N_PHY_PER_CORE)

/* TDM Indirect Address Space */
#define WUFEI_INDIRECT_MEM_ADD_TDM_RX_REG_TABLE_START     ( 0x000000 )
#define WUFEI_INDIRECT_MEM_ADD_TDM_RX_REG_TABLE_END       ( 0x006000 )

#define WUFEI_INDIRECT_MEM_ADD_TDM_TX_REG_TABLE_START     ( 0x300000 )
#define WUFEI_INDIRECT_MEM_ADD_TDM_TX_REG_TABLE_END       ( 0x306000 )

#define WUFEI_INDIRECT_MEM_ADD_TDM_RX_MODE_TABLE_START    ( 0x200000 )
#define WUFEI_INDIRECT_MEM_ADD_TDM_RX_MODE_TABLE_END      ( 0x2002A0 )

#define WUFEI_INDIRECT_MEM_ADD_TDM_TX_MODE_TABLE_START    ( 0x500000 )
#define WUFEI_INDIRECT_MEM_ADD_TDM_TX_MODE_TABLE_END      ( 0x5002A0 )

/* Ces Machine Address Space */
#define WUFEI_INDIRECT_MEM_ADD_MAC_RPT_TABLE_START    ( 0x000000 )
#define WUFEI_INDIRECT_MEM_ADD_MAC_RPT_TABLE_END      (WUFEI_INDIRECT_MEM_ADD_MAC_RPT_TABLE_START + (WUFEI_MAX_N_PHY_PER_CORE *16))

#define WUFEI_INDIRECT_MEM_ADD_MAC_TPT_TABLE_START    ( 0x020000 )
#define WUFEI_INDIRECT_MEM_ADD_MAC_TPT_TABLE_END      (WUFEI_INDIRECT_MEM_ADD_MAC_TPT_TABLE_START + (WUFEI_MAX_N_PHY_PER_CORE *16))

#define WUFEI_INDIRECT_MEM_ADD_CES_MAC_RX_CAS_TABLE_START ( 0x040000 )
#define WUFEI_INDIRECT_MEM_ADD_CES_MAC_RX_CAS_TABLE_END   ( 0x042980 )

#define WUFEI_INDIRECT_MEM_ADD_CES_MAC_TX_CAS_TABLE_START ( 0x060000 )
#define WUFEI_INDIRECT_MEM_ADD_CES_MAC_TX_CAS_TABLE_END   ( 0x062980 )

/*********************************************************************/
/*                            INDIRECT                               */
/*********************************************************************/
/************************************/
/* the indirect memory registers    */
/************************************/
#define WUFEI_INDIRECT_ADD_LOW_REG  (0x0000 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_ADD_HIGH_REG (0x0001 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_INDIRECT_DATA_REG     (0x0002 * WUFEI_ADDR_ABS_TO_DRV)

/*****************************************/
/* the Free Buffer pool register mapping */
/*****************************************/
#define WUFEI_FIFOM_DELTA (sizeof(WUFEI_hw_fifom))
#define WUFEI_FIFOM_BASE  (0x0080 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_FIFOM_SECTION_OFFSET(t) (WUFEI_FIFOM_BASE + WUFEI_FIFOM_DELTA * (t))
#define WUFEI_FIFOM(t) (( WUFEI_hw_fifom *) ( WUFEI_FIFOM_SECTION_OFFSET(t)))

/************************************/
/* the TDM register mapping         */
/************************************/
#define WUFEI_TDM_IDLE_BASE (0x0180 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_TDM_IDLE ((WUFEI_hw_idle_pattern *)(WUFEI_TDM_IDLE_BASE))

/************************************/
/* the TDM mode register            */
/************************************/
#define WUFEI_TDM_MODE_BASE (0x0181 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_TDM_MODE ((WP_U16 *)(WUFEI_TDM_MODE_BASE))

/************************************/
/* the TDM Loopback address         */
/************************************/
#define WUFEI_TDM_LPBK_SYS_BASE (0x0190 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_TDM_LPBK_SYS ((WUFEI_hw_tdm_lpbk_system *)(WUFEI_TDM_LPBK_SYS_BASE))

#define WUFEI_TDM_LPBK_LINE_BASE (0x01A0 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_TDM_LPBK_LINE ((WUFEI_hw_tdm_lpbk_line *)(WUFEI_TDM_LPBK_LINE_BASE))

/******************************************/
/* the Core interrupt register mapping   */
/*****************************************/
#define WUFEI_EVENT_BASE (0x0200 * WUFEI_ADDR_ABS_TO_DRV)
#define WUFEI_EVENT ((WUFEI_hw_events *) (WUFEI_EVENT_BASE))




/**********************************/
/* Fifo Manager addresses         */
/**********************************/

#define WUFEI_INDIR_FM_RX_PORT_CONF_DELTA (1)
#define WUFEI_INDIR_FM_RX_PORT_CONF_BASE  WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_CONF_START
#define WUFEI_INDIR_FM_RX_PORT_CONF_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_FM_RX_PORT_CONF_BASE + WUFEI_INDIR_FM_RX_PORT_CONF_DELTA * (t))
#define WUFEI_INDIR_FM_RX_PORT_CONF(t) ((WP_U32)\
(WUFEI_INDIR_FM_RX_PORT_CONF_SECTION_OFFSET(t)))
#define WUFEI_INDIR_FM_RX_PORT_CONF_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_CONF_END - WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_CONF_START)

#define WUFEI_INDIR_FM_TX_PORT_CONF_DELTA (1)
#define WUFEI_INDIR_FM_TX_PORT_CONF_BASE  WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_CONF_START
#define WUFEI_INDIR_FM_TX_PORT_CONF_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_FM_TX_PORT_CONF_BASE + WUFEI_INDIR_FM_TX_PORT_CONF_DELTA * (t))
#define WUFEI_INDIR_FM_TX_PORT_CONF(t) ((WP_U32)\
(WUFEI_INDIR_FM_TX_PORT_CONF_SECTION_OFFSET(t)))
#define WUFEI_INDIR_FM_TX_PORT_CONF_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_CONF_END - WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_CONF_START)

#define WUFEI_INDIR_FM_RX_PORT_DDR_MEM_DELTA (2)
#define WUFEI_INDIR_FM_RX_PORT_DDR_MEM_BASE  WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_DDR_MEM_START
#define WUFEI_INDIR_FM_RX_PORT_DDR_MEM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_FM_RX_PORT_DDR_MEM_BASE + WUFEI_INDIR_FM_RX_PORT_DDR_MEM_DELTA * (t))
#define WUFEI_INDIR_FM_RX_PORT_DDR_MEM(t) ((WP_U32)\
(WUFEI_INDIR_FM_RX_PORT_DDR_MEM_SECTION_OFFSET(t)))
#define WUFEI_INDIR_FM_RX_PORT_DDR_MEM_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_DDR_MEM_END - WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_DDR_MEM_START)

#define WUFEI_INDIR_FM_TX_PORT_DDR_MEM_DELTA (4)
#define WUFEI_INDIR_FM_TX_PORT_DDR_MEM_BASE  WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_DDR_MEM_START
#define WUFEI_INDIR_FM_TX_PORT_DDR_MEM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_FM_TX_PORT_DDR_MEM_BASE + WUFEI_INDIR_FM_TX_PORT_DDR_MEM_DELTA * (t))
#define WUFEI_INDIR_FM_TX_PORT_DDR_MEM(t) ((WP_U32)\
(WUFEI_INDIR_FM_TX_PORT_DDR_MEM_SECTION_OFFSET(t)))
#define WUFEI_INDIR_FM_TX_PORT_DDR_MEM_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_DDR_MEM_END - WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_DDR_MEM_START)

#define WUFEI_INDIR_FM_RX_PORT_BURST_MEM_DELTA (1)
#define WUFEI_INDIR_FM_RX_PORT_BURST_MEM_BASE  WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_BURST_MEM_START
#define WUFEI_INDIR_FM_RX_PORT_BURST_MEM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_FM_RX_PORT_BURST_MEM_BASE + WUFEI_INDIR_FM_RX_PORT_BURST_MEM_DELTA * (t))
#define WUFEI_INDIR_FM_RX_PORT_BURST_MEM(t) ((WP_U32)\
(WUFEI_INDIR_FM_RX_PORT_BURST_MEM_SECTION_OFFSET(t)))
#define WUFEI_INDIR_FM_RX_PORT_BURST_MEM_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_BURST_MEM_END - WUFEI_INDIRECT_MEM_ADD_FM_RX_PORT_BURST_MEM_START)

#define WUFEI_INDIR_FM_TX_PORT_BURST_MEM_DELTA (1)
#define WUFEI_INDIR_FM_TX_PORT_BURST_MEM_BASE  WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_BURST_MEM_START
#define WUFEI_INDIR_FM_TX_PORT_BURST_MEM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_FM_TX_PORT_BURST_MEM_BASE + WUFEI_INDIR_FM_TX_PORT_BURST_MEM_DELTA * (t))
#define WUFEI_INDIR_FM_TX_PORT_BURST_MEM(t) ((WP_U32)\
(WUFEI_INDIR_FM_TX_PORT_BURST_MEM_SECTION_OFFSET(t)))
#define WUFEI_INDIR_FM_TX_PORT_BURST_MEM_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_BURST_MEM_END - WUFEI_INDIRECT_MEM_ADD_FM_TX_PORT_BURST_MEM_START)

/*************************/
/* TDM addresses         */
/*************************/

#define WUFEI_INDIR_TDM_RX_REG_TABLE_DELTA (1)
#define WUFEI_INDIR_TDM_RX_REG_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_TDM_RX_REG_TABLE_START
#define WUFEI_INDIR_TDM_RX_REG_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_TDM_RX_REG_TABLE_BASE + WUFEI_INDIR_TDM_RX_REG_TABLE_DELTA * (t))
#define WUFEI_INDIR_TDM_RX_REG_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_TDM_RX_REG_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_TDM_RX_REG_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_TDM_RX_REG_TABLE_END - WUFEI_INDIRECT_MEM_ADD_TDM_RX_REG_TABLE_START)

#define WUFEI_INDIR_TDM_TX_REG_TABLE_DELTA (1)
#define WUFEI_INDIR_TDM_TX_REG_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_TDM_TX_REG_TABLE_START
#define WUFEI_INDIR_TDM_TX_REG_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_TDM_TX_REG_TABLE_BASE + WUFEI_INDIR_TDM_TX_REG_TABLE_DELTA * (t))
#define WUFEI_INDIR_TDM_TX_REG_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_TDM_TX_REG_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_TDM_TX_REG_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_TDM_TX_REG_TABLE_END - WUFEI_INDIRECT_MEM_ADD_TDM_TX_REG_TABLE_START)

#define WUFEI_INDIR_TDM_RX_MODE_TABLE_DELTA (1)
#define WUFEI_INDIR_TDM_RX_MODE_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_TDM_RX_MODE_TABLE_START
#define WUFEI_INDIR_TDM_RX_MODE_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_TDM_RX_MODE_TABLE_BASE + WUFEI_INDIR_TDM_RX_MODE_TABLE_DELTA * (t))
#define WUFEI_INDIR_TDM_RX_MODE_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_TDM_RX_MODE_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_TDM_RX_MODE_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_TDM_RX_MODE_TABLE_END - WUFEI_INDIRECT_MEM_ADD_TDM_RX_MODE_TABLE_START)

#define WUFEI_INDIR_TDM_TX_MODE_TABLE_DELTA (1)
#define WUFEI_INDIR_TDM_TX_MODE_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_TDM_TX_MODE_TABLE_START
#define WUFEI_INDIR_TDM_TX_MODE_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_TDM_TX_MODE_TABLE_BASE + WUFEI_INDIR_TDM_TX_MODE_TABLE_DELTA * (t))
#define WUFEI_INDIR_TDM_TX_MODE_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_TDM_TX_MODE_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_TDM_TX_MODE_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_TDM_TX_MODE_TABLE_END - WUFEI_INDIRECT_MEM_ADD_TDM_TX_MODE_TABLE_START)

/***********************/
/* Machine addresses   */
/***********************/

#define WUFEI_INDIR_MAC_RPT_TABLE_DELTA (1)
#define WUFEI_INDIR_MAC_RPT_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_MAC_RPT_TABLE_START
#define WUFEI_INDIR_MAC_RPT_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_MAC_RPT_TABLE_BASE + WUFEI_INDIR_MAC_RPT_TABLE_DELTA * (t))
#define WUFEI_INDIR_MAC_RPT_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_MAC_RPT_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_MAC_RPT_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_MAC_RPT_TABLE_END - WUFEI_INDIRECT_MEM_ADD_MAC_RPT_TABLE_START)

#define WUFEI_INDIR_MAC_TPT_TABLE_DELTA (1)
#define WUFEI_INDIR_MAC_TPT_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_MAC_TPT_TABLE_START
#define WUFEI_INDIR_MAC_TPT_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_MAC_TPT_TABLE_BASE + WUFEI_INDIR_MAC_TPT_TABLE_DELTA * (t))
#define WUFEI_INDIR_MAC_TPT_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_MAC_TPT_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_MAC_TPT_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_MAC_TPT_TABLE_END - WUFEI_INDIRECT_MEM_ADD_MAC_TPT_TABLE_START)

#define WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_DELTA (1)
#define WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_CES_MAC_RX_CAS_TABLE_START
#define WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_BASE + WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_DELTA * (t))
#define WUFEI_INDIR_CES_MAC_RX_CAS_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_CES_MAC_RX_CAS_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_CES_MAC_RX_CAS_TABLE_END - WUFEI_INDIRECT_MEM_ADD_CES_MAC_RX_CAS_TABLE_START)

#define WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_DELTA (1)
#define WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_BASE  WUFEI_INDIRECT_MEM_ADD_CES_MAC_TX_CAS_TABLE_START
#define WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_BASE + WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_DELTA * (t))
#define WUFEI_INDIR_CES_MAC_TX_CAS_TABLE(t) ((WP_U32)\
(WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_SECTION_OFFSET(t)))
#define WUFEI_INDIR_CES_MAC_TX_CAS_TABLE_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_CES_MAC_TX_CAS_TABLE_END - WUFEI_INDIRECT_MEM_ADD_CES_MAC_TX_CAS_TABLE_START)




/************************************/
/* the Mode RX -SBI address         */
/************************************/
#define WUFEI_INDIR_SBI_MODE_RX_DELTA (1) /*single sbi unit*/
#define WUFEI_INDIR_SBI_MODE_RX_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_MODE_RX_START
#define WUFEI_INDIR_SBI_MODE_RX_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_MODE_RX_BASE + WUFEI_INDIR_SBI_MODE_RX_DELTA * (t))
#define WUFEI_INDIR_SBI_MODE_RX(t) ((WUFEI_hw_sbi_mode_ram*)\
(WUFEI_INDIR_SBI_MODE_RX_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_MODE_RX_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_MODE_RX_END - WUFEI_INDIRECT_MEM_ADD_SBI_MODE_RX_START)

/*************************************/
/* the State RX -SBI address         */
/*************************************/
#define WUFEI_INDIR_SBI_STATE_RX_DELTA (2)
#define WUFEI_INDIR_SBI_STATE_RX_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_STATE_RX_START
#define WUFEI_INDIR_SBI_STATE_RX_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_STATE_RX_BASE + WUFEI_INDIR_SBI_STATE_RX_DELTA * (t))
#define WUFEI_INDIR_SBI_STATE_RX(t) ((WUFEI_hw_sbi_rx_state_ram*)\
(WUFEI_INDIR_SBI_STATE_RX_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_STATE_RX_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_STATE_RX_END - WUFEI_INDIRECT_MEM_ADD_SBI_STATE_RX_START)

/************************************/
/* the ALM/V5  RX -SBI address      */
/************************************/
#define WUFEI_INDIR_SBI_RX_ALM_V5_DELTA (1)
#define WUFEI_INDIR_SBI_RX_ALM_V5_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_RX_ALM_V5_START
#define WUFEI_INDIR_SBI_RX_ALM_V5_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_RX_ALM_V5_BASE + WUFEI_INDIR_SBI_RX_ALM_V5_DELTA * (t))
#define WUFEI_INDIR_SBI_RX_ALM_V5(t)((WUFEI_hw_sbi_alm_v5*)\
(WUFEI_INDIR_SBI_RX_ALM_V5_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_RX_ALM_V5_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_RX_ALM_V5_END - WUFEI_INDIRECT_MEM_ADD_SBI_RX_ALM_V5_START)

/************************************/
/* the Mode TX -SBI address         */
/************************************/
#define WUFEI_INDIR_SBI_MODE_TX_DELTA (1)
#define WUFEI_INDIR_SBI_MODE_TX_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_MODE_TX_START
#define WUFEI_INDIR_SBI_MODE_TX_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_MODE_TX_BASE + WUFEI_INDIR_SBI_MODE_TX_DELTA * (t))
#define WUFEI_INDIR_SBI_MODE_TX(t) ((WUFEI_hw_sbi_mode_ram*)\
(WUFEI_INDIR_SBI_MODE_TX_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_MODE_TX_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_MODE_TX_END - WUFEI_INDIRECT_MEM_ADD_SBI_MODE_TX_START)

/*************************************/
/* the State TX -SBI address         */
/*************************************/
#define WUFEI_INDIR_SBI_STATE_TX_DELTA (1) /*single sbi unit*/
#define WUFEI_INDIR_SBI_STATE_TX_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_STATE_TX_START
#define WUFEI_INDIR_SBI_STATE_TX_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_STATE_TX_BASE + WUFEI_INDIR_SBI_STATE_TX_DELTA * (t))
#define WUFEI_INDIR_SBI_STATE_TX(t) ((WUFEI_hw_sbi_tx_state_ram*)\
(WUFEI_INDIR_SBI_STATE_TX_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_STATE_TX_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_STATE_TX_END - WUFEI_INDIRECT_MEM_ADD_SBI_STATE_TX_START)

/************************************/
/* the Cas RX -SBI address         */
/************************************/
#define WUFEI_INDIR_SBI_CAS_RX_DELTA (1)
#define WUFEI_INDIR_SBI_CAS_RX_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_CAS_RX_START
#define WUFEI_INDIR_SBI_CAS_RX_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_CAS_RX_BASE + WUFEI_INDIR_SBI_CAS_RX_DELTA * (t))
#define WUFEI_INDIR_SBI_CAS_RX(t) ((WUFEI_hw_sbi_cas_ram*)\
(WUFEI_INDIR_SBI_CAS_RX_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_CAS_RX_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_CAS_RX_END - WUFEI_INDIRECT_MEM_ADD_SBI_CAS_RX_START)

/************************************/
/* the Cas TX -SBI address         */
/************************************/
#define WUFEI_INDIR_SBI_CAS_TX_DELTA (1)
#define WUFEI_INDIR_SBI_CAS_TX_BASE  WUFEI_INDIRECT_MEM_ADD_SBI_CAS_TX_START
#define WUFEI_INDIR_SBI_CAS_TX_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_SBI_CAS_TX_BASE + WUFEI_INDIR_SBI_CAS_TX_DELTA * (t))
#define WUFEI_INDIR_SBI_CAS_TX(t) ((WUFEI_hw_sbi_cas_ram*)\
(WUFEI_INDIR_SBI_CAS_TX_SECTION_OFFSET(t)))
#define WUFEI_INDIR_SBI_CAS_TX_SIZE \
   (WUFEI_INDIRECT_MEM_ADD_SBI_CAS_TX_END - WUFEI_INDIRECT_MEM_ADD_SBI_CAS_TX_START)

/************************************/
/* the phy rx -RSPT address         */
/************************************/
#define WUFEI_RX_PHY_DELTA (sizeof(WUFEI_rspt))
#define WUFEI_RX_PHY_BASE  WUFEI_INDIRECT_MEM_ADD_RSPT_START
#define WUFEI_RX_PHY_SECTION_OFFSET(t) (WUFEI_RX_PHY_BASE + WUFEI_RX_PHY_DELTA * (t))
#define WUFEI_RX_PHY(t) ((WUFEI_rspt *) (WUFEI_RX_PHY_SECTION_OFFSET(t)))

/************************************/
/* the phy tx -TSPT address         */
/************************************/
#define WUFEI_TX_PHY_DELTA (sizeof(WUFEI_tspt))
#define WUFEI_TX_PHY_BASE  WUFEI_INDIRECT_MEM_ADD_TSPT_START
#define WUFEI_TX_PHY_SECTION_OFFSET(t) (WUFEI_TX_PHY_BASE + WUFEI_TX_PHY_DELTA * (t))
#define WUFEI_TX_PHY(t) ((WUFEI_tspt *) (WUFEI_TX_PHY_SECTION_OFFSET(t)))

/************************************/
/* the RX -TDM address              */
/************************************/
#define WUFEI_RX_TDM_DELTA (sizeof(WUFEI_hw_tdm_entry))
#define WUFEI_RX_TDM_BASE  WUFEI_INDIRECT_MEM_ADD_RTDM_START
#define WUFEI_RX_TDM_SECTION_OFFSET(t) (WUFEI_RX_TDM_BASE + WUFEI_RX_TDM_DELTA * (t))
#define WUFEI_RX_TDM(t) ((WUFEI_hw_tdm_entry *) (WUFEI_RX_TDM_SECTION_OFFSET(t)))

/************************************/
/* the TX -TDM address              */
/************************************/
#define WUFEI_TX_TDM_DELTA (sizeof(WUFEI_hw_tdm_entry))
#define WUFEI_TX_TDM_BASE  WUFEI_INDIRECT_MEM_ADD_TTDM_START
#define WUFEI_TX_TDM_SECTION_OFFSET(t) (WUFEI_TX_TDM_BASE + WUFEI_TX_TDM_DELTA * (t))
#define WUFEI_TX_TDM(t) ((WUFEI_hw_tdm_entry *) (WUFEI_TX_TDM_SECTION_OFFSET(t)))


/************* UFE core ID mask ************/
/* UFE core id structure*/
#define WUFEI_UFE_ID_S (0)
#define WUFEI_UFE_ID_W (2)
#define WUFEI_UFE_ID_F(v) WUFEI_FIELD(v, WUFEI_UFE_ID_S)
#define WUFEI_UFE_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_UFE_ID_S, WUFEI_UFE_ID_W)
#define WUFEI_UFE_ID_M \
        WUFEI_MASK(WUFEI_UFE_ID_S, WUFEI_UFE_ID_W)

/* UFE core id structure*/
#define WUFEI_UFE_CORE_ID_S (2)
#define WUFEI_UFE_CORE_ID_W (2)
#define WUFEI_UFE_CORE_ID_F(v) WUFEI_FIELD(v, WUFEI_UFE_CORE_ID_S)
#define WUFEI_UFE_CORE_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_UFE_CORE_ID_S, WUFEI_UFE_CORE_ID_W)
#define WUFEI_UFE_CORE_ID_M \
        WUFEI_MASK(WUFEI_UFE_CORE_ID_S, WUFEI_UFE_CORE_ID_W)

#define WUFEI_ID(a,b) WUFEI_UFE_ID_F(a)| WUFEI_UFE_CORE_ID_F(b)
/************************************/

#define WUFEI_PHY_NOT_CONNECTED 0xffff


/*************************************************/
/*     Clock Recovery: second core handeling    */
/***********************************************/
#define WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_B_OFFSET 0x00020000
#define WUFEI_CORE_FROM_LINE_ID(t)  (t >= 672) 
#define WUFEI_LINE_ID_CORE_CONVERT(t)  (t - (WUFEI_CORE_FROM_LINE_ID(t)*672))
#define WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_OFFSET(t) (WUFEI_CORE_FROM_LINE_ID(t)*WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_B_OFFSET)


/*************************************************/
/*     Clock Recovery: Tributery mode RAM       */
/***********************************************/
#define WUFEI_INDIRECT_TRIB_MODE_RAM_DELTA (1)
#define WUFEI_INDIRECT_TRIB_MODE_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_TRIB_MODE_RAM_START
#define WUFEI_INDIRECT_TRIB_MODE_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIRECT_TRIB_MODE_RAM_BASE + \
                    WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_OFFSET(t) + \
                    WUFEI_INDIRECT_TRIB_MODE_RAM_DELTA * WUFEI_LINE_ID_CORE_CONVERT(t))

#define WUFEI_INDIRECT_TRIB_MODE_RAM(t) ((WUFEI_hw_clk_rec_trib_mode_ram *) (\
                                       WUFEI_INDIRECT_TRIB_MODE_RAM_SECTION_OFFSET(t)))

/****************************************************************/
/*     Clock Recovery: Tributery stuffing indication RAM       */
/**************************************************************/
#define WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM_DELTA (1)
#define WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_TRIB_STF_IND_RAM_START

#define WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM_BASE + \
                    WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_OFFSET(t) + \
                    WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM_DELTA * WUFEI_LINE_ID_CORE_CONVERT(t))

#define WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM(t) ((WUFEI_hw_clk_rec_trib_stf_ind_ram *) (\
                                       WUFEI_INDIRECT_TRIB_STUFF_INDICATION_RAM_SECTION_OFFSET(t)))

/******************************************************/
/*     Clock Recovery: Tributery GAP count RAM       */
/****************************************************/
#define WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM_DELTA (1)
#define WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_TRIB_GAP_CNT_RAM_START

#define WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM_BASE + \
                    WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_OFFSET(t) + \
                    WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM_DELTA * WUFEI_LINE_ID_CORE_CONVERT(t))

#define WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM(t) ((WUFEI_hw_clk_rec_trib_gap_count_ram *) (\
                                       WUFEI_INDIRECT_TRIB_GAP_COUNT_RAM_SECTION_OFFSET(t)))

/*****************************************************************/
/*     Clock Recovery: Tributery GAP Fractional count RAM       */
/***************************************************************/
#define WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM_DELTA (2)
#define WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_TRIB_GAP_FRAC_RAM_START

#define WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM_BASE + \
                    WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_OFFSET(t) + \
                    WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM_DELTA * WUFEI_LINE_ID_CORE_CONVERT(t))

#define WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM(t) ((WUFEI_hw_clk_rec_trib_gap_frac_count_ram *) (\
                                       WUFEI_INDIRECT_TRIB_GAP_FRAC_COUNT_RAM_SECTION_OFFSET(t)))

/******************************************************/
/*     Clock Recovery: Tributery Delta GAP RAM       */
/****************************************************/
#define WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM_DELTA (2)
#define WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_DELTA_GAP_RAM_START


#define WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM_BASE + \
                    WUFEI_INDIRECT_CLOCK_RECOVERY_CORE_OFFSET(t) + \
                    WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM_DELTA * WUFEI_LINE_ID_CORE_CONVERT(t))

#define WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM(t) ((WUFEI_hw_clk_rec_trib_delta_gap_ram *) (\
                                       WUFEI_INDIRECT_TRIB_DELTA_GAP_RAM_SECTION_OFFSET(t)))

/****** Defines for UFE3 ******/

/***********************************/
/* the TX -Clock Translator RAM    */
/***********************************/
#define WUFEI_INDIR_CLK_TRANS_RAM_DELTA (1)
#define WUFEI_INDIR_CLK_TRANS_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_CLK_TRANS_RAM_START
#define WUFEI_INDIR_CLK_TRANS_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_CLK_TRANS_RAM_BASE + WUFEI_INDIR_CLK_TRANS_RAM_DELTA * (t))
#define WUFEI_INDIR_CLK_TRANS_RAM(t) ((WUFEI_hw_clk_rec_ct_tx_ram *) (\
                                       WUFEI_INDIR_CLK_TRANS_RAM_SECTION_OFFSET(t)))

/*********************************************************/
/* the RX - Clock Generator RAM                         */
/*******************************************************/
#define WUFEI_INDIR_CGEN_RX_RAM_DELTA (1)
#define WUFEI_INDIR_CGEN_RX_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_CGEN_RX_RAM_START
#define WUFEI_INDIR_CGEN_RX_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_CGEN_RX_RAM_BASE + WUFEI_INDIR_CGEN_RX_RAM_DELTA * (t))
#define WUFEI_INDIR_CGEN_RX_RAM(t) ((WUFEI_hw_clk_rec_cgen_rx_ram *) (\
                                       WUFEI_INDIR_CGEN_RX_RAM_SECTION_OFFSET(t)))

/*********************************************************/
/* the RX - Clock Recovery Timestamp RX port RAM address */
/*********************************************************/
#define WUFEI_INDIR_TS_RX_PORT_RAM_DELTA (0x400)
#define WUFEI_INDIR_TS_RX_PORT_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_TS_RX_PORT_RAM_START
#define WUFEI_INDIR_TS_RX_PORT_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_TS_RX_PORT_RAM_BASE + WUFEI_INDIR_TS_RX_PORT_RAM_DELTA * (t))
#define WUFEI_INDIR_TS_RX_PORT_RAM(t) ((WUFEI_hw_clk_rec_ts_port_ram *) (\
                                       WUFEI_INDIR_TS_RX_PORT_RAM_SECTION_OFFSET(t)))

/*********************************************************/
/* the RX - Clock Recovery Timestamp RX mask RAM address */
/*********************************************************/
#define WUFEI_INDIR_TS_RX_MASK_RAM_DELTA (0x400)
#define WUFEI_INDIR_TS_RX_MASK_RAM_BASE  WUFEI_INDIRECT_MEM_ADD_TS_RX_MASK_RAM_START
#define WUFEI_INDIR_TS_RX_MASK_RAM_SECTION_OFFSET(t) \
                   (WUFEI_INDIR_TS_RX_MASK_RAM_BASE + WUFEI_INDIR_TS_RX_MASK_RAM_DELTA * (t))
#define WUFEI_INDIR_TS_RX_MASK_RAM(t) ((WUFEI_hw_clk_rec_ts_mask_ram *) (\
                                       WUFEI_INDIR_TS_RX_MASK_RAM_SECTION_OFFSET(t)))



#endif /* WUFEI_HW_CONFIG_H*/
