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



void OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_SetRX(OMIINO_FRAMER_STATUS_SONET_SDH_PORT_LINE_K1K2_TYPE * pStatusK1K2, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pRX_K1K2)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatusK1K2,0);
	OMIINO_FRAMER_ASSERT(NULL!=pRX_K1K2,0);

	pStatusK1K2->RX.K1 = pRX_K1K2->K1;
	pStatusK1K2->RX.K2 = pRX_K1K2->K2;
}




void OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_GetRX(OMIINO_FRAMER_STATUS_SONET_SDH_PORT_LINE_K1K2_TYPE * pStatusK1K2, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatusK1K2,0);
	OMIINO_FRAMER_ASSERT(NULL!=pRX_K1K2,0);

	pRX_K1K2->K1 = pStatusK1K2->RX.K1;
	pRX_K1K2->K2 = pStatusK1K2->RX.K2;
}



