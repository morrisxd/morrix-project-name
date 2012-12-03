/*
 * arnonm@wintegra.co.il
 * Copyright (C) 2002-2010 Wintegra. All rights reserved.
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 *
 * Defines of the Wintegra WinPath3 address-MAP, registers, etc.
 *
 */
#ifndef _WINTEGRA_WINPATH3_H
#define _WINTEGRA_WINPATH3_H

//#include "wds.h"


/*
 * Core ID
 */
#define Core0 0x0A0 /* The core with L1 cache */
#define Core1 0x0A2 /* The core with L1+L2 cache */


/*
 * Interrupt Controller
 */
#define MAP_IC_INTERUPT_L               (0xc280)
#define MAP_IC_INTERUPT_H               (0xc284)
#define MAP_IC_MASK_MIPS_0_L            (0xc288)
#define MAP_IC_MASK_MIPS_0_H            (0xc28c)
#define MAP_IC_IRPT_MIPS_0_L            (0xc290)
#define MAP_IC_IRPT_MIPS_0_H            (0xc294)
#define MAP_IC_FIRST_MIPS_0             (0xc298)
#define MAP_IC_MASK_MIPS_1_L            (0xc29c)
#define MAP_IC_MASK_MIPS_1_H            (0xc2a0)
#define MAP_IC_IRPT_MIPS_1_L            (0xc2a4)
#define MAP_IC_IRPT_MIPS_1_H            (0xc2a8)
#define MAP_IC_FIRST_MIPS_1             (0xc2ac)
#define MAP_IC_MASK_EXTERNAL_L          (0xc2b0)
#define MAP_IC_MASK_EXTERNAL_H          (0xc2b4)
#define MAP_IC_IRPT_EXTERNAL_L          (0xc2b8)
#define MAP_IC_IRPT_EXTERNAL_H          (0xc2bc)
#define MAP_IC_FIRST_EXTERNAL           (0xc2c0)
#define MAP_IC_MASK_PIN_L               (0xc2c4)
#define MAP_IC_MASK_PIN_H               (0xc2c8)
#define MAP_IC_IRPT_PIN_L               (0xc2cc)
#define MAP_IC_IRPT_PIN_H               (0xc2d0)
#define MAP_IC_FIRST_PIN                (0xc2d4)
#define MAP_IC_SW_INT                   (0xc2d8)
#define MAP_IC_INTMODE                  (0xc2dc)

#define WINPATH3_CORE0 0x0A0 /* The core with L1 cache */
#define WINPATH3_CORE1 0x0A2 /* The core with L1+L2 cache */

/* BIU registers*/
/* Packet (DDR) bus */
#define MAP_SIU1_REG_HWM_TR_ERR_STAT0        (0x10288)
#define MAP_SIU1_REG_HWM_TR_ERR_ADDR         (0x1028c)
#define MAP_SIU1_REG_HWM_TR_ERR_STAT1        (0x10294)
#define MAP_SIU1_REG_HWM_TR_ERR_STAT2        (0x10298)

#define WINPATH_PARAM_BIU_TESR         MAP_SIU1_REG_HWM_TR_ERR_STAT0
#define WINPATH_PARAM_BIU_TEAR         MAP_SIU1_REG_HWM_TR_ERR_ADDR
#define WINPATH_PARAM_BIU_HBMS1        MAP_SIU1_REG_HWM_TR_ERR_STAT1
#define WINPATH_PARAM_BIU_HBMS2        MAP_SIU1_REG_HWM_TR_ERR_STAT2

/* Param (SRAM) bus */
#define MAP_SIU2_REG_HWM_TR_ERR_STAT0        (0x10488)
#define MAP_SIU2_REG_HWM_TR_ERR_ADDR         (0x1048c)
#define MAP_SIU2_REG_HWM_TR_ERR_STAT1        (0x10494)
#define MAP_SIU2_REG_HWM_TR_ERR_STAT2        (0x10498)

