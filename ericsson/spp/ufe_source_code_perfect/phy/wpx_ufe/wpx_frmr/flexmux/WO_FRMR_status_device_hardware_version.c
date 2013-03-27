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


void OMIINO_FRAMER_STATUS_DeviceDriver_GetHardwareVersionStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pHardwareVersionStr)
{
    *pHardwareVersionStr = 0;
    strcat(pHardwareVersionStr,pStatus->HardwareInformation.ProductName);
	strcat(pHardwareVersionStr, " ");
    strcat(pHardwareVersionStr,pStatus->HardwareInformation.Version);
	strcat(pHardwareVersionStr, " ");
    strcat(pHardwareVersionStr,pStatus->HardwareInformation.Date);
	strcat(pHardwareVersionStr, " ");
    strcat(pHardwareVersionStr,pStatus->HardwareInformation.Time);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareProductNameStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pProductNameStr)
{
    memset(pStatus->HardwareInformation.ProductName,'\0',OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_PRODUCT_NAME_FIELD+1);
    strncpy(pStatus->HardwareInformation.ProductName,pProductNameStr,OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_PRODUCT_NAME_FIELD);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareVersionStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pHardwareVersionStr)
{
    memset(pStatus->HardwareInformation.Version,'\0',OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_VERSION_FIELD+1);
    strncpy(pStatus->HardwareInformation.Version,pHardwareVersionStr,OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_VERSION_FIELD);
}



void OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareDateStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pDateStr)
{
    memset(pStatus->HardwareInformation.Date,'\0',OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_DATE_FIELD+1);
    strncpy(pStatus->HardwareInformation.Date,pDateStr,OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_DATE_FIELD);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareTimeStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pTimeStr)
{
    memset(pStatus->HardwareInformation.Time,'\0',OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_TIME_FIELD+1);
    strncpy(pStatus->HardwareInformation.Time,pTimeStr,OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_TIME_FIELD);
}



