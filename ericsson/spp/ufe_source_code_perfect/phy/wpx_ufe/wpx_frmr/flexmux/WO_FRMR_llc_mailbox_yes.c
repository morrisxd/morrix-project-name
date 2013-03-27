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


void LowLevelComms_DefineAccess(void)
{
	U8 iDevice;

	for(iDevice=0;iDevice<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;iDevice++)
    {
		WPX_FRMR_LowLevelCommsEnableAccess(iDevice);
	}
}

