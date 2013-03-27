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





/*
 *
 * Section: Initialization 
 *
 */



void WPX_UFE_FRAMER_Initialize(		WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE                         (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES], 
									WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE			*		pEnvironmentTable,
									WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE				*		pAutonomousOutputTable,
									WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE	*		pPerformanceMonitoringDeviceTable,
									WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE       *       pDeviceLevelDiagnosticTable,
									WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE                       *       pOmniSpyTable,
									U8																TestMode)
{
    OMIINO_Product_Initialize(  pMailboxMemoryMap,
                                pEnvironmentTable,
                                pAutonomousOutputTable,
                                pPerformanceMonitoringDeviceTable,
                                pDeviceLevelDiagnosticTable,
                                pOmniSpyTable,
								TestMode);
}






void WPX_UFE_FRAMER_ENTER(U32 iAPI)
{
    OMIINO_FRAMER_ENVIRONMENT_WRAPPER_EnterCriticalRegion(OMIINO_FRAMER_DeviceDriverHandleGet());
	OMIINO_HOUSEKEEPING_ENTRY(iAPI);
}



void WPX_UFE_FRAMER_EXIT(U32 iAPI)
{
    OMIINO_HOUSEKEEPING_EXIT(iAPI);
    OMIINO_FRAMER_ENVIRONMENT_WRAPPER_LeaveCriticalRegion(OMIINO_FRAMER_DeviceDriverHandleGet());
}



/*
 *
 * Section: Device Protection API
 *
 */

U8 WPX_FRMR_SetDevProtectMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceProtectionMode)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_SET_DEVICE_PROTECTION_MODE);
	Result=OMIINO_API_INTERNAL_SetDeviceProtectionMode(TransactionIdentifier, iDevice, DeviceProtectionMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_SET_DEVICE_PROTECTION_MODE);
	
	return Result;
}


U8 WPX_FRMR_GetDevProtectMode(U8 iDevice, U8 * pDeviceProtectionMode)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_GET_DEVICE_PROTECTION_MODE);
	Result=OMIINO_API_INTERNAL_GetDeviceProtectionMode(iDevice, pDeviceProtectionMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_GET_DEVICE_PROTECTION_MODE);
	
	return Result;
}






U8 WPX_FRMR_SetDevCardProtectOperatingMode(U32 TransactionIdentifier, U8 iDevice, U8 CardProtectionOperatingMode)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_SET_DEVICE_CARD_PROTECTION_OPERATION_MODE);
	Result=OMIINO_API_INTERNAL_SetDeviceCardProtectionOperatingMode(TransactionIdentifier, iDevice, CardProtectionOperatingMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_SET_DEVICE_CARD_PROTECTION_OPERATION_MODE);
	
	return Result;
}


U8 WPX_FRMR_GetDevCardProtectOperatingMode(U8 iDevice, U8 * pCardProtectionOperatingMode)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_GET_DEVICE_CARD_PROTECTION_OPERATION_MODE);
	Result=OMIINO_API_INTERNAL_GetDeviceCardProtectionOperatingMode(iDevice, pCardProtectionOperatingMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_GET_DEVICE_CARD_PROTECTION_OPERATION_MODE);
	
	return Result;
}






/*
 *
 * Section: Low Level Comms Permission API
 *
 */

U8 WPX_FRMR_LowLevelCommsEnableAccess(U8 iDevice)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_LOW_LEVEL_COMMS_ENABLE_ACCESS);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_LOW_LEVEL_COMMS_ENABLE_ACCESS(iDevice);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_LOW_LEVEL_COMMS_ENABLE_ACCESS);
	
	return Result;
}



U8 WPX_FRMR_LowLevelCommsDisableAccess(U8 iDevice)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_LOW_LEVEL_COMMS_DISABLE_ACCESS);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_LOW_LEVEL_COMMS_DISABLE_ACCESS(iDevice);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_LOW_LEVEL_COMMS_DISABLE_ACCESS);
	
	return Result;
}


/*
 *
 * Section: OmniSpy API
 *
 */

U8 WPX_FRMR_OMNISPY_SouthboundMessage(U8 iDevice, U32 * pMessage, U8 Length)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_SERVICE_SOUTHBOUND_LOWLEVEL_COMMS);
    Result=OMIINO_API_INTERNAL_OMNISPY_SouthboundMessage(iDevice, pMessage, Length);
    WPX_UFE_FRAMER_EXIT(API_SERVICE_SOUTHBOUND_LOWLEVEL_COMMS);

    return Result;
}





/*
 *
 * Section: CONFIGURATION PERFORMANCE MONITORING
 *
 */




U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_GetMonitoringState(U8 iDevice, U8 iLinePort, U8 iPoint, U8 *pState)
{
    U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_PORT_GET_MONITORING);
    Result=WPX_FRMR_PL_PM_SONET_SDH_Port_GetMonitoringState(iDevice, iLinePort, iPoint, pState);
    WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_PORT_GET_MONITORING);

    return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U8 *pState)
{
    U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_HIGH_ORDER_PATH_GET_MONITORING);
        Result=WPX_FRMR_PL_PM_SONET_SDH_HO_Path_GetMonitoringState(iDevice, &LineEndpoint, iPoint, pState);
        WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_HIGH_ORDER_PATH_GET_MONITORING);
    }

    return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U8 *pState)
{
    U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_LOW_ORDER_PATH_GET_MONITORING);
        Result=WPX_FRMR_PL_PM_SONET_SDH_LO_Path_GetMonitoringState(iDevice, &LineEndpoint, iPoint, pState);
        WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_LOW_ORDER_PATH_GET_MONITORING);
    }

    return Result;
}


U8 WPX_FRMR_PM_E1_GetMonitoringCrc(U8 iDevice, U32 iSocketClientPDH, U8 *pState)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_E1_GET_MONITORING_CRC);
    Result=OMIINO_API_INTERNAL_PM_E1_GET_MONITORING_CRC_State(iDevice, iSocketClientPDH, pState);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_E1_GET_MONITORING_CRC);

    return Result;
}


U8 WPX_FRMR_PM_T1_GetMonitoringCrc(U8 iDevice, U32 iSocketClientPDH, U8 *pState)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_T1_GET_MONITORING_CRC);
    Result=OMIINO_API_INTERNAL_PM_T1_GET_MONITORING_CRC_State(iDevice, iSocketClientPDH, pState);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_T1_GET_MONITORING_CRC);

    return Result;
}









U8 WPX_FRMR_PM_SONET_SDH_Port_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint)
{
    U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_PORT_ENABLE_MONITORING);
    Result=WPX_FRMR_PL_PM_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iDevice, iLinePort, iPoint);
    WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_PORT_ENABLE_MONITORING);

    return Result;
}

