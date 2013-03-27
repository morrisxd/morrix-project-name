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




void OMIINO_FRAMER_PortMap_Initialize(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	U8 iLinePort;

	for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS; iLinePort++)
	{
		pPortMap->LogicalToPresentation[iLinePort]=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;
		pPortMap->PresentationToLogical[iLinePort]=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;
		pPortMap->LogicalProtectionPartner[iLinePort]=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;
	}
}



void OMIINO_FRAMER_DEVICE_PortMap_Initialize(OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM)
{
    U32 i;

    for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;i++)
    {
		OMIINO_FRAMER_PortMap_Initialize(&pDeviceRAM[i].PortMap);
    }
}



U8 OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iLogicalLinePort, U8 *pPresentationLinePort)
{
	U8 Result=0;
	*pPresentationLinePort=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLogicalLinePort)
	{
		*pPresentationLinePort=pPortMap->LogicalToPresentation[iLogicalLinePort];
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS != *pPresentationLinePort)
		{
			Result=1;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iPresentationLinePort, U8 *piLogicalLinePort)
{
	U8 Result=0;
	*piLogicalLinePort=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iPresentationLinePort)
	{
		*piLogicalLinePort=pPortMap->PresentationToLogical[iPresentationLinePort];
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS != *piLogicalLinePort)
		{
			Result=1;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iLogicalLinePort, U8 iPresentationLinePort)
{
	U8 Result=0;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iPresentationLinePort)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iPresentationLinePort)
		{
			Result=1;
			pPortMap->PresentationToLogical[iPresentationLinePort]=iLogicalLinePort;
			pPortMap->LogicalToPresentation[iLogicalLinePort ]=iPresentationLinePort;
		}
	}

	return Result;
}








U8 OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iLogicalLinePort, U8 *pLogical_ProtectionPartner_iLinePort)
{
	U8 Result=0;
	*pLogical_ProtectionPartner_iLinePort=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLogicalLinePort)
	{
		*pLogical_ProtectionPartner_iLinePort=pPortMap->LogicalProtectionPartner[iLogicalLinePort];
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS != *pLogical_ProtectionPartner_iLinePort)
		{
			Result=1;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_PortMap_Logical_iLinePort_Logical_Side_A_iLinePort(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iLogicalLinePort, U8 *pLogical_Side_A_iLinePort)
{
	U8 iCandidate_SideA;
	*pLogical_Side_A_iLinePort=iLogicalLinePort;

	if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort(pPortMap, iLogicalLinePort, &iCandidate_SideA))
	{
		if(iCandidate_SideA<iLogicalLinePort)
		{
			*pLogical_Side_A_iLinePort=iCandidate_SideA;
		}
	}

	return 1;
}




U8 OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Presentation_ProtectionPartner_iLinePort(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iPresentationLinePort, U8 * pPresentation_ProtectionPartner_iLinePort)
{
	U8 Result=0;
	U8 iLogicalLinePort;
	U8 LogicalProtectionPartner_iLinePort;

	*pPresentation_ProtectionPartner_iLinePort=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iPresentationLinePort)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(pPortMap, iPresentationLinePort, &iLogicalLinePort))
		{
			LogicalProtectionPartner_iLinePort=pPortMap->LogicalProtectionPartner[iLogicalLinePort];
			if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS != LogicalProtectionPartner_iLinePort)
			{
				if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(pPortMap, LogicalProtectionPartner_iLinePort, pPresentation_ProtectionPartner_iLinePort))
				{
					Result=1;
				}
			}
		}
	}

	return Result;
}




U8 OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iLogicalLinePort, U8 Logical_ProtectionPartner_iLinePort)
{
	U8 Result=0;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLogicalLinePort)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>Logical_ProtectionPartner_iLinePort)
		{
			if(iLogicalLinePort!=Logical_ProtectionPartner_iLinePort)
			{
				Result=1;
				pPortMap->LogicalProtectionPartner[iLogicalLinePort]=Logical_ProtectionPartner_iLinePort;
				pPortMap->LogicalProtectionPartner[Logical_ProtectionPartner_iLinePort]=iLogicalLinePort;
			}
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_PortMap_Break_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(OMIINO_PORT_MAP_TYPE *pPortMap, U8 iLogicalLinePort)
{
	U8 Result=0;
	U8 Logical_ProtectionPartner_iLinePort=pPortMap->LogicalProtectionPartner[iLogicalLinePort];

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLogicalLinePort)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>Logical_ProtectionPartner_iLinePort)
		{
			if(iLogicalLinePort!=Logical_ProtectionPartner_iLinePort)
			{
				Result=1;
				pPortMap->LogicalProtectionPartner[iLogicalLinePort]=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;
				pPortMap->LogicalProtectionPartner[Logical_ProtectionPartner_iLinePort]=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;
			}
		}
	}

	return Result;
}




void OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_OC12_Line(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 4, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 6, 3);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 4);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 5);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 5, 6);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 7, 7);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 1);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 3);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 4, 5);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 6, 7);
}




void OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_OC12_Card(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 3);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 4, 4);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 5, 5);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 6, 6);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 7, 7);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 4);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 1, 5);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 6);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 3, 7);
}





U8 OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_OC12(OMIINO_PORT_MAP_TYPE *pPortMap, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceProtectionMode)
	{
		case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
				Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_OC12_Line(pPortMap);
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_OC12_Card(pPortMap);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceProtectionMode);
				break;
	}

	return(Result);
}








