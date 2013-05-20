/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_phy_data.h
 *
 * Purpose: Define board-specific data used by either the phy utilities or
 * by the wpx_board_data.c.
 *
 * The data in this file is defined in a form which can be used for all targets
 * using the board.
 *
 * This file may be included only by wpx_phy_data.h and by wpx_board_data.c.
 *
 * This file must not be included by any file in wddi/sources, since a build
 * of WDDI should always result in a board-independent object.
 *
 * The addresses defined here must be physical addresses only.  Any target must
 * do its own translation of these addresses to virtual addresses.
 *
 * The data contained in this file should not be needed by an
 * application.
 ****************************************************************************/

#ifndef WPX_BOARD_DATA_H
#define WPX_BOARD_DATA_H

#ifndef WPX_BOARD_IF_H
#include "../board/wpx_board_if.h"
#endif

/* backward compatability */
#define WPX_CpldSet WPX_FpgaSet

/* Physical address of Winpath register base */
WP_U32 WPX_AppWpIntRegBaseGet(WP_U32 wpid);


/* Update FPGA registers*/
void WPX_FpgaMatchRegSet(WP_U32 wpid);
WP_status WPX_FpgaUpdateMux(WP_U32 wpid, wpx_board_configuration configuration);
WP_status WPX_FpgaSet(WP_U32 port, WP_U32 mode);
WP_status WPX_BoardSerdesInit( WP_U32 wpid,
                               WP_port port,
                               WP_U32 loopback_mode);
WP_status WPX_BoardSetUpiLevel( WP_U32 wpid,
                                WP_port port,
                                WP_U32 level);
WP_status WPX_BoardZarlinkPllConfig(WP_U32 wpid, WP_U32 setup_cmd);
WP_status WPX_BoardPhyAel2005Config(WP_U32 wpid, WP_port port);
WP_status WPX_SfpTxEnable(WP_U32 wpid, 
                          WP_U8 enable, 
                          WP_U32 mask);

typedef struct  {
      WP_U8 revision_id; /*offset 0*/
      WP_U8 reserved1;
      WP_U8 reset_all;/*offset 2*/
      WP_U8 reserved2;
      WP_U8 reset_10G;/*offset 4*/
      WP_U8 reserved3[3];
      WP_U8 reset_pll;/*offset 8*/
      WP_U8 reserved4;
      WP_U8 int_source;/*offset 0xa*/
      WP_U8 reserved5;
      WP_U8 int_mask;/*offset 0xc*/
      WP_U8 reserved6;
      WP_U8 pll_in1_ctrl;/*offset 0xe*/
      WP_U8 reserved7;
      WP_U8 pll_in2_ctrl;/*offset 0x10*/
      WP_U8 reserved8;
      WP_U8 mux_select_1;/*offset 0x12*/
      WP_U8 reserved9;
      WP_U8 mux_select_2;/*offset 0x14*/
      WP_U8 reserved10;
      WP_U8 mux_select_3;/*offset 0x16*/
      WP_U8 reserved11[3];
      WP_U8 serial_control;/*offset 0x1a*/
      WP_U8 reserved12;
      WP_U8 sfp_tx_control;/*offset 0x1c*/
      WP_U8 reserved13;
      WP_U8 leds;/*offset 0x1e*/
      WP_U8 reserved14[3];
      WP_U8 dip_switch;/*offset 0x22*/
      WP_U8 reserved15;
      WP_U8 card_status;/*offset 0x24*/
      WP_U8 reserved16;
      WP_U8 interrupt_select;/*offset 0x26*/
      WP_U8 reserved17;
      WP_U8 spi_read;/*offset 0x28*/
      WP_U8 reserved18[7];
      WP_U8 spi_pll;/*offset 0x30*/
      WP_U8 reserved20;
      WP_U8 spi_10G;/*offset 0x32*/
      WP_U8 reserved21;
      WP_U8 spi_10G_opt;/*offset 0x34*/
      WP_U8 reserved22;
      WP_U8 lattice;/*offset 0x36*/
      WP_U8 reserved23;
      WP_U32 fpga_config;    /*offset 0x38*/
      WP_U32 fpga_reg_addr;  /*offset 0x3c*/
      WP_U32 fpga_write_data;/*offset 0x40*/
      WP_U32 fpga_read_data; /*offset 0x44*/
      WP_U32 reserved24;
      WP_U8  fpga_smw;       /*offset 0x4c*/
      WP_U8 reserved25[3];
      WP_U8 brg_mux_select;      /* 0x50 */
      WP_U8 reserved31;
      WP_U8 pps_and_fclk_select; /* 0x52 */
      WP_U8 reserved32;
      WP_U8 sma_direction;       /* 0x54 */
      WP_U8 reserved33[3];
      WP_U8 pll_source;          /* 0x58 */
      WP_U8 reserved34;
}wpx_fpga;


