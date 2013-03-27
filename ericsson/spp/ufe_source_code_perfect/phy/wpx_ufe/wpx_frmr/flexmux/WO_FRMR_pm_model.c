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



void OMIINO_PERFORMANCE_MONITORING_Initialize_DiscreteClient_DataTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE *pPM_Data)
{
	int i;
	int iDiscreteClientPort;
	int iPoint;

	for(i=0;i<pPM_Data->MAX_DiscreteClientPerformanceMonitoringTableElements;i++)
	{
		for(iDiscreteClientPort=0;iDiscreteClientPort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS;iDiscreteClientPort++)
		{
			for(iPoint=0;iPoint<WPX_UFE_FRAMER_CLIENT_SIDE_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT;iPoint++)
			{
				pPM_Data->DiscreteClientPerformanceMonitoringTable[i].Count[iDiscreteClientPort][iPoint]=0;
				pPM_Data->DiscreteClientPerformanceMonitoringTable[i].IsMonitored[iDiscreteClientPort][iPoint]=0;
			}
		}
	}
}





void OMIINO_PERFORMANCE_MONITORING_Initialize_HighOrder_DataTable(U8 iDevice, WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE *pPM_Device_RAM)
{
	int i;
	U8 iLinePort;
	U8 iPoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM=OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	U8 DeviceMode;
	U8 iU;
	U8 iJ;
	U8 iK;

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				for(i=0;i<pPM_Device_RAM->MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements;i++)
				{
					for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
					{
						for(iJ=0;iJ<WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT;iJ++)
						{
							for(iK=0;iK<WPX_UFE_FRAMER_BUILD_OPTION_MAX_K;iK++)
							{
								for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH;iPoint++)
								{
									pPM_Device_RAM->SONET_SDH_PerformanceMonitoringHighOrderPathTable[i].Count.SDH[iLinePort][iJ][iK][iPoint]=0;
									pPM_Device_RAM->SONET_SDH_PerformanceMonitoringHighOrderPathTable[i].IsMonitored.SDH[iLinePort][iJ][iK][iPoint]=0;
								}
							}
						}
					}
				}
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				for(i=0;i<pPM_Device_RAM->MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements;i++)
				{
					for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
					{
						for(iU=0;iU<WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT;iU++)
						{
							for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH;iPoint++)
							{
								pPM_Device_RAM->SONET_SDH_PerformanceMonitoringHighOrderPathTable[i].Count.SONET[iLinePort][iU][iPoint]=0;
								pPM_Device_RAM->SONET_SDH_PerformanceMonitoringHighOrderPathTable[i].IsMonitored.SONET[iLinePort][iU][iPoint]=0;
							}
						}
					}
				}
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceMode);
				break;

	}
}


void OMIINO_PERFORMANCE_MONITORING_Initialize_LowOrder_DataTable(U8 iDevice, WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE *pPM_Device_RAM)
{
	int i;
	U8 iLinePort;
	U8 iPoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM=OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	U8 DeviceMode;
	U8 iU;
	U8 iV;
	U8 iW;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				for(i=0;i<pPM_Device_RAM->MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements;i++)
				{
					for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
					{
						for(iJ=0;iJ<WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT;iJ++)
						{
							for(iK=0;iK<WPX_UFE_FRAMER_BUILD_OPTION_MAX_K;iK++)
							{
								for(iL=0;iL<WPX_UFE_FRAMER_BUILD_OPTION_MAX_L;iL++)
								{
									for(iM=0;iM<WPX_UFE_FRAMER_BUILD_OPTION_MAX_M;iM++)
									{
										for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH;iPoint++)
										{
											pPM_Device_RAM->SONET_SDH_PerformanceMonitoringLowOrderPathTable[i].Count.SDH[iLinePort][iJ][iK][iL][iM][iPoint]=0;
											pPM_Device_RAM->SONET_SDH_PerformanceMonitoringLowOrderPathTable[i].IsMonitored.SDH[iLinePort][iJ][iK][iL][iM][iPoint]=0;
										}
									}
								}
							}
						}
					}
				}
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				for(i=0;i<pPM_Device_RAM->MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements;i++)
				{
					for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
					{
						for(iU=0;iU<WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT;iU++)
						{
							for(iV=0;iV<WPX_UFE_FRAMER_BUILD_OPTION_MAX_V;iV++)
							{
								for(iW=0;iW<WPX_UFE_FRAMER_BUILD_OPTION_MAX_W;iW++)
								{
									for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH;iPoint++)
									{
										pPM_Device_RAM->SONET_SDH_PerformanceMonitoringLowOrderPathTable[i].Count.SONET[iLinePort][iU][iV][iW][iPoint]=0;
										pPM_Device_RAM->SONET_SDH_PerformanceMonitoringLowOrderPathTable[i].IsMonitored.SONET[iLinePort][iU][iV][iW][iPoint]=0;
									}
								}
							}
						}
					}
				}
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceMode);
				break;

	}
}




