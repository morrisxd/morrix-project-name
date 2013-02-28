/*****************************************************************************
 * (C) Copyright Wintegra 2000-2009.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************
/****************************************************************************
*
* File: trs_propagated_params.h
*
* Description:
*
* Module Originator: Lior Shetrit
* Creation Date:     July 11, 2009
*
* Change History:
* +------------+--------------------+----------------------------------------
* | Date       | By                 | Description
* +------------+--------------------+----------------------------------------
* |            |                    |
* +------------+--------------------+----------------------------------------
* |            |                    |
* +------------+--------------------+----------------------------------------
*****************************************************************************/

#ifndef _TRS_PROPAGATED_PARAMS_H_
#define _TRS_PROPAGATED_PARAMS_H_

#include "shared.h"
#include "interworking.h"

/* This file defines the generic macro used to read/write
 * the Propagated Parameters table from/to the external memory.
 * Also, the Propagated Parameters table is defined here. */

/* Used as an input for M_TrsPropagatedParams
 * to specify whether the macro should read
 * or write the propagated parameters. */
enum TRS_PROP_PARAMS_RW_MODE
{
   TRS_PROP_PARAMS_READ,
   TRS_PROP_PARAMS_WRITE
};

/* Used as an input for M_TrsPropagatedParams
 * to specify whether the macro should backup
 * the tar register provided to it. */
enum TAR_BACKUP_MODE
{
   TAR_BACKUP_MODE_ENABLE,
   TAR_BACKUP_MODE_DISABLE
};

/* TRS Propagated Parameters table.
 * Currently, only contains the IW Propagated Parameters table. */

struct S_TrsPropagatedParams
{
      struct S_IwPropagatedParams IwPropagatedParams;
};

/* This macro should not be compiled for the DPS test util. */
#ifndef DPS_LEVEL_0_TESTING_UTIL

/*****************************************************************************
 *
 * Macro name:    M_TrsPropagatedParams
 *
 * Description:
 *    Read data from the propagate parameters table in the external memory.
 *    A propagated parameters table exists for each TRS and is thus
 *    valid only for a specific thread.
 *    The offset for each table is the index of the TRS.
 *    The TRS index resides in r0 of the TRS (not the r0 of the hardware) and
 *    is retrieved using a tar TRS pointer.
 *
 * Inputs:
 *
 *    TAR_BACKUP_MODE
 *       specifies whether TrsIndexTarPtr is needed to be backed up using TarRegBakTempReg.
 *       TAR_BACKUP_MODE = TAR_BACKUP_MODE_ENABLE => TrsIndexTarPtr is backed up.
 *       TAR_BACKUP_MODE = TAR_BACKUP_MODE_DISABLE => TrsIndexTarPtr isn't backed up.
 *
 *
 * Outputs:
 *
 *    TrsPropParams
 *       the propagated parameters table in the TRS.
 *
 *    TrsIndexTarPtr
 *       tar register.
 *
 *    TarRegBakTempReg
 *       backup register for TrsIndexTarPtr.
 *       Only used when TAR_BACKUP_MODE = TAR_BACKUP_MODE_ENABLE.
 *
 *    TempReg
 *       temporary register that is used in this macro.
 *
 ******************************************************************************/
inline func M_TrsPropagatedParams(in                                 TRS_PROP_PARAMS_RW_MODE,
                                  out struct S_TrsPropagatedParams   TrsPropParams,
                                  out                                TrsIndexTarPtr,
                                  out                                TarRegBakTempReg,
                                  out                                TempReg,
                                  in                                 TAR_BACKUP_MODE)
{
   register TrsPropParamsTableOffset = TempReg;

   // Backup the tar register if necessary
   if (TAR_BACKUP_MODE == TAR_BACKUP_MODE_ENABLE)
   {
      TarRegBakTempReg = TrsIndexTarPtr;
   }

   // Set the tar register to point to the register that holds the TRS index.
   TrsIndexTarPtr = &TRS_INDEX_REG;
   // Create offset to the propagated parameters table (*TrsIndexTarPtr = TRS index)
   TrsPropParamsTableOffset = *TrsIndexTarPtr * sizeof(S_TrsPropagatedParams);

   if (TRS_PROP_PARAMS_RW_MODE == TRS_PROP_PARAMS_READ)
   {
      // Read the propagated parameters from the external memory
      TrsPropParams = _mem(r0, B_TRS_PROPAGATED_PARAMS[TrsPropParamsTableOffset], sizeof(S_TrsPropagatedParams), TRS_PROP_PARAMS_BUS);
   }
   else /* TRS_PROP_PARAMS_RW_MODE == TRS_PROP_PARAMS_WRITE */
   {
      // Save the propagated parameters to the external memory
      _mem(r0, B_TRS_PROPAGATED_PARAMS[TrsPropParamsTableOffset], sizeof(S_TrsPropagatedParams), TRS_PROP_PARAMS_BUS) = TrsPropParams;
   }

   // Restore the tar register if necessary
   if (TAR_BACKUP_MODE == TAR_BACKUP_MODE_ENABLE)
   {
      TrsIndexTarPtr = TarRegBakTempReg;
   }
}

#endif // DPS_LEVEL_0_TESTING_UTIL

#endif // _TRS_PROPAGATED_PARAMS_H_

/*****************************************************************************
 * (C) Copyright Wintegra 2000 - 2002, All rights reserved.
 *****************************************************************************/
