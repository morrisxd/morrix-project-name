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



void OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetClientType(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pPortConfiguration, U8 ClientType)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_BANDIWDTH_TYPES>ClientType,ClientType);

	pPortConfiguration->ClientType = ClientType;
}




void OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetClientType(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pPortConfiguration, U8 *pClientType)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortConfiguration,0);
	OMIINO_FRAMER_ASSERT(NULL!=pClientType,0);

	*pClientType = pPortConfiguration->ClientType;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_BANDIWDTH_TYPES>(*pClientType),(*pClientType));
}





