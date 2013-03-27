/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_registry.h
 *
 * Purpose: the registry srtucture
 *
 ****************************************************************************/

#ifndef WUFEI_REGISTRY_H
#define WUFEI_REGISTRY_H

#ifndef WUFE_SYSTEM_H
#include "wufe_system.h"
#endif
#ifndef WUFE_API_H
#include "wufe_api.h"
#endif
#ifndef WUFE_LINE_H
#include "wufe_line.h"
#endif
#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif
#ifndef WUFE_USER_CONFIG_H
#include "wufe_user_config.h"
#endif
#ifndef WUFEI_PHY_H
#include "wufe_phy.h"
#endif
#ifndef WUFEI_CLK_REC_H
#include "wufei_clk_rec.h"
#endif
#ifndef WPI_MEMORY_H
#include "include/core/memory/wpi_memory.h"
#endif

#define WUFEI_MALLOC WPL_MALLOC
#define WUFEI_FREE WPL_FREE

#define WUFEI_1MB_SIZE 1024
#define WUFEI_MAX_N_BANKS 8

/* WP3 chips select general turn-off register address + value */
#define WUFEI_SIU4X_GEN_TOFF_ADDRESS 0x1e010890
#define WUFEI_SIU4X_GEN_TOFF_VALUE   0x00000006
#define WUFEI_TURNOFF_MASK           0x000000F

typedef enum
{
	WUFEI_EMPHY_FIRST = 1,
	WUFEI_EMPHY_SECOND
}wufei_emphy_number;

struct wufei_phy_registry
{
      WP_U16   line_id; /*Line ID the PHY belongs to...*/
      WP_U16   phy_id;  /*Phy ID */
      WP_U8    rx_active; /*RX stream is active*/
      WP_U8    tx_active; /*TX stream is active*/
      WP_U8    master; /*PHY is clock recovery master - WUFEI_clk_rec_master*/
      WP_U16   emphy_port_id; /*Emphy & TSPT/RSPT access index*/
      WUFE_phy cfg;
      WP_U16   rx_bank_number; /*UFE4 Only!*/
      WP_U16   tx_bank_number; /*UFE4 Only!*/
};

typedef struct
{
      WP_U16 tdm_slots[2/*RX and TX */][WUFE_N_SLOTS_E1];
}WUFEI_tdm_slots;


struct wufei_line_registry
{
   WP_U16           line_id;
   WP_U16           line_mode;
   WP_U16           state_mode;
   WP_U16           n_attach_phy;
   WP_U16           n_rx_active_phy;
   WP_U16           n_tx_active_phy;
   WP_U8            transfer_type;
   WP_U8            framed;
   WP_U8            hmvip_num;
   WP_U8            spe_id;
   WP_U16           sub_spe_entry;
   WP_U8            cas_enable;
   WP_U8            tx_clk_master;
   WP_U8            clock_rec_mode;
   WP_U8            stm1_id;
   WP_U16           clock_translator_id;
   WP_U16           rx_clock_rec_if_id; /* 0-31 adap + regular diff, 32-367 extended diff */
   WUFE_cad_cr_extended ext_mode_active; /* 0-adaptive or regular diff, 1-extended differential */
   WP_U16           tx_clock_rec_if_id;
   WP_U8            ufe_core_id;
   WP_U8            t1oe1_mode;
   WP_U8            e3_atm_g751_enabled[2]; /*0 - Rx, 1- Tx*/
   WUFEI_tdm_slots *tdm_ram_mng;/*init to WUFEI_NOT_VALID_SLOT */
   WP_U8            aug4;
   WP_U8            aug1;
   WP_U8            au3;
   WP_U8            tug2;
   WP_U8            tu;
   WUFE_line_loopback_type loopback_type;
   WUFE_line_clock_rec_params *clock_rec_line_params;
   WUFE_line_pdh_loopback_type pdh_loopback_type;
   WP_U32					   pdh_loopback_index;
 };

