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



void OMIINO_FRAMER_SONET_SDH_Port_EnableScrambling(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);

	pPortHierarchy->ScramblingIsEnabled = 1;
}



void OMIINO_FRAMER_SONET_SDH_Port_DisableScrambling(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);

	pPortHierarchy->ScramblingIsEnabled = 0;
}




void OMIINO_FRAMER_SONET_SDH_Port_GetScrambling(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 *pScramblingIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pScramblingIsEnabled,0);

	*pScramblingIsEnabled = pPortHierarchy->ScramblingIsEnabled;

	OMIINO_FRAMER_ASSERT(1>=(*pScramblingIsEnabled),(*pScramblingIsEnabled));
}





