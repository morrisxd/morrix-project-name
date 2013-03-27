/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_line.h
 *
 * Purpose: the user system function call and structures
 *
 ****************************************************************************/

#ifndef WUFE_LINE_H
#define WUFE_LINE_H

/*************************************************
 ***************** line enums ********************
 ************************************************/
typedef enum
{/* Order of bandwidth do not change !*/
   WUFE_CAD_TYPE_T1           =0,
   WUFE_CAD_TYPE_E1,
   WUFE_CAD_TYPE_HMVIP_4_T1,
   WUFE_CAD_TYPE_HMVIP_4_E1,
   WUFE_CAD_TYPE_E3,
   WUFE_CAD_TYPE_DS3,

   WUFE_SBI_TYPE_T1,
   WUFE_SBI_TYPE_E1,
   WUFE_SBI_TYPE_DS3,
   WUFE_SBI_TYPE_E3,
   WUFE_SBI_TYPE_IDLE, /* not specified */

   WUFE_GENERIC_TYPE_T1,
   WUFE_GENERIC_TYPE_E1,
   WUFE_GENERIC_TYPE_DS3, /* not supported yet */
   WUFE_GENERIC_TYPE_E3,   /* not supported yet */

   WUFE_SDH_TYPE_T1,
   WUFE_SDH_TYPE_E1,
   WUFE_SDH_TYPE_DS3,
   WUFE_SDH_TYPE_E3,
   WUFE_SDH_TYPE_VC11,
   WUFE_SDH_TYPE_VC12,
   WUFE_SDH_TYPE_TU3,
   WUFE_SDH_TYPE_VC3,
   WUFE_SDH_TYPE_VC4,
   WUFE_SDH_TYPE_VC44C,
   WUFE_SDH_TYPE_VCAT,

   WUFE_SONET_TYPE_T1,
   WUFE_SONET_TYPE_E1,
   WUFE_SONET_TYPE_DS3,
   WUFE_SONET_TYPE_E3,
   WUFE_SONET_TYPE_VT1_5,
   WUFE_SONET_TYPE_VT2,
   WUFE_SONET_TYPE_TU3,
   WUFE_SONET_TYPE_STS1_SPE,
   WUFE_SONET_TYPE_STS3,
   WUFE_SONET_TYPE_STS12,
   WUFE_SONET_TYPE_VCAT,

   WUFE_DCC_TYPE_MS,
   WUFE_DCC_TYPE_RS
}WUFE_line_transf_type;

typedef enum
{/*HW confiuration do not change.*/
   WUFE_UNFRAMED = 0,
   WUFE_FRAMED = 1
}WUFE_un_framed;

typedef enum
{/*HW confiuration do not change.*/
   WUFE_SINGLE_FRAME = 0,
   WUFE_MULTI_FRAME
}WUFE_multi_frame;

typedef enum
{/*HW confiuration do not change.*/
   WUFE_MULTI_F_SYNC_HIGH_START_OF_FRAME = 0,
   WUFE_MULTI_F_SYNC_LOW_START_OF_FRAME
}WUFE_multi_frame_sync;

typedef enum
{/*HW confiuration do not change.*/
   WUFE_MULTI_FRAME_16 = 0,
   WUFE_MULTI_FRAME_24 = 1
}WUFE_multi_frame_num;

typedef enum
{/*HW confiuration do not change.
   for CAD and SBI RX and TX*/
   WUFE_CAS_DISABLE = 0,
   WUFE_CAS_ENABLE = 1
}WUFE_cas_enable;

typedef enum
{/*HW confiuration do not change.*/
   WUFE_CAS_LOWER_NIBBLE = 1,
   WUFE_CAS_HIGHER_NIBBLE = 0
}WUFE_cas_alignmet_location;

typedef enum
{
   WUFE_LINE_CLOCK_SLAVE  = 0,
   WUFE_LINE_CLOCK_MASTER = 1
}WUFE_clock_master;

typedef enum
{
   WUFE_LINE_CLOCK_TRANSLATE_DISABLE  = 0,
   WUFE_LINE_CLOCK_TRANSLATE_ENABLE   = 1
}WUFE_clock_translate_enable;

