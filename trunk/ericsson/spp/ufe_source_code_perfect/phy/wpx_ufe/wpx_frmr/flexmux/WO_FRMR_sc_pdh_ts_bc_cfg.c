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


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetTimestampByteCount(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 * pAnyTimestampByteCount)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	*pAnyTimestampByteCount=pClient->TimestampByteCount;
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetTimestampByteCount(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 AnyTimestampByteCount)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->TimestampByteCount = AnyTimestampByteCount;
}






void OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetEnableTimestamping(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U32 Gain)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->TimestampingIsEnabled=1;
	pClient->Gain=Gain;
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetDisableTimestamping(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	pClient->TimestampingIsEnabled=0;
}



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetTimestamping(OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 * pTimeStamping_IsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);

	*pTimeStamping_IsEnabled=pClient->TimestampingIsEnabled;
}


