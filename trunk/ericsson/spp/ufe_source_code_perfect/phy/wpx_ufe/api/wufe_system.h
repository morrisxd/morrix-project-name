/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufe_system.h
 *
 * Purpose: the user system function call and structures
 *
 ****************************************************************************/

#ifndef WUFE_SYSTEM_H
#define WUFE_SYSTEM_H

#ifndef WUFE_LINE_H
#include "wufe_line.h"
#endif
#ifndef WUFE_SBI_H
#include "wufe_sbi.h"
#endif
#ifndef WUFE_USER_CONFIG_H
#include "wufe_user_config.h"
#endif
#ifndef WUFE_CLK_REC_H
#include "wufe_clk_rec.h"
#endif

/*************************************************
 ****************   EMPHY   **********************
 ************************************************/
#define WUFE_TX_PARITY_CHECK  1
#define WUFE_TX_PARITY_IGNORE 0
#define WUFE_PRI_DECRPTOR_NULL {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}

/* The number of priority descriptor in a priority table
   HW configuration in the EMPHY. */
#define WUFE_N_PRI_DESCRIPTOR 4
/* The number of valus that determine the priority descriptor
 (i.e. ) 1)less than 1/4 full.    2)between 1/4 1/2
         3) between 1/2 and 3/4   4) more than 3/4 full.
 HW configuration in the EMPHY. */
#define WUFE_PRI_DESCRIPTOR_LENGTH  4

typedef enum
{/* the WUFE_line_interface is the Glue logic define + 1 */
   WUFE_INTERFACE_FIRST             = 0,
   WUFE_INTERFACE_SBI               = WUFE_INTERFACE_FIRST,
   WUFE_INTERFACE_CAD_UNCHAN_DS3_E3,
   WUFE_INTERFACE_CAD_HMVIP,
   WUFE_INTERFACE_CAD_E1_T1,
   WUFE_INTERFACE_SONET,
   WUFE_INTERFACE_SDH,
   WUFE_INTERFACE_GENERIC,
   WUFE_INTERFACE_LAST               = WUFE_INTERFACE_GENERIC,
   WUFE_INTERFACE_INVALID            = -1
}WUFE_line_interface;

/* for Enable disable functions*/
typedef enum
{
   WUFE_SYS_EMPHY    = 1<<0,
   WUFE_SYS_SBI_RX_1 = 1<<1,
   WUFE_SYS_SBI_RX_2 = 1<<2,
   WUFE_SYS_SBI_RX_3 = 1<<3,
   WUFE_SYS_SBI_RX_4 = 1<<4,
   WUFE_SYS_SBI_RX_5 = 1<<5,
   WUFE_SYS_SBI_RX_6 = 1<<6,
   WUFE_SYS_SBI_RX_7 = 1<<7,
   WUFE_SYS_SBI_RX_8 = 1<<8,
   WUFE_SYS_SBI_RX_9 = 1<<9,
   WUFE_SYS_SBI_RX_10 = 1<<10,
   WUFE_SYS_SBI_RX_11 = 1<<11,
   WUFE_SYS_SBI_RX_12 = 1<<12,
   WUFE_SYS_SBI_TX_1 = 1<<13,
   WUFE_SYS_SBI_TX_2 = 1<<14,
   WUFE_SYS_SBI_TX_3 = 1<<15,
   WUFE_SYS_SBI_TX_4 = 1<<16,
   WUFE_SYS_SBI_TX_5 = 1<<17,
   WUFE_SYS_SBI_TX_6 = 1<<18,
   WUFE_SYS_SBI_TX_7 = 1<<19,
   WUFE_SYS_SBI_TX_8 = 1<<20,
   WUFE_SYS_SBI_TX_9 = 1<<21,
   WUFE_SYS_SBI_TX_10 = 1<<22,
   WUFE_SYS_SBI_TX_11 = 1<<23,
   WUFE_SYS_SBI_TX_12 = 1<<24,
   WUFE_SYS_SBI_RX_CORE0   =
   (WUFE_SYS_SBI_RX_1 | WUFE_SYS_SBI_RX_2 | WUFE_SYS_SBI_RX_3|
    WUFE_SYS_SBI_RX_4 | WUFE_SYS_SBI_RX_5 | WUFE_SYS_SBI_RX_6),
   WUFE_SYS_SBI_RX_CORE1   =
   (WUFE_SYS_SBI_RX_7 | WUFE_SYS_SBI_RX_8 | WUFE_SYS_SBI_RX_9|
    WUFE_SYS_SBI_RX_10 | WUFE_SYS_SBI_RX_11 | WUFE_SYS_SBI_RX_12),
   WUFE_SYS_SBI_RX =
   (WUFE_SYS_SBI_RX_CORE0 | WUFE_SYS_SBI_RX_CORE1),
   WUFE_SYS_SBI_TX_CORE0   =
   (WUFE_SYS_SBI_TX_1 | WUFE_SYS_SBI_TX_2 | WUFE_SYS_SBI_TX_3|
    WUFE_SYS_SBI_TX_4 | WUFE_SYS_SBI_TX_5 | WUFE_SYS_SBI_TX_6),
   WUFE_SYS_SBI_TX_CORE1   =
   (WUFE_SYS_SBI_TX_7 | WUFE_SYS_SBI_TX_8 | WUFE_SYS_SBI_TX_9|
    WUFE_SYS_SBI_TX_10 | WUFE_SYS_SBI_TX_11 | WUFE_SYS_SBI_TX_12),
   WUFE_SYS_SBI_TX =
   (WUFE_SYS_SBI_TX_CORE0 | WUFE_SYS_SBI_TX_CORE1),
   WUFE_SYS_SBI_FULL =
   (WUFE_SYS_SBI_RX | WUFE_SYS_SBI_TX)
} WUFE_sys_property;

