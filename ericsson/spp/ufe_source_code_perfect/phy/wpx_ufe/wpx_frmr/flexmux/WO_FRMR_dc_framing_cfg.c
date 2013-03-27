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



void OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetFraming(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pClient, U8 Framing)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_FRAMING_TYPES>Framing,Framing);

	pClient->Framing = Framing;
}




void OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetFraming(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE * pClient, U8 *pFraming)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pFraming,0);

	*pFraming = pClient->Framing;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_CLIENT_MAX_FRAMING_TYPES>(*pFraming),(*pFraming));
}





