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
#include "WO_FRMR_firmware_image.h"


#ifndef XALYO_FRAMER_ACCESS
int Verify(U8 iDevice, U8 Length)
{
    int rc = 1 ;
    int i ;

    for(i = 0 ; i < Length ; i++)
    {
        if (OMIINO_RAM.Device[iDevice].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].RAM[i]!=OMIINO_RAM.FirmwareDownload[iDevice].Buffer[i])
        {
            rc = 0 ;
            break;
        }
    }

    return rc ;
}
#else
int Verify(U8 iDevice, U8 Length)
{
    int rc = 1 ;
    int i ;
    volatile unsigned int  temp;
    for(i = 0 ; i < Length ; i++)
    {
        WP_MEM_GET(temp, OMIINO_RAM.Device[iDevice].MemoryMap.Address[WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION].RAM[i]);
        if (temp!=OMIINO_RAM.FirmwareDownload[iDevice].Buffer[i])
        {
            rc = 0 ;
            break;
        }
    }

    return rc ;
}
#endif



void OMIINO_FRAMER_FIRMWARE_DOWNLOAD_SendBuildPersonalityOnFirmwareRunning(U8 iDevice)
{
	U8 HardwareBuildVariant;
	OMIINO_FRAMER_STATUS_DEVICE_TYPE * pDeviceStatusRAM;   

    pDeviceStatusRAM = OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, iDevice);
    OMIINO_FRAMER_STATUS_DeviceDriver_GetHardwareVariant(pDeviceStatusRAM, &HardwareBuildVariant);
	
	OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_DEVICE_READY(iDevice, HardwareBuildVariant);
}


void OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Next_Packet(U8 iDevice)
{
	U8 Length;
    int i;

    for(i=0;i<(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4);i++)
    {
        OMIINO_RAM.FirmwareDownload[iDevice].Buffer[i]=0xFFFFFFFF;
    }

	Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_BOOTLOADER_DATA_PACKET_Send(0,&UFE4_FIRMWARE_IMAGE_DATA_TYPE[OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount][0],
							/*FIRMWARE_LOAD_UFE4_FIRMWARE_IMAGE_DATA_TYPE_BLOCK_SIZE*4*/ 96, OMIINO_RAM.FirmwareDownload[iDevice].Buffer);

	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION,OMIINO_RAM.FirmwareDownload[iDevice].Buffer,Length);
	}

    

	OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount++;

	return;
}

void OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Run_Command(U8 iDevice)
{
	U8 Length;
    int i;

    for(i=0;i<(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4);i++)
    {
        OMIINO_RAM.FirmwareDownload[iDevice].Buffer[i]=0x00000000;
    }

	Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_BOOTLOADER_START_FIRMWARE_Send(0,OMIINO_RAM.FirmwareDownload[iDevice].Buffer);

	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap,WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION,OMIINO_RAM.FirmwareDownload[iDevice].Buffer,Length);
	}

	return;
}

void OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Initialize_Device( U8 iDevice, U8 TestMode )
{
	if(0==TestMode)
	{
		OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_HELLO ;
	}
	else
	{
		OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_TEST_MODE_NO_FIRMWARE_BOOTLOADER_RUNNING ;
	}

	OMIINO_RAM.FirmwareDownload[iDevice].LastBootHelloCount = 0;
	OMIINO_RAM.FirmwareDownload[iDevice].LastFirmwareHelloCount = 0;
	OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount = 0;
    OMIINO_RAM.FirmwareDownload[iDevice].SuspectPacketCount = 0;

	memset(OMIINO_RAM.FirmwareDownload[iDevice].Buffer, 0, WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX);

}

void OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Initialize(U8 TestMode)
{
	U8 i ;

	for( i = 0 ; i < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES ; i++ )
	{
		OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Initialize_Device(i, TestMode);
	}
}

