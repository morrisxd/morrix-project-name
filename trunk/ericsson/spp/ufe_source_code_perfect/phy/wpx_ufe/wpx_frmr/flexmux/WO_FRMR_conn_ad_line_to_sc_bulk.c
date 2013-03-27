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


	



U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection_SDH(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 BuildPersonality;
	U8 DeviceProtectionMode;
	int iElement;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;

	OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, &BuildPersonality);

	pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, &DeviceProtectionMode);

	LineEndpoint.Bandwidth=Bandwidth;
	LineEndpoint.iLinePort=iLinePort;

	for(iElement=0;iElement<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iElement++)
	{
		if(WPX_UFE_FRAMER_OK==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].ErrorCode)
		{
			if(iLinePort==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpectedPort)
			{
				LineEndpoint.u.sdh.iJ=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_J;
				LineEndpoint.u.sdh.iK=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_K;
				LineEndpoint.u.sdh.iL=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_L;
				LineEndpoint.u.sdh.iM=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_M;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, &LineEndpoint, iElement);
			}
		}
	}
	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection_SONET(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 BuildPersonality;
	U8 DeviceProtectionMode;
	int iElement;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;

	OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, &BuildPersonality);

	pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, &DeviceProtectionMode);

	LineEndpoint.Bandwidth=Bandwidth;
	LineEndpoint.iLinePort=iLinePort;

	for(iElement=0;iElement<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iElement++)
	{
		if(WPX_UFE_FRAMER_OK==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].ErrorCode)
		{
			if(iLinePort==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpectedPort)
			{
				LineEndpoint.u.sonet.iU=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_U;
				LineEndpoint.u.sonet.iV=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_V;
				LineEndpoint.u.sonet.iW=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_W;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, &LineEndpoint, iElement);
			}
		}
	}
	return Result;
}





U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection_SDH(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 BuildPersonality;
	U8 DeviceProtectionMode;
	int iElement;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;

	OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, &BuildPersonality);

	pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, &DeviceProtectionMode);

	LineEndpoint.Bandwidth=Bandwidth;
	LineEndpoint.iLinePort=iLinePort;

	for(iElement=0;iElement<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iElement++)
	{
		if(WPX_UFE_FRAMER_OK==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].ErrorCode)
		{
			if(iLinePort==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpectedPort)
			{
				LineEndpoint.u.sdh.iJ=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_J;
				LineEndpoint.u.sdh.iK=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_K;
				LineEndpoint.u.sdh.iL=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_L;
				LineEndpoint.u.sdh.iM=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_M;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, &LineEndpoint, iElement);
			}
		}
	}
	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection_SONET(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 BuildPersonality;
	U8 DeviceProtectionMode;
	int iElement;
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;

	OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, &BuildPersonality);

	pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, &DeviceProtectionMode);

	LineEndpoint.Bandwidth=Bandwidth;
	LineEndpoint.iLinePort=iLinePort;

	for(iElement=0;iElement<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iElement++)
	{
		if(WPX_UFE_FRAMER_OK==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].ErrorCode)
		{
			if(iLinePort==OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpectedPort)
			{
				LineEndpoint.u.sonet.iU=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_U;
				LineEndpoint.u.sonet.iV=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_V;
				LineEndpoint.u.sonet.iW=OMIINO_RAM.SocketDynamicRule[BuildPersonality][DeviceProtectionMode].Element[iElement].iExpected_W;
				OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, &LineEndpoint, iElement);
			}
		}
	}
	return Result;
}

