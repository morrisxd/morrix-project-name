/*--------------------------------------------------------------------------*/
/*                                                                          */
/*        Copyright (c, U32 * pFormattedMessage) 2010  Omiino Ltd                                    */
/*                                                                          */
/*        All rights reserved.                                              */
/*        This code is provided under license and or Non-disclosure         */
/*        Agreement and must be used solely for the purpose for which it    */
/*        was provided. It must not be passed to any third party without    */
/*        the written permission of Omiino Ltd.                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/



#include "WO_FRMR_private.h"


void OMIINO_LLC_SOUTHBOUND_FORMATTER_SetField_HeaderLength(U32 *pHeader, U32 Value)
{
    U8 *pHeaderLengthField=(U8 *)&pHeader[2];

    UtilityPutU32(Value, pHeaderLengthField);
}

U32 OMIINO_LLC_SOUTHBOUND_FORMATTER_GetHeaderLength(void)
{
    return 16; /* MessageID, TID, PayloadLength, Attributes */
}


U32 OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthCalculatePayloadLength(U32 OverallLength)
{
    return OverallLength-OMIINO_LLC_SOUTHBOUND_FORMATTER_GetHeaderLength();
}


void OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(U32 *pHeader, U8 OverallLength)
{
    U32 HeaderPayloadLengthField=OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthCalculatePayloadLength(OverallLength);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_SetField_HeaderLength(pHeader, HeaderPayloadLengthField);
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(U8 * pFormattedMessage, char * pPathTrace)
{
    memcpy(pFormattedMessage,pPathTrace,64);
    return 64;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(U32 * pFormattedMessage, U32 Field_SouthboundMessageID, U32 Field_TransactionIdentifier, U32 Field_PayloadLength, U32 Field_Attributes)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    UtilityPutU32(Field_SouthboundMessageID, piAddAt);
    piAddAt+=4;
    Result+=4;

    UtilityPutU32(Field_TransactionIdentifier, piAddAt);
    piAddAt+=4;
    Result+=4;

    UtilityPutU32(Field_PayloadLength, piAddAt);
    piAddAt+=4;
    Result+=4;

    UtilityPutU32(Field_Attributes, piAddAt);
    piAddAt+=4;
    Result+=4;

    return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(U8 * pFormattedMessage, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pAny_SONET_SDH_ENDPOINT_TYPE)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->iLinePort;
    piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->Bandwidth;

	if (OMIINO_FRAMER_Is_SDH_Endpoint(pAny_SONET_SDH_ENDPOINT_TYPE))
	{
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sdh.iJ;
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sdh.iK;
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sdh.iL;
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sdh.iM;
	}
	else
	{
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sonet.iU;
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sonet.iV;
		piAddAt[Result++]=pAny_SONET_SDH_ENDPOINT_TYPE->u.sonet.iW;
	}
    return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_AUX_RESP_WithPayload(U32 TransactionIdentifier, U8 API_Result, U32 CatalogueID, char *pUnFormattedMessage, U8 PayloadLength, U32 * pFormattedMessage)
{
	int i;
	U8 Result=0;
    U8 *piAddAt=(U8 *)&pFormattedMessage[1];

	UtilityPutU32(0x00000002, (unsigned char *)pFormattedMessage);

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(&pFormattedMessage[1], 0x02, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=API_Result;
    Result+=1;
	piAddAt+=1;

	UtilityPutU32(CatalogueID, (unsigned char *)piAddAt);
	piAddAt+=4;
    Result+=4;

	for(i=0;i<PayloadLength;i++)
	{
		*piAddAt=pUnFormattedMessage[i];
		piAddAt+=1;
		Result+=1;
	}

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(&pFormattedMessage[1], Result);

	return Result+4;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_AUX_RESP(U32 TransactionIdentifier, U8 API_Result, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)&pFormattedMessage[1];


    UtilityPutU32(0x00000002, (unsigned char *)pFormattedMessage);

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(&pFormattedMessage[1], 0x01, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=API_Result;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(&pFormattedMessage[1], Result);

	return Result+4;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PROGRESS_REPORT(U32 CatalogueID, U32 Iteration, U32 TotalIterations, char * pMessage, U8 MessageLength, U32 * pFormattedMessage)
{
    int i;
	U8 Result=0;
    U8 *piAddAt=(U8 *)&pFormattedMessage[1];

	UtilityPutU32(0x00000002, (unsigned char *)pFormattedMessage);

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(&pFormattedMessage[1], 0x02, 0xFFFFFFFF, 0, 0);
    piAddAt+=Result;

    *piAddAt=0; /* Reserved for Result if we want to add it */
    Result+=1;
	piAddAt+=1;

	UtilityPutU32(CatalogueID, (unsigned char *)piAddAt);
	piAddAt+=4;
    Result+=4;

	UtilityPutU32(1+Iteration, (unsigned char *)piAddAt);
	piAddAt+=4;
    Result+=4;

	UtilityPutU32(TotalIterations, (unsigned char *)piAddAt);
	piAddAt+=4;
    Result+=4;

	for(i=0;i<MessageLength;i++)
	{
		*piAddAt=pMessage[i];
		piAddAt+=1;
		Result+=1;
	}

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(&pFormattedMessage[1], Result);

	return Result+4;
}




/*
 *
 * Section: CONFIGURATION DEVICE
 *
 */



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetBuildPersonality(U32 TransactionIdentifier, U8 BuildPersonality, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_SET_BUILD_VARIANT, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=BuildPersonality;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetMode(U32 TransactionIdentifier, U8 DeviceMode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_SET_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=DeviceMode;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}





U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_Set_AIS_Insertion(U32 TransactionIdentifier, U8 AIS_InsertionMode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_SET_AIS_INSERTION_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AIS_InsertionMode;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetProtectionMode(U32 TransactionIdentifier, U8 DeviceProtectionMode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_SET_PROTECTION_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=DeviceProtectionMode;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetCardProtectionOperatingMode(U32 TransactionIdentifier, U8 DeviceCardProtectionOperatingMode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_SET_CARD_PROTECTION_OPERATING_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=DeviceCardProtectionOperatingMode;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


void OMIINO_LLC_SOUTHBOUND_FORMATTER_DEMO_ONLY_CONFIGURE_GTP_CLOCK_SOURCE_MESSAGE(U8 iDevice)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted) /* Support for first device only */
	{	
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_DEMO_ONLY_CONFIGURE_GTP_CLOCK_SOURCE, 0, 0, 0);
		piAddAt+=16;
		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}



void OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_Poke(U8 iDevice, U32 TransactionIdentifier, U32 Address, U32 Value,  U32 * pFormattedMessage)
{
    U8 *			piAddAt=(U8 *)pFormattedMessage;
	U8				Length=0;

	UtilityPutU32(0x00000001, piAddAt); /* Padding */
	Length=4;
	piAddAt+=4;

	UtilityPutU32(MESSAGE_ID_OMNISPY_SOUTHBOUND_DEVICE_DIAGNOSTIC_POKE, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(TransactionIdentifier, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(8, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(0, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(Address, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(Value, piAddAt);
	piAddAt+=4;
	Length+=4;

	OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, pFormattedMessage, Length);
}



void OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_BlockPoke(U8 iDevice, U32 TransactionIdentifier, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten,	U32 Value,  U32 * pFormattedMessage)
{
    U8 *			piAddAt=(U8 *)pFormattedMessage;
	U8				Length=0;

	UtilityPutU32(0x00000001, piAddAt); /* Padding */
	Length=4;
	piAddAt+=4;

	UtilityPutU32(MESSAGE_ID_OMNISPY_SOUTHBOUND_DEVICE_DIAGNOSTIC_BLOCK_POKE, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(TransactionIdentifier, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(8, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(0, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(StartAddress, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(Value, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(NumberOfU32AddressesToBeWritten, piAddAt);
	piAddAt+=4;
	Length+=4;

	OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, pFormattedMessage, Length);
}



void OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_BlockPokeVerify(U8 iDevice, U32 TransactionIdentifier, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten,	U32 Value,  U32 * pFormattedMessage)
{
    U8 *			piAddAt=(U8 *)pFormattedMessage;
	U8				Length=0;

	UtilityPutU32(0x00000001, piAddAt); /* Padding */
	Length=4;
	piAddAt+=4;

	UtilityPutU32(MESSAGE_ID_OMNISPY_SOUTHBOUND_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(TransactionIdentifier, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(8, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(0, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(StartAddress, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(Value, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(NumberOfU32AddressesToBeWritten, piAddAt);
	piAddAt+=4;
	Length+=4;

	OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, pFormattedMessage, Length);
}



void OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_Peek(U8 iDevice, U32 TransactionIdentifier, U32 Address,  U32 * pFormattedMessage)
{
    U8 *			piAddAt=(U8 *)pFormattedMessage;
	U8				Length=0;

	UtilityPutU32(0x00000001, piAddAt); /* Padding */
	Length=4;
	piAddAt+=4;

	UtilityPutU32(MESSAGE_ID_OMNISPY_SOUTHBOUND_DEVICE_DIAGNOSTIC_PEEK, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(TransactionIdentifier, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(4, piAddAt);
	piAddAt+=4;
	Length+=4;
	UtilityPutU32(0, piAddAt);
	piAddAt+=4;
	Length+=4;

	UtilityPutU32(Address, piAddAt);
	piAddAt+=4;
	Length+=4;

	OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, pFormattedMessage, Length);
}






/*
 *
 * Section: Device Driver Info towards Firmware
 *
 */
		
	
U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DRIVER_ProductRelease(U32 TransactionIdentifier, char * pAnyStr, U8 AnyStrLength, U32 * pFormattedMessage)
{
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DRIVER_PRODUCT_RELEASE, TransactionIdentifier, AnyStrLength, 0);
    piAddAt+=Result;
	memcpy(piAddAt, pAnyStr, AnyStrLength); 
	Result+=AnyStrLength;

    return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DRIVER_SoftwareBuildDate(U32 TransactionIdentifier, char * pAnyStr, U8 AnyStrLength, U32 * pFormattedMessage)
{
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DRIVER_SOFTWARE_BUILD_DATE, TransactionIdentifier, AnyStrLength, 0);
    piAddAt+=Result;
	memcpy(piAddAt, pAnyStr, AnyStrLength); 
	Result+=AnyStrLength;

    return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DRIVER_SoftwareBuildVersion(U32 TransactionIdentifier, char * pAnyStr, U8 AnyStrLength, U32 * pFormattedMessage)
{
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DRIVER_SOFTWARE_BUILD_VERSION, TransactionIdentifier, AnyStrLength, 0);
    piAddAt+=Result;
	memcpy(piAddAt, pAnyStr, AnyStrLength); 
	Result+=AnyStrLength;

    return Result;
}


/*
 *
 * Section: CONFIGURATION FACILITIES
 *
 */


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_SONET_SDH_PortAlarmKey(U32 TransactionIdentifier, U8 AlarmCategory, U32 Key, U8 iLinePort, U32 * pFormattedMessage)
{
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PORT_ALARM_KEY_ADD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

	*piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

	*piAddAt=AlarmCategory;
    piAddAt+=1;
    Result+=1;

	UtilityPutU32(Key, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	
    return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_SONET_SDH_PathAlarmKey(U32 TransactionIdentifier, U8 AlarmCategory, U32 Key, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 * pFormattedMessage)
{
	U8 Result=0;
    U8 BytesEncoded=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PATH_ALARM_KEY_ADD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

	*piAddAt=AlarmCategory;
    piAddAt+=1;
    Result+=1;

	UtilityPutU32(Key, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	
    return Result;
}







U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 BytesEncoded=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_CREATE_FACILITY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	
    return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_SocketClientAlarmKey(U32 TransactionIdentifier, U8 AlarmCategory, U32 iSocketClientPDH, U32 AlarmKey, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_SOCKET_CLIENT_ALARM_KEY_ADD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
    Result+=1;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    UtilityPutU32(AlarmKey, piAddAt);
    piAddAt+=4;
    Result+=4;


    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 ClientType, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CREATE_FACILITY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=ClientType;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(U32 TransactionIdentifier, U8 iDiscreteClientPort, U8 ClientType, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATE_FACILITY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=ClientType;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 BytesEncoded=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_DELETE_FACILITY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 ClientType, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DELETE_FACILITY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

	*piAddAt=ClientType;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);


	return Result;
}







U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETE_FACILITY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT
 *
 */

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState(U32 TransactionIdentifier, U8 iLinePort,U8 Tx_State, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SET_STATE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Tx_State;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iLinePort, U8 PortRate,U8 Tx_Enable, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SET_RATE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=PortRate;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Tx_Enable;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Enable(U32 TransactionIdentifier, U8 iLinePort, U8 DCC_Mode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=DCC_Mode;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Disable(U32 TransactionIdentifier, U8 iLinePort, U8 DCC_Mode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=DCC_Mode;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_EnableScrambling(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_ENABLE_SCRAMBLING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DisableScrambling(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DISABLE_SCRAMBLING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}








U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_A(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_B(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_RESET(U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_RESET, 0, 0, 0);
    piAddAt+=Result;

	return Result;
}







U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Enable(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 BytesEncoded=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

	BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Disable(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 BytesEncoded=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

	BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableShallow(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallow(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DriveRecoveredClock_Set(U32 TransactionIdentifier, U8 iLinePort, U8 OutputSelector, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=OutputSelector;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SquelchRecoveredClock_Set(U32 TransactionIdentifier, U8 Squelch_Mode, U8 OutputSelector, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SQUELCH_RECOVERED_CLOCK, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=Squelch_Mode;
    piAddAt+=1;
    Result+=1;

    *piAddAt=OutputSelector;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableDeep(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableDeep(U32 TransactionIdentifier, U8 iLinePort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}





/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT SECTION
 *
 */




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(U32 TransactionIdentifier, U8 iLinePort, U8 Mode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Mode;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}






U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetTX(U32 TransactionIdentifier, U8 iLinePort, char * pTX_TraceString, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(piAddAt, pTX_TraceString);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(U32 TransactionIdentifier, U8 iLinePort, char * pEX_TraceString, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SET_EX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(piAddAt, pEX_TraceString);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iLinePort, U8 Threshold, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_EXCESSIVE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iLinePort, U8 Threshold, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SET_SIGNAL_DEGRADE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT LINE
 *
 */




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(U32 TransactionIdentifier, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_SONET_SDH_PORT_K1K2_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=pTX_K1K2->K1;
    piAddAt+=1;
    Result+=1;

    *piAddAt=pTX_K1K2->K2;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(U32 TransactionIdentifier, U8 iLinePort, U8 TX_S1, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_SONET_SDH_PORT_S1_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=TX_S1;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iLinePort, U8 Threshold, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_EXCESSIVE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iLinePort, U8 Threshold, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SET_SIGNAL_DEGRADE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH HO PATH
 *
 */



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Mode, U32 * pFormattedMessage)
{
    U8 Result=0;
	U8 BytesEncoded=0;
	 U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=Mode;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(piAddAt, pTX_TraceString);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SET_EX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(piAddAt, pEX_TraceString);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SetRdiMode(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 ERDI_MODE, U32 * pFormattedMessage)
{
	U8 BytesEncoded=0;
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_RDI_SET_MODE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=ERDI_MODE;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);


	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SetTX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Value, U32 * pFormattedMessage)
{
	U8 BytesEncoded=0;
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=Value;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);


	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GetRX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 * pFormattedMessage)
{
	U8 BytesEncoded=0;
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);


	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_C2, U32 * pFormattedMessage)
{
	U8 BytesEncoded=0;
	U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=TX_C2;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);


	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 EX_C2, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SET_EX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=EX_C2;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_EXCESSIVE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SET_SIGNAL_DEGRADE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}






/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH LO PATH
 *
 */


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(piAddAt, pTX_TraceString);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SET_EX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeGenericPathTrace(piAddAt, pEX_TraceString);

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_V5_SignalLabel, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_TX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=TX_V5_SignalLabel;
    piAddAt+=1;
    Result+=1;


    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 EX_V5_SignalLabel, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SET_EX, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=EX_V5_SignalLabel;
    piAddAt+=1;
    Result+=1;


    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveThreshold(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_EXCESSIVE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;


    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetSignalDegradeThreshold(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SET_SIGNAL_DEGRADE_THRESHOLD, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=Threshold;
    piAddAt+=1;
    Result+=1;


    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}












/*
 *
 * Section: CONFIGURATION FACILITIES SOCKET CLIENT PDH
 *
 */






U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetFraming(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 Framing, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_FRAMING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=Framing;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableTimestamping(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 Gain, U8 ClientType, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_TIMESTAMPING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

	UtilityPutU32(Gain, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=ClientType;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableTimestamping(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 ClientType, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_TIMESTAMPING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

	UtilityPutU32(ClientType, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetTimestampByteCount(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 AnyTimestampByteCount, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_TIMESTAMP_BYTE_COUNT, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=AnyTimestampByteCount;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 Mapping, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_MAPPING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=Mapping;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecovery(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 ClockRecoveryMode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SET_CLOCK_RECOVERY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=ClockRecoveryMode;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SetFEAC(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 Code, U32 * pFormattedMessage)
{
   U8 Result=0;
   U8 *piAddAt=(U8 *)pFormattedMessage;
   
   Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SET_FEAC, TransactionIdentifier, 0, 0);
   piAddAt+=Result;
   
   UtilityPutU32(iSocketClientPDH, piAddAt);
   piAddAt+=4;
   Result+=4;
   
   *piAddAt=Code;
   piAddAt+=1;
   Result+=1;
   
   OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
   
   return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableCAS(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLE_CAS, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableCAS(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLE_CAS, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_SOCKET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_SOCKET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLE_TOWARDS_LINE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLE_TOWARDS_LINE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_ENABLE_TOWARDS_LINE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DISABLE_TOWARDS_LINE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Set(U32 TransactionIdentifier, U8 OutputSelector, U32 iSocketClientPDH, U32 RateParameter, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVE_RECOVERED_CLOCK, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

	*piAddAt=OutputSelector;
	piAddAt++;
	Result++;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

	UtilityPutU32(RateParameter, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableShallow(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableShallow(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableDeep(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableDeep(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 PRBS_Pattern, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=PRBS_Pattern;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 PRBS_Pattern, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=PRBS_Pattern;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}





U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 PRBS_Pattern, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=PRBS_Pattern;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 PRBS_Pattern, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    *piAddAt=PRBS_Pattern;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}






/*
 *
 * Section: CONFIGURATION FACILITIES DISCRETE CLIENT
 *
 */



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(U32 TransactionIdentifier, U8 iDiscreteClientPort, U8 Framing, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_FRAMING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Framing;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(U32 TransactionIdentifier, U8 iDiscreteClientPort, U8 Mapping, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_MAPPING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=Mapping;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(U32 TransactionIdentifier, U8 iDiscreteClientPort, U8 ClockRecoveryMode, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SET_CLOCK_RECOVERY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=ClockRecoveryMode;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLE_CAS, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);


	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLE_CAS, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_SOCKET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_SOCKET, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLE_TOWARDS_LINE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLE_TOWARDS_LINE, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_EnableShallow(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_ENABLE_SHALLOW, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_DisableShallow(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_DISABLE_SHALLOW, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_EnableDeep(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_ENABLE_DEEP, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_DisableDeep(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DIAGNOSTIC_LOOPBACK_DISABLE_DEEP, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}







/*
 *
 * Section: CONNECTIONS FUNCTIONS
 *
 */




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_CONNECTIONS_THROUGH_CREATE_CONNECTION, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pFirstLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pSecondLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_CONNECTIONS_THROUGH_DELETE_CONNECTION, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pFirstLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pSecondLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}





U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_CREATE_CONNECTION, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_CONNECTIONS_ADD_DROP_TO_CLIENT_SOCKET_DELETE_CONNECTION, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATE_CONNECTION, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDiscreteClientPort, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETE_CONNECTION, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iDiscreteClientPort;
    piAddAt+=1;
    Result+=1;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategoryForEndpoint(U32 TransactionIdentifier, U8 AlarmCategory, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 * pFormattedMessage)
{
    U8 Result=0;
	U8 BytesEncoded;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY_FOR_LINE_ENDPOINT, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
	Result+=1;

	BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategoryForEndpoint(U32 TransactionIdentifier, U8 AlarmCategory, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 * pFormattedMessage)
{
    U8 Result=0;
	U8 BytesEncoded;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY_FOR_LINE_ENDPOINT, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
	Result+=1;

	BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
	Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
	Result+=1;
 
    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

    return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
	Result+=1;
 
    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory, U32 * pFormattedMessage)
{
    U8 Result=0;
     U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLE_ALARM_REPORTING_FOR_ALARM_CATEGORY, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=AlarmCategory;
    piAddAt+=1;
	Result+=1;
 
    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_BOOTLOADER_DATA_PACKET_Send(U32 TransactionIdentifier, U32 *pMessageData, U8 Length, U32 *pFormattedMessage)
{
	U8 Result=0;
	U8 *piAddAt=(U8 *)pFormattedMessage;
    U8 *pMessage=(U8 *)pMessageData;
    U32 AnyInt;
    U8 i ;

	Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_BOOTLOADER_DATA_PACKET, TransactionIdentifier, 0, 0);
	piAddAt+=Result;


    for( i = 0 ; i < Length+4 ; i+=4)
    {
#ifndef XALYO_FRAMER_ACCESS
        AnyInt=UtilityGetU32(pMessage);
#else
{
unsigned int *ptr = (unsigned int *)pMessage;
AnyInt = ptr[0];
}
#endif
        UtilityPutU32(AnyInt, piAddAt);
        piAddAt+=4;
        pMessage+=4;
    }
	Result+=Length;

	OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage,Result);

	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_BOOTLOADER_START_FIRMWARE_Send(U32 TransactionIdentifier, U32 *pFormattedMessage)
{
	U8 Result=0;

	Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage,OMINNO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_BOOTLOADER_START_FIRMWARE, TransactionIdentifier, 0, 0);

	OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage,Result);

	return Result;
}







U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(U32 TransactionIdentifier, U8 iLinePort, U8 iPoint, U16 CompressedKey, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;
																			
    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_PORT_ENABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

	UtilityPutU16(CompressedKey, piAddAt);
	piAddAt+=2;
    Result+=2;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(U32 TransactionIdentifier, U8 iLinePort, U8 iPoint, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;
																			
    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_PORT_DISABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    *piAddAt=iLinePort;
    piAddAt+=1;
    Result+=1;

    *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}





U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U16 CompressedKey, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HIGH_ORDER_PATH_ENABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

	UtilityPutU16(CompressedKey, piAddAt);
	piAddAt+=2;
    Result+=2;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HIGH_ORDER_PATH_DISABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U16 CompressedKey, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LOW_ORDER_PATH_ENABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

	UtilityPutU16(CompressedKey, piAddAt);
	piAddAt+=2;
    Result+=2;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(U32 TransactionIdentifier, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U32 * pFormattedMessage)
{
    U8 BytesEncoded=0;
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LOW_ORDER_PATH_DISABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    BytesEncoded=OMIINO_LLC_SOUTHBOUND_FORMATTER_Encode_SONET_SDH_ENDPOINT_TYPE(piAddAt, pLineEndpoint);
    piAddAt+=BytesEncoded;
    Result+=BytesEncoded;

    *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);

	return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E1_ENABLE_MONITORING_CRC(U32 TransactionIdentifier, U32 iSocketClientPDH, U16 CompressedKey, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PM_E1_ENABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;


	UtilityPutU16(CompressedKey, piAddAt);
	piAddAt+=2;
    Result+=2;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}




U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_T1_ENABLE_MONITORING_CRC(U32 TransactionIdentifier, U32 iSocketClientPDH, U16 CompressedKey, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PM_T1_ENABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

	UtilityPutU16(CompressedKey, piAddAt);
	piAddAt+=2;
    Result+=2;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E3T3_ENABLE_MONITORING(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 iPoint, U16 CompressedKey, U32 * pFormattedMessage)
{
   U8 Result=0;
   U8 *piAddAt=(U8 *)pFormattedMessage;
   
   Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PM_E3T3_ENABLE_MONITORING, TransactionIdentifier, 0, 0);
   piAddAt+=Result;
   
   UtilityPutU32(iSocketClientPDH, piAddAt);
   piAddAt+=4;
   Result+=4;

   *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;
   
   UtilityPutU16(CompressedKey, piAddAt);
   piAddAt+=2;
   Result+=2;

   OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
   return Result;
}


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E1_DISABLE_MONITORING_CRC(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PM_E1_DISABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}



U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_T1_DISABLE_MONITORING_CRC(U32 TransactionIdentifier, U32 iSocketClientPDH, U32 * pFormattedMessage)
{
    U8 Result=0;
    U8 *piAddAt=(U8 *)pFormattedMessage;

    Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PM_T1_DISABLE_MONITORING, TransactionIdentifier, 0, 0);
    piAddAt+=Result;

    UtilityPutU32(iSocketClientPDH, piAddAt);
    piAddAt+=4;
    Result+=4;

    OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
	return Result;
}

U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E3T3_DISABLE_MONITORING(U32 TransactionIdentifier, U32 iSocketClientPDH, U8 iPoint, U32 * pFormattedMessage)
{
   U8 Result=0;
   U8 *piAddAt=(U8 *)pFormattedMessage;
   
   Result+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(pFormattedMessage, OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_PM_E3T3_DISABLE_MONITORING, TransactionIdentifier, 0, 0);
   piAddAt+=Result;
   
   UtilityPutU32(iSocketClientPDH, piAddAt);
   piAddAt+=4;
   Result+=4;

   *piAddAt=iPoint;
    piAddAt+=1;
    Result+=1;

   OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(pFormattedMessage, Result);
   return Result;
}

