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




U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_Get_MAX_M_For_LO_BandwidthType( WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	U8 Result = 3;

	switch(pEndpoint->Bandwidth)
	{    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				Result=3;
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				Result=WPX_UFE_FRAMER_BUILD_OPTION_MAX_M;
				break;

		default:
				OMIINO_FRAMER_RSE(pEndpoint->Bandwidth);
				break;
	}

	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_iJ_Valid_ForConfiguredPort(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 Any_iJ)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 MAX_iJ;

	MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(pPortHierarchy);
	if(MAX_iJ<=Any_iJ)
	{
		Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_iU_Valid_ForConfiguredPort(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 Any_iU)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 MAX_iU;

	MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(pPortHierarchy);
	if(MAX_iU<=Any_iU)
	{
		Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
	}

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(OMIINO_FRAMER_CONFIGURATION_CONNECTION_FOREIGN_KEY_TYPE *pConnectionInformation)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH==pConnectionInformation->ConnectionType)
	{
		Result=WPX_UFE_FRAMER_ERROR_THROUGH_CONNECTED_ENDPOINT_PREVENTS_CONFIGURATION_CHANGE;
	}

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pAnyLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pHO_Path))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(pHO_Path->Bandwidth.Configuration.Payload!=pAnyLineEndpoint->Bandwidth)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityDoesNotExist(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *pHO_Path)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pHO_Path))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pAnyLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(pLO_Path))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(pLO_Path->Bandwidth.Configuration.Payload!=pAnyLineEndpoint->Bandwidth)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityDoesNotExist(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE *pLO_Path)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(pLO_Path))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	return Result;
}

U8 OMIINO_FRAMER_HighOrder_SONET_HO_DeleteBlockedByChildren(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8																Result=WPX_UFE_FRAMER_OK;
	U8																iV_Walk;
	U8																iW_Walk;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE		*	pDeviceWalkingLO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE									Walking_Endpoint;   


	Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15;
	Walking_Endpoint.iLinePort=pLineEndpoint->iLinePort;
	Walking_Endpoint.u.sonet.iU=pLineEndpoint->u.sonet.iU;

	for(iV_Walk=0;iV_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_V;iV_Walk++)
	{
		for(iW_Walk=0;iW_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_W;iW_Walk++)
		{
			Walking_Endpoint.u.sonet.iV=iV_Walk;
			Walking_Endpoint.u.sonet.iW=iW_Walk;

			pDeviceWalkingLO_PathRAM=OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &Walking_Endpoint);
			if(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST!=pDeviceWalkingLO_PathRAM->Configuration.FacilityState)
			{
				Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD;
				break; /* out of loop */
			}
		}
	}

	return Result;




}

U8 OMIINO_FRAMER_HighOrder_SDH_HO_VC4_DeleteBlockedByChildren(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8																Result=WPX_UFE_FRAMER_OK;
	U8																iK_Walk;
	U8																iL_Walk;
	U8																iM_Walk;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE		*	pDeviceWalkingLO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE									Walking_Endpoint;   

	Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11;
	Walking_Endpoint.iLinePort=pLineEndpoint->iLinePort;
	Walking_Endpoint.u.sdh.iJ=pLineEndpoint->u.sdh.iJ;
        Walking_Endpoint.u.sdh.iK=pLineEndpoint->u.sdh.iK;
 	for(iK_Walk=0;iK_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_K;iK_Walk++)
 	{
		for(iL_Walk=0;iL_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_L;iL_Walk++)
		{
			for(iM_Walk=0;iM_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_M;iM_Walk++)
			{
                           Walking_Endpoint.u.sdh.iK=iK_Walk;
				Walking_Endpoint.u.sdh.iL=iL_Walk;
				Walking_Endpoint.u.sdh.iM=iM_Walk;

				pDeviceWalkingLO_PathRAM=OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &Walking_Endpoint);
				if(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST!=pDeviceWalkingLO_PathRAM->Configuration.FacilityState)
				{
					Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD;
					break; /* out of loop */
				}
			}
		}
 	}

	return Result;
}

