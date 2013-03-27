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

#include <stdio.h>
#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"
#include "WO_FRMR_API_public.h"


int WPX_UFE_FRAMER_MIPS_RequestedPersonality=WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12;


void WPX_UFE_FRAMER_MIPS_Message(char * AnyMessage)
{
	printf("%s\n", AnyMessage);
}


void WPX_UFE_FRAMER_MIPS_ReportMailboxError(char * AnyMessage)
{
	printf("%s\n", AnyMessage);
}



char DeviceDriverInformation[256];
void WPX_UFE_FRAMER_MIPS_ReportVersionInformation(void)
{
	WPX_UFE_FRAMER_MIPS_Message("\n\n\n");
	if(WPX_UFE_FRAMER_OK==WPX_FRMR_STATUS_DEV_DRV_VERSION_GetInfo(DeviceDriverInformation))
	{
		WPX_UFE_FRAMER_MIPS_Message(DeviceDriverInformation);
	}
	else
	{
		WPX_UFE_FRAMER_MIPS_Message("[XILINX] Device Driver Information Unavailable\n");
	}
	WPX_UFE_FRAMER_MIPS_Message("\n\n\n");
}



void WPX_UFE_FRAMER_MIPS_DebugHook(void)
{
	OMIINO_LLC_BindMailboxCopyErrorHandler(WPX_UFE_FRAMER_MIPS_ReportMailboxError);

	WPX_UFE_FRAMER_MIPS_DisplayChipSelect_Serial_1();
	
}



U32 iTestNumber=0;
U32 iTestCase=0;
char MessageBuffer[128];

void WPX_UFE_FRAMER_MIPS_MonitorLoopHook(unsigned char TestMode)
{
	unsigned int Address;
	unsigned int Value;
	unsigned int Writes;

	if(1==TestMode)
	{
		switch(iTestCase)
		{
			case 0:
				/*if(iTestNumber<16)
				{
					Address=((0x10000000)+(iTestNumber*4));
					Value=0x00010000+iTestNumber;

					sprintf(MessageBuffer, "[XILINX] Device Driver TestMode Poke Test %d Address=%08x Value=%08x\n", iTestNumber, Address, Value);
					WPX_UFE_FRAMER_MIPS_Message(MessageBuffer);
					WPX_FRMR_DEV_DIAG_Poke(iTestNumber, 0, Address, Value);
					WPX_FRMR_DEV_DIAG_Peek(0, Address);
					iTestNumber++;
					WPX_UFE_FRAMER_MIPS_Delay(500);
				}
				else
				{
					iTestCase++;
					iTestNumber=0;
				}
				break;

			case 1:
				if(iTestNumber<16)
				{
					Address=((0x12000000)+(iTestNumber*4));
					Value=0x00020000+iTestNumber;

					sprintf(MessageBuffer, "[XILINX] Device Driver TestMode Poke Test %d Address=%08x Value=%08x\n", iTestNumber, Address, Value);
					WPX_UFE_FRAMER_MIPS_Message(MessageBuffer);
					WPX_FRMR_DEV_DIAG_Poke(iTestNumber, 0, Address, Value);
					WPX_FRMR_DEV_DIAG_Peek(0, Address);
					iTestNumber++;
					WPX_UFE_FRAMER_MIPS_Delay(500);
				}
				else
				{
					iTestCase++;
					iTestNumber=0;
				}
				break;

			case 2:
				Address=0x10000100;
				Value=0x0A0A0A0A;
				Writes=16;
				sprintf(MessageBuffer, "[XILINX] Device Driver TestMode Block Poke Address=%08x Writes=%08x Value=%08x\n", Address, Writes, Value);
				WPX_FRMR_DEV_DIAG_BlockPoke(200, 0, Address, Writes, Value);
				iTestCase++;
				WPX_UFE_FRAMER_MIPS_Delay(1000);
				break;

			case 3:
				Address=0x12000100;
				Value=0x0B0B0B0B;
				Writes=16;
				sprintf(MessageBuffer, "[XILINX] Device Driver TestMode Block Poke Address=%08x Writes=%08x Value=%08x\n", Address, Writes, Value);
				WPX_FRMR_DEV_DIAG_BlockPoke(300, 0, Address, Writes, Value);
				iTestCase++;
				WPX_UFE_FRAMER_MIPS_Delay(1000);
				break;

			case 4:*/
				Address=0x11000000;
				Value=0x64646464;
				Writes=1024;
				sprintf(MessageBuffer, "[XILINX] Device Driver TestMode Block Poke Verify Address=%08x Writes=%08x Value=%08x\n", Address, Writes, Value);
				WPX_FRMR_DEV_DIAG_BlockPokeVerify(400, 0, Address, Writes, Value);
				iTestCase++;
				break;

			default:
				break;
		}

	}
}



volatile U32 WPX_UFE_FRAMER_MIPS_MonitorLoopForever=WPX_UFE_FRAMER_MIPS_CONST_FOREVER;

void WPX_UFE_FRAMER_MIPS_MonitorLoop(void)
{
	while(WPX_UFE_FRAMER_MIPS_MonitorLoopForever)
	{
		WPX_UFE_FRAMER_MIPS_MonitorLoopHook(0);
		WPX_UFE_FRAMER_MIPS_ServiceNorthboundLowLevelComms();
	}
}


