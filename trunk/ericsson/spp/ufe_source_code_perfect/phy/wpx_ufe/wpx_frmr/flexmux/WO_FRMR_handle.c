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



void OMIINO_FRAMER_DeviceDriverHandleCreate( OMIINO_FRAMER_ENVIRONMENT_WRAPPER_TABLE_TYPE * pEnvionmentWrapperTable, char *pHandleName)
{
    OMIINO_FRAMER_ENVIRONMENT_WRAPPER_CriticalRegionCreateHandle(&(pEnvionmentWrapperTable->DeviceDriverHandle), pHandleName);
}


U8 OMIINO_FRAMER_DeviceDriverHandleGet(void)
{
    return(OMIINO_RAM.EnvironmentWrapper.DeviceDriverHandle);
}





