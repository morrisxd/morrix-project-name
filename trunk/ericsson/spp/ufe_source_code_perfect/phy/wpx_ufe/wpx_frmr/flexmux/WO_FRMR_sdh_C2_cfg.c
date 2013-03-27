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





void OMIINO_FRAMER_SONET_SDH_HO_Path_C2_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pPathConfiguration, U8 C2_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
    pPathConfiguration->TX=C2_TX;
}


void OMIINO_FRAMER_SONET_SDH_HO_Path_C2_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pPathConfiguration, U8 * pC2_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pC2_TX,0);

    *pC2_TX=pPathConfiguration->TX;
}



void OMIINO_FRAMER_SONET_SDH_HO_Path_C2_SetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pPathConfiguration, U8 C2_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
    pPathConfiguration->TX=C2_EX;
}



void OMIINO_FRAMER_SONET_SDH_HO_Path_C2_GetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pPathConfiguration, U8 * pC2_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPathConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pC2_EX,0);

    *pC2_EX=pPathConfiguration->EX;
}