U8 WPX_FRMR_PM_SONET_SDH_Port_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_PORT_DISABLE_MONITORING);
    Result=WPX_FRMR_PL_PM_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iDevice, iLinePort, iPoint);
    WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_PORT_DISABLE_MONITORING);

    return Result;
}



U8 WPX_FRMR_PM_SONET_SDH_HO_Path_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint)
{
    U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING);
        Result=WPX_FRMR_PL_PM_SONET_SDH_HO_Path_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING);
    }

    return Result;
}



U8 WPX_FRMR_PM_SONET_SDH_HO_Path_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint)
{
    U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING);
        Result=WPX_FRMR_PL_PM_SONET_SDH_HO_Path_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING);
    }

    return Result;
}




U8 WPX_FRMR_PM_SONET_SDH_LO_Path_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint)
{
    U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING);
        Result=WPX_FRMR_PL_PM_SONET_SDH_LO_Path_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING);
    }

    return Result;
}




U8 WPX_FRMR_PM_SONET_SDH_LO_Path_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint)
{
    U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING);
        Result=WPX_FRMR_PL_PM_SONET_SDH_LO_Path_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING);
    }

    return Result;
}



U8 WPX_FRMR_PM_E1_EnableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_E1_ENABLE_MONITORING_CRC);
    Result=OMIINO_API_INTERNAL_PM_E1_ENABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_E1_ENABLE_MONITORING_CRC);

    return Result;
}



U8 WPX_FRMR_PM_E1_DisableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_E1_DISABLE_MONITORING_CRC);
    Result=OMIINO_API_INTERNAL_PM_E1_DISABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_E1_DISABLE_MONITORING_CRC);

    return Result;
}

U8 WPX_FRMR_PM_E3T3_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 iPoint)
{
   U8 Result;
   
   WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_E3T3_ENABLE_MONITORING);
   Result=OMIINO_API_INTERNAL_PM_E3T3_ENABLE_MONITORING(TransactionIdentifier, iDevice, iSocketClientPDH, iPoint);
   WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_E3T3_ENABLE_MONITORING);
   
   return Result;
}



U8 WPX_FRMR_PM_E3T3_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 iPoint)
{
   U8 Result;
   
   WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_E3T3_DISABLE_MONITORING);
   Result=OMIINO_API_INTERNAL_PM_E3T3_DISABLE_MONITORING(TransactionIdentifier, iDevice, iSocketClientPDH, iPoint);
   WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_E3T3_DISABLE_MONITORING);
   
   return Result;
}

U8 WPX_FRMR_PM_E3T3_GetMonitoring(U8 iDevice, U32 iSocketClientPDH, U8 iPoint, U8 *pState)
{
   U8 Result;
   
   WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_E3T3_GET_MONITORING);
   Result=OMIINO_API_INTERNAL_PM_E3T3_GET_MONITORING_State(iDevice, iSocketClientPDH, iPoint, pState);
   WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_E3T3_GET_MONITORING);
   
   return Result;
}



U8 WPX_FRMR_PM_T1_EnableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_T1_ENABLE_MONITORING_CRC);
    Result=OMIINO_API_INTERNAL_PM_T1_ENABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_T1_ENABLE_MONITORING_CRC);

    return Result;
}





U8 WPX_FRMR_PM_T1_DisableMonitoringCrc(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_PM_T1_DISABLE_MONITORING_CRC);
    Result=OMIINO_API_INTERNAL_PM_T1_DISABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_PM_T1_DISABLE_MONITORING_CRC);

    return Result;
}





U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_DISCRETE_PORT_ENABLE_MONITORING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring(TransactionIdentifier, iDevice, iDiscreteClientPort, iPoint);
    WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_DISCRETE_PORT_ENABLE_MONITORING);

    return Result;
}




U8 WPX_UFE_FRAMER_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_PM_SONET_SDH_DISCRETE_PORT_DISABLE_MONITORING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring(TransactionIdentifier, iDevice, iDiscreteClientPort, iPoint);
    WPX_UFE_FRAMER_EXIT(API_PM_SONET_SDH_DISCRETE_PORT_DISABLE_MONITORING);

    return Result;
}







/*
 *
 * Section: DEVICE 
 *
 */


U8  WPX_UFE_FRAMER_DeviceIsReady(U8 iDevice)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_DEVICE_IS_READY);
    Result=OMIINO_API_INTERNAL_DeviceIsReady(iDevice);
    WPX_UFE_FRAMER_EXIT(API_DEVICE_IS_READY);
	
    return Result;
}



U8 WPX_FRMR_ServiceNorthboundLowLevelComms(U8 iDevice)
{
	U8 Result;
    
	WPX_UFE_FRAMER_ENTER(API_SERVICE_NORTHBOUND_LOWLEVEL_COMMS);
	Result=OMIINO_API_INTERNAL_ServiceNorthboundLowLevelComms(iDevice);    
	WPX_UFE_FRAMER_EXIT(API_SERVICE_NORTHBOUND_LOWLEVEL_COMMS);
	
    return Result;
}




/* 
 *
 * NB Do not acquire semaphore as this routine is designed to be
 * called from within an ISR
 *
 */

void WPX_UFE_FRAMER_ISR(U8 iDevice)
{
	OMIINO_API_INTERNAL_ISR(iDevice);
}





/*
 *
 * Section: CONFIGURATION DEVICE
 *
 */



U8 WPX_FRMR_DEV_SetBuildPersonality(U8 iDevice, U8 BuildPersonality)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_SET_BUILD_PERSONALITY);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_SetBuildPersonality(iDevice, BuildPersonality);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_SET_BUILD_PERSONALITY);

	return Result;
}



U8 WPX_FRMR_DEV_GetBuildPersonality(U8 iDevice, U8 * pBuildPersonality)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_GET_BUILD_PERSONALITY);
	Result= OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_GetBuildPersonality(iDevice, pBuildPersonality);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_GET_BUILD_PERSONALITY);

	return Result;
}




U8 WPX_FRMR_DEV_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceMode)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_SET_MODE);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_SetMode(TransactionIdentifier, iDevice, DeviceMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_SET_MODE);
	
	return Result;
}




U8 WPX_FRMR_DEV_GetMode(U8 iDevice, U8 * pDeviceMode)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_GET_MODE);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_GetMode(iDevice, pDeviceMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_GET_MODE);

	return Result;
}





U8 WPX_FRMR_DEV_Enable_AIS_Insertion(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_SET_MODE);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Enable_AIS_Insertion(TransactionIdentifier, iDevice);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_SET_MODE);
	
	return Result;

}

