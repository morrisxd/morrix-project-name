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


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyMessage(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    U32 MessageIdentifier=0;
    U8 * pMessage;
    U8  Length;
    U32 U32_Length;

	pMessage=(U8 *)pFramerRAM->Device[iDevice].LLC_NorthboundBuffer.Buffer;
    U32_Length=16+UtilityGetU32(&pMessage[8]);

    if(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX>=U32_Length)
    {
        Length=(U8)U32_Length;

        if(4<Length)
        {
            MessageIdentifier=UtilityGetU32(&pMessage[0]);

            if(MAX_APIS>MessageIdentifier)
            {
                (*pFramerRAM->LLC_Table.AuxiliaryTable.Parser[MessageIdentifier])(MessageIdentifier, iDevice, pMessage, Length);
            }
            else
            {
                if((unsigned int)OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE!=MessageIdentifier)
                {
                    OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
                }
            }
        }
        else
        {
            OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(0x0000DEAD, iDevice, pMessage, Length, __LINE__, __FILE__);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(0x0000DEAD, iDevice, pMessage, 0xFF, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryParser_NULL(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
}


void OMIINO_LLC_Northbound_AuxiliaryParser_IGNORE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DRIVER_RESET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	OMIINO_API_INTERNAL_Reset();
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_LIST_ALL_ACTIVE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_LIST_ALL_ACTIVE(iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}








void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_INGRESS_SET_PRBS_TERM(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8  API_Result;
	U32 TransactionIdentifier;
    U8 PRBS_Pattern;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	PRBS_Pattern=pMessage[16];

	API_Result=OMIINO_API_INTERNAL_BULK_SOCKET_INGRESS_SET_PRBS_TERM(TransactionIdentifier, iDevice, PRBS_Pattern);

	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_INGRESS_SET_PRBS_GEN(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8  API_Result;
	U32 TransactionIdentifier;
    U8 PRBS_Pattern;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	PRBS_Pattern=pMessage[16];

	API_Result=OMIINO_API_INTERNAL_BULK_SOCKET_INGRESS_SET_PRBS_GEN(TransactionIdentifier, iDevice, PRBS_Pattern);

	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_EGRESS_SET_PRBS_TERM(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8  API_Result;
	U32 TransactionIdentifier;
    U8 PRBS_Pattern;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	PRBS_Pattern=pMessage[16];

	API_Result=OMIINO_API_INTERNAL_BULK_SOCKET_EGRESS_SET_PRBS_TERM(TransactionIdentifier, iDevice, PRBS_Pattern);

	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_EGRESS_SET_PRBS_GEN(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8  API_Result;
	U32 TransactionIdentifier;
    U8 PRBS_Pattern;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	PRBS_Pattern=pMessage[16];

	API_Result=OMIINO_API_INTERNAL_BULK_SOCKET_EGRESS_SET_PRBS_GEN(TransactionIdentifier, iDevice, PRBS_Pattern);

	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LINE(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LINE(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_HO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_HO(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_HO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_HO(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_LO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_LO(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_LO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_LO(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_ALL(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_ENABLE_ALL(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_ALL(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_ALL(TransactionIdentifier, iDevice);
	
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];
	U8 Bandwidth=pMessage[17];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 AnyClientType=pMessage[16];
	U8 Framing=pMessage[17];
	U8 Mapping=pMessage[18];
	U8 ClockParameter=pMessage[19];
	U8 AnyTimestampByteCount=pMessage[20];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_SOCKET_CLIENT_PDH_Facility_Create(TransactionIdentifier, iDevice, AnyClientType, Framing, Mapping, ClockParameter, AnyTimestampByteCount);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];
	U8 Bandwidth=pMessage[17];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_SOCKET_CLIENT_PDH_Facility_Delete(TransactionIdentifier, iDevice);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];
	U8 Bandwidth=pMessage[17];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_ENABLE_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_PM_ENABLE_LINE(TransactionIdentifier, iDevice, iLinePort);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_DISABLE_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_PM_DISABLE_LINE(TransactionIdentifier, iDevice, iLinePort);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_ENABLE_HO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_PM_ENABLE_HO(TransactionIdentifier, iDevice, iLinePort);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_DISABLE_HO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_PM_DISABLE_HO(TransactionIdentifier, iDevice, iLinePort);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_ENABLE_LO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_PM_ENABLE_LO(TransactionIdentifier, iDevice, iLinePort);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_DISABLE_LO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_PM_DISABLE_LO(TransactionIdentifier, iDevice, iLinePort);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DELETE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];
	U8 Bandwidth=pMessage[17];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CREATE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];
	U8 Bandwidth=pMessage[17];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	API_Result=WPX_UFE_FRAMER_OK;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 iLinePort=pMessage[16];
	U8 Bandwidth=pMessage[17];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}








void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_SET_BUILD_PERSONALITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 BuildPersonality;
	U8 API_Result;
	U8  iNextU8=16;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	BuildPersonality=pMessage[iNextU8++];

	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_SetBuildPersonality(iDevice, BuildPersonality);
	OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_GET_BUILD_PERSONALITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8	BuildPersonality;
	U8	API_Result;

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_GetBuildPersonality(iDevice, &BuildPersonality);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_GET_BUILD_PERSONALITY, (char *)&BuildPersonality, 1);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}


void OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_DRIVER_VERSION_GET_INFORMATION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	char ProductReleaseStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];
    char SoftwareBuildDateTimeStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];
    char SoftwareBuildVersionStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);


	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);


	memset(ProductReleaseStr,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1);
    OMIINO_FRAMER_STATUS_DeviceDriver_GetProductReleaseStr(&OMIINO_RAM.Driver,ProductReleaseStr);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, WPX_UFE_FRAMER_OK, AUXILIARY_RESPONSE_DEVICE_DRIVER_PRODUCT_RELEASE, ProductReleaseStr, (unsigned char)strlen(ProductReleaseStr));

	memset(SoftwareBuildDateTimeStr,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1);
	OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildDateTimeStr(&OMIINO_RAM.Driver,SoftwareBuildDateTimeStr);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, WPX_UFE_FRAMER_OK, AUXILIARY_RESPONSE_DEVICE_DRIVER_BUILD_DATE_TIME, SoftwareBuildDateTimeStr, (unsigned char)strlen(SoftwareBuildDateTimeStr));

	memset(SoftwareBuildVersionStr,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1);
    OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildVersionStr(&OMIINO_RAM.Driver,SoftwareBuildVersionStr);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, WPX_UFE_FRAMER_OK, AUXILIARY_RESPONSE_DEVICE_DRIVER_BUILD_VERSION, SoftwareBuildVersionStr, (unsigned char)strlen(SoftwareBuildVersionStr));
}


void OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_VERSION_GET_FIRMWARE_INFORMATION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	char Information[OMIINO_FRAMER_MAX_CHARS_IN_DEVICE_INFORMATION_FIELD+1];
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 API_Result;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	memset(Information,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DEVICE_INFORMATION_FIELD+1);
	API_Result=OMIINO_API_INTERNAL_STATUS_DEVICE_VERSION_GetFirmwareInformation(iDevice, Information);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_FIRMWARE_VERSION, Information, (unsigned char)strlen(Information));
}


void OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_VERSION_GET_HARDWARE_INFORMATION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	char Information[OMIINO_FRAMER_MAX_CHARS_IN_DEVICE_INFORMATION_FIELD+1];
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 API_Result;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	memset(Information,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DEVICE_INFORMATION_FIELD+1);
	API_Result=OMIINO_API_INTERNAL_STATUS_DEVICE_VERSION_GetHardwareInformation(iDevice, Information);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_HARDWARE_INFORMATION, Information, (unsigned char)strlen(Information));
}


void OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_BUILD_GET_HARDWARE_VARIANT(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	char Information;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	U8 API_Result;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_STATUS_DEVICE_BUILD_GetHardwareVariant(iDevice, (unsigned char *)&Information);
	
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_HARDWARE_VARIANT, &Information, 1);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_GET_DEVICE_CARD_PROTECTION_OPERATING_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 CardProtectionOperatingMode;
	U8 API_Result;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_GetDeviceCardProtectionOperatingMode(iDevice, &CardProtectionOperatingMode);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_CARD_PROTECTION_OPERATING_MODE, (char *)&CardProtectionOperatingMode, 1);
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_GET_DEVICE_PROTECTION_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 DeviceProtectionMode;
	U8 API_Result;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_GetDeviceProtectionMode(iDevice, &DeviceProtectionMode);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_PROTECTION_MODE, (char *)&DeviceProtectionMode, 1);
}


void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_DEVICE_GET_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 DeviceMode;
	U8 API_Result;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_GetMode(iDevice, &DeviceMode);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_MODE, (char *)&DeviceMode, 1);
}



void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_RATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 PortRate;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetRate(iDevice, iLinePort, (unsigned char *)&PortRate);

	Payload[0]=iLinePort;
	Payload[1]=PortRate;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PORT_GET_RATE, (char *)Payload, 2);
}





void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 DCC_Mode;
	U8 DCC_State;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[3];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	DCC_Mode=pMessage[17];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Get(iDevice, iLinePort, DCC_Mode, &DCC_State);

	Payload[0]=iLinePort;
	Payload[1]=DCC_Mode;
	Payload[2]=DCC_State;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PORT_DCC_GET, (char *)Payload, 3);
}



void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_RX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 API_Result;
	U8 iLinePort=0;
	char Payload_iLinePort_PathTrace[64+2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	Payload_iLinePort_PathTrace[0]=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetRX(iDevice, iLinePort, &Payload_iLinePort_PathTrace[1]);

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_RX, Payload_iLinePort_PathTrace, 1+(unsigned char)strlen(&Payload_iLinePort_PathTrace[1]));
}


void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 API_Result;
	U8 iLinePort=0;
	char Payload_iLinePort_PathTrace[64+2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	Payload_iLinePort_PathTrace[0]=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetTX(iDevice, iLinePort, &Payload_iLinePort_PathTrace[1]);

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_TX, Payload_iLinePort_PathTrace, 1+(unsigned char)strlen(&Payload_iLinePort_PathTrace[1]));
}

void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 API_Result;
	U8 iLinePort=0;
	char Payload_iLinePort_PathTrace[64+2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	Payload_iLinePort_PathTrace[0]=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetEX(iDevice, iLinePort, &Payload_iLinePort_PathTrace[1]);

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_EX, Payload_iLinePort_PathTrace, 1+(unsigned char)strlen(&Payload_iLinePort_PathTrace[1]));
}


void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 Mode;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetMode(iDevice, iLinePort, (unsigned char *)&Mode);

	Payload[0]=iLinePort;
	Payload[1]=Mode;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_J0_GET_MODE, (char *)Payload, 2);
}







void OMIINO_LLC_Northbound_Auxiliary_API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_RX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 API_Result;
	U8 iLinePort=0;
	WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE K1K2;
	char Payload[3];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GetRX(iDevice, iLinePort, &K1K2);

	Payload[0]=iLinePort;
	Payload[1]=K1K2.K1;
	Payload[2]=K1K2.K2;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_K1K2_GET_RX, (char *)Payload, 3);
}




void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 API_Result;
	U8 iLinePort=0;
	WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE K1K2;
	char Payload[3];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GetTX(iDevice, iLinePort, &K1K2);

	Payload[0]=iLinePort;
	Payload[1]=K1K2.K1;
	Payload[2]=K1K2.K2;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_K1K2_GET_TX, (char *)Payload, 3);
}




void OMIINO_LLC_Northbound_Auxiliary_API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_RX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 Value;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GetRX(iDevice, iLinePort, (unsigned char *)&Value);

	Payload[0]=iLinePort;
	Payload[1]=Value;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_S1_GET_RX, (char *)Payload, 2);
}


void OMIINO_LLC_Northbound_Auxiliary_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 Value;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GetTX(iDevice, iLinePort, (unsigned char *)&Value);

	Payload[0]=iLinePort;
	Payload[1]=Value;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_LINE_S1_GET_TX, (char *)Payload, 2);
}



void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_DEEP_SYSTEM_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 LoopbackIsEnabled;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GetDeepSystemState(iDevice, iLinePort, (unsigned char *)&LoopbackIsEnabled);

	Payload[0]=iLinePort;
	Payload[1]=LoopbackIsEnabled;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DEEP_SYSTEM_STATE, (char *)Payload, 2);
}


void OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_SHALLOW_LINE_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 LoopbackIsEnabled;
	U8 API_Result;
	U8 iLinePort=0;
	char Payload[2];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GetShallowLineState(iDevice, iLinePort, (unsigned char *)&LoopbackIsEnabled);

	Payload[0]=iLinePort;
	Payload[1]=LoopbackIsEnabled;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_SHALLOW_LINE_STATE, (char *)Payload, 2);
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_SET_DEVICE_PROTECTION_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceMode) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  DeviceProtectionMode;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        DeviceProtectionMode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_SetDeviceProtectionMode(TransactionIdentifier, iDevice, DeviceProtectionMode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_SET_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceMode) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  DeviceMode;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        DeviceMode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_SetMode(TransactionIdentifier, iDevice, DeviceMode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_SET_AIS_INSERTION_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AIS_InsertionMode;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AIS_InsertionMode=pMessage[iNextU8++];

		if(1==AIS_InsertionMode)
		{
			API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Enable_AIS_Insertion(TransactionIdentifier, iDevice);
		}
		else
		{
			API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Disable_AIS_Insertion(TransactionIdentifier, iDevice);
		}

        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_GET_AIS_INSERTION_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 AIS_InsertionMode;
	U8 API_Result;
	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Get_AIS_Insertion(iDevice, &AIS_InsertionMode);
	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_AIS_INSERTION_MODE, (char *)&AIS_InsertionMode, 1);
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_POKE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32  Address;
    U32  Value;

    if(24!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        Address=UtilityGetU32(&pMessage[iNextU8]);
        Value=UtilityGetU32(&pMessage[iNextU8+4]);

		API_Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_Poke(TransactionIdentifier, iDevice, Address, Value);

        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_BLOCK_POKE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32  StartAddress;
    U32  Value;
	U32 NumberOfU32AddressesToBeWritten;

    if(24!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        StartAddress=UtilityGetU32(&pMessage[iNextU8]);
        Value=UtilityGetU32(&pMessage[iNextU8+4]);
		NumberOfU32AddressesToBeWritten=UtilityGetU32(&pMessage[iNextU8+8]);

		API_Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_BlockPoke(TransactionIdentifier, iDevice, StartAddress, NumberOfU32AddressesToBeWritten, Value);

        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32  StartAddress;
    U32  Value;
	U32 NumberOfU32AddressesToBeWritten;

    if(24!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        StartAddress=UtilityGetU32(&pMessage[iNextU8]);
        Value=UtilityGetU32(&pMessage[iNextU8+4]);
		NumberOfU32AddressesToBeWritten=UtilityGetU32(&pMessage[iNextU8+8]);

		API_Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_BlockPokeVerify(TransactionIdentifier, iDevice, StartAddress, NumberOfU32AddressesToBeWritten, Value);

        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_PEEK(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32  Address;

    if(20!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        Address=UtilityGetU32(&pMessage[iNextU8]);

		API_Result=OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_Peek(iDevice, Address);

        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}






void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint) */

    U8  API_Result;
    U8  Result=1;
    U8  BytesParsedThisField;
    U8  iNextU8=16;
    
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;

    if(21>Length)
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        
        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, &LineEndpoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Bandwidth, U8 ClientApplication) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 ClientType;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

        ClientType=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(TransactionIdentifier, iDevice, iSocketClientPDH, ClientType);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_E1_GET_MONITORING_CRC(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
	U8  Payload[6];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

		UtilityPutU32(iSocketClientPDH, Payload);
        API_Result=OMIINO_API_INTERNAL_PM_E1_GET_MONITORING_CRC_State(iDevice, iSocketClientPDH, &Payload[4]);
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_E1_GET, (char *)Payload, 5);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_E1_ENABLE_MONITORING_CRC(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_PM_E1_ENABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_E1_DISABLE_MONITORING_CRC(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_PM_E1_DISABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}








void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_T1_GET_MONITORING_CRC(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
	U8  Payload[6];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

		UtilityPutU32(iSocketClientPDH, Payload);
        API_Result=OMIINO_API_INTERNAL_PM_T1_GET_MONITORING_CRC_State(iDevice, iSocketClientPDH, &Payload[4]);
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_T1_GET, (char *)Payload, 5);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_T1_ENABLE_MONITORING_CRC(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_PM_T1_ENABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_T1_DISABLE_MONITORING_CRC(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_PM_T1_DISABLE_MONITORING_CRC(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Bandwidth) */
    
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U8 ClientType;

    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        ClientType=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(TransactionIdentifier, iDevice, iDiscreteClientPort, ClientType);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint) */

    U8  API_Result;
    U8  Result=1;
    U8  BytesParsedThisField;
    U8  iNextU8=16;
    
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;

    if(21>Length)
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
  
        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, &LineEndpoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 PortRate;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        PortRate=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(TransactionIdentifier, iDevice, iLinePort, PortRate,WPX_UFE_FRAMER_SFP_TX_ENABLE);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 DCC_Mode;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        DCC_Mode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Enable(TransactionIdentifier, iDevice, iLinePort, DCC_Mode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 DCC_Mode;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        DCC_Mode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Disable(TransactionIdentifier, iDevice, iLinePort, DCC_Mode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_SET_DEVICE_CARD_PROTECTION_OPERATING_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 CardProtectionOperatingMode;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        CardProtectionOperatingMode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_SetDeviceCardProtectionOperatingMode(TransactionIdentifier, iDevice, CardProtectionOperatingMode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}









void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_ENABLE_SCRAMBLING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_EnableScrambling(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_EnableScrambling(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DISABLE_SCRAMBLING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DisableScrambling(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DisableScrambling(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_FORCE_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
	U8 ForceIs_A;
	U8 Payload[2];



    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetForceState(iDevice, iLinePort, &ForceIs_A);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[0]=iLinePort;
			Payload[1]=ForceIs_A;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_GET_FORCE_STATE, (char *)Payload, 2);
		}
		else
		{
			OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
		}
        
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_A(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_A(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_B(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_B(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 OutputSelector;
    U8  iNextU8=16;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);
    OutputSelector=pMessage[iNextU8++];

    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Get(iDevice, OutputSelector, &iLinePort);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET, (char *)&iLinePort, 1);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Set(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
	U8 OutputSelector;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        OutputSelector=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Set(TransactionIdentifier, iDevice, OutputSelector, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableShallowLine(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallowLine(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableDeepSystem(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;


    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableDeepSystem(TransactionIdentifier, iDevice, iLinePort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Mode) */
    
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Mode;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        Mode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(TransactionIdentifier, iDevice, iLinePort, Mode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pEX_TraceString) */
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  LocalBytesParsed;

    U32     TransactionIdentifier;
    U8      iLinePort;
    char    PathTrace[64+1];


    if(18>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        if(Length>iNextU8)
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &LocalBytesParsed);
            iNextU8+=LocalBytesParsed;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(TransactionIdentifier, iDevice, iLinePort, PathTrace);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pTX_TraceString) */
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  LocalBytesParsed;

    U32     TransactionIdentifier;
    U8      iLinePort;
    char    PathTrace[64+1];


    if(18>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];

        if(Length>iNextU8)
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &LocalBytesParsed);
            iNextU8+=LocalBytesParsed;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetTX(TransactionIdentifier, iDevice, iLinePort, PathTrace);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;
	char Payload[2];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iLinePort=pMessage[iNextU8++];
	Payload[0]=iLinePort;
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GetExcessive(iDevice, iLinePort, &Threshold);
    
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		Payload[1]=Threshold;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD, Payload, 2);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD, Payload, 1);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;
	char Payload[2];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iLinePort=pMessage[iNextU8++];
	Payload[0]=iLinePort;
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GetSignalDegradeThreshold(iDevice, iLinePort, &Threshold);
    
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		Payload[1]=Threshold;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD, Payload, 2);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD, Payload, 1);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;
	char Payload[2];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iLinePort=pMessage[iNextU8++];
	Payload[0]=iLinePort;
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GetExcessive(iDevice, iLinePort, &Threshold);
    
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		Payload[1]=Threshold;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_FAIL_THRESHOLD, Payload, 2);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_FAIL_THRESHOLD, Payload, 1);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;
	char Payload[2];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iLinePort=pMessage[iNextU8++];
	Payload[0]=iLinePort;
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GetSignalDegradeThreshold(iDevice, iLinePort, &Threshold);
    
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		Payload[1]=Threshold;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_DEGRADE_THRESHOLD, Payload, 2);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_DEGRADE_THRESHOLD, Payload, 1);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold) */
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        Threshold=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(TransactionIdentifier, iDevice, iLinePort, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold) */
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        Threshold=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, iLinePort, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE K1K2;


    if(19!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        K1K2.K1=pMessage[iNextU8++];
        K1K2.K2=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(TransactionIdentifier, iDevice, iLinePort, &K1K2);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 TX_S1) */
     U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 TX_S1;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        TX_S1=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(TransactionIdentifier, iDevice, iLinePort, TX_S1);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold) */
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        Threshold=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(TransactionIdentifier, iDevice, iLinePort, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 Threshold;


    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        Threshold=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, iLinePort, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	/* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Enable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType) */
	    
	U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;


    if(21>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }


    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Enable(TransactionIdentifier, iDevice, &LineEndpoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	/* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Disable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType) */
	    
	U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;


    if(21>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }


    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Disable(TransactionIdentifier, iDevice, &LineEndpoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{	    
	U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
	U8 IsEnabled;
    U8  BytesParsedThisField;
	U8 EndpointEncodedBytes;
	unsigned char Payload[1+16];

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;


    if(21>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }


    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GetState(iDevice, &LineEndpoint, &IsEnabled);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=IsEnabled;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}






void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	U8 EndpointEncodedBytes;
	unsigned char Payload[16+64+1];
	U8 J1_Length;

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetTX(iDevice, &LineEndpoint, (char *)&Payload[EndpointEncodedBytes]);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
                   J1_Length=(U8)strlen((const char *)&Payload[EndpointEncodedBytes]);
                   /*before we call the OMIINO_AUX_ResponseWithPayload function we make sure that we going to read less chars then the size of the array*/
                   OMIINO_FRAMER_ASSERT((J1_Length+EndpointEncodedBytes <= 16+64+1),J1_Length+EndpointEncodedBytes);

                   OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_TX, (char *)Payload, J1_Length+EndpointEncodedBytes);
                   
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_TX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	U8 EndpointEncodedBytes;
	unsigned char Payload[16+64+1];
	U8 J1_Length;

    
    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetEX(iDevice, &LineEndpoint, (char *)&Payload[EndpointEncodedBytes]);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
                   J1_Length=(U8)strlen((const char *)&Payload[EndpointEncodedBytes]);
                   
/*before we call the OMIINO_AUX_ResponseWithPayload function we make sure that we going to read less chars then the size of the array*/
                        OMIINO_FRAMER_ASSERT((J1_Length+EndpointEncodedBytes <= 16+64+1),J1_Length+EndpointEncodedBytes);
                        

			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_EX, (char *)Payload, J1_Length+EndpointEncodedBytes);
		
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_EX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_RX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	U8 EndpointEncodedBytes;
	unsigned char Payload[16+64+1];
	U8 J1_Length;

   
    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GetRX(iDevice, &LineEndpoint, (char *)&Payload[EndpointEncodedBytes]);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{

                   J1_Length=(U8)strlen((const char *)&Payload[EndpointEncodedBytes]);
                   OMIINO_FRAMER_ASSERT((J1_Length+EndpointEncodedBytes <= 16+64+1),J1_Length+EndpointEncodedBytes);

                   OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_RX, (char *)Payload, J1_Length+EndpointEncodedBytes);
		
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GET_RX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}






void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	U8 EndpointEncodedBytes;
	unsigned char Payload[16+64+1];
	U8 J2_Length;

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GetTX(iDevice, &LineEndpoint, (char *)&Payload[EndpointEncodedBytes]);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			J2_Length=(U8)strlen((const char *)&Payload[EndpointEncodedBytes]);
            OMIINO_FRAMER_ASSERT((J2_Length+EndpointEncodedBytes <= 16+64+1),J2_Length+EndpointEncodedBytes);

			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_TX, (char *)Payload, J2_Length+EndpointEncodedBytes);

		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_TX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	U8 EndpointEncodedBytes;
	unsigned char Payload[16+64+1];
	U8 J2_Length;

    
    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GetEX(iDevice, &LineEndpoint, (char *)&Payload[EndpointEncodedBytes]);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			J2_Length=(U8)strlen((const char *)&Payload[EndpointEncodedBytes]);
            OMIINO_FRAMER_ASSERT((J2_Length+EndpointEncodedBytes <= 16+64+1),J2_Length+EndpointEncodedBytes);
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_EX, (char *)Payload, J2_Length+EndpointEncodedBytes);

		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_EX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_RX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;

	U8 EndpointEncodedBytes;
	unsigned char Payload[16+64+1];
	U8 J2_Length;

   
    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GetRX(iDevice, &LineEndpoint, (char *)&Payload[EndpointEncodedBytes]);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			J2_Length=(U8)strlen((const char *)&Payload[EndpointEncodedBytes]);
            OMIINO_FRAMER_ASSERT((J2_Length+EndpointEncodedBytes <= 16+64+1),J2_Length+EndpointEncodedBytes);
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_RX, (char *)Payload, J2_Length+EndpointEncodedBytes);

		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_J2_GET_RX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Mode=0;
	U8 EndpointEncodedBytes;
	U8 Payload[16];


    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		Mode=pMessage[iNextU8++];
    }

    if(Result)
    {		
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetMode(iDevice, &LineEndpoint, &Mode);
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
		Payload[EndpointEncodedBytes]=Mode;
        OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_J1_GETMODE, (char *)Payload, 1+EndpointEncodedBytes);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Mode) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Mode=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            Mode=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(TransactionIdentifier, iDevice, &LineEndpoint, Mode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    char    PathTrace[64+1];


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(Length>iNextU8)
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, PathTrace);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    char    PathTrace[64+1];


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(Length>iNextU8)
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, PathTrace);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_RX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;
    U8 C2=0;
	U8 Payload[16+1];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

	if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GetRX(iDevice, &LineEndpoint, &C2);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=C2;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_RX, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_RX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;
    U8 C2=0;
	U8 Payload[16+1];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

	if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GetTX(iDevice, &LineEndpoint, &C2);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=C2;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_TX, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_TX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;
    U8 C2=0;
	U8 Payload[16+1];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

	if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GetEX(iDevice, &LineEndpoint, &C2);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=C2;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_EX, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_C2_GET_EX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_C2) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 TX_C2=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            TX_C2=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, TX_C2);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_C2) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 EX_C2=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            EX_C2=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, EX_C2);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            Threshold=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            Threshold=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GetExcessiveThreshold(iDevice, &LineEndpoint, &Threshold);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=Threshold;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GetSignalDegradeThreshold(iDevice, &LineEndpoint, &Threshold);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=Threshold;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveThreshold(iDevice, &LineEndpoint, &Threshold);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=Threshold;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GetSignalDegradeThreshold(iDevice, &LineEndpoint, &Threshold);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=Threshold;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_GetSignalLabelTx(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 SignalLabel=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetTX(iDevice, &LineEndpoint, &SignalLabel);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=SignalLabel;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_TX, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_TX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_GetSignalLabelEx(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 SignalLabel=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;


    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetEX(iDevice, &LineEndpoint, &SignalLabel);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=SignalLabel;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_EX, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_EX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_GetSignalLabelRx(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 SignalLabel=0;
	U8 Payload[16+1];
	U8 EndpointEncodedBytes;


    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetRX(iDevice, &LineEndpoint, &SignalLabel);
        if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes]=SignalLabel;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_RX, (char *)Payload, 1+EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_LABEL_RX, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}








	





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    char    PathTrace[64+1];


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(Length>iNextU8)
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, PathTrace);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString) */


    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    char    PathTrace[64+1];


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(Length>iNextU8)
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, PathTrace);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_V5_SignalLabel) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 TX_V5_SignalLabel=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            TX_V5_SignalLabel=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, &LineEndpoint, TX_V5_SignalLabel);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_V5_SignalLabel) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 EX_V5_SignalLabel=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            EX_V5_SignalLabel=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, &LineEndpoint, EX_V5_SignalLabel);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_FAIL_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold) */
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            Threshold=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveThreshold(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;

    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8 Threshold=0;


    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            Threshold=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }
    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, &LineEndpoint, Threshold);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 Framing;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        Framing=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetFraming(TransactionIdentifier, iDevice, iSocketClientPDH, Framing);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}








