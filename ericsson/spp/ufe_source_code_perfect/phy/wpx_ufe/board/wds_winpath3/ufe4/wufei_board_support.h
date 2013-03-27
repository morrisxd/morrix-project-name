/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_board_support.h
 *
 * Purpose: the UFE4 board support registers map and board support defines
 *
 ****************************************************************************/
#ifndef WUFEI_BOARD_SUPPORT_H
#define WUFEI_BOARD_SUPPORT_H

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFEI_HW_TYPES_H
#include "wufei_hw_types.h"
#endif

#define MAX_SFP			8
#define MAX_BYTES		16
#define LOW		(WP_U8) 0
#define HIGH		(WP_U8) 1
#define OK 		        0
#define ERROR 		        1
#define NO_DEVICE	        2
#define WUFEI_UFE412_CHIP_SELECT  4 //valid values are 0-7

/* ERROR CODES */
#define ERR_SCL_BUSY		0x1
#define ERR_SDA_BUSY		0x2
#define ERR_STOP_SEQ		0x4
#define ERR_START_SEQ		0x8
#define ERR_SCL_ACK		0x10
#define ERR_SDA_ACK		0x20
#define ERR_ACK_TIMEOUT		0x40
#define ERR_SCL_SEND		0x80
#define ERR_SDA_SEND		0x100
#define ERR_SCL_READ		0x200
#define ERR_SCL_NOACK		0x400
#define ERR_SDA_NOACK		0x800
#define ERR_INIT		0x1000
#define ERR_DEVICE_DETECT	0x2000

struct i2c_regs {
	WP_U16 scl;
	WP_U16 sda_wr;
	WP_U16 sda_rd;
	WP_U16 busy;
	WP_S32 res[6];
	WP_U16 absent;
};

struct sfp_cmd {
	WP_S32 base;
	WP_U8 sfp_id;
	WP_U8 data;
	WP_S32 present_sfp;
	WP_S32 present_list[MAX_SFP];
};

typedef struct
{
      WUFEI_reg_space  bs_ind_low; /* indirect address low*/
      WUFEI_reg_space  bs_ind_high; /* indirect address high*/
      WUFEI_reg_space  bs_ind_cmd; /* indirect cmd*/
      WUFEI_reg_space  bs_ind_data; /* indirect data*/
      WUFEI_reg_space  reserved1[4]; /*reserved for future enhancements*/
      WUFEI_reg_space  bs_int_reg; /* interrupt register*/
      WUFEI_reg_space  bs_int_mask; /* interrupt mask register*/
      WUFEI_reg_space  reserved2[8]; /*reserved for future enhancements*/
      WUFEI_reg_space  bs_soft_reset; /* software reset */
      WUFEI_reg_space  reserved3[13]; /*reserved for future enhancements*/
      WUFEI_reg_space  bs_lock_indication; /* lock indication*/
      WUFEI_reg_space  bs_lock_lost_indication; /* lost lock indication*/
      WUFEI_reg_space  bs_lock_lost_indication_mask; /* lost lock indication mask*/
      WUFEI_reg_space  reserved4[13]; /*reserved for future enhancements*/
      WUFEI_reg_space  bs_ext_pll; /* external pll configuration register*/
      WUFEI_reg_space  bs_hif_zl_rd; /* external pll configuration register*/
      WUFEI_reg_space  bs_gpled; /* led light register*/
      WUFEI_reg_space  bs_clk_out_sel; /* Output Clock Selector register*/
      WUFEI_reg_space  reserved5[12]; /*reserved for future enhancements*/
      WUFEI_reg_space  bs_sfp_fo_scl; /* SFP I2C clock */
      WUFEI_reg_space  bs_sfp_fo_sda_wr; /* SFP I2C data */
      WUFEI_reg_space  bs_sfp_fo_sda_rd; /* SFP data read */
      WUFEI_reg_space  bs_sfp_fo_s_busy; /*SFP I2C busy */
      WUFEI_reg_space  bs_sfp_fo_tx_disable; /* disable SFP */
      WUFEI_reg_space  reserved6[11]; /*reserved for future enhancements*/
      WUFEI_reg_space  bs_sfp_fo_absent; /*sfp absent event register*/
      WUFEI_reg_space  bs_sfp_fo_absent_mask; /*sfp absent mask event register*/
      WUFEI_reg_space  bs_sfp_fo_los; /*sfp los event register*/
      WUFEI_reg_space  bs_sfp_fo_los_mask; /*sfp los mask event register*/
}WUFEI_hw_bs;