#define WINPATH_PACKET_BIU_TESR        MAP_SIU2_REG_HWM_TR_ERR_STAT0
#define WINPATH_PACKET_BIU_TEAR        MAP_SIU2_REG_HWM_TR_ERR_ADDR
#define WINPATH_PACKET_BIU_HBMS1       MAP_SIU2_REG_HWM_TR_ERR_STAT1
#define WINPATH_PACKET_BIU_HBMS2       MAP_SIU2_REG_HWM_TR_ERR_STAT2

/* Internal (Global) bus */
#define MAP_SIU3_REG_HWM_TR_ERR_ADDR         (0x10628)
#define MAP_SIU3_REG_HWM_TR_ERR_STAT0        (0x1062c)
#define MAP_SIU3_REG_HWM_TR_ERR_STAT1        (0x10630)
#define MAP_SIU3_REG_HWM_TR_ERR_STAT2        (0x10634)

#define WINPATH_INTERNAL_BIU_TESR      MAP_SIU3_REG_HWM_TR_ERR_ADDR
#define WINPATH_INTERNAL_BIU_TEAR      MAP_SIU3_REG_HWM_TR_ERR_STAT0
#define WINPATH_INTERNAL_BIU_HBMS1     MAP_SIU3_REG_HWM_TR_ERR_STAT1
#define WINPATH_INTERNAL_BIU_HBMS2     MAP_SIU3_REG_HWM_TR_ERR_STAT2

/* RIF */
#define MAP_SIU3I_BIU_TESR                   (0x107f0)
#define MAP_SIU3I_BIU_TEAR                   (0x107f4)
#define MAP_SIU3I_BIU_HBMS1                  (0x107e8)
#define MAP_SIU3I_BIU_HBMS2                  (0x107ec)

#define WINPATH_RIF_BIU_TESR           MAP_SIU3I_BIU_TESR
#define WINPATH_RIF_BIU_TEAR           MAP_SIU3I_BIU_TEAR
#define WINPATH_RIF_BIU_HBMS1          MAP_SIU3I_BIU_HBMS1
#define WINPATH_RIF_BIU_HBMS2          MAP_SIU3I_BIU_HBMS2

/* Host bus */
#define MAP_SIU4X_BIU_TESR                   (0x108f0)
#define MAP_SIU4X_BIU_TEAR                   (0x108f4)
#define MAP_SIU4X_BIU_HBMS1                  (0x108e8)
#define MAP_SIU4X_BIU_HBMS2                  (0x108ec)

#define WINPATH_HOST_BIU_TESR          MAP_SIU4X_BIU_TESR
#define WINPATH_HOST_BIU_TEAR          MAP_SIU4X_BIU_TEAR
#define WINPATH_HOST_BIU_HBMS1         MAP_SIU4X_BIU_HBMS1
#define WINPATH_HOST_BIU_HBMS2         MAP_SIU4X_BIU_HBMS2

/* Application bus */
#define MAP_SIU4D_REG_HWM_TR_ERR_STAT0        (0x10A88)
#define MAP_SIU4D_REG_HWM_TR_ERR_ADDR         (0x10A8c)
#define MAP_SIU4D_REG_HWM_TR_ERR_STAT1        (0x10A94)
#define MAP_SIU4D_REG_HWM_TR_ERR_STAT2        (0x10A98)

#define WINPATH_APP_BIU_TESR           MAP_SIU4D_REG_HWM_TR_ERR_STAT0
#define WINPATH_APP_BIU_TEAR           MAP_SIU4D_REG_HWM_TR_ERR_ADDR
#define WINPATH_APP_BIU_HBMS1          MAP_SIU4D_REG_HWM_TR_ERR_STAT1
#define WINPATH_APP_BIU_HBMS2          MAP_SIU4D_REG_HWM_TR_ERR_STAT2)

/*
 *   TA registers
 */
