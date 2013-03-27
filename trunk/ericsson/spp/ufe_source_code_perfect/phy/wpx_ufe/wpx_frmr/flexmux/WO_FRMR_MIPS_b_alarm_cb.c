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

#include <stdio.h>
#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"



void WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_SONET_SDH_PortAlarmAnnounce(U8 iDevice, U8 iPresentationLinePort, U8 AlarmCategory, U8 IsAsserted)
{
	char Payload[3];
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Payload[0]=(unsigned char)iLogicalLinePort; 
			Payload[1]=AlarmCategory;
			Payload[2]=IsAsserted;

			OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_LINE_PORT_ALARM, Payload, 3);
		}
	}
}



void WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_SONET_SDH_PathAlarmAnnounce(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 AlarmCategory, U8 IsAsserted)
{
	char Payload[24];
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;
	U8 iLogicalLinePort;
	U8 Result;


	Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		/* We have a Presentation LineEndpoint (port) */
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), LineEndpoint.iLinePort, &iLogicalLinePort))
		{
			LineEndpoint.iLinePort=iLogicalLinePort;
			/* We have a Logical LineEndpoint (port) */
			EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, (U8 *)Payload);

			Payload[EndpointEncodedBytes]=AlarmCategory;
			Payload[EndpointEncodedBytes+1]=IsAsserted;

			OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PATH_ALARM, Payload, EndpointEncodedBytes+2);
		}
	}
}



void WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_DiscreteClientPortAlarmAnnounce(U8 iDevice, U8 iDiscreteClientPort, U8 AlarmCategory, U8 IsAsserted)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);
	OMIINO_REMOVE_COMPILER_WARNING(iDiscreteClientPort);
	OMIINO_REMOVE_COMPILER_WARNING(AlarmCategory);
	OMIINO_REMOVE_COMPILER_WARNING(IsAsserted);

	
}



void WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_SocketClientSidePortAlarmAnnounce(U8 iDevice, U32 iSocketClientPDH, U8 AlarmCategory, U8 IsAsserted)
{
	char Payload[6];

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		UtilityPutU32(iSocketClientPDH, (U8 *)Payload);
		Payload[4]=AlarmCategory;
		Payload[5]=IsAsserted;

		OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_CLIENT_ALARM, Payload, 6);
	}
}



void WPX_UFE_FRAMER_MIPS_Initialize_BindingAlarmCallbackTable(WPX_UFE_FRAMER_ALARM_OUTPUT_TABLE_ANNOUNCE_TYPE * pAlarmCallbackTable)
{
	pAlarmCallbackTable->DiscreteClientPortAlarmAnnounce=WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_DiscreteClientPortAlarmAnnounce;
	pAlarmCallbackTable->SocketClientSidePortAlarmAnnounce=WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_SocketClientSidePortAlarmAnnounce;
	pAlarmCallbackTable->SONET_SDH_PathAlarmAnnounce=WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_SONET_SDH_PathAlarmAnnounce;
	pAlarmCallbackTable->SONET_SDH_PortAlarmAnnounce=WPX_UFE_FRAMER_MIPS_AlarmCallbackTable_SONET_SDH_PortAlarmAnnounce;

}
