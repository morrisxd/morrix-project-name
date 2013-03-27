/*****************************************************************************
 * (C) Copyright 2002-2006, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*                      TDM CARD INITIALIZATION                         */
/************************************************************************/

/* This initialization code assumes that the host port for the
   QuadFALC/OctalFALC is implemented as a 64-bit port. */

#include <stdio.h>

#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WP_PROTOTYPES_H
#include "api/wp_prototypes.h"
#endif
#ifndef WPI_REG_H
#include "include/core/hardware/wpi_reg.h"
#endif
#ifndef WPI_REGISTRY_H
#include "include/core/registry/wpi_registry.h"
#endif
#ifndef WPI_HANDLE_H
#include "include/core/registry/wpi_handle.h"     /* For WPI_ERROR */
#endif
#include "veneer/wpi_board_data_int.h"
#include "veneer/wpi_veneer.h"
/* The following file picks up where the PHYs are in the memory map.
 * It also picks up whether it is a quad or octal card. */
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif
#ifndef WPX_TDM_QFALC_H
#include "wpx_tdm_qfalc.h"
#endif
#include "veneer/wpi_veneer.h"
WP_status WPUI_TdmQfalcCardInit(wpu_qfalc *xcvr,
                                WP_U32 q_port,
                                WP_U32 mode);

/*****************************************************************************
 *
 * Function:   WPU_TdmQfalcCardInit
 *
 * Purpose:    Initialize (or partially initialize) T1/E1 card QFALC
 *
 *
 * Description:
 *
 *     Given a TDI port, do one of the following depending on the mode.
 *
 *     (A) Initialize the QFALC clock (applies to all 4/8 ports on the card.
 *     (B) Initialize a single port according to the mode.
 *
 *
 * Inputs:
 *     WP_U32 port      (WP_PORT_TDM1 ... WP_PORT_TDM8)
 *     WP_U32 mode      (group mode)
 *                      WPU_MODE_TDM_CLOCK_8MHZ
 *                      WPU_MODE_TDM_CLOCK_16MHZ
 *
 *                      (individual port mode)
 *                      WPU_MODE_TDM_E1_A
 *                      WPU_MODE_TDM_E1_B
 *                      WPU_MODE_TDM_E1_S
 *                      WPU_MODE_TDM_E1_CAS
 *                      WPU_MODE_TDM_T1
 *                      WPU_MODE_TDM_T1_FRAMELESS
 *                      WPU_MODE_TDM_T1_S
 *                      WPU_MODE_TDM_T1_CAS
 * Outputs:
 *     The state of the QFALC is changed.
 *
 * Return Value:
 *     WP_OK            if successful
 *     WP_ERR_PORT      if port is invalid
 *     WP_ERR_CONFIG    if mode is invalid
 *
 * Called by:
 *     User application
 *
 ****************************************************************************/
WP_status WPU_TdmQfalcCardInit(WP_U32 port, WP_U32 mode)
{
   wpu_qfalc *xcvr;
   WP_U32 q_port;
   wpi_match_mask save;
   WP_status status;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();

   /* Get the register base for the QFALC connected to the port. */
   switch (port)
   {
      case WP_PORT_TDM1:
      case WP_PORT_TDM2:
      case WP_PORT_TDM3:
      case WP_PORT_TDM4:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_TDM4_QFALC);
         q_port = port - WP_PORT_TDM1;
         break;
      }
      case WP_PORT_TDM5:
      case WP_PORT_TDM6:
      case WP_PORT_TDM7:
      case WP_PORT_TDM8:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_TDM8_QFALC);
         q_port = port - WP_PORT_TDM5;
         break;
      }
      case WP_PORT_TDM9:
      case WP_PORT_TDM10:
      case WP_PORT_TDM11:
      case WP_PORT_TDM12:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_RTDM4_QFALC);
         q_port = port - WP_PORT_TDM9;
         break;
      }
      case WP_PORT_TDM13:
      case WP_PORT_TDM14:
      case WP_PORT_TDM15:
      case WP_PORT_TDM16:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[1]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_RTDM8_QFALC);
         q_port = port - WP_PORT_TDM13;
         break;
      }
      case WP_UFE_UPI1_TDM1:
      case WP_UFE_UPI1_TDM2:
      case WP_UFE_UPI1_TDM3:
      case WP_UFE_UPI1_TDM4:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->ufe[0]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_UFE_UPI1_TDM4_QFALC);
         q_port = port - WP_UFE_UPI1_TDM1;
         break;
      }
      case WP_UFE_UPI1_TDM5:
      case WP_UFE_UPI1_TDM6:
      case WP_UFE_UPI1_TDM7:
      case WP_UFE_UPI1_TDM8:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->ufe[0]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_UFE_UPI1_TDM8_QFALC);
         q_port = port - WP_UFE_UPI1_TDM5;
         break;
      }
      case WP_UFE_UPI2_TDM1:
      case WP_UFE_UPI2_TDM2:
      case WP_UFE_UPI2_TDM3:
      case WP_UFE_UPI2_TDM4:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->ufe[1]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_UFE_UPI2_TDM4_QFALC);
         q_port = port - WP_UFE_UPI2_TDM1;
         break;
      }
      case WP_UFE_UPI2_TDM5:
      case WP_UFE_UPI2_TDM6:
      case WP_UFE_UPI2_TDM7:
      case WP_UFE_UPI2_TDM8:
      {
         WPX_SetBank(&save, &wpi_board_data.match_mask->ufe[1]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(0, APP_UFE_UPI2_TDM8_QFALC);
         q_port = port - WP_UFE_UPI2_TDM5;
         break;
      }

      default:
         status = WPI_ERROR(WP_ERR_PORT);
         goto clean_return;
   }

   status = WPUI_TdmQfalcCardInit(xcvr, q_port, mode);

  clean_return:

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}