void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
	U32 Gain;

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

		Gain=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableTimestamping(TransactionIdentifier, iDevice, iSocketClientPDH, Gain);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;


    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableTimestamping(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_SOCKET_CLIENT_PDH_GET_TIMESTAMPING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetTimeStamping(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_TIMESTAMP, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_TIMESTAMP, (char *)Payload, 4);
	}
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U32 TimestampByteCount;

    if(24!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

		TimestampByteCount=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetTimestampByteCount(TransactionIdentifier, iDevice, iSocketClientPDH, (unsigned char)TimestampByteCount);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Mapping) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 Mapping;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        Mapping=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(TransactionIdentifier, iDevice, iSocketClientPDH, Mapping);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 ClockRecoveryMode;
	U8 Payload[4+1];

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
		iNextU8+=4;

		UtilityPutU32(iSocketClientPDH, Payload);
		API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetClockRecoveryMode(iDevice, iSocketClientPDH, &ClockRecoveryMode);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[4]=ClockRecoveryMode;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY_MODE, (char *)Payload, 5);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY_MODE, (char *)Payload, 4);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecovery(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClockRecoveryMode) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 ClockRecoveryMode;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        ClockRecoveryMode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecoveryMode(TransactionIdentifier, iDevice, iSocketClientPDH, ClockRecoveryMode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

	UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetClientType(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE, (char *)Payload, 4);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_FRAMING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

	UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetFraming(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_FRAMING, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_FRAMING, (char *)Payload, 4);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

	UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetMapping(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_MAPPING, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_MAPPING, (char *)Payload, 4);
	}
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 Payload[4+4];
	U8 ByteResult;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

	UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetTimestampByteCount(iDevice, iSocketClientPDH, &ByteResult);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32((unsigned int)ByteResult, &Payload[4]);
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT, (char *)Payload, 8);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT, (char *)Payload, 4);
	}
}






