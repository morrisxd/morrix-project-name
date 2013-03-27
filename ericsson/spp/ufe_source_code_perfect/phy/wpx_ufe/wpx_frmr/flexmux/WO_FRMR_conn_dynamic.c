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



U8 OMIINO_FRAMER_ConnectionsDynamicRule_EndpointsAreCompatible(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pAnyEndpoint, OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(pClient->ClientType)
	{   
		case    WPX_UFE_FRAMER_CLIENT_E1:
		case    WPX_UFE_FRAMER_CLIENT_C12:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_T1:
		case    WPX_UFE_FRAMER_CLIENT_C11:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}
				break;				

		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}				
				break;

		case    WPX_UFE_FRAMER_CLIENT_C4:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}				
				break;

		case    WPX_UFE_FRAMER_CLIENT_C3:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}				
				break;

		case    WPX_UFE_FRAMER_CLIENT_T3:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}				
				break;

		case    WPX_UFE_FRAMER_CLIENT_E3:
				switch(pAnyEndpoint->Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
							break;

				}				
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
				break;
	}

	return Result;
}


U8 OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pAnyEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pHO_Path;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pLO_Path;

	if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pAnyEndpoint->Bandwidth))
    {
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pAnyEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pAnyEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pAnyEndpoint);

			if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pHO_Path))
			{
				Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_ALREADY_CONNECTED;
			}
		}
    }
    else
    {
		pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pAnyEndpoint);

		if(!OMIINO_FRAMER_SONET_SDH_LO_Facility_IsFree(pLO_Path))
		{
			Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_ALREADY_CONNECTED;
		}
    }

	return Result;
}




U8 OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pAnyEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pHO_Path;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pLO_Path;

	if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pAnyEndpoint->Bandwidth))
    {
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pAnyEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pAnyEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pAnyEndpoint);

			if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsConnected(pHO_Path))
			{
				Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_NOT_CONNECTED;
			}
		}
    }
    else
    {
		pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pAnyEndpoint);

		if(!OMIINO_FRAMER_SONET_SDH_LO_Facility_IsConnected(pLO_Path))
		{
			Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_NOT_CONNECTED;
		}
    }

	return Result;
}



