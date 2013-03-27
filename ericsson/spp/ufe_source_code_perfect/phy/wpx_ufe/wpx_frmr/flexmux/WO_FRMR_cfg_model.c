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
#include "WO_FRMR_configuration_defaults_private.h"




void OMIINO_FRAMER_InitializeDiagnosticLoopback(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode, U8 LoopbackIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(1>=LoopbackIsEnabled,LoopbackIsEnabled);

	if(1==LoopbackIsEnabled)
	{
		OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableShallow(pAnyLoopbackConfigurationNode);
		OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableDeep(pAnyLoopbackConfigurationNode);
	}
	else
	{
		OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableShallow(pAnyLoopbackConfigurationNode);
		OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(pAnyLoopbackConfigurationNode);
	}
}



void OMIINO_FRAMER_DISCRETE_CLIENT_Initialize_CAS(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pClient, U8 CAS_IsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(1>=CAS_IsEnabled,CAS_IsEnabled);

	if(1==CAS_IsEnabled)
	{
		OMIINO_FRAMER_DISCRETE_CLIENT_Port_Enable_CAS(pClient);
	}
	else
	{
		OMIINO_FRAMER_DISCRETE_CLIENT_Port_Disable_CAS(pClient);
	}
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Initialize_CAS(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 CAS_IsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(1>=CAS_IsEnabled,CAS_IsEnabled);

	if(1==CAS_IsEnabled)
	{
		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Enable_CAS(pClient);
	}
	else
	{
		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Disable_CAS(pClient);
	}
}



void OMIINO_FRAMER_CLIENT_Initialize_AIS_Control(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode, U8 AIS_ControlIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(1>=AIS_ControlIsEnabled,AIS_ControlIsEnabled);

	if(1==AIS_ControlIsEnabled)
	{
		OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsLine(pAnyAISConfigurationNode);
		OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsSocket(pAnyAISConfigurationNode);
	}
	else
	{
		OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsLine(pAnyAISConfigurationNode);
		OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsSocket(pAnyAISConfigurationNode);
	}
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_InitializeFacilityState(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
}



void OMIINO_FRAMER_DISCRETE_CLIENT_InitializeFacilityState(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pClient)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
}



OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice, U8 iDiscreteClientPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS>iDiscreteClientPort,iDiscreteClientPort);
	return &(pConfigurationRAMForDevice->DiscreteClient[iDiscreteClientPort].Configuration);
}




OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE * OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);
	return &(pConfigurationRAMForDevice->SocketClient.Configuration);
}



OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice, U32 iSocketClientPDH)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS>iSocketClientPDH,iSocketClientPDH);
	return &(pConfigurationRAMForDevice->SocketClient.Element[iSocketClientPDH].Configuration);
}





void OMIINO_FRAMER_InitializeConfigurationForSocketClientPDH(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE * pConfigurationRAMForSocketClientPDH)
{
	U8 OutputSelector;

	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForSocketClientPDH,0);

	for(OutputSelector=0;OutputSelector<MAX_SOCKET_CLIENT_RECOVERED_CLOCKS;OutputSelector++)
	{
		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_SetDriveRecoveredClock(pConfigurationRAMForSocketClientPDH, OutputSelector, OMIINO_DEFAULT_SOCKET_CLIENT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK, OMIINO_DEFAULT_SOCKET_CLIENT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_PARAMETER);
	}
}

void OMIINO_FRAMER_CLIENT_Initialize_Connection(OMIINO_FRAMER_CONFIGURATION_CONNECTION_FOREIGN_KEY_TYPE * pConnection)
{
	pConnection->ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
}



