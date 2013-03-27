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

#include "WO_FRMR_vga_driver_public.h"





void Environment_Screen_Alarms_PortAlarmAnnounce(U8 iDevice, U8 iLinePort, U8 AlarmCategory, U8 IsAsserted)
{
	char Payload[3];

	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	Payload[0]=iLinePort; 
	Payload[1]=AlarmCategory;
	Payload[2]=IsAsserted;

	OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_LINE_PORT_ALARM, Payload, 3);
}


void Environment_Screen_Alarms_PathAlarmAnnounce(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE * pLineEndpoint, U8 AlarmCategory, U8 IsAsserted)
{
	char Payload[24];
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;

	WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pLineEndpoint);
	EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, (U8 *)Payload);

	Payload[EndpointEncodedBytes]=AlarmCategory;
	Payload[EndpointEncodedBytes+1]=IsAsserted;

	OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PATH_ALARM, Payload, EndpointEncodedBytes+2);
}



void Environment_Screen_Alarms_Initialize(void)
{
	Environment_ReportLoggerInitialize(&Environment_Screen.Logger[PAGE_ALARMS], PAGE_ALARMS, 0);
}
