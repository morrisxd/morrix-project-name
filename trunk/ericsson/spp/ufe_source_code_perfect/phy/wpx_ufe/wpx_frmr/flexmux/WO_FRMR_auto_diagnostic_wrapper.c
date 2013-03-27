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





void OMIINO_DIAGNOSTIC_WRAPPER_BindCallbacks(OMIINO_DEVICE_DIAGNOSTIC_PRIVATE_ANNOUNCE_TYPE *pTo, WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE *pFrom)
{
   U8 j;

   for (j = 0; j < WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; ++j)
      pTo->DeviceIsReadyReported[j] = 0;

   pTo->DeviceReady=pFrom->DeviceReady;
   pTo->FirmwareRestart=pFrom->FirmwareRestart;
   pTo->Callback=pFrom->DeviceDiagnostic;
   pTo->PeekResponse=pFrom->PeekResponse;
   pTo->BlockPokeVerifyResponse=pFrom->BlockPokeVerifyResponse;
   pTo->AlarmMapResponse=pFrom->AlarmMapResponse;
   pTo->SSBitsGetRxResponse=pFrom->SSBitsGetRxResponse;
   pTo->EnterCount=0;
   pTo->ExitCount=0;
   pTo->UnBoundCalls=0;
}



void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_PEEK_RESPONSE(U8 iDevice, U32 Address, U32 Value)
{
    if(OMIINO_RAM.DeviceLevelDiagnosticTable.PeekResponse)
    {
		OMIINO_RAM.DeviceLevelDiagnosticTable.EnterCount++;
		(*OMIINO_RAM.DeviceLevelDiagnosticTable.PeekResponse)(iDevice, Address, Value);
		OMIINO_RAM.DeviceLevelDiagnosticTable.ExitCount++;
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}


void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_ALARM_MAP_RESPONSE(U8 iDevice)
{
    if(OMIINO_RAM.DeviceLevelDiagnosticTable.AlarmMapResponse)
    {
		OMIINO_RAM.DeviceLevelDiagnosticTable.EnterCount++;
		(*OMIINO_RAM.DeviceLevelDiagnosticTable.AlarmMapResponse)(iDevice);
		OMIINO_RAM.DeviceLevelDiagnosticTable.ExitCount++;
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}

void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_SS_BITS_RESPONSE(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType,U8 Value)
{
    if(OMIINO_RAM.DeviceLevelDiagnosticTable.SSBitsGetRxResponse)
    {
		OMIINO_RAM.DeviceLevelDiagnosticTable.EnterCount++;
		(*OMIINO_RAM.DeviceLevelDiagnosticTable.SSBitsGetRxResponse)(iDevice,pCommon_SDH_SONET_LineEndpointType,Value);
		OMIINO_RAM.DeviceLevelDiagnosticTable.ExitCount++;
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}


void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_BLOCK_POKE_VERIFY_RESPONSE(U8 iDevice, U32 VerifyResult)
{
	if(OMIINO_RAM.DeviceLevelDiagnosticTable.BlockPokeVerifyResponse)
    {
		OMIINO_RAM.DeviceLevelDiagnosticTable.EnterCount++;
		(*OMIINO_RAM.DeviceLevelDiagnosticTable.BlockPokeVerifyResponse)(iDevice, VerifyResult);
		OMIINO_RAM.DeviceLevelDiagnosticTable.ExitCount++;
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}


void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_FIRMWARE_RESTART(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
    if(OMIINO_RAM.DeviceLevelDiagnosticTable.FirmwareRestart)
    {
		OMIINO_RAM.DeviceLevelDiagnosticTable.EnterCount++;
		(*OMIINO_RAM.DeviceLevelDiagnosticTable.FirmwareRestart)(iDevice, Reported_HardwareBuildVariant);
		OMIINO_RAM.DeviceLevelDiagnosticTable.ExitCount++;
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}


void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_DEVICE_READY(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
    if(OMIINO_RAM.DeviceLevelDiagnosticTable.DeviceReady)
    {
		if(0==OMIINO_RAM.DeviceLevelDiagnosticTable.DeviceIsReadyReported[iDevice])
		{
			OMIINO_RAM.DeviceLevelDiagnosticTable.DeviceIsReadyReported[iDevice]++;

			OMIINO_RAM.DeviceLevelDiagnosticTable.EnterCount++;
			(*OMIINO_RAM.DeviceLevelDiagnosticTable.DeviceReady)(iDevice, Reported_HardwareBuildVariant);
			OMIINO_RAM.DeviceLevelDiagnosticTable.ExitCount++;
		}
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}



void OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_TYPE(U8 iDevice, U8 DiagnosticAlarmCategory, U8 IsAsserted)
{
    if(OMIINO_RAM.DeviceLevelDiagnosticTable.Callback)
    {
        (*OMIINO_RAM.DeviceLevelDiagnosticTable.Callback)(iDevice, DiagnosticAlarmCategory, IsAsserted);
    }
    else
    {
        OMIINO_RAM.DeviceLevelDiagnosticTable.UnBoundCalls++;
    }
}



