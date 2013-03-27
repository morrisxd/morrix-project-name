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




U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_DISCRETE_CLIENT_PORTS<=iDiscreteClientPort)
	{
		Result = WPX_UFE_FRAMER_ERROR_DISCRETE_CLIENT_PORT_OUT_OF_RANGE;
	}

	return Result;
}




U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_TypeInRange(U8 Type)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Type)
    {   
		case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_T1:
		case    WPX_UFE_FRAMER_CLIENT_E3:
                case    WPX_UFE_FRAMER_CLIENT_T3:
                Result = WPX_UFE_FRAMER_OK;
                break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_OUT_OF_RANGE;
                break;

    }

	return Result;
}



U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_MappingInRange(U8 Mapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Mapping)
    {   
		case	WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC:
		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_SYNC:
		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BYTE_SYNC:
                   Result = WPX_UFE_FRAMER_OK;
                break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_OUT_OF_RANGE;
                break;

    }

	return Result;
}



U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_FramingInRange(U8 Framing)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Framing)
    {   
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
		case    WPX_UFE_FRAMER_CLIENT_UNFRAMED:
		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23:
                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM:
                   Result = WPX_UFE_FRAMER_OK;
                break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_OUT_OF_RANGE;
                break;

    }

	return Result;
}

		        
				


U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_ClockRecoveryModeInRange(U8 ClockRecoveryMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(ClockRecoveryMode)
    {   
		case    WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH:  /* Drop through */ 
		case    WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK:
                Result = WPX_UFE_FRAMER_OK;
				break;

		case    WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING:
		case	WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH:
                Result = WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_INVALID_FOR_DISCRETE_CLIENT;
                break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_OUT_OF_RANGE;
                break;

    }

	return Result;
}





