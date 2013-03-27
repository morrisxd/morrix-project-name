#ifndef  _WPX_UFE_FRAMER_MIPS_PRIVATE_
#define  _WPX_UFE_FRAMER_MIPS_PRIVATE_


/*
	About: Copyright

	Copyright (c) 2010  Omiino Ltd

	All rights reserved.  

	This code is provided under license and or Non-disclosure     
	Agreement and must be used solely for the purpose for which it
	was provided. 

	It must not be passed to any third party without
	the written permission of Omiino Ltd.   

	Visit <http://www.omiino.com> or send messages to support@omiino.com.


*/


#ifdef __cplusplus
extern "C"
{
#endif

#define WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH	(3)
#define WPX_UFE_FRAMER_MIPS_CONST_FOREVER		(1)

extern int WPX_UFE_FRAMER_MIPS_RequestedPersonality;

typedef struct WPX_UFE_FRAMER_MIPS_BOARD_TYPE
{
    WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE					EnvironmentTable;
    WPX_UFE_FRAMER_AUTONOMOUS_OUTPUT_TABLE_TYPE				AutonomousOutputTable;
    WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE	PerformanceMonitoringDeviceTable;
    WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE		DeviceLevelDiagnosticTable;
    WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE						OmniSpyTable;
    WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE					MailboxMemory[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES];

} WPX_UFE_FRAMER_MIPS_BOARD_TYPE;


void WPX_UFE_FRAMER_MIPS_StartDriver(int AnyPersonality);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingAlarmCallbackTable(WPX_UFE_FRAMER_ALARM_OUTPUT_TABLE_ANNOUNCE_TYPE * pAlarmCallbackTable);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingDiagnosticCallbackTable(WPX_UFE_FRAMER_DEVICE_LEVEL_DIAGNOSTIC_TABLE_TYPE * pDiagnosticCallbackTable);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingEnvironment(WPX_UFE_FRAMER_ENVIRONMENT_TABLE_TYPE * pEnvironmentTable);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingOmnispyCallbackTable(WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE * pOmnispyCallbackTable);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingPerformanceMonitoringCallbackTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_ANNOUNCE_TYPE * pPerformanceMonitoringCallbackTable);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingPerformanceMonitoringDataTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_RAM);
void WPX_UFE_FRAMER_MIPS_Initialize_BindingSignallingCallbackTable(WPX_UFE_FRAMER_SIGNALLING_TABLE_ANNOUNCE_TYPE * pSignallingCallbackTable);
void WPX_UFE_FRAMER_MIPS_Initialize_MemoryMap(WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES]);
void WPX_UFE_FRAMER_MIPS_MonitorLoop(void);
char * WPX_UFE_FRAMER_MIPS_CommonLineEndpointToTxt(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType);

void WPX_UFE_FRAMER_MIPS_SONET_SDH_PortPerformanceMonitoringAnnounceData(U8 iDevice, U8 iTableIndex, U32 TimeStamp);
void WPX_UFE_FRAMER_MIPS_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounceData(U8 iDevice, U8 iTableIndex, U32 TimeStamp);
void WPX_UFE_FRAMER_MIPS_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounceData(U8 iDevice, U8 iTableIndex, U32 TimeStamp);

void WPX_UFE_FRAMER_MIPS_ServiceNorthboundLowLevelComms(void);
void WPX_UFE_FRAMER_MIPS_Enable_MailboxCommunicationChannels(void);


void WPX_UFE_FRAMER_MIPS_BoardLevel_MailboxEnter(void);
void WPX_UFE_FRAMER_MIPS_BoardLevel_MailboxExit(void);
void WPX_UFE_FRAMER_MIPS_BoardLevel_Initialization(void);

void WPX_UFE_FRAMER_605_Initialize_MemoryMap(WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE (*pMailboxMemoryMap)[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES]);



/* Debug MIPS environment */
void WPX_UFE_FRAMER_MIPS_DisplayChipSelect_Serial_1(void);
void WPX_UFE_FRAMER_MIPS_BoardSupportReset(void);
void WPX_UFE_FRAMER_MIPS_Delay(unsigned int AnyTimeInterval);
void WPX_UFE_FRAMER_MIPS_Message(char * AnyMessage);
void WPX_UFE_FRAMER_MIPS_ReportMailboxError(char * AnyMessage);





#ifdef __cplusplus
}
#endif


#endif /* _WPX_UFE_FRAMER_MIPS_PRIVATE_ */