typedef enum
{
   WUFE_LINE_CLOCK_TRANSLATE_ORIG_CLK  = 0,
   WUFE_LINE_CLOCK_TRANSLATE_DIV2_CLK  = 1
}WUFE_clock_translate_div2;

typedef enum
{
   WUFE_CAD_NORMAL           =0,
   WUFE_CAD_LOOPBACK_SYSTEM,
   WUFE_CAD_LOOPBACK_LINE,
   WUFE_CAD_LOOPBACK_INVALID
}WUFE_cad_loopback;

typedef enum
{
   WUFE_CAD_CR_REGULAR     =0,
   WUFE_CAD_CR_EXTENDED
}WUFE_cad_cr_extended;

/*************************************************
 ********************  LINE  *********************
 ************************************************/
typedef enum
{
   WUFE_LINE_CLOCK_REC_DISABLE = 0x0,
   WUFE_LINE_CLOCK_REC_ENABLE_TX = 0x1,
   WUFE_LINE_CLOCK_REC_ENABLE_RX = 0x2,
   WUFE_LINE_CLOCK_REC_ENABLE_DUPLEX = (WUFE_LINE_CLOCK_REC_ENABLE_TX |
                                        WUFE_LINE_CLOCK_REC_ENABLE_RX),
   WUFE_LINE_CLOCK_REC_ENABLE = WUFE_LINE_CLOCK_REC_ENABLE_DUPLEX,
   WUFE_LINE_CLOCK_REC_DCO_ONLY,
   WUFE_LINE_CLOCK_REC_NA
}WUFE_line_clock_rec_mode;

typedef enum
{
   WUFE_LINE_TX_CLOCK_REC_DISABLE = 0,
   WUFE_LINE_TX_CLOCK_REC_ENABLE
}WUFE_line_tx_clock_rec_mode;

typedef enum
{
   WUFE_CLOCK_REC_PHYSICAL_CLK_DISABLE = 0,
   WUFE_CLOCK_REC_PHYSICAL_CLK_ENABLE
}WUFE_line_clock_rec_physical_clock;

typedef enum
{
   WUFE_CLOCK_REC_DCO_ID_0 = 0,
   WUFE_CLOCK_REC_DCO_ID_1
}WUFE_line_clock_rec_physical_clock_id;

typedef struct
{
   WUFE_line_tx_clock_rec_mode           tx_clk_rec_enable;             /* Not relevant in UFE3 */
   WUFE_line_clock_rec_physical_clock    clk_rec_physical_clock_enable; /* Not relevant in UFE3 */
   WUFE_line_clock_rec_physical_clock_id clk_rec_physical_clock_id;     /* Not relevant in UFE3 */
}WUFE_line_clock_rec_params;

/*************************************************
 ********************   CAD  *********************
 ************************************************/

typedef struct
{
      WUFE_struct_id                struct_id;
      WP_U16                        cad_id;
      WUFE_line_transf_type         transfer_type;
      WUFE_un_framed                framed;
      WUFE_multi_frame              multi_frame;
      WUFE_multi_frame_sync         multi_frame_sync_level;
      WUFE_multi_frame_num          multi_frame_num;
      WUFE_cas_enable               cas_enable;
      WUFE_cas_alignmet_location    cas_alignment_location;
      WUFE_line_clock_rec_mode      clock_rec_mode;
      WP_U16                        rx_clk_rec_if;
      WUFE_cad_cr_extended          ext_mode_active;
      WUFE_cad_loopback             cad_loopback;
      WUFE_line_clock_rec_params    *clock_rec_line_params; /* Not relevant in UFE3 */
}WUFE_line_cad;

/* this structure is define for line reconfigure */
typedef struct
{
      WUFE_multi_frame              multi_frame;
      WUFE_multi_frame_sync         multi_frame_sync_level;
      WUFE_multi_frame_num          multi_frame_num;
      WUFE_cas_enable               cas_enable;
      WUFE_cas_alignmet_location    cas_alignment_location;
      WUFE_line_clock_rec_mode      clock_rec_mode;
      WP_U16                        rx_clk_rec_if;
      WUFE_cad_cr_extended          ext_mode_active;
      WUFE_line_clock_rec_params    *clock_rec_line_params; /* Not relevant in UFE3 */
}WUFE_line_cad_reconfig;

/*************************************************
 ********************   SBI  *********************
 ************************************************/
