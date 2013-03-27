/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_clk_rec.h
 *
 * Purpose: the UFE clock recovery internal structures and prototypes.
 *
 ****************************************************************************/
#ifndef WUFEI_CLK_REC_H
#define WUFEI_CLK_REC_H


#ifndef WUFE_CLK_REC_H
#include "wufe_clk_rec.h"
#endif
#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif

#define WUFE_CLOCK_REC_REGULAR_INSTANCES       64
#define WUFE_CLOCK_REC_BUFFER_SIZE_INVALID     0x7FFF
#define WUFE_CLOCK_REC_INVALID_VALUE           0xFFFF
#define WUFE_CLOCK_REC_E1_FREQ                 2048000
#define WUFE_CLOCK_REC_T1_FREQ                 1544000

/***********************************************************************
 *                 GPIO output Clocks defines                          *
 ***********************************************************************/
#define WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_0    0x0000 /* Physical clock 0 */
#define WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_TX_CLK_1    0x0001 /* Physical clock 1 */
#define WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_LINE_CLK_0  0x0002
#define WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_PDH_LINE_CLK_1  0x0003
#define WUFEI_GPIO_CLK_OUT_0_SELECT_GP_CLK_IN_0          0x0006
#define WUFEI_GPIO_CLK_OUT_0_SELECT_GP_CLK_IN_1          0x0007
#define WUFEI_GPIO_CLK_OUT_0_SELECT_SYSTEM_CLK           0x0008
#define WUFEI_GPIO_CLK_OUT_0_SELECT_XO_PLL_REF_CLK       0x0009
#define WUFEI_GPIO_CLK_OUT_0_SELECT_RECV_SDH_LINE_CLK_1  0x000a
#define WUFEI_GPIO_CLK_OUT_0_SELECT_NONE                 0x000f /*0x4, 0x5 and 0xb-0xf reserved*/

#define WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_TX_CLK_0    0x0000 /* Physical clock 0 */
#define WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_TX_CLK_1    0x0100 /* Physical clock 1 */
#define WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_0  0x0200
#define WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_PDH_LINE_CLK_1  0x0300
#define WUFEI_GPIO_CLK_OUT_1_SELECT_GP_CLK_IN_0          0x0600
#define WUFEI_GPIO_CLK_OUT_1_SELECT_GP_CLK_IN_1          0x0700
#define WUFEI_GPIO_CLK_OUT_1_SELECT_SYSTEM_CLK           0x0800
#define WUFEI_GPIO_CLK_OUT_1_SELECT_XO_PLL_REF_CLK       0x0900
#define WUFEI_GPIO_CLK_OUT_1_SELECT_RECV_SDH_LINE_CLK_1  0x0a00
#define WUFEI_GPIO_CLK_OUT_1_SELECT_NONE                 0x0f00 /*0x0400, 0x0500 and 0x0b00-0x0f00 reserved*/

#define WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_XO_REF_CLK      0x0000
#define WUFEI_GPIO_REF_CLK_OUTPUT_SELECT_DIFF_REF_CLK    0x1000

typedef enum
{
   WUFEI_CLK_REC_VALID = 0x01,
   WUFEI_CLK_REC_USED = 0x02,
   WUFEI_CLK_REC_OCCUPIED = 0x04,
   WUFEI_CLK_REC_ACTIVE = (WUFEI_CLK_REC_VALID |
                           WUFEI_CLK_REC_USED)
}WUFEI_clk_rec_valid;

typedef struct
{
   WP_U32 phy_n;
   WP_U32 line_n;
   WP_U8* line_id_arr;
   WP_U32 data_unit_size; /*unframed line must acquire PHYs with a common data unit size*/
   WP_U8  transfer_type;  /*lines belong to the same CR must be from the same transfer type*/
   WP_U8  framed; /*lines that belong to the same CR must be from the same framing mode*/
   WP_U8  enable;
}WUFEI_clk_rec_rx;

typedef struct
{
   WP_U16  phy_id;/*phy index associated with this translator*/
   WP_U16  n_active_lines;/*number of slave phys that retreive the clock */
   WP_U8   state; /*entry state - WUFEI_clk_rec_valid type*/
   WP_U8   tx_data_unit_size;/*data unit size of the master PHY*/
}WUFEI_clk_rec_tx;

typedef struct
{
   WUFE_clock_rec_system cfg;
   WUFEI_clk_rec_rx *rx_arr;
   WUFEI_clk_rec_tx *tx_arr;
   WUFE_clk_rec_drc_divisor_list drc; /*if no user config passed-default values used. regular mode */
   WUFE_clk_rec_drc_divisor_list ext_drc; /*if no user config passed-default values used. ext. mode*/
   WP_U16 max_hw_rx_clk_rec_if;
   WP_U16 max_hw_ext_diff_rx_clk_rec_if;
   WP_U16 max_hw_tx_clk_rec_if;
   WP_U16 max_hw_ext_diff_tx_clk_rec_if;
} WUFEI_clk_rec_registry;