U8 WPX_FRMR_DEV_Disable_AIS_Insertion(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_SET_MODE);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Disable_AIS_Insertion(TransactionIdentifier, iDevice);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_SET_MODE);
	
	return Result;
}

U8 WPX_FRMR_DEV_Get_AIS_Insertion(U8 iDevice, U8 * pAIS_InsertionMode)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_GET_MODE);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Get_AIS_Insertion(iDevice, pAIS_InsertionMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_GET_MODE);

	return Result;
}





U8 WPX_FRMR_DEV_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iLinePort)
{
    U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET);
	Result=WPX_FRMR_PL_DEV_DIAG_DRV_RecClkSet(TransactionIdentifier, iDevice, OutputSelector, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET);

	return Result;
}

U8 WPX_FRMR_DEV_Squelch_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 Squelch_Mode)
{
    U8 Result;
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK_SET);
	Result=WPX_FRMR_PL_DEV_SQUELCH_RecClkSet(TransactionIdentifier, iDevice, OutputSelector, Squelch_Mode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK_SET);
	return Result;
}


U8 WPX_FRMR_DEV_DIAG_DRV_RecClkGet(U8 iDevice, U8 OutputSelector, U8 * piLinePort)
{
    U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET);

	*piLinePort=0;

	Result=WPX_FRMR_PL_DEV_DIAG_DRV_RecClkGet(iDevice, OutputSelector, piLinePort);

	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET);

	return Result;
}

U8 WPX_FRMR_DEV_Squelch_RecClkGet(U8 iDevice, U8 OutputSelector, U8* pSquelch_Mode)
{
    U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK_GET);

	Result=WPX_FRMR_PL_DEV_SQUELCH_RecClkGet(iDevice, OutputSelector, pSquelch_Mode);

	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK_GET);

	return Result;
}







/*
 *
 * Section: CONFIGURATION FACILITIES
 *
 */


U8 WPX_FRMR_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
	
    
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY);
	    Result=WPX_FRMR_PL_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY);
    }

	return Result;
}






U8 WPX_FRMR_SOCKET_PDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClientType)
{
	U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(TransactionIdentifier, iDevice, iSocketClientPDH, ClientType);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY);
	

	return Result;
}




U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClientType)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(TransactionIdentifier, iDevice, iDiscreteClientPort, ClientType);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY);

	return Result;
}



U8 WPX_FRMR_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
	
    
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY);
	    Result=WPX_FRMR_PL_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY);
    }

	return Result;
}




U8 WPX_FRMR_SOCKET_PDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY);
	
	return Result;
}








U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY);
	Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY);

	return Result;
}



/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT
 *
 */



U8 WPX_FRMR_SONET_SDH_PORT_SFP_SetTxState(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Tx_State)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SFP_SetTxState(TransactionIdentifier, iDevice, iLinePort, Tx_State);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState);

	return Result;
}

U8 WPX_FRMR_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SetRate(TransactionIdentifier, iDevice, iLinePort, PortRate,WPX_UFE_FRAMER_SFP_TX_ENABLE);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE);

	return Result;
}

U8 WPX_FRMR_SONET_SDH_PORT_SetRate_Without_Tx_Enable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SetRate(TransactionIdentifier, iDevice, iLinePort, PortRate,WPX_UFE_FRAMER_SFP_NO_TX_ENABLE);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE);

	return Result;
}

U8 WPX_FRMR_SONET_SDH_PORT_GetRate(U8 iDevice, U8 iLinePort, U8 * pPortRate)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_RATE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_GetRate(iDevice, iLinePort, pPortRate);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_RATE);
	
	return Result;
}






U8 WPX_FRMR_SONET_SDH_PORT_DCC_Enable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 DCC_Mode)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_DCC_Enable(TransactionIdentifier, iDevice, iLinePort, DCC_Mode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE);

	return Result;
}




U8 WPX_FRMR_SONET_SDH_PORT_DCC_Disable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 DCC_Mode)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_DCC_Disable(TransactionIdentifier, iDevice, iLinePort, DCC_Mode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE);

	return Result;
}





U8 WPX_FRMR_SONET_SDH_PORT_DCC_Get(U8 iDevice, U8 iLinePort, U8 DCC_Mode, U8 * pDCC_State)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_DCC_Get(iDevice, iLinePort, DCC_Mode, pDCC_State);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET);
	
	return Result;
}





/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT SECTION
 *
 */




U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Mode)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_SetMode(TransactionIdentifier, iDevice, iLinePort, Mode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE);

	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_GetMode(U8 iDevice, U8 iLinePort, U8 * pMode)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_MODE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_GetMode(iDevice, iLinePort, pMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_MODE);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pTX_TraceString)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX);
    Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_SetTX(TransactionIdentifier, iDevice, iLinePort, pTX_TraceString);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX);

	return Result;
}


U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_SetEX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pEX_TraceString)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_SetEX(TransactionIdentifier, iDevice, iLinePort, pEX_TraceString);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX);
	
	return Result;
}




U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_GetTX(U8 iDevice, U8 iLinePort, char * pTX_TraceString)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_TX);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_GetTX(iDevice, iLinePort, pTX_TraceString);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_TX);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_SEC_J0_GetEX(U8 iDevice, U8 iLinePort, char * pEX_TraceString)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_EX);
    Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_GetEX(iDevice, iLinePort, pEX_TraceString);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_EX);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_FAIL_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(TransactionIdentifier, iDevice, iLinePort, Threshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_FAIL_THRESHOLD);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result;
	

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(TransactionIdentifier, iDevice, iLinePort, Threshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD);
	
	return Result;
}


U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_GetExcessiveTh(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_GetExcessiveTh(iDevice, iLinePort, pThreshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_SEC_B1_GetSigDegradeTh(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_GetSigDegradeTh(iDevice, iLinePort, pThreshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD);

	return Result;
}





/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT LINE
 *
 */




U8 WPX_FRMR_SONET_SDH_PORT_LINE_K1K2_SetTx(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	U8 Result;
	

	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SET_TX);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_K1K2_SetTx(TransactionIdentifier, iDevice, iLinePort, pTX_K1K2);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SET_TX);

	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_LINE_K1K2_GetTx(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_TX);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_K1K2_GetTx(iDevice, iLinePort, pTX_K1K2);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_TX);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_LINE_S1_SetTx(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 TX_S1)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SET_TX);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_S1_SetTx(TransactionIdentifier, iDevice, iLinePort, TX_S1);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SET_TX);

	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_LINE_S1_GetTx(U8 iDevice, U8 iLinePort, U8 * pTX_S1)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_TX);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_S1_GetTx(iDevice, iLinePort, pTX_S1);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_TX);

	return Result;
}