void OMIINO_FRAMER_InitializeConfigurationForSocketClientPDH_Element(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pConfigurationRAMForSocketClientPDH)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForSocketClientPDH,0);

	OMIINO_FRAMER_SOCKET_CLIENT_PDH_InitializeFacilityState(pConfigurationRAMForSocketClientPDH);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetClientType(pConfigurationRAMForSocketClientPDH, OMIINO_DEFAULT_SOCKET_CLIENT_PDH_BANDWIDTH);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetFraming(pConfigurationRAMForSocketClientPDH, OMIINO_DEFAULT_SOCKET_CLIENT_PDH_FRAMING);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetMapping(pConfigurationRAMForSocketClientPDH, OMIINO_DEFAULT_SOCKET_CLIENT_PDH_MAPPING);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Initialize_CAS(pConfigurationRAMForSocketClientPDH, OMIINO_DEFAULT_SOCKET_CLIENT_PDH_CAS);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetClockRecovery(pConfigurationRAMForSocketClientPDH, OMIINO_DEFAULT_SOCKET_CLIENT_PDH_CLOCK_RECOVERY);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Ingress_Term_PRBS(&(pConfigurationRAMForSocketClientPDH->Diagnostic), OMIINO_DEFAULT_PRBS_PATTERN);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Ingress_Gen_PRBS(&(pConfigurationRAMForSocketClientPDH->Diagnostic), OMIINO_DEFAULT_PRBS_PATTERN);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Egress_Term_PRBS(&(pConfigurationRAMForSocketClientPDH->Diagnostic), OMIINO_DEFAULT_PRBS_PATTERN);
	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Egress_Gen_PRBS(&(pConfigurationRAMForSocketClientPDH->Diagnostic), OMIINO_DEFAULT_PRBS_PATTERN);
	OMIINO_FRAMER_InitializeDiagnosticLoopback(&(pConfigurationRAMForSocketClientPDH->Diagnostic.Loopback), OMIINO_DEFAULT_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK);
	OMIINO_FRAMER_CLIENT_Initialize_AIS_Control(&(pConfigurationRAMForSocketClientPDH->AIS_Control), OMIINO_DEFAULT_SOCKET_CLIENT_PDH_AIS_CONTROL);
	OMIINO_FRAMER_CLIENT_Initialize_Connection(&(pConfigurationRAMForSocketClientPDH->Connection));
}






void OMIINO_FRAMER_InitializeConfigurationForDiscreteClientPort(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pConfigurationRAMForDiscreteClientPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDiscreteClientPort,0);

	OMIINO_FRAMER_DISCRETE_CLIENT_InitializeFacilityState(pConfigurationRAMForDiscreteClientPort);
	OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetClientType(pConfigurationRAMForDiscreteClientPort, OMIINO_DEFAULT_DISCRETE_CLIENT_BANDWIDTH);
	OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetFraming(pConfigurationRAMForDiscreteClientPort, OMIINO_DEFAULT_DISCRETE_CLIENT_FRAMING);
	OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetMapping(pConfigurationRAMForDiscreteClientPort, OMIINO_DEFAULT_DISCRETE_CLIENT_MAPPING);
	OMIINO_FRAMER_DISCRETE_CLIENT_Initialize_CAS(pConfigurationRAMForDiscreteClientPort, OMIINO_DEFAULT_DISCRETE_CLIENT_CAS);
	OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetClockRecovery(pConfigurationRAMForDiscreteClientPort, OMIINO_DEFAULT_DISCRETE_CLIENT_CLOCK_RECOVERY);
	OMIINO_FRAMER_InitializeDiagnosticLoopback(&(pConfigurationRAMForDiscreteClientPort->Diagnostic.Loopback), OMIINO_DEFAULT_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK);
	OMIINO_FRAMER_CLIENT_Initialize_AIS_Control(&(pConfigurationRAMForDiscreteClientPort->AIS_Control), OMIINO_DEFAULT_DISCRETE_CLIENT_AIS_CONTROL);
	OMIINO_FRAMER_CLIENT_Initialize_Connection(&(pConfigurationRAMForDiscreteClientPort->Connection));
}


OMIINO_FRAMER_DEVICE_TYPE * OMIINO_FRAMER_GetRAMForDevice(OMIINO_FRAMER_RAM_TYPE *pRAM, U8 iDevice)
{
	OMIINO_FRAMER_ASSERT(NULL!=pRAM,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);
    return &(pRAM->Device[iDevice]);
}



OMIINO_FRAMER_HIERARCHY_TYPE * OMIINO_FRAMER_GetHierarchyRAMForDevice(OMIINO_FRAMER_RAM_TYPE *pRAM, U8 iDevice)
{
	OMIINO_FRAMER_ASSERT(NULL!=pRAM,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);
    return &(pRAM->Device[iDevice].Hierarchy);
}


OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice, U8 iLinePort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>iLinePort,iLinePort);
	return &(pConfigurationRAMForDevice->SONET_SDH_Port[iLinePort]);
}




OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	return &(pConfigurationRAMForPort->Section.J0.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * OMIINO_FRAMER_GetConfigurationRAMForLinePortB1(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	return &(pConfigurationRAMForPort->Section.B1.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * OMIINO_FRAMER_GetConfigurationRAMForLinePortB2(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	return &(pConfigurationRAMForPort->Line.B2.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_S1_TYPE * OMIINO_FRAMER_GetConfigurationRAMForLinePortS1(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	return &(pConfigurationRAMForPort->Line.S1.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_K1K2_TYPE * OMIINO_FRAMER_GetConfigurationRAMForLinePortK1K2(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	return &(pConfigurationRAMForPort->Line.K1K2.Configuration);
}





OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * OMIINO_FRAMER_GetConfigurationRAMForHO_Path(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pEndpoint)
{
    U8 PortMode;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path=NULL;

	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pEndpoint),0);
	OMIINO_FRAMER_ASSERT((1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pEndpoint->Bandwidth)),pEndpoint->Bandwidth);

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pConfigurationRAMForPort->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
            switch(pEndpoint->Bandwidth)
            {
                case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
                case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
                        pHO_Path = &(pConfigurationRAMForPort->u.sdh_HO_Path[pEndpoint->u.sdh.iJ].VC4);
                        break;

                case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI: 
                case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO: /* Bandwidth for VC3-LO Also stored in HO */
                        pHO_Path = &(pConfigurationRAMForPort->u.sdh_HO_Path[pEndpoint->u.sdh.iJ].VC3[pEndpoint->u.sdh.iK]);
                        break;

                default:
                        OMIINO_FRAMER_RSE(pEndpoint->Bandwidth);
                        break;
            }
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
            pHO_Path = &(pConfigurationRAMForPort->u.sonet_HO_Path[pEndpoint->u.sonet.iU]);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	

	return (pHO_Path);
}



OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_B3(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_OVERHEAD_TYPE * pConfigurationRAMForHO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForHO_Path,0);
	return &(pConfigurationRAMForHO_Path->B3.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_C2(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_OVERHEAD_TYPE * pConfigurationRAMForHO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForHO_Path,0);
	return &(pConfigurationRAMForHO_Path->C2.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_OVERHEAD_TYPE * pConfigurationRAMForHO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForHO_Path,0);
	return &(pConfigurationRAMForHO_Path->J1.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_J2(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_OVERHEAD_TYPE * pConfigurationRAMForLO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForLO_Path,0);
	return &(pConfigurationRAMForLO_Path->J2.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_SignalLabel(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_OVERHEAD_TYPE * pConfigurationRAMForLO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForLO_Path,0);
	return &(pConfigurationRAMForLO_Path->V5_SignalLabel.Configuration);
}


OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_BIP2(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_OVERHEAD_TYPE * pConfigurationRAMForLO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForLO_Path,0);
	return &(pConfigurationRAMForLO_Path->V5_BIP2.Configuration);
}





U8 OMIINO_FRAMER_GetConfigurationRAMForLO_Path_Exists(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pEndpoint)
{
	U8 Result=1;
	U8 PortMode;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;

	if(NULL==pConfigurationRAMForDevice)
	{
		Result=0;
	}

	if(Result)
	{
		if(WPX_UFE_FRAMER_OK!=OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pEndpoint))
		{
			Result=0;
		}
	}	

	if(Result)
	{
		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pEndpoint->Bandwidth))
		{
			Result=0;
		}
	}

	if(Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS<=pEndpoint->iLinePort)
		{
			Result=0;
		}
	}

	if(Result)
	{
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pConfigurationRAMForDevice, pEndpoint->iLinePort);
		OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pDevicePortRAM->Configuration), &PortMode);

		switch(PortMode)
		{
			case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				if(!OMIINO_FRAMER_Is_SDH_Endpoint(pEndpoint))
				{
					Result=0;
				}
				break;

			case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				if(OMIINO_FRAMER_Is_SDH_Endpoint(pEndpoint))
				{
					Result=0;
				}
				break;

			default:
				Result=0;
				break;
		}	
	}

	if(Result)
	{
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pConfigurationRAMForDevice, pEndpoint->iLinePort);
		OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pDevicePortRAM->Configuration), &PortMode);

		switch(PortMode)
		{
			case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				if(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration))<=pEndpoint->u.sdh.iJ)
				{
					Result=0;
				}
				break;

			case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				if(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration))<=pEndpoint->u.sonet.iU)
				{
					Result=0;
				}
				break;

			default:
				Result=0;
				break;
		}	
	}

	return Result;
}



OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * OMIINO_FRAMER_GetConfigurationRAMForLO_Path(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pEndpoint)
{
    U8 PortMode;
    U8 iLow;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE	* pLO_Path=NULL;

	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pEndpoint),0);
	OMIINO_FRAMER_ASSERT(0==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pEndpoint->Bandwidth),pEndpoint->Bandwidth);

	pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pConfigurationRAMForDevice, pEndpoint->iLinePort);

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pDevicePortRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>pEndpoint->iLinePort,pEndpoint->iLinePort);
			OMIINO_FRAMER_ASSERT(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration))>pEndpoint->u.sdh.iJ,pEndpoint->u.sdh.iJ);
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_K>pEndpoint->u.sdh.iK,pEndpoint->u.sdh.iK);
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_L>pEndpoint->u.sdh.iL,pEndpoint->u.sdh.iL);
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_M>pEndpoint->u.sdh.iM,pEndpoint->u.sdh.iM);

			iLow=OMIINO_FRAMER_CONFIGURATION_Calculate_SONET_SDH_HO_PATH_OverheadiLow(pEndpoint);
			pLO_Path = &(pConfigurationRAMForDevice->SONET_SDH_LO_Path[iLow].Element[pEndpoint->u.sdh.iL][pEndpoint->u.sdh.iM]);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS>pEndpoint->iLinePort,pEndpoint->iLinePort);
			OMIINO_FRAMER_ASSERT(OMIINO_FRAMER_SONET_SDH_Port_Get_Max_Configured_HO_Paths_ForPort(&(pDevicePortRAM->Configuration))>pEndpoint->u.sonet.iU,pEndpoint->u.sonet.iU);
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_V>pEndpoint->u.sonet.iV,pEndpoint->u.sonet.iV);
			OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_W>pEndpoint->u.sonet.iW,pEndpoint->u.sonet.iW);

			iLow=OMIINO_FRAMER_CONFIGURATION_Calculate_SONET_SDH_HO_PATH_OverheadiLow(pEndpoint);
			pLO_Path = &(pConfigurationRAMForDevice->SONET_SDH_LO_Path[iLow].Element[pEndpoint->u.sonet.iV][pEndpoint->u.sonet.iW]);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	

	return (pLO_Path);


}





