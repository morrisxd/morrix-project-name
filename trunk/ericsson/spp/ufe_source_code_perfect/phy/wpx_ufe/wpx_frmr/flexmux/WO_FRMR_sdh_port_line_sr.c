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





U8 OMIINO_FRAMER_SONET_SDH_LineOverhead_StaticRule_B2_DegradeThresholdInRange(U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Threshold)
    {
		case WPX_UFE_FRAMER_THRESHOLD_OFF:		/* Drop through */ 
		case WPX_UFE_FRAMER_THRESHOLD_10E5:      
		case WPX_UFE_FRAMER_THRESHOLD_10E6:
		case WPX_UFE_FRAMER_THRESHOLD_10E7:      
		case WPX_UFE_FRAMER_THRESHOLD_10E8:      
		case WPX_UFE_FRAMER_THRESHOLD_10E9:
			Result = WPX_UFE_FRAMER_OK;
			break;

        default: 
			Result = WPX_UFE_FRAMER_ERROR_BIP_THRESHOLD_OUT_OF_RANGE;
			break;
    }

	return Result;
}




U8 OMIINO_FRAMER_SONET_SDH_LineOverhead_StaticRule_B2_ExcessiveThresholdInRange(U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Threshold)
    {
		case WPX_UFE_FRAMER_THRESHOLD_OFF:		/* Drop through */ 
		case WPX_UFE_FRAMER_THRESHOLD_10E3:
		case WPX_UFE_FRAMER_THRESHOLD_10E4:
		case WPX_UFE_FRAMER_THRESHOLD_10E5:      
			Result = WPX_UFE_FRAMER_OK;
			break;

        default: 
			Result = WPX_UFE_FRAMER_ERROR_BIP_THRESHOLD_OUT_OF_RANGE;
			break;
    }

	return Result;
}