U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_FAIL_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(TransactionIdentifier, iDevice, iLinePort, Threshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_FAIL_THRESHOLD);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(TransactionIdentifier, iDevice, iLinePort, Threshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD);
	
	return Result;
}


U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_GetExcessiveTh(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_FAIL_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_GetExcessiveTh(iDevice, iLinePort, pThreshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_FAIL_THRESHOLD);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_LINE_B2_GetSigDegradeTh(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_DEGRADE_THRESHOLD);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_GetSigDegradeTh(iDevice, iLinePort, pThreshold);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_DEGRADE_THRESHOLD);

	return Result;
}







/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH HO PATH
 *
 */



U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_SetMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Mode)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_SetMode(TransactionIdentifier, iDevice, &LineEndpoint, Mode);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE);
    }
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_GetMode(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pMode)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_MODE);
        Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_GetMode(iDevice, &LineEndpoint, pMode);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_MODE);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, pTX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, pEX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX);
    }
	
	return Result;
}




U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_TX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_GetTX(iDevice, &LineEndpoint, pTX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_TX);
    }
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_J1_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_EX);
        Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_GetEX(iDevice, &LineEndpoint, pEX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_EX);
    }


	return Result;
}


U8 WPX_FRMR_SONET_SDH_HO_PATH_SetRdiMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 ErdiMode)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_ERDI_SetRdiMode);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_SetRdiMode(TransactionIdentifier, iDevice, &LineEndpoint, ErdiMode);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_ERDI_SetRdiMode);
    }


	return Result;
}

U8 WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Set_Tx(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Value)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_SS_BITS_SET_TX(TransactionIdentifier, iDevice, &LineEndpoint, Value);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX);
    }


	return Result;
}
U8 WPX_FRMR_SONET_SDH_HO_PATH_SS_BITS_Get_Rx(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_SS_BITS_GET_RX(TransactionIdentifier, iDevice, &LineEndpoint);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX);
    }


	return Result;
}

U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 TX_C2)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, TX_C2);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX);
    }


	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 EX_C2)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, EX_C2);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX);
    }


	return Result;
}


U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pTX_C2)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_TX);
        Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_GetTX(iDevice, &LineEndpoint, pTX_C2);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_TX);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_C2_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pEX_C2)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_EX);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_GetEX(iDevice, &LineEndpoint, pEX_C2);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_EX);
    }

	return Result;
}










U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_FAIL_THRESHOLD);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_SetExcessiveTh(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_FAIL_THRESHOLD);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD);
    }

	return Result;
}




U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_GetExcessiveTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_GetExcessiveTh(iDevice, &LineEndpoint, pThreshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD);
    }

	return Result;
}


U8 WPX_FRMR_SONET_SDH_HO_PATH_B3_GetSigDegradeTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
	
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD);
	    Result=WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_GetSigDegradeTh(iDevice, &LineEndpoint, pThreshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD);
    }

	return Result;
}







/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH LO PATH
 *
 */


U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, pTX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX);
    }

	return Result;
}


U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
	
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, pEX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX);
    }

	return Result;
}


U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pTX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_TX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_GetTX(iDevice, &LineEndpoint, pTX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_TX);
    }
	
	return Result;
}


U8 WPX_FRMR_SONET_SDH_LO_PATH_J2_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pEX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_EX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_GetEX(iDevice, &LineEndpoint, pEX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_EX);
    }

    return Result;
}




U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 TX_V5_SignalLabel)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, TX_V5_SignalLabel);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 EX_V5_SignalLabel)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, EX_V5_SignalLabel);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX);
    }

	return Result;
}


U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pTX_V5_SignalLabel)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_TX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(iDevice, &LineEndpoint, pTX_V5_SignalLabel);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_TX);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pEX_V5_SignalLabel)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_EX);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(iDevice, &LineEndpoint, pEX_V5_SignalLabel);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_EX);
    }

	return Result;
}


U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_FAIL_THRESHOLD);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveTh(TransactionIdentifier, iDevice, &LineEndpoint,Threshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_FAIL_THRESHOLD);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 Threshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_SetSigDegradeTh(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD);
    }

	return Result;
}



U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveTh(iDevice, &LineEndpoint, pThreshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD);
    }

	return Result;
}


U8 WPX_FRMR_SONET_SDH_LO_PATH_V5_BIP2_GetSigDegradeTh(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pThreshold)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
 
    if(WPX_UFE_FRAMER_OK==Result)
    {
        WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD);
        Result=WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_GetSigDegradeTh(iDevice, &LineEndpoint, pThreshold);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD);
    }

	return Result;
}









/*
 *
 * Section: CONFIGURATION FACILITIES SOCKET CLIENT PDH
 *
 */






U8 WPX_FRMR_SOCKET_PDH_GetClientType(U8 iDevice, U32 iSocketClientPDH, U8 * pClientType)
{
	U8 Result;
  
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetClientType(iDevice, iSocketClientPDH, pClientType);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_SetFraming(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Framing)
{
	U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetFraming(TransactionIdentifier, iDevice, iSocketClientPDH, Framing);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING);

	return Result;
}




U8 WPX_FRMR_SOCKET_PDH_GetFraming(U8 iDevice, U32 iSocketClientPDH, U8 * pFraming)
{
	U8 Result;
 
    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_FRAMING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetFraming(iDevice, iSocketClientPDH, pFraming);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_FRAMING);

	return Result;
}




U8 WPX_FRMR_SOCKET_PDH_EnableTimeStamp(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U32 Gain)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableTimestamping(TransactionIdentifier, iDevice, iSocketClientPDH, Gain);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING);

	return Result;
}




U8 WPX_FRMR_SOCKET_PDH_DisableTimeStamp(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableTimestamping(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_GetTimeStamp(U8 iDevice, U32 iSocketClientPDH, U8 * pTimeStamping_IsEnabled)
{
	U8 Result;
 
    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetTimeStamping(iDevice, iSocketClientPDH, pTimeStamping_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING);

    return Result;
}


U8 WPX_FRMR_SOCKET_PDH_SetTimestampByteCnt(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 AnyTimestampByteCount)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetTimestampByteCount(TransactionIdentifier, iDevice, iSocketClientPDH, AnyTimestampByteCount);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_GetTimestampByteCnt(U8 iDevice, U32 iSocketClientPDH, U8 * pAnyTimestampByteCount)
{
	U8 Result;
 
    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetTimestampByteCount(iDevice, iSocketClientPDH, pAnyTimestampByteCount);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT);

    return Result;
}





U8 WPX_FRMR_SOCKET_PDH_SetMapping(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Mapping)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(TransactionIdentifier, iDevice, iSocketClientPDH, Mapping);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_GetMapping(U8 iDevice, U32 iSocketClientPDH, U8 * pMapping)
{
	U8 Result;
 
    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetMapping(iDevice, iSocketClientPDH, pMapping);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING);

    return Result;
}



