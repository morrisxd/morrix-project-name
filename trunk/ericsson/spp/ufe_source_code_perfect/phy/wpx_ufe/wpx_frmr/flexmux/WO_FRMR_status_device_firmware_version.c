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
#include "WO_FRMR_firmware_image.h"

void OMIINO_FRAMER_STATUS_DeviceDriver_GetFirmwareVersionStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pFirmwareVersionStr)
{
    
    pFirmwareVersionStr="";
    strcat(pFirmwareVersionStr,pStatus->FirmwareInformation.ProductName);
	strcat(pFirmwareVersionStr, " ");
    strcat(pFirmwareVersionStr,pStatus->FirmwareInformation.Version);
	strcat(pFirmwareVersionStr, " ");
    strcat(pFirmwareVersionStr,pStatus->FirmwareInformation.DateTime);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetFirmwareProductNameStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pProductNameStr)
{
    memset(pStatus->FirmwareInformation.ProductName,'\0',OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_PRODUCT_NAME_FIELD+1);
    strncpy(pStatus->FirmwareInformation.ProductName,pProductNameStr,OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_PRODUCT_NAME_FIELD);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetFirmwareVersionStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pVersionStr)
{
    memset(pStatus->FirmwareInformation.Version,'\0',OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_VERSION_FIELD+1);
    strncpy(pStatus->FirmwareInformation.Version,pVersionStr,OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_VERSION_FIELD);
}


void OMIINO_FRAMER_STATUS_DeviceDriver_SetFirmwareDateTimeStr(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pStatus, char * pDateTimeStr)
{
    memset(pStatus->FirmwareInformation.DateTime,'\0',OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_DATE_TIME_FIELD+1);
    strncpy(pStatus->FirmwareInformation.DateTime,UFE4FirmwareTimeStamp,OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_DATE_TIME_FIELD);
}


