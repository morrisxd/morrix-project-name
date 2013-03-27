/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufe_sbi.h
 *
 * Purpose: the user sbi function call and structures
 *
 ****************************************************************************/

#ifndef WUFE_SBI_H
#define WUFE_SBI_H

#ifndef WUFE_USER_CONFIG_H
#include "wufe_user_config.h"
#endif
#ifndef WUFE_CLK_REC_H
#include "wufe_clk_rec.h"
#endif

/*************************************************
 ****************    SBI     *********************
 ************************************************/
typedef enum
{
   WUFE_SBI_CLK_19_MHZ=0,
   WUFE_SBI_CLK_77_MHZ=1,
   WUFE_SBI_CLK_INVALID
} WUFE_sbi_clock;

typedef enum
{
   WUFE_SBI_STM_0=0,
   WUFE_SBI_STM_1=1,
   WUFE_SBI_STM_2=2,
   WUFE_SBI_STM_3=3,
   WUFE_SBI_STM_INVALID
}WUFE_sbi_stm;

typedef enum
{
   WUFE_SBI_PARITY_NONE =0,
   WUFE_SBI_PARITY_ODD = 1,
   WUFE_SBI_PARITY_EVEN = 2,
   WUFE_SBI_PARITY_INVALID
} WUFE_sbi_parity;

typedef enum
{
   WUFE_SBI_C1FP_ALIGN_4  =1,
   WUFE_SBI_C1FP_ALIGN_48 =0,
   WUFE_SBI_C1FP_INVALID  =2
}WUFE_sbi_c1fp;

typedef enum
{
   WUFE_SBI_BUS_SLAVE     =0,
   WUFE_SBI_BUS_MASTER    =1,
   WUFE_SBI_BUS_INVALID
}WUFE_sbi_bus_cntrl;

typedef enum
{
   WUFE_SBI_NORMAL           =0,
   WUFE_SBI_LOOPBACK_SYSTEM  =1,
   WUFE_SBI_LOOPBACK         = WUFE_SBI_LOOPBACK_SYSTEM, /*UFE2 backward compatibility*/
   WUFE_SBI_LOOPBACK_LINE,
   WUFE_SBI_LOOPBACK_INVALID
}WUFE_loopback;

/*************************************************
 **************** SBI GLOBAL      ****************
 ************************************************/
#define WUFE_MAX_N_SBI_SPE (WUFE_MAX_N_CORE*WUFE_MAX_N_SPE*WUFE_MAX_N_STM1)

typedef struct
{
   WUFE_sbi_clock        in_clock;
   WUFE_sbi_stm          stm_mode;
   WUFE_sbi_parity       rx_parity;
   WUFE_sbi_parity       tx_parity;
   WUFE_sbi_c1fp         add_c1fp;
   WUFE_sbi_c1fp         drop_c1fp;
   WUFE_sbi_bus_cntrl    bus_master;
   WUFE_line_transf_type transfer_type_spe[WUFE_MAX_N_SBI_SPE];
   WUFE_loopback         loopback;
   WP_U16                max_clock_translators;
   WP_U16                max_ext_diff_clock_translators;
}WUFE_sbi_global;

typedef struct
{
   WUFE_sbi_clock        in_clock;
   WUFE_sbi_stm          stm_mode;
   WUFE_sbi_parity       rx_parity;
   WUFE_sbi_parity       tx_parity;
   WUFE_sbi_c1fp         add_c1fp;
   WUFE_sbi_c1fp         drop_c1fp;
   WUFE_sbi_bus_cntrl    bus_master;
   WUFE_line_transf_type transfer_type_spe[WUFE_MAX_N_SBI_SPE];
   WUFE_loopback         loopback;
}WUFE_sbi_global_reconfig;

/*************************************************
 **************** SBI CLOCK TRANSLATOR ***********
 ************************************************/
typedef struct
{
   WP_U16 id;
   WP_U8 tributary_type;
   WP_U8 clock_div2_mode;
}WUFE_sbi_clock_translator;

#endif /* WUFE_SBI_H*/
