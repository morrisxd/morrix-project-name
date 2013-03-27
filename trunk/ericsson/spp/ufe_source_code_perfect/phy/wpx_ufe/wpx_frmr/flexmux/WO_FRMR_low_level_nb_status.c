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



void OMIINO_LLC_Northbound_StatusMessage(U8 iDevice, OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
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

            if(OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_MAX_MESSAGES>MessageIdentifier)
            {
				(*pFramerRAM->LLC_Table.StatusTable.Parser[MessageIdentifier])(MessageIdentifier, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length);
            }
            else
            {
                if((unsigned int)OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE!=MessageIdentifier)
                {
                    OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length, __LINE__, __FILE__);
                }
            }
        }
        else
        {
            OMIINO_LLC_Northbound_Status_ErrorHandler(0x0000DEAD, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length, __LINE__, __FILE__);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_Status_ErrorHandler(0x0000DEAD, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, 0xFF, __LINE__, __FILE__);
    }
}







void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_ALARM_CHANGE(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U32 Key, U8 IsAsserted)
{
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	OMIINO_FRAMER_ALARM_MANAGER_ALARM_STATUS_CHANGE(iDevice, &(pHierarchyRAM->Alarms), Key, IsAsserted, 0, pHierarchyRAM);
}

void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_ALARM(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U32 iReport;
    U32 N_Reports=0;
    U32 Key, FeacKey;
    U8  IsAsserted;
    OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM = &(pHierarchyRAM->Alarms);


    if(24>Length)  /* No Transaction Identifier with Autonomous Signalling Messages */
    {
        Result=0;
        OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }

    if(Result)
    {
        N_Reports=UtilityGetU32(&pMessage[iNextU8]);
        iNextU8+=4;

        if(WPX_UFE_FRAMER_MAX_ALARM_REPORTS_PER_MESSAGE<N_Reports)
        {
            Result=0;
            OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
        }
    }

    if(Result)
    {
        for(iReport=0;iReport<N_Reports;iReport++)
        {
            if(Length>iNextU8)
            {
                Key=UtilityGetU32(&pMessage[iNextU8]);
                iNextU8+=4;

                /*Checking if alarm is T3 FEAC, we need to extrac the FEAC code from the message */
                FeacKey = Key&~0xFE000000;

                if (FeacKey< MAX_ALARM_MANAGER_NODE_ELEMENTS && pAlarmMangerRAM->Element[FeacKey].AlarmCategory == WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC)
                {
                   IsAsserted = (Key & 0xFE000000) >> 25;
                   Key = FeacKey;
                }
                else
                {
                   if(0x80000000&Key)
                   {
                      IsAsserted=1;
                      Key=Key&~0x80000000;
                   }
                   else
                   {
                      IsAsserted=0;
                   }
                }

				if(MAX_ALARM_MANAGER_NODE_ELEMENTS>Key)
				{
					OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_ALARM_CHANGE(iDevice, pHierarchyRAM, Key, IsAsserted);
				}
            }
            else
            {
                OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
                break;
            }
        }
    }
}





void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_C2_CHANGE(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 C2)
{
	U8 Result;

	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(&(pHierarchyRAM->SONET_SDH_Port[pLineEndpoint->iLinePort]), pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(&(pHierarchyRAM->SONET_SDH_Port[pLineEndpoint->iLinePort]), pLineEndpoint);

		OMIINO_FRAMER_SONET_SDH_HO_Path_C2_SetRX(&(pDeviceHO_PathRAM->Overhead.C2.Status), C2);
	}
}

void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_SS_BITS_RX(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Value)
{
	U8 Result;
    WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE                   LineEndpointToSend;
/*	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;*/
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(&(pHierarchyRAM->SONET_SDH_Port[pLineEndpoint->iLinePort]), pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{

        WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_PresentationEndpointToExternal(iDevice, &LineEndpointToSend, pLineEndpoint);
        switch(OMIINO_RAM.Device[iDevice].BuildPersonality)
        {
        case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
        case WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:
        case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
        case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
            switch(pLineEndpoint->Bandwidth)
            {
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
                OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), LineEndpointToSend.u.SDH.stm1, &LineEndpointToSend.u.SDH.stm1);
                break;
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
            case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
                OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), LineEndpointToSend.u.SONET.sts3, &LineEndpointToSend.u.SONET.sts3);
                break;
            default:
                break;
            }
        default:
            break;
        }


/*		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(&(pHierarchyRAM->SONET_SDH_Port[pLineEndpoint->iLinePort]), pLineEndpoint);*/
        OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_SS_BITS_RESPONSE(iDevice,&LineEndpointToSend,Value);
	}
}