/*used by PllLock check*/
#define WUFEI_PLL_LOCK_TIMEOUT 0x1000
#define WUFEI_PLL_LOCK_DELAY 100

#define WUFEI_ZARLINK_PLL_LAST_ADDRESS_BIT 6
#define WUFEI_ZARLINK_PLL_LAST_DATA_BIT 7
#define WUFEI_ZARLINK_DELAY 500

/* Freq offset is a 28bit signed value */
#define WUFEI_ZLPLL_FREERUN_OFFSET_MAX_PPB 0x07FFFFFF
#define WUFEI_ZLPLL_FREERUN_OFFSET_MIN_PPB 0x08000000

enum
{
   WUFE_PLL_CONFIG_DEFAULT = 0,
   WUFE_PLL_CONFIG_APLL_NORMAL,
   WUFE_PLL_CONFIG_APLL_FREERUN_NO_OFFSET,
   WUFE_PLL_CONFIG_APLL_FREERUN_WITH_OFFSET
};

#define WUFE_BOARD_SUPPORT_ENABLE  0
#define WUFE_BOARD_SUPPORT_DISABLE 1

/***************************** Indirect ****************************************/
/*Indirect sub block*/
#define WUFEI_BS_IND_SUB_BLOCK_S (0)
#define WUFEI_BS_IND_SUB_BLOCK_W (4)
#define WUFEI_BS_IND_SUB_BLOCK_F(v) WUFEI_FIELD(v, WUFEI_BS_IND_SUB_BLOCK_S)
#define WUFEI_BS_IND_SUB_BLOCK_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_IND_SUB_BLOCK_S, WUFEI_BS_IND_SUB_BLOCK_W)
#define WUFEI_BS_IND_SUB_BLOCK_M \
        WUFEI_MASK(WUFEI_BS_IND_SUB_BLOCK_S, WUFEI_BS_IND_SUB_BLOCK_W)

/*Indirect block num*/
#define WUFEI_BS_IND_BLOCK_NUM_S (12)
#define WUFEI_BS_IND_BLOCK_NUM_W (4)
#define WUFEI_BS_IND_BLOCK_NUM_F(v) WUFEI_FIELD(v, WUFEI_BS_IND_BLOCK_NUM_S)
#define WUFEI_BS_IND_BLOCK_NUM_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_IND_BLOCK_NUM_S, WUFEI_BS_IND_BLOCK_NUM_W)
#define WUFEI_BS_IND_BLOCK_NUM_M \
        WUFEI_MASK(WUFEI_BS_IND_BLOCK_NUM_S, WUFEI_BS_IND_BLOCK_NUM_W)

/***************************** SW reset ****************************************/
#define WUFEI_BS_RESET_SW_S (0)
#define WUFEI_BS_RESET_SW_W (1)
#define WUFEI_BS_RESET_SW_F(v) WUFEI_FIELD(v, WUFEI_BS_RESET_SW_S)
#define WUFEI_BS_RESET_SW_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_RESET_SW_S, WUFEI_BS_RESET_SW_W)
#define WUFEI_BS_RESET_SW_M \
        WUFEI_MASK(WUFEI_BS_RESET_SW_S, WUFEI_BS_RESET_SW_W)

/***************************** interrupt ****************************************/
#define WUFEI_BS_INTERRUPT_INTU_S (0)
#define WUFEI_BS_INTERRUPT_INTU_W (1)
#define WUFEI_BS_INTERRUPT_INTU_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_INTU_S)
#define WUFEI_BS_INTERRUPT_INTU_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_INTU_S, WUFEI_BS_INTERRUPT_INTU_W)
#define WUFEI_BS_INTERRUPT_INTU_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_INTU_S, WUFEI_BS_INTERRUPT_INTU_W)

