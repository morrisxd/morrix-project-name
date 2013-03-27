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


U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	if(1==OMIINO_RAM.Device[iDevice].TestMode)
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_TEST_MODE_PREVENTS_REQUEST;
	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceInTestMode(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	if(0==OMIINO_RAM.Device[iDevice].TestMode)
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_TEST_MODE_PREVENTS_REQUEST;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceProtectionMode(U8 iDevice, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceProtectionMode)
	{
		case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
				if(!OMIINO_DEVICE_PERSONALITY_SupportsNeitherLineNorCardProtection(iDevice))
				{
					Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				}
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
				if(!OMIINO_DEVICE_PERSONALITY_SupportsLineProtection(iDevice))
				{
					Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				}
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
				if(!OMIINO_DEVICE_PERSONALITY_SupportsCardProtection(iDevice))
				{
					Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				}
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceProtectionMode);
				break;
	}
	
	return Result;
}



U8 OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_RAM.Device[iDevice].M_of_N.Limit<=OMIINO_RAM.Device[iDevice].M_of_N.Current)
	{
		Result = WPX_UFE_FRAMER_PM_M_OF_N_LIMIT_REACHED;
	}

	return Result;
}

U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_DEVICE_PERSONALITY_IsDefined(iDevice))
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_NOT_DEFINED;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_DEVICE_PERSONALITY_SupportsDiscreteClientE1T1(iDevice))
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_DOES_NOT_SUPPORT_DISCRETE_CLIENT_E1T1;
	}

	return Result;
}




U8 OMIINO_FRAMER_DeviceIsFreeOfLineSideFacilities(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 iLinePort;

	for(iLinePort=0; iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS; iLinePort++)
	{
		if(OMIINO_FRAMER_SONET_SDH_Port_Has_Facilities(iDevice, iLinePort))
		{
			Result=WPX_UFE_FRAMER_ERROR_PORT_HAS_FACILITIES;
			break; /* out of loop */
		}
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceIsFreeOfPortConfiguration(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 iLinePort;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM=OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	for(iLinePort=0; iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS; iLinePort++)
	{
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
		
		if(WPX_UFE_FRAMER_LINE_PORT_RATE_OFF!=pDevicePortRAM->Configuration.Rate)
		{
			Result=WPX_UFE_FRAMER_ERROR_PORT_RATE_IS_CONFIGURED;
			break; /* out of loop */
		}
	}

	return Result;
}







U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Logical_Side_A_iLinePort;

	if(OMIINO_DEVICE_PERSONALITY_SupportsLineProtection(iDevice))
	{
		if(OMIINO_FRAMER_PortMap_Logical_iLinePort_Logical_Side_A_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLinePort, &Logical_Side_A_iLinePort))
		{
			if(iLinePort!=Logical_Side_A_iLinePort)
			{
				Result = WPX_UFE_FRAMER_ERROR_PROTECTION_PORT_CANNOT_BE_CONFIGURED;
			}
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_DEVICE_PERSONALITY_SupportsSONETSDH(iDevice))
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_CAPABILITY_DOES_NOT_SUPPORT_SONET_SDH;
	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceDynamicRule_SDH_HighOrderIndexMatch(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;


	if(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pConfigurationRAMForPort->Configuration))<=pEndpoint->u.sdh.iJ)
	{
		Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}


	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_K<=pEndpoint->u.sdh.iK)
	{
		Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceDynamicRule_SONET_HighOrderIndexMatch(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pConfigurationRAMForPort->Configuration))<=pEndpoint->u.sonet.iU)
	{
		Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pEndpoint)
{
	U8 PortMode;
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pConfigurationRAMForPort->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			Result=OMIINO_FRAMER_DeviceDynamicRule_SDH_HighOrderIndexMatch(pConfigurationRAMForPort, pEndpoint);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			Result=OMIINO_FRAMER_DeviceDynamicRule_SONET_HighOrderIndexMatch(pConfigurationRAMForPort, pEndpoint);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	

	return Result;
}



