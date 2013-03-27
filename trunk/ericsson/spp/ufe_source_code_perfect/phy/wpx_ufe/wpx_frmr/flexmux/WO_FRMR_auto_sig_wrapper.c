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


U8 OMIINO_PortSignallingMessageShouldBeRaisedForLinePort(U8 iDevice, U8 iLinePort)
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

void OMIINO_SIGNALLING_WRAPPER_BindCallbacks(OMIINO_SIGNALLING_TABLE_PRIVATE_ANNOUNCE_TYPE * pTo, WPX_UFE_FRAMER_SIGNALLING_TABLE_ANNOUNCE_TYPE * pFrom)
{
    pTo->LineSidePrivateSectionK1K2Announce.Callback=pFrom->LineSideSectionK1K2Announce;
    pTo->LineSidePrivateSectionK1K2Announce.EnterCount=0;
    pTo->LineSidePrivateSectionK1K2Announce.ExitCount=0;
    pTo->LineSidePrivateSectionK1K2Announce.UnBoundCalls=0;

    pTo->LineSidePrivateSectionS1Announce.Callback=pFrom->LineSideSectionS1Announce;
    pTo->LineSidePrivateSectionS1Announce.EnterCount=0;
    pTo->LineSidePrivateSectionS1Announce.ExitCount=0;
    pTo->LineSidePrivateSectionS1Announce.UnBoundCalls=0;
}


void OMIINO_SIGNALLING_WRAPPER_SONET_SDH_SECTION_K1K2_ANNOUNCE_TYPE(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	U8 iLogicalLinePort;
	U8 iPresentationLinePort;
	U8 iExternalLinePort;

	iLogicalLinePort=iLinePort;
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionK1K2Announce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionK1K2Announce.EnterCount++;
		if(OMIINO_PortSignallingMessageShouldBeRaisedForLinePort(iDevice, iLogicalLinePort))
		{
			if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLogicalLinePort, &iPresentationLinePort))
			{
				iExternalLinePort=iPresentationLinePort;
				(*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionK1K2Announce.Callback)(iDevice, iExternalLinePort, pRX_K1K2);
			}
		}
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionK1K2Announce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionK1K2Announce.UnBoundCalls++;
    }
}



void OMIINO_SIGNALLING_WRAPPER_SONET_SDH_SECTION_S1_ANNOUNCE_TYPE(U8 iDevice, U8 iLinePort, U8 S1)
{

	U8 iLogicalLinePort;
	U8 iPresentationLinePort;
	U8 iExternalLinePort;

	iLogicalLinePort=iLinePort;
    if(OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionS1Announce.Callback)
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionS1Announce.EnterCount++;
		if(OMIINO_PortSignallingMessageShouldBeRaisedForLinePort(iDevice, iLogicalLinePort))
		{
			if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLogicalLinePort, &iPresentationLinePort))
			{
				iExternalLinePort=iPresentationLinePort;
				(*OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionS1Announce.Callback)(iDevice, iExternalLinePort, S1);
			}
		}
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionS1Announce.ExitCount++;
    }
    else
    {
        OMIINO_RAM.AutonomousOutputTable.AutonomousPrivateSignallingTable.LineSidePrivateSectionS1Announce.UnBoundCalls++;
    }
}