typedef struct
{
   WP_U32 line_id;
}WUFEI_tdm_line_entry;

typedef struct
{
   WP_U8  state_mode;
   WP_U8  sbi_rx;
   WP_U8  sbi_tx;
   WP_U8  sbi_spe_rx[WUFE_MAX_N_SBI_SPE];
   WP_U8  sbi_spe_tx[WUFE_MAX_N_SBI_SPE];
}WUFEI_system_enable;

typedef struct
{
   WP_U32 in;
   WP_U16* line_id_arr;
}WUFEI_sbi_clock_translator;

typedef struct
{
   WP_U8 mode;
   WP_U16 n_units;
   WP_U16 n_ext_diff_units;
}WUFEI_clk_rec_hw_config_block;

typedef struct
{
   WUFEI_clk_rec_hw_config_block clk_translator;
   WUFEI_clk_rec_hw_config_block dco;
   WUFEI_clk_rec_hw_config_block cgen;
   WUFEI_clk_rec_hw_config_block clk_rec_tx;
   WUFEI_clk_rec_hw_config_block clk_rec_rx;
}WUFEI_clk_rec_hw_config;

typedef struct
{
   WP_U16 release_id;
   WP_U16 secondary_release_id;
   WP_U8 line_interface;
   WP_U8 full_ufe_model;
   WP_U8 state_machine[WUFE_MAX_N_CORE];
   WP_U8 emphy_setup_mode;
   WP_U8 core_enabled[WUFE_MAX_N_CORE];
   WP_U16 zbt_size[WUFE_MAX_N_CORE];
   WP_U8 zbt_internal[WUFE_MAX_N_CORE];
   WUFEI_clk_rec_hw_config *clk_rec_hw_config;
   WP_U16 bandwidth;
   WP_U16 tdm_sys_loop[WUFE_MAX_N_CORE]; /*used for ufe3 only*/
   WP_U16 tdm_line_loop[WUFE_MAX_N_CORE]; /*used for ufe3 only*/
   WP_U16 max_buf_size; /*used for ufe4 only*/
   WP_U16 framer_type; /*used for ufe4 only*/
   WP_U8 num_of_banks; /*used for ufe4 only*/
}WUFEI_system_hw_config;

typedef struct
{
   WP_U16* phy_id_pool[WUFE_MAX_N_CORE];
   WP_U32  max_n_phys;
}WUFEI_phy_id_block;

typedef struct
{
   WP_U32  transfer_type;
   WP_U32  num_of_lines;
}WUFEI_tug2_info;

struct wufei_system_registry
{
   WP_U8 wufe_id;
   WUFE_system             cfg;
   WUFEI_system_hw_config  hw_config;
   WUFEI_system_enable     sys_enable;
   WUFEI_line_registry    *line_arr;
   WUFEI_phy_registry     *phy_arr;
   WUFEI_clk_rec_registry *clk_rec;
   WUFEI_phy_id_block     *phy_id_block;
   WP_S32                  phy_assign_cnt;
   WP_U16                  core1_phyid_offset;
   WP_U32                  max_phy_per_core;
   WUFEI_tdm_line_entry    tdm_line_arr[WUFE_MAX_N_AUG4*WUFE_MAX_N_SBI_SPE][WUFEI_SPE_LINE_ADDRESS_SPACE];
   WUFEI_sbi_clock_translator *sbi_clock_translator_block;
   WP_U32                  rx_fbp_size[WUFE_MAX_N_CORE];
   WP_U32                  tx_fbp_size[WUFE_MAX_N_CORE];
   WP_U32                  fm_bank_status[WUFE_MAX_N_CORE][WUFEI_MAX_N_BANKS];
   WUFEI_tug2_info         tug2_arr[WUFE_MAX_N_CORE*WUFE_MAX_N_AUG4*WUFE_MAX_N_AUG1*WUFE_MAX_N_AU3*WUFE_MAX_N_TUG2];
   WUFEI_clock_rec_dco_registry clk_rec_dco_reg[WUFE_MAX_N_DCO]; /* Not relevant in UFE3 */
   wufei_emphy_number emphy_number; /* for secondary EMPHY */
   WP_U32					pdh_line_loopback[WUFEI_MAX_PDH_LOOPBACK];
   WP_U32					pdh_system_loopback[WUFEI_MAX_PDH_LOOPBACK];
};