typedef enum
{
   WUFE_FM_LOOPBACK_ENABLE =0,
   WUFE_FM_LOOPBACK_DISABLE
}WUFE_fm_line_loopback;

/*************************************************
 ****************  CLOCK RECOVERY ****************
 ************************************************/
typedef enum
{
   WUFE_CLOCK_REC_DISABLE = 0,
   WUFE_CLOCK_REC_ENABLE = 1
}WUFE_clock_rec_mode;

typedef enum
{
   WUFE_SYSTEM_CLOCK_REC_DISABLE = 0,
   WUFE_SYSTEM_CLOCK_REC_ENABLE  = 1
}WUFE_system_clock_rec;

/*************************************************
 ****************  EMPHY     *********************
 ************************************************/

typedef struct
{
      WP_U8 rx_pri_table[WUFE_N_PRI_DESCRIPTOR] [WUFE_PRI_DESCRIPTOR_LENGTH];
      WP_U8 tx_pri_table[WUFE_N_PRI_DESCRIPTOR] [WUFE_PRI_DESCRIPTOR_LENGTH];
      WP_U8 tx_inframe_pri_table[WUFE_N_PRI_DESCRIPTOR] [WUFE_PRI_DESCRIPTOR_LENGTH];
      WP_U8 rx_parity_mode;
      WP_U8 tx_parity_mode;
      WP_U8 tx_parity_check;
}WUFE_emphy_global;

/*************************************************
 **************** FIFO MANAG *********************
 ************************************************/
typedef struct
{
   WP_U16 tx_fbp_size;
   WP_U16 rx_fbp_size;
   WUFE_fm_line_loopback loopback;
}WUFE_fifo_manager;

/*************************************************
 **************** IDLE PATTERNS ******************
 ************************************************/
typedef struct
{
      WP_U8               tx_idle_byte_data_pattern;
      WP_U8               tx_idle_nibble_cas_pattern;
}WUFE_idle_pattern;

typedef enum
{
   WUFE_MAX_BUF_SIZE_64 =0,
   WUFE_MAX_BUF_SIZE_128,
   WUFE_MAX_BUF_SIZE_256 
}WUFE_max_buff_size;

typedef struct
{
      WP_U8 rx_parity_mode;
      WP_U8 tx_parity_mode;
}WUFE_tdm_parity;

typedef enum
{
   WUFE_FRAMER_PERSONALITY_412_1_OC12 =0,
   WUFE_FRAMER_PERSONALITY_412_2_OC3,
   WUFE_FRAMER_PERSONALITY_412_4_OC3,
   WUFE_FRAMER_PERSONALITY_448_4_OC12,
   WUFE_FRAMER_PERSONALITY_448_8_OC3
}WUFE_framer_build_personality;

/*************************************************
 ******************** SYSTEM *********************
 ************************************************/
typedef struct
{
      WP_U16                       max_line_number;
      WP_U16                       max_phy_number;
      WUFE_emphy_global            emphy;
      WUFE_fifo_manager            fifo_manager;
      WUFE_idle_pattern            idle_pattern;
      WUFE_line_interface          line_interface;
      WUFE_sbi_global              sbi;
      WUFE_system_clock_rec        system_clock_rec_enable;
      WUFE_clock_rec_system        *clock_rec_sys_params;  /* Relevant for UFE3 only */
      WUFE_max_buff_size           max_buff_num;
      WUFE_clk_rec_system_params   *clk_rec_system_params; /* Not relevant in UFE3 */
      WUFE_tdm_parity              tdm_parity;
      WUFE_framer_build_personality build_personality; /* Not relevant in UFE3 */
}WUFE_system;

typedef enum
{
   WUFE_DEVICE_TYPE_UFE412_T1=1,
   WUFE_DEVICE_TYPE_UFE412_T2,
   WUFE_DEVICE_TYPE_UFE412_T4,
   WUFE_DEVICE_TYPE_UFE412_M1,
   WUFE_DEVICE_TYPE_UFE412_M2,
   WUFE_DEVICE_TYPE_UFE412_M4,
   WUFE_DEVICE_TYPE_UFE448_T24,
   WUFE_DEVICE_TYPE_UFE448_T48,
   WUFE_DEVICE_TYPE_UFE448_M24,
   WUFE_DEVICE_TYPE_UFE448_M48,
   WUFE_DEVICE_TYPE_NONE
}  WUFE_device_type;

#define WUFE_NO_CHANGE 0xffffffff

typedef enum
{/*UFE4 Global configuration HW defenitions*/
   WUFE_FRAMER_TYPE_UNUSED =0,
   WUFE_FRAMER_TYPE_INTERNAL =1,
   WUFE_FRAMER_TYPE_EXTERNAL =2
}WUFE_framer_type;

enum {
   WUFE_SYSTEM_RECONFIG_SBI =0,
   WUFE_SYSTEM_RECONFIG_IDLE_PATTERN,
   WUFE_SYSTEM_RECONFIG_FM_LOOPBACK,
   WUFE_SYSTEM_RECONFIG_BUILD_PERSONALITY
};

#endif /* WUFE_SYSTEM_H*/
