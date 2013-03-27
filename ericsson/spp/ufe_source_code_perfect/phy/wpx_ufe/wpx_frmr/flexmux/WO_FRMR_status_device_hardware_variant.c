/*--------------------------------------------------------------------------*/
/*                                                                          */
/*        Copyright (c) 2010  Omiino Ltd                                    */
/*                                                                          */
/*        All rights reserved.                                              */
/*        This code is provided under license and or Non-disclosure         */
/*        Agreement and must be used solely for the purpose for which it    */
/*        was provided. It must not be passed to any third party without    */
/*        the written permission of Omiino Ltd.                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/



#include "WO_FRMR_private.h"


void OMIINO_FRAMER_STATUS_DeviceDriver_GetHardwareVariant(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, U8 * pHardwareVariant)
{
    *pHardwareVariant=pStatus->HardwareVariant;
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareVariant(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, U8 HardwareVariant)
{
    pStatus->HardwareVariant=HardwareVariant;
}
