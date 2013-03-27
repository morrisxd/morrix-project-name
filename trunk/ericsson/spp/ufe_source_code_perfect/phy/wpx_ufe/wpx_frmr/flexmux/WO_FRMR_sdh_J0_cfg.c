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



void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, U8 J0_Mode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_J0_ModeInRange(J0_Mode),J0_Mode);

	pPortConfiguration->Mode = J0_Mode;
}




void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, U8 *pJ0_Mode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_Mode,0);

	*pJ0_Mode = pPortConfiguration->Mode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_J0_ModeInRange((*pJ0_Mode)),(*pJ0_Mode));
}



void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, char * pJ0_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_TX,0);

	memset(pPortConfiguration->TX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1);

	strncpy( pPortConfiguration->TX, pJ0_TX, pPortConfiguration->Mode );

	switch(pPortConfiguration->Mode)
	{
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE:
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:
			/* do nothing - no CRC */
			break;
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:
		    OMIINO_FRAMER_Add_CRC7_ToTraceString(pPortConfiguration->Mode, pPortConfiguration->TX);
			break;
		default:
			OMIINO_FRAMER_RSE(pPortConfiguration->Mode);
			break;
	}
}


void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, char * pJ0_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_TX,0);

	memset(pJ0_TX, '\0', pPortConfiguration->Mode+1);

	strncpy( pJ0_TX, pPortConfiguration->TX, pPortConfiguration->Mode );
    if(WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE!=pPortConfiguration->Mode)
    {
        pJ0_TX[(pPortConfiguration->Mode)-1]='\0';
    }
}


void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetTX_WithCRC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, char * pJ0_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_TX,0);

	memset(pJ0_TX, '\0', pPortConfiguration->Mode+1);

	strncpy( pJ0_TX, pPortConfiguration->TX, pPortConfiguration->Mode );
    if(WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE!=pPortConfiguration->Mode)
    {
        pJ0_TX[pPortConfiguration->Mode]='\0';
    }
}



void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, char * pJ0_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_EX,0);

	memset(pPortConfiguration->EX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1);

	strncpy( pPortConfiguration->EX, pJ0_EX, pPortConfiguration->Mode );

	switch(pPortConfiguration->Mode)
	{
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE:
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:
			/* do nothing - no CRC */
			break;
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:
			OMIINO_FRAMER_Add_CRC7_ToTraceString(pPortConfiguration->Mode, pPortConfiguration->EX);
			break;
		default:
			OMIINO_FRAMER_RSE(pPortConfiguration->Mode);
			break;
	}
}



void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, char * pJ0_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_EX,0);

	memset(pJ0_EX, '\0', pPortConfiguration->Mode+1);

	strncpy( pJ0_EX, pPortConfiguration->EX, pPortConfiguration->Mode );
    if(WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE!=pPortConfiguration->Mode)
    {
        pJ0_EX[(pPortConfiguration->Mode-1)]='\0';
    }
}



void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetEX_WithCRC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE * pPortConfiguration, char * pJ0_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_EX,0);

	memset(pJ0_EX, '\0', pPortConfiguration->Mode+1);

	strncpy( pJ0_EX, pPortConfiguration->EX, pPortConfiguration->Mode );
    if(WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE!=pPortConfiguration->Mode)
    {
        pJ0_EX[pPortConfiguration->Mode]='\0';
    }
}