#define WUFEI_BS_INTERRUPT_INTF_S (1)
#define WUFEI_BS_INTERRUPT_INTF_W (1)
#define WUFEI_BS_INTERRUPT_INTF_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_INTF_S)
#define WUFEI_BS_INTERRUPT_INTF_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_INTF_S, WUFEI_BS_INTERRUPT_INTF_W)
#define WUFEI_BS_INTERRUPT_INTF_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_INTF_S, WUFEI_BS_INTERRUPT_INTF_W)

#define WUFEI_BS_INTERRUPT_EXT_PLL_S (2)
#define WUFEI_BS_INTERRUPT_EXT_PLL_W (1)
#define WUFEI_BS_INTERRUPT_EXT_PLL_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_EXT_PLL_S)
#define WUFEI_BS_INTERRUPT_EXT_PLL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_EXT_PLL_S, WUFEI_BS_INTERRUPT_EXT_PLL_W)
#define WUFEI_BS_INTERRUPT_EXT_PLL_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_EXT_PLL_S, WUFEI_BS_INTERRUPT_EXT_PLL_W)

#define WUFEI_BS_INTERRUPT_BS_PLL_S (3)
#define WUFEI_BS_INTERRUPT_BS_PLL_W (1)
#define WUFEI_BS_INTERRUPT_BS_PLL_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_BS_PLL_S)
#define WUFEI_BS_INTERRUPT_BS_PLL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_BS_PLL_S, WUFEI_BS_INTERRUPT_BS_PLL_W)
#define WUFEI_BS_INTERRUPT_BS_PLL_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_BS_PLL_S, WUFEI_BS_INTERRUPT_BS_PLL_W)

#define WUFEI_BS_INTERRUPT_SFP_S (4)
#define WUFEI_BS_INTERRUPT_SFP_W (1)
#define WUFEI_BS_INTERRUPT_SFP_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_SFP_S)
#define WUFEI_BS_INTERRUPT_SFP_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_SFP_S, WUFEI_BS_INTERRUPT_SFP_W)
#define WUFEI_BS_INTERRUPT_SFP_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_SFP_S, WUFEI_BS_INTERRUPT_SFP_W)

#define WUFEI_BS_INTERRUPT_MASKU_S (0)
#define WUFEI_BS_INTERRUPT_MASKU_W (1)
#define WUFEI_BS_INTERRUPT_MASKU_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_MASKU_S)
#define WUFEI_BS_INTERRUPT_MASKU_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_MASKU_S, WUFEI_BS_INTERRUPT_MASKU_W)
#define WUFEI_BS_INTERRUPT_MASKU_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_MASKU_S, WUFEI_BS_INTERRUPT_MASKU_W)

#define WUFEI_BS_INTERRUPT_MASKF_S (1)
#define WUFEI_BS_INTERRUPT_MASKF_W (1)
#define WUFEI_BS_INTERRUPT_MASKF_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_MASKF_S)
#define WUFEI_BS_INTERRUPT_MASKF_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_MASKF_S, WUFEI_BS_INTERRUPT_MASKF_W)
#define WUFEI_BS_INTERRUPT_MASKF_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_MASKF_S, WUFEI_BS_INTERRUPT_MASKF_W)

#define WUFEI_BS_INTERRUPT_MASK_EXT_PLL_S (2)
#define WUFEI_BS_INTERRUPT_MASK_EXT_PLL_W (1)
#define WUFEI_BS_INTERRUPT_MASK_EXT_PLL_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_MASK_EXT_PLL_S)
#define WUFEI_BS_INTERRUPT_MASK_EXT_PLL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_MASK_EXT_PLL_S, WUFEI_BS_INTERRUPT_MASK_EXT_PLL_W)
#define WUFEI_BS_INTERRUPT_MASK_EXT_PLL_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_MASK_EXT_PLL_S, WUFEI_BS_INTERRUPT_MASK_EXT_PLL_W)

