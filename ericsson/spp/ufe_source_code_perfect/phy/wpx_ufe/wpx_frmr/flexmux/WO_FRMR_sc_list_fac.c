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

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_List_Facility(U8 iDevice, U32 iClientFacility, OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8 *			pPayloadStart;
	U8				Length=0;

	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted) /* Support for first device only */
	{
		pPayloadStart=&piAddAt[20];
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_CLIENT_FACILITY_EXISTS, 0, 8, 0);
		UtilityPutU32(iClientFacility, pPayloadStart);
		UtilityPutU32((U32)(pClient->ClientType), 4+pPayloadStart);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, 8+Length);
	}
}

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_FacilitiesForDevice(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice) 
{
	U32 iClientFacility;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient;
	
	for(iClientFacility=0;iClientFacility<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iClientFacility++)
	{
		pClient=OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pConfigurationRAMForDevice, iClientFacility);
		if(OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Exists(pClient))
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_List_Facility(iDevice, iClientFacility, pClient);
		}
	}
}