U8 WPX_FRMR_SOCKET_PDH_SetClkRec(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClockRecoveryMode)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecoveryMode(TransactionIdentifier, iDevice, iSocketClientPDH, ClockRecoveryMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY);

    return Result;
}

U8 WPX_FRMR_SOCKET_PDH_E3T3_SetFEAC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Code)
{
   U8 Result;
   
   WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SET_FEAC);
   Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SetFEAC(TransactionIdentifier, iDevice, iSocketClientPDH, Code);
   WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SET_FEAC);
   
   return Result;
}


U8 WPX_FRMR_SOCKET_PDH_GetClkRec(U8 iDevice, U32 iSocketClientPDH, U8 * pClockRecoveryMode)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetClockRecoveryMode(iDevice, iSocketClientPDH, pClockRecoveryMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY);

    return Result;
}


U8 WPX_FRMR_SOCKET_PDH_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableCAS(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableCAS(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_Get_CAS_State(U8 iDevice, U32 iSocketClientPDH, U8 * pCAS_IsEnabled)
{
	U8 Result;
 
    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_Get_CAS_State(iDevice, iSocketClientPDH, pCAS_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE);

	return Result;
}




U8 WPX_FRMR_SOCKET_PDH_AIS_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsSocket(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_AIS_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsSocket(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_AIS_GetTowardsSocketState(U8 iDevice, U32 iSocketClientPDH, U8 * pAIS_IsEnabled)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GetTowardsSocketState(iDevice, iSocketClientPDH, pAIS_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE);

    return Result;
}


U8 WPX_FRMR_SOCKET_PDH_AIS_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsLine(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_AIS_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsLine(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_AIS_GetTowardsLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pAIS_IsEnabled)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_LINE_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GetTowardsLineState(iDevice, iSocketClientPDH, pAIS_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_LINE_STATE);

	return Result;
}

U8 WPX_FRMR_SOCKET_PDH_RDI_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_ENABLE_TOWARDS_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_EnableTowardsLine(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_ENABLE_TOWARDS_LINE);

	return Result;
}

U8 WPX_FRMR_SOCKET_PDH_RDI_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DISABLE_TOWARDS_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DisableTowardsLine(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DISABLE_TOWARDS_LINE);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_RDI_GetTowardsLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pRDI_IsEnabled)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_GET_TOWARDS_LINE_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_GetTowardsLineState(iDevice, iSocketClientPDH, pRDI_IsEnabled);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_GET_TOWARDS_LINE_STATE);

	return Result;
}

U8 WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U32 iSocketClientPDH, U32 ClockRate)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Set(TransactionIdentifier, iDevice, OutputSelector, iSocketClientPDH, ClockRate);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_DIAG_DRV_RecClkGet(U8 iDevice, U8 OutputSelector, U32 * piSocketClientPDH)
{
    U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Get(iDevice, OutputSelector, piSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET);

	return Result;
}





U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsSet(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressTermPrbsGet(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result;
  

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Get(iDevice, iSocketClientPDH, pPRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressGenPrbsSet(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_IngressGenPrbsGet(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result;
  

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Get(iDevice, iSocketClientPDH, pPRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsSet(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressTermPrbsGet(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result;
  

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Get(iDevice, iSocketClientPDH, pPRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_EgressGenPrbs_Get(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result;
  

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Get(iDevice, iSocketClientPDH, pPRBS_Pattern);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET);

	return Result;
}









/*
 *
 * Section: CONFIGURATION FACILITIES DISCRETE CLIENT
 *
 */




U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClientType(U8 iDevice, U8 iDiscreteClientPort, U8 * pClientType)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_BANDWIDTH_TYPE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClientType(iDevice, iDiscreteClientPort, pClientType);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_BANDWIDTH_TYPE);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Framing)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(TransactionIdentifier, iDevice, iDiscreteClientPort, Framing);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING);

	return Result;
}

U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetFraming(U8 iDevice, U8 iDiscreteClientPort, U8 * pFraming)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_FRAMING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetFraming(iDevice, iDiscreteClientPort, pFraming);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_FRAMING);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Mapping)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(TransactionIdentifier, iDevice, iDiscreteClientPort, Mapping);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetMapping(U8 iDevice, U8 iDiscreteClientPort, U8 * pMapping)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_MAPPING);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetMapping(iDevice, iDiscreteClientPort, pMapping);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_MAPPING);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClockRecoveryMode)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(TransactionIdentifier, iDevice, iDiscreteClientPort, ClockRecoveryMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClockRecovery(U8 iDevice, U8 iDiscreteClientPort, U8 * pClockRecoveryMode)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CLOCK_RECOVERY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClockRecovery(iDevice, iDiscreteClientPort, pClockRecoveryMode);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CLOCK_RECOVERY);


	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_Get_CAS_State(U8 iDevice, U8 iDiscreteClientPort, U8 * pCAS_IsEnabled)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CAS_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_Get_CAS_State(iDevice, iDiscreteClientPort, pCAS_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CAS_STATE);

	return Result;
}




U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsSocketState(U8 iDevice, U8 iDiscreteClientPort, U8 * pAIS_IsEnabled)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsSocketState(iDevice, iDiscreteClientPort, pAIS_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(TransactionIdentifier, iDevice, iDiscreteClientPort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsLineState(U8 iDevice, U8 iDiscreteClientPort, U8 * pAIS_IsEnabled)
{
	U8 Result;
  
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_LINE_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsLineState(iDevice, iDiscreteClientPort, pAIS_IsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_LINE_STATE);

	return Result;
}




/*
 *
 * Section: CONNECTIONS FUNCTIONS
 *
 */




U8 WPX_FRMR_CONNECTIONS_ThroughCreate(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_FirstLineEndpointType, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_SecondLineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &FirstLineEndpoint, pCommon_SDH_SONET_FirstLineEndpointType);
    if(WPX_UFE_FRAMER_OK==Result)
    {
        Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &SecondLineEndpoint, pCommon_SDH_SONET_SecondLineEndpointType);
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
        WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION);
        Result=WPX_FRMR_PL_CONNECTIONS_ThroughCreate(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION);
    }

	return Result;
}


U8 WPX_FRMR_CONNECTIONS_ThroughDelete(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_FirstLineEndpointType, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_SecondLineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE FirstLineEndpoint;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE SecondLineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &FirstLineEndpoint, pCommon_SDH_SONET_FirstLineEndpointType);
    if(WPX_UFE_FRAMER_OK==Result)
    {
        Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &SecondLineEndpoint, pCommon_SDH_SONET_SecondLineEndpointType);
    }

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION);
        Result=WPX_FRMR_PL_CONNECTIONS_ThroughDelete(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION);
    }

	return Result;
}




