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



void OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetClockRecovery(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pClient, U8 ClockRecovery)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_TRANSMIT_TIMING_MAX>ClockRecovery,ClockRecovery);

	pClient->ClockRecovery = ClockRecovery;
}




void OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetClockRecovery(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pClient, U8 *pClockRecovery)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pClockRecovery,0);

	*pClockRecovery = pClient->ClockRecovery;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_TRANSMIT_TIMING_MAX>(*pClockRecovery),(*pClockRecovery));
}





