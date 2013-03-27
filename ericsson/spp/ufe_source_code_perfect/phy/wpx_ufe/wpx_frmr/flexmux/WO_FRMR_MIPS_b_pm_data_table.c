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


WPX_UFE_FRAMER_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ENDPOINT_TYPE		WPX_UFE_FRAMER_MIPS_PM_RAM_DiscreteClientTable_Device[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES][WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH];
WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_HO_PATH_ENDPOINT_TYPE	WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES][WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH];
WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_LO_PATH_ENDPOINT_TYPE	WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES][WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH];
WPX_UFE_FRAMER_PDH_PERFORMANCE_MONITORING_ENDPOINT_TYPE					WPX_UFE_FRAMER_MIPS_PM_RAM_PDH_Table_Device[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES][WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH];
WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_PORT_ENDPOINT_TYPE		WPX_UFE_FRAMER_MIPS_PM_RAM_PortTable_Device[WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES][WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH];



/*
 * NOTE: This is test code. It is not recommended that the data be retrieved in the context of the callback.
 *       With large number of points enabled this will block the device driver (only using single point of each type for testing).
 *       Processing should be performed in a separate context.
 */

#define WPX_UFE_FRAMER_MAX_PM_PORTS_UNDER_TEST	2


void WPX_UFE_FRAMER_MIPS_SONET_SDH_PortPerformanceMonitoringAnnounceData(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	U8 iLinePort;
	U8 iPoint;
	char Payload[10];

	if(WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH>iTableIndex)
	{
		for(iLinePort=0; iLinePort<WPX_UFE_FRAMER_MAX_PM_PORTS_UNDER_TEST; iLinePort++)
		{
			for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT; iPoint++)
			{
				if(1==WPX_UFE_FRAMER_MIPS_PM_RAM_PortTable_Device[iDevice][iTableIndex].IsMonitored[iLinePort][iPoint])
				{
					Payload[0]=iLinePort; 	
					Payload[1]=iPoint; 
					UtilityPutU32(WPX_UFE_FRAMER_MIPS_PM_RAM_PortTable_Device[iDevice][iTableIndex].Count[iLinePort][iPoint], (unsigned char *)&Payload[2]);
					UtilityPutU32(TimeStamp, (unsigned char *)&Payload[6]);

					OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_PORT_READ, Payload, 10);
				}
			}
		}
	}
}


void WPX_UFE_FRAMER_MIPS_SONET_SDH_HighOrderPathPerformanceMonitoringAnnounceData(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	U8 iLinePort;
	U8 iPoint;
	U8 iJ=0;
	U8 iK=0;
	U8 iU=0;
	char Payload[12];

	if(WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH>iTableIndex)
	{
		for(iLinePort=0; iLinePort<WPX_UFE_FRAMER_MAX_PM_PORTS_UNDER_TEST; iLinePort++)
		{
			if(WPX_UFE_FRAMER_DEVICE_MODE_SDH==WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[iDevice][iTableIndex].PortMode)
			{
				for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH; iPoint++)
				{
					if(1==WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[iDevice][iTableIndex].IsMonitored.SDH[iLinePort][iJ][iK][iPoint]) /* TODO iterate through iJ & iK */
					{
						Payload[0]=iLinePort;
						Payload[1]=iJ;
						Payload[2]=iK;
						Payload[3]=iPoint; 
						UtilityPutU32(WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[iDevice][iTableIndex].Count.SDH[iLinePort][iJ][iK][iPoint], (unsigned char *)&Payload[4]);
						UtilityPutU32(TimeStamp, (unsigned char *)&Payload[8]);

						OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_SDH_HO_READ, Payload, 12);
					}
				}
			}
			else
			{
				for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH; iPoint++)
				{
					if(1==WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[iDevice][iTableIndex].IsMonitored.SONET[iLinePort][iU][iPoint]) /* TODO iterate through iU */
					{
						Payload[0]=iLinePort;
						Payload[1]=iU;
						Payload[2]=iPoint; 
						UtilityPutU32(WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[iDevice][iTableIndex].Count.SONET[iLinePort][iU][iPoint], (unsigned char *)&Payload[3]);
						UtilityPutU32(TimeStamp, (unsigned char *)&Payload[7]);

						OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_SONET_HO_READ, Payload, 11);
					}
				}
			}
		}
	}
}

