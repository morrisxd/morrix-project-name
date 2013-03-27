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
#include "WO_FRMR_configuration_defaults_private.h"


void OMIINO_A_ResetModelForDevice(U8 iDevice)
{
	OMIINO_FRAMER_InitializeStatusForDevice(iDevice, OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice)); 
	OMIINO_FRAMER_InitializeConfigurationForDevice(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice));
	OMIINO_FRAMER_PerformanceMonitoring_M_of_N_SetLimit(&OMIINO_RAM.Device[iDevice].M_of_N, OMIINO_DEFAULT_M_OF_N_PM_MONITORED_POINTS);
	OMIINO_FRAMER_PerformanceMonitoring_M_of_N_SetCurrent(&OMIINO_RAM.Device[iDevice].M_of_N);
}



void OMIINO_MODEL_CONTROL_HandleEvent_For_S_BACK_TO_BOOTLOADER(OMIINO_MODEL_CONTROL_FSM_TYPE * pFSM, int AnyEvent)
{
	U8 HardwareBuildVariant;
	OMIINO_FRAMER_STATUS_DEVICE_TYPE * pDeviceStatusRAM;   

    

	switch(AnyEvent)
	{
		case	E_FIRMWARE_HELLO:
				pFSM->State=S_IN_FIRMWARE;
				OMIINO_A_ResetModelForDevice(pFSM->iDevice);
				pDeviceStatusRAM = OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, 0); /* DMCC TODO - hardcoded device 0 */
				OMIINO_FRAMER_STATUS_DeviceDriver_GetHardwareVariant(pDeviceStatusRAM, &HardwareBuildVariant);
				OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_FIRMWARE_RESTART(pFSM->iDevice, HardwareBuildVariant);
				break;

		case	E_BOOTLOADER_HELLO:
				/* No Change, Ignore */
				break;

		default:
				/* Ignore */
				break;
	}
}



void OMIINO_MODEL_CONTROL_HandleEvent_For_S_IN_FIRMWARE(OMIINO_MODEL_CONTROL_FSM_TYPE * pFSM, int AnyEvent)
{
	switch(AnyEvent)
	{
		case	E_FIRMWARE_HELLO:
				/* No Change, Ignore */
				break;

		case	E_BOOTLOADER_HELLO:
				pFSM->State=S_BACK_TO_BOOTLOADER;
				break;

		default:
				/* Ignore */
				break;
	}
}



void OMIINO_MODEL_CONTROL_HandleEvent_For_S_IN_BOOTLOADER(OMIINO_MODEL_CONTROL_FSM_TYPE * pFSM, int AnyEvent)
{
	switch(AnyEvent)
	{
		case	E_FIRMWARE_HELLO:
				pFSM->State=S_IN_FIRMWARE;
				break;

		case	E_BOOTLOADER_HELLO:
				/* No Change, Ignore */
				break;

		default:
				/* Ignore */
				break;
	}
}




void OMIINO_MODEL_CONTROL_HandleEvent_For_S_RESET_MODEL_INIT(OMIINO_MODEL_CONTROL_FSM_TYPE * pFSM, int AnyEvent)
{
	switch(AnyEvent)
	{
		case	E_FIRMWARE_HELLO:
				pFSM->State=S_IN_FIRMWARE;
				break;

		case	E_BOOTLOADER_HELLO:
				pFSM->State=S_IN_BOOTLOADER;
				break;

		default:
				/* Ignore */
				break;
	}
}



void OMIINO_MODEL_CONTROL_HandleEvent(OMIINO_MODEL_CONTROL_FSM_TYPE * pFSM, int AnyEvent)
{
	switch(pFSM->State)
	{
		case	S_RESET_MODEL_INIT:
				OMIINO_MODEL_CONTROL_HandleEvent_For_S_RESET_MODEL_INIT(pFSM,AnyEvent);
				break;

		case	S_IN_BOOTLOADER:
				OMIINO_MODEL_CONTROL_HandleEvent_For_S_IN_BOOTLOADER(pFSM,AnyEvent);
				break;

		case	S_IN_FIRMWARE:
				OMIINO_MODEL_CONTROL_HandleEvent_For_S_IN_FIRMWARE(pFSM,AnyEvent);
				break;

		case	S_BACK_TO_BOOTLOADER:
				OMIINO_MODEL_CONTROL_HandleEvent_For_S_BACK_TO_BOOTLOADER(pFSM,AnyEvent);
				break;

		case	S_TEST_MODE:
				/* Do Nothing */
				break;
		default:
				pFSM->State=S_RESET_MODEL_INIT;
				break;

	}

}

void OMIINO_MODEL_CONTROL_FSM_Initialize(OMIINO_MODEL_CONTROL_FSM_TYPE * pFSM, U8 iDevice, U8 TestMode)
{
	pFSM->iDevice=iDevice;

	if(0==TestMode)
	{
		pFSM->State=S_RESET_MODEL_INIT;
	}
	else
	{
		pFSM->State=S_TEST_MODE;
	}
}

	


void OMIINO_MODEL_CONTROL_FSM_Announce_FirmwareHelloFor(U8 iDevice)
{
	OMIINO_MODEL_CONTROL_HandleEvent(&OMIINO_RAM.Device[iDevice].ModelControl_FSM, E_FIRMWARE_HELLO);
}


void OMIINO_MODEL_CONTROL_FSM_Announce_BootloaderHelloFor(U8 iDevice)
{
	OMIINO_MODEL_CONTROL_HandleEvent(&OMIINO_RAM.Device[iDevice].ModelControl_FSM, E_BOOTLOADER_HELLO);
}

