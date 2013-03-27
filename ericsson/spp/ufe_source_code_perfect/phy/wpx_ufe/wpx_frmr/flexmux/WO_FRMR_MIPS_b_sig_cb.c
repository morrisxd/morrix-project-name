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



void WPX_UFE_FRAMER_MIPS_SignallingCallbackTable_LineSideSectionK1K2Announce(U8 iDevice, U8 iPresentationLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	char Payload[3];
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Payload[0]=iLogicalLinePort; 
			Payload[1]=pRX_K1K2->K1;
			Payload[2]=pRX_K1K2->K2;

			OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_K1K2_CHANGE, Payload, 3);
		}
	}
}


void WPX_UFE_FRAMER_MIPS_SignallingCallbackTable_LineSideSectionS1Announce(U8 iDevice, U8 iPresentationLinePort, U8 S1)
{
	char Payload[2];
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Payload[0]=iLogicalLinePort; 
			Payload[1]=S1;

			OMIINO_AUX_ResponseWithPayload(iDevice, 0, 0, AUXILIARY_RESPONSE_S1_CHANGE, Payload, 2);
		}
	}
}



void WPX_UFE_FRAMER_MIPS_Initialize_BindingSignallingCallbackTable(WPX_UFE_FRAMER_SIGNALLING_TABLE_ANNOUNCE_TYPE * pSignallingCallbackTable)
{
	pSignallingCallbackTable->LineSideSectionK1K2Announce=WPX_UFE_FRAMER_MIPS_SignallingCallbackTable_LineSideSectionK1K2Announce;
	pSignallingCallbackTable->LineSideSectionS1Announce=WPX_UFE_FRAMER_MIPS_SignallingCallbackTable_LineSideSectionS1Announce;
}
