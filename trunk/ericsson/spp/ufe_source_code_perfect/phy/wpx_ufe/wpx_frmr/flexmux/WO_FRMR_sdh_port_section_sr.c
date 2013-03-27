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







U8 OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_J1_ModeInRange(U8 Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Mode)
    {    
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:      
			Result = WPX_UFE_FRAMER_OK;
			break;

        default: 
			Result = WPX_UFE_FRAMER_ERROR_PATH_TRACE_OUT_OF_RANGE;
			break;
    }

	return Result;
}






U8 OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_J0_ModeInRange(U8 Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Mode)
    {    
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE:       /* Drop through */ 
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:      
			Result = WPX_UFE_FRAMER_OK;
			break;

        default: 
			Result = WPX_UFE_FRAMER_ERROR_PATH_TRACE_OUT_OF_RANGE;
			break;
    }

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_B1_ExcessiveThresholdInRange(U8 Threshold)
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



U8 OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_B1_DegradeThresholdInRange(U8 Threshold)
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




