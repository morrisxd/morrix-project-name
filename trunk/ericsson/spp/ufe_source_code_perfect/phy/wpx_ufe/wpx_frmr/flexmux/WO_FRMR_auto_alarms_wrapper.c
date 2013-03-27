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


void OMIINO_ALARM_WRAPPER_BindCallbacks(OMIINO_ALARM_OUTPUT_TABLE_PRIVATE_ANNOUNCE_TYPE *pTo, WPX_UFE_FRAMER_ALARM_OUTPUT_TABLE_ANNOUNCE_TYPE *pFrom)
{
    pTo->DiscreteClientPrivatePortAlarmAnnounce.Callback=pFrom->DiscreteClientPortAlarmAnnounce;
    pTo->DiscreteClientPrivatePortAlarmAnnounce.EnterCount=0;
    pTo->DiscreteClientPrivatePortAlarmAnnounce.ExitCount=0;
    pTo->DiscreteClientPrivatePortAlarmAnnounce.UnBoundCalls=0;

    
    pTo->SocketClientSidePrivatePortAlarmAnnounce.Callback=pFrom->SocketClientSidePortAlarmAnnounce;
    pTo->SocketClientSidePrivatePortAlarmAnnounce.EnterCount=0;
    pTo->SocketClientSidePrivatePortAlarmAnnounce.ExitCount=0;
    pTo->SocketClientSidePrivatePortAlarmAnnounce.UnBoundCalls=0;


    
    pTo->SONET_SDH_PrivatePathAlarmAnnounce.Callback=pFrom->SONET_SDH_PathAlarmAnnounce;
    pTo->SONET_SDH_PrivatePathAlarmAnnounce.EnterCount=0;
    pTo->SONET_SDH_PrivatePathAlarmAnnounce.ExitCount=0;
    pTo->SONET_SDH_PrivatePathAlarmAnnounce.UnBoundCalls=0;
    
    pTo->SONET_SDH_PrivatePortAlarmAnnounce.Callback=pFrom->SONET_SDH_PortAlarmAnnounce;
    pTo->SONET_SDH_PrivatePortAlarmAnnounce.EnterCount=0;
    pTo->SONET_SDH_PrivatePortAlarmAnnounce.ExitCount=0;
    pTo->SONET_SDH_PrivatePortAlarmAnnounce.UnBoundCalls=0;

}


U8 OMIINO_PortAlarmMessageShouldBeRaisedForLinePort(U8 iDevice, U8 iLinePort) 
{
	U8 Result=0;
	U8 CurrentPortRate;
	U8 iWorkingPort=iLinePort;
	U8 Logical_Side_A_iLinePort;

	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	if(OMIINO_DEVICE_PERSONALITY_SupportsLineProtection(iDevice))
	{
		if(OMIINO_FRAMER_PortMap_Logical_iLinePort_Logical_Side_A_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLinePort, &Logical_Side_A_iLinePort))
		{
			iWorkingPort=Logical_Side_A_iLinePort;
		}
	}

	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iWorkingPort);
	OMIINO_FRAMER_SONET_SDH_Port_GetRate(&(pDevicePortRAM->Configuration), &CurrentPortRate);

	if(WPX_UFE_FRAMER_LINE_PORT_RATE_OFF!=CurrentPortRate)
	{
		Result=1;
	}

	return Result;
}


void OMIINO_ALARM_WRAPPER_SONET_SDH_PORT_ALARM_ANNOUNCE_TYPE(U8 iDevice, U8 iLinePort, U8 AlarmCategory, U8 IsAsserted, U8 IsForced)
{
	U8 iLogicalLinePort;
	U8 iPresentationLinePort;
	U8 iExternalLinePort;

	iLogicalLinePort=iLinePort;
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePortAlarmAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePortAlarmAnnounce.EnterCount++;

		if(OMIINO_PortAlarmMessageShouldBeRaisedForLinePort(iDevice, iLogicalLinePort)||IsForced)
		{
			if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLogicalLinePort, &iPresentationLinePort))
			{
				iExternalLinePort=iPresentationLinePort;
				(*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePortAlarmAnnounce.Callback)(iDevice, iExternalLinePort, AlarmCategory, IsAsserted);
			}
		}
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePortAlarmAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePortAlarmAnnounce.UnBoundCalls++;
    }
}



void OMIINO_ALARM_WRAPPER_SONET_SDH_PATH_ALARM_ANNOUNCE_TYPE(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 AlarmCategory, U8 IsAsserted)
{
	U8 iLogicalLinePort;
	U8 iPresentationLinePort;
    U8 LocalResult;
    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE Common_SDH_SONET_LineEndpointType;

	iLogicalLinePort=pLineEndpoint->iLinePort;
	if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLogicalLinePort, &iPresentationLinePort))
	{
		pLineEndpoint->iLinePort=iPresentationLinePort;

		LocalResult=WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_PresentationEndpointToExternal(iDevice, &Common_SDH_SONET_LineEndpointType, pLineEndpoint);

		if(WPX_UFE_FRAMER_OK==LocalResult)
		{
			if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePathAlarmAnnounce.Callback)
			{
				OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePathAlarmAnnounce.EnterCount++;
				(*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePathAlarmAnnounce.Callback)(iDevice, &Common_SDH_SONET_LineEndpointType, AlarmCategory, IsAsserted);
				OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePathAlarmAnnounce.ExitCount++;
			}
			else
			{
				OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SONET_SDH_PrivatePathAlarmAnnounce.UnBoundCalls++;
			}
		}

                /* Reinstate logical iLinePort for subsequent reference */
                pLineEndpoint->iLinePort=iLogicalLinePort;
	}
}



void OMIINO_ALARM_WRAPPER_DISCRETE_CLIENT_PORT_ALARM_ANNOUNCE_TYPE(U8 iDevice, U8 iDiscreteClientPort, U8 AlarmCategory, U8 IsAsserted)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.DiscreteClientPrivatePortAlarmAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.DiscreteClientPrivatePortAlarmAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.DiscreteClientPrivatePortAlarmAnnounce.Callback)(iDevice, iDiscreteClientPort, AlarmCategory, IsAsserted);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.DiscreteClientPrivatePortAlarmAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.DiscreteClientPrivatePortAlarmAnnounce.UnBoundCalls++;
    }
}



void OMIINO_ALARM_WRAPPER_SOCKET_CLIENT_PDH_ALARM_ANNOUNCE_TYPE(U8 iDevice, U32 iSocketClientPDH, U8 AlarmCategory, U8 IsAsserted)
{
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SocketClientSidePrivatePortAlarmAnnounce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SocketClientSidePrivatePortAlarmAnnounce.EnterCount++;
        (*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SocketClientSidePrivatePortAlarmAnnounce.Callback)(iDevice, iSocketClientPDH, AlarmCategory, IsAsserted);
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SocketClientSidePrivatePortAlarmAnnounce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateAlarmTable.SocketClientSidePrivatePortAlarmAnnounce.UnBoundCalls++;
    }
}