void OMIINO_PERFORMANCE_MONITORING_Initialize_LinePort_DataTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_TABLE_TYPE *pPM_Data)
{
	int i;
	int iLinePort;
	int iPoint;

	for(i=0;i<pPM_Data->MAX_SONET_SDH_PerformanceMonitoringPortTableElements;i++)
	{
		for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
		{
			for(iPoint=0;iPoint<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_PORT;iPoint++)
			{
				pPM_Data->SONET_SDH_PerformanceMonitoringPortTable[i].Count[iLinePort][iPoint]=0;
				pPM_Data->SONET_SDH_PerformanceMonitoringPortTable[i].IsMonitored[iLinePort][iPoint]=0;
			}
		}
	}
}




void OMIINO_PERFORMANCE_MONITORING_BindDataTable(WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pTo, WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pFrom)
{
    U8 i;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRamForDevice;

    for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;i++)
    {
        pTo->Device[i].DiscreteClientPerformanceMonitoringTable=pFrom->Device[i].DiscreteClientPerformanceMonitoringTable;
        pTo->Device[i].MAX_DiscreteClientPerformanceMonitoringTableElements=pFrom->Device[i].MAX_DiscreteClientPerformanceMonitoringTableElements;


        pTo->Device[i].SONET_SDH_PerformanceMonitoringHighOrderPathTable=pFrom->Device[i].SONET_SDH_PerformanceMonitoringHighOrderPathTable;
        pTo->Device[i].MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements=pFrom->Device[i].MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements;

        pTo->Device[i].SONET_SDH_PerformanceMonitoringLowOrderPathTable=pFrom->Device[i].SONET_SDH_PerformanceMonitoringLowOrderPathTable;
        pTo->Device[i].MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements=pFrom->Device[i].MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements;

		pTo->Device[i].PDH_PerformanceMonitoringTable=pFrom->Device[i].PDH_PerformanceMonitoringTable;
		pTo->Device[i].MAX_PDH_PerformanceMonitoringTableElements=pFrom->Device[i].MAX_PDH_PerformanceMonitoringTableElements;

        pTo->Device[i].SONET_SDH_PerformanceMonitoringPortTable=pFrom->Device[i].SONET_SDH_PerformanceMonitoringPortTable;
        pTo->Device[i].MAX_SONET_SDH_PerformanceMonitoringPortTableElements=pFrom->Device[i].MAX_SONET_SDH_PerformanceMonitoringPortTableElements;
    }

	for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;i++)
    {
		pHierarchyRamForDevice=OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i);
			
		OMIINO_FRAMER_PerformanceMonitoringEngine_SetMaxDepth(&(pHierarchyRamForDevice->PerformanceMonitoring), OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_DISCRETE_CLIENT, pTo->Device[i].MAX_DiscreteClientPerformanceMonitoringTableElements);
			
		OMIINO_FRAMER_PerformanceMonitoringEngine_SetMaxDepth(&(pHierarchyRamForDevice->PerformanceMonitoring), OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_HO,    pTo->Device[i].MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements);
			  
		OMIINO_FRAMER_PerformanceMonitoringEngine_SetMaxDepth(&(pHierarchyRamForDevice->PerformanceMonitoring), OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_LO,    pTo->Device[i].MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements);
			
		OMIINO_FRAMER_PerformanceMonitoringEngine_SetMaxDepth(&(pHierarchyRamForDevice->PerformanceMonitoring), OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH,			  pTo->Device[i].MAX_PDH_PerformanceMonitoringTableElements);
			
		OMIINO_FRAMER_PerformanceMonitoringEngine_SetMaxDepth(&(pHierarchyRamForDevice->PerformanceMonitoring), OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT,  pTo->Device[i].MAX_SONET_SDH_PerformanceMonitoringPortTableElements);
	}

	for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;i++)
    {
		OMIINO_PERFORMANCE_MONITORING_Initialize_DiscreteClient_DataTable(&pTo->Device[i]);
		OMIINO_PERFORMANCE_MONITORING_Initialize_HighOrder_DataTable(i, &pTo->Device[i]);
		OMIINO_PERFORMANCE_MONITORING_Initialize_LowOrder_DataTable(i, &pTo->Device[i]);
		OMIINO_PERFORMANCE_MONITORING_Initialize_LinePort_DataTable(&pTo->Device[i]);
	}

}