U8 OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatches_SONET_Mode(U8 AlarmCategory)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AlarmCategory)
	{
		case WPX_UFE_FRAMER_SDH_LOS:			
		case WPX_UFE_FRAMER_SDH_LOF:			
		case WPX_UFE_FRAMER_SDH_RS_TIM:		
		case WPX_UFE_FRAMER_SDH_MS_AIS:		
		case WPX_UFE_FRAMER_SDH_MS_RDI:		
		case WPX_UFE_FRAMER_SDH_OOF:			
		case WPX_UFE_FRAMER_SDH_MS_EXC:		
		case WPX_UFE_FRAMER_SDH_MS_DEG:		
		case WPX_UFE_FRAMER_SDH_AU_AIS:		
		case WPX_UFE_FRAMER_SDH_AU_LOP:		
		case WPX_UFE_FRAMER_SDH_HP_LOM:		
		case WPX_UFE_FRAMER_SDH_HP_TIM:		
		case WPX_UFE_FRAMER_SDH_HP_PLM:		
		case WPX_UFE_FRAMER_SDH_HP_UNEQ:		
		case WPX_UFE_FRAMER_SDH_HP_RDI:		
		case WPX_UFE_FRAMER_SDH_HP_EXC:		
		case WPX_UFE_FRAMER_SDH_HP_DEG:		
		case WPX_UFE_FRAMER_SDH_LP_TIM:		
		case WPX_UFE_FRAMER_SDH_LP_PLM:		
		case WPX_UFE_FRAMER_SDH_LP_UNEQ:		
		case WPX_UFE_FRAMER_SDH_LP_RDI:		
		case WPX_UFE_FRAMER_SDH_LP_EXC:		
		case WPX_UFE_FRAMER_SDH_LP_DEG:		
		case WPX_UFE_FRAMER_SDH_TU_AIS:		
		case WPX_UFE_FRAMER_SDH_TU_LOP:	
		case WPX_UFE_FRAMER_SDH_RS_EXC:		
		case WPX_UFE_FRAMER_SDH_RS_DEG:
			Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_PREVENTS_REQUEST;
			break;

		case WPX_UFE_FRAMER_SONET_LOS:		
		case WPX_UFE_FRAMER_SONET_LOF:		
		case WPX_UFE_FRAMER_SONET_TIM_S:		
		case WPX_UFE_FRAMER_SONET_AIS_L:		
		case WPX_UFE_FRAMER_SONET_RDI_L:		
		case WPX_UFE_FRAMER_SONET_OOF:		
		case WPX_UFE_FRAMER_SONET_EXC_L:		
		case WPX_UFE_FRAMER_SONET_DEG_L:		
		case WPX_UFE_FRAMER_SONET_LOM_P:		
		case WPX_UFE_FRAMER_SONET_TIM_P:		
		case WPX_UFE_FRAMER_SONET_PLM_P:		
		case WPX_UFE_FRAMER_SONET_UNEQ_P:		
		case WPX_UFE_FRAMER_SONET_RDI_P:		
		case WPX_UFE_FRAMER_SONET_EXC_P:		
		case WPX_UFE_FRAMER_SONET_DEG_P:		
		case WPX_UFE_FRAMER_SONET_TIM_V:		
		case WPX_UFE_FRAMER_SONET_PLM_V:		
		case WPX_UFE_FRAMER_SONET_UNEQ_V:		
		case WPX_UFE_FRAMER_SONET_RDI_V:		
		case WPX_UFE_FRAMER_SONET_EXC_V:		
		case WPX_UFE_FRAMER_SONET_DEG_V:		
		case WPX_UFE_FRAMER_SONET_AIS_V:		
		case WPX_UFE_FRAMER_SONET_LOP_V:		
		case WPX_UFE_FRAMER_SONET_AIS_P:		
		case WPX_UFE_FRAMER_SONET_LOP_P:		
		case WPX_UFE_FRAMER_SONET_EXC_S:		
		case WPX_UFE_FRAMER_SONET_DEG_S:
			Result=WPX_UFE_FRAMER_OK;
			break;

		default:
			Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_PREVENTS_REQUEST;
	}

	return Result;
}




