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






U8 OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_J1_ModeInRange(U8 Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Mode)
    {    
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_16_BYTE:	/* Drop through */ 
		case WPX_UFE_FRAMER_PATH_TRACE_MODE_64_BYTE:      
			Result = WPX_UFE_FRAMER_OK;
			break;

        default: 
			Result = WPX_UFE_FRAMER_ERROR_PATH_TRACE_OUT_OF_RANGE;
			break;
    }

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_B3_ExcessiveThresholdInRange(U8 Threshold)
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




U8 OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_B3_DegradeThresholdInRange(U8 Threshold)
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



U8 OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_G1_ThresholdInRange(U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Threshold)
    {    
		case WPX_UFE_FRAMER_THRESHOLD_10E3:       /* Drop through */ 
		case WPX_UFE_FRAMER_THRESHOLD_10E4:
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

U8 OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_G1_ErdiModeInRange(U8 Mode)
{
	U8 Result;

	if(Mode < WPX_UFE_FRAMER_RDI_MODE_MAX )
		Result = WPX_UFE_FRAMER_OK;
	else
		Result = WPX_UFE_FRAMER_ERROR_RDI_MODE_OUT_OF_RANGE;
	return Result;
}
U8 OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_SS_BITS_TxValueInRange(U8 Value)
{
	U8 Result;

	if(Value <= WPX_UFE_FRAMER_SS_BITS_TX_MAX)
		Result = WPX_UFE_FRAMER_OK;
	else
		Result =WPX_UFE_FRAMER_ERROR_SS_BITS_VALUE_OUT_OF_RANGE ;
	return Result;
}

