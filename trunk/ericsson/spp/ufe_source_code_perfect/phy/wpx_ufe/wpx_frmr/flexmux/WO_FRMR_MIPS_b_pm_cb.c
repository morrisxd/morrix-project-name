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





U8  WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iDevice=0;
U8  WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iTableIndex=0;
U32 WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_Timestamp=0;




void WPX_UFE_FRAMER_MIPS_SONET_SDH_PortPerformanceMonitoringAnnounce(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	char Payload[5];

	Payload[0]=iTableIndex; 
	UtilityPutU32(TimeStamp, (unsigned char *)&Payload[1]);

	OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_PORT_TIMESTAMP, Payload, 5);

	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iDevice=iDevice;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iTableIndex=iTableIndex;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_Timestamp=TimeStamp;

	WPX_UFE_FRAMER_MIPS_SONET_SDH_PortPerformanceMonitoringAnnounceData(iDevice, iTableIndex, TimeStamp);
}


void WPX_UFE_FRAMER_MIPS_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	char Payload[5];

	Payload[0]=iTableIndex; 
	UtilityPutU32(TimeStamp, (unsigned char *)&Payload[1]);

	OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_HO_TIMESTAMP, Payload, 5);

	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iDevice=iDevice;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iTableIndex=iTableIndex;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_Timestamp=TimeStamp;

	WPX_UFE_FRAMER_MIPS_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounceData(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_MIPS_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	char Payload[5];

	Payload[0]=iTableIndex; 
	UtilityPutU32(TimeStamp, (unsigned char *)&Payload[1]);

	OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_LO_TIMESTAMP, Payload, 5);

	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iDevice=iDevice;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iTableIndex=iTableIndex;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_Timestamp=TimeStamp;

	WPX_UFE_FRAMER_MIPS_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounceData(iDevice, iTableIndex, TimeStamp);
}

void WPX_UFE_FRAMER_MIPS_PerformanceMonitoringCallbackTable_DiscreteClientPortPerformanceMonitoringAnnounce(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iDevice=iDevice;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iTableIndex=iTableIndex;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_Timestamp=TimeStamp;
}


void WPX_UFE_FRAMER_MIPS_PDH_PerformanceMonitoringAnnounce(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iDevice=iDevice;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_iTableIndex=iTableIndex;
	WPX_UFE_FRAMER_MIPS_PerformanceMonitoring_Timestamp=TimeStamp;
}


void WPX_UFE_FRAMER_MIPS_Initialize_BindingPerformanceMonitoringCallbackTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_ANNOUNCE_TYPE * pPerformanceMonitoringCallbackTable)
{
	pPerformanceMonitoringCallbackTable->DiscreteClientPortPerformanceMonitoringAnnounce=WPX_UFE_FRAMER_MIPS_PerformanceMonitoringCallbackTable_DiscreteClientPortPerformanceMonitoringAnnounce;
	pPerformanceMonitoringCallbackTable->PDH_PerformanceMonitoringAnnounce=WPX_UFE_FRAMER_MIPS_PDH_PerformanceMonitoringAnnounce;
	pPerformanceMonitoringCallbackTable->SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce=WPX_UFE_FRAMER_MIPS_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounce;
	pPerformanceMonitoringCallbackTable->SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce=WPX_UFE_FRAMER_MIPS_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounce;
	pPerformanceMonitoringCallbackTable->SONET_SDH_PortPerformanceMonitoringAnnounce=WPX_UFE_FRAMER_MIPS_SONET_SDH_PortPerformanceMonitoringAnnounce;
}