void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetShallowSystemState(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_SHALLOW_SYSTEM_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_SHALLOW_SYSTEM_STATE, (char *)Payload, 4);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_DEEP_LINE_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetDeepLineState(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_DEEP_LINE_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_DEEP_LINE_STATE, (char *)Payload, 4);
	}
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GetTowardsSocketState(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_AIS_CONTROL_TOWARDS_SOCKET_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_AIS_CONTROL_TOWARDS_SOCKET_STATE, (char *)Payload, 4);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_LINE_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GetTowardsLineState(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_AIS_CONTROL_TOWARDS_LINE_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_AIS_CONTROL_TOWARDS_LINE_STATE, (char *)Payload, 4);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    UtilityPutU32(iSocketClientPDH, Payload);
    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_Get_CAS_State(iDevice, iSocketClientPDH, &Payload[4]);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_CAS_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_GET_CAS_STATE, (char *)Payload, 4);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableCAS(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableCAS(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsSocket(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsSocket(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsLine(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsLine(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U32 TransactionIdentifier;
    U32 RateParameter=0;
	U8 OutputSelector;
	U8  iNextU8=16;
	U8	ResponseMessage[5];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	OutputSelector=pMessage[iNextU8++];

    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_RateGet(iDevice, OutputSelector, &RateParameter);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		ResponseMessage[0]=OutputSelector;
		UtilityPutU32(RateParameter, &ResponseMessage[1]);
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_GET, (char *)ResponseMessage, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
	U8 OutputSelector;
	U8  iNextU8=16;
	U8	ResponseMessage[5];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);
	OutputSelector=pMessage[iNextU8++];

    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Get(iDevice, OutputSelector, &iSocketClientPDH);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		ResponseMessage[0]=OutputSelector;
		UtilityPutU32(iSocketClientPDH, &ResponseMessage[1]);
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET, (char *)ResponseMessage, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
	U32 RateParameter;
	U8 OutputSelector;


    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
		OutputSelector=pMessage[iNextU8++];

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

		RateParameter=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;
		

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Set(TransactionIdentifier, iDevice, OutputSelector, iSocketClientPDH, RateParameter);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_SHALLOW(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableShallowSystem(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_SHALLOW(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableShallowSystem(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_DEEP(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableDeepLine(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_DEEP(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;

    if(20!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableDeepLine(TransactionIdentifier, iDevice, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;


    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Get(iDevice, iSocketClientPDH, &PRBS_Pattern);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=PRBS_Pattern;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;


    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Get(iDevice, iSocketClientPDH, &PRBS_Pattern);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=PRBS_Pattern;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;


    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Get(iDevice, iSocketClientPDH, &PRBS_Pattern);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=PRBS_Pattern;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
    iNextU8+=4;


    API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Get(iDevice, iSocketClientPDH, &PRBS_Pattern);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=PRBS_Pattern;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}





void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        PRBS_Pattern=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        PRBS_Pattern=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        PRBS_Pattern=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 PRBS_Pattern;

    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        PRBS_Pattern=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(TransactionIdentifier, iDevice, iSocketClientPDH, PRBS_Pattern);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 State;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);
    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_PRBS_GetState(iDevice, iSocketClientPDH, &State);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=State;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
    
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 State;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);
    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetShallowSystemState(iDevice, iSocketClientPDH, &State);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=State;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_SHALLOW_SYSTEM_STATE_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U32 iSocketClientPDH=0;
    U8 State;
	U8 Payload[4+1];

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

    TransactionIdentifier=UtilityGetU32(&pMessage[4]);
    iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;

    API_Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_PRBS_GetState(iDevice, iSocketClientPDH, &State);
	if(WPX_UFE_FRAMER_OK==API_Result)
	{
		UtilityPutU32(iSocketClientPDH, Payload);
		Payload[4]=State;
		OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE, (char *)Payload, 5);
	}
	else
	{
		OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
	}
}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Framing) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U8 Framing;

    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        Framing=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(TransactionIdentifier, iDevice, iDiscreteClientPort, Framing);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Framing) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U8 Mapping;

    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        Mapping=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(TransactionIdentifier, iDevice, iDiscreteClientPort, Mapping);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClockRecoveryMode) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U8 ClockRecoveryMode;

    if(18!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        ClockRecoveryMode=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(TransactionIdentifier, iDevice, iDiscreteClientPort, ClockRecoveryMode);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;

    if(17!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(TransactionIdentifier, iDevice, iDiscreteClientPort);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint) */

    U8  API_Result;
    U8  Result=1;
    U8  BytesParsedThisField;
    U8  iNextU8=16;
    
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  FirstLineEndpoint;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  SecondLineEndpoint;

    if(26>Length)
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        
        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &FirstLineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &SecondLineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint) */

    U8  API_Result;
    U8  Result=1;
    U8  BytesParsedThisField;
    U8  iNextU8=16;
    
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  FirstLineEndpoint;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  SecondLineEndpoint;

    if(26>Length)
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);
        
        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &FirstLineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &SecondLineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, &FirstLineEndpoint, &SecondLineEndpoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  BytesParsedThisField;
    U8  iNextU8=16;
    
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;
    U32                             iSocketClientPDH=0;


    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
        if(4==(Length-iNextU8))
        {
            iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
            iNextU8+=4;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, &LineEndpoint, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  BytesParsedThisField;
    U8  iNextU8=16;
    
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;
    U32                             iSocketClientPDH=0;



    TransactionIdentifier=UtilityGetU32(&pMessage[4]);

    if(Length>iNextU8)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
        iNextU8+=BytesParsedThisField;
    }
    else
    {
        Result=0;
    }


    if(Result)
    {
        if(4==(Length-iNextU8))
        {
            iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
            iNextU8+=4;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, &LineEndpoint, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U32 iSocketClientPDH=0;


    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, iDiscreteClientPort, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U32 iSocketClientPDH=0;


    if(21!=Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];

        iSocketClientPDH=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, iDiscreteClientPort, iSocketClientPDH);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;
	U8 IsEnabled;
	U8 Payload[2];

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GetAlarmReportingForAlarmCategory(AlarmCategory, &IsEnabled);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[0]=AlarmCategory;
			Payload[1]=IsEnabled;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY, (char *)Payload, 2);
		}
		else
		{
			OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;
	U8 IsEnabled;
	U8 Payload[2];

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GetAlarmReportingForAlarmCategory(AlarmCategory, &IsEnabled);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[0]=AlarmCategory;
			Payload[1]=IsEnabled;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY, (char *)Payload, 2);
		}
		else
		{
			OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_DRIVER_CLIENT_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_DRIVER_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}








void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_DEVICE_DRIVER_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8  AlarmCategory;
	U8 IsEnabled;
	U8 Payload[2];

    if(17!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        AlarmCategory=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GetAlarmReportingForAlarmCategory(AlarmCategory, &IsEnabled);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[0]=AlarmCategory;
			Payload[1]=IsEnabled;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY, (char *)Payload, 2);
		}
		else
		{
			OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}










void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_PORT_GET_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8 API_Result;
	U8 iLinePort=0;
	U8 iPoint=0;
	U8 State;
	char Payload[3];

	U32 TransactionIdentifier=UtilityGetU32(&pMessage[4]);

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	iLinePort=pMessage[16];
	iPoint=pMessage[17];
	API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_GetMonitoringState(iDevice, iLinePort, iPoint, (unsigned char *)&State);

	Payload[0]=iLinePort;
	Payload[1]=iPoint;
	Payload[2]=State;

	OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_PORT_GET, (char *)Payload, 3);
}






