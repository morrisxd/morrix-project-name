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



void WPX_UFE_FRAMER_API_InterruptServiceRequest(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		/* TODO WPX_UFE_FRAMER_API_InterruptServiceRequest */
	}
}


