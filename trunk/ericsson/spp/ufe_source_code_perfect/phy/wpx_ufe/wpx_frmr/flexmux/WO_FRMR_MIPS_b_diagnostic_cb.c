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
#include "WO_FRMR_MIPS_private.h"



void WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_DeviceDiagnostic(U8 iDevice, U8 DiagnosticAlarmCategory, U8 IsAsserted)
{
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);
	OMIINO_REMOVE_COMPILER_WARNING(DiagnosticAlarmCategory);
	OMIINO_REMOVE_COMPILER_WARNING(IsAsserted);
}

void WPX_UFE_FRAMER_MIPS_DoSetBuildPersonality(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
	switch(Reported_HardwareBuildVariant)
	{

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_1_OC48_6:
				WPX_FRMR_DEV_SetBuildPersonality(iDevice, (U8)WPX_UFE_FRAMER_MIPS_RequestedPersonality);
				WPX_UFE_FRAMER_MIPS_Message("[XILINX] Device Driver Configure DEMO ONLY GTP Clock Source\n");
				WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(iDevice);
				OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_ALL(0, iDevice);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_2_OC24_6:
				/* TODO */
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_3_OC12_6:
				WPX_FRMR_DEV_SetBuildPersonality(iDevice, (U8)WPX_UFE_FRAMER_MIPS_RequestedPersonality);
				WPX_UFE_FRAMER_MIPS_Message("[XILINX] Device Driver Configure DEMO ONLY GTP Clock Source\n");
				WPX_FRMR_DEMO_ONLY_Configure_GTP_ClockSource(iDevice);
				OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_ALL(0, iDevice);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_4_DISCRETE_ONLY_6:
				/* TODO */
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_5_OC48_6:
				/* TODO */
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_6_MIXED_6:
				/* TODO */
				break;

		default:
				break;
	}
}



void WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_DeviceReady(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
	WPX_UFE_FRAMER_MIPS_DoSetBuildPersonality(iDevice, Reported_HardwareBuildVariant);
}



void WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_FirmwareRestart(U8 iDevice, unsigned char Reported_HardwareBuildVariant)
{
	WPX_UFE_FRAMER_MIPS_DoSetBuildPersonality(iDevice, Reported_HardwareBuildVariant);
}



U32 WPX_UFE_FRAMER_MIPS_PeekResponseCount=0;

void WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_PeekResponse(U8 iDevice, U32 Address, U32 Value)
{
	char LocalBuffer[128];

    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
	sprintf(LocalBuffer, "[XILINX] PEEK RESPONSE Count %d Address=%08x Value=%08x", WPX_UFE_FRAMER_MIPS_PeekResponseCount++, Address, Value);
	WPX_UFE_FRAMER_MIPS_Message(LocalBuffer);
}



void WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_BlockPokeVerfiyResponse(U8 iDevice, U32 VerifyResult)
{
	char LocalBuffer[128];

    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
	sprintf(LocalBuffer, "[XILINX] BLOCK POKE VERIFY RESPONSE Result=%08x", VerifyResult);
	WPX_UFE_FRAMER_MIPS_Message(LocalBuffer);
}





void WPX_UFE_FRAMER_MIPS_Initialize_BindingDiagnosticCallbackTable(WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE * pDiagnosticCallbackTable)
{
	pDiagnosticCallbackTable->DeviceDiagnostic=WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_DeviceDiagnostic;
	pDiagnosticCallbackTable->DeviceReady=WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_DeviceReady;
	pDiagnosticCallbackTable->FirmwareRestart=WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_FirmwareRestart;
	pDiagnosticCallbackTable->PeekResponse=WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_PeekResponse;
	pDiagnosticCallbackTable->BlockPokeVerifyResponse=WPX_UFE_FRAMER_MIPS_DiagnosticCallbackTable_BlockPokeVerfiyResponse;
}



