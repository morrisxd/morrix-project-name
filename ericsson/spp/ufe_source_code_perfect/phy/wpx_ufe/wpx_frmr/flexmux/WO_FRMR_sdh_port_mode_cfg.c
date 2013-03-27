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



void OMIINO_FRAMER_SONET_SDH_Port_SetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 PortMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_DEVICE_MODES>PortMode,PortMode);

	pPortHierarchy->PortMode = PortMode;
}




void OMIINO_FRAMER_SONET_SDH_Port_GetMode(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 *pPortMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pPortMode,0);

	*pPortMode = pPortHierarchy->PortMode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_DEVICE_MODES>(*pPortMode),(*pPortMode));
}