WP_status WPX_BoardRevisionGet(WP_U32 wpid, wpx_fpga *fpga, WP_U32 *revision);

#define WPX_ZARLINK_PLL_LAST_ADDRESS_BIT 6
#define WPX_ZARLINK_PLL_LAST_DATA_BIT 7
#define WPX_XGI1_PHY_OFFSET 1
#define WPX_XGI2_PHY_OFFSET 2
#define WPX_AEL2005_WRITE_OK_BIT  0x00000000
#define WPX_AEL2005_WRITE_OK_MASK 0x00000100
#define WPX_AEL2005_POOL_LOOPS 1000
#define WPX_AEL2005_POOL_DELAY 10
#define WPX_ZARLING_DELAY 500

#define WPX_FPGA_MUX_SELECT1_UPI1     0x2
#define WPX_FPGA_MUX_SELECT1_UPI2     0x1
#define WPX_FPGA_MUX_SELECT1_UPI1_UPI2_UPI3 0x0
#define WPX_FPGA_MUX_SELECT1_NON_UPI   0x7
#define WPX_PHY_REG_MULTIPLIER 1

/* EMPHY clock configuration */
#define WPX_FPGA_MUX_SELECT2_UPI1_UPI2_100M  0x18
#define WPX_FPGA_MUX_SELECT2_UPI_104MHZ 0x7

/* Clock Recovery and Timing defines */
#define WPX_FPGA_BRG_C_INPUT_SYNT_CLK  0x0
#define WPX_FPGA_BRG_C_INPUT_SMA7      0x8
#define WPX_FPGA_BRG_C_INPUT_APLL0_CLK 0x10
#define WPX_FPGA_BRG_C_INPUT_P1_CLK0   0x18 /* Will be available from PCB REV3 */
#define WPX_FPGA_BRG_D_INPUT_SYNT_CLK  0x0
#define WPX_FPGA_BRG_D_INPUT_SMA6      0x1
#define WPX_FPGA_BRG_D_INPUT_SMA4      0x2
#define WPX_FPGA_BRG_D_INPUT_P0_CLK0   0x3

#define WPX_FPGA_SMA1_OUTPUT_TCXO      0x0
#define WPX_FPGA_SMA1_OUTPUT_APLL0_CLK 0x10
#define WPX_FPGA_SMA1_OUTPUT_P0_CLK0   0x20 /* Available from PCB REV3 */
#define WPX_FPGA_SMA1_OUTPUT_P1_CLK0   0x30 /* Available from PCB REV3 */
#define WPX_FPGA_CR_OUTPUT_PPS_1       0x0
#define WPX_FPGA_CR_OUTPUT_PPS_2       0x4
#define WPX_FPGA_SYNC_ETH_CLK0         0x8
#define WPX_FPGA_SYNC_ETH_CLK1         0xc
#define WPX_FPGA_CR_OUTPUT_FCLK_1      0x0
#define WPX_FPGA_CR_OUTPUT_FCLK_2      0x1

#define WPX_FPGA_SMA5_DIR_SMA_TO_WP    0x0
#define WPX_FPGA_SMA5_DIR_WP_TO_SMA    0x10
#define WPX_FPGA_SMA4_DIR_SMA_TO_WP    0x0
#define WPX_FPGA_SMA4_DIR_WP_TO_SMA    0x8
#define WPX_FPGA_SMA3_DIR_SMA_TO_WP    0x0
#define WPX_FPGA_SMA3_DIR_WP_TO_SMA    0x4
#define WPX_FPGA_SMA2_DIR_SMA_TO_WP    0x0
#define WPX_FPGA_SMA2_DIR_WP_TO_SMA    0x2
#define WPX_FPGA_SMA1_DIR_SMA_TO_WP    0x0 /* N/A */
#define WPX_FPGA_SMA1_DIR_MUX_TO_SMA   0x1
#define WPX_FPGA_SERDES_X_APLL0_CLK    0x4
#define WPX_FPGA_SERDES_X_SELECT_CLK   0x0
#define WPX_FPGA_SERDES_1_CLK_100      0x2
#define WPX_FPGA_SERDES_1_CLK_125      0x0
#define WPX_FPGA_SERDES_0_CLK_100      0x1
#define WPX_FPGA_SERDES_0_CLK_125      0x0

