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




U8 OMIINO_FRAMER_Diagnostic_StaticRule_PRBS_PatternInRange(U8 PRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(PRBS_Pattern)
    {    
		case WPX_UFE_FRAMER_PRBS_DISABLED:            /* Drop through */ 
        case WPX_UFE_FRAMER_PRBS_15:
		case WPX_UFE_FRAMER_PRBS_20:                 
		case WPX_UFE_FRAMER_PRBS_23:
		case WPX_UFE_FRAMER_PRBS_COUNT_SEQUENCE:      
		case WPX_UFE_FRAMER_PRBS_ALL_1:               
		case WPX_UFE_FRAMER_PRBS_ALL_0:
		case WPX_UFE_FRAMER_PRBS_ALTERNATING_01:
			Result = WPX_UFE_FRAMER_OK;
			break;

        default: 
			Result = WPX_UFE_FRAMER_ERROR_PRBS_PATTERN_TYPE_OUT_OF_RANGE;
			break;
    }

	return Result;
}



