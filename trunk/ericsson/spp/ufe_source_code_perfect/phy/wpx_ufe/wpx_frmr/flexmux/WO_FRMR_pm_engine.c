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





U8 OMIINO_FRAMER_PerformanceMonitoringEngine_GetMonitor_LinePortPoint(	U8 iDevice,
																		WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE *   pPM_Device_RAM,
																		OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey)
{
	return (pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[0].IsMonitored[pKey->u.LinePort.iPort][pKey->u.LinePort.iDataPointIdentifier]);
}



void OMIINO_FRAMER_PerformanceMonitoringEngine_Monitor_LinePortPoint(	U8 iDevice,
																		WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE *   pPM_Device_RAM,
																		OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey)
{
	int i;

	for(i=0;i<pPM_Device_RAM->Device[iDevice].MAX_SONET_SDH_PerformanceMonitoringPortTableElements;i++)
	{
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[i].IsMonitored[pKey->u.LinePort.iPort][pKey->u.LinePort.iDataPointIdentifier]=1;
	}
}




void OMIINO_FRAMER_PerformanceMonitoringEngine_UnMonitor_LinePoint(	U8 iDevice,
																	WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE *   pPM_Device_RAM,
																	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey)
{
	int i;

	for(i=0;i<pPM_Device_RAM->Device[iDevice].MAX_SONET_SDH_PerformanceMonitoringPortTableElements;i++)
	{
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[i].IsMonitored[pKey->u.LinePort.iPort][pKey->u.LinePort.iDataPointIdentifier]=0;
	}
}





U8  OMIINO_FRAMER_PerformanceMonitoringEngine_NewReport(U8                                                 iDevice,
                                                        WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE *   pPM_Device_RAM,
                                                        OMIINO_FRAMER_PM_ENGINE_TYPE *                      pPM_Engine_RAM,
                                                        U8                                                  iElement)
{
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM=OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	U8 DeviceMode;


    pPM_Engine_RAM->Element[iElement].DataItemsFiled=0;
    
    pPM_Engine_RAM->Element[iElement].iCurrent++;
    if(pPM_Engine_RAM->Element[iElement].MaxDepth<=pPM_Engine_RAM->Element[iElement].iCurrent)
    {
        pPM_Engine_RAM->Element[iElement].iCurrent=0;
    }
  


    switch(iElement)
    {
        case    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT:
				memset(pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[pPM_Engine_RAM->Element[iElement].iCurrent].Count,0,PM_PORT_ARRAY_SIZE);
                break;
               
        case    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_HO:
				OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
				switch(DeviceMode)
				{
					case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
							memset(pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[pPM_Engine_RAM->Element[iElement].iCurrent].Count.SDH,0,PM_HO_SDH_ARRAY_SIZE);
							break;

					case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
							memset(pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[pPM_Engine_RAM->Element[iElement].iCurrent].Count.SONET,0,PM_HO_SONET_ARRAY_SIZE);
							break;

					default:
							OMIINO_FRAMER_RSE(DeviceMode);
							break;

				}
                break;

	


        case    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_LO:
				OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
				switch(DeviceMode)
				{
					case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
							memset(pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[pPM_Engine_RAM->Element[iElement].iCurrent].Count.SDH,0,PM_LO_SDH_ARRAY_SIZE);
							break;

					case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
							memset(pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[pPM_Engine_RAM->Element[iElement].iCurrent].Count.SONET,0,PM_LO_SONET_ARRAY_SIZE);
							break;

					default:
							OMIINO_FRAMER_RSE(DeviceMode);
							break;

				}
                break;

        case    OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_DISCRETE_CLIENT:
				/* TODO */
                break;

		case	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH:
				memset(pPM_Device_RAM->Device[iDevice].PDH_PerformanceMonitoringTable[pPM_Engine_RAM->Element[iElement].iCurrent].E1T1,0,PM_PDH_ARRAY_SIZE);
				break;
				break;


        default:
                OMIINO_FRAMER_RSE(iElement);
                break;

    }

    return pPM_Engine_RAM->Element[iElement].iCurrent;
}

