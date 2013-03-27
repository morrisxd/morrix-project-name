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




U8 OMIINO_FRAMER_SONET_SDH_Port_StaticRule_DCC_InRange(U8 DCC_Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_DCC_MAX<=DCC_Mode)
	{
		Result = WPX_UFE_FRAMER_ERROR_DCC_SETTING_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_StaticRule_OutputSelectorInRange(U8 OutputSelector)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(MAX_SONET_SDH_RECOVERED_CLOCKS<=OutputSelector)
	{
		Result = WPX_UFE_FRAMER_ERROR_SDH_OUTPUT_SELECTOR_OUT_OF_RANGE;
	}

	return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_StaticRule_SquelchModeInRange(U8 Squelch_Mode)
{
	U8 Result;

	if(Squelch_Mode < WPX_UFE_FRAMER_RECOVERED_CLOCK_SQUELCH_MODE_MAX)
		Result = WPX_UFE_FRAMER_OK;
	else
		Result = WPX_UFE_FRAMER_ERROR_SQUELCH_MODE_OUT_OF_RANGE;
	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS<=iLinePort)
	{
		Result = WPX_UFE_FRAMER_ERROR_LINE_PORT_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_Port_StaticRule_SFP_StateInRange(U8 SfpState)
{
    U8 Result = WPX_UFE_FRAMER_ERROR_SFP_STATE_OUT_OF_RANGE;
    switch(SfpState)
    {
    case    WPX_UFE_FRAMER_SFP_TX_STATE_DISABLE:
    case    WPX_UFE_FRAMER_SFP_TX_STATE_ENABLE:
        Result = WPX_UFE_FRAMER_OK;
        break;
    default:
        break;
    }
    return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_Port_StaticRule_RateInRange(U8 PortRate)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(PortRate)
    {
    
		case    WPX_UFE_FRAMER_LINE_PORT_RATE_OFF:	  /* Drop through */ 
        case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM1:
        case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM4:
        case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC3:
        case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC12:
                Result = WPX_UFE_FRAMER_OK;
                break;

        default:
                Result=WPX_UFE_FRAMER_ERROR_PORT_RATE_OUT_OF_RANGE;
                break;

    }

	return Result;
}





