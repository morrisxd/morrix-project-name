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



void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Ingress_PRBS_State(OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 PRBS_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_PRBS_STATE_MAX_TYPES>PRBS_State,PRBS_State);

	pClient->Ingress_PRBS_State = PRBS_State;
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Ingress_PRBS_State(OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 *pPRBS_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pPRBS_State,0);

	*pPRBS_State = pClient->Ingress_PRBS_State;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_PRBS_STATE_MAX_TYPES>(*pPRBS_State),(*pPRBS_State));
}


void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Egress_PRBS_State(OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 PRBS_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_PRBS_STATE_MAX_TYPES>PRBS_State,PRBS_State);

	pClient->Egress_PRBS_State = PRBS_State;
}




void OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Egress_PRBS_State(OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient, U8 *pPRBS_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pClient,0);
	OMIINO_FRAMER_ASSERT(NULL!=pPRBS_State,0);

	*pPRBS_State = pClient->Egress_PRBS_State;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_PRBS_STATE_MAX_TYPES>(*pPRBS_State),(*pPRBS_State));
}