U8 OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Bootloader_Hello(U8 iDevice, U32 count)
{
	U8 rc = WPX_UFE_FRAMER_OK ;

	switch(OMIINO_RAM.FirmwareDownload[iDevice].CurrentState)
	{
		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_HELLO:
			OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount=0;
			OMIINO_RAM.FirmwareDownload[iDevice].LastBootHelloCount = count;
			OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Next_Packet(iDevice);
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK;
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK:
			if (count < OMIINO_RAM.FirmwareDownload[iDevice].LastBootHelloCount)
			{
				OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount=0;
				OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Next_Packet(iDevice);
				OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK;
			}
			OMIINO_RAM.FirmwareDownload[iDevice].LastBootHelloCount = count;
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_PENDING:
		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING:
			OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount=0;
			OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Next_Packet(iDevice);
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK;
			break;

		case OMIINO_FRAMER_TEST_MODE_NO_FIRMWARE_BOOTLOADER_RUNNING:
			/* Do Nothing */
			break;

		default:
			break;
	}

	return rc;
}

U8 OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Firmware_Hello(U8 iDevice, U32 count)
{
	U8 rc = WPX_UFE_FRAMER_OK ;

	switch(OMIINO_RAM.FirmwareDownload[iDevice].CurrentState)
	{
		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_HELLO:
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING;
			OMIINO_RAM.FirmwareDownload[iDevice].LastFirmwareHelloCount = count;
			OMIINO_FRAMER_FIRMWARE_DOWNLOAD_SendBuildPersonalityOnFirmwareRunning(iDevice);
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK:
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING;
			OMIINO_RAM.FirmwareDownload[iDevice].LastFirmwareHelloCount = count;
			OMIINO_FRAMER_FIRMWARE_DOWNLOAD_SendBuildPersonalityOnFirmwareRunning(iDevice);
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_PENDING:
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING;
			OMIINO_RAM.FirmwareDownload[iDevice].LastFirmwareHelloCount = count;
			OMIINO_FRAMER_FIRMWARE_DOWNLOAD_SendBuildPersonalityOnFirmwareRunning(iDevice);
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING:
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING;
			OMIINO_RAM.FirmwareDownload[iDevice].LastFirmwareHelloCount = count;
			OMIINO_FRAMER_FIRMWARE_DOWNLOAD_SendBuildPersonalityOnFirmwareRunning(iDevice);
			break;

		case OMIINO_FRAMER_TEST_MODE_NO_FIRMWARE_BOOTLOADER_RUNNING:
			/* Do Nothing */
			break;

		default:
			break;
	}

	return rc;
}

U8 OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Ack(U8 iDevice, U32 count)
{
	U8 rc = WPX_UFE_FRAMER_OK ;

	switch(OMIINO_RAM.FirmwareDownload[iDevice].CurrentState)
	{
		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_HELLO:
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK:
			if ( count == OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount )
			{
				if (FIRMWARE_LOAD_UFE4_FIRMWARE_IMAGE_DATA_TYPE_NUMBER_OF_BLOCKS == OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount)
				{
					OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_PENDING;
					OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Run_Command(iDevice);
				}
				else
				{
					OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Send_Next_Packet(iDevice) ;
				}
			}
			else
			{
					/* TODO: Revisit when watchdog available */
			}
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_PENDING:
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING:
			break;

		case OMIINO_FRAMER_TEST_MODE_NO_FIRMWARE_BOOTLOADER_RUNNING:
			/* Do Nothing */
			break;

		default:
			break;
	}

	return rc;
}

U8 OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Nak(U8 iDevice)
{
	U8 rc = WPX_UFE_FRAMER_OK ;

	switch(OMIINO_RAM.FirmwareDownload[iDevice].CurrentState)
	{
		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_HELLO:
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_ACK:
			OMIINO_RAM.FirmwareDownload[iDevice].CurrentState = OMIINO_FRAMER_FIRMWARE_DOWNLOAD_WAITING_ON_HELLO;
			OMIINO_RAM.FirmwareDownload[iDevice].LastBootHelloCount = 0;
			OMIINO_RAM.FirmwareDownload[iDevice].LastPacketCount = 0;
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_PENDING:
			break;

		case OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING:
			break;

		case OMIINO_FRAMER_TEST_MODE_NO_FIRMWARE_BOOTLOADER_RUNNING:
			/* Do Nothing */
			break;

		default:
			break;
	}


	return rc;
}

U8 OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Firmware_Running(U8 iDevice)
{
	return (OMIINO_RAM.FirmwareDownload[iDevice].CurrentState == OMIINO_FRAMER_FIRMWARE_DOWNLOAD_RUNNING); 
}


