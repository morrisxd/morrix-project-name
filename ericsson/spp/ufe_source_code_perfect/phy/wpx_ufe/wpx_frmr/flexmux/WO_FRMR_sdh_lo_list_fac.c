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



void OMIINO_FRAMER_List_LO_Facility(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * LineEndpoint)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8 *			pPayloadStart;
	U8				Length=0;
	U8				EndpointEncodedBytes;


	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted) /* Support for first device only */
	{
		pPayloadStart=&piAddAt[20];
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(LineEndpoint, pPayloadStart);
		Length+=EndpointEncodedBytes;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_FACILITY_EXISTS, 0, EndpointEncodedBytes, 0);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}



void OMIINO_FRAMER_List_SONET_LO_FacilitiesForParent(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pParentContainerEndpoint)
{
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pDeviceLO_PathRAM;
	U8 Result;
	U8 iV;
	U8 iW;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE WalkingEndpoint;

	OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&WalkingEndpoint, pParentContainerEndpoint);
	WalkingEndpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2;
	for(iV=0;iV<7;iV++)
	{
		for(iW=0;iW<3;iW++)
		{
			WalkingEndpoint.u.sonet.iV=iV;
			WalkingEndpoint.u.sonet.iW=iW;

			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &WalkingEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, &WalkingEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_List_LO_Facility(iDevice,&WalkingEndpoint); 
			}
		}
	}
	WalkingEndpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15;
	for(iV=0;iV<7;iV++)
	{
		for(iW=0;iW<4;iW++)
		{
			WalkingEndpoint.u.sonet.iV=iV;
			WalkingEndpoint.u.sonet.iW=iW;

			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &WalkingEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, &WalkingEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_List_LO_Facility(iDevice,&WalkingEndpoint); 
			}
		}
	}
}




void OMIINO_FRAMER_List_SDH_LO_FacilitiesForParent(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pParentContainerEndpoint)
{
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pDeviceLO_PathRAM;
	U8 Result;
	U8 iK;
	U8 iL;
	U8 iM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE WalkingEndpoint;

	OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&WalkingEndpoint, pParentContainerEndpoint);
	WalkingEndpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12;
	for(iK=0;iK<3;iK++)
	{
		for(iL=0;iL<7;iL++)
		{
			for(iM=0;iM<3;iM++)
			{
				WalkingEndpoint.u.sdh.iK=iK;
				WalkingEndpoint.u.sdh.iL=iL;
				WalkingEndpoint.u.sdh.iM=iM;

				pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &WalkingEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, &WalkingEndpoint);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_List_LO_Facility(iDevice,&WalkingEndpoint); 
				}
			}
		}
	}
	WalkingEndpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11;
	for(iK=0;iK<3;iK++)
	{
		for(iL=0;iL<7;iL++)
		{
			for(iM=0;iM<4;iM++)
			{
				WalkingEndpoint.u.sdh.iK=iK;
				WalkingEndpoint.u.sdh.iL=iL;
				WalkingEndpoint.u.sdh.iM=iM;

				pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &WalkingEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, &WalkingEndpoint);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_List_LO_Facility(iDevice,&WalkingEndpoint); 
				}
			}
		}
	}
}