void WPX_UFE_FRAMER_MIPS_SONET_SDH_LowOrderPathPerformanceMonitoringAnnounceData(U8 iDevice, U8 iTableIndex, U32 TimeStamp)
{
	U8 iLinePort;
	U8 iPoint;
	U8 iJ=0;
	U8 iK=0;
	U8 iL=0;
	U8 iM=0;
	U8 iU=0;
	U8 iV=0;
	U8 iW=0;
	char Payload[14];

	if(WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH>iTableIndex)
	{
		for(iLinePort=0; iLinePort<WPX_UFE_FRAMER_MAX_PM_PORTS_UNDER_TEST; iLinePort++)
		{
			if(WPX_UFE_FRAMER_DEVICE_MODE_SDH==WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[iDevice][iTableIndex].PortMode)
			{
				for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH; iPoint++)
				{
					if(1==WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[iDevice][iTableIndex].IsMonitored.SDH[iLinePort][iJ][iK][iL][iM][iPoint]) /* TODO iterate through iJ, iK, iL & iM */
					{
						Payload[0]=iLinePort;
						Payload[1]=iJ;
						Payload[2]=iK;
						Payload[3]=iL;
						Payload[4]=iM;
						Payload[5]=iPoint; 
						UtilityPutU32(WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[iDevice][iTableIndex].Count.SDH[iLinePort][iJ][iK][iL][iM][iPoint], (unsigned char *)&Payload[6]);
						UtilityPutU32(TimeStamp, (unsigned char *)&Payload[10]);

						OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_SDH_LO_READ, Payload, 14);
					}
				}
			}
			else
			{
				for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH; iPoint++)
				{
					if(1==WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[iDevice][iTableIndex].IsMonitored.SONET[iLinePort][iU][iV][iW][iPoint]) /* TODO iterate through iU */
					{
						Payload[0]=iLinePort;
						Payload[1]=iU;
						Payload[2]=iV;
						Payload[3]=iW;
						Payload[4]=iPoint; 
						UtilityPutU32(WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[iDevice][iTableIndex].Count.SONET[iLinePort][iU][iV][iW][iPoint], (unsigned char *)&Payload[5]);
						UtilityPutU32(TimeStamp, (unsigned char *)&Payload[9]);

						OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_PM_SONET_LO_READ, Payload, 13);
					}
				}
			}
		}
	}
}



void WPX_UFE_FRAMER_MIPS_Initialize_BindingPerformanceMonitoringDataTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_RAM)
{
	int i;

	for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;i++)
    {
        pPM_RAM->Device[i].DiscreteClientPerformanceMonitoringTable=WPX_UFE_FRAMER_MIPS_PM_RAM_DiscreteClientTable_Device[i];
        pPM_RAM->Device[i].MAX_DiscreteClientPerformanceMonitoringTableElements=WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH;


        pPM_RAM->Device[i].SONET_SDH_PerformanceMonitoringHighOrderPathTable=WPX_UFE_FRAMER_MIPS_PM_RAM_HiTable_Device[i];
        pPM_RAM->Device[i].MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements=WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH;

        pPM_RAM->Device[i].SONET_SDH_PerformanceMonitoringLowOrderPathTable=WPX_UFE_FRAMER_MIPS_PM_RAM_LoTable_Device[i];
        pPM_RAM->Device[i].MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements=WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH;

		pPM_RAM->Device[i].PDH_PerformanceMonitoringTable=WPX_UFE_FRAMER_MIPS_PM_RAM_PDH_Table_Device[i];
		pPM_RAM->Device[i].MAX_PDH_PerformanceMonitoringTableElements=WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH;

        pPM_RAM->Device[i].SONET_SDH_PerformanceMonitoringPortTable=WPX_UFE_FRAMER_MIPS_PM_RAM_PortTable_Device[i];
        pPM_RAM->Device[i].MAX_SONET_SDH_PerformanceMonitoringPortTableElements=WPX_UFE_FRAMER_MAX_PM_TEST_TABLE_DEPTH;
    }
}