U8 OMIINO_FRAMER_HighOrder_SDH_HO_VC3_HI_DeleteBlockedByChildren(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8																Result=WPX_UFE_FRAMER_OK;
	U8																iL_Walk;
	U8																iM_Walk;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE		*	pDeviceWalkingLO_PathRAM;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE									Walking_Endpoint;   

	Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11;
	Walking_Endpoint.iLinePort=pLineEndpoint->iLinePort;
	Walking_Endpoint.u.sdh.iJ=pLineEndpoint->u.sdh.iJ;
        Walking_Endpoint.u.sdh.iK=pLineEndpoint->u.sdh.iK;
        for(iL_Walk=0;iL_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_L;iL_Walk++)
        {
           for(iM_Walk=0;iM_Walk<WPX_UFE_FRAMER_BUILD_OPTION_MAX_M;iM_Walk++)
           {
              Walking_Endpoint.u.sdh.iL=iL_Walk;
              Walking_Endpoint.u.sdh.iM=iM_Walk;
              
              pDeviceWalkingLO_PathRAM=OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &Walking_Endpoint);
              if(WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST!=pDeviceWalkingLO_PathRAM->Configuration.FacilityState)
              {
                 Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD;
                 break; /* out of loop */
              }
           }
        }

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC4_4C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iJ_Walk;
    U8 iJ=pLineEndpoint->u.sdh.iJ; 
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                      Walking_J_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM;

    if(0!=iJ)
    {
        Result=WPX_UFE_FRAMER_ERROR_VC4_4C_J_INCORRECT;
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_J_Endpoint, pLineEndpoint);
        Walking_J_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;

		for(iJ_Walk=0;iJ_Walk<OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));iJ_Walk++)
        {
            if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT>iJ_Walk)
            {
            Walking_J_Endpoint.u.sdh.iJ=iJ_Walk;
            }
            else
            {
                Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
            }

			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_J_Endpoint);

				if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
				{
					Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
					break;
				}
			}
        }
    }

    return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC4(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                          Walking_J_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE			* pDeviceHO_PathRAM;
           
    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_J_Endpoint, pLineEndpoint);
        Walking_J_Endpoint.u.sdh.iJ=0;

		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_J_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
				}
			}
		}
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC3_HI(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                      Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_ParentPathRAM;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
        Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
		Walking_Endpoint.u.sdh.iK=0;
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);

        for(iK=0; iK<WPX_UFE_FRAMER_BUILD_OPTION_MAX_K; iK++)
        {
            Walking_Endpoint.u.sdh.iK=iK;
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);
				switch(pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL:              /* Drop Through */
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
							goto DYNAMIC_RULE_CREATE_FACILITY_SDH_VC3_HI_EXIT; 
							break;
				}
			}
        }
    }