#define WUFEI_BS_INTERRUPT_MASK_BS_PLL_S (3)
#define WUFEI_BS_INTERRUPT_MASK_BS_PLL_W (1)
#define WUFEI_BS_INTERRUPT_MASK_BS_PLL_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_MASK_BS_PLL_S)
#define WUFEI_BS_INTERRUPT_MASK_BS_PLL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_MASK_BS_PLL_S, WUFEI_BS_INTERRUPT_MASK_BS_PLL_W)
#define WUFEI_BS_INTERRUPT_MASK_BS_PLL_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_MASK_BS_PLL_S, WUFEI_BS_INTERRUPT_MASK_BS_PLL_W)

#define WUFEI_BS_INTERRUPT_MASK_SFP_S (4)
#define WUFEI_BS_INTERRUPT_MASK_SFP_W (1)
#define WUFEI_BS_INTERRUPT_MASK_SFP_F(v) WUFEI_FIELD(v, WUFEI_BS_INTERRUPT_MASK_SFP_S)
#define WUFEI_BS_INTERRUPT_MASK_SFP_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_INTERRUPT_MASK_SFP_S, WUFEI_BS_INTERRUPT_MASK_SFP_W)
#define WUFEI_BS_INTERRUPT_MASK_SFP_M \
        WUFEI_MASK(WUFEI_BS_INTERRUPT_MASK_SFP_S, WUFEI_BS_INTERRUPT_MASK_SFP_W)

/**************************** Lock Indication ****************************************/
#define WUFEI_BS_LOCK_IND_SYS_S (0)
#define WUFEI_BS_LOCK_IND_SYS_W (1)
#define WUFEI_BS_LOCK_IND_SYS_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_SYS_S)
#define WUFEI_BS_LOCK_IND_SYS_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_SYS_S, WUFEI_BS_LOCK_IND_SYS_W)
#define WUFEI_BS_LOCK_IND_SYS_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_SYS_S, WUFEI_BS_LOCK_IND_SYS_W)

#define WUFEI_BS_LOCK_IND_DDR_IO_S (1)
#define WUFEI_BS_LOCK_IND_DDR_IO_W (1)
#define WUFEI_BS_LOCK_IND_DDR_IO_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_DDR_IO_S)
#define WUFEI_BS_LOCK_IND_DDR_IO_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_DDR_IO_S, WUFEI_BS_LOCK_IND_DDR_IO_W)
#define WUFEI_BS_LOCK_IND_DDR_IO_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_DDR_IO_S, WUFEI_BS_LOCK_IND_DDR_IO_W)

#define WUFEI_BS_LOCK_IND_FRAMER_S (2)
#define WUFEI_BS_LOCK_IND_FRAMER_W (1)
#define WUFEI_BS_LOCK_IND_FRAMER_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_FRAMER_S)
#define WUFEI_BS_LOCK_IND_FRAMER_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_FRAMER_S, WUFEI_BS_LOCK_IND_FRAMER_W)
#define WUFEI_BS_LOCK_IND_FRAMER_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_FRAMER_S, WUFEI_BS_LOCK_IND_FRAMER_W)

#define WUFEI_BS_LOCK_IND_EXT_PLL_S (3)
#define WUFEI_BS_LOCK_IND_EXT_PLL_W (1)
#define WUFEI_BS_LOCK_IND_EXT_PLL_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_EXT_PLL_S)
#define WUFEI_BS_LOCK_IND_EXT_PLL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_EXT_PLL_S, WUFEI_BS_LOCK_IND_EXT_PLL_W)
#define WUFEI_BS_LOCK_IND_EXT_PLL_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_EXT_PLL_S, WUFEI_BS_LOCK_IND_EXT_PLL_W)

#define WUFEI_BS_LOCK_IND_ADAP_S (4)
#define WUFEI_BS_LOCK_IND_ADAP_W (1)
#define WUFEI_BS_LOCK_IND_ADAP_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_ADAP_S)
#define WUFEI_BS_LOCK_IND_ADAP_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_ADAP_S, WUFEI_BS_LOCK_IND_ADAP_W)
#define WUFEI_BS_LOCK_IND_ADAP_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_ADAP_S, WUFEI_BS_LOCK_IND_ADAP_W)