U8 WPX_FRMR_CONNECTIONS_AddDropLineToSocketCreate(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U32 iSocketClientPDH)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION);
        Result=WPX_FRMR_PL_CONNECTIONS_AddDropLineToSocketCreate(TransactionIdentifier, iDevice, &LineEndpoint, iSocketClientPDH);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION);
    }

	return Result;
}


U8 WPX_FRMR_CONNECTIONS_AddDropLineToSocketDelete(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U32 iSocketClientPDH)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION);
        Result=WPX_FRMR_PL_CONNECTIONS_AddDropLineToSocketDelete(TransactionIdentifier, iDevice, &LineEndpoint, iSocketClientPDH);
	    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION);
    }

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, iDiscreteClientPort, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION);

	return Result;
}


U8 WPX_UFE_FRAMER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH)
{
	U8 Result;
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, iDiscreteClientPort, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION);

	return Result;
}




/*
 *
 * Section: ALARM REPORTING
 *
 */


U8 WPX_FRMR_DEV_DRV_SONET_SDH_EnableAlarm(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}



U8 WPX_FRMR_DEV_DRV_SONET_SDH_DisableAlarm(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}



U8 WPX_FRMR_DEV_DRV_SONET_SDH_GetAlarm(U8 AlarmCategory, U8 * pIsEnabled)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GetAlarmReportingForAlarmCategory(AlarmCategory, pIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}


U8 WPX_FRMR_DEV_DRV_SOCKET_EnableAlarm(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}



U8 WPX_FRMR_DEV_DRV_SOCKET_DisableAlarm(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}



U8 WPX_FRMR_DEV_DRV_SOCKET_GetAlarm(U8 AlarmCategory, U8 * pIsEnabled)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GetAlarmReportingForAlarmCategory(AlarmCategory, pIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}







U8 WPX_UFE_FRAMER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}



U8 WPX_UFE_FRAMER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}



U8 WPX_UFE_FRAMER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GetAlarmReportingForAlarmCategory(U8 AlarmCategory, U8 * pIsEnabled)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GetAlarmReportingForAlarmCategory(AlarmCategory, pIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY);

	return Result;
}










/*
 *
 * Section: PORT PROTECTION
 *
 */


U8 WPX_FRMR_SONET_SDH_PORT_ForceA(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_ForceA(TransactionIdentifier, iDevice, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_ForceB(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_ForceB(TransactionIdentifier, iDevice, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B);
	
	return Result;
}



U8 WPX_FRMR_SONET_SDH_PORT_GetForceState(U8 iDevice, U8 iLinePort, U8 * pForceSide)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_FORCE_STATE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_GetForceState(iDevice, iLinePort, pForceSide);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_FORCE_STATE);
	
	return Result;
}





/*
 *
 * Section: DIAGNOSTIC LOOPBACK
 *
 */


U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW_LINE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(TransactionIdentifier, iDevice, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW_LINE);

	return Result;
}




U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW_LINE);
    Result=WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(TransactionIdentifier, iDevice, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW_LINE);
	
	return Result;
}


U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState(U8 iDevice, U8 iLinePort, U8 * pLoopbackIsEnabled)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_SHALLOW_LINE_STATE);
    Result=WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState(iDevice, iLinePort, pLoopbackIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_SHALLOW_LINE_STATE);
	
	return Result;
}





U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result;
	
	
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP_SYSTEM);
    Result=WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(TransactionIdentifier, iDevice, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP_SYSTEM);

	return Result;
}




U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP_SYSTEM);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem(TransactionIdentifier, iDevice, iLinePort);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP_SYSTEM);

	return Result;
}


U8 WPX_FRMR_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState(U8 iDevice, U8 iLinePort, U8 * pLoopbackIsEnabled)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_DEEP_SYSTEM_STATE);
	Result=WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState(iDevice, iLinePort, pLoopbackIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_DEEP_SYSTEM_STATE);
	
	return Result;
}







U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_SHALLOW_SYSTEM);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableShallowSystem(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_SHALLOW_SYSTEM);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;


	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_SHALLOW_SYSTEM);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableShallowSystem(TransactionIdentifier, iDevice, iSocketClientPDH);
    WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_SHALLOW_SYSTEM);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetShallowSystemState(U8 iDevice, U32 iSocketClientPDH, U8 * pLoopbackIsEnabled)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetShallowSystemState(iDevice, iSocketClientPDH, pLoopbackIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM_STATE);

	return Result;
}



U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkEnableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

    WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_DEEP_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableDeepLine(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_DEEP_LINE);

	return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkDisableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_DEEP_LINE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableDeepLine(TransactionIdentifier, iDevice, iSocketClientPDH);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_DEEP_LINE);

    return Result;
}


U8 WPX_FRMR_SOCKET_PDH_DIAG_LpbkGetDeepLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pLoopbackIsEnabled)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_DEEP_LINE_STATE);
    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetDeepLineState(iDevice, iSocketClientPDH, pLoopbackIsEnabled);
	WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_DEEP_LINE_STATE);

    return Result;
}




U8 WPX_FRMR_SONET_SDH_DIAG_LpbkEnable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {	
		WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE);
		Result=WPX_FRMR_PL_SONET_SDH_DIAG_LpbkEnable(TransactionIdentifier, iDevice, &LineEndpoint);
		WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE);
	}

	return Result;
}


U8 WPX_FRMR_SONET_SDH_DIAG_LpbkDisable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {		
		WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE);
		Result=WPX_FRMR_PL_SONET_SDH_DIAG_LpbkDisable(TransactionIdentifier, iDevice, &LineEndpoint);
		WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE);
	}

	return Result;
}



U8 WPX_FRMR_SONET_SDH_DIAG_LpbkGetState(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pLoopbackIsEnabled)
{
	U8 Result;
	    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {	
		WPX_UFE_FRAMER_ENTER(API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE);
		Result=WPX_FRMR_PL_SONET_SDH_DIAG_LpbkGetState(iDevice, &LineEndpoint, pLoopbackIsEnabled);
		WPX_UFE_FRAMER_EXIT(API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE);
	}

	return Result;
}






/*
 *
 * Section: DIAGNOSTIC PEEK POKE
 *
 * NOTE Peek Response returned via callback 
 * WPX_UFE_FRAMER_PEEK_RESPONSE_ANNOUNCE_TYPE PeekResponse
 *
 * NOTE Block PokeVerify Response returned via callback 
 * WPX_UFE_FRAMER_BLOCK_POKE_VERIFY_RESPONSE_ANNOUNCE_TYPE BlockPokeVerifyResponse
 *
 */