DYNAMIC_RULE_CREATE_FACILITY_SDH_VC3_HI_EXIT:

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS48C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iU_Walk;
    U8 FortyEight=48;
    U8 iU=pLineEndpoint->u.sonet.iU; 
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                      Walking_U_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM;

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(FortyEight>WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT)
        {
            Result = WPX_UFE_FRAMER_ERROR_BUILD_OPTION_PREVENTS_FACILITY_CREATION;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(0!=iU)
        {
            Result=WPX_UFE_FRAMER_ERROR_STS_U_INCORRECT;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
    }


    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;

        for(iU_Walk=0;iU_Walk<48;iU_Walk++)
        {
            Walking_U_Endpoint.u.sonet.iU=iU_Walk;

			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_U_Endpoint);

				if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
				{
					Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
					break;
				}
			}
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS12C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iU_Walk;
    U8 Twelve=12;
    U8 iU=pLineEndpoint->u.sonet.iU; 
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                          Walking_U_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE			* pDeviceHO_PathRAM;

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(Twelve>WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT)
        {
            Result = WPX_UFE_FRAMER_ERROR_BUILD_OPTION_PREVENTS_FACILITY_CREATION;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(0!=(iU%12))
        {
            Result=WPX_UFE_FRAMER_ERROR_STS_U_INCORRECT;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
    }


    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;

        for(iU_Walk=iU;iU_Walk<(iU+12);iU_Walk++)
        {
            Walking_U_Endpoint.u.sonet.iU=iU_Walk;

			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_U_Endpoint);

				if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
				{
					Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD;
					break;
				}
			}
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C;
        Walking_U_Endpoint.u.sonet.iU=0;

        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT;
				}
			}
		}
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS3C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iU_Walk;
    U8 Three=3;
    U8 iU=pLineEndpoint->u.sonet.iU; 
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                          Walking_U_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE			* pDeviceHO_PathRAM;

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(Three>WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT)
        {
            Result = WPX_UFE_FRAMER_ERROR_BUILD_OPTION_PREVENTS_FACILITY_CREATION;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(0!=(iU%3))
        {
            Result=WPX_UFE_FRAMER_ERROR_STS_U_INCORRECT;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
    }


    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;

        for(iU_Walk=iU;iU_Walk<(iU+3);iU_Walk++)
        {
            Walking_U_Endpoint.u.sonet.iU=iU_Walk;

			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_U_Endpoint);

				if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
				{
					Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_CHILD;
					break;
				}
			}
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C;
        Walking_U_Endpoint.u.sonet.iU=OMIINO_FRAMER_SONET_HIERARCHY_Given_STS3c_Get_STS12c(OMIINO_FRAMER_SONET_HIERARCHY_Given_STS1_Get_STS3c(iU));

        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C;
        Walking_U_Endpoint.u.sonet.iU=0;

        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT;
				}
			}
		}
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS1(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iU=pLineEndpoint->u.sonet.iU; 
    U8 One=1;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                          Walking_U_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE			* pDeviceHO_PathRAM;

    if(WPX_UFE_FRAMER_OK==Result)
    {
        if(One>WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT)
        {
            Result = WPX_UFE_FRAMER_ERROR_BUILD_OPTION_PREVENTS_FACILITY_CREATION;
        }
    }


    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C;
        Walking_U_Endpoint.u.sonet.iU=OMIINO_FRAMER_SONET_HIERARCHY_Given_STS1_Get_STS3c(iU);

        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C;
        Walking_U_Endpoint.u.sonet.iU=OMIINO_FRAMER_SONET_HIERARCHY_Given_STS3c_Get_STS12c(OMIINO_FRAMER_SONET_HIERARCHY_Given_STS1_Get_STS3c(iU));

        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_U_Endpoint, pLineEndpoint);
        Walking_U_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C;
        Walking_U_Endpoint.u.sonet.iU=0;

        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C==pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PARENT;
				}
			}
		}
    }

    return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC3_LO(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                      Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_ParentPathRAM;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_PathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
			}
		}
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
        Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
		Walking_Endpoint.u.sdh.iK=0;
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(0==OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST;
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);

        for(iK=0; iK<WPX_UFE_FRAMER_BUILD_OPTION_MAX_K; iK++)
        {
            Walking_Endpoint.u.sdh.iK=iK;
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);
				switch(pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
				{
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL:              /* Drop Through */
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
							goto DYNAMIC_RULE_CREATE_FACILITY_SDH_VC3_LO_EXIT; 
							break;
				}
			}
        }
    }

