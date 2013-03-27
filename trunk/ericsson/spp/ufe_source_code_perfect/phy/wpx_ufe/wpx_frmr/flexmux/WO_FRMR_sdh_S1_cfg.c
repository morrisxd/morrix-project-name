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



void OMIINO_FRAMER_SONET_SDH_Port_Line_S1_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_S1_TYPE * pPortConfigurationS1, U8 S1_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfigurationS1,0);

	pPortConfigurationS1->TX = S1_TX;
}




void OMIINO_FRAMER_SONET_SDH_Port_Line_S1_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_S1_TYPE * pPortConfigurationS1, U8 *pS1_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfigurationS1,0);
	OMIINO_FRAMER_ASSERT(NULL!=pS1_TX,0);

	*pS1_TX = pPortConfigurationS1->TX;
}



