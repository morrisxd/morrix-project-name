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


#include <string.h>
#include "WO_FRMR_private.h"




void Bulk_TIM(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pBuffer)
{
	memset(pBuffer, '\0', 16);


	switch(pLineEndpoint->Bandwidth)
    {
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4: 
				sprintf((char *)pBuffer, "P%dJ%d", 1+pLineEndpoint->iLinePort, 1+pLineEndpoint->u.sdh.iJ);
				break;
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:  
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
				sprintf((char *)pBuffer, "P%dJ%dK%d", 1+pLineEndpoint->iLinePort, 1+pLineEndpoint->u.sdh.iJ,1+pLineEndpoint->u.sdh.iK);
				break;
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				sprintf((char *)pBuffer, "P%dJ%dK%dL%dM%d", 1+pLineEndpoint->iLinePort, 1+pLineEndpoint->u.sdh.iJ,1+pLineEndpoint->u.sdh.iK,1+pLineEndpoint->u.sdh.iL,1+pLineEndpoint->u.sdh.iM);
                break;

        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				sprintf((char *)pBuffer, "P%dU%d", 1+pLineEndpoint->iLinePort, 1+pLineEndpoint->u.sonet.iU);
				break;

        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2: 
        case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				sprintf((char *)pBuffer, "P%dU%dV%dW%d", 1+pLineEndpoint->iLinePort, 1+pLineEndpoint->u.sonet.iU, 1+pLineEndpoint->u.sonet.iV, 1+pLineEndpoint->u.sonet.iW);
                break;

        default:
                break;
    }

}




void OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iLinePort, U8 AnyBandwidth)
{
	pLineEndpoint->Bandwidth=AnyBandwidth;
	pLineEndpoint->iLinePort=iLinePort;
	pLineEndpoint->u.sonet.iU=0;
	pLineEndpoint->u.sonet.iV=0;
	pLineEndpoint->u.sonet.iW=0;
}

void OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iLinePort, U8 AnyBandwidth)
{
	pLineEndpoint->Bandwidth=AnyBandwidth;
	pLineEndpoint->iLinePort=iLinePort;
	pLineEndpoint->u.sdh.iJ=0;
	pLineEndpoint->u.sdh.iK=0;
	pLineEndpoint->u.sdh.iL=0;
	pLineEndpoint->u.sdh.iM=0;
}

void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_4C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C); 

	OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

	Bulk_TIM(&LineEndpoint, Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;
		OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

		Bulk_TIM(&LineEndpoint, Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_HI_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Enable_Ho_SONET(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 
	for(iU=0;iU<MAX_iU;iU+=3)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
	}

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 
	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
	}
}

void OMIINO_API_INTERNAL_BULK_PM_Disable_Ho_SONET(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 
	for(iU=0;iU<MAX_iU;iU+=3)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
	}

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 
	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
	}
}



void OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SDH_For_Bandwidth(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 AnyLowOrderBandwidth)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, AnyLowOrderBandwidth);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				LineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<3;iM++)
				{
					LineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
					OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
				}
			}
		}
	}
}




void OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SDH_For_Bandwidth(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 AnyLowOrderBandwidth)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, AnyLowOrderBandwidth);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		/* DMCC OMIINO_AUX_ResponseProgressReport(iDevice, AUXILIARY_RESPONSE_WORKING, (U32)iJ, (U32)MAX_iJ, "Disable LO SDH", 14); */

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				LineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<3;iM++)
				{
					LineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
					OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
				}
			}
		}
	}
}



void OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SONET_For_Bandwidth(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 AnyLowOrderBandwidth)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, AnyLowOrderBandwidth); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			LineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<3;iW++)
			{
				LineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
				OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SONET_For_Bandwidth(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 AnyLowOrderBandwidth)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, AnyLowOrderBandwidth); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;

		/* DMCC OMIINO_AUX_ResponseProgressReport(iDevice, AUXILIARY_RESPONSE_WORKING, (U32)iU, (U32)MAX_iU, "Disable LO SONET", 16); */

		for(iV=0;iV<7;iV++)
		{
			LineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<3;iW++)
			{
				LineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_BIP);
				OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_V5_REI);
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SDH(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SDH_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11);
	OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SDH_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12);
}


void OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SDH(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SDH_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11);
	OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SDH_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12);
}


void OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SONET(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SONET_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15);
	OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SONET_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2);
}


void OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SONET(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SONET_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15);
	OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SONET_For_Bandwidth(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2);
}




void OMIINO_API_INTERNAL_BULK_PM_Enable_Ho_SDH(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4);
		LineEndpoint.u.sdh.iJ=iJ;

		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);

		OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI);
		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
		}
		
		OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO);
		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Disable_Ho_SDH(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4);
		LineEndpoint.u.sdh.iJ=iJ;

		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
		OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);

		OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI);
		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
		}
		
		OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO);
		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B3);
			OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_G1);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Enable_LO_For_Port(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SDH(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				OMIINO_API_INTERNAL_BULK_PM_Enable_LO_SONET(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				/* Ignore */
				break;
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Disable_LO_For_Port(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SDH(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				OMIINO_API_INTERNAL_BULK_PM_Disable_LO_SONET(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				/* Ignore */
				break;
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Enable_HO_For_Port(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				OMIINO_API_INTERNAL_BULK_PM_Enable_Ho_SDH(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				OMIINO_API_INTERNAL_BULK_PM_Enable_Ho_SONET(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				/* Ignore */
				break;
	}
}


void OMIINO_API_INTERNAL_BULK_PM_Disable_HO_For_Port(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);

	switch(DeviceMode)
	{
		case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				OMIINO_API_INTERNAL_BULK_PM_Disable_Ho_SDH(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				OMIINO_API_INTERNAL_BULK_PM_Disable_Ho_SONET(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				/* Ignore */
				break;
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_LO_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC12_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				LineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<3;iM++)
				{
					LineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

					Bulk_TIM(&LineEndpoint, Buffer);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC11_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				LineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<4;iM++)
				{
					LineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);
				
					Bulk_TIM(&LineEndpoint, Buffer);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);

				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS48C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C); 

	OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

	Bulk_TIM(&LineEndpoint, Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);

}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS12C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 

	OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

	Bulk_TIM(&LineEndpoint, Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS3C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 

	for(iU=0;iU<MAX_iU;iU+=3)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

		Bulk_TIM(&LineEndpoint, Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS1_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

		Bulk_TIM(&LineEndpoint, Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT2_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			LineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<3;iW++)
			{
				LineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

				Bulk_TIM(&LineEndpoint, Buffer);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT15_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;
	U8 Buffer[16];

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			LineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<4;iW++)
			{
				LineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);

				Bulk_TIM(&LineEndpoint, Buffer);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, (char *)Buffer);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, 0x02);
			}
		}
	}
}


















void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_4C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C); 

	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_HI_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;
			OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_LO_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC12_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				LineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<3;iM++)
				{
					LineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC11_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iJ = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SDH(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11);

	for(iJ=0;iJ<MAX_iJ;iJ++)
	{
		LineEndpoint.u.sdh.iJ=iJ;

		for(iK=0;iK<3;iK++)
		{
			LineEndpoint.u.sdh.iK=iK;

			for(iL=0;iL<7;iL++)
			{
				LineEndpoint.u.sdh.iL=iL;

				for(iM=0;iM<4;iM++)
				{
					LineEndpoint.u.sdh.iM=iM;
					OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
				}
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS48C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C); 

	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);

}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS12C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C); 

	OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS3C_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C); 
	
	for(iU=0;iU<MAX_iU;iU+=3)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS1_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;
		OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT2_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			LineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<3;iW++)
			{
				LineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
			}
		}
	}
}


void OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT15_Facility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	U8 MAX_iU = OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration));
	U8 iU;
	U8 iV;
	U8 iW;

	OMIINO_BULK_Default_WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE_For_SONET(&LineEndpoint, iLinePort, WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15); 

	for(iU=0;iU<MAX_iU;iU++)
	{
		LineEndpoint.u.sonet.iU=iU;

		for(iV=0;iV<7;iV++)
		{
			LineEndpoint.u.sonet.iV=iV;

			for(iW=0;iW<4;iW++)
			{
				LineEndpoint.u.sonet.iW=iW;
				OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
			}
		}
	}
}
