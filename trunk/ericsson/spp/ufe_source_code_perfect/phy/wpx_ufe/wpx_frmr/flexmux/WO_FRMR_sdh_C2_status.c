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





void OMIINO_FRAMER_SONET_SDH_HO_Path_C2_SetRX(OMIINO_FRAMER_STATUS_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pStatus, U8 C2_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatus,0);
    pStatus->RX=C2_RX;
}


void OMIINO_FRAMER_SONET_SDH_HO_Path_C2_GetRX(OMIINO_FRAMER_STATUS_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pStatus, U8 * pC2_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatus,0);
	OMIINO_FRAMER_ASSERT(NULL!=pC2_RX,0);

    *pC2_RX=pStatus->RX;
}