void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_SS_BITS(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{

    U8  Result=1;
    U8  iNextU8=16; /* MessageIdentifier in first 4 bytes of received message */
    U8  BytesParsedThisField;


    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8	Value;
    if(Length>21)
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
        if(1==(Length-iNextU8))
        {
            Value=pMessage[iNextU8++];
            OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_SS_BITS_RX(iDevice, pHierarchyRAM, &LineEndpoint, Value);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}

void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_C2(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{

    U8  Result=1;
    U8  iNextU8=16; /* MessageIdentifier in first 4 bytes of received message */
    U8  BytesParsedThisField;


    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8	C2;

    if(Length>21)
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
        if(1==(Length-iNextU8))
        {
            C2=pMessage[iNextU8++];
            OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_C2_CHANGE(iDevice, pHierarchyRAM, &LineEndpoint, C2);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J0_CHANGE(U8 iDevice, U8 iLinePort, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pPathTrace)
{
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetRX(&(pHierarchyRAM->SONET_SDH_Port[iLinePort].Section.J0), (char *)pPathTrace);
}


void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J0(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U8  LocalBytesParsed;

    U8	    iLinePort;
    char    PathTrace[64+1];


    iLinePort=pMessage[iNextU8++];

    if(Length>(iNextU8-16))
    {
		memset(PathTrace,'\0',65);
        Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &LocalBytesParsed);
        iNextU8+=LocalBytesParsed;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
		OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J0_CHANGE(iDevice, iLinePort, pHierarchyRAM, (U8 *)PathTrace);
    }
    else
    {
        OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRBS_SOCKET_INGRESS(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  iNextU8=16;
	U32	iSocket;
	U8	PRBS_State;

	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

    iSocket=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;
	PRBS_State=pMessage[iNextU8];

	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Ingress_PRBS_State(&(pHierarchyRAM->SocketClient.Element[iSocket].Status), PRBS_State);
}


void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRBS_SOCKET_EGRESS(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  iNextU8=16;
	U32	iSocket;
	U8	PRBS_State;


	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
	OMIINO_REMOVE_COMPILER_WARNING(Length);
	OMIINO_REMOVE_COMPILER_WARNING(iDevice);

    iSocket=UtilityGetU32(&pMessage[iNextU8]);
	iNextU8+=4;
	PRBS_State=pMessage[iNextU8];

	OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Egress_PRBS_State(&(pHierarchyRAM->SocketClient.Element[iSocket].Status), PRBS_State);
}


void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J1_CHANGE(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pPathTrace)
{
	U8 Result;

	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(&(pHierarchyRAM->SONET_SDH_Port[pLineEndpoint->iLinePort]), pLineEndpoint);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(&(pHierarchyRAM->SONET_SDH_Port[pLineEndpoint->iLinePort]), pLineEndpoint);

		OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetRX(&(pDeviceHO_PathRAM->Overhead.J1), (char * )pPathTrace);
	}
}


void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J1(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U8  LocalBytesParsed;


    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    char    PathTrace[64+1];

    if(17<Length)
    {
		Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &LocalBytesParsed);
        iNextU8+=LocalBytesParsed;
    }
    else
    {
        Result=0;
    }

    if(Result)
    {
        if(Length>(iNextU8-16))
        {
			memset(PathTrace,'\0',65);
            Result=OMIINO_LLC_Northbound_Utility_Parse_PathTrace(&pMessage[iNextU8], (Length-iNextU8), PathTrace, &LocalBytesParsed);
            iNextU8+=LocalBytesParsed;
        }
    }

    if(Result)
    {
        OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J1_CHANGE(iDevice, pHierarchyRAM, &LineEndpoint, (U8 *)PathTrace);
    }
    else
    {
        OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J2_CHANGE(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pPathTrace)
{
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	if(OMIINO_FRAMER_GetConfigurationRAMForLO_Path_Exists(pHierarchyRAM, pLineEndpoint))
	{
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pHierarchyRAM, pLineEndpoint);
		OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetRX(&(pDeviceLO_PathRAM->Overhead.J2), (char *)pPathTrace);
	}
}



void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J2(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U8  LocalBytesParsed;


    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    char    PathTrace[64+1];

    if(17<Length)
    {
        Result=OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(&pMessage[iNextU8], (Length-iNextU8), &LineEndpoint, &LocalBytesParsed);
        iNextU8+=LocalBytesParsed;
    }

    if(Result)
    {
        if(Length>(iNextU8-16))
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
        OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J2_CHANGE(iDevice, pHierarchyRAM, &LineEndpoint, (U8 *)PathTrace);
    }
    else
    {
        OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}





void OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_V5_CHANGE(U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 V5)
{
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice,iDevice);

	if(OMIINO_FRAMER_GetConfigurationRAMForLO_Path_Exists(pHierarchyRAM, pLineEndpoint))
	{
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pHierarchyRAM, pLineEndpoint);
		OMIINO_FRAMER_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_SetRX(&(pDeviceLO_PathRAM->Overhead.V5_SignalLabel.Status), V5);
	}
}


void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_V5_SIGNAL_LABEL(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{

    U8  Result=1;
    U8  iNextU8=16;
    U8  BytesParsedThisField;


    WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE LineEndpoint;
    U8	V5;

    if(Length>21)
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
        if(1==(Length-iNextU8))
        {
            V5=pMessage[iNextU8++];
            OMIINO_LLC_Northbound_Status_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_V5_CHANGE(iDevice, pHierarchyRAM, &LineEndpoint, V5);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}









void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRODUCT_HARDWARE_INFORMATION(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  iNextU8=16;
	OMIINO_FRAMER_STATUS_DEVICE_TYPE *pStatus=&(pHierarchyRAM->Status);

    if(Length>=80)
    {
		OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareProductNameStr(pStatus, (char*)&pMessage[iNextU8]);
		iNextU8+=OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_PRODUCT_NAME_FIELD;
		OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareVersionStr(pStatus, (char*)&pMessage[iNextU8]);
		iNextU8+=OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_VERSION_FIELD;
		OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareDateStr(pStatus, (char*)&pMessage[iNextU8]);
		iNextU8+=OMIINO_FRAMER_MAX_CHARS_IN_HARDWARE_DATE_FIELD;
		OMIINO_FRAMER_STATUS_DeviceDriver_SetHardwareTimeStr(pStatus, (char*)&pMessage[iNextU8]);
    }
    else
    {
        OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}






void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRODUCT_FIRMWARE_INFORMATION(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  iNextU8=16;
	OMIINO_FRAMER_STATUS_DEVICE_TYPE *pStatus=&(pHierarchyRAM->Status);

    if(Length>=72)
    {
		OMIINO_FRAMER_STATUS_DeviceDriver_SetFirmwareProductNameStr(pStatus, (char*)&pMessage[iNextU8]);
		iNextU8+=OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_PRODUCT_NAME_FIELD;
		OMIINO_FRAMER_STATUS_DeviceDriver_SetFirmwareDateTimeStr(pStatus, (char*)&pMessage[iNextU8]);
		iNextU8+=OMIINO_FRAMER_MAX_CHARS_IN_SOFTWARE_DATE_TIME_FIELD;
		OMIINO_FRAMER_STATUS_DeviceDriver_SetFirmwareVersionStr(pStatus, (char*)&pMessage[iNextU8]);
    }
    else
    {
        OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_REQUEST_DRIVER_INFORMATION(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
	char ProductReleaseStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];
    char SoftwareBuildDateTimeStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];
    char SoftwareBuildVersionStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];


	OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);


	memset(ProductReleaseStr,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1);
    OMIINO_FRAMER_STATUS_DeviceDriver_GetProductReleaseStr(&OMIINO_RAM.Driver,ProductReleaseStr);
    Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DRIVER_ProductRelease(0, ProductReleaseStr, (unsigned char)strlen(ProductReleaseStr), OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}

	memset(SoftwareBuildDateTimeStr,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1);
	OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildDateTimeStr(&OMIINO_RAM.Driver,SoftwareBuildDateTimeStr);
    Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DRIVER_SoftwareBuildDate(0, SoftwareBuildDateTimeStr, (unsigned char)strlen(SoftwareBuildDateTimeStr), OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}

	memset(SoftwareBuildVersionStr,'\0',OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1);
    OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildVersionStr(&OMIINO_RAM.Driver,SoftwareBuildVersionStr);
	Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DRIVER_SoftwareBuildVersion(0, SoftwareBuildVersionStr, (unsigned char)strlen(SoftwareBuildVersionStr), OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}
}



void OMIINO_LLC_Northbound_Status_NULL(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    OMIINO_LLC_Northbound_Status_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
}



void OMIINO_LLC_Northbound_Status_IGNORE(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);
}



void OMIINO_LLC_Northbound_StatusTableInitialize(OMNISPY_STATUS_TABLE_TYPE * pStatusTable)
{
    int i;

    for(i=0;i<OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_MAX_MESSAGES;i++)
    {
        pStatusTable->Parser[i]=OMIINO_LLC_Northbound_Status_NULL;
    }

    pStatusTable->Parser[OMIINO_SOUTHBOUND_MESSAGE_CATALOGUE_STATUS_DEFAULT_RAM_IGNORE			]=OMIINO_LLC_Northbound_Status_IGNORE;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_ALARM						]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_ALARM;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_C2							]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_C2;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J0							]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J0;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J1							]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J1;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J2							]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_J2;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_V5_SIGNAL_LABEL				]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_V5_SIGNAL_LABEL;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRODUCT_HARDWARE_INFORMATION]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRODUCT_HARDWARE_INFORMATION;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRODUCT_FIRMWARE_INFORMATION]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRODUCT_FIRMWARE_INFORMATION;
	pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_REQUEST_DRIVER_INFORMATION  ]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_REQUEST_DRIVER_INFORMATION;
	pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRBS_SOCKET_INGRESS         ]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRBS_SOCKET_INGRESS;
	pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRBS_SOCKET_EGRESS          ]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_PRBS_SOCKET_EGRESS;
    pStatusTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_SS_BITS                     ]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_SS_BITS;
}
