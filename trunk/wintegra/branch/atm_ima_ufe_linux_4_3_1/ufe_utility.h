/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2012.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *  Application : ATM IMA UFE
 *  File Name   : ufe_utility.h
 *
 *****************************************************************************/

/****************************************************************************
 *
 * Example: ufe_utility.h
 * Demonstrates prototypes and definitions of UFE utility functions
 *
 ****************************************************************************/
#ifndef UFE_UTILITY_H
#define UFE_UTILITY_H

#include <stdio.h>
#include <wufei_user_config.h>
#include <wufe_types.h>
#include <wufe_system.h>
#include <wufe_events.h>
#include <wufe_ufe.h>

#define PHY_TYPE_E1	1
#define PHY_TYPE_T1	0





WT_ufe ufe={0};

typedef struct
{
	WP_U32      ufeid;
	WP_U32      max_lines;
	WP_U32      max_phys;
	WP_U32      octets_per_line;
	WUFE_handle line_handle[WT_MAX_LINE_INDEX] ;
	WUFE_handle phy_handle[WT_MAX_LINE_INDEX] ;
} ufe4_system_data;

/******************************************
 * UFE4 SDH app system
 *****************************************/
ufe4_system_data ufe4_app_system = {
	/* ufeid           */ 0,
	/* max_lines       */ MAX_UFE_LINES,
	/* max_phys        */ MAX_UFE_LINES,
	/* octets_per_line */ OCTETS_PER_LINE,
	/* line_handle     */ {0},
	/* phy_handle      */ {0}
};


/******************************************
 * UFE4 SDh phy configuration
 *****************************************/
WUFE_phy ufe4_phy_cfg = {
	/* struct_id         */ WUFE_STRUCT_PHY_ATM,
	/* rx_fifo_param     */ {
		/* fifo_max_size     */ 2,
		/* priority_table    */ 0
	},
	/* rx_slot_group */ {
		/* hmvip_id          */ 0,
		/* slot_arr          */ { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
			17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
		/* n_slots           */ 30
	},
	/* tx_fifo_param; */ {
		/* fifo_max_size     */ 2,
		/* priority_table    */ 0,
		/* fast_mode         */ WUFE_PHY_FIFO_MODE_SLOW,
		/* transmit_th       */ 0,
		/* wait_type         */ WUFE_PHY_TX_WAIT_TYPE_D
	},
	/* tx_slot_group */ {
		/* hmvip_id          */ 0,
		/* slot_arr          */ { 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
			17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
		/* n_slots           */ 30
	},
	/* type          */ {
		/* atm             */ {
			/* rx_descrambler_mode        */ WUFE_PHY_ATM_RX_DESCRAMBLER_DISABLE,
			/* rx_coset_mode              */ WUFE_PHY_ATM_RX_NO_COSET,
			/* rx_idle_cell_discard       */ WUFE_PHY_ATM_RX_IDLE_CELL_DISCARD,
			/* rx_unsigned_cell_discard   */ WUFE_PHY_ATM_RX_UNASSIGNED_CELL_DISCARD,
			/* rx_hec_error_discard       */ WUFE_PHY_ATM_RX_HEC_ERR_TRANSFER,
			/* rx_hec_correction_mode     */ WUFE_PHY_ATM_RX_HEC_CORECTION_DISABLE,
			/* rx_cell_deliniation        */ WUFE_PHY_ATM_RX_CELL_DELINEATION_BYTE_RESOLUTION,
			/* rx_alpha                   */ 2,
			/* rx_delta                   */ 7,
			/* tx_scrambler_mode          */ WUFE_PHY_ATM_TX_SCRAMBLER_DISABLE,
			/* tx_coset_mode              */ WUFE_PHY_ATM_TX_NO_COSET,
			/* tx_idle_unsigned_cell_mode */ WUFE_PHY_ATM_TX_UNASSIGNED_CELL_MODE,
			/* tx_underrun_interrupt_mode */ WUFE_PHY_ATM_TX_UNDERRUN_INT_DISABLE
		}
	}
};


WUFE_line_sdh line_cfg[1] =
{
	{
		/*struct_id*/WUFE_STRUCT_LINE_SDH,
		/*stm4*/0,
		/*stm1*/0,
		/*stm0*/0,
		/*tug2*/0,
		/*tu*/0,
		/*transfer_type*/ WUFE_SDH_TYPE_E1,
		/*framed*/ WUFE_FRAMED,
		/*cas_enable*/WUFE_CAS_DISABLE,
		0
	}
};



/******************************************
 * UFE4 SDH system configuration
 *****************************************/
WUFE_system ufe4_system_cfg = {
	/* max_line_number     */ WTI_MAX_LINES,
	/* max_phy_number      */ WTI_MAX_LINES,
	/* emphy_global        */ {
		/* rx_pri_table          */ {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}},
		/* tx_pri_table          */ {{0, 0, 1, 2}, {0, 0, 1, 2}, {0, 0, 1, 2}, {0, 0, 1, 2}},
		/* tx_inframe_pri_table  */ {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}},
		/* rx_parity_mode        */ WUFE_RX_PARITY_EVEN,
		/* tx_parity_mode        */ WUFE_TX_PARITY_EVEN,
		/* tx_parity_check       */ WUFE_TX_PARITY_IGNORE//WUFE_TX_PARITY_CHECK
	},
	/* fifo_manager        */ {
		/* tx_fbp_size           */ MAX_UFE_LINES * 4,
		/* rx_fbp_size           */ MAX_UFE_LINES * 5
	},
	/* idle_pattern        */ {
		/* tx_idle_byte_data_pattern  */ 0xff,
		/* tx_idle_nibble_cas_pattern */ 0x4
	},
	/* line_interface      */ WUFE_INTERFACE_SDH,
	/* sbi                 */ {0},
	/* clock_rec_enable */ WUFE_CLOCK_REC_DISABLE
};






///////////////
// Functions //
///////////////

void WT_UfeTerminateOnError(WUFE_status handle, WP_CHAR *s,WP_U32 id,WP_U32 LineNum);
void WT_UfeLineSdhCreate(WT_ufe *ufe,
		WP_U32 line_index,
		WT_ufe_line_sdh_params *line_sdh_params,
		WP_U8 enable);

void WT_UfeLineCreate(WT_ufe *ufe, WP_U32 index, void *config, WP_U8 enable);

void WT_UfeLineConfigureAndCreateT1SDH(WP_U32 i, WP_U16 transfer_type, WP_U16 framing_mode, WP_U8 cas_enable, WP_U8 enable, WUFE_line_clock_rec_params *line_cr_params);

void WT_UfeLineSocketLpbkSetup(WT_ufe *ufe, int line_index, WUFE_line_loopback_type lpbk_type);

#if 0
void WT_UfePhyTransCreate(WT_ufe *ufe,
		WP_U32 line_index,
		WP_U32 phy_index,
		WP_U32 n_slots,
		WP_U8 *slots_arr,
		WP_U32 ufe_dataunit_size);
#endif

void WT_UfePhyEnable(WT_ufe *ufe, WP_U32 phy_index);

void WT_UfePhyDelete(WT_ufe *ufe, WP_U32 phy_index);

void WT_UfeLineDelete(WT_ufe *ufe, WP_U32 index);

void  WT_UfeRelease(WT_ufe *ufe);

void  WT_UfeSystemCreate(WT_ufe *ufe);

void  WT_UfeInitializeFirmware(WP_U32 ufe_id);

void  WT_UfeSystemEnable(WT_ufe *ufe);

void WT_UfePhyDisable(WT_ufe *ufe, WP_U32 phy_index);


#endif
