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



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Enable_CAS(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->CAS_IsEnabled = 1;
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Disable_CAS(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->CAS_IsEnabled = 0;
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_CAS_State(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 *pCAS_IsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pCAS_IsEnabled,0);

	*pCAS_IsEnabled = pClient->CAS_IsEnabled;

	OMIINO_FRAMER_ASSERT(1>=(*pCAS_IsEnabled),(*pCAS_IsEnabled));
}





