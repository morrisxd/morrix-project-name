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




void OMIINO_FRAMER_CONNECTIONS_MakeUnProtectedDiscreteClientToClientSocket(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, U8 iDiscreteClient, U32 iSocketClient)
{
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	        * pClient;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE                    * pDiscretePortRAM;

    pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClient);

    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pClient->Connection.ConnectionType,pClient->Connection.ConnectionType);

    pClient->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_DISCRETE_CLIENT_TO_SOCKET_CLIENT;
    pClient->Connection.u.UnProtectedDiscreteClientToSocketClient.iDiscreteClient=iDiscreteClient;
    OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_MakeConnected(pClient);


    pDiscretePortRAM = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClient);

    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_NONE==pDiscretePortRAM->Connection.ConnectionType,pDiscretePortRAM->Connection.ConnectionType);

    pDiscretePortRAM->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_DISCRETE_CLIENT_TO_SOCKET_CLIENT;
    pDiscretePortRAM->Connection.u.UnProtectedDiscreteClientToSocketClient.iSocketClient=iSocketClient;
    OMIINO_FRAMER_DISCRETE_CLIENT_Facility_MakeConnected(pDiscretePortRAM);
}




void OMIINO_FRAMER_CONNECTIONS_BreakUnProtectedDiscreteClientToClientSocket(OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM, U8 iDiscreteClient, U32 iSocketClient)
{
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	        * pClient;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE                    * pDiscretePortRAM;

    pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClient);

    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_DISCRETE_CLIENT_TO_SOCKET_CLIENT==pClient->Connection.ConnectionType,pClient->Connection.ConnectionType);

    pClient->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
    OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_MakeUnConnected(pClient);


    pDiscretePortRAM = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClient);

    OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CONNECTION_TYPE_DISCRETE_CLIENT_TO_SOCKET_CLIENT==pDiscretePortRAM->Connection.ConnectionType,pDiscretePortRAM->Connection.ConnectionType);

    pDiscretePortRAM->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
    OMIINO_FRAMER_DISCRETE_CLIENT_Facility_MakeUnConnected(pDiscretePortRAM);
}