void OMIINO_FRAMER_PerformanceMonitoringEngine_DiscreteClient_DataPointAnnounce(U8 iDevice, 
                                                                                WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_Device_RAM,
                                                                                OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM,
                                                                                U32 TimeStamp,
                                                                                OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey,
                                                                                U16 Count)
{
    U8  iElement=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_DISCRETE_CLIENT;
    U8  iCurrent=pPM_Engine_RAM->Element[iElement].iCurrent;
    U32 CurrentTimeStamp=pPM_Device_RAM->Device[iDevice].DiscreteClientPerformanceMonitoringTable[iCurrent].TimeStamp;
    
    if(TimeStamp!=CurrentTimeStamp)
    {
        if(0!=pPM_Engine_RAM->Element[iElement].DataItemsFiled)
        {
            OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_DISCRETE_CLIENT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(iDevice, iCurrent, TimeStamp);
            iCurrent=OMIINO_FRAMER_PerformanceMonitoringEngine_NewReport(iDevice, pPM_Device_RAM, pPM_Engine_RAM, iElement);
        }
    }
    pPM_Device_RAM->Device[iDevice].DiscreteClientPerformanceMonitoringTable[iCurrent].Count[pKey->u.DiscreteClient.iPort][pKey->u.DiscreteClient.iDataPointIdentifier]=Count;
    pPM_Engine_RAM->Element[iElement].DataItemsFiled++;   
   
}


void OMIINO_FRAMER_PerformanceMonitoringEngine_PDH_DataPointAnnounce(U8 iDevice, 
                                                                                WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_Device_RAM,
                                                                                OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM,
                                                                                U32 TimeStamp,
                                                                                OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey,
                                                                                U16 Count)
{
    U8  iElement=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH;
    U8  iCurrent=pPM_Engine_RAM->Element[iElement].iCurrent;
    U32 CurrentTimeStamp=pPM_Device_RAM->Device[iDevice].PDH_PerformanceMonitoringTable[iCurrent].TimeStamp;
    
    
    if(0==pPM_Engine_RAM->Element[iElement].DataItemsFiled)
    {
        pPM_Device_RAM->Device[iDevice].PDH_PerformanceMonitoringTable[iCurrent].TimeStamp=TimeStamp;
    }
    else
    {
       if(TimeStamp > 1000 + CurrentTimeStamp)
    {
        if(0!=pPM_Engine_RAM->Element[iElement].DataItemsFiled)
        {
            OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_PDH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(iDevice, iCurrent, TimeStamp);
            iCurrent=OMIINO_FRAMER_PerformanceMonitoringEngine_NewReport(iDevice, pPM_Device_RAM, pPM_Engine_RAM, iElement);
              pPM_Device_RAM->Device[iDevice].PDH_PerformanceMonitoringTable[iCurrent].TimeStamp=TimeStamp;
        }
    }
	pPM_Device_RAM->Device[iDevice].PDH_PerformanceMonitoringTable[iCurrent].E1T1[pKey->u.PDH.iPDHPort][pKey->u.PDH.iDataPointIdentifier]=Count;
    }
   
    pPM_Engine_RAM->Element[iElement].DataItemsFiled++;   
}

void OMIINO_FRAMER_PerformanceMonitoringEngine_HO_DataPointAnnounce(            U8 iDevice, 
                                                                                WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_Device_RAM,
                                                                                OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM,
                                                                                U32 TimeStamp,
                                                                                OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey,
                                                                                U16 Count)
{
    U8  iElement=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_HO;
    U8  iCurrent=pPM_Engine_RAM->Element[iElement].iCurrent;
    U32 CurrentTimeStamp=pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iCurrent].TimeStamp;
    
	if(0==pPM_Engine_RAM->Element[iElement].DataItemsFiled)
	{
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iCurrent].TimeStamp=TimeStamp;
	}
	else
	{
		if(TimeStamp>1000+CurrentTimeStamp)
		{
			if(0!=pPM_Engine_RAM->Element[iElement].DataItemsFiled)
			{
				OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_SONET_SDH_HO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(iDevice, iCurrent, TimeStamp);
				iCurrent=OMIINO_FRAMER_PerformanceMonitoringEngine_NewReport(iDevice, pPM_Device_RAM, pPM_Engine_RAM, iElement);
				pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iCurrent].TimeStamp=TimeStamp;
			}
		}
	}

    if(OMIINO_FRAMER_Is_SONET_Endpoint(&pKey->u.HO.LineEndpoint))
    {
        pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iCurrent].Count.SONET			[pKey->u.HO.LineEndpoint.iLinePort]
                                                                                                                        [pKey->u.HO.LineEndpoint.u.sonet.iU]
                                                                                                                        [pKey->u.HO.iDataPointIdentifier]=Count;
    }
    else
    {
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iCurrent].Count.SDH			[pKey->u.HO.LineEndpoint.iLinePort]
                                                                                                                        [pKey->u.HO.LineEndpoint.u.sdh.iJ]
                                                                                                                        [pKey->u.HO.LineEndpoint.u.sdh.iK]
                                                                                                                        [pKey->u.HO.iDataPointIdentifier]=Count;
                                                                                                     
    }
    
    pPM_Engine_RAM->Element[iElement].DataItemsFiled++;
}