void WPX_UFE_FRAMER_MIPS_MonitorLoopTestMode(void)
{
	while(WPX_UFE_FRAMER_MIPS_MonitorLoopForever)
	{
		WPX_UFE_FRAMER_MIPS_MonitorLoopHook(1);
		WPX_UFE_FRAMER_MIPS_ServiceNorthboundLowLevelComms();
	}
}





WPX_UFE_FRAMER_MIPS_BOARD_TYPE WPX_UFE_FRAMER_MIPS_Board;


void WPX_UFE_FRAMER_605_Patch(void)
{
	/* Test MIPS environment within 605 build - comment out when running on a genuine MIPS environment */
	WPX_UFE_FRAMER_605_Initialize_MemoryMap(&WPX_UFE_FRAMER_MIPS_Board.MailboxMemory);
}


void WPX_UFE_FRAMER_MIPS_Initialize(void)
{
	WPX_UFE_FRAMER_MIPS_Initialize_BindingAlarmCallbackTable(&WPX_UFE_FRAMER_MIPS_Board.AutonomousOutputTable.AutonomousAlarmTable);
	WPX_UFE_FRAMER_MIPS_Initialize_BindingDiagnosticCallbackTable(&WPX_UFE_FRAMER_MIPS_Board.DeviceLevelDiagnosticTable);
	WPX_UFE_FRAMER_MIPS_Initialize_BindingEnvironment(&WPX_UFE_FRAMER_MIPS_Board.EnvironmentTable);
	WPX_UFE_FRAMER_MIPS_Initialize_BindingOmnispyCallbackTable(&WPX_UFE_FRAMER_MIPS_Board.OmniSpyTable);
	WPX_UFE_FRAMER_MIPS_Initialize_BindingPerformanceMonitoringCallbackTable(&WPX_UFE_FRAMER_MIPS_Board.AutonomousOutputTable.AutonomousPerformanceMonitoringTable);
	WPX_UFE_FRAMER_MIPS_Initialize_BindingPerformanceMonitoringDataTable(&WPX_UFE_FRAMER_MIPS_Board.PerformanceMonitoringDeviceTable);
	WPX_UFE_FRAMER_MIPS_Initialize_BindingSignallingCallbackTable(&WPX_UFE_FRAMER_MIPS_Board.AutonomousOutputTable.AutonomousSignallingTable);
	WPX_UFE_FRAMER_MIPS_Initialize_MemoryMap(&WPX_UFE_FRAMER_MIPS_Board.MailboxMemory);


	/* Add this call if MIPS code is to be tested on 605 board WPX_UFE_FRAMER_605_Patch(); */	
}


void WPX_UFE_FRAMER_MIPS_StartDriver(int AnyPersonality)
{
	WPX_UFE_FRAMER_MIPS_RequestedPersonality=AnyPersonality;

	WPX_UFE_FRAMER_MIPS_DebugHook();

	WPX_UFE_FRAMER_MIPS_Initialize();

	WPX_UFE_FRAMER_Initialize(		&WPX_UFE_FRAMER_MIPS_Board.MailboxMemory, 
									&WPX_UFE_FRAMER_MIPS_Board.EnvironmentTable,
									&WPX_UFE_FRAMER_MIPS_Board.AutonomousOutputTable,
									&WPX_UFE_FRAMER_MIPS_Board.PerformanceMonitoringDeviceTable,
									&WPX_UFE_FRAMER_MIPS_Board.DeviceLevelDiagnosticTable,
									&WPX_UFE_FRAMER_MIPS_Board.OmniSpyTable,
									0);

	WPX_UFE_FRAMER_MIPS_Enable_MailboxCommunicationChannels();

	LowLevelComms_DefineAccess();

	WPX_UFE_FRAMER_MIPS_ReportVersionInformation();
	WPX_UFE_FRAMER_MIPS_Message("[>] WPX_UFE_FRAMER_MIPS_MonitorLoop");
	WPX_UFE_FRAMER_MIPS_MonitorLoop();
	WPX_UFE_FRAMER_MIPS_Message("[<] WPX_UFE_FRAMER_MIPS_MonitorLoop");
}



void WPX_UFE_FRAMER_MIPS_StartDriver_TestMode(int AnyPersonality)
{
	WPX_UFE_FRAMER_MIPS_RequestedPersonality=AnyPersonality;

	WPX_UFE_FRAMER_MIPS_DebugHook();

	WPX_UFE_FRAMER_MIPS_Initialize();

	WPX_UFE_FRAMER_Initialize(		&WPX_UFE_FRAMER_MIPS_Board.MailboxMemory, 
									&WPX_UFE_FRAMER_MIPS_Board.EnvironmentTable,
									&WPX_UFE_FRAMER_MIPS_Board.AutonomousOutputTable,
									&WPX_UFE_FRAMER_MIPS_Board.PerformanceMonitoringDeviceTable,
									&WPX_UFE_FRAMER_MIPS_Board.DeviceLevelDiagnosticTable,
									&WPX_UFE_FRAMER_MIPS_Board.OmniSpyTable,
									1);

	WPX_UFE_FRAMER_MIPS_Enable_MailboxCommunicationChannels();

	LowLevelComms_DefineAccess();

	WPX_UFE_FRAMER_MIPS_ReportVersionInformation();
	WPX_UFE_FRAMER_MIPS_Message("[>] WPX_UFE_FRAMER_MIPS_MonitorLoop");
	WPX_UFE_FRAMER_MIPS_MonitorLoopTestMode();
	WPX_UFE_FRAMER_MIPS_Message("[<] WPX_UFE_FRAMER_MIPS_MonitorLoop");
}