struct wufei_mapping_block
{
   WP_U32 ufe_base;
   WP_U32 attributes;
};

typedef struct
{
      WUFEI_system_registry *system_registry;
      WUFEI_mapping_block *mapping_block;
      WP_U8 dbg_mode;
      WUFE_device_type    device_type;
      WP_U32 build_time;
}WUFEI_registry;


extern WP_CHAR wufei_log[];


#if WUFE_DEBUG_ENABLE_WUFE_CALL_API
#define WUFEI_UFE_LOCK(wufe_id) \
   do { \
      WPI_WDDI_ATTACH(); \
   } while (0)
#else
#define WUFEI_UFE_LOCK(wufe_id) WUFEI_UfeRegistryLock(wufe_id);
#endif


#define WUFEI_UFE_UNLOCK(wufe_id) WUFEI_UfeRegistryUnLock(wufe_id);



#if WUFE_DEBUG_ENABLE_WUFE_CALL_API
#define WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,system_registry,status) do { \
      WPI_WDDI_ATTACH(); \
      WUFEI_RETURN_IF_ERROR(status,WUFEI_SystemRegistryGetAndLock(wufe_id,system_registry));} while (0)
#else
#define WUFEI_REGISTY_GET_WRITE_ATTACH(wufe_id,system_registry,status)        \
   WUFEI_RETURN_IF_ERROR(status,WUFEI_SystemRegistryGetAndLock(wufe_id,system_registry))
#endif



#define WUFEI_REGISTY_GET_WRITE_DETACH(wufe_id) \
       WUFEI_UFE_UNLOCK(wufe_id)


/* TU structure*/
#define WUFEI_TU_S (0)
#define WUFEI_TU_W (2)
#define WUFEI_TU_F(v) WUFEI_FIELD(v, WUFEI_TU_S)
#define WUFEI_TU_V(f) \
        WUFEI_VALUE(f, WUFEI_TU_S, WUFEI_TU_W)
#define WUFEI_TU_M \
        WUFEI_MASK(WUFEI_TU_S, WUFEI_TU_W)

/* TUG2 structure*/
#define WUFEI_TUG2_S (2)
#define WUFEI_TUG2_W (3)
#define WUFEI_TUG2_F(v) WUFEI_FIELD(v, WUFEI_TUG2_S)
#define WUFEI_TUG2_V(f) \
        WUFEI_VALUE(f, WUFEI_TUG2_S, WUFEI_TUG2_W)
#define WUFEI_TUG2_M \
        WUFEI_MASK(WUFEI_TUG2_S, WUFEI_TUG2_W)

#define WUFEI_SUB_SPE(a,b) WUFEI_TUG2_F(a)|WUFEI_TU_F(b)

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * importtant if it is change change
 * the api/wufe_phy.h WUFE_PHY_INDEX_GET macro
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/* handle stucture (as in WDDI)*/
#define WUFEI_HANDLE_INDEX_S (0)
#define WUFEI_HANDLE_INDEX_W (16)
#define WUFEI_HANDLE_INDEX_F(v) WUFEI_FIELD(v, WUFEI_HANDLE_INDEX_S)
#define WUFEI_HANDLE_INDEX_V(f) \
        WUFEI_VALUE(f, WUFEI_HANDLE_INDEX_S, WUFEI_HANDLE_INDEX_W)
