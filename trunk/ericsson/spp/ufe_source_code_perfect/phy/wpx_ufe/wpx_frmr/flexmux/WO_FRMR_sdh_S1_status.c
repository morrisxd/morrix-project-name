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



void OMIINO_FRAMER_SONET_SDH_Port_Line_S1_SetRX(OMIINO_FRAMER_STATUS_SONET_SDH_PORT_LINE_S1_TYPE * pStatusS1, U8 S1_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatusS1,0);

	pStatusS1->RX = S1_RX;
}




void OMIINO_FRAMER_SONET_SDH_Port_Line_S1_GetRX(OMIINO_FRAMER_STATUS_SONET_SDH_PORT_LINE_S1_TYPE * pStatusS1, U8 *pS1_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatusS1,0);
	OMIINO_FRAMER_ASSERT(NULL!=pS1_RX,0);

	*pS1_RX = pStatusS1->RX;
}