#define WINPATH_TA_BASE                (0x1000)
#define WM_TA_BASE_REGS    512
#define WM_TA_ORDER_REGS   128
#define WM_TA_DEV_MAP_REGS 32
#define	MAP_TA_TRS_STAT			                        (0x1b00)/* Offset from WDS_WINPATH_BASE */
#define	MAP_TA_DEV_MAP			                        (0x1a00)/* Offset from WDS_WINPATH_BASE */
#define	MAP_TA_TIME_STAMP_TIMER_PRESCALAR			(0x1f80)/* Offset from WDS_WINPATH_BASE */
#define	MAP_TA_PC			                        (0x1d00)/* Offset from WDS_WINPATH_BASE */  
#define WM_TA_DEV_MAP_RES  (((MAP_TA_TRS_STAT-MAP_TA_DEV_MAP)-(WM_TA_DEV_MAP_REGS*4))/4)
#define WM_TA_TRS_SETS     64
#define WM_TA_PC_RES       (((MAP_TA_TIME_STAMP_TIMER_PRESCALAR-MAP_TA_PC)-(WM_TA_TRS_SETS*4))/4)

typedef struct wm_ta {
      unsigned int          b[WM_TA_BASE_REGS];
      volatile unsigned int order[WM_TA_ORDER_REGS];
      unsigned int          dev_map[WM_TA_DEV_MAP_REGS];
      unsigned int          res1[WM_TA_DEV_MAP_RES];
      volatile unsigned int trs_stat[WM_TA_TRS_SETS];
      volatile unsigned int ta_cam[WM_TA_TRS_SETS];
      volatile unsigned int ta_pc[WM_TA_TRS_SETS];
      unsigned int          res2[WM_TA_PC_RES];
      unsigned int          ts_prescalar;
      volatile unsigned int ts_timer[2];
} wm_ta;


/*
 *   Ethernet
 */
#define WINPATH_ENET_BASE             (0xB000)

/*
 *  Clocks and Reset (CR) registers
 */
#define MAP_CLK_SIU1_PLLCFG1     (0xfb04)
#define MAP_CLK_SIU2_PLLCFG1     (0xfc04)
#define MAP_CLK_INTERNAL_PLLCFG1 (0xff04)
#define MAP_CLK_SIU4D_PLLCFG1    (0xfd04)
#define MAP_CLK_MIPS_PLLCFG1     (0xfe04)

/*
 * UART registers
 */
#define WINPATH_UART_BASE       0xC500
#define WINPATH_UART_RBR        0
#define WINPATH_UART_TBR        0x4
#define WINPATH_UART_IER        0x8
#define WINPATH_UART_IIR        0xc
#define WINPATH_UART_FCR        0x10
#define WINPATH_UART_LCR        0x14
#define WINPATH_UART_MCR        0x18
#define WINPATH_UART_LSR        0x1c
#define WINPATH_UART_SCRATCH    0x24
#define WINPATH_UART_CLKDIV     0x28

#define	MAP_PCIE_REG_OF_EP_SEND_INTERRUPT    (0x11548)

#define MAP_I2C_CFG                          (0x10e80)
#define MAP_I2C_CLKDIV                       (0x10e84)
#define MAP_I2C_DEV_ADDR                     (0x10e88)
#define MAP_I2C_ADDR                         (0x10e8c)
#define MAP_I2C_DATA_OUT                     (0x10e90)
#define MAP_I2C_DATA_IN                      (0x10e94)
#define MAP_I2C_STATUS                       (0x10e98)
#define MAP_I2C_SDEN                         (0x10e9c)
#define MAP_I2C_BYTCNT                       (0x10ea0)

/*
 * Fuses
 */
#define	MAP_FUSE_REGITER0		     (0x10f08)
#define	MAP_FUSE_REGITER1		     (0x10f0C)

/*
 * interface functions
 */
extern char *prom_getcmdline(void);
extern void prom_init_cmdline(void);
extern void prom_meminit(void);
extern void mips_reboot_setup(void);
extern int prom_putchar(char);
extern unsigned int getHostSystemClock(int);
extern int prom_get_ethernet_addr(unsigned char *);

/* Memory descriptor management. */
#define PROM_MAX_PMEMBLOCKS    32
struct prom_pmemblock {
  unsigned long base; /* Within KSEG0. */
  unsigned int size;  /* In bytes. */
  unsigned int type;  /* free or prom memory */
};


#define WINPATH_INSTRUMENTATION_ENABLE 0

#if WINPATH_INSTRUMENTATION_ENABLE
extern void inst_mark(unsigned int code);
#endif

#endif  /* !(_WINTEGRA_WINPATH3_H) */
