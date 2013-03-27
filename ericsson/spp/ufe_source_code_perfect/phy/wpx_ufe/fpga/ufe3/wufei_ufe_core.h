/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_ufe_core.h
 *
 * Purpose: the UFE core and general registers defines
 *
 ****************************************************************************/
#ifndef WUFEI_UFE_CORE_H
#define WUFEI_UFE_CORE_H

/*used by WUFEI_UfeCoreClockMonitorPllLock()*/
#define WUFEI_UFE_PLL_LOCK_TIMEOUT 0x1000000
#define WUFEI_UFE_PLL_LOCK_DELAY 100

enum
{
   WUFEI_UFE_LIU_MODE_SBI=1,
   WUFEI_UFE_LIU_MODE_CAD,
   WUFEI_UFE_LIU_MODE_ALIATHON,
   WUFEI_UFE_LIU_MODE_NA
}



#endif /*WUFEI_UFE_CORE_H*/
