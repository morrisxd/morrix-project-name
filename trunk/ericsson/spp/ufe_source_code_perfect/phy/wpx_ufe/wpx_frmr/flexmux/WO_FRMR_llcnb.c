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




void OMIINO_LLC_HandleNorthbound_Status_MessagesForDevice(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=&pFramerRAM->Device[iDevice];

    if(OMIINO_LLC_API_TryReceive(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS, pDeviceRAM->LLC_NorthboundBuffer.Buffer, &pDeviceRAM->LLC.Length))
    {
        OMIINO_LLC_Northbound_StatusMessage(iDevice,pFramerRAM);
        OMIINO_LLC_Register_CTRL_ToggleOwnership(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_STATUS);
    }
}


void OMIINO_LLC_HandleNorthbound_PerformanceMonitoring_MessagesForDevice(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=&pFramerRAM->Device[iDevice];

    if(OMIINO_LLC_API_TryReceive(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING, pDeviceRAM->LLC_NorthboundBuffer.Buffer, &pDeviceRAM->LLC.Length))
    {
        OMIINO_LLC_Northbound_PerformanceMonitoringMessage(iDevice,pFramerRAM);
        OMIINO_LLC_Register_CTRL_ToggleOwnership(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_PERFORMANCE_MONITORING);
    }
}


void OMIINO_LLC_HandleNorthbound_Signalling_MessagesForDevice(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=&pFramerRAM->Device[iDevice];

    if(OMIINO_LLC_API_TryReceive(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING, pDeviceRAM->LLC_NorthboundBuffer.Buffer, &pDeviceRAM->LLC.Length))
    {
        OMIINO_LLC_Northbound_SignallingMessage(iDevice,pFramerRAM);
        OMIINO_LLC_Register_CTRL_ToggleOwnership(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_SIGNALLING);
    }
}


void OMIINO_LLC_HandleNorthbound_OmniSpy_MessagesForDevice(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=&pFramerRAM->Device[iDevice];

	if(OMIINO_LLC_API_TryReceive(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY, pDeviceRAM->LLC_NorthboundBuffer.Buffer, &pDeviceRAM->LLC.Length))
    {
		OMIINO_LLC_Northbound_OmniSpyMessage(iDevice,pFramerRAM);
        OMIINO_LLC_Register_CTRL_ToggleOwnership(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_OMNISPY);
    }
}


void OMIINO_LLC_HandleNorthbound_AuxiliaryOmniSpy_MessagesForDevice(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=&pFramerRAM->Device[iDevice];

    if(OMIINO_LLC_API_TryReceive(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY, pDeviceRAM->LLC_NorthboundBuffer.Buffer, &pDeviceRAM->LLC.Length))
    {
        OMIINO_LLC_Northbound_AuxiliaryOmniSpyMessage(iDevice,pFramerRAM);
        OMIINO_LLC_Register_CTRL_ToggleOwnership(&pDeviceRAM->MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_NORTHBOUND_AUXILIARY_OMNISPY);
    }
}



void OMIINO_LLC_HandleNorthboundMessagesForDevice(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
	if(OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Firmware_Running(iDevice))
    {
        OMIINO_LLC_HandleNorthbound_Status_MessagesForDevice(iDevice, pFramerRAM);
        OMIINO_LLC_HandleNorthbound_PerformanceMonitoring_MessagesForDevice(iDevice, pFramerRAM);
        OMIINO_LLC_HandleNorthbound_Signalling_MessagesForDevice(iDevice, pFramerRAM);
        OMIINO_LLC_HandleNorthbound_OmniSpy_MessagesForDevice(iDevice, pFramerRAM);
    }
	else
	{
		if(1==OMIINO_RAM.Device[iDevice].TestMode)
		{
	        OMIINO_LLC_HandleNorthbound_OmniSpy_MessagesForDevice(iDevice, pFramerRAM);
		}
	}
    OMIINO_LLC_HandleNorthbound_AuxiliaryOmniSpy_MessagesForDevice(iDevice, pFramerRAM);
}


void OMIINO_LLC_NorthboundIntialize(OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    OMIINO_LLC_Northbound_AuxiliaryOmniSpyTableInitialize(&(pFramerRAM->LLC_Table.AuxiliaryTable));
    OMIINO_LLC_Northbound_SignallingTableInitialize(&(pFramerRAM->LLC_Table.SignallingTable));
    OMIINO_LLC_Northbound_StatusTableInitialize(&(pFramerRAM->LLC_Table.StatusTable));
    OMIINO_LLC_Northbound_PerformanceMonitoringTableInitialize(&(pFramerRAM->LLC_Table.PerformanceMonitoringTable));
	OMIINO_LLC_Northbound_OmnispyTableInitialize(&(pFramerRAM->LLC_Table.OmnispyTable));
}