DYNAMIC_RULE_CREATE_FACILITY_SDH_VC3_LO_EXIT:

    return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_LO(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iWalking_M;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                              Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE					* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE				* pDeviceHO_ParentPathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE		* pDeviceLO_PathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE		* pDeviceWalkingLO_PathRAM;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
   
    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
        Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
		Walking_Endpoint.u.sdh.iK=0;
		Walking_Endpoint.u.sdh.iL=0;
		Walking_Endpoint.u.sdh.iM=0;
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
			{
				Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI;
				Walking_Endpoint.u.sdh.iK=pLineEndpoint->u.sdh.iK;
				Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);
					if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
					{
						Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST;
					}
					else
					{
						if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
						{
    						Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
						}
					}
				}
			}
			else
			{
				if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
				{
    				Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

        if(OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(pDeviceLO_PathRAM))
	    {
		    Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
	    }
        else
        {
            OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
            for(iWalking_M=0;iWalking_M<OMIINO_FRAMER_SONET_SDH_DynamicRule_Get_MAX_M_For_LO_BandwidthType(pLineEndpoint);iWalking_M++)
            {
                Walking_Endpoint.u.sdh.iM=iWalking_M;
                pDeviceWalkingLO_PathRAM=OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &Walking_Endpoint);
                if(OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(pDeviceWalkingLO_PathRAM))
	            {					
					if(pLineEndpoint->Bandwidth!=pDeviceWalkingLO_PathRAM->Bandwidth.Configuration.Payload)
                    {
                        Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
                        break;
                    }
	            }
            }
        }
    }

    return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_LO_In_STS1(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8													Result = WPX_UFE_FRAMER_OK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE				Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE *	pDeviceHO_ParentPathRAM;

	OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);

	Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;
	Walking_Endpoint.u.sonet.iU=pLineEndpoint->u.sonet.iU;
	Walking_Endpoint.u.sonet.iV=0;
	Walking_Endpoint.u.sonet.iW=0;
	pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

	if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST;
	}
	else
	{
		if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
		{
			Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
		}
	}


	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_LO(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    U8 iWalking_W;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE						Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				*	pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE  *	pDeviceLO_PathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE  *	pDeviceWalkingLO_PathRAM;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

	Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_LO_In_STS1(pDevicePortRAM, pLineEndpoint);

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

        if(OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(pDeviceLO_PathRAM))
	    {
		    Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
	    }
        else
        {
            OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
            for(iWalking_W=0;iWalking_W<OMIINO_FRAMER_SONET_SDH_DynamicRule_Get_MAX_M_For_LO_BandwidthType(pLineEndpoint);iWalking_W++)
            {
                Walking_Endpoint.u.sonet.iW=iWalking_W;
                pDeviceWalkingLO_PathRAM=OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, &Walking_Endpoint);
                if(OMIINO_FRAMER_SONET_SDH_LO_Facility_Exists(pDeviceWalkingLO_PathRAM))
	            {
					if(pLineEndpoint->Bandwidth!=pDeviceWalkingLO_PathRAM->Bandwidth.Configuration.Payload)
                    {
                        Result=WPX_UFE_FRAMER_ERROR_FACILITY_BLOCKED_BY_PEER;
                        break;
                    }
	            }
            }
        }
    }

    return Result;
}