#define WPX_FPGA_OSCI_SRC_FROM_SYNT_CLK          0x0  /* Available from PCB REV4 */
#define WPX_FPGA_OSCI_SRC_FROM_SMA7              0x10 /* Available from PCB REV4 */
#define WPX_FPGA_PLL_REFCLK2_FROM_SYNC_ETH_CLK1  0x0
#define WPX_FPGA_PLL_REFCLK2_FROM_CLK_REC_FCLK_2 0x4
#define WPX_FPGA_PLL_REFCLK2_FROM_SMA7           0x8
#define WPX_FPGA_PLL_REFCLK1_FROM_SYNC_ETH_CLK0  0x0
#define WPX_FPGA_PLL_REFCLK1_FROM_CLK_REC_FCLK_1 0x1
#define WPX_FPGA_PLL_REFCLK1_FROM_SMA6           0x2

#define WPX_FPGA_DIP_SWITCH_BOARD_TYPE_SHIFT 4
#define WPX_FPGA_DIP_SWITCH_REVISION_SHIFT   5
#define WPX_FPGA_DIP_SWITCH_BOARD_TYPE_MASK 0x30
#define WPX_FPGA_SINGLE_XGI 0
#define WPX_FPGA_DUAL_XGI 1
#define WPX_SFP_EN_SERD_0_1 0x1
#define WPX_SFP_EN_SERD_2 0x2
#define WPX_SFP_EN_SERD_3 0x4
#define WPX_SFP_EN_SERD_4 0x8
#define WPX_SFP_EN_ALL (WPX_SFP_EN_SERD_0_1 | WPX_SFP_EN_SERD_2 | WPX_SFP_EN_SERD_3 | WPX_SFP_EN_SERD_4)

#define WPX_SFP_EN_SERD_0_1_MASK 0x1
#define WPX_SFP_EN_SERD_2_MASK 0x4
#define WPX_SFP_EN_SERD_3_MASK 0x8
#define WPX_SFP_EN_SERD_4_MASK 0x2

#define WPX_SFP_EN_SERD_0_1_MASK_SHIFT 0
#define WPX_SFP_EN_SERD_2_MASK_SHIFT 2
#define WPX_SFP_EN_SERD_3_MASK_SHIFT 3
#define WPX_SFP_EN_SERD_4_MASK_SHIFT 1

#define WPX_SFP_EN_SERD_0_1_ENABLE 0
#define WPX_SFP_EN_SERD_0_1_DISABLE 1
#define WPX_SFP_EN_SERD_2_ENABLE_SINGLE_XGI 0
#define WPX_SFP_EN_SERD_2_DISABLE_SINGLE_XGI 1
#define WPX_SFP_EN_SERD_2_ENABLE_DUAL_XGI 1
#define WPX_SFP_EN_SERD_2_DISABLE_DUAL_XGI 0
#define WPX_SFP_EN_SERD_3_ENABLE 1
#define WPX_SFP_EN_SERD_3_DISABLE 0
#define WPX_SFP_EN_SERD_4_ENABLE 0
#define WPX_SFP_EN_SERD_4_DISABLE 1

enum
{
   WPX_BOARD_REV_SLA_SLB = 0,
   WPX_BOARD_REV_SPO = 1,
};


typedef struct  {
      WP_U8 value;
}wpx_fpga_config;