U8 WPX_FRMR_DEV_DIAG_Poke(U32 TransactionIdentifier, U8 iDevice, U32 Address, U32 Value)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_DEVICE_DIAGNOSTIC_POKE);
    Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_Poke(TransactionIdentifier, iDevice, Address, Value);
	WPX_UFE_FRAMER_EXIT(API_DEVICE_DIAGNOSTIC_POKE);

    return Result;
}



U8 WPX_FRMR_DEV_DIAG_Peek(U8 iDevice, U32 Address)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_DEVICE_DIAGNOSTIC_PEEK);
    Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_Peek(iDevice, Address);
	WPX_UFE_FRAMER_EXIT(API_DEVICE_DIAGNOSTIC_PEEK);

    return Result;
}


U8 WPX_FRMR_DEV_DIAG_BlockPoke(U32 TransactionIdentifier, U8 iDevice, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten, U32 Value)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_DEVICE_DIAGNOSTIC_BLOCK_POKE);
    Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_BlockPoke(TransactionIdentifier, iDevice, StartAddress, NumberOfU32AddressesToBeWritten, Value);
	WPX_UFE_FRAMER_EXIT(API_DEVICE_DIAGNOSTIC_BLOCK_POKE);

    return Result;
}


U8 WPX_FRMR_DEV_DIAG_BlockPokeVerify(U32 TransactionIdentifier, U8 iDevice, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten, U32 Value)
{
	U8 Result;
 
	WPX_UFE_FRAMER_ENTER(API_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY);
    Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_BlockPokeVerify(TransactionIdentifier, iDevice, StartAddress, NumberOfU32AddressesToBeWritten, Value);
	WPX_UFE_FRAMER_EXIT(API_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY);

    return Result;
}






/*
 *
 * Section: STATUS DEVICE DRIVER
 *
 */


U8 WPX_FRMR_STATUS_DEV_DRV_VERSION_GetInfo(char *pDeviceDriverInformation)
{
	U8 Result;
    
	WPX_UFE_FRAMER_ENTER(API_STATUS_DEVICE_DRIVER_VERSION_GET_INFORMATION);
    Result=OMIINO_API_INTERNAL_STATUS_DEVICE_DRIVER_VERSION_GetInformation(pDeviceDriverInformation);
	WPX_UFE_FRAMER_EXIT(API_STATUS_DEVICE_DRIVER_VERSION_GET_INFORMATION);


	return Result;
}

/*
 *
 * Section: STATUS DEVICE
 *
 */


U8 WPX_FRMR_STATUS_DEV_VERSION_GetFirmwareInfo(U8 iDevice, char *pFirmwareInformation)
{
	U8 Result;
 	WPX_UFE_FRAMER_ENTER(API_STATUS_DEVICE_VERSION_GET_FIRMWARE_INFORMATION);
    Result=OMIINO_API_INTERNAL_STATUS_DEVICE_VERSION_GetFirmwareInformation(iDevice, pFirmwareInformation);
	WPX_UFE_FRAMER_EXIT(API_STATUS_DEVICE_VERSION_GET_FIRMWARE_INFORMATION);

	return Result;
}


U8 WPX_FRMR_STATUS_DEV_VERSION_GetHardwareInfo(U8 iDevice, char *pHardwareInformation)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_STATUS_DEVICE_VERSION_GET_HARDWARE_INFORMATION);
    Result=OMIINO_API_INTERNAL_STATUS_DEVICE_VERSION_GetHardwareInformation(iDevice, pHardwareInformation);
	WPX_UFE_FRAMER_EXIT(API_STATUS_DEVICE_VERSION_GET_HARDWARE_INFORMATION);


	return Result;
}






U8 WPX_UFE_FRAMER_STATUS_DEVICE_BUILD_GetHardwareVariant(U8 iDevice, U8 * pHardwareBuildVariant)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_STATUS_DEVICE_BUILD_GET_HARDWARE_VARIANT);
    Result=OMIINO_API_INTERNAL_STATUS_DEVICE_BUILD_GetHardwareVariant(iDevice, pHardwareBuildVariant);
	WPX_UFE_FRAMER_EXIT(API_STATUS_DEVICE_BUILD_GET_HARDWARE_VARIANT);

	return Result;
}



/*
 *
 * Section: STATUS FACILITIES SONET/SDH PORT SECTION
 *
 */


U8 WPX_FRMR_STATUS_SONET_SDH_PORT_SEC_J0_GetRX(U8 iDevice, U8 iLinePort, char * pRX_TraceString)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_RX);
    Result=WPX_FRMR_PL_STATUS_SONET_SDH_PORT_SEC_J0_GetRX(iDevice, iLinePort, pRX_TraceString);
	WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_RX);


	return Result;
}




/*
 *
 * Section: STATUS FACILITIES SONET/SDH PORT LINE
 *
 */

U8 WPX_FRMR_STATUS_SONET_SDH_PORT_LINE_S1_GetRX(U8 iDevice, U8 iLinePort, U8 * pRX_S1)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_RX);
    Result=WPX_FRMR_PL_STATUS_SONET_SDH_PORT_LINE_S1_GetRX(iDevice, iLinePort, pRX_S1);
	WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_RX);

	return Result;
}


U8 WPX_FRMR_STATUS_SONET_SDH_PORT_LINE_K1K2_GetRX(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	U8 Result;
	

	WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_RX);
    Result=WPX_FRMR_PL_STATUS_SONET_SDH_PORT_LINE_K1K2_GetRX(iDevice, iLinePort, pRX_K1K2);
	WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_RX);

	return Result;
}




/*
 *
 * Section: STATUS FACILITIES SONET/SDH HO PATH
 *
 */

U8 WPX_FRMR_STATUS_SONET_SDH_HO_PATH_J1_GetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pRX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GET_RX);
        Result=WPX_FRMR_PL_STATUS_SONET_SDH_HO_PATH_J1_GetRX(iDevice, &LineEndpoint, pRX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GET_RX);
    }

	return Result;
}


U8 WPX_FRMR_STATUS_SONET_SDH_HO_PATH_C2_GetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pRX_C2)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GET_RX);
        Result=WPX_FRMR_PL_STATUS_SONET_SDH_HO_PATH_C2_GetRX(iDevice, &LineEndpoint, pRX_C2);
	    WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GET_RX);
    }

	return Result;
}




/*
 *
 * Section: STATUS FACILITIES SONET/SDH LO PATH
 *
 */

