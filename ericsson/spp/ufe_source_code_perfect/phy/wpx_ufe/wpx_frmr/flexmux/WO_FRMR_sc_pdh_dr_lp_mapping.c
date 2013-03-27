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





U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientOkForPortRate(U8 iDevice, U8 BuildPersonality, U8 iProtectionMode, U32 iSocketClientPDH)
{
	U8 iExpectedPort;
	U8 PortRate;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

    U8 Result = OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].ErrorCode;
	if(Result==WPX_UFE_FRAMER_OK)
	{
		iExpectedPort=OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpectedPort;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iExpectedPort);
		OMIINO_FRAMER_SONET_SDH_Port_GetRate(&(pDevicePortRAM->Configuration), &PortRate);

		switch(PortRate)
		{
                   case	WPX_UFE_FRAMER_LINE_PORT_RATE_OFF:
                      Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
					break;

			case	WPX_UFE_FRAMER_LINE_PORT_RATE_STM1:
			case	WPX_UFE_FRAMER_LINE_PORT_RATE_OC3:
					if(0!=OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_J)
					{
						Result=WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE;
					}
					break;

			case	WPX_UFE_FRAMER_LINE_PORT_RATE_STM4:
			case	WPX_UFE_FRAMER_LINE_PORT_RATE_OC12:
					Result=WPX_UFE_FRAMER_OK;
					break;

			default:
					Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
					break;
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect_SONET(U8 BuildPersonality, U8 iProtectionMode, U32 iSocketClientPDH, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].ErrorCode;

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpectedPort!=pLineEndpoint->iLinePort)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_U!=pLineEndpoint->u.sonet.iU)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_V!=pLineEndpoint->u.sonet.iV)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_W!=pLineEndpoint->u.sonet.iW)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect_SDH(U8 BuildPersonality, U8 iProtectionMode, U32 iSocketClientPDH, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].ErrorCode;

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpectedPort!=pLineEndpoint->iLinePort)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_J!=pLineEndpoint->u.sdh.iJ)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_K!=pLineEndpoint->u.sdh.iK)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_L!=pLineEndpoint->u.sdh.iL)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}


	if(Result==WPX_UFE_FRAMER_OK)
	{
		if(OMIINO_RAM.SocketDynamicRule[BuildPersonality][iProtectionMode].Element[iSocketClientPDH].iExpected_M!=pLineEndpoint->u.sdh.iM)
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_PORT_NOT_MAPPED_TO_LINE_ENDPOINT;
		}
	}

	return Result;
}



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect(U8 iDevice, U8 iProtectionMode, U32 iSocketClientPDH, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 BuildPersonality;
	OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);

	OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, &BuildPersonality);

	switch(BuildPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:

		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
				if(OMIINO_FRAMER_Is_SDH_Endpoint(pLineEndpoint))
				{
					Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect_SDH(BuildPersonality,iProtectionMode, iSocketClientPDH, pLineEndpoint);
				}
				else
				{
					Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect_SONET(BuildPersonality,iProtectionMode, iSocketClientPDH, pLineEndpoint);
				}
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
				break;

	}

	return Result;
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(U8 iPersonality, U8 iProtectionMode, U32 iElement, U8 iPort, U8 iJ, U8 iK, U8 iL, U8 iM)
{
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].ErrorCode=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpectedPort=iPort;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_J=iJ;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_K=iK;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_L=iL;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_M=iM;
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(U8 iPersonality, U8 iProtectionMode, U32 iElement, U8 iPort, U8 iU, U8 iV, U8 iW)
{
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].ErrorCode=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpectedPort=iPort;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_U=iU;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_V=iV;
	OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].iExpected_W=iW;
}

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<8;iPort+=2)
	{
		for(iJ=0;iJ<4;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;
	for(iPort=0;iPort<8;iPort+=2)
	{
		for(iU=0;iU<12;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12(void)
{
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iJ=0;iJ<4;iJ++)
	{
		for(iK=0;iK<3;iK++)
		{
			for(iL=0;iL<7;iL++)
			{
				for(iM=0;iM<4;iM++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, 0, iJ, iK, iL, iM);
				}
			}
		}
	}

	iSocket=0;
	for(iU=0;iU<12;iU++)
	{
		for(iV=0;iV<7;iV++)
		{
			for(iW=0;iW<4;iW++)
			{
				OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, 0, iU, iV, iW);
			}
		}
	}
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<8;iPort+=2)
	{
		for(iJ=0;iJ<1;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;
	for(iPort=0;iPort<8;iPort+=2)
	{
		for(iU=0;iU<3;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<16;iPort+=2)
	{
		for(iJ=0;iJ<1;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
                if (336 == iSocket)
                   iSocket+=336;
                
	}

	iSocket=0;
	for(iPort=0;iPort<16;iPort+=2)
	{
		for(iU=0;iU<3;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
                if (336 == iSocket)
                   iSocket+=336;
	}
}






void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<4;iPort+=2)
	{
		for(iJ=0;iJ<1;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;
	for(iPort=0;iPort<8;iPort+=2)
	{
		for(iU=0;iU<3;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_LINE, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}





void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<4;iPort++)
	{
		for(iJ=0;iJ<4;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;
	for(iPort=0;iPort<4;iPort++)
	{
		for(iU=0;iU<12;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12(void)
{
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iJ=0;iJ<4;iJ++)
	{
		for(iK=0;iK<3;iK++)
		{
			for(iL=0;iL<7;iL++)
			{
				for(iM=0;iM<4;iM++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, 0, iJ, iK, iL, iM);
				}
			}
		}
	}

	iSocket=0;
	for(iU=0;iU<12;iU++)
	{
		for(iV=0;iV<7;iV++)
		{
			for(iW=0;iW<4;iW++)
			{
				OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, 0, iU, iV, iW);
			}
		}
	}
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<4;iPort++)
	{
		for(iJ=0;iJ<1;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;

	for(iPort=0;iPort<4;iPort++)
	{
		for(iU=0;iU<3;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<8;iPort++)
	{
		for(iJ=0;iJ<1;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;

	for(iPort=0;iPort<8;iPort++)
	{
		for(iU=0;iU<3;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3(void)
{
	U8 iPort;
	U8 iJ;
	U8 iK;
	U8 iL;
	U8 iM;
	U8 iU;
	U8 iV;
	U8 iW;
	U32 iSocket=0;

	for(iPort=0;iPort<2;iPort++)
	{
		for(iJ=0;iJ<1;iJ++)
		{
			for(iK=0;iK<3;iK++)
			{
				for(iL=0;iL<7;iL++)
				{
					for(iM=0;iM<4;iM++)
					{
						OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SDH(WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iJ, iK, iL, iM);
					}
				}
			}
		}
	}

	iSocket=0;
	for(iPort=0;iPort<2;iPort++)
	{
		for(iU=0;iU<3;iU++)
		{
			for(iV=0;iV<7;iV++)
			{
				for(iW=0;iW<4;iW++)
				{
					OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Install_SONET(WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3, WPX_UFE_DEVICE_PROTECTION_MODE_CARD, iSocket++, iPort, iU, iV, iW);
				}
			}
		}
	}
}






void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_Default(void)
{
	int iPersonality;
	int iElement;
	int iProtectionMode;

	for(iPersonality=0;iPersonality<WPX_UFE_FRAMER_BUILD_PERSONALITY_MAX; iPersonality++)
	{
		for(iElement=0;iElement<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iElement++)
		{
			for(iProtectionMode=0;iProtectionMode<WPX_UFE_MAX_DEVICE_PROTECTION_MODES;iProtectionMode++)
			{
				OMIINO_RAM.SocketDynamicRule[iPersonality][iProtectionMode].Element[iElement].ErrorCode=WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE;
			}
		}
	}
}

void OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize(void)
{
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_Default();

   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_LINE_WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3();

   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3();
   OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMapping_Initialize_WPX_UFE_DEVICE_PROTECTION_MODE_CARD_WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3();
}





