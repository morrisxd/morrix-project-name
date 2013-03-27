/*
 *        Copyright (c) 2010  Omiino Ltd
 *
 *        All rights reserved.
 *        This code is provided under license and or Non-disclosure Agreement
 *        and must be used solely for the purpose for which it was provided.
 *        It must not be passed to any third party without the written
 *        permission of Omiino Ltd.
 *
 */



#include "WO_FRMR_private.h"




void OMIINO_FRAMER_CONNECTIONS_MakeThrough(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint)
{
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pHO_Path=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pLO_Path=NULL;


    if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pFirstLineEndpoint->Bandwidth))
	{
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pFirstLineEndpoint->iLinePort);

		pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pFirstLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pHO_Path->Configuration.Connection.ConnectionType,pHO_Path->Configuration.Connection.ConnectionType);

        pHO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH;
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&(pHO_Path->Configuration.Connection.u.Through.LineEndpoint),pSecondLineEndpoint);
        
        OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeConnected(pHO_Path);


        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pSecondLineEndpoint->iLinePort);
		pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pSecondLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pHO_Path->Configuration.Connection.ConnectionType,pHO_Path->Configuration.Connection.ConnectionType);
      
        pHO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH;
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&(pHO_Path->Configuration.Connection.u.Through.LineEndpoint),pFirstLineEndpoint);
        OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeConnected(pHO_Path);

	}
	else
	{
        pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pFirstLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pLO_Path->Configuration.Connection.ConnectionType,pLO_Path->Configuration.Connection.ConnectionType);

        pLO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH;
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&(pLO_Path->Configuration.Connection.u.Through.LineEndpoint),pSecondLineEndpoint);
        OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeConnected(pLO_Path);

        pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pSecondLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pLO_Path->Configuration.Connection.ConnectionType,pLO_Path->Configuration.Connection.ConnectionType);

        pLO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH;
        OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&(pLO_Path->Configuration.Connection.u.Through.LineEndpoint),pFirstLineEndpoint);
        OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeConnected(pLO_Path);
	}

}

void OMIINO_FRAMER_CONNECTIONS_BreakThrough(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint)
{
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pHO_Path=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pLO_Path=NULL;


    if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pFirstLineEndpoint->Bandwidth))
	{
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pFirstLineEndpoint->iLinePort);
		pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pFirstLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH==pHO_Path->Configuration.Connection.ConnectionType,pHO_Path->Configuration.Connection.ConnectionType);
        
        pHO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
        OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeUnConnected(pHO_Path);


        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pSecondLineEndpoint->iLinePort);
		pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pSecondLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH==pHO_Path->Configuration.Connection.ConnectionType,pHO_Path->Configuration.Connection.ConnectionType);

        pHO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
        OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeUnConnected(pHO_Path);
	}
	else
	{
        pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pFirstLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH==pLO_Path->Configuration.Connection.ConnectionType,pLO_Path->Configuration.Connection.ConnectionType);

        pLO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
        OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeUnConnected(pLO_Path);

        pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pSecondLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_THROUGH==pLO_Path->Configuration.Connection.ConnectionType,pLO_Path->Configuration.Connection.ConnectionType);

        pLO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
        OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeUnConnected(pLO_Path);
	}

}

