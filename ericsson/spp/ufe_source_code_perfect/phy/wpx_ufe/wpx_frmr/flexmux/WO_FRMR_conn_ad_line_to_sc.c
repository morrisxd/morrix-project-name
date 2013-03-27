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




void OMIINO_FRAMER_CONNECTIONS_MakeUnProtectedLineToClientSocket(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint, U32 iSocketClient)
{
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pHO_Path=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pLO_Path=NULL;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE		* pClient;


    if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
	{
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pHO_Path->Configuration.Connection.ConnectionType,pHO_Path->Configuration.Connection.ConnectionType);

        pHO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_LINE_TO_SOCKET_CLIENT;
        pHO_Path->Configuration.Connection.u.UnProtectedLineToSocketClient.iSocketClient=iSocketClient;
        OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeConnected(pHO_Path);
	}
	else
	{
        pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pLO_Path->Configuration.Connection.ConnectionType,pLO_Path->Configuration.Connection.ConnectionType);

        pLO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_LINE_TO_SOCKET_CLIENT;
        pLO_Path->Configuration.Connection.u.UnProtectedLineToSocketClient.iSocketClient=iSocketClient;
        OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeConnected(pLO_Path);

	}

    pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClient);

    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pClient->Connection.ConnectionType,pClient->Connection.ConnectionType);

    pClient->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_LINE_TO_SOCKET_CLIENT;
    OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&(pClient->Connection.u.UnProtectedLineToSocketClient.LineEndpoint),pLineEndpoint);
    OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_MakeConnected(pClient);

}




void OMIINO_FRAMER_CONNECTIONS_BreakUnProtectedLineToClientSocket(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint, U32 iSocketClient)
{
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pHO_Path=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pLO_Path=NULL;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE		* pClient;


    if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
	{
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pHO_Path = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_LINE_TO_SOCKET_CLIENT==pHO_Path->Configuration.Connection.ConnectionType,pHO_Path->Configuration.Connection.ConnectionType);

        pHO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
        OMIINO_FRAMER_SONET_SDH_HO_Facility_MakeUnConnected(pHO_Path);
	}
	else
	{
        pLO_Path = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

        OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_LINE_TO_SOCKET_CLIENT==pLO_Path->Configuration.Connection.ConnectionType,pLO_Path->Configuration.Connection.ConnectionType);

        pLO_Path->Configuration.Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
        OMIINO_FRAMER_SONET_SDH_LO_Facility_MakeUnConnected(pLO_Path);
	}

    pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClient);

    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_LINE_TO_SOCKET_CLIENT==pClient->Connection.ConnectionType,pClient->Connection.ConnectionType);

    pClient->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
    OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_MakeUnConnected(pClient);

}

