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


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_GetDriveRecoveredClockRateParameterFOrOutputSelector(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE * pClient, U8 OutputSelector, U32 * pRateParameter)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pRateParameter,0);

	*pRateParameter = pClient->DriveRecovered_SocketClient_RateParameter[OutputSelector];

}





void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_SetDriveRecoveredClock(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE * pClient, U8 OutputSelector, U32 iSocketClientPDH, U32 RateParameter)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS>iSocketClientPDH,iSocketClientPDH);

	pClient->iDriveRecovered_SocketClient_Clock[OutputSelector] = iSocketClientPDH;
	pClient->DriveRecovered_SocketClient_RateParameter[OutputSelector]=RateParameter;
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_GetDriveRecoveredClock(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE * pClient, U8 OutputSelector, U32 * piSocketClientPDH)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=piSocketClientPDH,0);

	*piSocketClientPDH = pClient->iDriveRecovered_SocketClient_Clock[OutputSelector];

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS>(*piSocketClientPDH),(*piSocketClientPDH));
}