U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC4_4C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=WPX_UFE_FRAMER_OK;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC4(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=OMIINO_FRAMER_HighOrder_SDH_HO_VC4_DeleteBlockedByChildren(pDeviceRAM, pLineEndpoint);
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC3_HI(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=OMIINO_FRAMER_HighOrder_SDH_HO_VC3_HI_DeleteBlockedByChildren(pDeviceRAM, pLineEndpoint);
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS48C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=WPX_UFE_FRAMER_OK;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS12C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=WPX_UFE_FRAMER_OK;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS3C(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=WPX_UFE_FRAMER_OK;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS1(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE         * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE      * pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
	    
		if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
		switch(pDeviceHO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=OMIINO_FRAMER_HighOrder_SONET_HO_DeleteBlockedByChildren(pDeviceRAM,pLineEndpoint);
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
				OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
                    break;
        }
    }

    return Result;
}












U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC3_LO(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE						Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE					* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE				* pDeviceHO_ParentPathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE				* pDeviceHO_PathRAM=NULL;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
   
    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
        Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
		Walking_Endpoint.u.sdh.iK=0;
		Walking_Endpoint.u.sdh.iL=0;
		Walking_Endpoint.u.sdh.iM=0;

		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
			{
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST;
			}
			else
			{
				if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
				{
    				Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		    
			if(WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO!=pDeviceHO_PathRAM->Bandwidth.Configuration.Payload)
			{
				Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
			}
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			switch(pDeviceHO_PathRAM->Configuration.FacilityState)
			{
				case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
						Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
						break;

				case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
						Result=WPX_UFE_FRAMER_OK;
						break;

				case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
						Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
						break;

				default:
						OMIINO_FRAMER_RSE(pDeviceHO_PathRAM->Configuration.FacilityState);
						break;
			}
		}       
    }

    return Result;
}




U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_LO(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                              Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE					* pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE				* pDeviceHO_ParentPathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE		* pDeviceLO_PathRAM;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
   
    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
        Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
		Walking_Endpoint.u.sdh.iK=0;
		Walking_Endpoint.u.sdh.iL=0;
		Walking_Endpoint.u.sdh.iM=0;

		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
			{
				Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI;
				Walking_Endpoint.u.sdh.iK=pLineEndpoint->u.sdh.iK;
				pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);
				if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
				{
					Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST;
				}
				else
				{
					if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
					{
    					Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
					}
				}
			}
			else
			{
				if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
				{
    				Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
        
		switch(pDeviceLO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=WPX_UFE_FRAMER_OK;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
					OMIINO_FRAMER_RSE(pDeviceLO_PathRAM->Configuration.FacilityState);
                    break;
        }
       
    }

    return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_LO(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE                          Walking_Endpoint;            
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE             * pDevicePortRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE          * pDeviceHO_ParentPathRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE	* pDeviceLO_PathRAM;

    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
   
    if(WPX_UFE_FRAMER_OK==Result)
    {
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
        Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C;
		Walking_Endpoint.u.sonet.iV=0;
		Walking_Endpoint.u.sonet.iW=0;

		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

			if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
			{
				OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
				Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C;
				Walking_Endpoint.u.sonet.iV=0;
				Walking_Endpoint.u.sonet.iW=0;

				Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, &Walking_Endpoint);
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

					if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
					{
						OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&Walking_Endpoint, pLineEndpoint);
						Walking_Endpoint.Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;
						Walking_Endpoint.u.sonet.iV=0;
						Walking_Endpoint.u.sonet.iW=0;

						Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, &Walking_Endpoint);
						if(WPX_UFE_FRAMER_OK==Result)
						{
							pDeviceHO_ParentPathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, &Walking_Endpoint);

							if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_Exists(pDeviceHO_ParentPathRAM))
							{
								Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_DOES_NOT_EXIST;
							}
							else
							{
								if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
								{
    								Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
								}
							}
						}
					}
					else
					{
						if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
						{
    						Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
						}
					}
				}
			}
			else
			{
				if(!OMIINO_FRAMER_SONET_SDH_HO_Facility_IsFree(pDeviceHO_ParentPathRAM))
				{
    				Result = WPX_UFE_FRAMER_ERROR_FACILITY_PARENT_IS_CONNECTED;
				}
			}
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
        
		switch(pDeviceLO_PathRAM->Configuration.FacilityState)
        {
            case    WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE   :
                    Result=WPX_UFE_FRAMER_OK;
                    break;

            case    WPX_UFE_FRAMER_FACILITY_STATE_CONNECTED      :
                    Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
                    break;

            default:
				OMIINO_FRAMER_RSE(pDeviceLO_PathRAM->Configuration.FacilityState);
                    break;
        }
       
    }

    return Result;
}