WP_status WPUI_TdmQfalcCardInit(wpu_qfalc *xcvr,
                                WP_U32 q_port,
                                WP_U32 mode)
{
   switch (mode)
   {
      case WPU_MODE_TDM_CLOCK_8MHZ:
      case WPU_MODE_TDM_CLOCK_16MHZ:
      {
         /* This programs the QFALC for an 8 or 16 MHz
          * oscillator.  It affects all ports on the
          * QFALC and should be set before setting
          * any individual port.  Ignore the argument
          * in favor of setting according to what the
          * board data says. */

         WPI_MEM_BYTE_SET(xcvr->phy[0].gcm1, 0x0);
         WPI_MEM_BYTE_SET(xcvr->phy[0].gcm2, 0x58);
         WPI_MEM_BYTE_SET(xcvr->phy[0].gcm3, 0xd2);
         WPI_MEM_BYTE_SET(xcvr->phy[0].gcm4, 0xc2);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[0].gcm5, 0x07);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[0].gcm5, 0x03);
         WPI_MEM_BYTE_SET(xcvr->phy[0].gcm6, 0x10);
         break;
      }

      case WPU_MODE_TDM_E1:
      {
         /* In the original code, this is the "case 0"
          * for WPU_MODE_TDM_E1.*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x81);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x1c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
           WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
           WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_E1_S:
      {
         /* This is used for IMA with E1.
          * Owners: rogers, jaime */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x81);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x1c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x07);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_E1_X:
      {
         /* Common clock
          * Owners: rogers, jaime */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x04);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x81);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x1c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x07);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_E1_MASTER:
      {
         /* Common clock
          * Owners: rogers, jaime */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x81);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x03);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x1c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);

         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x07);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x10); /* unused */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_E1_SLAVE:
      {
         /* Common clock
          * Owners: rogers, jaime */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00); /* !mas */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x04); /* elt */

         /* Use respective port DPLL output clk for DCO-R */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, q_port << 6);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x81);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x03);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x1c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0xdf); /* xtm */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x07); /* unused */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x10); /* unused */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_T1:
      {
         /* These aer the original T1 modes.*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_T1_S:
      {
         /* Used for T1 IMA.
          * owners rogers, jaime. */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x07);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_T1_MASTER:
      {
         /* Used for T1 IMA.
          * owners rogers, jaime. */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);

         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x07);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_T1_SLAVE:
      {
         /* Used for T1 IMA.
          * owners rogers, jaime. */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00); /* !mas */

         /* Use respective port DPLL output clk for DCO-R */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, q_port << 6);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x04); /* fmr5.xtm = 1 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x25); /* elt */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x07);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      /* The following code initialize E1 and T1 falcs for
       * transparent protocol. Originator: ziva */
      case WPU_MODE_TDM_TRANS_E1_MASTER:
      case WPU_MODE_TDM_TRANS_E1_SLAVE:
      case WPU_MODE_TDM_TRANS_E1_MASTER_4M:
      case WPU_MODE_TDM_TRANS_E1_MASTER_8M:

      {
         if (mode == WPU_MODE_TDM_TRANS_E1_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         if (mode == WPU_MODE_TDM_TRANS_E1_MASTER_4M)
         {

            WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4E);
         }
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x81);
         if (mode == WPU_MODE_TDM_TRANS_E1_MASTER_4M)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x0A);
         }
         else
            if (mode == WPU_MODE_TDM_TRANS_E1_MASTER_8M)
            {
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x82);
            }
            else
            {
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
            }

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x1c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_TRANS_E1_FRAMELESS_MASTER:
      case WPU_MODE_TDM_TRANS_E1_FRAMELESS_SLAVE:
      {
         if (mode == WPU_MODE_TDM_TRANS_E1_FRAMELESS_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x20);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_TRANS_E1_CAS_MASTER:
      case WPU_MODE_TDM_TRANS_E1_CAS_SLAVE:
      {
         if (mode == WPU_MODE_TDM_TRANS_E1_CAS_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x91);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].ttr1, 0xff);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].ttr2, 0xff);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].ttr3, 0xff);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].ttr4, 0xff);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x5c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x02);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x02);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4,  0x74);
         }
         else
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x02);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x51);
         break;
      }

      case WPU_MODE_TDM_TRANS_T1_MASTER:
      case WPU_MODE_TDM_TRANS_T1_SLAVE:
      {
         if (mode == WPU_MODE_TDM_TRANS_T1_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x04); /* fmr5 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_TRANS_T1_FRAMELESS_MASTER:
      case WPU_MODE_TDM_TRANS_T1_FRAMELESS_SLAVE:
      {
         if (mode == WPU_MODE_TDM_TRANS_T1_FRAMELESS_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x40); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x04);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }

      case WPU_MODE_TDM_TRANS_T1_CAS_MASTER:
      case WPU_MODE_TDM_TRANS_T1_CAS_SLAVE:
      {
         if (mode == WPU_MODE_TDM_TRANS_T1_CAS_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x61);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x16);
         }
         else
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e);
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x10);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x72);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4, 0x74);
         }
         else
         {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x02);
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);

         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x51);
         break;
      }

      /**********************************/
      /* OCTAL over UFE2 configurations */
      /**********************************/

      case WPU_MODE_TDM_UFE_E1_UNFRAMED:
      case WPU_MODE_TDM_UFE_E1_FRAMED:
      {
         WP_U8 fmr2, xsp, xsw , cmr2, xc1, rc1;
         if( mode == WPU_MODE_TDM_UFE_E1_FRAMED)
         {
            fmr2 = 0x81;
            xsp  = 0x1c;
            xsw  = 0x9f;
            cmr2 = 0x05;
            xc1  = 0x06;
            rc1  = 0x02;

         }
         else
         {
            fmr2 = 0x30;
            xsp  = 0x20;
            xsw  = 0x1f;
            cmr2 = 0x0f;
            xc1  = 0x03;
            rc1  = 0x03;

         }
         /* Owners: eladk , shikva */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x04);

         /* Use respective port DPLL output clk for DCO-R */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, q_port << 6);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, cmr2);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, fmr2); /*unfrm-0x30   frm-0x81*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  xsp); /*unfrm-0x20 frm-0x1c*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  xsw); /*unfrm-0x1f frm-0x9f*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  xc1);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  rc1);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);

         break;
      }/* WPU_MODE_TDM_UFE_E1_UNFRAMED and  WPU_MODE_TDM_UFE_E1_FRAMED*/

      case WPU_MODE_TDM_UFE_E1_UNFRAMED_EXT_CLK:
      case WPU_MODE_TDM_UFE_E1_FRAMED_EXT_CLK:
      {
         WP_U8 fmr2, xsp, xsw , cmr2, xc1, rc1;
         if( mode == WPU_MODE_TDM_UFE_E1_FRAMED_EXT_CLK)
         {/* FRAMED */
            fmr2 = 0x81;
            xsp  = 0x1c;
            xsw  = 0x9f;
            cmr2 = 0x04;
            xc1  = 0x05;
            rc1  = 0x02;
         }
         else
         { /* UNFRAMED */
            fmr2 = 0x30;
            xsp  = 0x20;
            xsw  = 0x1f;
            cmr2 = 0x04;
            xc1  = 0x03;
            rc1  = 0x03;

         }
         /* Owners: eladk , shikva */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x00);

         /* Use respective port DPLL output clk for DCO-R */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, (q_port << 6) |0x18);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, cmr2);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, fmr2); /*unfrm-0x30   frm-0x81*/
         /* WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02); */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x00); /* bypass the transmitter jitter buffer*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x08);
         /*Synchronous Pulse Transmit with the first raising edge  */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  xsp); /*unfrm-0x20 frm-0x1c*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  xsw); /*unfrm-0x1f frm-0x9f*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  xc1);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  rc1);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10); /*     SYPX: */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x47); /* MSB just not 0 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x12);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4,  0x15); /* MSB just not 0 */

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);
         break;
      }/* WPU_MODE_TDM_UFE_E1_UNFRAMED_EXT_CLK and  WPU_MODE_TDM_UFE_E1_FRAMED_EXT_CLK */

      case WPU_MODE_TDM_UFE_T1_UNFRAMED:
      {
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x40); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x04);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x21); /* eladk - Multiframe Sync*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x0a); /* eladk - Multiframe Sync*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);

         break;
      }/* WPU_MODE_TDM_UFE_T1_UNFRAMED */

      case WPU_MODE_TDM_UFE_T1_UNFRAMED_EXT_CLK:
      {
         if (mode == WPU_MODE_TDM_TRANS_T1_FRAMELESS_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                ( 0x00 |
                                  0x08 | /* for disabling connection with rx clock*/
                                  0x10 ));
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                ( 0x40 |
                                  0x08 | /* for disabling connection with rx clock*/
                                  0x10 ));
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                ( 0x80 |
                                  0x08 | /* for disabling connection with rx clock*/
                                  0x10 ));
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                ( 0xc0 |
                                  0x08 | /* for disabling connection with rx clock*/
                                  0x10 ));
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x04);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x40); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x00); /* bypass the tx elastic buffer */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x08); /*Synchronous Pulse Transmit with the
                                                           first raising edge  */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x10); /* eladk - Multiframe Sync*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x44);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x12);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4, 0x15); /* eladk - Multiframe Sync */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02); /* eladk - Multiframe Sync */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);

         break;
      }/* WPU_MODE_TDM_UFE_T1_UNFRAMED */

      case WPU_MODE_TDM_UFE_T1_FRAMED:
      {
         if (mode == WPU_MODE_TDM_TRANS_T1_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x05);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x04); /* fmr5 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0d);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x09);
         if(APP_TDM_QFALC_TYPE == QFALC_OCTAL)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x10);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);

         break;
      }

#if 0
 {
    WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

    switch (q_port)
    {
       case 0:
          WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                           (0x00 |
                            0x08 | /* for disabling connection with rx clock*/
                            0x10 ));
          break;
       case 1:
          WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                           (0x40 |
                            0x08 | /* for disabling connection with rx clock*/
                            0x10 ));
          break;
       case 2:
          WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                           (0x80 |
                                 0x08 | /* for disabling connection with rx clock*/
                                 0x10 ));
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                (0xc0 |
                                 0x08 | /* for disabling connection with rx clock*/
                                 0x10 ));
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x04);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         /*         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x04); (i.e. fmr5  XTM =1)*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x00); /* fmr5 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x08);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0c); /* used to be 0xa */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x09);  /* used to be 0xa */

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr,0x50);

         break;
      }
