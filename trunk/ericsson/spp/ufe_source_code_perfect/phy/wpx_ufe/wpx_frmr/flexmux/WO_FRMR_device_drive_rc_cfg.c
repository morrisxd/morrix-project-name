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



void OMIINO_FRAMER_DEVICE_Diagnostic_SetDriveRecoveredClock(OMIINO_FRAMER_CONFIGURATION_DEVICE_DIAGNOSTIC_TYPE * pDeviceDiagnostic, U8 iLinePort, U8 OutputSelector)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceDiagnostic,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLinePort,iLinePort);
	OMIINO_FRAMER_ASSERT(MAX_SONET_SDH_RECOVERED_CLOCKS>OutputSelector,OutputSelector);

	pDeviceDiagnostic->iLinePort_DriveRecoveredClock[OutputSelector] = iLinePort;
}


void OMIINO_FRAMER_DEVICE_SetSquelchRecoveredClock(OMIINO_FRAMER_CONFIGURATION_DEVICE_DIAGNOSTIC_TYPE * pDeviceDiagnostic,U8 Squelch_Mode, U8 OutputSelector)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceDiagnostic,0);
	OMIINO_FRAMER_ASSERT(MAX_SONET_SDH_RECOVERED_CLOCKS>OutputSelector,OutputSelector);

	pDeviceDiagnostic->SquelchRecoveredClock[OutputSelector] = Squelch_Mode;
}


void OMIINO_FRAMER_DEVICE_Diagnostic_GetDriveRecoveredClock(OMIINO_FRAMER_CONFIGURATION_DEVICE_DIAGNOSTIC_TYPE * pDeviceDiagnostic, U8 * piLinePort, U8 OutputSelector)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceDiagnostic,0);
	OMIINO_FRAMER_ASSERT(NULL!=piLinePort,0);
	OMIINO_FRAMER_ASSERT(MAX_SONET_SDH_RECOVERED_CLOCKS>OutputSelector,OutputSelector);

	*piLinePort = pDeviceDiagnostic->iLinePort_DriveRecoveredClock[OutputSelector];

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>(*piLinePort),(*piLinePort));
}

void OMIINO_FRAMER_DEVICE_GetSquelchRecoveredClock(OMIINO_FRAMER_CONFIGURATION_DEVICE_DIAGNOSTIC_TYPE * pDeviceDiagnostic,U8* pSquelch_Mode, U8 OutputSelector)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceDiagnostic,0);
	OMIINO_FRAMER_ASSERT(NULL!=pSquelch_Mode,0);
	OMIINO_FRAMER_ASSERT(MAX_SONET_SDH_RECOVERED_CLOCKS>OutputSelector,OutputSelector);

	*pSquelch_Mode = pDeviceDiagnostic->SquelchRecoveredClock[OutputSelector];

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_RECOVERED_CLOCK_SQUELCH_MODE_MAX>(*pSquelch_Mode),(*pSquelch_Mode));
}


