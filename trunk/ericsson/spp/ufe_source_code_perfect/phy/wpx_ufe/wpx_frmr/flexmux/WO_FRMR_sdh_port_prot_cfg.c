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



void OMIINO_FRAMER_SONET_SDH_Port_Force_A(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_PROTECTION_TYPE * pPortProtection)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortProtection,0);

	pPortProtection->Force = WPX_UFE_FRAMER_PROTECTION_FORCE_A;
}



void OMIINO_FRAMER_SONET_SDH_Port_Force_B(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_PROTECTION_TYPE * pPortProtection)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortProtection,0);

	pPortProtection->Force = WPX_UFE_FRAMER_PROTECTION_FORCE_B;
}




void OMIINO_FRAMER_SONET_SDH_Port_GetForceState(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_PROTECTION_TYPE * pPortProtection, U8 *pForceSide)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortProtection,0);
	OMIINO_FRAMER_ASSERT(NULL!=pForceSide,0);

	*pForceSide = pPortProtection->Force;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_PROTECTION_FORCE_MAX>(*pForceSide),(*pForceSide));
}





