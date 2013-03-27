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


void OMIINO_FRAMER_ListPortFacility(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * LineEndpoint)
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


U8 OMIINO_FRAMER_MODE_SDH_Port_VC4_4C_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;
	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_J_Endpoint;
	U8 iJ_Walk;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C))
	{
		Walking_J_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C;
		Walking_J_Endpoint.iLinePort=iLinePort;
		Walking_J_Endpoint.u.sdh.iK=0;
		Walking_J_Endpoint.u.sdh.iL=0;
		Walking_J_Endpoint.u.sdh.iM=0;

		for(iJ_Walk=0;iJ_Walk<(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration))/4);iJ_Walk++)
		{
			Walking_J_Endpoint.u.sdh.iJ=iJ_Walk;

			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_J_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result=1;
				OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_J_Endpoint);
				OMIINO_FRAMER_List_SDH_LO_FacilitiesForParent(iDevice, &Walking_J_Endpoint);
			}
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_MODE_SDH_Port_VC4_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;
	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_J_Endpoint;
	U8 iJ_Walk;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4))
	{
		Walking_J_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
		Walking_J_Endpoint.iLinePort=iLinePort;
		Walking_J_Endpoint.u.sdh.iK=0;
		Walking_J_Endpoint.u.sdh.iL=0;
		Walking_J_Endpoint.u.sdh.iM=0;

		for(iJ_Walk=0;iJ_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iJ_Walk++)
		{
            if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT>iJ_Walk)
            {
			Walking_J_Endpoint.u.sdh.iJ=iJ_Walk;
            }
            else
            {
                continue;
            }
            

			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_J_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result=1;
				OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_J_Endpoint);
				OMIINO_FRAMER_List_SDH_LO_FacilitiesForParent(iDevice, &Walking_J_Endpoint);
			}
		}
	}

	return Result;
}






U8 OMIINO_FRAMER_MODE_SDH_Port_VC3_HI_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;
	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_Endpoint;
	U8 iJ_Walk;
	U8 iK_Walk;


	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI))
	{
		Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI;
		Walking_Endpoint.iLinePort=iLinePort;
		
		Walking_Endpoint.u.sdh.iL=0;
		Walking_Endpoint.u.sdh.iM=0;

		for(iJ_Walk=0;iJ_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iJ_Walk++)
		{
			Walking_Endpoint.u.sdh.iJ=iJ_Walk;

			for(iK_Walk=0;iK_Walk<3;iK_Walk++)
			{
				Walking_Endpoint.u.sdh.iK=iK_Walk;
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

				if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
				{
					if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
					{
						Result=1;
						OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_Endpoint);
						OMIINO_FRAMER_List_SDH_LO_FacilitiesForParent(iDevice, &Walking_Endpoint);
					}
				}
			}
		}
	}

	return Result;
}




U8 OMIINO_FRAMER_MODE_SDH_Port_VC3_LO_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;
	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_Endpoint;
	U8 iJ_Walk;
	U8 iK_Walk;


	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI))
	{
		Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO;
		Walking_Endpoint.iLinePort=iLinePort;
		
		Walking_Endpoint.u.sdh.iL=0;
		Walking_Endpoint.u.sdh.iM=0;

		for(iJ_Walk=0;iJ_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iJ_Walk++)
		{
			Walking_Endpoint.u.sdh.iJ=iJ_Walk;

			for(iK_Walk=0;iK_Walk<3;iK_Walk++)
			{
				Walking_Endpoint.u.sdh.iK=iK_Walk;
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

				if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
				{
					if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
					{
						Result=1;
						OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_Endpoint);
					}
				}
			}
		}
	}

	return Result;
}







void OMIINO_FRAMER_MODE_SDH_Port_List_Facilities(U8 iDevice, U8 iLinePort)
{
	OMIINO_FRAMER_MODE_SDH_Port_VC4_Facilities_List(iDevice, iLinePort);
	OMIINO_FRAMER_MODE_SDH_Port_VC3_HI_Facilities_List(iDevice, iLinePort);
	OMIINO_FRAMER_MODE_SDH_Port_VC3_LO_Facilities_List(iDevice, iLinePort);
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS48C_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;

	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_Endpoint;
	U8 iU_Walk;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C))
	{
		Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C;
		Walking_Endpoint.iLinePort=iLinePort;
		Walking_Endpoint.u.sonet.iV=0;
		Walking_Endpoint.u.sonet.iW=0;

		for(iU_Walk=0;iU_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iU_Walk++)
		{
			Walking_Endpoint.u.sonet.iU=iU_Walk;

			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result=1;
				OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_Endpoint);
				OMIINO_FRAMER_List_SONET_LO_FacilitiesForParent(iDevice, &Walking_Endpoint);
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS12C_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;

	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_Endpoint;
	U8 iU_Walk;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C))
	{
		Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C;
		Walking_Endpoint.iLinePort=iLinePort;
		Walking_Endpoint.u.sonet.iV=0;
		Walking_Endpoint.u.sonet.iW=0;

		for(iU_Walk=0;iU_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iU_Walk+=12)
		{
			Walking_Endpoint.u.sonet.iU=iU_Walk;

			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result=1;
				OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_Endpoint);
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS3C_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;

	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_Endpoint;
	U8 iU_Walk;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C))
	{
		Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C;
		Walking_Endpoint.iLinePort=iLinePort;
		Walking_Endpoint.u.sonet.iV=0;
		Walking_Endpoint.u.sonet.iW=0;

		for(iU_Walk=0;iU_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iU_Walk+=3)
		{
			Walking_Endpoint.u.sonet.iU=iU_Walk;

			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result=1;
				OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_Endpoint);
				OMIINO_FRAMER_List_SONET_LO_FacilitiesForParent(iDevice, &Walking_Endpoint);
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS1_Facilities_List(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;

	OMIINO_FRAMER_HIERARCHY_TYPE									*	pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						*	pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					*	pDeviceHO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE										Walking_Endpoint;
	U8 iU_Walk;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
	if(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1))
	{
		Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;
		Walking_Endpoint.iLinePort=iLinePort;
		Walking_Endpoint.u.sonet.iV=0;
		Walking_Endpoint.u.sonet.iW=0;

		for(iU_Walk=0;iU_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iU_Walk++)
		{
			Walking_Endpoint.u.sonet.iU=iU_Walk;

			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result=1;
				OMIINO_FRAMER_ListPortFacility(iDevice, &Walking_Endpoint);
				OMIINO_FRAMER_List_SONET_LO_FacilitiesForParent(iDevice, &Walking_Endpoint);
			}
		}
	}

	return Result;
}



void OMIINO_FRAMER_MODE_SONET_Port_List_Facilities(U8 iDevice, U8 iLinePort)
{
	OMIINO_FRAMER_MODE_SONET_Port_STS12C_Facilities_List(iDevice, iLinePort);
	OMIINO_FRAMER_MODE_SONET_Port_STS3C_Facilities_List(iDevice, iLinePort);
	OMIINO_FRAMER_MODE_SONET_Port_STS1_Facilities_List(iDevice, iLinePort);
}


void OMIINO_FRAMER_SONET_SDH_Port_List_Facilities(U8 iDevice, U8 iLinePort)
{
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				OMIINO_FRAMER_MODE_SDH_Port_List_Facilities(iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				OMIINO_FRAMER_MODE_SONET_Port_List_Facilities(iDevice, iLinePort);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceMode);
				break;

	}
}

