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


void OMIINO_FRAMER_STATUS_DeviceDriver_GetProductReleaseStr(OMIINO_DEVICE_DRIVER_RAM_TYPE * pDriverRAM, char * pProductReleaseStr)
{
    strncpy(pProductReleaseStr,pDriverRAM->ProductReleaseStr,OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildDateTimeStr(OMIINO_DEVICE_DRIVER_RAM_TYPE * pDriverRAM, char * pSoftwareBuildDateTimeStr)
{
    strncpy(pSoftwareBuildDateTimeStr,pDriverRAM->SoftwareBuildDateTimeStr,OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildVersionStr(OMIINO_DEVICE_DRIVER_RAM_TYPE * pDriverRAM, char * pSoftwareBuildVersionStr)
{
    strncpy(pSoftwareBuildVersionStr,pDriverRAM->SoftwareBuildVersionStr,OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD);
}

