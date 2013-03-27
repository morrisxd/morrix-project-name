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



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_GainInRange(U32 Gain)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(MAX_TIMESTAMP_GAIN<Gain)
	{
		Result = WPX_UFE_FRAMER_ERROR_TIMESTAMP_GAIN_OUT_OF_RANGE;
	}

	return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_RateParameterInRange(U32 RateParameter)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(RateParameter)
	{
		case RECOVERED_CLOCK_RATE_PARAMETER_1544:
		case RECOVERED_CLOCK_RATE_PARAMETER_2048:
			Result=WPX_UFE_FRAMER_OK;
			break;

		default:
			Result=WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_RATE_PARAMETER_OUT_OF_RANGE;
			break;
	}

	return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_OutputSelectorInRange(U8 OutputSelector)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(MAX_SOCKET_CLIENT_RECOVERED_CLOCKS<=OutputSelector)
	{
		Result = WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_OUTPUT_SELECTOR_OUT_OF_RANGE;
	}

	return Result;
}

U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS<=iSocketClientPDH)
	{
		Result = WPX_UFE_FRAMER_ERROR_SOCKET_CLIENT_PORT_OUT_OF_RANGE;
	}

	return Result;
}



  
  
		 




U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_TypeInRange(U8 Type)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Type)
    {   
		case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_T1:
		case    WPX_UFE_FRAMER_CLIENT_E3:
                case    WPX_UFE_FRAMER_CLIENT_T3:
		case    WPX_UFE_FRAMER_CLIENT_C4:	
		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
		case    WPX_UFE_FRAMER_CLIENT_C3:
		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:	
                Result = WPX_UFE_FRAMER_OK;
                break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_OUT_OF_RANGE;
                break;

    }

	return Result;
}





U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketIsCompatibleWithType(U32 iSocketClientPDH, U8 ClientType)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(ClientType)
	{   
		case    WPX_UFE_FRAMER_CLIENT_E1:  
		case    WPX_UFE_FRAMER_CLIENT_T1:
				break;

		case    WPX_UFE_FRAMER_CLIENT_T3:
		case    WPX_UFE_FRAMER_CLIENT_E3:
				if(0!=(iSocketClientPDH%28))
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C4:
				if(0!=iSocketClientPDH%84)
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C4_4C:
				if(0!=(iSocketClientPDH%336))
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C3:
				if(0!=(iSocketClientPDH%28))
				{
	                Result = WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_C12:	
		case    WPX_UFE_FRAMER_CLIENT_C11:	
				break;

		default:
				Result = WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
				break;
	}

	return Result;
}







U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_MappingInRange(U8 Mapping)
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



U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_TimestampByteCountInRange(U8 AnyTimestampByteCount)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(0xC0!=AnyTimestampByteCount)
	{
		if(!(0x02<=AnyTimestampByteCount && 0x40>=AnyTimestampByteCount))
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_TIMESTAMP_BYTE_COUNT_OUT_OF_RANGE;
		}
	}

	return Result;
}




U8 OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_FramingInRange(U8 Framing)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Framing)
    {   
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
		case    WPX_UFE_FRAMER_CLIENT_UNFRAMED:
		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
		case	WPX_UFE_FRAMER_CLIENT_PCM31:
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





U8 OMIINO_FRAMER_SOCKET_CLIENT_PortStaticRule_ClockRecoveryModeInRange(U8 ClockRecoveryMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(ClockRecoveryMode)
    {   
		case    WPX_UFE_FRAMER_TRANSMIT_TIMING_RX_PATH:					/* Drop through */ 
		case    WPX_UFE_FRAMER_TRANSMIT_TIMING_SONET_TIMING:			/* Drop through */
		case	WPX_UFE_FRAMER_TRANSMIT_TIMING_WINPATH:					/* Drop through */
		case    WPX_UFE_FRAMER_TRANSMIT_TIMING_INPUT_REFERENCE_CLOCK:
                Result = WPX_UFE_FRAMER_OK;
				break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLOCK_RECOVERY_MODE_OUT_OF_RANGE;
                break;

    }

	return Result;
}


