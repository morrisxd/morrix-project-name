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




U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_SOCKET_CLIENT(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF);
	
	return WPX_UFE_FRAMER_OK;
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_SOCKET_CLIENT(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE_SDH(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LOS   );	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LOF   );	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_RS_TIM);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_AIS);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_RDI);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_OOF);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_EXC);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_DEG);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_RS_EXC);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_RS_DEG);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE_SONET(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOS	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOF	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_TIM_S);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_AIS_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_RDI_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_OOF);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_S);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_S);

	return WPX_UFE_FRAMER_OK;
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE_SDH(TransactionIdentifier, iDevice);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE_SONET(TransactionIdentifier, iDevice);
				break;

		default:
				/* Ignore */
				break;
	}
	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE_SDH(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LOS);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LOF);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_RS_TIM);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_AIS);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_RDI);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_OOF);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_EXC);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_MS_DEG);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_AU_AIS);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_AU_LOP);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE_SONET(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOS	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOF	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_TIM_S);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_AIS_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_RDI_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_OOF);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_L);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_L);

	return WPX_UFE_FRAMER_OK;
}




U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE_SDH(TransactionIdentifier, iDevice);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE_SONET(TransactionIdentifier, iDevice);
				break;

		default:
				/* Ignore */
				break;
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO_SDH(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_LOM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_TIM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_PLM	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_UNEQ);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_RDI	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_EXC	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_DEG	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_AU_AIS);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_AU_LOP);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO_SONET(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOM_P	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_TIM_P	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_PLM_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_UNEQ_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_RDI_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_AIS_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOP_P	);

	return WPX_UFE_FRAMER_OK;
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO_SDH(TransactionIdentifier, iDevice);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO_SONET(TransactionIdentifier, iDevice);
				break;

		default:
				/* Ignore */
				break;
	}
	return Result;
}






U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO_SDH(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_LOM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_TIM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_PLM	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_UNEQ	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_RDI	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_EXC	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_HP_DEG	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_AU_AIS    );
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_AU_LOP    );

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO_SONET(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOM_P	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_TIM_P	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_PLM_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_UNEQ_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_RDI_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_AIS_P	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOP_P	);

	return WPX_UFE_FRAMER_OK;
}




U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO_SDH(TransactionIdentifier, iDevice);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO_SONET(TransactionIdentifier, iDevice);
				break;

		default:
				/* Ignore */
				break;
	}
	return Result;
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO_SDH(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_TIM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_PLM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_UNEQ);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_RDI	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_EXC	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_DEG	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_TU_AIS	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_TU_LOP	);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO_SONET(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_TIM_V	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_PLM_V	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_UNEQ_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_RDI_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_AIS_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOP_V	);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO_SDH(TransactionIdentifier, iDevice);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO_SONET(TransactionIdentifier, iDevice);
				break;

		default:
				/* Ignore */
				break;
	}
	return Result;
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO_SDH(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_TIM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_PLM	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_UNEQ	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_RDI	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_EXC	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_LP_DEG	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_TU_AIS	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SDH_TU_LOP	);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO_SONET(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_TIM_V	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_PLM_V	);	
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_UNEQ_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_RDI_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_EXC_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_DEG_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_AIS_V	);
	OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, WPX_UFE_FRAMER_SONET_LOP_V	);

	return WPX_UFE_FRAMER_OK;
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO_SDH(TransactionIdentifier, iDevice);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO_SONET(TransactionIdentifier, iDevice);
				break;

		default:
				/* Ignore */
				break;
	}
	return Result;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_ALL(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE_SDH(TransactionIdentifier, iDevice);
	OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO(TransactionIdentifier, iDevice);
	OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO(TransactionIdentifier, iDevice);
	OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_SOCKET_CLIENT(TransactionIdentifier, iDevice);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_ALL(U32 TransactionIdentifier, U8 iDevice)
{
	OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE_SDH(TransactionIdentifier, iDevice);
	OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO(TransactionIdentifier, iDevice);
	OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO(TransactionIdentifier, iDevice);
	OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_SOCKET_CLIENT(TransactionIdentifier, iDevice);

	return WPX_UFE_FRAMER_OK;
}




void OMIINO_API_INTERNAL_BULK_SDH_ALARM_FetchReportingStateForAll(U8 * pReportingState)
{
	U8 i;

	for(i=0;i<WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;i++)
	{
		OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GetAlarmReportingForAlarmCategory(i, &(pReportingState[i]));
	}
}



void OMIINO_API_INTERNAL_BULK_SDH_ALARM_SetReportingStateForAll(U8 * pNewReportingState)
{
	U8 i;

	for(i=0;i<WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES;i++)
	{
		if(1==pNewReportingState[i])
		{
			OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(0, i);
		}
		else
		{
			OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(0, i);
		}
	}
}



U8 OMIINO_API_INTERNAL_BULK_ALARM_LIST_ALL_ACTIVE(U8 iDevice)
{
	U32 iWalkingNodeKey;

	for(iWalkingNodeKey=0;iWalkingNodeKey<MAX_ALARM_MANAGER_NODE_ELEMENTS;iWalkingNodeKey++)
	{
		if(OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[iWalkingNodeKey].IsActive)
		{
			if(OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[iWalkingNodeKey].IsAsserted)
			{
				OMIINO_FRAMER_ALARM_MANAGER_ALARM_ANNOUNCE_CHANGE(iDevice, 1, &(OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[iWalkingNodeKey]));
			}
		}
	}

	return WPX_UFE_FRAMER_OK;
}