void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_PORT_ENABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 iPoint=0;

    if(18!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        iPoint=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iDevice, iLinePort, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_PORT_DISABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iLinePort;
    U8 iPoint=0;

    if(18!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iLinePort=pMessage[iNextU8++];
        iPoint=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iDevice, iLinePort, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}











void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_HIGH_ORDER_PATH_GET_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;
    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;
	U8 iPoint=0;
	U8 State;
	U8 Payload[16+1];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            iPoint=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }

	if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_GetMonitoringState(iDevice, &LineEndpoint, iPoint, &State);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes++]=iPoint;
			Payload[EndpointEncodedBytes++]=State;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_HO_GET, (char *)Payload, EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_HO_GET, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}




void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;
    U8                              iPoint=0;

    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            iPoint=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNAURE: OMIINO_API_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;



    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;
    U8                              iPoint=0;

    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            iPoint=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}



void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_LOW_ORDER_PATH_GET_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;
    U32 TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
	U8 EndpointEncodedBytes;
	U8 iPoint=0;
	U8 State;
	U8 Payload[16+1];

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            iPoint=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }

	if(Result)
    {
		EndpointEncodedBytes=OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(&LineEndpoint, Payload);
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_GetMonitoringState(iDevice, &LineEndpoint, iPoint, &State);
		if(WPX_UFE_FRAMER_OK==API_Result)
		{
			Payload[EndpointEncodedBytes++]=iPoint;
			Payload[EndpointEncodedBytes++]=State;
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_LO_GET, (char *)Payload, EndpointEncodedBytes);
		}
		else
		{
			OMIINO_AUX_ResponseWithPayload(iDevice, TransactionIdentifier, API_Result, AUXILIARY_RESPONSE_PM_LO_GET, (char *)Payload, EndpointEncodedBytes);
		}
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;
    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;
    U8                              iPoint=0;

    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            iPoint=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;



    U32                             TransactionIdentifier=0;
    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  LineEndpoint;
    U8                              iPoint=0;

    if(22>Length) 
    {
        Result=0; 
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        if(Length>iNextU8)
        {
            Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &BytesParsedThisField);
            iNextU8+=BytesParsedThisField;
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(1==(Length-iNextU8))
        {
            iPoint=pMessage[iNextU8++];
        }
        else
        {
            Result=0;
        }
    }

    if(Result)
    {
        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, &LineEndpoint, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_DISCRETE_PORT_ENABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U8 iPoint=0;

    if(18!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];
        iPoint=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring(TransactionIdentifier, iDevice, iDiscreteClientPort, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }

}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_DISCRETE_PORT_DISABLE_MONITORING(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    /* SIGNATURE: OMIINO_API_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint) */

    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
    
    U32 TransactionIdentifier;
    U8 iDiscreteClientPort;
    U8 iPoint=0;

    if(18!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        TransactionIdentifier=UtilityGetU32(&pMessage[4]);

        iDiscreteClientPort=pMessage[iNextU8++];
        iPoint=pMessage[iNextU8++];

        API_Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring(TransactionIdentifier, iDevice, iDiscreteClientPort, iPoint);
        OMIINO_AUX_Response(iDevice,TransactionIdentifier,API_Result);
    }
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BOOTLOADER_HELLO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
	U32 Count;
    
    if((iNextU8+4)!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        Count=UtilityGetU32(&pMessage[iNextU8]);

		API_Result=OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Bootloader_Hello(iDevice,Count);

		OMIINO_MODEL_CONTROL_FSM_Announce_BootloaderHelloFor(iDevice);

	}
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BOOTLOADER_ACK(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
    U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
	U32 Count;
    
    if(20!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        Count=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

		API_Result=OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Ack(iDevice,Count);
	}
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BOOTLOADER_NAK(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
	U32 Count;
    
    if(20!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        Count=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

		API_Result=OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Nak(iDevice);
	}
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}

void OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_FIRMWARE_HELLO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	U8  API_Result;
    U8  Result=1;
    U8  iNextU8=16;
	U32 Count;
	U32 HardwareVariant;
	OMIINO_FRAMER_STATUS_DEVICE_TYPE * pDeviceStatusRAM;
    
    if(24!=Length) 
    {
        Result=0;
    }

    if(Result)
    {
        Count=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;
		HardwareVariant=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

		pDeviceStatusRAM = OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareVariant(pDeviceStatusRAM, (U8)HardwareVariant);
		
		

		API_Result=OMIINO_FRAMER_FIRMWARE_DOWNLOAD_Process_Firmware_Hello(iDevice,Count);

		OMIINO_MODEL_CONTROL_FSM_Announce_FirmwareHelloFor(iDevice);
	}
    else
    {
        OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(MessageIdentifier, iDevice, pMessage, Length, __LINE__, __FILE__);
    }
}



void Get_API_NoLongerSupported(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	OMIINO_AUX_Response(iDevice,UtilityGetU32(&pMessage[4]),WPX_UFE_FRAMER_ERROR_API_NO_LONGER_SUPPORTED);
}


void Get_Stub_TODO(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length)
{
	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	OMIINO_AUX_Response(iDevice,UtilityGetU32(&pMessage[4]),WPX_UFE_FRAMER_ERROR_NOT_IMPLEMENTED);
}



void Milestone_1_Bindings(OMNISPY_AUXILIARY_TABLE_TYPE * pAuxiliaryTable)
{
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_SET_BUILD_PERSONALITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_SET_BUILD_PERSONALITY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_GET_FRAME_ALIGNMENT_OFFSET]=Get_API_NoLongerSupported;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_SCRAMBLING_STATE]=Get_API_NoLongerSupported;
    pAuxiliaryTable->Parser[API_DEFAULT_RAM_IGNORE]=OMIINO_LLC_Northbound_AuxiliaryParser_IGNORE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_SET_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_SET_MODE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_SET_FRAME_ALIGNMENT_OFFSET]=Get_API_NoLongerSupported;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_ENABLE_SCRAMBLING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_ENABLE_SCRAMBLING;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DISABLE_SCRAMBLING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DISABLE_SCRAMBLING;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP_SYSTEM]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP_SYSTEM]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_FAIL_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SET_TX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SET_TX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_FAIL_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_FAIL_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_FAIL_THRESHOLD;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_SET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_SHALLOW_SYSTEM]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_SHALLOW;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_SHALLOW_SYSTEM]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_SHALLOW;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_DEEP_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_ENABLE_DEEP;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_DEEP_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_DISABLE_DEEP;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION;
    pAuxiliaryTable->Parser[API_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION;
    pAuxiliaryTable->Parser[API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION;
    pAuxiliaryTable->Parser[API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION;
    pAuxiliaryTable->Parser[API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION;
    pAuxiliaryTable->Parser[API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY;

	pAuxiliaryTable->Parser[API_PM_SONET_SDH_PORT_ENABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_PORT_ENABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_PORT_DISABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_PORT_DISABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_PORT_GET_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_PORT_GET_MONITORING;

	pAuxiliaryTable->Parser[API_PM_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_HIGH_ORDER_PATH_GET_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_HIGH_ORDER_PATH_GET_MONITORING;

	pAuxiliaryTable->Parser[API_PM_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_LOW_ORDER_PATH_GET_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_LOW_ORDER_PATH_GET_MONITORING;

	pAuxiliaryTable->Parser[API_PM_SONET_SDH_DISCRETE_PORT_ENABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_DISCRETE_PORT_ENABLE_MONITORING;
	pAuxiliaryTable->Parser[API_PM_SONET_SDH_DISCRETE_PORT_DISABLE_MONITORING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_PM_SONET_SDH_DISCRETE_PORT_DISABLE_MONITORING;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE;
    pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GET_STATE;
	pAuxiliaryTable->Parser[API_BOOTLOADER_HELLO]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BOOTLOADER_HELLO;
	pAuxiliaryTable->Parser[API_BOOTLOADER_ACK]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BOOTLOADER_ACK;
	pAuxiliaryTable->Parser[API_BOOTLOADER_NAK]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BOOTLOADER_NAK;
	pAuxiliaryTable->Parser[API_FIRMWARE_HELLO]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_FIRMWARE_HELLO;
	pAuxiliaryTable->Parser[API_DEVICE_DRIVER_RESET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DRIVER_RESET;
	pAuxiliaryTable->Parser[API_STATUS_DEVICE_DRIVER_VERSION_GET_INFORMATION]=OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_DRIVER_VERSION_GET_INFORMATION;
	pAuxiliaryTable->Parser[API_STATUS_DEVICE_VERSION_GET_FIRMWARE_INFORMATION]=OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_VERSION_GET_FIRMWARE_INFORMATION;
	pAuxiliaryTable->Parser[API_STATUS_DEVICE_VERSION_GET_HARDWARE_INFORMATION]=OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_VERSION_GET_HARDWARE_INFORMATION;
	pAuxiliaryTable->Parser[API_STATUS_DEVICE_BUILD_GET_HARDWARE_VARIANT]=OMIINO_LLC_Northbound_Auxiliary_API_STATUS_DEVICE_BUILD_GET_HARDWARE_VARIANT;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_GET_MODE]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_DEVICE_GET_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_RATE]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_RATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_SHALLOW_LINE_STATE]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_SHALLOW_LINE_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_DEEP_SYSTEM_STATE]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GET_DEEP_SYSTEM_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_MODE]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_TX]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_TX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_EX]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_EX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_TX]=OMIINO_LLC_Northbound_Auxiliary_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_TX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_TX]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_TX;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_RX]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GET_RX;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_RX]=OMIINO_LLC_Northbound_Auxiliary_API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GET_RX;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_RX]=OMIINO_LLC_Northbound_Auxiliary_API_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GET_RX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_TX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_EX;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GET_RX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GET_RX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_TX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_EX;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GET_RX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GET_RX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_TX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_EX;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GET_RX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GET_RX;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_FAIL_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GET_SIGNAL_DEGRADE_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_FAIL_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B2_GET_SIGNAL_DEGRADE_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_FAIL_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GET_SIGNAL_DEGRADE_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_FAIL_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GET_SIGNAL_DEGRADE_THRESHOLD;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_TX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_GetSignalLabelTx;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_EX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_GetSignalLabelEx;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GET_RX]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_GetSignalLabelRx;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_BANDWIDTH_TYPE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_FRAMING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_FRAMING;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_MAPPING;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_DEEP_LINE_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_DEEP_LINE_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_LINE_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_SOCKET_CLIENT_PDH_AIS_CONTROL_GET_TOWARDS_LINE_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_GET_BUILD_PERSONALITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_GET_BUILD_PERSONALITY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CLOCK_RECOVERY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_GET;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_FORCE_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GET_FORCE_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GET_STATE;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GET_STATE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOOPBACK_GET_SHALLOW_SYSTEM;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMP_BYTE_COUNT;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT;

	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY;
	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_CREATE_FACILITY;
	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY;
	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_FACILITIES_SOCKET_CLIENT_DELETE_FACILITY;

	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION;
	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION;
	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_CREATE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CREATE_CONNECTION;
	pAuxiliaryTable->Parser[API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CLIENT_DELETE_CONNECTION]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DELETE_CONNECTION;

	pAuxiliaryTable->Parser[API_BULK_PM_ENABLE_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_ENABLE_LINE; 
	pAuxiliaryTable->Parser[API_BULK_PM_DISABLE_LINE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_DISABLE_LINE;
	pAuxiliaryTable->Parser[API_BULK_PM_ENABLE_HO]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_ENABLE_HO;   
	pAuxiliaryTable->Parser[API_BULK_PM_DISABLE_HO]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_DISABLE_HO;  
	pAuxiliaryTable->Parser[API_BULK_PM_ENABLE_LO]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_ENABLE_LO;   
	pAuxiliaryTable->Parser[API_BULK_PM_DISABLE_LO]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_PM_DISABLE_LO;  

	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING; 
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_TIMESTAMPING]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_SOCKET_CLIENT_PDH_GET_TIMESTAMPING;


	pAuxiliaryTable->Parser[API_CONFIGURATION_PM_E1_ENABLE_MONITORING_CRC] = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_E1_ENABLE_MONITORING_CRC   ;  
	pAuxiliaryTable->Parser[API_CONFIGURATION_PM_E1_DISABLE_MONITORING_CRC]= OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_E1_DISABLE_MONITORING_CRC  ; 
	pAuxiliaryTable->Parser[API_CONFIGURATION_PM_E1_GET_MONITORING_CRC]= OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_E1_GET_MONITORING_CRC  ; 

	pAuxiliaryTable->Parser[API_CONFIGURATION_PM_T1_ENABLE_MONITORING_CRC] = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_T1_ENABLE_MONITORING_CRC   ;  
	pAuxiliaryTable->Parser[API_CONFIGURATION_PM_T1_DISABLE_MONITORING_CRC]= OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_T1_DISABLE_MONITORING_CRC  ;
	pAuxiliaryTable->Parser[API_CONFIGURATION_PM_T1_GET_MONITORING_CRC]= OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_PM_T1_GET_MONITORING_CRC  ;

	pAuxiliaryTable->Parser[API_BULK_ALARM_ENABLE_LINE] = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_LINE; 
	pAuxiliaryTable->Parser[API_BULK_ALARM_DISABLE_LINE]= OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_LINE; 
	pAuxiliaryTable->Parser[API_BULK_ALARM_ENABLE_HO]   = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_HO;    
	pAuxiliaryTable->Parser[API_BULK_ALARM_DISABLE_HO]  = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_HO;   
	pAuxiliaryTable->Parser[API_BULK_ALARM_ENABLE_LO]   = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_LO;    
	pAuxiliaryTable->Parser[API_BULK_ALARM_DISABLE_LO]  = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_LO;   
	pAuxiliaryTable->Parser[API_BULK_ALARM_ENABLE_ALL]  = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_ENABLE_ALL;   
	pAuxiliaryTable->Parser[API_BULK_ALARM_DISABLE_ALL] = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_DISABLE_ALL;
	pAuxiliaryTable->Parser[API_BULK_ALARM_LIST_ALL_ACTIVE] = OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_ALARM_LIST_ALL_ACTIVE;

	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY;
	pAuxiliaryTable->Parser[API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GET_ALARM_REPORTING_FOR_ALARM_CATEGORY;


	pAuxiliaryTable->Parser[API_CONFIGURATION_SET_DEVICE_PROTECTION_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_SET_DEVICE_PROTECTION_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_GET_DEVICE_PROTECTION_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_GET_DEVICE_PROTECTION_MODE;

	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_GET]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK_RATE_GET;

	pAuxiliaryTable->Parser[API_BULK_SOCKET_INGRESS_SET_PRBS_TERM]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_INGRESS_SET_PRBS_TERM;
	pAuxiliaryTable->Parser[API_BULK_SOCKET_INGRESS_SET_PRBS_GEN] =OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_INGRESS_SET_PRBS_GEN;
	pAuxiliaryTable->Parser[API_BULK_SOCKET_EGRESS_SET_PRBS_TERM] =OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_EGRESS_SET_PRBS_TERM;
	pAuxiliaryTable->Parser[API_BULK_SOCKET_EGRESS_SET_PRBS_GEN]  =OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_BULK_SOCKET_EGRESS_SET_PRBS_GEN;

	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET]=OMIINO_LLC_Northbound_Auxiliary_API_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET;

	pAuxiliaryTable->Parser[API_CONFIGURATION_SET_DEVICE_CARD_PROTECTION_OPERATION_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_SET_DEVICE_CARD_PROTECTION_OPERATING_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_GET_DEVICE_CARD_PROTECTION_OPERATION_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_GET_DEVICE_CARD_PROTECTION_OPERATING_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_SET_DEVICE_AIS_INSERTION_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_SET_AIS_INSERTION_MODE;
	pAuxiliaryTable->Parser[API_CONFIGURATION_GET_DEVICE_AIS_INSERTION_MODE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_CONFIGURATION_DEVICE_GET_AIS_INSERTION_MODE;

	pAuxiliaryTable->Parser[API_DEVICE_DIAGNOSTIC_POKE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_POKE;
	pAuxiliaryTable->Parser[API_DEVICE_DIAGNOSTIC_PEEK]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_PEEK;
	pAuxiliaryTable->Parser[API_DEVICE_DIAGNOSTIC_BLOCK_POKE]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_BLOCK_POKE;
	pAuxiliaryTable->Parser[API_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY]=OMIINO_LLC_Northbound_AuxiliaryOmniSpyParser_API_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY;
}




void Milestone_2_Bindings(OMNISPY_AUXILIARY_TABLE_TYPE * pAuxiliaryTable)
{
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_BANDWIDTH_TYPE]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_FRAMING]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_MAPPING]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CLOCK_RECOVERY]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CAS_STATE]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_SOCKET_STATE]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GET_TOWARDS_LINE_STATE]=Get_Stub_TODO;
	pAuxiliaryTable->Parser[API_STATUS_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_PRBS_GET_STATE]=Get_Stub_TODO;
}


void OMIINO_LLC_Northbound_AuxiliaryOmniSpyTableInitialize(OMNISPY_AUXILIARY_TABLE_TYPE * pAuxiliaryTable)
{
    int i;

    for(i=0;i<MAX_APIS;i++)
    {
        pAuxiliaryTable->Parser[i]=OMIINO_LLC_Northbound_AuxiliaryParser_NULL;
    }

	Milestone_1_Bindings(pAuxiliaryTable);
	Milestone_2_Bindings(pAuxiliaryTable);
}