U8 OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatches_SDH_Mode(U8 AlarmCategory)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AlarmCategory)
	{
		case WPX_UFE_FRAMER_SDH_LOS:			
		case WPX_UFE_FRAMER_SDH_LOF:			
		case WPX_UFE_FRAMER_SDH_RS_TIM:		
		case WPX_UFE_FRAMER_SDH_MS_AIS:		
		case WPX_UFE_FRAMER_SDH_MS_RDI:		
		case WPX_UFE_FRAMER_SDH_OOF:			
		case WPX_UFE_FRAMER_SDH_MS_EXC:		
		case WPX_UFE_FRAMER_SDH_MS_DEG:		
		case WPX_UFE_FRAMER_SDH_AU_AIS:		
		case WPX_UFE_FRAMER_SDH_AU_LOP:		
		case WPX_UFE_FRAMER_SDH_HP_LOM:		
		case WPX_UFE_FRAMER_SDH_HP_TIM:		
		case WPX_UFE_FRAMER_SDH_HP_PLM:		
		case WPX_UFE_FRAMER_SDH_HP_UNEQ:		
		case WPX_UFE_FRAMER_SDH_HP_RDI:		
		case WPX_UFE_FRAMER_SDH_HP_EXC:		
		case WPX_UFE_FRAMER_SDH_HP_DEG:		
		case WPX_UFE_FRAMER_SDH_LP_TIM:		
		case WPX_UFE_FRAMER_SDH_LP_PLM:		
		case WPX_UFE_FRAMER_SDH_LP_UNEQ:		
		case WPX_UFE_FRAMER_SDH_LP_RDI:		
		case WPX_UFE_FRAMER_SDH_LP_EXC:		
		case WPX_UFE_FRAMER_SDH_LP_DEG:		
		case WPX_UFE_FRAMER_SDH_TU_AIS:		
		case WPX_UFE_FRAMER_SDH_TU_LOP:	
		case WPX_UFE_FRAMER_SDH_RS_EXC:		
		case WPX_UFE_FRAMER_SDH_RS_DEG:
			Result=WPX_UFE_FRAMER_OK;
			break;

		case WPX_UFE_FRAMER_SONET_LOS:		
		case WPX_UFE_FRAMER_SONET_LOF:		
		case WPX_UFE_FRAMER_SONET_TIM_S:		
		case WPX_UFE_FRAMER_SONET_AIS_L:		
		case WPX_UFE_FRAMER_SONET_RDI_L:		
		case WPX_UFE_FRAMER_SONET_OOF:		
		case WPX_UFE_FRAMER_SONET_EXC_L:		
		case WPX_UFE_FRAMER_SONET_DEG_L:		
		case WPX_UFE_FRAMER_SONET_LOM_P:		
		case WPX_UFE_FRAMER_SONET_TIM_P:		
		case WPX_UFE_FRAMER_SONET_PLM_P:		
		case WPX_UFE_FRAMER_SONET_UNEQ_P:		
		case WPX_UFE_FRAMER_SONET_RDI_P:		
		case WPX_UFE_FRAMER_SONET_EXC_P:		
		case WPX_UFE_FRAMER_SONET_DEG_P:		
		case WPX_UFE_FRAMER_SONET_TIM_V:		
		case WPX_UFE_FRAMER_SONET_PLM_V:		
		case WPX_UFE_FRAMER_SONET_UNEQ_V:		
		case WPX_UFE_FRAMER_SONET_RDI_V:		
		case WPX_UFE_FRAMER_SONET_EXC_V:		
		case WPX_UFE_FRAMER_SONET_DEG_V:		
		case WPX_UFE_FRAMER_SONET_AIS_V:		
		case WPX_UFE_FRAMER_SONET_LOP_V:		
		case WPX_UFE_FRAMER_SONET_AIS_P:		
		case WPX_UFE_FRAMER_SONET_LOP_P:		
		case WPX_UFE_FRAMER_SONET_EXC_S:		
		case WPX_UFE_FRAMER_SONET_DEG_S:
			Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_PREVENTS_REQUEST;
			break;

		default:
			Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_PREVENTS_REQUEST;
	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatchesMode(U8 DeviceMode, U8 AlarmCategory)
{
	U8 Result=WPX_UFE_FRAMER_OK;

    switch(DeviceMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			Result=OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatches_SDH_Mode(AlarmCategory);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			Result=OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatches_SONET_Mode(AlarmCategory);
            break;

        default:
            Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_OUT_OF_RANGE;
            break;
    }	

	return Result;
}




