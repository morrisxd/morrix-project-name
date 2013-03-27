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



void OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_K1K2_TYPE * pPortConfigurationK1K2, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfigurationK1K2,0);
	OMIINO_FRAMER_ASSERT(NULL!=pTX_K1K2,0);

	pPortConfigurationK1K2->TX.K1 = pTX_K1K2->K1;
	pPortConfigurationK1K2->TX.K2 = pTX_K1K2->K2;
}




void OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_K1K2_TYPE * pPortConfigurationK1K2, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pTX_K1K2)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfigurationK1K2,0);
	OMIINO_FRAMER_ASSERT(NULL!=pTX_K1K2,0);

	pTX_K1K2->K1 = pPortConfigurationK1K2->TX.K1;
	pTX_K1K2->K2 = pPortConfigurationK1K2->TX.K2;
}