#define WUFE_LINE_SBI_CT_NOT_SET 0xffff
typedef struct
{
      WUFE_struct_id                struct_id;
      WP_U8                         spe_num;
      WP_U16                        sub_spe_id;
      WUFE_un_framed                framed;
      WUFE_cas_enable               cas_enable;
      WUFE_clock_master             tx_clk_master;
      WP_U16                        clock_translator_id;
      WUFE_line_clock_rec_mode      clock_rec_mode; /* for CD 0-31: set adap/diff, for 32-367 ignured */
      WP_U16                        rx_clk_rec_if;
      WP_U8                         t1oe1_mode;
}WUFE_line_sbi;

/* this structure is define for line reconfigure */
typedef struct
{
      WUFE_cas_enable               cas_enable;
      WUFE_clock_master             tx_clk_master;
      WP_U16                        clock_translator_id;
      WUFE_line_clock_rec_mode      clock_rec_mode;
      WP_U16                        rx_clk_rec_if;
      WP_U8                         t1oe1_mode;
}WUFE_line_sbi_reconfig;

/*************************************************
 ********************   SDH  *********************
 ************************************************/

typedef struct
{
      WUFE_struct_id struct_id;
      WP_U8 stm4;
      WP_U8 stm1;
      WP_U8 stm0;
      WP_U8 tug2;
      WP_U8 tu;
      WUFE_line_transf_type transfer_type;
      WUFE_un_framed  framed;
      WUFE_cas_enable cas_enable;
      WUFE_line_clock_rec_params *clock_rec_line_params; /* Not relevant in UFE3 */
} WUFE_line_sdh;

typedef struct
{
      WUFE_cas_enable cas_enable;
      WUFE_line_clock_rec_params *clock_rec_line_params; /* Not relevant in UFE3 */
}WUFE_line_sdh_reconfig;

/*************************************************
 ********************   SONET  *******************
 ************************************************/

typedef struct
{
      WUFE_struct_id struct_id;
      WP_U8 sts12;
      WP_U8 sts3;
      WP_U8 sts1;
      WP_U8 vt_group;
      WP_U8 vt;
      WUFE_line_transf_type transfer_type;
      WUFE_un_framed  framed;
      WUFE_cas_enable cas_enable;
      WUFE_line_clock_rec_params *clock_rec_line_params; /* Not relevant in UFE3 */
} WUFE_line_sonet;

typedef struct
{
      WUFE_cas_enable cas_enable;
      WUFE_line_clock_rec_params *clock_rec_line_params; /* Not relevant in UFE3 */
}WUFE_line_sonet_reconfig;

/*************************************************
 ********************   DCC  *********************
 ************************************************/
typedef struct
{
      WUFE_struct_id struct_id;
      WUFE_line_transf_type transfer_type;
      WP_U8 line_port_id;
} WUFE_line_dcc;

/*************************************************
 ********************   SOCKET  *********************
 ************************************************/
typedef struct
{
      WUFE_struct_id struct_id;
      WP_U8 ufe_core_id;
      WP_U8 stm1;
      WP_U8 spe;
      WP_U8 tug2;
      WP_U8 tu;
      WUFE_line_transf_type transfer_type;
      WUFE_un_framed  framed;
      WUFE_cas_enable cas_enable;
} WUFE_line_generic;

typedef enum
{
   WUFE_LINE_LOOPBACK_TYPE_LINE   = (1<<0), /* from line to line */
   WUFE_LINE_LOOPBACK_TYPE_SYSTEM = (1<<1), /* from ufe to ufe */
   WUFE_LINE_LOOPBACK_TYPE_NONE   = (1<<2)
} WUFE_line_loopback_type;

typedef enum
{
   WUFE_LINE_LOOPBACK_PDH_TYPE_LINE, /* from ufe to line (line facing loopback)*/
   WUFE_LINE_LOOPBACK_PDH_TYPE_SYSTEM, /* from ufe to winpath (system facing loopback)*/
   WUFE_LINE_LOOPBACK_PDH_TYPE_NONE
} WUFE_line_pdh_loopback_type;
/*****************************************/
/********     Use Macros           *******/
/*****************************************/

#define WUFE_LINE_INDEX_GET(line_handle) (line_handle)&((1<<16)-1)

#endif /* WUFE_LINE_H*/