U8 WPX_FRMR_STATUS_SONET_SDH_LO_PATH_J2_GetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, char * pRX_TraceString)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GET_RX);
        Result=WPX_FRMR_PL_STATUS_SONET_SDH_LO_PATH_J2_GetRX(iDevice, &LineEndpoint, pRX_TraceString);
	    WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GET_RX);
    }

	return Result;
}




U8 WPX_FRMR_STATUS_SONET_SDH_LO_PATH_V5_SigLabelGetRX(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 * pRX_V5_SignalLabel)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);
    if(WPX_UFE_FRAMER_OK==Result)
    {
	    WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_RX);
        Result=WPX_FRMR_PL_STATUS_SONET_SDH_LO_PATH_V5_SigLabelGetRX(iDevice, &LineEndpoint, pRX_V5_SignalLabel);
        WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_RX);
    }

	return Result;
}





/*
 *
 * Section: STATUS DIAGNOSTICS
 *
 */



U8 WPX_FRMR_STATUS_SOCKET_PDH_DIAG_IngressPrbsGetState(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_State)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE);
    Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_PRBS_GetState(iDevice, iSocketClientPDH, pPRBS_State);
	WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE);

	return Result;
}




U8 WPX_FRMR_STATUS_SOCKET_PDH_DIAG_EgressPrbsGetState(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_State)
{
	U8 Result;

	WPX_UFE_FRAMER_ENTER(API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE);
    Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_PRBS_GetState(iDevice, iSocketClientPDH, pPRBS_State);
	WPX_UFE_FRAMER_EXIT(API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE);

	return Result;
}





/*
 *
 * Section: Status PERFORMANCE MONITORING Data Collection
 *
 */

U8 WPX_FRMR_STATUS_PM_SONET_SDH_Port_ReadData(U8 iDevice, U8 iTableIndex, U8 iLinePort, U8 iPoint, U32 * pCount)
{
    U8 Result;

	

	WPX_UFE_FRAMER_ENTER(API_PM_COLLECTION_SONET_SDH_PORT);
    Result=WPX_FRMR_PL_STATUS_PM_SONET_SDH_Port_ReadData(iDevice, iTableIndex, iLinePort, iPoint, pCount);
    WPX_UFE_FRAMER_EXIT(API_PM_COLLECTION_SONET_SDH_PORT);

    return Result;
}


U8 WPX_FRMR_STATUS_PM_SONET_SDH_HO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U32 * pCount)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
   U8 iLogicalLinePort;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
		WPX_UFE_FRAMER_ENTER(API_PM_COLLECTION_SONET_SDH_HO_PATH);

      if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
      {
         if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), LineEndpoint.iLinePort, &iLogicalLinePort))
         {
            LineEndpoint.iLinePort=iLogicalLinePort;
		Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_HO_PATH_ReadData(iDevice, iTableIndex, &LineEndpoint, iPoint, pCount);
         }
      }
   
		WPX_UFE_FRAMER_EXIT(API_PM_COLLECTION_SONET_SDH_HO_PATH);
	}

    return Result;
}


U8 WPX_FRMR_STATUS_PM_SONET_SDH_LO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType, U8 iPoint, U32 * pCount)
{
	U8 Result;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

    Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(iDevice, &LineEndpoint, pCommon_SDH_SONET_LineEndpointType);

    if(WPX_UFE_FRAMER_OK==Result)
    {
		WPX_UFE_FRAMER_ENTER(API_PM_COLLECTION_SONET_SDH_LO_PATH);
		Result=WPX_FRMR_PL_STATUS_PM_SONET_SDH_LO_PATH_ReadData(iDevice, iTableIndex, &LineEndpoint, iPoint, pCount);
		WPX_UFE_FRAMER_EXIT(API_PM_COLLECTION_SONET_SDH_LO_PATH);
	}

    return Result;
}



U8 WPX_UFE_FRAMER_STATUS_PERFORMANCE_MONITORING_DiscreteClientPort_ReadData(U8 iDevice, U8 iTableIndex, U8 iDiscreteClientPort, U8 iPoint, U32 * pCount)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_PM_COLLECTION_DISCRETE_CLIENT_PORT);
    Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_DiscreteClientPort_ReadData(iDevice, iTableIndex, iDiscreteClientPort, iPoint, pCount);
    WPX_UFE_FRAMER_EXIT(API_PM_COLLECTION_DISCRETE_CLIENT_PORT);

    return Result;
}




U8 WPX_FRMR_STATUS_PM_SocketPort_PDH_E1T1_ReadData(U8 iDevice, U8 iTableIndex, U32 iSocketClientPDH, U8 iPoint, U32 * pCount)
{
    U8 Result;

	WPX_UFE_FRAMER_ENTER(API_PM_COLLECTION_SOCKET_CLIENT_PDH_E1T1_CLIENT_PORT);
    Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SocketClientPort_PDH_E1T1_ReadData(iDevice, iTableIndex, iSocketClientPDH, iPoint, pCount);
    WPX_UFE_FRAMER_EXIT(API_PM_COLLECTION_SOCKET_CLIENT_PDH_E1T1_CLIENT_PORT);

    return Result;
}

U8 WPX_FRMR_STATUS_PM_SocketPort_PDH_E3T3_ReadData(U8 iDevice, U8 iTableIndex, U32 iSocketClientPDH, U8 iPoint, U32 * pCount)
{
    U8 Result;

    WPX_UFE_FRAMER_ENTER(API_PM_COLLECTION_SOCKET_CLIENT_PDH_E3T3_CLIENT_PORT);
    Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SocketClientPort_PDH_E3T3_ReadData(iDevice, iTableIndex, iSocketClientPDH, iPoint, pCount);
    WPX_UFE_FRAMER_EXIT(API_PM_COLLECTION_SOCKET_CLIENT_PDH_E3T3_CLIENT_PORT);

    return Result;
}



/* This API needs to be called for the DEMO BOARD ONLY - after set build personality */ 
void WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(U8 iDevice)
{
	OMIINO_API_INTERNAL_DEMO_ONLY_Configure_GTP_ClockSource(iDevice);
}


U8 WPX_FRMR_ReportDeviceAlarmMap(U8 iDevice)
{
   U8 result;

   if (WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES > iDevice)
   {
      WPX_UFE_FRAMER_ENTER(API_BULK_ALARM_LIST_ALL_ACTIVE);
      result = OMIINO_API_INTERNAL_BULK_ALARM_LIST_ALL_ACTIVE(iDevice);
      WPX_UFE_FRAMER_EXIT(API_BULK_ALARM_LIST_ALL_ACTIVE);

      OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_ALARM_MAP_RESPONSE(iDevice);

      result = WPX_UFE_FRAMER_OK;
   }
   else
      result = WPX_UFE_FRAMER_ERROR_DEVICE_OUT_OF_RANGE;

   return result;
}

