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





void OMIINO_FRAMER_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SetRX(OMIINO_FRAMER_STATUS_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pStatus, U8 V5_SignalLabel_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatus,0);
    pStatus->RX=V5_SignalLabel_RX;
}


void OMIINO_FRAMER_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GetRX(OMIINO_FRAMER_STATUS_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pStatus, U8 * pV5_SignalLabel_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pStatus,0);
	OMIINO_FRAMER_ASSERT(NULL!=pV5_SignalLabel_RX,0);

    *pV5_SignalLabel_RX=pStatus->RX;
}