/*WPU_MODE_TDM_UFE_T1_FRAMED */
#endif

      case WPU_MODE_TDM_UFE_T1_FRAMED_EXT_TX_CLK:
      {
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                (0x00 |
                                 0x08 | /* for disabling connection with rx clock*/
                                 0x10 ));
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                (0x40 |
                                 0x08 | /* for disabling connection with rx clock*/
                                 0x10 ));
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                (0x80 |
                                 0x08 | /* for disabling connection with rx clock*/
                                 0x10 ));
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1,
                                (0xc0 |
                                 0x08 | /* for disabling connection with rx clock*/
                                 0x10 ));
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x04);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* fmr4 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x00); /* fmr5 XTM =0 */
         /* WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);  the transmit/receive elastic
            store 2 frame */
         /*         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x32);
                    the transmit elastic store 2 frame; bypass receive buffer */

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x00 ); /* bypass the transmit elastic store */

         /*         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x30 );
                    bypass the tx and rx
                    elastic store */

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x08);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x09);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x47);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x12);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4, 0x15); /* MSB just not 0 */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x50);

         break;
      }/* WPU_MODE_TDM_UFE_T1_EXT_TX_CLK */

      /**********************************/
      /* OCTAL over UFE2 configurations */
      /**********************************/
      /*          CAS modes             */
      /**********************************/

      case WPU_MODE_TDM_UFE_TRANS_T1_CAS_MASTER:
      case WPU_MODE_TDM_UFE_TRANS_T1_CAS_SLAVE:
      {
         if (mode == WPU_MODE_TDM_TRANS_T1_CAS_MASTER)
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x01);
         else
            WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);

         switch (q_port)
         {
            case 0:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x00);
               break;
            case 1:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x40);
               break;
            case 2:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0x80);
               break;
            case 3:
               WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, 0xc0);
               break;
            default:
               break;
         }
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x18);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x60); /* might be 0x61 (tdi cas)*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x0e); /* (fmr4) might be 0x16 (tdi cas) */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic2, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim1, 0x30);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x21);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcd,  0x0a);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pcr,  0x15);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0xd7);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm1, 0x22);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x01);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x40); /* might be 0x04 (ufe)*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1, 0x0a); /* change 0x0b to 0x0a (TDI configs) */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1, 0x0a); /* change 0x0b to 0x0a (TDI configs) */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1, 0x10);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3, 0x72);  /* might be ... */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4, 0x74);  /* might be ... */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2, 0x40);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x51);
         break;
      }
      case WPU_MODE_TDM_UFE_E1_SLAVE_CAS:
      {
         /* Owners: eladk , shikva */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim0, 0x00);
