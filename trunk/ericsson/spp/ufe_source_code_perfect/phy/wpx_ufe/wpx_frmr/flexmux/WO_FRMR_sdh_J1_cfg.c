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



void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, U8 J1_Mode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_J1_ModeInRange(J1_Mode),J1_Mode);

	pPathConfiguration->Mode = J1_Mode;
}




void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, U8 *pJ1_Mode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_Mode,0);

	*pJ1_Mode = pPathConfiguration->Mode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_J1_ModeInRange((*pJ1_Mode)),(*pJ1_Mode));
}



void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, char * pJ1_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_TX,0);

    memset(pPathConfiguration->TX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1);

	strncpy( pPathConfiguration->TX, pJ1_TX, pPathConfiguration->Mode );

	switch(pPathConfiguration->Mode)
	{
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:
			/* do nothing - no CRC */
			break;
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:
			OMIINO_FRAMER_Add_CRC7_ToTraceString(pPathConfiguration->Mode, pPathConfiguration->TX);
			break;
		default:
			OMIINO_FRAMER_RSE(pPathConfiguration->Mode);
			break;
	}
}


void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, char * pJ1_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_TX,0);

	memset(pJ1_TX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ1_TX, pPathConfiguration->TX, pPathConfiguration->Mode );
	pJ1_TX[(pPathConfiguration->Mode)-1]='\0';
}


void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetTX_WithCRC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, char * pJ1_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_TX,0);

	memset(pJ1_TX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ1_TX, pPathConfiguration->TX, pPathConfiguration->Mode );
	pJ1_TX[pPathConfiguration->Mode]='\0';
}



void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, char * pJ1_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_EX,0);

	memset(pPathConfiguration->EX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1);

	strncpy( pPathConfiguration->EX, pJ1_EX, pPathConfiguration->Mode );

	switch(pPathConfiguration->Mode)
	{
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:
			/* do nothing - no CRC */
			break;
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:
		    OMIINO_FRAMER_Add_CRC7_ToTraceString(pPathConfiguration->Mode, pPathConfiguration->EX);
			break;
		default:
			OMIINO_FRAMER_RSE(pPathConfiguration->Mode);
			break;
	}
}



void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, char * pJ1_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_EX,0);

	memset(pJ1_EX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ1_EX, pPathConfiguration->EX, pPathConfiguration->Mode );
	pJ1_EX[pPathConfiguration->Mode-1]='\0';
}



void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetEX_WithCRC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPathConfiguration, char * pJ1_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_EX,0);

	memset(pJ1_EX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ1_EX, pPathConfiguration->EX, pPathConfiguration->Mode );
	pJ1_EX[(pPathConfiguration->Mode)]='\0';
}