typedef struct
{
   WP_U8  dco_active;          /* The DCO is active and associate to a line */
   WP_U32 dco_reference_clock; /* The DCO reference clock (in Hz) */
   WUFE_clk_rec_drc_divisor divisor;  /* The DCO nominal (int+rem) divisor */
}WUFEI_clock_rec_dco_registry;

WUFE_status WUFEI_ClockRecSystemSetup(WUFEI_system_registry *system_registry,
                                      WUFE_clock_rec_system *params);
WUFE_status WUFEI_ClockRecSystemDelete(WUFEI_system_registry *system_registry);

WUFE_status WUFEI_ClockRecPhyEnable(WUFEI_system_registry *system_registry,
                                    WP_U32 core_id,
                                    WP_U16 emphy_port_id,
                                    WP_U16 phy_id,
                                    WP_U32 direction);

WUFE_status WUFEI_ClockRecPhyDisable(WUFEI_system_registry *system_registry,
                                     WP_U32 core_id,
                                     WP_U16 emphy_port_id,
                                     WP_U16 phy_id,
                                     WP_U32 direction);

WUFE_status  WUFEI_ClockRecPhyDelete(WUFEI_system_registry* system_registry,
                                     WP_U16 phy_id);

WUFE_status WUFEI_ClockRecTxInterfaceIdValidate(WUFEI_system_registry *system_registry,
                                                WUFE_line_transf_type transf_type,
                                                WP_U16 id);

WUFE_status WUFEI_CadClockRecTxExtendedModeValidate(WUFEI_system_registry *system_registry,
                                                    WUFE_cad_cr_extended extended_active,
                                                    WP_U16 line_id);

WUFE_status WUFEI_ClockRecRxInterfaceCreate(WUFEI_system_registry *system_registry,
                                            WUFEI_line_registry *line_registry);

WUFE_status WUFEI_ClockRecRxInterfaceEnable(WUFEI_system_registry *system_registry,
                                            WP_U16 clock_rec_if_id,
                                            WP_U16 line_id);

WUFE_status WUFEI_ClockRecRxInterfaceDisable(WUFEI_system_registry *system_registry,
                                             WP_U16 clock_rec_if_id,
                                             WP_U16 line_id);

WUFE_status WUFEI_ClockRecRxInterfaceDelete(WUFEI_system_registry *system_registry,
                                            WP_U16 clock_rec_if_id,
                                            WP_U16 line_id);

WUFE_status WUFEI_ClockRecConfigDivider(WP_U32 wufe_id,
                                        WUFEI_system_registry *system_registry,
                                        WP_U32 cr_id,
                                        WP_U32 line_transfer_type,
                                        WP_U8 enable);

WUFE_status WUFEI_ClockRecMethodGet(WUFEI_system_registry *system_registry,
                                    WP_U8 *method);
void WUFEI_ClockRecSystemDisplay(WUFEI_system_registry *system_registry);

/* UFE4 functions */
void WUFEI_LineClockRecClearParams(WUFEI_line_registry *line_registry);
WUFE_status WUFEI_ClockRecSystemEnable(WUFEI_system_registry *system_registry);
WUFE_status WUFEI_ClockRecSystemDisable(WUFEI_system_registry *system_registry);
WUFE_status WUFEI_ClockRecLineEnable(WP_U32 wufe_id, WUFEI_system_registry *system_registry,
                                     WP_U32 line_id, WUFEI_line_registry *line_registry);
WUFE_status WUFEI_ClockRecLineDisable(WP_U32 wufe_id, WUFEI_system_registry *system_registry,
                                      WP_U32 line_id, WUFEI_line_registry *line_registry);
WUFE_status WUFEI_ClockRecLineTxModeRamSet(WP_U16 wufe_id,
                                           WP_U16 line_id,
                                           WUFE_line_transf_type line_transfer_type,
                                           WP_U8 enable);
WUFE_status WUFEI_ClkRecModeSet(WUFEI_system_registry *system_registry, WP_U32 enable);

WUFE_status WUFEI_ClkRecPhysicalClockSet(WUFEI_system_registry *system_registry, WP_U32 line_id);
WUFE_status WUFEI_ClkRecPhysicalClockClear(WUFEI_system_registry *system_registry, WP_U32 line_id);
WUFE_status WUFEI_HwClockRecDisplay(WUFEI_system_registry *system_registry);

WUFE_status WUFEI_UfeGpioOutputClockSet(WUFEI_system_registry* system_registry,
                                        WP_U32 gpio_number, WP_U8 gpio_clock, WP_U8 gpio_ref_clk_out);
WUFE_status WUFEI_UfeGpioOutputClockRead(WUFEI_system_registry* system_registry,
                                         WP_U16 *gpio_register);
void WUFEI_UfeGpioOutputClockdisplay(WP_U16 gpio_register);
WUFE_status WUFEI_UfeGpioClockDefaultSetup(WUFEI_system_registry* system_registry);

#endif /*WUFEI_CLK_REC_H*/