void OMIINO_FRAMER_PortMap_Define_PERSONALITY_1_PLUS_1_OC3_OC12_Line(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 1);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 1);
}




void OMIINO_FRAMER_PortMap_Define_PERSONALITY_1_PLUS_1_OC3_OC12_Card(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 1);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 1);
}





U8 OMIINO_FRAMER_PortMap_Define_PERSONALITY_1_PLUS_1_OC3_OC12(OMIINO_PORT_MAP_TYPE *pPortMap, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceProtectionMode)
	{
		case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
				Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_1_PLUS_1_OC3_OC12_Line(pPortMap);
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_1_PLUS_1_OC3_OC12_Card(pPortMap);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceProtectionMode);
				break;
	}

	return(Result);
}










void OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_Line(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 4, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 6, 3);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 4);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 5);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 5, 6);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 7, 7);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 1);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 3);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 4, 5);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 6, 7);
}




void OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_Card(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 3);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 4, 4);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 5, 5);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 6, 6);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 7, 7);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 4);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 1, 5);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 6);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 3, 7);
}





U8 OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3(OMIINO_PORT_MAP_TYPE *pPortMap, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceProtectionMode)
	{
		case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
				Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_Line(pPortMap);
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_Card(pPortMap);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceProtectionMode);
				break;
	}

	return(Result);
}





void OMIINO_FRAMER_PortMap_Define_PERSONALITY_8_PLUS_8_OC3_Line(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 4, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 6, 3);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 8, 4);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 10, 5);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 12, 6);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 14, 7);

	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 8);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 9);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 5, 10);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 7, 11);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 9, 12);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 11, 13);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 13, 14);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 15, 15);


	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 1);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 3);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 4, 5);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 6, 7);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 8, 9);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 10, 11);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 12, 13);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 14, 15);
}




void OMIINO_FRAMER_PortMap_Define_PERSONALITY_8_PLUS_8_OC3_Card(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 3);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 4, 4);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 5, 5);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 6, 6);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 7, 7);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 8, 8);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 9, 9);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 10, 10);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 11, 11);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 12, 12);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 13, 13);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 14, 14);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 15, 15);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 8);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 1, 9);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 10);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 3, 11);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 4, 12);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 5, 13);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 6, 14);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 7, 15);
}






U8 OMIINO_FRAMER_PortMap_Define_PERSONALITY_8_PLUS_8_OC3(OMIINO_PORT_MAP_TYPE *pPortMap, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceProtectionMode)
	{
		case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
				Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_8_PLUS_8_OC3_Line(pPortMap);
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_8_PLUS_8_OC3_Card(pPortMap);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceProtectionMode);
				break;
	}

	return(Result);
}









void OMIINO_FRAMER_PortMap_Define_PERSONALITY_2_PLUS_2_OC3_Line(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 3);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 1);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 2, 3);
}




void OMIINO_FRAMER_PortMap_Define_PERSONALITY_2_PLUS_2_OC3_Card(OMIINO_PORT_MAP_TYPE *pPortMap)
{
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 0, 0);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 1, 1);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 2, 2);
	OMIINO_FRAMER_PortMap_Make_Presentation_iLogicalLinePort_Mapping(pPortMap, 3, 3);

	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 0, 2);
	OMIINO_FRAMER_PortMap_Make_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort_Mapping(pPortMap, 1, 3); 
}





U8 OMIINO_FRAMER_PortMap_Define_PERSONALITY_2_PLUS_2_OC3(OMIINO_PORT_MAP_TYPE *pPortMap, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceProtectionMode)
	{
		case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
				Result=WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_NOT_COMPATIBLE_WITH_PERSONALITY;
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_2_PLUS_2_OC3_Line(pPortMap);
				break;

		case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
				OMIINO_FRAMER_PortMap_Define_PERSONALITY_2_PLUS_2_OC3_Card(pPortMap);
				break;

		default:
				OMIINO_FRAMER_RSE(DeviceProtectionMode);
				break;
	}

	return(Result);
}










U8 OMIINO_FRAMER_PortMap_Define(OMIINO_FRAMER_DEVICE_TYPE *pDeviceRAM, U8 AnyPersonality, U8 DeviceProtectionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
				Result=OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3_OC12(&pDeviceRAM->PortMap, DeviceProtectionMode);
				break;
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:	
				Result=OMIINO_FRAMER_PortMap_Define_PERSONALITY_1_PLUS_1_OC3_OC12(&pDeviceRAM->PortMap, DeviceProtectionMode);
				break;
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:	
				Result=OMIINO_FRAMER_PortMap_Define_PERSONALITY_4_PLUS_4_OC3(&pDeviceRAM->PortMap, DeviceProtectionMode);
				break;
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:	
				Result=OMIINO_FRAMER_PortMap_Define_PERSONALITY_2_PLUS_2_OC3(&pDeviceRAM->PortMap, DeviceProtectionMode);
				break;

		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:					
				Result=OMIINO_FRAMER_PortMap_Define_PERSONALITY_8_PLUS_8_OC3(&pDeviceRAM->PortMap, DeviceProtectionMode);
				break;

		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12_4_PLUS_4_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_16_OC3:		
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_4_PLUS_4_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_64_DISCRETE_ONLY:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_32_DISCRETE_ONLY:				
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_16_DISCRETE_ONLY:				
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_OC48:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_32_DISCRETE:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_32_DISCRETE:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_UNDEFINED:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_UNDEFINED;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
				break;

	}

	return Result;
}