void OMIINO_FRAMER_SONET_SDH_InitializePortLine(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);

	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(&(pConfigurationRAMForPort->Line.B2.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_B2_SIGNAL_DEGRADE_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(&(pConfigurationRAMForPort->Line.B2.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_B2_SIGNAL_FAIL_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_Port_Line_S1_SetTX(&(pConfigurationRAMForPort->Line.S1.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_S1_TX);
}



void OMIINO_FRAMER_SONET_SDH_InitializePortSection(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);

	OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetMode(&(pConfigurationRAMForPort->Section.J0.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_MODE);
	OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetTX(&(pConfigurationRAMForPort->Section.J0.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_TX);
	OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetEX(&(pConfigurationRAMForPort->Section.J0.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_EX);

	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(&(pConfigurationRAMForPort->Section.B1.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_B1_SIGNAL_DEGRADE_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(&(pConfigurationRAMForPort->Section.B1.Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_B1_SIGNAL_FAIL_THRESHOLD);
}



void OMIINO_FRAMER_SONET_SDH_InitializePortScrambling(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort, U8 ScramblingIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	OMIINO_FRAMER_ASSERT(1>=ScramblingIsEnabled,ScramblingIsEnabled);

	if(1==ScramblingIsEnabled)
	{
		OMIINO_FRAMER_SONET_SDH_Port_EnableScrambling(pConfigurationRAMForPort);
	}
	else
	{
		OMIINO_FRAMER_SONET_SDH_Port_DisableScrambling(pConfigurationRAMForPort);
	}
}





void OMIINO_FRAMER_SONET_SDH_InitializePort_DCC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort, U8 DCC_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);
	OMIINO_FRAMER_ASSERT(2>=DCC_State,DCC_State);

	OMIINO_FRAMER_SONET_SDH_Port_Set_DCC(pConfigurationRAMForPort, WPX_UFE_FRAMER_DCC_RS_SECTION_D1_D3, DCC_State);
	OMIINO_FRAMER_SONET_SDH_Port_Set_DCC(pConfigurationRAMForPort, WPX_UFE_FRAMER_DCC_MS_LINE_D4_D12, DCC_State);
}








void OMIINO_FRAMER_SONET_SDH_InitializePort_HO_PATH_PM_State(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
	U8 i;
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_Path,0);

	for(i=0;i<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_HO_PATH;i++)
	{
		pHO_Path->PM_State[i]=0;
		pHO_Path->PM_CompressedKey[i]=0xFFFF;
	}
}


void OMIINO_FRAMER_SONET_SDH_InitializeConfiguration_HO_PATH_OverheadBandwidth(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_BANDWIDTH_TYPE * pHO_PathBandwidth)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_PathBandwidth,0);

	pHO_PathBandwidth->Payload=WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL;
}


void OMIINO_FRAMER_SONET_SDH_InitializeConfiguration_HO_PATH_OverheadFacilityState(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_Path,0);
    pHO_Path->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
}

void OMIINO_FRAMER_SONET_SDH_InitializeConfiguration_HO_PATH_OverheadConnection(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_Path,0);
	pHO_Path->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
}

void OMIINO_FRAMER_CONFIGURATION_Initialize_SONET_SDH_HO_PATH_OverheadiLow(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_Path,0);
	pHO_Path->iLow=0;
}

U8 OMIINO_FRAMER_CONFIGURATION_Calculate_SONET_SDH_HO_PATH_OverheadiLow(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	U8 iLow;

 	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pEndpoint),0);

	if(OMIINO_FRAMER_Is_SONET_Endpoint(pEndpoint))
	{
		iLow=((pEndpoint->iLinePort)*(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT))+(pEndpoint->u.sonet.iU);
	}
	else
	{
		iLow =	((pEndpoint->iLinePort)*WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT*WPX_UFE_FRAMER_BUILD_OPTION_MAX_K) +
				((pEndpoint->u.sdh.iJ*WPX_UFE_FRAMER_BUILD_OPTION_MAX_K)) +
				+ pEndpoint->u.sdh.iK;
	}

 	OMIINO_FRAMER_ASSERT(iLow<WPX_UFE_FRAMER_SONET_SDH_LO_PATH_MAX_ILOW_ELEMENTS,iLow);

	return(iLow);
}