/*         WPI_MEM_BYTE_SET(xcvr->phy[q_port].lim2, 0x04);*/

         /* Use respective port DPLL output clk for DCO-R */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr1, q_port << 6);

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmr2, 0x0f);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr0, 0xf0);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr1, 0x4c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].fmr2, 0x91); /*unfrm-0x30   frm-0x81 cas 0x91*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic1, 0x02);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].sic3, 0x00); /*??? not in UDIK */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm0, 0x9c);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xpm2, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].loop, 0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsp,  0x5c); /*unfrm-0x20 frm-0x1c cas-0x5c*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xsw,  0x9f); /*unfrm-0x1f frm-0x9f*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc0,  0x00);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].xc1,  0x03);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc0,  0x80);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].rc1,  0x03);
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc1,  0x21); /*diff*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc2,  0x40); /*diff*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc5,  0x0a); /* tamirs - For Multiframe Sync:0x0a*/
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc3,  0x02); /*cas */
         WPI_MEM_BYTE_SET(xcvr->phy[q_port].pc4,  0x74); /*cas */

         WPI_MEM_BYTE_SET(xcvr->phy[q_port].cmdr, 0x51);/*??? not in Eyalh*/
         break;
      } /* WPU_MODE_TDM_UFE_E1_SLAVE_CAS */

      default:
         return WPI_ERROR(WP_ERR_CONFIG);
   }

   return WP_OK;
}

