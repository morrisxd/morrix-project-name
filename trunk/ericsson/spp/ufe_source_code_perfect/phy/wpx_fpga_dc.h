/*****************************************************************************
 * (C) Copyright 2010, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_fpga_dc.h
 *
 * Purpose: FPGA daughter card (DC) 
 *
 ****************************************************************************/
#ifndef WPX_FPGA_DC_H
#define WPX_FPGA_DC_H

#define WPU_FPGA_DC_SPHY 1
#define WPU_FPGA_DC_MPHY 0
#define WPU_FPGA_DC_SCLAV 1
#define WPU_FPGA_DC_MCLAV 0
#define WPU_FPGA_DC_LEVEL2   0
#define WPU_FPGA_DC_LEVEL3   1
#define WPU_FPGA_DC_BUS8     0
#define WPU_FPGA_DC_BUS16    1
#define WPU_FPGA_DC_BUS32    2
#define WPU_FPGA_DC_MASTER   1
#define WPU_FPGA_DC_SLAVE    0
#define WPU_FPGA_DC_UTOPIA      1
#define WPU_FPGA_DC_POS      0
#define WPU_FPGA_DC_PARITY_EVEN 1
#define WPU_FPGA_DC_PARITY_ODD  0
#define WPU_FPGA_DC_CORE_LB 1
#define WPU_FPGA_DC_NO_CORE_LB 0
#define WPU_FPGA_DC_RX_CLAV_TH    5
#define WPU_FPGA_DC_TX_CLAV_TH    16
#define WPU_UTOPIA_CELL_SIZE 52
#define WPU_FPGA_DC_INV_MODE      0
#define WPU_FPGA_DC_PAUSE_SUPP    1
#define WPU_FPGA_DC_NO_PAUSE_SUPP    0
#define WPU_FPGA_DC_ENABLE 0

#define WPU_FPGA_DC_SPHY_MPHY_OFFSET 0 
#define WPU_FPGA_DC_SCLAV_MCLAV_OFFSET 1
#define WPU_FPGA_DC_LEVEL_OFFSET 2 
#define WPU_FPGA_DC_BUS_WIDTH_OFFSET 3 
#define WPU_FPGA_DC_ATM_POS_OFFSET 5 
#define WPU_FPGA_DC_MASTER_SLAVE_OFFSET 6 
#define WPU_FPGA_DC_PAUSE_SUPPORT_OFFSET 7
#define WPU_FPGA_DC_PARITY_OFFSET 8
#define WPU_FPGA_DC_UNV_MODE_OFFSET 9

#define WPU_FPGA_DC_MODE_LOOPBACK 0x1
#define WPU_FPGA_DC_MODE_LOOPBACK_OFFSET 0x1

typedef struct
{
      WP_U16 rev_id;         /*0x0 */
      WP_U16 param;          /*0x2 */
      WP_U16 led;            /*0x4 */
      WP_U16 mac_r_data_low; /*0x6 */
      WP_U16 mac_r_data_high;/*0x8 */
      WP_U16 loopback;       /*0xa */
      WP_U16 rx_du;          /*0xc */
      WP_U16 tx_du;          /*0xe */
      WP_U16 rx_clav_th;     /*0x10 */
      WP_U16 tx_clav_th;     /*0x12 */
      WP_U16 reserved[5];    /*0x14 */
      WP_U16 master_reset;   /*0x1e */ 
      WP_U16 flow_control;   /*0x20 */
}wpu_fpga_dc;

   

#endif /*WPX_FPGA_DC_H*/
