/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_clk_translator.h
 *
 * Purpose: the UFE clock translator internal structures and prototypes.
 *
 ****************************************************************************/
#ifndef WUFEI_CLK_TRANSLATOR_H
#define WUFEI_CLK_TRANSLATOR_H

#ifndef WUFEI_REGISTRY_INT_H
#include "wufei_registry_int.h"
#endif

/*************************************************
 **************** SBI CLOCK TRANSLATOR ***********
 ************************************************/

#define WUFEI_SBI_CT_RAM_SIZE_PER_CORE         256
#define WUFE_CORE_MASK     0x0100
#define WUFE_STM_MASK      0x0080
#define WUFE_SPE_MASK      0x0060
#define WUFE_SUBSPE_MASK   0x001f
#define WUFE_CORE_OFFSET   168
#define WUFE_STM_OFFSET    84
#define WUFE_SPE_OFFSET    28


/* The SBI clock translator tributary type must not be changed ! */

enum {
   WUFEI_SBI_CT_TRIB_E1 = 0,
   WUFEI_SBI_CT_TRIB_T1,
   WUFEI_SBI_CT_TRIB_TYPE_INVALID
};

WUFE_status WUFEI_SbiClockTranslatorSetup(WUFEI_system_registry* system_registry);

WUFE_status WUFEI_SbiClockTranslatorDisplay(WUFEI_system_registry *system_registry);

WUFE_status WUFEI_SbiClockTranslatorDelete(WUFEI_system_registry* system_registry);

WUFE_status WUFEI_SbiClockTranslatorValidate(WUFEI_system_registry* system_registry,
                                             WP_U16 clock_translator_id,
                                             WUFE_un_framed line_framed_mode,
                                             WUFE_line_transf_type line_tributary_type);

WUFE_status WUFEI_ClockTranslatorInit(WUFEI_system_registry *system_registry,
                                      WP_U32 line_id,
                                      WP_U32 ct_id,
                                      WP_U16 tributary_id,
                                      WP_U16 framed_mode,
                                      WUFE_line_transf_type transfer_type);

WUFE_status WUFEI_SbiClockTranslatorRelease(WUFEI_system_registry *system_registry,
                                            WP_U32 line_id,
                                            WP_U16 ct_id);

#endif /*WUFEI_CLK_TRANSLATOR_H*/
