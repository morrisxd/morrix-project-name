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


#ifndef WPI_WDDI_H
#include "include/core/registry/wpi_wddi.h"
#endif

#ifndef WPI_TYPES_H
#include "include/core/registry/wpi_types.h"
#endif

#ifndef WPI_PTP_H
#include "include/iw/ptp/wpi_ptp.h"
#endif

#ifndef WPX_PTP_H
#include "wpx_ptp.h"
#endif

#include "veneer/wpi_board_data_int.h"




WPTP_status WPTP_SystemInit(WPTP_ptp_system *ptp_system_config){return 0;}
WPTP_status WPTP_ClockDomainInit(WP_handle cd_handle, WPTP_clock_domain_config *cd_config ){return 0;}
WPTP_status WPTP_ClockDomainEnable(WP_handle cd_handle){return 0;}
WPTP_status WPTP_ClockDomainDisable(WP_handle cd_handle){return 0;}
WPTP_status WPTP_TSRead(WP_handle cd_handle,
                        WPTP_REG *sec_msb,
                        WPTP_REG *sec_mid,
                        WPTP_REG *sec_lsb,
                        WPTP_REG *nsec_msb,
                        WPTP_REG *nsec_lsb){return 0;}
WPTP_status WPTP_SetOffsetAbsolute(WP_handle cd_handle,
                                   WPTP_REG sec_msb,
                                   WPTP_REG sec_mid,
                                   WPTP_REG sec_lsb,
                                   WPTP_REG nsec_msb,
                                   WPTP_REG nsec_lsb){return 0;}

WPTP_status WPTP_SetOffsetAbsoluteONPPS(WP_handle cd_handle,
                                        WPTP_REG sec_msb,
                                        WPTP_REG sec_mid,
                                        WPTP_REG sec_lsb,
                                        WPTP_REG nsec_msb,
                                        WPTP_REG nsec_lsb){return 0;}


WPTP_status WPTP_SetOffsetAddative(WP_handle cd_handle,
                                   WP_U8    add_or_sub,
                                   WPTP_REG sec_msb,
                                   WPTP_REG sec_mid,
                                   WPTP_REG sec_lsb,
                                   WPTP_REG nsec_msb,
                                   WPTP_REG nsec_lsb){return 0;}

WPTP_status WPTP_DCODividerSet(WP_handle cd_handle,
                               WPTP_REG dco_int,
                               WPTP_REG dco_remainder_msb,
                               WPTP_REG dco_remainder_lsb){return 0;}



WPTP_status WPTP_SystemDisplay(void){return 0;}
WPTP_status WPTP_ClockDomainDisplay(WP_handle cd_handle){return 0;}
WPTP_status WPTP_SoftReset(void){return 0;}
WPTP_status WPTP_CheckPllLock(void){return 0;}


 /* advanced user access to registers */
void WPTP_REG_WRITE(WPTP_REG reg, WP_U8 cd_or_gen,WPTP_REG value){}
void WPTP_REG_READ(WPTP_REG reg, WP_U8 cd_or_gen,WPTP_REG *value_ptr){}