WP_status WPU_TdmQfalcCardInitBim(WP_U32 port, WP_U32 mode)
{
   wpu_qfalc *xcvr;
   WP_U32 q_port;
   wpi_match_mask save;
   WP_status status;

   /* Restrict access to match/mask regs. */
   WPI_WDDI_ATTACH();


   /* Get the register base for the QFALC connected to the port. */
   switch (port)
   {
      case WP_PORT_TDM1:
      case WP_PORT_TDM2:
      case WP_PORT_TDM3:
      case WP_PORT_TDM4:
      {
         /* Save current match/mask, install OC-12 values. */
         WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(1, APP_TDM8_QFALC_BIM);
         q_port = port - WP_PORT_TDM1;
         break;
      }
      case WP_PORT_TDM5:
      case WP_PORT_TDM6:
      case WP_PORT_TDM7:
      case WP_PORT_TDM8:
      {
         /* Save current match/mask, install OC-12 values. */
         WPX_SetBank(&save, &wpi_board_data.match_mask->tdm[0]);
         xcvr = (wpu_qfalc *) WPL_PHY_VIRTUAL(1, APP_TDM4_QFALC_BIM);
         q_port = port - WP_PORT_TDM5;
         break;
      }
      default:
         status = WPI_ERROR(WP_ERR_PORT);
         goto clean_return;
   }

   status = WPUI_TdmQfalcCardInit(xcvr, q_port, mode);

  clean_return:

   /* Restore the registers. */
   WPX_RestoreBank(&save);

   /* Relinquish the lock (on the match/mask registers). */
   WPI_WDDI_DETACH();

   return status;
}