enum
{
   WPX_FPGA_NA = 0,
   WPX_FPGA_BRG_C_IN_SYNT_CLK,
   WPX_FPGA_BRG_C_IN_SMA7,
   WPX_FPGA_BRG_C_IN_APLL0_CLK,
   WPX_FPGA_BRG_C_IN_P1_CLK0,   /* Available from PCB REV3 */
   WPX_FPGA_BRG_D_IN_SYNT_CLK,
   WPX_FPGA_BRG_D_IN_SMA6,
   WPX_FPGA_BRG_D_IN_SMA4,
   WPX_FPGA_BRG_D_IN_P0_CLK0,
   WPX_FPGA_BRG_B_IN_SMA5,
   WPX_FPGA_CR_FCLK1_OUT_SMA5,
   WPX_FPGA_CR_FCLK1_OUT_SMA4,
   WPX_FPGA_CR_FCLK2_OUT_SMA4,
   WPX_FPGA_CR_PPS1_OUT_SMA3,
   WPX_FPGA_CR_PPS1_OUT_SMA2,
   WPX_FPGA_CR_PPS2_OUT_SMA2,
   WPX_FPGA_CR_PPS1_IN_SMA3,
   WPX_FPGA_CR_PPS2_IN_SMA2,
   WPX_FPGA_SYNC_ETH0_OUT_SMA2,
   WPX_FPGA_SYNC_ETH1_OUT_SMA2,
   WPX_FPGA_SMA1_OUT_TCXO,
   WPX_FPGA_SMA1_OUT_APLL0_CLK,
   WPX_FPGA_SMA1_OUT_P0_CLK0,  /* Available from PCB REV3 */
   WPX_FPGA_SMA1_OUT_P1_CLK0,  /* Available from PCB REV3 */
   WPX_FPGA_SERDES_SELECT_CLK,
   WPX_FPGA_SERDES_CLK_APLL0,
   WPX_FPGA_SERDES_CLK_100,
   WPX_FPGA_SERDES_CLK_125,
   WPX_FPGA_OSCI_SOURCE_SYNT_CLK,
   WPX_FPGA_OSCI_SOURCE_SMA7,
   WPX_FPGA_PLL_REFCLK2_SYNC_ETH_CLK1,
   WPX_FPGA_PLL_REFCLK2_CLK_REC_FCLK_2,
   WPX_FPGA_PLL_REFCLK2_SMA7,
   WPX_FPGA_PLL_REFCLK1_SYNC_ETH_CLK0,  
   WPX_FPGA_PLL_REFCLK1_CLK_REC_FCLK_1, 
   WPX_FPGA_PLL_REFCLK1_SMA6            
};


typedef struct  {
      WP_U8 sma1_out;
      WP_U8 brg_b;
      WP_U8 brg_c;
      WP_U8 brg_d;
      WP_U8 clk_rec_fclk1;
      WP_U8 clk_rec_fclk2;
      WP_U8 clk_rec_pps1_output;
      WP_U8 clk_rec_pps1_input;
      WP_U8 clk_rec_pps2_output;
      WP_U8 clk_rec_pps2_input;
      WP_U8 sync_eth_clk0_output;
      WP_U8 sync_eth_clk1_output;
      WP_U8 serdes_x_select;
      WP_U8 serdes1_ref_clk;
      WP_U8 serdes0_ref_clk;
      WP_U8 osci_sel;
      WP_U8 pll_ref_clk2_sel;
      WP_U8 pll_ref_clk1_sel;
}wpx_wp_cr_pin_io;

enum
{
   WPX_PLL_CONFIG_DEFAULT = 0,
   WPX_PLL_CONFIG_APLL_125,
   WPX_PLL_CONFIG_APLL_125_FREERUN,
   WPX_PLL_CONFIG_APLL_125_MANUAL_NORMAL,
   WPX_PLL_CONFIG_APLL_125_P0_CLK0_100,
};

/***********************************************************
 * PLL parametes structures
 ************************************************************/
enum wpx_pll_clock_out
{
   wpx_apll_clk0,
   wpx_p0_clk0,
   wpx_p1_clk0
};

enum wpx_apll_clk0_rate
{
   /* SONET/SDH MODE
      eth_en = 0
      f_seln = 0 */
   wpx_f_77_76,
   wpx_f_38_88,
   wpx_f_19_44,
   wpx_f_9_72,
   wpx_f_51_84,
   wpx_f_25_92,
   wpx_f_12_96,
   wpx_f_6_48,

   /* Ethernet MODE
      eth_en = 1
      f_seln = 1 */
   wpx_f_125,
   wpx_f_62_5,
   wpx_f_50,
   wpx_f_25,
   wpx_f_12_5
};

typedef struct
{
   WP_U32 wpid;
   enum wpx_pll_clock_out clock_out;
   WP_U32 clock_rate;
   enum wpx_apll_clk0_rate apll_rate;
}WPX_pll_cfg;

/* CR and PTP FPGA registers */
WP_status WPX_CrBoardPinConfig(WP_U32 wpid, wpx_wp_cr_pin_io wp_cr_pin_io);
WP_status WPX_CrBoardConfiguration(WP_U32 wpid, wpx_board_configuration configuration,
                                   wpx_fpga_config *config_value);
void WPX_ConfigBrgMux(wpx_fpga *fpga, wpx_fpga_config *config_value);
void WPX_ConfigPpsAndFclkSelect(wpx_fpga *fpga, wpx_fpga_config *config_value);
void WPX_ConfigSmaDirection(wpx_fpga *fpga, wpx_fpga_config *config_value);
void WPX_ConfigPllSource(wpx_fpga *fpga, wpx_fpga_config *config);
void WPX_ConfigMux3Reg(wpx_fpga *fpga, wpx_fpga_config *config);
void WPX_FpgaFirmwareInitializationStart(WP_U32 wpid);
#endif