void OMIINO_FRAMER_SONET_SDH_InitializeConfigurationFor_HO_PATH_Overhead(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_OVERHEAD_TYPE * pHO_PathOverhead)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_PathOverhead,0);

	OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetMode(&(pHO_PathOverhead->J1.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_MODE);
	OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetTX(&(pHO_PathOverhead->J1.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_TX);
	OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetEX(&(pHO_PathOverhead->J1.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_EX);
	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(&(pHO_PathOverhead->B3.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_B3_SIGNAL_DEGRADE_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(&(pHO_PathOverhead->B3.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_B3_SIGNAL_FAIL_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetTX(&(pHO_PathOverhead->C2.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_TX);
	OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetEX(&(pHO_PathOverhead->C2.Configuration), OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_EX);
}

	


void OMIINO_FRAMER_SONET_SDH_InitializeConfigurationFor_HO_PATH(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pHO_Path,0);

	OMIINO_FRAMER_SONET_SDH_InitializeConfiguration_HO_PATH_OverheadBandwidth(&(pHO_Path->Bandwidth.Configuration));
	OMIINO_FRAMER_SONET_SDH_InitializeConfiguration_HO_PATH_OverheadFacilityState(&(pHO_Path->Configuration));
	OMIINO_FRAMER_SONET_SDH_InitializeConfiguration_HO_PATH_OverheadConnection(&(pHO_Path->Configuration));
	OMIINO_FRAMER_CONFIGURATION_Initialize_SONET_SDH_HO_PATH_OverheadiLow(&(pHO_Path->Configuration));
    OMIINO_FRAMER_SONET_SDH_InitializeConfigurationFor_HO_PATH_Overhead(&(pHO_Path->Overhead));	
	OMIINO_FRAMER_SONET_SDH_InitializePort_HO_PATH_PM_State(&(pHO_Path->Configuration));	

}




void OMIINO_FRAMER_CONFIGURATION_InitializeFor_SONET_SDH_Port(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pConfigurationRAMForPort)
{
    U8 J;
    U8 K;
    U8 U;
    U8 PortMode;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_VC4_VC3_PATH_TYPE *pVC4_VC3;

 	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForPort,0);


	OMIINO_FRAMER_SONET_SDH_Port_SetRate(&(pConfigurationRAMForPort->Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_RATE);
	OMIINO_FRAMER_SONET_SDH_InitializePortScrambling(&(pConfigurationRAMForPort->Configuration), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SCRAMBLING);
	OMIINO_FRAMER_SONET_SDH_InitializePort_DCC(&(pConfigurationRAMForPort->Configuration), 0);
	OMIINO_FRAMER_InitializeDiagnosticLoopback(&(pConfigurationRAMForPort->Configuration.Diagnostic.Loopback), OMIINO_DEFAULT_SONET_SDH_LINE_PORT_DIAGNOSTIC_LOOPBACK);
	OMIINO_FRAMER_SONET_SDH_InitializePortSection(pConfigurationRAMForPort);
	OMIINO_FRAMER_SONET_SDH_InitializePortLine(pConfigurationRAMForPort);
	OMIINO_FRAMER_SONET_SDH_Port_Force_A(&(pConfigurationRAMForPort->Configuration.Protection));

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pConfigurationRAMForPort->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
            for(J=0; J<WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT; J++)
            {
                pVC4_VC3=&(pConfigurationRAMForPort->u.sdh_HO_Path[J]);
                OMIINO_FRAMER_SONET_SDH_InitializeConfigurationFor_HO_PATH(&(pVC4_VC3->VC4));

                for(K=0; K<WPX_UFE_FRAMER_BUILD_OPTION_MAX_K; K++)
                {
                    pVC4_VC3=&(pConfigurationRAMForPort->u.sdh_HO_Path[J]);
                    OMIINO_FRAMER_SONET_SDH_InitializeConfigurationFor_HO_PATH(&(pVC4_VC3->VC3[K]));
                }
            }
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
            for(U=0; U<WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT; U++)
            {
                OMIINO_FRAMER_SONET_SDH_InitializeConfigurationFor_HO_PATH(&(pConfigurationRAMForPort->u.sonet_HO_Path[U]));
            }
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	

}





void OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Bandwidth(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_BANDWIDTH_TYPE * pLO_PathBandwidth)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pLO_PathBandwidth,0);

	pLO_PathBandwidth->Payload=WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL;
}


void OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_FacilityState(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pLO_Path,0);
    pLO_Path->FacilityState=WPX_UFE_FRAMER_FACILITY_STATE_DOES_NOT_EXIST;
}


void OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_PM_State(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path)
{
	U8 i;
 	OMIINO_FRAMER_ASSERT(NULL!=pLO_Path,0);

	for(i=0;i<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH;i++)
	{
		pLO_Path->PM_State[i]=0;
		pLO_Path->PM_CompressedKey[i]=0xFFFF;
	}
}

void OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Connection(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pLO_Path,0);
	pLO_Path->Connection.ConnectionType=WPX_UFE_FRAMER_CONNECTION_TYPE_NONE;
}




void OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Overhead(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_OVERHEAD_TYPE * pLO_PathOverhead)
{
 	OMIINO_FRAMER_ASSERT(NULL!=pLO_PathOverhead,0);

	OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetMode(&(pLO_PathOverhead->J2.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_MODE);
	OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetTX(&(pLO_PathOverhead->J2.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_TX);
	OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetEX(&(pLO_PathOverhead->J2.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_EX);
	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(&(pLO_PathOverhead->V5_BIP2.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_V5_BIP2_SIGNAL_DEGRADE_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(&(pLO_PathOverhead->V5_BIP2.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_V5_BIP2_SIGNAL_FAIL_THRESHOLD);
	OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetTX(&(pLO_PathOverhead->V5_SignalLabel.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_TX);
	OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetEX(&(pLO_PathOverhead->V5_SignalLabel.Configuration), OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_EX);
}



void OMIINO_FRAMER_InitializeConfigurationFor_SONET_SDH_LowOrderElement(OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path)
{
	OMIINO_FRAMER_ASSERT(NULL!=pLO_Path,0);

	OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Bandwidth(&(pLO_Path->Bandwidth.Configuration));
	OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_PM_State(&(pLO_Path->Configuration));
	OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_FacilityState(&(pLO_Path->Configuration));
	OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Connection(&(pLO_Path->Configuration));
    OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Overhead(&(pLO_Path->Overhead));
}



void OMIINO_FRAMER_InitializeConfigurationFor_SONET_SDH_LowOrder(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice)
{
	U8 K=0;
	U8 L=0;
	U8 M=0;
	U32 MaxLowOrderPath_K=WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS*WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT*WPX_UFE_FRAMER_BUILD_OPTION_MAX_K;

	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);

	for(K=0; K<MaxLowOrderPath_K; K++)
	{
		for(L=0; L<WPX_UFE_FRAMER_BUILD_OPTION_MAX_L; L++)
		{
			for(M=0; M<WPX_UFE_FRAMER_BUILD_OPTION_MAX_M; M++)
			{
				OMIINO_FRAMER_InitializeConfigurationFor_SONET_SDH_LowOrderElement(&(pConfigurationRAMForDevice->SONET_SDH_LO_Path[K].Element[L][M]));
			}
		}
	}
}





void OMIINO_FRAMER_InitializeConfigurationForDevice(OMIINO_FRAMER_HIERARCHY_TYPE * pConfigurationRAMForDevice)
{
	U32 i=0;

	OMIINO_FRAMER_ASSERT(NULL!=pConfigurationRAMForDevice,0);

	OMIINO_FRAMER_InitializeDeviceParameters(pConfigurationRAMForDevice, OMIINO_DEFAULT_DEVICE_MODE, OMIINO_DEFAULT_DEVICE_PROTECTION_MODE, OMIINO_DEFAULT_DEVICE_CARD_PROTECTION_OPERATING_MODE, OMIINO_DEFAULT_DEVICE_AIS_INSERTION_STATE);

	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_Initialize(&(pConfigurationRAMForDevice->PerformanceMonitoringKeyManager));

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS; i++)
	{
		OMIINO_FRAMER_CONFIGURATION_InitializeFor_SONET_SDH_Port(OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pConfigurationRAMForDevice, (U8)i));
	}

	OMIINO_FRAMER_InitializeConfigurationFor_SONET_SDH_LowOrder(pConfigurationRAMForDevice);

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS; i++)
	{
		OMIINO_FRAMER_InitializeConfigurationForDiscreteClientPort(OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pConfigurationRAMForDevice, (U8)i));
	}

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS; i++)
	{
		OMIINO_FRAMER_InitializeConfigurationForSocketClientPDH(OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH(pConfigurationRAMForDevice));
		OMIINO_FRAMER_InitializeConfigurationForSocketClientPDH_Element(OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pConfigurationRAMForDevice, i));
	}
}

		


void OMIINO_FRAMER_CONFIGURATION_ModelInitialize(OMIINO_FRAMER_RAM_TYPE *pRAM, U8 TestMode)
{
	U8 i=0;

	OMIINO_FRAMER_ASSERT(NULL!=pRAM,0);

	for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
	{
		OMIINO_FRAMER_Device_SetTestMode(&OMIINO_RAM.Device[i], TestMode);
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_SetLimit(&OMIINO_RAM.Device[i].M_of_N, OMIINO_DEFAULT_M_OF_N_PM_MONITORED_POINTS);
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_SetCurrent(&OMIINO_RAM.Device[i].M_of_N);
		OMIINO_FRAMER_InitializeConfigurationForDevice(OMIINO_FRAMER_GetHierarchyRAMForDevice(pRAM, i));
	}
}

