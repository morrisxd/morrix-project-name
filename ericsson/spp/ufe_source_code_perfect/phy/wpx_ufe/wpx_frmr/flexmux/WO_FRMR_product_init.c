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


void OMIINO_Product_SetFirewalls(void)
{
    int i;
    int j;
    int Signature=0x00000001;

    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_TOP, Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_BOT, Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M1,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M2,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M3,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M4,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M5,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M6,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M7,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M8,  Signature++);
    OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Firewall_M9,  Signature++);

    for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;i++)
    {
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Firewall_Device_A, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Firewall_Device_B, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Firewall_Device_C, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Firewall_Device_D, Signature++);

        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_A, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_B, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_C, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_D, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_E, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_F, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_G, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_H, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_I, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_J, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.Firewall_Configuration_K, Signature++);

	    for(j=0;j<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;j++)
		{
			OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.SONET_SDH_Port[j].Firewall_Configuration_SonetSdh_A, Signature++);
			OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.SONET_SDH_Port[j].Firewall_Configuration_SonetSdh_B, Signature++);
			OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].Hierarchy.SONET_SDH_Port[j].Firewall_Configuration_SonetSdh_C, Signature++);
		}

        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].LLC.Firewall_TOP, Signature++);
        OMIINO_FRAMER_FIREWALL_AddItemToObservationList(&OMIINO_RAM.Utility.Firewall, (U32)&OMIINO_RAM.Device[i].LLC.Firewall_BOT, Signature++);
    }
}



void OMIINO_Product_InitializeConfiguration(OMIINO_FRAMER_RAM_TYPE *pFramerRAM, U8 TestMode)
{
	U8	i;

	OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize();

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
	{
		OMIINO_MODEL_CONTROL_FSM_Initialize(&(pFramerRAM->Device[i].ModelControl_FSM), i, TestMode);
	}

    OMIINO_FRAMER_UTILITIES_Initialize(&pFramerRAM->Utility);
    OMIINO_Product_SetFirewalls();
    OMIINO_DEVICE_PERSONALITY_Initialize(pFramerRAM->Device);    
	OMIINO_FRAMER_CONFIGURATION_ModelInitialize(pFramerRAM, TestMode);
	OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Initialize(TestMode);
	OMIINO_FRAMER_DEVICE_PortMap_Initialize(pFramerRAM->Device);
}



void OMIINO_ProductConfiguration_SetBuildPersonality(OMIINO_FRAMER_RAM_TYPE *pFramerRAM, U8 BuildPersonality)
{
	U8	i;

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
	{
		OMIINO_FRAMER_Device_SetBuildPersonality(&(pFramerRAM->Device[i]), BuildPersonality);
	}
}




void OMIINO_ProductConfiguration_SendBuildSelectionToFirmware(U8 iDevice)
{
	U8	BuildPersonality;
	U8	Length;

	OMIINO_FRAMER_Device_GetBuildPersonality(&(OMIINO_RAM.Device[iDevice]), &BuildPersonality);
	Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetBuildPersonality(0, BuildPersonality, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}
}




void OMIINO_Product_InitializeStatus(OMIINO_FRAMER_RAM_TYPE *pFramerRAM)
{
	QC_API_Internal_Initialize();
	OMIINO_FRAMER_STATUS_ModelInitialize(pFramerRAM);
    OMIINO_FRAMER_DefineBuildInformation();
}


void OMIINO_Product_InitializeBindings(		OMIINO_FRAMER_RAM_TYPE                          *       pFramerRAM,
                                            WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE                         (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES], 
								            WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE			*		pEnvironmentTable,
								            WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE				*		pAutonomousOutputTable,
								            WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE	*		pPerformanceMonitoringDeviceTable,
                                            WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE       *       pDeviceLevelDiagnosticTable,
                                            WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE                       *       pOmniSpyTable)
{
    OMIINO_ALARM_WRAPPER_BindCallbacks(&(pFramerRAM->AutonomousOutputTable.AutonomousPrivateAlarmTable), &(pAutonomousOutputTable->AutonomousAlarmTable));
    OMIINO_PERFORMANCE_MONITORING_WRAPPER_BindCallbacks(&(pFramerRAM->AutonomousOutputTable.AutonomousPrivatePerformanceMonitoringTable), &(pAutonomousOutputTable->AutonomousPerformanceMonitoringTable));
    OMIINO_SIGNALLING_WRAPPER_BindCallbacks(&(pFramerRAM->AutonomousOutputTable.AutonomousPrivateSignallingTable), &(pAutonomousOutputTable->AutonomousSignallingTable));
    OMIINO_DIAGNOSTIC_WRAPPER_BindCallbacks(&(pFramerRAM->DeviceLevelDiagnosticTable), pDeviceLevelDiagnosticTable);
	OMIINO_PERFORMANCE_MONITORING_BindDataTable(&pFramerRAM->PerformanceMonitoringDeviceTable,pPerformanceMonitoringDeviceTable);
    OMIINO_OMNISPY_BindCallbacks(&pFramerRAM->OmniSpy_RAM.AutonomousOutputTable,pOmniSpyTable);
    OMIINO_FRAMER_BindMemoryMap(pFramerRAM, pMailboxMemoryMap);
	OMIINO_FRAMER_ENVIRONMENT_WRAPPER_BindEnvironment(pFramerRAM, pEnvironmentTable);

    OMIINO_FRAMER_DeviceDriverHandleCreate( &pFramerRAM->EnvironmentWrapper, "OMIINO_DD_HANDLE");
}





void OMIINO_Product_InitializeComms(OMIINO_FRAMER_RAM_TYPE *pFramerRAM)
{
	U32 i;

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
	{
		OMIINO_RAM.Device[i].LLC_AccessIsPermitted=0;
	}

    OMIINO_LLC_NorthboundIntialize(pFramerRAM);
}


void OMIINO_Product_PreInitialize(WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE                       *       pOmniSpyTable)
{
	U32 i;

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
	{
		OMIINO_FRAMER_ALARM_MANAGER_PreInitialize(&(OMIINO_RAM.Device[i].Hierarchy.Alarms));
	}

	if(pOmniSpyTable->PreProductInitialization.Integrity)
	{
		(*pOmniSpyTable->PreProductInitialization.Integrity)();
	}
}

void OMIINO_Product_Initialize(		WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE					(*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES], 
								    WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE					*		pEnvironmentTable,
								    WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE				*		pAutonomousOutputTable,
								    WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE	*		pPerformanceMonitoringDeviceTable,
                                    WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE       *       pDeviceLevelDiagnosticTable,
                                    WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE                       *       pOmniSpyTable,
									U8																TestMode)
{
	OMIINO_Product_PreInitialize(pOmniSpyTable);

    OMIINO_Product_InitializeConfiguration(&OMIINO_RAM, TestMode);
    OMIINO_Product_InitializeStatus(&OMIINO_RAM);
    OMIINO_Product_InitializeComms(&OMIINO_RAM);
	
    OMIINO_Product_InitializeBindings(&OMIINO_RAM,
                                      pMailboxMemoryMap, 
								      pEnvironmentTable,
								      pAutonomousOutputTable,
								      pPerformanceMonitoringDeviceTable,
                                      pDeviceLevelDiagnosticTable,
                                      pOmniSpyTable); 

}