#define WUFEI_BS_LOCK_IND_DIFF_S (5)
#define WUFEI_BS_LOCK_IND_DIFF_W (1)
#define WUFEI_BS_LOCK_IND_DIFF_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_DIFF_S)
#define WUFEI_BS_LOCK_IND_DIFF_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_DIFF_S, WUFEI_BS_LOCK_IND_DIFF_W)
#define WUFEI_BS_LOCK_IND_DIFF_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_DIFF_S, WUFEI_BS_LOCK_IND_DIFF_W)

#define WUFEI_BS_LOCK_IND_DDR0_S (6)
#define WUFEI_BS_LOCK_IND_DDR0_W (1)
#define WUFEI_BS_LOCK_IND_DDR0_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_DDR0_S)
#define WUFEI_BS_LOCK_IND_DDR0_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_DDR0_S, WUFEI_BS_LOCK_IND_DDR0_W)
#define WUFEI_BS_LOCK_IND_DDR0_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_DDR0_S, WUFEI_BS_LOCK_IND_DDR0_W)

#define WUFEI_BS_LOCK_IND_DDR1_S (7)
#define WUFEI_BS_LOCK_IND_DDR1_W (1)
#define WUFEI_BS_LOCK_IND_DDR1_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_IND_DDR1_S)
#define WUFEI_BS_LOCK_IND_DDR1_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_IND_DDR1_S, WUFEI_BS_LOCK_IND_DDR1_W)
#define WUFEI_BS_LOCK_IND_DDR1_M \
        WUFEI_MASK(WUFEI_BS_LOCK_IND_DDR1_S, WUFEI_BS_LOCK_IND_DDR1_W)

#define WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_S (8)
#define WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_W (1)
#define WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_S)
#define WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_S, WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_W)
#define WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_M \
        WUFEI_MASK(WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_S, WUFEI_BS_LOCK_EXT_PLL_HOLDOVER_W)

#define WUFEI_BS_LOCK_EMPHY_RX_CLK_S (9)
#define WUFEI_BS_LOCK_EMPHY_RX_CLK_W (1)
#define WUFEI_BS_LOCK_EMPHY_RX_CLK_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_EMPHY_RX_CLK_S)
#define WUFEI_BS_LOCK_EMPHY_RX_CLK_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_EMPHY_RX_CLK_S, WUFEI_BS_LOCK_EMPHY_RX_CLK_W)
#define WUFEI_BS_LOCK_EMPHY_RX_CLK_M \
        WUFEI_MASK(WUFEI_BS_LOCK_EMPHY_RX_CLK_S, WUFEI_BS_LOCK_EMPHY_RX_CLK_W)

#define WUFEI_BS_LOCK_EMPHY_TX_CLK_S (10)
#define WUFEI_BS_LOCK_EMPHY_TX_CLK_W (1)
#define WUFEI_BS_LOCK_EMPHY_TX_CLK_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_EMPHY_TX_CLK_S)
#define WUFEI_BS_LOCK_EMPHY_TX_CLK_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_EMPHY_TX_CLK_S, WUFEI_BS_LOCK_EMPHY_TX_CLK_W)
#define WUFEI_BS_LOCK_EMPHY_TX_CLK_M \
        WUFEI_MASK(WUFEI_BS_LOCK_EMPHY_TX_CLK_S, WUFEI_BS_LOCK_EMPHY_TX_CLK_W)

/**************************** Lock Lost Indication ****************************************/
#define WUFEI_BS_LOCK_LOST_IND_SYS_S (0)
#define WUFEI_BS_LOCK_LOST_IND_SYS_W (1)
#define WUFEI_BS_LOCK_LOST_IND_SYS_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_LOST_IND_SYS_S)
#define WUFEI_BS_LOCK_LOST_IND_SYS_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_LOST_IND_SYS_S, WUFEI_BS_LOCK_LOST_IND_SYS_W)
#define WUFEI_BS_LOCK_LOST_IND_SYS_M \
        WUFEI_MASK(WUFEI_BS_LOCK_LOST_IND_SYS_S, WUFEI_BS_LOCK_LOST_IND_SYS_W)

