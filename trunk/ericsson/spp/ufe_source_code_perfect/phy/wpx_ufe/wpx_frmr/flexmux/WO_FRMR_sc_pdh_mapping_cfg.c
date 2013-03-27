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



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetMapping(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 Mapping)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_FRAMING_TYPES>Mapping,Mapping);

	pClient->Mapping = Mapping;
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetMapping(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 *pMapping)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pMapping,0);

	*pMapping = pClient->Mapping;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_FRAMING_TYPES>(*pMapping),(*pMapping));
}





