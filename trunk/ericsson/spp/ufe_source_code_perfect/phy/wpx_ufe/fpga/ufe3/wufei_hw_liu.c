/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/
#include <stdio.h>
#include <string.h>

#ifndef WUFE_TYPES_H
#include "wufe_types.h"
#endif

#ifndef WUFE_UFE_H
#include "wufe_ufe.h"
#endif

#ifndef WUFEI_HW_CAD_H
#include "wufei_hw_tdm.h"
#endif

#ifndef WUFEI_HW_CONFIG_H
#include "wufei_hw_config.h"
#endif

#ifndef WUFEI_HW_MEMORY_H
#include "wufei_hw_memory.h"
#endif

#ifndef WUFE_ERRORS_H
#include "wufe_errors.h"
#endif

extern WP_U8 debug_flag;

/*****************************************************************************
 *
 * Function:  WUFEI_LiuSocketEnable
 *
 * Purpose: sets the TDM TX/RX block assert requests to FM
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *     coreid - the CORE id
 *     direction - specifies the RX/TX direction
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *        WUFE_ERR_INVALID_DIRECTION
 * Called by:
 *     WUFEI_SystemCoreEnable
 *
 ****************************************************************************/
WUFE_status WUFEI_LiuSocketEnable(WP_U32 ufeid, WP_U32 coreid, WP_U32 direction)
{
   WP_U32 id = WUFEI_ID(ufeid, coreid);
   WP_U16 value=0;
   
   WUFEI_MEM_16_GET(id, WUFEI_TDM_MODE, &value);
   
   if(direction == WUFE_FULL_DUPLEX)
      value |= (WUFEI_TDM_MODE_TX_EN_M | WUFEI_TDM_MODE_RX_EN_M);
   else if(direction == WUFE_RX_DIRECTION)
      value |= WUFEI_TDM_MODE_RX_EN_M;
   else if(direction == WUFE_TX_DIRECTION)
      value |= WUFEI_TDM_MODE_RX_EN_M;
   else
      return WUFE_ERR_INVALID_DIRECTION;
   
   WUFEI_MEM_16_SET(id, WUFEI_TDM_MODE, value);
   
   return WUFE_OK;
}

/*****************************************************************************
 *
 * Function:  WUFEI_LiuSocketDisable
 *
 * Purpose: sets the TDM TX/RX block to disable requests to FM
 *
 * Description:
 *
 *
 * Inputs:
 *     ufeid - the UFE id
 *     coreid - the CORE id
 *     direction - specifies the RX/TX direction
 *
 * Outputs:
 *
 *
 * Return Value:
 *     Upon successful completion, a WUFE_OK  is returned.
 *     Otherwise the following error codes may be returned:
 *             WUFE_ERR_INVALID_DIRECTION
 * Called by:
 *     WUFEI_SystemCoreDisable
 *
 ****************************************************************************/
WUFE_status WUFEI_LiuSocketDisable(WP_U32 ufeid, WP_U32 coreid, WP_U32 direction)
{
   WP_U32 id = WUFEI_ID(ufeid, coreid);
   WP_U16 value=0;

   WUFEI_MEM_16_GET(id, WUFEI_TDM_MODE, &value);

   if(direction == WUFE_FULL_DUPLEX)
      value &= ~(WUFEI_TDM_MODE_TX_EN_M | WUFEI_TDM_MODE_RX_EN_M);
   else if(direction == WUFE_RX_DIRECTION)
      value &= ~WUFEI_TDM_MODE_RX_EN_M;
   else if(direction == WUFE_TX_DIRECTION)
      value &= ~WUFEI_TDM_MODE_RX_EN_M;
   else
      return WUFE_ERR_INVALID_DIRECTION;

   WUFEI_MEM_16_SET(id, WUFEI_TDM_MODE, value);

   return WUFE_OK;

}
