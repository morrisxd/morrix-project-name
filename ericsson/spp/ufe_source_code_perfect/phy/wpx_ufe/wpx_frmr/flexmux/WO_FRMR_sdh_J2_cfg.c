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



void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, U8 J2_Mode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_LO_Overhead_StaticRule_J2_ModeInRange(J2_Mode),J2_Mode);

	pPathConfiguration->Mode = J2_Mode;
}




void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, U8 *pJ2_Mode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_Mode,0);

	*pJ2_Mode = pPathConfiguration->Mode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_OK==OMIINO_FRAMER_SONET_SDH_LO_Overhead_StaticRule_J2_ModeInRange((*pJ2_Mode)),(*pJ2_Mode));
}



void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, char * pJ2_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_TX,0);

	memset(pPathConfiguration->TX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_16_BYTE_PATH_TRACE+1);

	strncpy( pPathConfiguration->TX, pJ2_TX, pPathConfiguration->Mode );
    OMIINO_FRAMER_Add_CRC7_ToTraceString(pPathConfiguration->Mode, pPathConfiguration->TX);

}


void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, char * pJ2_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_TX,0);

	memset(pJ2_TX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ2_TX, pPathConfiguration->TX, pPathConfiguration->Mode );
	pJ2_TX[(pPathConfiguration->Mode)-1]='\0';
}


void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetTX_WithCRC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, char * pJ2_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_TX,0);

	memset(pJ2_TX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ2_TX, pPathConfiguration->TX, pPathConfiguration->Mode );
	pJ2_TX[pPathConfiguration->Mode]='\0';
}



void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, char * pJ2_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_EX,0);

	memset(pPathConfiguration->EX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_16_BYTE_PATH_TRACE+1);

	strncpy( pPathConfiguration->EX, pJ2_EX, pPathConfiguration->Mode );
    OMIINO_FRAMER_Add_CRC7_ToTraceString(pPathConfiguration->Mode, pPathConfiguration->EX);

}



void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, char * pJ2_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_EX,0);

	memset(pJ2_EX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ2_EX, pPathConfiguration->EX, pPathConfiguration->Mode );
	pJ2_EX[(pPathConfiguration->Mode)-1]='\0';
}




void OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetEX_WithCRC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE * pPathConfiguration, char * pJ2_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ2_EX,0);

	memset(pJ2_EX, '\0', pPathConfiguration->Mode+1);

	strncpy( pJ2_EX, pPathConfiguration->EX, pPathConfiguration->Mode );
	pJ2_EX[pPathConfiguration->Mode]='\0';
}




