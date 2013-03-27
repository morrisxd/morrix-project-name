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




U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_IsFree(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_DISCRETE_CLIENT_Facility_IsFree(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_ALREADY_CONNECTED;
	}

	return Result;
}



U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_IsConnected(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_DISCRETE_CLIENT_Facility_IsConnected(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_NOT_CONNECTED;
	}

	return Result;
}




U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_DISCRETE_CLIENT_Facility_Exists(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
	}

	return Result;
}


U8 OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityDoesNotExist(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE *pClient)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_FRAMER_DISCRETE_CLIENT_Facility_Exists(pClient))
	{
		Result = WPX_UFE_FRAMER_ERROR_FACILITY_ALREADY_EXISTS;
	}

	return Result;
}





U8 OMIINO_FRAMER_DISCRETE_CLIENT_DynamicRule_MappingInRange(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE *pClient, U8 Mapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;


	switch(Mapping)
    {   
		case	WPX_UFE_FRAMER_CLIENT_MAPPING_BULK_FILL:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;
				
		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_ASYNC:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_T3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BYTE_SYNC:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
					case    WPX_UFE_FRAMER_CLIENT_T1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
                break;

		case    WPX_UFE_FRAMER_CLIENT_MAPPING_BIT_SYNC:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
                break;

    }

	return Result;
}



U8 OMIINO_FRAMER_DISCRETE_CLIENT_DynamicRule_FramingInRange(OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE *pClient, U8 Framing)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Framing)
    {   
		case    WPX_UFE_FRAMER_CLIENT_UNFRAMED:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_T3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case	WPX_UFE_FRAMER_CLIENT_PCM31:
		case	WPX_UFE_FRAMER_CLIENT_PCM31CRC:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_FRAMING_SF_D4:  /* Drop through */ 
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_ESF:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T1:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_E1:  /* Drop through */ 
					case    WPX_UFE_FRAMER_CLIENT_E3:
					case    WPX_UFE_FRAMER_CLIENT_T3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_M23: 
                case    WPX_UFE_FRAMER_CLIENT_FRAMING_DS3_CPARITY:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_T3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_E1: 
					case    WPX_UFE_FRAMER_CLIENT_E3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751:
		case    WPX_UFE_FRAMER_CLIENT_FRAMING_E3_G751_ATM:
				switch(pClient->ClientType)
				{   
					case    WPX_UFE_FRAMER_CLIENT_E3:
							Result = WPX_UFE_FRAMER_OK;
							break;

					case    WPX_UFE_FRAMER_CLIENT_T1:
					case    WPX_UFE_FRAMER_CLIENT_E1: 
					case    WPX_UFE_FRAMER_CLIENT_T3:
			                Result = WPX_UFE_FRAMER_ERROR_CLIENT_MAPPING_INVALID_FOR_CLIENT_TYPE;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_FACILITY_DOES_NOT_EXIST;
							break;
				}
				break;

        default:
                Result = WPX_UFE_FRAMER_ERROR_CLIENT_FRAMING_INVALID_FOR_CLIENT_TYPE;
                break;
    }

	return Result;
}