#define WUFEI_BS_LOCK_LOST_IND_DDR_S (1)
#define WUFEI_BS_LOCK_LOST_IND_DDR_W (1)
#define WUFEI_BS_LOCK_LOST_IND_DDR_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_LOST_IND_DDR_S)
#define WUFEI_BS_LOCK_LOST_IND_DDR_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_LOST_IND_DDR_S, WUFEI_BS_LOCK_LOST_IND_DDR_W)
#define WUFEI_BS_LOCK_LOST_IND_DDR_M \
        WUFEI_MASK(WUFEI_BS_LOCK_LOST_IND_DDR_S, WUFEI_BS_LOCK_LOST_IND_DDR_W)

#define WUFEI_BS_LOCK_LOST_IND_FRAMER_S (2)
#define WUFEI_BS_LOCK_LOST_IND_FRAMER_W (1)
#define WUFEI_BS_LOCK_LOST_IND_FRAMER_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_LOST_IND_FRAMER_S)
#define WUFEI_BS_LOCK_LOST_IND_FRAMER_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_LOST_IND_FRAMER_S, WUFEI_BS_LOCK_LOST_IND_FRAMER_W)
#define WUFEI_BS_LOCK_LOST_IND_FRAMER_M \
        WUFEI_MASK(WUFEI_BS_LOCK_LOST_IND_FRAMER_S, WUFEI_BS_LOCK_LOST_IND_FRAMER_W)

#define WUFEI_BS_LOCK_LOST_IND_EXT_PLL_S (3)
#define WUFEI_BS_LOCK_LOST_IND_EXT_PLL_W (1)
#define WUFEI_BS_LOCK_LOST_IND_EXT_PLL_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_LOST_IND_EXT_PLL_S)
#define WUFEI_BS_LOCK_LOST_IND_EXT_PLL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_LOST_IND_EXT_PLL_S, WUFEI_BS_LOCK_LOST_IND_EXT_PLL_W)
#define WUFEI_BS_LOCK_LOST_IND_EXT_PLL_M \
        WUFEI_MASK(WUFEI_BS_LOCK_LOST_IND_EXT_PLL_S, WUFEI_BS_LOCK_LOST_IND_EXT_PLL_W)

#define WUFEI_BS_LOCK_LOST_IND_ADAP_S (4)
#define WUFEI_BS_LOCK_LOST_IND_ADAP_W (1)
#define WUFEI_BS_LOCK_LOST_IND_ADAP_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_LOST_IND_ADAP_S)
#define WUFEI_BS_LOCK_LOST_IND_ADAP_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_LOST_IND_ADAP_S, WUFEI_BS_LOCK_LOST_IND_ADAP_W)
#define WUFEI_BS_LOCK_LOST_IND_ADAP_M \
        WUFEI_MASK(WUFEI_BS_LOCK_LOST_IND_ADAP_S, WUFEI_BS_LOCK_LOST_IND_ADAP_W)

#define WUFEI_BS_LOCK_LOST_IND_DIFF_S (5)
#define WUFEI_BS_LOCK_LOST_IND_DIFF_W (1)
#define WUFEI_BS_LOCK_LOST_IND_DIFF_F(v) WUFEI_FIELD(v, WUFEI_BS_LOCK_LOST_IND_DIFF_S)
#define WUFEI_BS_LOCK_LOST_IND_DIFF_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_LOCK_LOST_IND_DIFF_S, WUFEI_BS_LOCK_LOST_IND_DIFF_W)
#define WUFEI_BS_LOCK_LOST_IND_DIFF_M \
        WUFEI_MASK(WUFEI_BS_LOCK_LOST_IND_DIFF_S, WUFEI_BS_LOCK_LOST_IND_DIFF_W)

/******************** External PLL configuration register **************************/
#define WUFEI_BS_EXT_PLL_WREN_S (3)
#define WUFEI_BS_EXT_PLL_WREN_W (1)
#define WUFEI_BS_EXT_PLL_WREN_F(v) WUFEI_FIELD(v, WUFEI_BS_EXT_PLL_WREN_S)
#define WUFEI_BS_EXT_PLL_WREN_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_EXT_PLL_WREN_S, WUFEI_BS_EXT_PLL_WREN_W)
#define WUFEI_BS_EXT_PLL_WREN_M \
        WUFEI_MASK(WUFEI_BS_EXT_PLL_WREN_S, WUFEI_BS_EXT_PLL_WREN_W)