#define WUFEI_HANDLE_INDEX_M \
        WUFEI_MASK(WUFEI_HANLE_INDEX_S, WUFEI_HANDLE_INDEX_W)

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * importtant if it is change change
 * the api/wufe_phy.h WUFE_PHY_INDEX_GET macro
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#define WUFEI_HANDLE_UFE_CORE_ID_S (16)
#define WUFEI_HANDLE_UFE_CORE_ID_W (2)
#define WUFEI_HANDLE_UFE_CORE_ID_F(v) WUFEI_FIELD(v, WUFEI_HANDLE_UFE_CORE_ID_S)
#define WUFEI_HANDLE_UFE_CORE_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_HANDLE_UFE_CORE_ID_S, WUFEI_HANDLE_UFE_CORE_ID_W)
#define WUFEI_HANDLE_UFE_CORE_ID_M \
        WUFEI_MASK(WUFEI_HANDLE_UFE_CORE_ID_S, WUFEI_HANDLE_UFE_CORE_ID_W)

#define WUFEI_HANDLE_UFE_ID_S (18)
#define WUFEI_HANDLE_UFE_ID_W (2)
#define WUFEI_HANDLE_UFE_ID_F(v) WUFEI_FIELD(v, WUFEI_HANDLE_UFE_ID_S)
#define WUFEI_HANDLE_UFE_ID_V(f) \
        WUFEI_VALUE(f, WUFEI_HANDLE_UFE_ID_S, WUFEI_HANDLE_UFE_ID_W)
#define WUFEI_HANDLE_UFE_ID_M \
        WUFEI_MASK(WUFEI_HANDLE_UFE_ID_S, WUFEI_HANDLE_UFE_ID_W)

#define WUFEI_HANDLE_SUB_TYPE_S (20)
#define WUFEI_HANDLE_SUB_TYPE_W (6)
#define WUFEI_HANDLE_SUB_TYPE_F(v) WUFEI_FIELD(v, WUFEI_HANDLE_SUB_TYPE_S)
#define WUFEI_HANDLE_SUB_TYPE_V(f) \
        WUFEI_VALUE(f, WUFEI_HANDLE_SUB_TYPE_S, WUFEI_HANDLE_SUB_TYPE_W)
#define WUFEI_HANDLE_SUB_TYPE_M \
        WUFEI_MASK(WUFEI_HANDLE_SUB_TYPE_S, WUFEI_HANDLE_SUB_TYPE_W)

#define WUFEI_HANDLE_TYPE_S (26)
#define WUFEI_HANDLE_TYPE_W (6)
#define WUFEI_HANDLE_TYPE_F(v) WUFEI_FIELD(v, WUFEI_HANDLE_TYPE_S)
#define WUFEI_HANDLE_TYPE_V(f) \
        WUFEI_VALUE(f, WUFEI_HANDLE_TYPE_S, WUFEI_HANDLE_TYPE_W)
#define WUFEI_HANDLE_TYPE_M \
        WUFEI_MASK(WUFEI_HANDLE_TYPE_S, WUFEI_HANDLE_TYPE_W)

enum handle_type
{
   WUFE_HANDLE_TYPE_LINE,
   WUFE_HANDLE_TYPE_PHY
};

enum handle_sub_type_line
{
   WUFE_HANDLE_SUB_TYPE_CAD,
   WUFE_HANDLE_SUB_TYPE_SBI,
   WUFE_HANDLE_SUB_TYPE_GENERIC,
   WUFE_HANDLE_SUB_TYPE_SDH,
   WUFE_HANDLE_SUB_TYPE_SONET,
   WUFE_HANDLE_SUB_TYPE_DCC
};

enum
{
   WUFEI_FPGA_TYPE_UFE2,
   WUFEI_FPGA_TYPE_UFE2PLUS,
   WUFEI_FPGA_TYPE_UFE3,
   WUFEI_FPGA_TYPE_UFE4
};

enum
{
   WUFEI_DBG_LEVEL_0 = 0,
   WUFEI_DBG_LEVEL_1,
   WUFEI_DBG_LEVEL_MAX
};


#endif /* WUFEI_REGISTRY_H*/
