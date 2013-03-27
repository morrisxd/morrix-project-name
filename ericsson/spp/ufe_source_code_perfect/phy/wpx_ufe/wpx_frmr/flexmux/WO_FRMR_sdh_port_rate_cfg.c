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


U8 OMIINO_FRAMER_MODE_SDH_Port_VC4_4C_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
				break;
			}
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_MODE_SDH_Port_VC4_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
				break;
			}
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_MODE_SDH_Port_VC3_HI_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
					Result=1;
					break;
				}
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SDH_Port_Has_Facilities(U8 iDevice, U8 iLinePort)
{
	U8 Result;

	Result=OMIINO_FRAMER_MODE_SDH_Port_VC4_4C_Facilities_Exist(iDevice, iLinePort);
	if(0==Result)
	{
		Result=OMIINO_FRAMER_MODE_SDH_Port_VC4_Facilities_Exist(iDevice, iLinePort);
	}
	if(0==Result)
	{
		Result=OMIINO_FRAMER_MODE_SDH_Port_VC3_HI_Facilities_Exist(iDevice, iLinePort);
	}
	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS48C_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
				break;
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS12C_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
				break;
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS3C_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
				break;
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_MODE_SONET_Port_STS1_Facilities_Exist(U8 iDevice, U8 iLinePort)
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
				break;
			}
		}
	}

	return Result;
}



U8 OMIINO_FRAMER_MODE_SONET_Port_Has_Facilities(U8 iDevice, U8 iLinePort)
{
	U8 Result;

	Result=OMIINO_FRAMER_MODE_SONET_Port_STS12C_Facilities_Exist(iDevice, iLinePort);

	if(0==Result)
	{
		Result=OMIINO_FRAMER_MODE_SONET_Port_STS3C_Facilities_Exist(iDevice, iLinePort);
	}
	if(0==Result)
	{
		Result=OMIINO_FRAMER_MODE_SONET_Port_STS1_Facilities_Exist(iDevice, iLinePort);
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_Port_Has_Facilities(U8 iDevice, U8 iLinePort)
{
	U8 Result=0;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_FRAMER_MODE_SDH_Port_Has_Facilities(iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_FRAMER_MODE_SONET_Port_Has_Facilities(iDevice, iLinePort);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceMode);
				break;

	}


	return Result;
}


void OMIINO_FRAMER_SONET_SDH_Port_SetRate(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 PortRate)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_PORT_MAX_RATES>PortRate,PortRate);

	pPortHierarchy->Rate = PortRate;
}




void OMIINO_FRAMER_SONET_SDH_Port_GetRate(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 *pPortRate)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pPortRate,0);

	*pPortRate = pPortHierarchy->Rate;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_LINE_PORT_MAX_RATES>(*pPortRate),(*pPortRate));
}



U8 OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy)
{
    U8 Result=0;

	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);

	switch(pPortHierarchy->Rate)
    {    
		case    WPX_UFE_FRAMER_LINE_PORT_RATE_OFF:	  
                Result=0;
                break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM1:
                Result = 1;
                break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM4:
                Result = 4;
                break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC3:
                Result = 3;
                break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC12:
                Result = 12;
                break;

        default:
				OMIINO_FRAMER_RSE(pPortHierarchy->Rate);
                break;

    }

    return Result;
}