#define WUFEI_BS_EXT_PLL_CS_B_ASEL0_S (2)
#define WUFEI_BS_EXT_PLL_CS_B_ASEL0_W (1)
#define WUFEI_BS_EXT_PLL_CS_B_ASEL0_F(v) WUFEI_FIELD(v, WUFEI_BS_EXT_PLL_CS_B_ASEL0_S)
#define WUFEI_BS_EXT_PLL_CS_B_ASEL0_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_EXT_PLL_CS_B_ASEL0_S, WUFEI_BS_EXT_PLL_CS_B_ASEL0_W)
#define WUFEI_BS_EXT_PLL_CS_B_ASEL0_M \
        WUFEI_MASK(WUFEI_BS_EXT_PLL_CS_B_ASEL0_S, WUFEI_BS_EXT_PLL_CS_B_ASEL0_W)

#define WUFEI_BS_EXT_PLL_SCK_SCL_S (1)
#define WUFEI_BS_EXT_PLL_SCK_SCL_W (1)
#define WUFEI_BS_EXT_PLL_SCK_SCL_F(v) WUFEI_FIELD(v, WUFEI_BS_EXT_PLL_SCK_SCL_S)
#define WUFEI_BS_EXT_PLL_SCK_SCL_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_EXT_PLL_SCK_SCL_S, WUFEI_BS_EXT_PLL_SCK_SCL_W)
#define WUFEI_BS_EXT_PLL_SCK_SCL_M \
        WUFEI_MASK(WUFEI_BS_EXT_PLL_SCK_SCL_S, WUFEI_BS_EXT_PLL_SCK_SCL_W)

#define WUFEI_BS_EXT_PLL_SI_SDA_S (0)
#define WUFEI_BS_EXT_PLL_SI_SDA_W (1)
#define WUFEI_BS_EXT_PLL_SI_SDA_F(v) WUFEI_FIELD(v, WUFEI_BS_EXT_PLL_SI_SDA_S)
#define WUFEI_BS_EXT_PLL_SI_SDA_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_EXT_PLL_SI_SDA_S, WUFEI_BS_EXT_PLL_SI_SDA_W)
#define WUFEI_BS_EXT_PLL_SI_SDA_M \
        WUFEI_MASK(WUFEI_BS_EXT_PLL_SI_SDA_S, WUFEI_BS_EXT_PLL_SI_SDA_W)

/***************************** GPLED register ***********************************************/
/* 1-led is off, 0-led is on*/
#define WUFEI_BS_GPLED_OFF_S (0)
#define WUFEI_BS_GPLED_OFF_W (3)
#define WUFEI_BS_GPLED_OFF_F(v) WUFEI_FIELD(v, WUFEI_BS_GPLED_OFF_S)
#define WUFEI_BS_GPLED_OFF_V(f) \
        WUFEI_VALUE(f, WUFEI_BS_GPLED_OFF_S, WUFEI_BS_GPLED_OFF_W)
#define WUFEI_BS_GPLED_OFF_M \
        WUFEI_MASK(WUFEI_BS_GPLED_OFF_S, WUFEI_BS_GPLED_OFF_W)

WP_S32 start_sequence(struct sfp_cmd *);
WP_S32 stop_sequence(struct sfp_cmd *);
WP_S32 wait_for_ack(struct sfp_cmd *);
WP_S32 send_byte(struct sfp_cmd *);
WP_S32 set_scl(struct sfp_cmd *);
WP_S32 set_sda(struct sfp_cmd *);
WP_U8  get_sda (struct sfp_cmd *);
WP_S32 read_data(WP_S32 pos, struct sfp_cmd *, WP_U8 *rom_value);
WP_S32 wait_busy(void);
WP_S32 set_none_ack(struct sfp_cmd *);

#endif /* WUFEI_BOARD_SUPPORT_H*/