void OMIINO_FRAMER_PerformanceMonitoringEngine_LO_DataPointAnnounce(            U8 iDevice, 
                                                                                WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_Device_RAM,
                                                                                OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM,
                                                                                U32 TimeStamp,
                                                                                OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey,
                                                                                U16 Count)
{
    U8  iElement=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_LO;
    U8  iCurrent=pPM_Engine_RAM->Element[iElement].iCurrent;
    U32 CurrentTimeStamp=pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iCurrent].TimeStamp;
    
	if(0==pPM_Engine_RAM->Element[iElement].DataItemsFiled)
	{
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iCurrent].TimeStamp=TimeStamp;
	}
	else
	{
		if(TimeStamp>1000+CurrentTimeStamp)
		{
			if(0!=pPM_Engine_RAM->Element[iElement].DataItemsFiled)
			{
				OMIINO_PERFORMANCE_MONITORING_WRAPPER_API_SONET_SDH_LO_PATH_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(iDevice, iCurrent, TimeStamp);            
				iCurrent=OMIINO_FRAMER_PerformanceMonitoringEngine_NewReport(iDevice, pPM_Device_RAM, pPM_Engine_RAM, iElement);
				pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iCurrent].TimeStamp=TimeStamp;
			}
		}
	}


    if(OMIINO_FRAMER_Is_SONET_Endpoint(&pKey->u.LO.LineEndpoint))
    {
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iCurrent].Count.SONET[pKey->u.LO.LineEndpoint.iLinePort]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sonet.iU]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sonet.iV]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sonet.iW]
                                                                                                                        [pKey->u.LO.iDataPointIdentifier]=Count;
    }
    else
    {
        pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iCurrent].Count.SDH			[pKey->u.LO.LineEndpoint.iLinePort]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sdh.iJ]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sdh.iK]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sdh.iL]
                                                                                                                        [pKey->u.LO.LineEndpoint.u.sdh.iM]
                                                                                                                        [pKey->u.LO.iDataPointIdentifier]=Count;                                                                                                           
    }
    
    pPM_Engine_RAM->Element[iElement].DataItemsFiled++;
}


void OMIINO_FRAMER_PerformanceMonitoringEngine_LinePort_DataPointAnnounce(      U8 iDevice, 
                                                                                WPX_UFE_FRAMER_PERFORMANCE_MONITORING_DEVICE_TABLE_TYPE * pPM_Device_RAM,
                                                                                OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM,
                                                                                U32 TimeStamp,
                                                                                OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE * pKey,
                                                                                U32 Count)
{
    U8  iElement=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
    U8  iCurrent=pPM_Engine_RAM->Element[iElement].iCurrent;
    U32 CurrentTimeStamp=pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[iCurrent].TimeStamp;
    
	if(0==pPM_Engine_RAM->Element[iElement].DataItemsFiled)
	{
		pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[iCurrent].TimeStamp=TimeStamp;
	}
	else
	{
		if(TimeStamp>1000+CurrentTimeStamp)
		{
			if(0!=pPM_Engine_RAM->Element[iElement].DataItemsFiled)
			{
				OMIINO_PERFORMANCE_MONITORING_WRAPPER_SONET_SDH_PORT_PERFORMANCE_MONITORING_ANNOUNCE_TYPE(iDevice, iCurrent, TimeStamp);
				iCurrent=OMIINO_FRAMER_PerformanceMonitoringEngine_NewReport(iDevice, pPM_Device_RAM, pPM_Engine_RAM, iElement);
				pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[iCurrent].TimeStamp=TimeStamp;
			}
		}
	}

	pPM_Device_RAM->Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[iCurrent].Count[pKey->u.LinePort.iPort][pKey->u.LinePort.iDataPointIdentifier]=Count;
    pPM_Engine_RAM->Element[iElement].DataItemsFiled++;
}


void OMIINO_FRAMER_PerformanceMonitoringEngine_SetMaxDepth(OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM, int iElement, U8 AnyMaxDepth)
{
	pPM_Engine_RAM->Element[iElement].MaxDepth=AnyMaxDepth;
}

void OMIINO_FRAMER_PerformanceMonitoringEngine_Initialize(OMIINO_FRAMER_PM_ENGINE_TYPE * pPM_Engine_RAM)
{
    int i;

    for(i=0;i<OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MAX_KEYS;i++)
    {
        pPM_Engine_RAM->Element[i].iCurrent=0;
        pPM_Engine_RAM->Element[i].MaxDepth=0;
        pPM_Engine_RAM->Element[i].DataItemsFiled=0;
    }
}

