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
#include "WO_FRMR_configuration_defaults_private.h"

void QC_Begin(void)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{	
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_QC_BEGIN, 0, 0, 0);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}

void QC_Item(int iItem)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{	
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_QC_ITEM, 0, 8, 0);
		piAddAt+=16;

		UtilityPutU32((U32)iItem, piAddAt);
		Length+=4;
		piAddAt+=4;

		UtilityPutU32((U32)OMIINO_RAM.QC[iItem], piAddAt);
		Length+=4;
		piAddAt+=4;
		
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}

void QC_End(void)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{	
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_QC_END, 0, 0, 0);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}

void QC(void)
{
	int i;

	QC_Begin();
	for(i=0;i<QC_MAX;i++)
	{
		QC_Item(i);
	}
	QC_End();
}

void QC_API_Internal_Initialize(void)
{
	int i;

	for(i=0;i<QC_MAX;i++)
	{
		OMIINO_RAM.QC[i]=0;
	}
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_LOW_LEVEL_COMMS_ENABLE_ACCESS(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_CONFIGURATION_LOW_LEVEL_COMMS_ENABLE_ACCESS]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

    if(WPX_UFE_FRAMER_OK==Result)
	{
		OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted=1;
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_LOW_LEVEL_COMMS_DISABLE_ACCESS(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_CONFIGURATION_LOW_LEVEL_COMMS_DISABLE_ACCESS]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

    if(WPX_UFE_FRAMER_OK==Result)
	{
		OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted=0;
	}

	return Result;
}


/*
 *
 * Section: OmniSpy API
 *
 */

U8 OMIINO_API_INTERNAL_OMNISPY_SouthboundMessage(U8 iDevice, U32 * pMessage, U8 Length)
{
    U8 Result = WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_OMNISPY_SOUTHBOUNDMESSAGE]++;

    Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

    if(WPX_UFE_FRAMER_OK==Result)
	{
        Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
        if(WPX_UFE_FRAMER_OK!=Result)
        {
            Result=WPX_UFE_FRAMER_ERROR_DEVICE_NOT_READY;
        }
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
        Result=OMIINO_OMNISPY_SouthboundMessage(iDevice, pMessage, Length);
    }

    return Result;
}









void OMIINO_API_INTERNAL_Reset(void)
{
	U8 Length;
	U8 iDevice;


	InitializationProcedure();

	for(iDevice=0;iDevice<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES;iDevice++)
    {
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_RESET(OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
}






void OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(U8 iDevice, U32 NodeKey)
{
	U8 ReportingIsEnabled;

	OMIINO_FRAMER_Device_SONET_SDH_GetAlarmCategoryReporting(&OMIINO_RAM.Device[iDevice].Hierarchy, OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[NodeKey].AlarmCategory, &ReportingIsEnabled);
	if(OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[NodeKey].IsAsserted)
	{
		OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[NodeKey].IsAsserted=0;
		OMIINO_FRAMER_ALARM_MANAGER_ALARM_ANNOUNCE_CHANGE(iDevice, 1, &(OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[NodeKey]));
	}
}



void OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(U8 iDevice, U32 NodeKey)
{
	OMIINO_RAM.Device[iDevice].Hierarchy.Alarms.Element[NodeKey].IsAsserted=0;
}


void OMIINO_FRAMER_STATUS_Given_LO_PATH_ResetAlarmState(U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path)
{
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.J2.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[2]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[3]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[4]);
#if 0
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[5]);
#endif
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[1]);
}



void OMIINO_FRAMER_STATUS_Given_HO_PATH_ResetAlarmState(U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.G1.Status.Alarms.Key[0]);
#if 0	
OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.G1.Status.Alarms.Key[1]);
#endif
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.B3.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.B3.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.J1.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.C2.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.C2.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pHO_Path->Overhead.H4.Status.Alarms.Key[0]);
}



void OMIINO_FRAMER_STATUS_Clear_This_LO_PathAlarms(U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path)
{
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.J2.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[2]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[3]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[4]);
#if 0
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[5]);
#endif
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[1]);
}



void OMIINO_FRAMER_STATUS_Clear_This_HO_PathAlarms(U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path)
{
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.G1.Status.Alarms.Key[0]);
#if 0	
OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.G1.Status.Alarms.Key[1]);
#endif
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.B3.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.B3.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.J1.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.C2.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.C2.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[0]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[1]);
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pHO_Path->Overhead.H4.Status.Alarms.Key[0]);
}


void OMIINO_FRAMER_STATUS_Given_SocketClient_ResetAlarmState(U8 iDevice, OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClientStatus, U8 ClientType)
{
        if (ClientType == WPX_UFE_FRAMER_CLIENT_T3)
        {
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA]);
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS]);
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF]);
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC]);
        }
        else
        {
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI]);
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI]);
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS]);
           OMIINO_FRAMER_STATUS_GivenNodeKeyClearAlarm(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF]);
        }
}


void OMIINO_FRAMER_STATUS_Clear_This_SocketClient_Alarms(U8 iDevice, OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClientStatus, U8 ClientType)
{
        if (ClientType == WPX_UFE_FRAMER_CLIENT_T3)
        {
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA]);
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS]);
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF]);
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC]);
        }
        else
        {
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI]);
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI]);
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS]);
           OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF]);
        }
}

/*
 *
 * Section: Device Protection API
 *
 */

U8 OMIINO_API_INTERNAL_SetDeviceProtectionMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceProtectionMode)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 iLinePort;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;
	OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM;
	U8 Length;
	U8 BuildPersonality;

	OMIINO_REMOVE_COMPILER_WARNING(TransactionIdentifier);

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_SETPROTECTIONMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceProtectionModeInRange(DeviceProtectionMode);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceProtectionMode(iDevice, DeviceProtectionMode);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
		{
			if(OMIINO_FRAMER_SONET_SDH_Port_Has_Facilities(iDevice, iLinePort))
			{
				Result=WPX_UFE_FRAMER_ERROR_FACILITIES_EXIST;
				break;
			}
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_SetProtectionMode(pHierarchyRAM, DeviceProtectionMode);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, &BuildPersonality);
		Result=OMIINO_FRAMER_PortMap_Define(&(OMIINO_RAM.Device[iDevice]), BuildPersonality, DeviceProtectionMode);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetProtectionMode(TransactionIdentifier, DeviceProtectionMode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		switch(DeviceProtectionMode)
		{
			case	WPX_UFE_DEVICE_PROTECTION_MODE_NONE:
					OMIINO_FRAMER_RSE(DeviceProtectionMode);
					break;

			case	WPX_UFE_DEVICE_PROTECTION_MODE_LINE:
					pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
					OMIINO_FRAMER_Device_SetCardProtectionOperatingMode(pHierarchyRAM, WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_NONE);
					break;

			case	WPX_UFE_DEVICE_PROTECTION_MODE_CARD:
					pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
					OMIINO_FRAMER_Device_SetCardProtectionOperatingMode(pHierarchyRAM, WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER);
					
					Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetCardProtectionOperatingMode(TransactionIdentifier, WPX_UFE_DEVICE_CARD_PROTECTION_OPERATING_MODE_MASTER, OMIINO_RAM.Device[iDevice].LLC.Buffer);
					if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
					{
						OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
					}
					break;

			default:
					OMIINO_FRAMER_RSE(DeviceProtectionMode);
					break;
		}
	}

	return Result;
}





U8 OMIINO_API_INTERNAL_GetDeviceProtectionMode(U8 iDevice, U8 * pDeviceProtectionMode)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_GETPROTECTIONMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, pDeviceProtectionMode);
	}

	return Result;
}







U8 OMIINO_API_INTERNAL_SetDeviceCardProtectionOperatingMode(U32 TransactionIdentifier, U8 iDevice, U8 CardProtectionOperatingMode)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 iLinePort;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;
	U8 Length=0;
	U8 DeviceProtectionMode=WPX_UFE_DEVICE_PROTECTION_MODE_NONE;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_SETCARDPROTECTIONOPERATINGMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceCardProtectionOperatingModeInRange(CardProtectionOperatingMode);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, &DeviceProtectionMode);
		if(WPX_UFE_DEVICE_PROTECTION_MODE_CARD!=DeviceProtectionMode)
		{
			Result=WPX_UFE_FRAMER_ERROR_DEVICE_CARD_PROTECTION_OPERATING_MODE_NOT_APPLICABLE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
		{
			if(OMIINO_FRAMER_SONET_SDH_Port_Has_Facilities(iDevice, iLinePort))
			{
				Result=WPX_UFE_FRAMER_ERROR_FACILITIES_EXIST;
				break;
			}
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_SetCardProtectionOperatingMode(pHierarchyRAM, CardProtectionOperatingMode);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetCardProtectionOperatingMode(TransactionIdentifier, CardProtectionOperatingMode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}





U8 OMIINO_API_INTERNAL_GetDeviceCardProtectionOperatingMode(U8 iDevice, U8 * pCardProtectionOperatingMode)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_GETCARDPROTECTIONOPERATINGMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetCardProtectionOperatingMode(pHierarchyRAM, pCardProtectionOperatingMode);
	}

	return Result;
}





U8 OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_Poke(U32 TransactionIdentifier, U8 iDevice, U32 Address, U32 Value)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_DEVICE_DIAGNOSTIC_POKE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
        OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_Poke(iDevice, TransactionIdentifier, Address, Value, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	}

    return Result;
}



U8 OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_Peek(U8 iDevice, U32 Address)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_DEVICE_DIAGNOSTIC_PEEK]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
        OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_Peek(iDevice, 0, Address, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	}

    return Result;
}




U8 OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_BlockPoke(U32 TransactionIdentifier, U8 iDevice, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten, U32 Value)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_DEVICE_DIAGNOSTIC_BLOCK_POKE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(16<NumberOfU32AddressesToBeWritten)
		{
			Result=WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_BlockPoke(iDevice, TransactionIdentifier, StartAddress, NumberOfU32AddressesToBeWritten, Value, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	}

    return Result;
}



U8 OMIINO_API_INTERNAL_DEVICE_DIAGNOSTIC_BlockPokeVerify(U32 TransactionIdentifier, U8 iDevice, U32 StartAddress, U32 NumberOfU32AddressesToBeWritten, U32 Value)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_DEVICE_DIAGNOSTIC_BLOCK_POKE_VERIFY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DeviceInTestMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        OMIINO_LLC_SOUTHBOUND_FORMATTER_DEVICE_DIAGNOSTIC_BlockPokeVerify(iDevice, TransactionIdentifier, StartAddress, NumberOfU32AddressesToBeWritten, Value, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	}

    return Result;
}




/*
 *
 * Section: DEVICE 
 *
 */


U8  OMIINO_API_INTERNAL_DeviceIsReady(U8 iDevice)
{
    U8 Result = WPX_UFE_FRAMER_OK;

    Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
        if(WPX_UFE_FRAMER_OK!=Result)
        {
            Result=WPX_UFE_FRAMER_ERROR_DEVICE_NOT_READY;
        }
    }

    return Result;
}



U8 OMIINO_API_INTERNAL_ServiceNorthboundLowLevelComms(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	
	OMIINO_RAM.QC[QC_SERVICENORTHBOUNDLOWLEVELCOMMS]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_HandleNorthboundMessagesForDevice(iDevice, &OMIINO_RAM);
		}
	}

    return Result;
}



void OMIINO_API_INTERNAL_ISR(U8 iDevice)
{
	int i;

    for(i=0;i<WPX_UFE_FRAMER_UNI_DIRECTIONAL_MAX_COMMS_CHANNELS;i++)
    {
#ifndef XALYO_FRAMER_ACCESS
        *OMIINO_RAM.Device[iDevice].MemoryMap.Address[i].PEND=0x00000001;
#else
        volatile unsigned int *ptr = OMIINO_RAM.Device[iDevice].MemoryMap.Address[i].PEND;
        WP_MEM_SET(ptr[0], 0x00000001);
#endif
    }
}

/*
 *
 * Section: CONFIGURATION PRODUCT BUILD
 *
 */


U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_SetBuildPersonality(U8 iDevice, U8 BuildPersonality)
{
	U8 Length;
	U8 iLinePort;
	U8 DeviceProtectionMode;
	U32 iSocketPort;
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM;
	OMIINO_FRAMER_STATUS_DEVICE_TYPE *pStatusRAM;
	OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;


	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_SETBUILDPERSONALITY]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_BuildPersonalityInRange(BuildPersonality);
	}
    
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_BuildPersonalitySupported(BuildPersonality);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pStatusRAM=OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, iDevice);
		Result = OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule(pStatusRAM, BuildPersonality);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		/* DMCC - Should this be applied to all devices? */
		for(iLinePort=0;iLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLinePort++)
		{
			if(OMIINO_FRAMER_SONET_SDH_Port_Has_Facilities(iDevice, iLinePort))
			{
				Result=WPX_UFE_FRAMER_ERROR_FACILITIES_EXIST;
				break;
			}
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		for(iSocketPort=0;iSocketPort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;iSocketPort++)
		{
			pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pHierarchyRAM, iSocketPort);
			if(OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Exists(pClient))
			{
				Result=WPX_UFE_FRAMER_ERROR_FACILITIES_EXIST;
				break;
			}
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pHierarchyRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetProtectionMode(pHierarchyRAM, &DeviceProtectionMode);
		Result=OMIINO_FRAMER_PortMap_Define(&(OMIINO_RAM.Device[iDevice]), BuildPersonality, DeviceProtectionMode);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_SOUTHBOUND_MESSAGE_CATALOGUE_CONFIGURATION_RESET(OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}

		pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_SetBuildPersonality(pDeviceRAM, BuildPersonality);

		OMIINO_ProductConfiguration_SendBuildSelectionToFirmware(iDevice);

		OMIINO_DefaultAllPortsForDevice(iDevice);
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_GetBuildPersonality(U8 iDevice, U8 * pBuildPersonality)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_DEVICE_TYPE * pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_GETBUILDPERSONALITY]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pBuildPersonality);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM=OMIINO_FRAMER_GetRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetBuildPersonality(pDeviceRAM, pBuildPersonality);
	}

	return Result;
}


/*
 *
 * Section: CONFIGURATION DEVICE
 *
 */


U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 DeviceMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 i = 0;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;
    extern void OMIINO_FRAMER_STATUS_Initialize_SONET_SDH_Port(U8 iDevice,
                      OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM,
                      U8 iLinePort,
                      OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM);
    U8 currentDeviceMode;
    extern void OMIINO_FRAMER_STATUS_FreeAlarmKeysFor_SONET_SDH_Port(U8 iDevice,
                      OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM,
                      OMIINO_FRAMER_ALARM_MANAGER_DATA_TYPE * pAlarmMangerRAM);

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_SETMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceModeInRange(DeviceMode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceIsFreeOfLineSideFacilities(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceIsFreeOfPortConfiguration(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
                OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &currentDeviceMode);

                if (currentDeviceMode != DeviceMode)
                {
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS; i++)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, i);
             OMIINO_FRAMER_STATUS_FreeAlarmKeysFor_SONET_SDH_Port(iDevice, pDevicePortRAM, (&pDeviceRAM->Alarms));
			OMIINO_FRAMER_SONET_SDH_Port_SetMode(&(pDevicePortRAM->Configuration), DeviceMode);
             OMIINO_FRAMER_STATUS_Initialize_SONET_SDH_Port(iDevice, pDevicePortRAM, i, (&pDeviceRAM->Alarms));
         }

                   OMIINO_FRAMER_Device_SetMode(pDeviceRAM, DeviceMode);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_SetMode(TransactionIdentifier, DeviceMode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_GetMode(U8 iDevice, U8 * pDeviceMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_GETMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pDeviceMode);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetMode(pDeviceRAM, pDeviceMode);
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Enable_AIS_Insertion(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_ENABLEAISINSERTION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_Set_AIS_Insertion(pDeviceRAM, 1);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_Set_AIS_Insertion(TransactionIdentifier, 1, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Disable_AIS_Insertion(U32 TransactionIdentifier, U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DISABLEAISINSERTION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_Set_AIS_Insertion(pDeviceRAM, 0);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_Set_AIS_Insertion(TransactionIdentifier, 0, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}





U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_Get_AIS_Insertion(U8 iDevice, U8 *pAIS_InsertionMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_GETAISINSERTION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pAIS_InsertionMode);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_Get_AIS_Insertion(pDeviceRAM, pAIS_InsertionMode);
	}

	return Result;
}






U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Set(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iLinePort)
{
    U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DRIVERECOVEREDCLOCK_SET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_DEVICE_Diagnostic_SetDriveRecoveredClock(&(pDeviceRAM->Configuration.Diagnostic), iLinePort, OutputSelector);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_DriveRecoveredClock_Set(TransactionIdentifier, iLinePort, OutputSelector, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_SquelchRecoveredClock_Set(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 Squelch_Mode)
{
    U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DEVICE_SQUELCHRECOVEREDCLOCK_SET]++;
	/*check that the device parameter is legal*/
	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	/*check that the output selector parameter is legal*/
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}


	/*check that the squelch mode is legal (enable/disable) */
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_StaticRule_SquelchModeInRange(Squelch_Mode);
	}
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_DEVICE_SetSquelchRecoveredClock(&(pDeviceRAM->Configuration.Diagnostic), Squelch_Mode, OutputSelector);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SquelchRecoveredClock_Set(TransactionIdentifier, Squelch_Mode, OutputSelector, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Get(U8 iDevice, U8 OutputSelector, U8 * piLinePort)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DRIVERECOVEREDCLOCK_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_DEVICE_Diagnostic_GetDriveRecoveredClock(&(pDeviceRAM->Configuration.Diagnostic), piLinePort, OutputSelector);
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_SquelchRecoveredClock_Get(U8 iDevice, U8 OutputSelector, U8* pSquelch_Mode)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DEVICE_SQUELCHRECOVEREDCLOCK_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_DEVICE_GetSquelchRecoveredClock(&(pDeviceRAM->Configuration.Diagnostic), pSquelch_Mode, OutputSelector);
	}

	return Result;
}



/*
 *
 * Section: CONFIGURATION FACILITIES
 *
 */




/*
 *
 * DMCC - BULK APIs are for OMIINO
 *
 */


U8 OMIINO_API_INTERNAL_BULK_PM_ENABLE_LINE(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_RAM.QC[QC_BULK_PM_ENABLE_LINE]++;

	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1);
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2);
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1);
	
	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_PM_DISABLE_LINE(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_RAM.QC[QC_BULK_PM_DISABLE_LINE]++;

	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1);
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2);
	OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iDevice, iLinePort, WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1);
	
	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_PM_ENABLE_HO(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_RAM.QC[QC_BULK_PM_ENABLE_HO]++;

	OMIINO_API_INTERNAL_BULK_PM_Enable_HO_For_Port(TransactionIdentifier, iDevice, iLinePort);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_PM_DISABLE_HO(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_RAM.QC[QC_BULK_PM_DISABLE_HO]++;

	OMIINO_API_INTERNAL_BULK_PM_Disable_HO_For_Port(TransactionIdentifier, iDevice, iLinePort);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_PM_ENABLE_LO(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_RAM.QC[QC_BULK_PM_ENABLE_LO]++;

	OMIINO_API_INTERNAL_BULK_PM_Enable_LO_For_Port(TransactionIdentifier, iDevice, iLinePort);

	return WPX_UFE_FRAMER_OK;
}


U8 OMIINO_API_INTERNAL_BULK_PM_DISABLE_LO(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	OMIINO_RAM.QC[QC_BULK_PM_DISABLE_LO]++;

	OMIINO_API_INTERNAL_BULK_PM_Disable_LO_For_Port(TransactionIdentifier, iDevice, iLinePort);

	return WPX_UFE_FRAMER_OK;
}





U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 AlarmReportingState[WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES];

	OMIINO_RAM.QC[QC_BULK_CONFIGURATION_FACILITIES_SONET_SDH_CREATEFACILITY]++;

	OMIINO_API_INTERNAL_BULK_SDH_ALARM_FetchReportingStateForAll(AlarmReportingState);

	OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_ALL(TransactionIdentifier, iDevice);


	switch(Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_4C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_HI_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_LO_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC12_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC11_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS48C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS12C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS3C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS1_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT2_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT15_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;
	}

	OMIINO_API_INTERNAL_BULK_SDH_ALARM_SetReportingStateForAll(AlarmReportingState);

	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;


	OMIINO_RAM.QC[QC_BULK_CONFIGURATION_FACILITIES_SONET_SDH_DELETEFACILITY]++;

	switch(Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_4C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_HI_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_LO_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC12_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC11_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS48C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS12C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS3C_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS1_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT2_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT15_Facility(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;
	}

	return Result;
}






void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForSocketIfRequired(U32 TransactionIdentifier, U8 iDevice, U8 AnySocketAlarmCategory, U32 iSocketClientPDH)
{
	U8 Length;
	U8 ReportingEnabled;

	OMIINO_REMOVE_COMPILER_WARNING(iSocketClientPDH); /* TODO Individual granularity */

	OMIINO_FRAMER_Device_SOCKET_CLIENT_GetAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice), AnySocketAlarmCategory, &ReportingEnabled);

	if(1==ReportingEnabled)
	{
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AnySocketAlarmCategory, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
}







void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(U32 TransactionIdentifier, U8 iDevice, U8 AnyAlarmCategory, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Length;
	U8 ReportingEnabled;

	
	OMIINO_FRAMER_Device_SONET_SDH_GetAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice), AnyAlarmCategory, &ReportingEnabled);

	if(1==ReportingEnabled)
	{
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategoryForEndpoint(TransactionIdentifier, AnyAlarmCategory, pLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
}




void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SDH_HoPathAlarms(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_AU_AIS,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_AU_LOP,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_LOM,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_TIM,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_PLM,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_UNEQ, pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_RDI,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_EXC,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_DEG,  pLineEndpoint);
}

void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SONET_HoPathAlarms(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_AIS_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOP_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOM_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_TIM_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_PLM_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_UNEQ_P, pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_RDI_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_EXC_P,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_DEG_P,  pLineEndpoint);
}



void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_HoPathAlarms(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 PortMode;

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SDH_HoPathAlarms(TransactionIdentifier, iDevice, pLineEndpoint);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SONET_HoPathAlarms(TransactionIdentifier, iDevice, pLineEndpoint);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	
}





void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SDH_LoPathAlarms(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_TIM,	  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_PLM,	  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_UNEQ,  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_RDI,	  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_EXC,	  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_DEG,	  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_TU_AIS,	  pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_TU_LOP,	  pLineEndpoint);
}

void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SONET_LoPathAlarms(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_TIM_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_PLM_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_UNEQ_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_RDI_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_EXC_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_DEG_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_AIS_V,	 pLineEndpoint);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisAlarmForEndpointIfRequired(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOP_V,	 pLineEndpoint);
}



void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_LoPathAlarms(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 PortMode;

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SDH_LoPathAlarms(TransactionIdentifier, iDevice, pLineEndpoint);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SONET_LoPathAlarms(TransactionIdentifier, iDevice, pLineEndpoint);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	
}





U8 OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;

	OMIINO_RAM.QC[QC_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CREATEFACILITY]++;

    Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, pLineEndpoint);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_MODE);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_TX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_J1_EX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_B3_SIGNAL_DEGRADE_THRESHOLD);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_B3_SIGNAL_FAIL_THRESHOLD);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_TX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_HO_PATH_C2_EX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
				pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
				OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_HoPathAlarms(TransactionIdentifier, iDevice, pDevicePortRAM, pLineEndpoint);
			}
		}
		else
		{
			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_TX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_LO_PATH_J2_EX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_TX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, OMIINO_DEFAULT_SONET_SDH_LO_PATH_SIGNAL_LABEL_EX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
				pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
				OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_LoPathAlarms(TransactionIdentifier, iDevice, pDevicePortRAM, pLineEndpoint);
			}
		}
	}

	return Result;
}





void OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(U32 TransactionIdentifier, U8 iDevice, U8 AlarmCategory, U32 Key, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	U8 Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_SONET_SDH_PathAlarmKey(TransactionIdentifier, AlarmCategory, Key, pEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}
}

void OMIINO_FRAMER_STATUS_Send_SDH_LO_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_TIM, pLO_Path->Overhead.J2.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_EXC, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LP_DEG, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SDH_LP_RDI, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[2], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SDH_TU_AIS, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[3], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SDH_TU_LOP, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[4], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SDH_LP_PLM, pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SDH_LP_UNEQ,pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[1],  pEndpoint);
}

void OMIINO_FRAMER_STATUS_Send_SONET_LO_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{	
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_TIM_V, pLO_Path->Overhead.J2.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_EXC_V, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_DEG_V, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_RDI_V, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[2], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_AIS_V, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[3], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_LOP_V, pLO_Path->Overhead.V5_BIP2.Status.Alarms.Key[4], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_PLM_V, pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice,  WPX_UFE_FRAMER_SONET_UNEQ_V,pLO_Path->Overhead.V5_SignalLabel.Status.Alarms.Key[1],  pEndpoint);
}

void OMIINO_FRAMER_STATUS_Send_SDH_HO_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_RDI, pHO_Path->Overhead.G1.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_EXC, pHO_Path->Overhead.B3.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_DEG, pHO_Path->Overhead.B3.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_TIM, pHO_Path->Overhead.J1.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_PLM, pHO_Path->Overhead.C2.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_UNEQ, pHO_Path->Overhead.C2.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_AU_AIS, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_AU_LOP, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_HP_LOM, pHO_Path->Overhead.H4.Status.Alarms.Key[0], pEndpoint);

}

void OMIINO_FRAMER_STATUS_Send_SONET_HO_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_RDI_P, pHO_Path->Overhead.G1.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_EXC_P, pHO_Path->Overhead.B3.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_DEG_P, pHO_Path->Overhead.B3.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_TIM_P, pHO_Path->Overhead.J1.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_PLM_P, pHO_Path->Overhead.C2.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_UNEQ_P,pHO_Path->Overhead.C2.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_AIS_P, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[0], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOP_P, pHO_Path->Overhead.H1H2H3.Status.Alarms.Key[1], pEndpoint);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PathAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOM_P, pHO_Path->Overhead.H4.Status.Alarms.Key[0], pEndpoint);
}

void OMIINO_FRAMER_STATUS_Send_HO_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM, OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE * pHO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	U8 PortMode;

 	OMIINO_FRAMER_ASSERT(NULL!=pRAM,0);

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_Send_SDH_HO_AlarmKeys(TransactionIdentifier, iDevice, pHO_Path, pEndpoint);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_Send_SONET_HO_AlarmKeys(TransactionIdentifier, iDevice, pHO_Path, pEndpoint);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }
}



void OMIINO_FRAMER_STATUS_Send_LO_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM, OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE * pLO_Path, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pEndpoint)
{
	U8 PortMode;

 	OMIINO_FRAMER_ASSERT(NULL!=pRAM,0);

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_Send_SDH_LO_AlarmKeys(TransactionIdentifier, iDevice, pLO_Path, pEndpoint);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_Send_SONET_LO_AlarmKeys(TransactionIdentifier, iDevice, pLO_Path, pEndpoint);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_CREATEFACILITY]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(&(pDevicePortRAM->Configuration), pLineEndpoint->Bandwidth);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsRequiredBandwidth(&(pDevicePortRAM->Configuration), pLineEndpoint->Bandwidth);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(OMIINO_FRAMER_Is_SONET_Endpoint(pLineEndpoint))
			{
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_iU_Valid_ForConfiguredPort(&(pDevicePortRAM->Configuration), pLineEndpoint->u.sonet.iU);
			}
			else
			{
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_iJ_Valid_ForConfiguredPort(&(pDevicePortRAM->Configuration), pLineEndpoint->u.sdh.iJ);
			}
		} 

		if(WPX_UFE_FRAMER_OK==Result)
		{
			switch(pLineEndpoint->Bandwidth)
			{    
				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC4_4C(pDeviceRAM, pLineEndpoint);
						break;

				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC4(pDeviceRAM, pLineEndpoint);
						break;

				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC3_HI(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS48C(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS12C(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS3C(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_STS1(pDeviceRAM, pLineEndpoint);
						break;

				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_VC3_LO(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SDH_LO(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_CreateFacility_SONET_LO(pDeviceRAM, pLineEndpoint);
						break;

				default:
						OMIINO_FRAMER_RSE(pLineEndpoint->Bandwidth);
						break;
			}
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
			{
				pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

				Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
					OMIINO_FRAMER_SONET_SDH_HO_Facility_Create(pDeviceHO_PathRAM, pLineEndpoint->Bandwidth, pLineEndpoint);
					OMIINO_FRAMER_STATUS_Initialize_SONET_SDH_HO_PATH(iDevice, pDevicePortRAM, pDeviceHO_PathRAM, pLineEndpoint, &(pDeviceRAM->Alarms));
					OMIINO_FRAMER_STATUS_Given_HO_PATH_ResetAlarmState(iDevice, pDeviceHO_PathRAM);
				}
			}
			else
			{
				pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
				OMIINO_FRAMER_SONET_SDH_LO_Facility_Create(pDeviceLO_PathRAM, pLineEndpoint->Bandwidth);
				OMIINO_FRAMER_SONET_SDH_InitializePort_LO_PATH_Overhead(&(pDeviceLO_PathRAM->Overhead));
				OMIINO_FRAMER_STATUS_Initialize_SONET_SDH_LO_PATH(iDevice, pDevicePortRAM, pDeviceLO_PathRAM, pLineEndpoint, &(pDeviceRAM->Alarms));
				OMIINO_FRAMER_STATUS_Given_LO_PATH_ResetAlarmState(iDevice, pDeviceLO_PathRAM);
			}
		}
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, pLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				OMIINO_FRAMER_STATUS_Send_HO_AlarmKeys(TransactionIdentifier, iDevice, pDevicePortRAM, pDeviceHO_PathRAM, pLineEndpoint);
			}
		}
		else
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			OMIINO_FRAMER_STATUS_Send_LO_AlarmKeys(TransactionIdentifier, iDevice, pDevicePortRAM, pDeviceLO_PathRAM, pLineEndpoint);
		}
	}

	return Result;
}





void OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(U32 TransactionIdentifier, U8 iDevice, U8 AlarmCategory, U32 Key, U32 iSocketClientPDH)
{
	U8 Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_SocketClientAlarmKey(TransactionIdentifier, AlarmCategory, iSocketClientPDH, Key, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClientType)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;
	OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClientStatus;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CREATEFACILITY]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
        
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_TypeInRange(ClientType);		
    }
        
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketIsCompatibleWithType(iSocketClientPDH, ClientType);		
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0==OMIINO_DEVICE_PERSONALITY_SupportsClientType(iDevice, ClientType))
		{
			Result=WPX_UFE_FRAMER_ERROR_CLIENT_TYPE_NOT_SUPPORTED;
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityDoesNotExist(pClient);


		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketIsCompatibleWithPeers(iDevice, iSocketClientPDH, ClientType);
		}


		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketIsBlockedByOther(iDevice, iSocketClientPDH, ClientType);
		}


		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Create(pClient, ClientType);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
           
           pClientStatus = OMIINO_FRAMER_GetStatusRAMForSocketClientPDH_Element(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice), iSocketClientPDH);
           
           
           if (ClientType == WPX_UFE_FRAMER_CLIENT_T3)
           {
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC], iSocketClientPDH);
           }
           else
           {
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E_RFI], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_A_RAI], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_LOS_AIS], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_OOF_LOF], iSocketClientPDH);
           }
           
           Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_CreateFacility(TransactionIdentifier, iSocketClientPDH, ClientType, OMIINO_RAM.Device[iDevice].LLC.Buffer);
           
           OMIINO_FRAMER_STATUS_Given_SocketClient_ResetAlarmState(iDevice, pClientStatus, ClientType);
           if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
           {
              OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
           }
           
           if (ClientType == WPX_UFE_FRAMER_CLIENT_T3)
           {
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_XA], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_AIS], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_LOF], iSocketClientPDH);
              OMIINO_FRAMER_STATUS_Send_SocketClientAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC, pClientStatus->Alarms.Key[WPX_UFE_FRAMER_SOCKET_CLIENT_E3T3_FEAC], iSocketClientPDH);
           }         
           
           OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableTimestamping(TransactionIdentifier, iDevice, iSocketClientPDH);
           
	}
        
	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClientType)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE	* pDiscretePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CREATEFACILITY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	} 
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_TypeInRange(ClientType);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDiscretePortRAM = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityDoesNotExist(pDiscretePortRAM);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Facility_Create(pDiscretePortRAM, ClientType);
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_CreateFacility(TransactionIdentifier, iDiscreteClientPort, ClientType, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM=NULL;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_DELETEFACILITY]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(OMIINO_FRAMER_Is_SONET_Endpoint(pLineEndpoint))
			{
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_iU_Valid_ForConfiguredPort(&(pDevicePortRAM->Configuration), pLineEndpoint->u.sonet.iU);
			}
			else
			{
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_iJ_Valid_ForConfiguredPort(&(pDevicePortRAM->Configuration), pLineEndpoint->u.sdh.iJ);
			}
		} 

		if(WPX_UFE_FRAMER_OK==Result)
		{
			switch(pLineEndpoint->Bandwidth)
			{    
				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC4_4C(pDeviceRAM, pLineEndpoint);
						break;

				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC4(pDeviceRAM, pLineEndpoint);
						break;

				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC3_HI(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS48C(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS12C(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS3C(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_STS1(pDeviceRAM, pLineEndpoint);
						break;

				case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_VC3_LO(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SDH_LO(pDeviceRAM, pLineEndpoint);
						break;

				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
						Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_DeleteFacility_SONET_LO(pDeviceRAM, pLineEndpoint);
						break;

				default:
						OMIINO_FRAMER_RSE(pLineEndpoint->Bandwidth);
						break;
			}
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
			{
				Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

					OMIINO_FRAMER_STATUS_Clear_This_HO_PathAlarms(iDevice, pDeviceHO_PathRAM);
					OMIINO_FRAMER_SONET_SDH_HO_Facility_Delete(pDeviceHO_PathRAM);
					OMIINO_FRAMER_STATUS_FreeAlarmKeysFor_SONET_SDH_HO_PATH(iDevice, pDeviceHO_PathRAM, &(pDeviceRAM->Alarms));
				}
			}
			else
			{
				pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

				OMIINO_FRAMER_STATUS_Clear_This_LO_PathAlarms(iDevice, pDeviceLO_PathRAM);
				OMIINO_FRAMER_SONET_SDH_LO_Facility_Delete(pDeviceLO_PathRAM);
				OMIINO_FRAMER_STATUS_FreeAlarmKeysFor_SONET_SDH_LO_PATH(iDevice, pDeviceLO_PathRAM, &(pDeviceRAM->Alarms));
			}
		}
	}




	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, pLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;
	OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClientStatus;
	U8 ClientType;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DELETEFACILITY]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		if(WPX_UFE_FRAMER_FACILITY_STATE_CREATED_FREE!=OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_GetState(pClient))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_PART_OF_CONNECTION;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);


		pClientStatus = OMIINO_FRAMER_GetStatusRAMForSocketClientPDH_Element(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice), iSocketClientPDH);

		OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetClientType(pClient, &ClientType);
		OMIINO_FRAMER_STATUS_Clear_This_SocketClient_Alarms(iDevice, pClientStatus, ClientType);

		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Facility_Delete(pClient);
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DeleteFacility(TransactionIdentifier, iSocketClientPDH, ClientType, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE	* pDiscretePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DELETEFACILITY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDiscretePortRAM = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pDiscretePortRAM);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Facility_Delete(pDiscretePortRAM);
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DeleteFacility(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT
 *
 */


void OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(U32 TransactionIdentifier, U8 iDevice, U8 AlarmCategory, U32 Key, U8 iLinePort)
{
	U8 Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_SONET_SDH_PortAlarmKey(TransactionIdentifier, AlarmCategory, Key, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
	if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
	{
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
	}
}



void OMIINO_FRAMER_STATUS_Send_SDH_Port_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM_Port, U8 iLinePort)
{
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LOS,    pRAM_Port->Section.A1A2.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_LOF,    pRAM_Port->Section.A1A2.Status.Alarms.Key[1], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_OOF,    pRAM_Port->Section.A1A2.Status.Alarms.Key[2], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_RS_TIM, pRAM_Port->Section.J0.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_RS_EXC, pRAM_Port->Section.B1.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_RS_DEG, pRAM_Port->Section.B1.Status.Alarms.Key[1], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_MS_AIS, pRAM_Port->Line.K1K2.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_MS_RDI, pRAM_Port->Line.K1K2.Status.Alarms.Key[1], iLinePort);

	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_MS_EXC, pRAM_Port->Line.B2.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SDH_MS_DEG, pRAM_Port->Line.B2.Status.Alarms.Key[1], iLinePort);
}


void OMIINO_FRAMER_STATUS_Send_SONET_Port_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM_Port, U8 iLinePort)
{
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOS,   pRAM_Port->Section.A1A2.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_LOF,   pRAM_Port->Section.A1A2.Status.Alarms.Key[1], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_OOF,   pRAM_Port->Section.A1A2.Status.Alarms.Key[2], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_TIM_S, pRAM_Port->Section.J0.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_EXC_S, pRAM_Port->Section.B1.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_DEG_S, pRAM_Port->Section.B1.Status.Alarms.Key[1], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_EXC_L, pRAM_Port->Line.B2.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_DEG_L, pRAM_Port->Line.B2.Status.Alarms.Key[1], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_AIS_L, pRAM_Port->Line.K1K2.Status.Alarms.Key[0], iLinePort);
	OMIINO_FRAMER_STATUS_Send_SONET_SDH_PortAlarmKey(TransactionIdentifier, iDevice, WPX_UFE_FRAMER_SONET_RDI_L, pRAM_Port->Line.K1K2.Status.Alarms.Key[1], iLinePort);
}



void OMIINO_FRAMER_STATUS_Send_Port_AlarmKeys(U32 TransactionIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM_Port, U8 iLinePort)
{
	U8 PortMode;

 	OMIINO_FRAMER_ASSERT(NULL!=pRAM_Port,0);

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM_Port->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_Send_SDH_Port_AlarmKeys(TransactionIdentifier, iDevice, pRAM_Port, iLinePort);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_Send_SONET_Port_AlarmKeys(TransactionIdentifier, iDevice, pRAM_Port, iLinePort);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }
}




void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(U32 TransactionIdentifier, U8 AnyAlarmCategory)
{
	U8 Result;
	U8 ReportingEnabled;

	Result=OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GetAlarmReportingForAlarmCategory(AnyAlarmCategory, &ReportingEnabled);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==ReportingEnabled)
		{
			OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AnyAlarmCategory);
		}
	}
}



void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SDH_PortAlarms(U32 TransactionIdentifier)
{
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_LOS);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_LOF);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_RS_TIM);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_MS_AIS);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_OOF);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_MS_EXC);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_MS_DEG);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_MS_RDI);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_RS_EXC);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SDH_RS_DEG);
}

void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SONET_PortAlarms(U32 TransactionIdentifier)
{
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_LOS);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_LOF);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_TIM_S);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_AIS_L);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_RDI_L);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_OOF);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_EXC_L);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_DEG_L);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_EXC_S);
	OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_ThisPortAlarmIfRequired(TransactionIdentifier,WPX_UFE_FRAMER_SONET_DEG_S);
}





void OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_PortAlarms(U32 TransactionIdentifier, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM)
{
	U8 PortMode;

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SDH_PortAlarms(TransactionIdentifier);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_SONET_PortAlarms(TransactionIdentifier);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	
}












void OMIINO_FRAMER_STATUS_Clear_SDH_PortAlarmsForPort(U8 iDevice, U8 iLinePort)
{
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.A1A2.Status.Alarms.Key[0] ); /* WPX_UFE_FRAMER_SDH_LOS     */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.A1A2.Status.Alarms.Key[1] ); /* WPX_UFE_FRAMER_SDH_LOF     */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.A1A2.Status.Alarms.Key[2] ); /* WPX_UFE_FRAMER_SDH_OOF     */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.J0.Status.Alarms.Key[0]   ); /* WPX_UFE_FRAMER_SDH_RS_TIM  */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.K1K2.Status.Alarms.Key[0]    ); /* WPX_UFE_FRAMER_SDH_MS_AIS  */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.K1K2.Status.Alarms.Key[1]      ); /* WPX_UFE_FRAMER_SDH_MS_RDI  */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.B2.Status.Alarms.Key[0]      ); /* WPX_UFE_FRAMER_SDH_MS_EXC  */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.B2.Status.Alarms.Key[1]      ); /* WPX_UFE_FRAMER_SDH_MS_DEG  */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.B1.Status.Alarms.Key[0]   ); /* WPX_UFE_FRAMER_SDH_RS_EXC  */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.B1.Status.Alarms.Key[1]   ); /* WPX_UFE_FRAMER_SDH_RS_DEG  */
}




void OMIINO_FRAMER_STATUS_Clear_SONET_PortAlarmsForPort(U8 iDevice, U8 iLinePort)
{
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.A1A2.Status.Alarms.Key[0]);	/* WPX_UFE_FRAMER_SONET_LOS   */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.A1A2.Status.Alarms.Key[1]);	/* WPX_UFE_FRAMER_SONET_LOF   */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.A1A2.Status.Alarms.Key[2]);	/* WPX_UFE_FRAMER_SONET_OOF   */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.J0.Status.Alarms.Key[0]);		/* WPX_UFE_FRAMER_SONET_TIM_S */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.B1.Status.Alarms.Key[0]);		/* WPX_UFE_FRAMER_SONET_EXC_S */
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Section.B1.Status.Alarms.Key[1]);		/* WPX_UFE_FRAMER_SONET_DEG_S */ 
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.B2.Status.Alarms.Key[0]);		/* WPX_UFE_FRAMER_SONET_EXC_L */ 
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.B2.Status.Alarms.Key[1]);		/* WPX_UFE_FRAMER_SONET_DEG_L */ 
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.K1K2.Status.Alarms.Key[0]);		/* WPX_UFE_FRAMER_SONET_AIS_L */ 
	OMIINO_FRAMER_STATUS_Clear_ThisAlarmIfRequired(iDevice, OMIINO_RAM.Device[iDevice].Hierarchy.SONET_SDH_Port[iLinePort].Line.K1K2.Status.Alarms.Key[1]);		/* WPX_UFE_FRAMER_SONET_RDI_L */ 

}


void OMIINO_FRAMER_STATUS_Clear_PortAlarmsForPort(U8 iDevice, U8 iLinePort, OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pRAM)
{
	U8 PortMode;

	OMIINO_FRAMER_SONET_SDH_Port_GetMode(&(pRAM->Configuration), &PortMode);

    switch(PortMode)
    {
        case WPX_UFE_FRAMER_DEVICE_MODE_SDH:
			OMIINO_FRAMER_STATUS_Clear_SDH_PortAlarmsForPort(iDevice, iLinePort);
            break;

        case WPX_UFE_FRAMER_DEVICE_MODE_SONET:
			OMIINO_FRAMER_STATUS_Clear_SONET_PortAlarmsForPort(iDevice, iLinePort);
            break;

        default:
            OMIINO_FRAMER_RSE(PortMode);
            break;
    }	
}





void OMIINO_DefaultAllPortsForDevice(U8 iDevice)
{
    U8  iLogicalLinePort;

	for(iLogicalLinePort=0;iLogicalLinePort<WPX_UFE_FRAMER_BUILD_OPTION_MAX_LINE_SIDE_PORTS;iLogicalLinePort++)
	{
		OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(0, iDevice, iLogicalLinePort, WPX_UFE_FRAMER_LINE_PORT_RATE_OFF,WPX_UFE_FRAMER_SFP_TX_ENABLE);
	}
}





U8 OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate,U8 Tx_Enable)
{
	U8													Result = WPX_UFE_FRAMER_OK;
	U8													Logical_ProtectionPartner_iLinePort;
	WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE		TX_K1K2;
    OMIINO_FRAMER_HIERARCHY_TYPE *						pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE *		pDevicePortRAM;

	OMIINO_RAM.QC[QC_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SETRATE]++;

	Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(TransactionIdentifier, iDevice, iLinePort, PortRate,Tx_Enable);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_LINE_PORT_RATE_OFF==PortRate)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_STATUS_Clear_PortAlarmsForPort(iDevice, iLinePort, pDevicePortRAM);
			if(OMIINO_DEVICE_PERSONALITY_SupportsLineProtection(iDevice))
			{
				if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLinePort, &Logical_ProtectionPartner_iLinePort))
				{
					pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
					pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, Logical_ProtectionPartner_iLinePort);
					OMIINO_FRAMER_STATUS_Clear_PortAlarmsForPort(iDevice, Logical_ProtectionPartner_iLinePort, pDevicePortRAM);
				}
			}
		}
		else
		{
			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_MODE);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetTX(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_TX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_J0_EX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				TX_K1K2.K1=OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_K1_TX;
				TX_K1K2.K2=OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_K2_TX;

				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(TransactionIdentifier, iDevice, iLinePort, &TX_K1K2);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_S1_TX);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_B1_SIGNAL_FAIL_THRESHOLD);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_B2_SIGNAL_FAIL_THRESHOLD);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_SECTION_B1_SIGNAL_DEGRADE_THRESHOLD);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, iLinePort, OMIINO_DEFAULT_SONET_SDH_LINE_PORT_LINE_B2_SIGNAL_DEGRADE_THRESHOLD);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
				pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
				OMIINO_FRAMER_STATUS_Send_Port_AlarmKeys(TransactionIdentifier, iDevice, pDevicePortRAM, iLinePort);

				if(OMIINO_DEVICE_PERSONALITY_SupportsLineProtection(iDevice))
				{
					if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Logical_ProtectionPartner_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLinePort, &Logical_ProtectionPartner_iLinePort))
					{
						pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
						pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, Logical_ProtectionPartner_iLinePort);
						OMIINO_FRAMER_STATUS_Send_Port_AlarmKeys(TransactionIdentifier, iDevice, pDevicePortRAM, Logical_ProtectionPartner_iLinePort);
					}
				}
				OMIINO_FRAMER_STATUS_RefreshAlarmReportingFor_PortAlarms(TransactionIdentifier, pDevicePortRAM);
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort,U8 Tx_State)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE			* pDevicePortRAM;


	OMIINO_RAM.QC[QC_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SETSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_SFP_StateInRange(Tx_State);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
    {
        Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(iDevice, iLinePort);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
    }
    

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState(TransactionIdentifier, iLinePort,Tx_State, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 PortRate,U8 Tx_Enable)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE			* pDevicePortRAM;
	U8													CurrentPortRate;


	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SETRATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_RateInRange(PortRate);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(iDevice, iLinePort);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!OMIINO_DEVICE_PERSONALITY_SupportsPortRate(iDevice, PortRate))
		{
			Result=WPX_UFE_FRAMER_ERROR_PERSONALITY_PREVENTS_PORT_RATE;
		}
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_ModeSupportsRate(&(pDevicePortRAM->Configuration), PortRate);

    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_FRAMER_SONET_SDH_Port_Has_Facilities(iDevice, iLinePort))
		{
			Result=WPX_UFE_FRAMER_ERROR_PORT_HAS_FACILITIES;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetRate(iDevice, iLinePort, &CurrentPortRate);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			if(CurrentPortRate==PortRate)
			{
				Result=WPX_UFE_FRAMER_ERROR_PORT_THIS_PORT_RATE_ALREADY_CONFIGURED;
			}
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_SONET_SDH_Port_SetRate(&(pDevicePortRAM->Configuration), PortRate);

			if(WPX_UFE_FRAMER_LINE_PORT_RATE_OFF==PortRate)
			{
				OMIINO_FRAMER_CONFIGURATION_InitializeFor_SONET_SDH_Port(pDevicePortRAM);
			}
		}
	}
    

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallow(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(TransactionIdentifier, iLinePort, WPX_UFE_FRAMER_LINE_PORT_RATE_OFF,Tx_Enable, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}

        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(TransactionIdentifier, iLinePort, PortRate,Tx_Enable, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetRate(U8 iDevice, U8 iLinePort, U8 * pPortRate)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GETRATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(iDevice, iLinePort);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
		OMIINO_FRAMER_SONET_SDH_Port_GetRate(&(pDevicePortRAM->Configuration), pPortRate);
	}

	return Result;
}









U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Enable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 DCC_Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE			* pDevicePortRAM;


	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_ENABLE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_DCC_InRange(DCC_Mode);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(iDevice, iLinePort);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
		OMIINO_FRAMER_SONET_SDH_Port_Set_DCC(&(pDevicePortRAM->Configuration), DCC_Mode, 1);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Enable(TransactionIdentifier, iLinePort, DCC_Mode,OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
	return Result;
}





U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Disable(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 DCC_Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE			* pDevicePortRAM;


	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_DISABLE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}
    
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_DCC_InRange(DCC_Mode);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }
		
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(iDevice, iLinePort);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
		OMIINO_FRAMER_SONET_SDH_Port_Set_DCC(&(pDevicePortRAM->Configuration), DCC_Mode, 0);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Disable(TransactionIdentifier, iLinePort, DCC_Mode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
	return Result;
}









U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Get(U8 iDevice, U8 iLinePort, U8 DCC_Mode, U8 * pDCC_State)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_PortRateCanBeConfigured(iDevice, iLinePort);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
		OMIINO_FRAMER_SONET_SDH_Port_Get_DCC(&(pDevicePortRAM->Configuration), DCC_Mode, pDCC_State);
	}

	return Result;
}









U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_EnableScrambling(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_ENABLESCRAMBLING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_EnableScrambling(&(pDevicePortRAM->Configuration));
		}
    }

			
	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_EnableScrambling(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DisableScrambling(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DISABLESCRAMBLING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_DisableScrambling(&(pDevicePortRAM->Configuration));
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DisableScrambling(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetScramblingState(U8 iDevice, U8 iLinePort, U8 * pScramblingIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GETSCRAMBLINGSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_GetScrambling(&(pDevicePortRAM->Configuration), pScramblingIsEnabled);	
		}
    }

    return Result;
}




/*
 *
 * Section: PORT PROTECTION
 *
 */


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_A(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;
	
	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_A]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_Force_A(&(pDevicePortRAM->Configuration.Protection));
		}
    }

			
	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_A(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
	
	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_B(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_FORCE_B]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_Force_B(&(pDevicePortRAM->Configuration.Protection));
		}
    }

			
	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_B(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
	
	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetForceState(U8 iDevice, U8 iLinePort, U8 * pForceSide)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GETFORCESTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_GetForceState(&(pDevicePortRAM->Configuration.Protection), pForceSide);	
		}
    }

    return Result;
}




/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT SECTION
 *
 */




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE	* pDevicePort_J0_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SETMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_J0_ModeInRange(Mode);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_J0_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(pDevicePortRAM);
		    OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetMode(pDevicePort_J0_RAM, Mode);
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(TransactionIdentifier, iLinePort, Mode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetMode(U8 iDevice, U8 iLinePort, U8 * pMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE	* pDevicePort_J0_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GETMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pMode);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}



    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_J0_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetMode(pDevicePort_J0_RAM, pMode);
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pTX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE	* pDevicePort_J0_RAM;
	char J0_TX_WithCRC[WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1];

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pTX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }
	

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_J0_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetTX(pDevicePort_J0_RAM, pTX_TraceString);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetTX_WithCRC(pDevicePort_J0_RAM, J0_TX_WithCRC);
			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetTX(TransactionIdentifier, iLinePort, J0_TX_WithCRC, OMIINO_RAM.Device[iDevice].LLC.Buffer);
			if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
			}
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, char * pEX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE	* pDevicePort_J0_RAM;
	char J0_EX_WithCRC[WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1];

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_J0_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetEX(pDevicePort_J0_RAM, pEX_TraceString);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetEX_WithCRC(pDevicePort_J0_RAM, J0_EX_WithCRC);
			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(TransactionIdentifier, iLinePort, J0_EX_WithCRC, OMIINO_RAM.Device[iDevice].LLC.Buffer);
			if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
			}
		}
	}


	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetTX(U8 iDevice, U8 iLinePort, char * pTX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE	* pDevicePort_J0_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pTX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_J0_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetTX(pDevicePort_J0_RAM, pTX_TraceString);
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetEX(U8 iDevice, U8 iLinePort, char * pEX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_SECTION_J0_TYPE	* pDevicePort_J0_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_J0_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortJ0(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetEX(pDevicePort_J0_RAM, pEX_TraceString);
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SETEXCESSIVETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_B1_ExcessiveThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B1_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB1(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(pDevicePort_B1_RAM, Threshold);
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(TransactionIdentifier, iLinePort, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_SectionOverhead_StaticRule_B1_DegradeThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B1_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB1(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(pDevicePort_B1_RAM, Threshold);
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(TransactionIdentifier, iLinePort, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GetExcessive(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GETEXCESSIVE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B1_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB1(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetExcessiveThreshold(pDevicePort_B1_RAM, pThreshold);
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GetSignalDegradeThreshold(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B1_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB1(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetSignalDegradeThreshold(pDevicePort_B1_RAM, pThreshold);
		}
    }

	return Result;
}





/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH PORT LINE
 *
 */




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_K1K2_TYPE	* pDevicePort_K1K2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_K1K2_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortK1K2(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_SetTX(pDevicePort_K1K2_RAM, pTX_K1K2);
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(TransactionIdentifier, iLinePort, pTX_K1K2, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GetTX(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_K1K2_TYPE	* pDevicePort_K1K2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_K1K2_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortK1K2(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_GetTX(pDevicePort_K1K2_RAM, pTX_K1K2);
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 TX_S1)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE			* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_S1_TYPE * pDevicePort_S1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_S1_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortS1(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Line_S1_SetTX(pDevicePort_S1_RAM, TX_S1);
		}
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(TransactionIdentifier, iLinePort, TX_S1, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_SIGNALLING, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GetTX(U8 iDevice, U8 iLinePort, U8 * pTX_S1)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE						* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE			* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_LINE_S1_TYPE * pDevicePort_S1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_S1_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortS1(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_Port_Line_S1_GetTX(pDevicePort_S1_RAM, pTX_S1);
		}
    }

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SETEXCESSIVETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineOverhead_StaticRule_B2_ExcessiveThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B2_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB2(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(pDevicePort_B2_RAM, Threshold);
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(TransactionIdentifier, iLinePort, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineOverhead_StaticRule_B2_DegradeThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
	
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B2_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB2(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(pDevicePort_B2_RAM, Threshold);
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(TransactionIdentifier, iLinePort, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GetExcessive(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GETEXCESSIVE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B2_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB2(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetExcessiveThreshold(pDevicePort_B2_RAM, pThreshold);
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GetSignalDegradeThreshold(U8 iDevice, U8 iLinePort, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePort_B2_RAM = OMIINO_FRAMER_GetConfigurationRAMForLinePortB2(pDevicePortRAM);
			OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetSignalDegradeThreshold(pDevicePort_B2_RAM, pThreshold);
		}
    }

	return Result;
}







/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH HO PATH
 *
 */



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Mode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE		* pDevicePort_J1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SETMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_J1_ModeInRange(Mode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J1_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(&(pDeviceHO_PathRAM->Overhead));
				OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetMode(pDevicePort_J1_RAM, Mode);
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(TransactionIdentifier, pLineEndpoint, Mode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetMode(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE		* pDevicePort_J1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GETMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pMode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J1_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(&(pDeviceHO_PathRAM->Overhead));
				OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetMode(pDevicePort_J1_RAM, pMode);
			}
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE	* pDevicePort_J1_RAM;
	char															J1_TX_WithCRC[WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1];

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pTX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J1_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetTX(pDevicePort_J1_RAM, pTX_TraceString);

					OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetTX_WithCRC(pDevicePort_J1_RAM, J1_TX_WithCRC);
					Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, pLineEndpoint, J1_TX_WithCRC, OMIINO_RAM.Device[iDevice].LLC.Buffer);
					if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
					{
						OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
					}
				}
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE	* pDevicePort_J1_RAM;
	char															J1_EX_WithCRC[WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1];

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J1_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetEX(pDevicePort_J1_RAM, pEX_TraceString);

					OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetEX_WithCRC(pDevicePort_J1_RAM, J1_EX_WithCRC);
					Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, pLineEndpoint, J1_EX_WithCRC, OMIINO_RAM.Device[iDevice].LLC.Buffer);
					if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
					{
						OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
					}
				}
			}
		}
    }

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE		* pDevicePort_J1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pTX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J1_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetTX(pDevicePort_J1_RAM, pTX_TraceString);
				}
			}
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE		* pDevicePort_J1_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{	
				pDevicePort_J1_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_J1(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetEX(pDevicePort_J1_RAM, pEX_TraceString);
				}
			}
		}
	}

	return Result;
}

U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SetRdiMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 ERDI_MODE)
{

   U8 Result = WPX_UFE_FRAMER_OK;
   U8 Length;
   OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
   OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
   OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;

   OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_ERDI_SETMODE]++;

   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
/*
  check that the RDI MODE parameter that was passed to the function is valid.
  the valid values are : 
  WPX_UFE_FRAMER_RDI_MODE_ENABLE
  WPX_UFE_FRAMER_ERDI_MODE_ENABLE
*/
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_G1_ErdiModeInRange(ERDI_MODE);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
      {
         Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
      }
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

      Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

      if(WPX_UFE_FRAMER_OK==Result)
      {
         Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
         if(WPX_UFE_FRAMER_OK==Result)
         {
            pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
            Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
         }

      }
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SetRdiMode(TransactionIdentifier, pLineEndpoint, ERDI_MODE, OMIINO_RAM.Device[iDevice].LLC.Buffer);
      if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
      {
         OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
      }
   }

   return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Value)
{

   U8 Result = WPX_UFE_FRAMER_OK;
   U8 Length;
   OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
   OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
   OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;

   OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX]++;

   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
/*
  check that the Value parameter that was passed to the function is valid.
  the valid values are : 
  0x0,0x1,0x2 and 0x3
*/
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_SS_BITS_TxValueInRange(Value);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
      {
         Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
      }
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

      Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

      if(WPX_UFE_FRAMER_OK==Result)
      {
         Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
         if(WPX_UFE_FRAMER_OK==Result)
         {
            pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
            Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
         }

      }
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SetTX(TransactionIdentifier, pLineEndpoint, Value, OMIINO_RAM.Device[iDevice].LLC.Buffer);
      if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
      {
         OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
      }
   }

   return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{

   U8 Result = WPX_UFE_FRAMER_OK;
   U8 Length;
   OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
   OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
   OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;

   OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX]++;

   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
      {
         Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
      }
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

      Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

      if(WPX_UFE_FRAMER_OK==Result)
      {
         Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
         if(WPX_UFE_FRAMER_OK==Result)
         {
            pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
            Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
         }

      }
   }

   if(WPX_UFE_FRAMER_OK==Result)
   {
      Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GetRX(TransactionIdentifier, pLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
      if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
      {
         OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
      }
   }

   return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_C2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_C2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{	
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_C2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_C2(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetTX(pDevicePort_C2_RAM, TX_C2);	
				}
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, pLineEndpoint, TX_C2, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 EX_C2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_C2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_C2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_C2(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetEX(pDevicePort_C2_RAM, EX_C2);	
				}
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, pLineEndpoint, EX_C2, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pTX_C2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_C2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pTX_C2);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_C2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_C2(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_GetTX(pDevicePort_C2_RAM, pTX_C2);
				}
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pEX_C2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_C2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pEX_C2);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_C2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_C2(&(pDeviceHO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_GetEX(pDevicePort_C2_RAM, pEX_C2);
				}
			}
		}
    }

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B3_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SETEXCESSIVETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_B3_ExcessiveThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_B3_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_B3(&(pDeviceHO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(pDevicePort_B3_RAM, Threshold);
				}
				
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(TransactionIdentifier, pLineEndpoint, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B3_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_HO_Overhead_StaticRule_B3_DegradeThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_B3_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_B3(&(pDeviceHO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(pDevicePort_B3_RAM, Threshold);
				}
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(TransactionIdentifier, pLineEndpoint, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GetExcessiveThreshold(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B3_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GETEXCESSIVETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pThreshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_B3_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_B3(&(pDeviceHO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetExcessiveThreshold(pDevicePort_B3_RAM, pThreshold);
				}
			}
		}
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GetSignalDegradeThreshold(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_B3_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pThreshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(!(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth)))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_B3_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_HO_Path_B3(&(pDeviceHO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetSignalDegradeThreshold(pDevicePort_B3_RAM, pThreshold);	
				}
			}
		}
    }

	return Result;
}







/*
 *
 * Section: CONFIGURATION FACILITIES SONET/SDH LO PATH
 *
 */


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE	* pDevicePort_J2_RAM;
	char															J2_TX_WithCRC[WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1];

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pTX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_J2(&(pDeviceLO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{

					OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetTX(pDevicePort_J2_RAM, pTX_TraceString);
					
					OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetTX_WithCRC(pDevicePort_J2_RAM, J2_TX_WithCRC);
					Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, pLineEndpoint, J2_TX_WithCRC, OMIINO_RAM.Device[iDevice].LLC.Buffer);
					if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
					{
						OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
					}

				}
			}
		}
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE	* pDevicePort_J2_RAM;
	char															J2_EX_WithCRC[WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1];

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_J2(&(pDeviceLO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{

					OMIINO_FRAMER_SONET_SDH_LO_Path_J2_SetEX(pDevicePort_J2_RAM, pEX_TraceString);
					
					OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetEX_WithCRC(pDevicePort_J2_RAM, J2_EX_WithCRC);

					Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, pLineEndpoint, J2_EX_WithCRC, OMIINO_RAM.Device[iDevice].LLC.Buffer);
					if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
					{
						OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
					}
				}
			}
		}
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE		* pDevicePort_J2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pTX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_J2(&(pDeviceLO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetTX(pDevicePort_J2_RAM, pTX_TraceString);
				}
			}
		}
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_LO_PATH_OVERHEAD_J2_TYPE	* pDevicePort_J2_RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pEX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_J2_RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_J2(&(pDeviceLO_PathRAM->Overhead));

				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetEX(pDevicePort_J2_RAM, pEX_TraceString);
				}
			}
		}
    }

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_V5_SignalLabel)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_V5_SignalLabelRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNALLABEL_SETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_V5_StaticRule_InRange(TX_V5_SignalLabel);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}



    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_V5_SignalLabelRAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_SignalLabel(&(pDeviceLO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetTX(pDevicePort_V5_SignalLabelRAM, TX_V5_SignalLabel);
				}
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, pLineEndpoint, TX_V5_SignalLabel, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 EX_V5_SignalLabel)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_V5_SignalLabelRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNALLABEL_SETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_V5_StaticRule_InRange(EX_V5_SignalLabel);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_V5_SignalLabelRAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_SignalLabel(&(pDeviceLO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetEX(pDevicePort_V5_SignalLabelRAM, EX_V5_SignalLabel);
				}
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, pLineEndpoint, EX_V5_SignalLabel, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pTX_V5_SignalLabel)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_V5_SignalLabelRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNALLABEL_GETTX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pTX_V5_SignalLabel);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_V5_SignalLabelRAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_SignalLabel(&(pDeviceLO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_GetTX(pDevicePort_V5_SignalLabelRAM, pTX_V5_SignalLabel);
				}
			}
		}
    }

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pEX_V5_SignalLabel)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE	* pDevicePort_V5_SignalLabelRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNALLABEL_GETEX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pEX_V5_SignalLabel);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePort_V5_SignalLabelRAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_SignalLabel(&(pDeviceLO_PathRAM->Overhead));
					OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_GetEX(pDevicePort_V5_SignalLabelRAM, pEX_V5_SignalLabel);
				}
			}
		}
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_V5_BIP2RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SETEXCESSIVETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LO_Overhead_StaticRule_V5_BIP2_ThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_V5_BIP2RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_BIP2(&(pDeviceLO_PathRAM->Overhead));
				OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(pDevicePort_V5_BIP2RAM, Threshold);
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveThreshold(TransactionIdentifier, pLineEndpoint, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetSignalDegradeThreshold(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_V5_BIP2RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LO_Overhead_StaticRule_V5_BIP2_ThresholdInRange(Threshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_V5_BIP2RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_BIP2(&(pDeviceLO_PathRAM->Overhead));
				OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(pDevicePort_V5_BIP2RAM, Threshold);
			}
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetSignalDegradeThreshold(TransactionIdentifier, pLineEndpoint, Threshold, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveThreshold(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_V5_BIP2RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GETEXCESSIVETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pThreshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_V5_BIP2RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_BIP2(&(pDeviceLO_PathRAM->Overhead));
				OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetExcessiveThreshold(pDevicePort_V5_BIP2RAM, pThreshold);
			}
		}
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GetSignalDegradeThreshold(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE										* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE							* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE				* pDeviceLO_PathRAM;
    OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE	* pDevicePort_V5_BIP2RAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GETSIGNALDEGRADETHRESHOLD]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pThreshold);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePort_V5_BIP2RAM = OMIINO_FRAMER_GetConfigurationRAMFor_LO_Path_V5_BIP2(&(pDeviceLO_PathRAM->Overhead));
				OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetSignalDegradeThreshold(pDevicePort_V5_BIP2RAM, pThreshold);
			}
		}
    }

	return Result;
}













/*
 *
 * Section: CONFIGURATION FACILITIES SOCKET CLIENT PDH
 *
 */








U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetClientType(U8 iDevice, U32 iSocketClientPDH, U8 * pClientType)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GETCLIENTTYPE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pClientType);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetClientType(pClient, pClientType);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetFraming(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Framing)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SETFRAMING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_FramingInRange(Framing);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_FramingInRange(pClient, Framing);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetFraming(pClient, Framing);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetFraming(TransactionIdentifier, iSocketClientPDH, Framing, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetFraming(U8 iDevice, U32 iSocketClientPDH, U8 * pFraming)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GETFRAMING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pFraming);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetFraming(pClient, pFraming);
		}
	}

    return Result;
}





U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableTimestamping(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U32 Gain)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLETIMESTAMPING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_GainInRange(Gain);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);


		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetEnableTimestamping(pClient, Gain);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableTimestamping(TransactionIdentifier, iSocketClientPDH, Gain, pClient->ClientType, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}






U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableTimestamping(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U32 ClientType32=WPX_UFE_FRAMER_CLIENT_E1;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLETIMESTAMPING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
	

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		ClientType32=(U32)(pClient->ClientType);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetDisableTimestamping(pClient);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableTimestamping(TransactionIdentifier, iSocketClientPDH, ClientType32, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetTimeStamping(U8 iDevice, U32 iSocketClientPDH, U8 * pTimeStamping_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GETTIMESTAMPING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetTimestamping(pClient, pTimeStamping_IsEnabled);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetTimestampByteCount(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 AnyTimestampByteCount)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SETTIMESTAMPBYTECOUNT]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_TimestampByteCountInRange(AnyTimestampByteCount);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);


		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetTimestampByteCount(pClient, AnyTimestampByteCount);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetTimestampByteCount(TransactionIdentifier, iSocketClientPDH, AnyTimestampByteCount, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetTimestampByteCount(U8 iDevice, U32 iSocketClientPDH, U8 * pAnyTimestampByteCount)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GETTIMESTAMPBYTECOUNT]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetTimestampByteCount(pClient, pAnyTimestampByteCount);
		}
	}

	return Result;
}













U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Mapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SETMAPPING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_MappingInRange(Mapping);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_MappingInRange(pClient, Mapping);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetMapping(pClient, Mapping);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetMapping(TransactionIdentifier, iSocketClientPDH, Mapping, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetMapping(U8 iDevice, U32 iSocketClientPDH, U8 * pMapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GETMAPPING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pMapping);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetMapping(pClient, pMapping);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecoveryMode(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 ClockRecoveryMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SETCLOCKRECOVERYMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PortStaticRule_ClockRecoveryModeInRange(ClockRecoveryMode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_SetClockRecovery(pClient, ClockRecoveryMode);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_SetClockRecovery(TransactionIdentifier, iSocketClientPDH, ClockRecoveryMode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}

U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SetFEAC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 Code)
{
   U8 Result = WPX_UFE_FRAMER_OK;
   U8 Length;
   OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
   OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;
   
   OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SETFEAC]++;
   
   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
      
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_E3T3_SetFEAC(TransactionIdentifier, iSocketClientPDH, Code, OMIINO_RAM.Device[iDevice].LLC.Buffer);
      if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
      {
         OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
      }
   }
   
   return Result;
}

U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GetClockRecoveryMode(U8 iDevice, U32 iSocketClientPDH, U8 * pClockRecoveryMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GETCLOCKRECOVERYMODE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pClockRecoveryMode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_GetClockRecovery(pClient, pClockRecoveryMode);
		}
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_ENABLECAS]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_Supports_CAS(pClient);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Enable_CAS(pClient);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_EnableCAS(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DISABLECAS]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_Supports_CAS(pClient);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Disable_CAS(pClient);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DisableCAS(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_Get_CAS_State(U8 iDevice, U32 iSocketClientPDH, U8 * pCAS_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_GET_CAS_STATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pCAS_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_CAS_State(pClient, pCAS_IsEnabled);
		}
	}

    return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLETOWARDSSOCKET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsSocket(&(pClient->AIS_Control));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsSocket(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLETOWARDSSOCKET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsSocket(&(pClient->AIS_Control));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsSocket(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GetTowardsSocketState(U8 iDevice, U32 iSocketClientPDH, U8 * pAIS_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GETTOWARDSSOCKETSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
		
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pAIS_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_GetTowardsSocketState(&(pClient->AIS_Control), pAIS_IsEnabled);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_ENABLETOWARDSLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsLine(&(pClient->AIS_Control));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_EnableTowardsLine(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DISABLETOWARDSLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsLine(&(pClient->AIS_Control));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_DisableTowardsLine(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GetTowardsLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pAIS_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_AIS_CONTROL_GETTOWARDSLINESTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pAIS_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_GetTowardsLineState(&(pClient->AIS_Control), pAIS_IsEnabled);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{

	U8 Result = WPX_UFE_FRAMER_OK;

	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_ENABLETOWARDSLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result= OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_RdiTowardsLineOnlySupportedForFramedClients(pClient);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_RDI_CONTROL_LIB_EnableTowardsLine(&(pClient->RDI_Control));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_EnableTowardsLine(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;

}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{

	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DISABLETOWARDSLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result= OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_RdiTowardsLineOnlySupportedForFramedClients(pClient);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_RDI_CONTROL_LIB_DisableTowardsLine(&(pClient->RDI_Control));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_DisableTowardsLine(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}
	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_GetTowardsLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pRDI_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_RDI_CONTROL_GETTOWARDSLINESTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pRDI_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result= OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_RdiTowardsLineOnlySupportedForFramedClients(pClient);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_RDI_CONTROL_LIB_GetTowardsLineState(&(pClient->RDI_Control), pRDI_IsEnabled);
		}
	}
	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Set(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U32 iSocketClientPDH, U32 RateParameter)
{
    U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE								* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE			* pClientInterface;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;
	
	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVERECOVEREDCLOCK_SET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_RateParameterInRange(RateParameter);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientInterface = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH(pDeviceRAM);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_SetDriveRecoveredClock(pClientInterface, OutputSelector, iSocketClientPDH, RateParameter);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Set(TransactionIdentifier, OutputSelector, iSocketClientPDH, RateParameter, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_RateGet(U8 iDevice, U8 OutputSelector, U32 * pRateParameter)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE		* pClientInterface;
	
	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVERECOVEREDCLOCK_RATEGET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange((U8 *)pRateParameter);
	}



	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientInterface = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH(pDeviceRAM);

		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_GetDriveRecoveredClockRateParameterFOrOutputSelector(pClientInterface, OutputSelector, pRateParameter);
	}

	return Result;
}

U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DriveRecoveredClock_Get(U8 iDevice, U8 OutputSelector, U32 * piSocketClientPDH)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_TYPE		* pClientInterface;
	
	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_DRIVERECOVEREDCLOCK_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_OutputSelectorInRange(OutputSelector);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange((U8 *)piSocketClientPDH);
	}



	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientInterface = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH(pDeviceRAM);

		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Diagnostic_GetDriveRecoveredClock(pClientInterface, OutputSelector, piSocketClientPDH);
	}

	return Result;
}





U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_SET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_Diagnostic_StaticRule_PRBS_PatternInRange(PRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Ingress_Term_PRBS(&(pClient->Diagnostic),PRBS_Pattern);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Set(TransactionIdentifier, iSocketClientPDH, PRBS_Pattern, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Term_PRBS_Get(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_TERM_PRBS_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pPRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Ingress_Term_PRBS(&(pClient->Diagnostic),pPRBS_Pattern);
		}
	}

	return Result;
}







U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_SET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_Diagnostic_StaticRule_PRBS_PatternInRange(PRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Ingress_Gen_PRBS(&(pClient->Diagnostic),PRBS_Pattern);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Set(TransactionIdentifier, iSocketClientPDH, PRBS_Pattern, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_Gen_PRBS_Get(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_GEN_PRBS_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pPRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Ingress_Gen_PRBS(&(pClient->Diagnostic),pPRBS_Pattern);
		}
	}

	return Result;
}










U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_SET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_Diagnostic_StaticRule_PRBS_PatternInRange(PRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Egress_Term_PRBS(&(pClient->Diagnostic),PRBS_Pattern);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Set(TransactionIdentifier, iSocketClientPDH, PRBS_Pattern, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Term_PRBS_Get(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_TERM_PRBS_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pPRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Egress_Term_PRBS(&(pClient->Diagnostic),pPRBS_Pattern);
		}
	}

	return Result;
}







U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 PRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_SET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_Diagnostic_StaticRule_PRBS_PatternInRange(PRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Set_Diagnostic_Egress_Gen_PRBS(&(pClient->Diagnostic),PRBS_Pattern);
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Set(TransactionIdentifier, iSocketClientPDH, PRBS_Pattern, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_Gen_PRBS_Get(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_Pattern)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_GEN_PRBS_GET]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pPRBS_Pattern);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Egress_Gen_PRBS(&(pClient->Diagnostic),pPRBS_Pattern);
		}
	}

	return Result;
}






/*
 *
 * Section: CONFIGURATION FACILITIES DISCRETE CLIENT
 *
 */






U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClientType(U8 iDevice, U8 iDiscreteClientPort, U8 * pClientType)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GETCLIENTTYPE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pClientType);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetClientType(pClient, pClientType);
		}
	}	

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Framing)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SETFRAMING]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_FramingInRange(Framing);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_DISCRETE_CLIENT_DynamicRule_FramingInRange(pClient, Framing);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetFraming(pClient, Framing);
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetFraming(TransactionIdentifier, iDiscreteClientPort, Framing, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}

U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetFraming(U8 iDevice, U8 iDiscreteClientPort, U8 * pFraming)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GETFRAMING]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pFraming);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetFraming(pClient, pFraming);
		}
    }	

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 Mapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SETMAPPING]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_MappingInRange(Mapping);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result = OMIINO_FRAMER_DISCRETE_CLIENT_DynamicRule_MappingInRange(pClient, Mapping);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetMapping(pClient, Mapping);
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetMapping(TransactionIdentifier, iDiscreteClientPort, Mapping, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetMapping(U8 iDevice, U8 iDiscreteClientPort, U8 * pMapping)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GETMAPPING]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pMapping);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetMapping(pClient, pMapping);
		}
	}	

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 ClockRecoveryMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SETCLOCKRECOVERY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_ClockRecoveryModeInRange(ClockRecoveryMode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_SetClockRecovery(pClient, ClockRecoveryMode);
		}
    }	

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_SetClockRecovery(TransactionIdentifier, iDiscreteClientPort, ClockRecoveryMode, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GetClockRecovery(U8 iDevice, U8 iDiscreteClientPort, U8 * pClockRecoveryMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GETCLOCKRECOVERY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pClockRecoveryMode);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_GetClockRecovery(pClient, pClockRecoveryMode);
		}
	}	

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_ENABLECAS]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_Enable_CAS(pClient);
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_EnableCAS(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DISABLECAS]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_Disable_CAS(pClient);
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_DisableCAS(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_Get_CAS_State(U8 iDevice, U8 iDiscreteClientPort, U8 * pCAS_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_GET_CAS_STATE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pCAS_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DISCRETE_CLIENT_Port_Get_CAS_State(pClient, pCAS_IsEnabled);
		}
	}	

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLETOWARDSSOCKET]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsSocket(&(pClient->AIS_Control));
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsSocket(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLETOWARDSSOCKET]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsSocket(&(pClient->AIS_Control));
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsSocket(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsSocketState(U8 iDevice, U8 iDiscreteClientPort, U8 * pAIS_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GETTOWARDSSOCKETSTATE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pAIS_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_GetTowardsSocketState(&(pClient->AIS_Control), pAIS_IsEnabled);
		}
	}	

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_ENABLETOWARDSLINE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);
    
		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsLine(&(pClient->AIS_Control));
		}	
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_EnableTowardsLine(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DISABLETOWARDSLINE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsLine(&(pClient->AIS_Control));
		}
	}	


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_DisableTowardsLine(TransactionIdentifier, iDiscreteClientPort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GetTowardsLineState(U8 iDevice, U8 iDiscreteClientPort, U8 * pAIS_IsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_DISCRETE_CLIENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_DISCRETE_CLIENT_AIS_CONTROL_GETTOWARDSLINESTATE]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pAIS_IsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForDiscreteClientPort(pDeviceRAM, iDiscreteClientPort);
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_AIS_CONTROL_LIB_GetTowardsLineState(&(pClient->AIS_Control), pAIS_IsEnabled);
		}
	}	

	return Result;
}





/*
 *
 * Section: CONNECTIONS FUNCTIONS
 *
 */



/*
 *
 * DMCC - BULK APIs are for OMIINO
 *
 */



U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DELETECONNECTION]++;

	switch(Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection_SDH(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection_SONET(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;
	U8 AlarmReportingState[WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES];

	OMIINO_RAM.QC[QC_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CREATECONNECTION]++;

		


	OMIINO_API_INTERNAL_BULK_SDH_ALARM_FetchReportingStateForAll(AlarmReportingState);
	OMIINO_API_INTERNAL_BULK_ALARM_DISABLE_ALL(TransactionIdentifier, iDevice);

	switch(Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection_SDH(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				Result=OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection_SONET(TransactionIdentifier, iDevice, iLinePort, Bandwidth);
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;
	}

	OMIINO_API_INTERNAL_BULK_SDH_ALARM_SetReportingStateForAll(AlarmReportingState);

	return Result;
}




U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_BULK_CONFIGURATION_CONNECTIONS_THROUGH_CREATECONNECTION]++;

	switch(Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_4C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC4_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_HI_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC3_LO_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC12_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VC11_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS48C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS12C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS3C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_STS1_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT2_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Create_VT15_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 Bandwidth)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_BULK_CONFIGURATION_CONNECTIONS_THROUGH_DELETECONNECTION]++;

	switch(Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_4C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC4_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_HI_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC3_LO_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC12_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VC11_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS48C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS12C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS3C_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_STS1_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT2_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				OMIINO_API_INTERNAL_BULK_CONFIGURATION_FACILITIES_SONET_SDH_Delete_VT15_Connection(TransactionIdentifier, iDevice, iLinePort);
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
				break;
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_CONNECTIONS_THROUGH_CREATECONNECTION]++;

	Result=WPX_UFE_FRAMER_ERROR_FEATURE_NOT_SUPPORTED; /* TODO */
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pFirstLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pSecondLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreDifferent(pFirstLineEndpoint, pSecondLineEndpoint);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreSameType(pFirstLineEndpoint, pSecondLineEndpoint);
    }
	

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreSame_JKLM_UVW(pFirstLineEndpoint, pSecondLineEndpoint);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsArePortPairs(pFirstLineEndpoint, pSecondLineEndpoint);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pFirstLineEndpoint->iLinePort);
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pSecondLineEndpoint->iLinePort);
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pFirstLineEndpoint->Bandwidth))
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pFirstLineEndpoint->iLinePort);
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pFirstLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pFirstLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pFirstLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(pDeviceRAM, pFirstLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pSecondLineEndpoint->iLinePort);
				Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pSecondLineEndpoint);
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pSecondLineEndpoint);
					Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pSecondLineEndpoint);
				}

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(pDeviceRAM, pSecondLineEndpoint);
				}
			}
		}
		else
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pFirstLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pFirstLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(pDeviceRAM, pFirstLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pSecondLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pSecondLineEndpoint);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(pDeviceRAM, pSecondLineEndpoint);
				}
			}
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_CONNECTIONS_MakeThrough(pDeviceRAM, pFirstLineEndpoint, pSecondLineEndpoint);
		}
	}



	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, pFirstLineEndpoint, pSecondLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pSecondLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_CONNECTIONS_THROUGH_DELETECONNECTION]++;

	Result=WPX_UFE_FRAMER_ERROR_FEATURE_NOT_SUPPORTED; /* TODO */
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pFirstLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pSecondLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreDifferent(pFirstLineEndpoint, pSecondLineEndpoint);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreSameType(pFirstLineEndpoint, pSecondLineEndpoint);
    }
	

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreSame_JKLM_UVW(pFirstLineEndpoint, pSecondLineEndpoint);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsArePortPairs(pFirstLineEndpoint, pSecondLineEndpoint);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pFirstLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pSecondLineEndpoint->iLinePort);
			Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

			if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pFirstLineEndpoint->Bandwidth))
			{
				pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pFirstLineEndpoint->iLinePort);
				Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pFirstLineEndpoint);
				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pFirstLineEndpoint);
					Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pFirstLineEndpoint);
				}

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pFirstLineEndpoint);
				}

				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pSecondLineEndpoint->iLinePort);
					Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pSecondLineEndpoint);
					if(WPX_UFE_FRAMER_OK==Result)
					{
						pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pSecondLineEndpoint);
						Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pSecondLineEndpoint);
					}

					if(WPX_UFE_FRAMER_OK==Result)
					{
						Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pSecondLineEndpoint);
					}
				}
			}
			else
			{
				pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pFirstLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pFirstLineEndpoint);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pFirstLineEndpoint);
				}

				if(WPX_UFE_FRAMER_OK==Result)
				{
					pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pSecondLineEndpoint);
					Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pSecondLineEndpoint);

					if(WPX_UFE_FRAMER_OK==Result)
					{
						Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pSecondLineEndpoint);
					}
				}
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_CONNECTIONS_BreakThrough(pDeviceRAM, pFirstLineEndpoint, pSecondLineEndpoint);
			}
		}
	}



	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, pFirstLineEndpoint, pSecondLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U8 DeviceProtectionMode;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CREATECONNECTION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetProtectionMode(pDeviceRAM, &DeviceProtectionMode);
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect(iDevice,  DeviceProtectionMode, iSocketClientPDH, pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
    	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(pDeviceRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_IsFree(pClient);
				}
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_EndpointsAreCompatible(pLineEndpoint, pClient);
			}	
		}
		else
		{
			pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsFree(pDeviceRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_IsFree(pClient);
				}
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_EndpointsAreCompatible(pLineEndpoint, pClient);
			}
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_CONNECTIONS_MakeUnProtectedLineToClientSocket(pDeviceRAM, pLineEndpoint, iSocketClientPDH);
		}
	}
		

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, pLineEndpoint, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U8 DeviceProtectionMode;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE		* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DELETECONNECTION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetProtectionMode(pDeviceRAM, &DeviceProtectionMode);
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_ClientAndLineEndpointMappingIsCorrect(iDevice,  DeviceProtectionMode, iSocketClientPDH, pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
    	pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_IsConnected(pClient);
				}
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_EndpointsAreCompatible(pLineEndpoint, pClient);
			}	
		}
		else
		{
			pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

				if(WPX_UFE_FRAMER_OK==Result)
				{
					Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_IsConnected(pClient);
				}
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				Result = OMIINO_FRAMER_ConnectionsDynamicRule_EndpointsAreCompatible(pLineEndpoint, pClient);
			}
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_CONNECTIONS_BreakUnProtectedLineToClientSocket(pDeviceRAM, pLineEndpoint, iSocketClientPDH);
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, pLineEndpoint, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CREATECONNECTION]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
        pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        OMIINO_FRAMER_CONNECTIONS_MakeUnProtectedDiscreteClientToClientSocket(pDeviceRAM,  iDiscreteClientPort, iSocketClientPDH);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDiscreteClientPort, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DELETECONNECTION]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }
    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupportsDiscreteClientE1T1(iDevice);
    }	

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
        OMIINO_FRAMER_CONNECTIONS_BreakUnProtectedDiscreteClientToClientSocket(pDeviceRAM,  iDiscreteClientPort, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_CONNECTIONS_ADD_DROP_DISCRETE_CLIENT_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDiscreteClientPort, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}




/*
 *
 * Section: ALARM REPORTING
 *
 */


U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U8 i;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
	U8 DeviceMode;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_ENABLEALARMREPORTINGFORALARMCATEGORY]++;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_SONET_SDH_AlarmCategoryInRange(AlarmCategory);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(0); /* Assumes all devices instances must operate in the same mode */
    }

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, 0);
		OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
		Result=OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatchesMode(DeviceMode, AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
		{
			OMIINO_FRAMER_Device_SONET_SDH_EnableAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i), AlarmCategory);

			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory, OMIINO_RAM.Device[i].LLC.Buffer);
			if(OMIINO_RAM.Device[i].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[i].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[i].LLC.Buffer, Length);
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U8 i;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
	U8 DeviceMode;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DISABLEALARMREPORTINGFORALARMCATEGORY]++;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_SONET_SDH_AlarmCategoryInRange(AlarmCategory);
	}

	
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(0); /* Assumes all devices instances must operate in the same mode */
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, 0);
		OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
		Result=OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatchesMode(DeviceMode, AlarmCategory);
	}

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
		{
			OMIINO_FRAMER_Device_SONET_SDH_DisableAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i), AlarmCategory);

			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory, OMIINO_RAM.Device[i].LLC.Buffer);
			if(OMIINO_RAM.Device[i].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[i].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[i].LLC.Buffer, Length);
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GetAlarmReportingForAlarmCategory(U8 AlarmCategory, U8 * pIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE * pDeviceRAM;
	U8 DeviceMode;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_SONET_SDH_GETALARMREPORTINGFORALARMCATEGORY]++;

	*pIsEnabled=0;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_SONET_SDH_AlarmCategoryInRange(AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pIsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(0); /* TODO Need iDevice - Assumes all devices instances must operate in the same mode */
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, 0);
		OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
		Result=OMIINO_FRAMER_DeviceDynamicRule_AlarmCategoryMatchesMode(DeviceMode, AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		/* TODO Need iDevice -  This is applied per device driver so all devices will be configured the same */
		OMIINO_FRAMER_Device_SONET_SDH_GetAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, 0), AlarmCategory, pIsEnabled);
	}

	return Result;
}







U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U8 i;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_ENABLEALARMREPORTINGFORALARMCATEGORY]++;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_SOCKET_CLIENT_AlarmCategoryInRange(AlarmCategory);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
		{
			OMIINO_FRAMER_Device_SOCKET_CLIENT_EnableAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i), AlarmCategory);

			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory, OMIINO_RAM.Device[i].LLC.Buffer);
			if(OMIINO_RAM.Device[i].LLC_AccessIsPermitted)
			{
                           OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[i].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[i].LLC.Buffer, Length);
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U8 i;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DISABLEALARMREPORTINGFORALARMCATEGORY]++;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_SOCKET_CLIENT_AlarmCategoryInRange(AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
		{
			OMIINO_FRAMER_Device_SOCKET_CLIENT_DisableAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i), AlarmCategory);

			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory, OMIINO_RAM.Device[i].LLC.Buffer);
			if(OMIINO_RAM.Device[i].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[i].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[i].LLC.Buffer, Length);
			}
		}
	}

    return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GetAlarmReportingForAlarmCategory(U8 AlarmCategory, U8 * pIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_SOCKET_CLIENT_GETALARMREPORTINGFORALARMCATEGORY]++;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_SOCKET_CLIENT_AlarmCategoryInRange(AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pIsEnabled);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		/* NB TODO Need iDevice - This is applied per device driver so all devices will be configured the same */
		OMIINO_FRAMER_Device_SOCKET_CLIENT_GetAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, 0), AlarmCategory, pIsEnabled);
	}

    return Result;
}












U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_EnableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length=0;
	U8 i;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_ENABLEALARMREPORTINGFORALARMCATEGORY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DISCRETE_CLIENT_AlarmCategoryInRange(AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
		{
			OMIINO_FRAMER_Device_DISCRETE_CLIENT_EnableAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i), AlarmCategory);

			OMIINO_REMOVE_COMPILER_WARNING(TransactionIdentifier);
			OMIINO_REMOVE_COMPILER_WARNING(Length);
			/* TODO Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_EnableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory, OMIINO_RAM.Device[i].LLC.Buffer); */
			if(OMIINO_RAM.Device[i].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[i].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[i].LLC.Buffer, Length);
			}
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DisableAlarmReportingForAlarmCategory(U32 TransactionIdentifier, U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length=0;
	U8 i;
    
	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DISABLEALARMREPORTINGFORALARMCATEGORY]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DISCRETE_CLIENT_AlarmCategoryInRange(AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		for(i=0; i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES; i++)
		{
			OMIINO_FRAMER_Device_DISCRETE_CLIENT_DisableAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, i), AlarmCategory);

			OMIINO_REMOVE_COMPILER_WARNING(TransactionIdentifier);
			OMIINO_REMOVE_COMPILER_WARNING(Length);
			/* Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_DisableAlarmReportingForAlarmCategory(TransactionIdentifier, AlarmCategory, OMIINO_RAM.Device[i].LLC.Buffer); */
			if(OMIINO_RAM.Device[i].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[i].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[i].LLC.Buffer, Length);
			}
		}
	}

    return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GetAlarmReportingForAlarmCategory(U8 AlarmCategory, U8 * pIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	OMIINO_RAM.QC[QC_CONFIGURATION_DEVICE_DRIVER_DISCRETE_CLIENT_GETALARMREPORTINGFORALARMCATEGORY]++;

 	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
   
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(0);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DISCRETE_CLIENT_AlarmCategoryInRange(AlarmCategory);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pIsEnabled);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		/* NB TODO Need iDevice - This is applied per device driver so all devices will be configured the same */
		OMIINO_FRAMER_Device_DISCRETE_CLIENT_GetAlarmCategoryReporting(OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, 0), AlarmCategory, pIsEnabled);
	}

    return Result;
}






















/*
 *
 * Section: DIAGNOSTIC LOOPBACK
 *
 */


U8 OMIINO_Port_MapTo_SideA(U8 iDevice, U8 iLinePort)
{
	U8 Result=iLinePort;
	U8 Logical_Side_A_iLinePort;
	
	if(OMIINO_DEVICE_PERSONALITY_SupportsLineProtection(iDevice))
	{
		if(OMIINO_FRAMER_PortMap_Logical_iLinePort_Logical_Side_A_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLinePort, &Logical_Side_A_iLinePort))
		{
			Result=Logical_Side_A_iLinePort;
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLESHALLOWLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableShallow(&(pDevicePortRAM->Configuration.Diagnostic.Loopback));
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableShallow(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLESHALLOWLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
	
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableShallow(&(pDevicePortRAM->Configuration.Diagnostic.Loopback));
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallow(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GetShallowLineState(U8 iDevice, U8 iLinePort, U8 * pLoopbackIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GETSHALLOWLINESTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}



    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));


		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetShallowState(&(pDevicePortRAM->Configuration.Diagnostic.Loopback), pLoopbackIsEnabled);
		}
    }

    return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_ENABLEDEEPSYSTEM]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}



    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableDeep(&(pDevicePortRAM->Configuration.Diagnostic.Loopback));
		}
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableDeep(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DISABLEDEEPSYSTEM]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(&(pDevicePortRAM->Configuration.Diagnostic.Loopback));
		}
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableDeep(TransactionIdentifier, iLinePort, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GetDeepSystemState(U8 iDevice, U8 iLinePort, U8 * pLoopbackIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GETDEEPSYSTEMSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


    
    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));

		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));


		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetDeepState(&(pDevicePortRAM->Configuration.Diagnostic.Loopback),pLoopbackIsEnabled);
		}
    }

	return Result;
}








U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_ENABLESHALLOWSYSTEM]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableShallow(&(pClient->Diagnostic.Loopback));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableShallow(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableShallowSystem(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DISABLESHALLOWSYSTEM]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableShallow(&(pClient->Diagnostic.Loopback));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableShallow(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetShallowSystemState(U8 iDevice, U32 iSocketClientPDH, U8 * pLoopbackIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GETSHALLOWSYSTEMSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pLoopbackIsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetShallowState(&(pClient->Diagnostic.Loopback),pLoopbackIsEnabled);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	U32 i;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_ENABLEDEEPLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		for(i=0;i<WPX_UFE_FRAMER_BUILD_OPTION_MAX_SOCKET_CLIENT_PDH_PORTS;i++)
		{
			/* DMCC - only one instance offered at this stage, but model one per client ready for upgrade */
			pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(&(pClient->Diagnostic.Loopback));
		}

		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
		OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableDeep(&(pClient->Diagnostic.Loopback));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_EnableDeep(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableDeepLine(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DISABLEDEEPLINE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(&(pClient->Diagnostic.Loopback));
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_DisableDeep(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GetDeepLineState(U8 iDevice, U32 iSocketClientPDH, U8 * pLoopbackIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_LOOPBACK_GETDEEPLINESTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pLoopbackIsEnabled);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetDeepState(&(pClient->Diagnostic.Loopback),pLoopbackIsEnabled);
		}
	}

	return Result;
}






U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Enable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_ENABLE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);

        if(WPX_UFE_FRAMER_OK==Result)
	    {
	        pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

            pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	        Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
        }
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
			
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableDeep(&(pDeviceHO_PathRAM->Configuration.Diagnostic.Loopback));
			}
		}
		else
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableDeep(&(pDeviceLO_PathRAM->Configuration.Diagnostic.Loopback));
			}
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Enable(TransactionIdentifier, pLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Disable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_DISABLE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);

        if(WPX_UFE_FRAMER_OK==Result)
	    {
	        pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

            pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	        Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
        }
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
			
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(&(pDeviceHO_PathRAM->Configuration.Diagnostic.Loopback));
			}
		}
		else
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(&(pDeviceLO_PathRAM->Configuration.Diagnostic.Loopback));
			}
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Disable(TransactionIdentifier, pLineEndpoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GetState(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pLoopbackIsEnabled)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM=NULL;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GETSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);

        if(WPX_UFE_FRAMER_OK==Result)
	    {
	        pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

            pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
	        Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
        }
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		if(OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
			
			Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
			if(WPX_UFE_FRAMER_OK==Result)
			{
				pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
			}

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetDeepState(&(pDeviceHO_PathRAM->Configuration.Diagnostic.Loopback),pLoopbackIsEnabled);
			}
		}
		else
		{
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetDeepState(&(pDeviceLO_PathRAM->Configuration.Diagnostic.Loopback),pLoopbackIsEnabled);
			}
		}
	}

	return Result;
}



/*
 *
 * Section: STATUS DEVICE DRIVER
 *
 */


U8 OMIINO_API_INTERNAL_STATUS_DEVICE_DRIVER_VERSION_GetInformation(char *pDeviceDriverInformation)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    char ProductReleaseStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];
    char SoftwareBuildDateTimeStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];
    char SoftwareBuildVersionStr[OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD+1];

	OMIINO_RAM.QC[QC_STATUS_DEVICE_DRIVER_VERSION_GETINFORMATION]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange((U8 *)pDeviceDriverInformation);

	if(WPX_UFE_FRAMER_OK==Result)
	{
        OMIINO_FRAMER_STATUS_DeviceDriver_GetProductReleaseStr(&OMIINO_RAM.Driver,ProductReleaseStr);
        OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildDateTimeStr(&OMIINO_RAM.Driver,SoftwareBuildDateTimeStr);
        OMIINO_FRAMER_STATUS_DeviceDriver_GetSoftwareBuildVersionStr(&OMIINO_RAM.Driver,SoftwareBuildVersionStr);

        sprintf(pDeviceDriverInformation,       "OMIINO Framer Driver Version Information\nProduct Release: %s\nSoftware Build Date and Time: %s\nSoftware Build Version: %s",
                                                ProductReleaseStr,
                                                SoftwareBuildDateTimeStr,
                                                SoftwareBuildVersionStr);
                                                
                                                
                                                
                                                
	}

    return Result;
}

/*
 *
 * Section: STATUS DEVICE
 *
 */


U8 OMIINO_API_INTERNAL_STATUS_DEVICE_VERSION_GetFirmwareInformation(U8 iDevice, char *pFirmwareInformation)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_STATUS_DEVICE_TYPE * pDeviceStatusRAM;
    
	OMIINO_RAM.QC[QC_STATUS_DEVICE_VERSION_GETFIRMWAREINFORMATION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange((U8 *)pFirmwareInformation);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
    {
        pDeviceStatusRAM = OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, iDevice);
        OMIINO_FRAMER_STATUS_DeviceDriver_GetFirmwareVersionStr(pDeviceStatusRAM, pFirmwareInformation);
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_STATUS_DEVICE_VERSION_GetHardwareInformation(U8 iDevice, char *pHardwareInformation)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_STATUS_DEVICE_TYPE * pDeviceStatusRAM;

	OMIINO_RAM.QC[QC_STATUS_DEVICE_VERSION_GETHARDWAREINFORMATION]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange((U8 *)pHardwareInformation);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
        pDeviceStatusRAM = OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_STATUS_DeviceDriver_GetHardwareVersionStr(pDeviceStatusRAM, pHardwareInformation);
	}

	return Result;
}






U8 OMIINO_API_INTERNAL_STATUS_DEVICE_BUILD_GetHardwareVariant(U8 iDevice, U8 * pHardwareBuildVariant)
{
    U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_STATUS_DEVICE_TYPE * pDeviceStatusRAM;
    
	OMIINO_RAM.QC[QC_STATUS_DEVICE_BUILD_GETHARDWAREVARIANT]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pHardwareBuildVariant);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
        pDeviceStatusRAM = OMIINO_FRAMER_GetStatusRAMForDevice(&OMIINO_RAM, iDevice);
        OMIINO_FRAMER_STATUS_DeviceDriver_GetHardwareVariant(pDeviceStatusRAM, pHardwareBuildVariant);
    }

	return Result;
}



/*
 *
 * Section: STATUS FACILITIES SONET/SDH PORT SECTION
 *
 */


U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetRX(U8 iDevice, U8 iLinePort, char * pRX_TraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pRX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetRX(&(pDevicePortRAM->Section.J0), pRX_TraceString);
		}
	}

	return Result;
}




/*
 *
 * Section: STATUS FACILITIES SONET/SDH PORT LINE
 *
 */

U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GetRX(U8 iDevice, U8 iLinePort, U8 * pRX_S1)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pRX_S1);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_SONET_SDH_Port_Line_S1_GetRX(&(pDevicePortRAM->Line.S1.Status), pRX_S1);
		}
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GetRX(U8 iDevice, U8 iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE				* pDevicePortRAM;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange((U8 *)pRX_K1K2);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
			pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, iLinePort);
			OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_GetRX(&(pDevicePortRAM->Line.K1K2.Status), pRX_K1K2);
		}
	}

	return Result;
}




/*
 *
 * Section: STATUS FACILITIES SONET/SDH HO PATH
 *
 */

U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pRX_TraceString)
{
	U8												Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE					* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE		* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE	* pDeviceHO_PathRAM=NULL;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pRX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetRX(&(pDeviceHO_PathRAM->Overhead.J1), pRX_TraceString);
			}
		}

	}

    return Result;
}


U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pRX_C2)
{
	U8												Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE					* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE		* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE	* pDeviceHO_PathRAM=NULL;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pRX_C2);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		
		Result=OMIINO_FRAMER_DeviceDynamicRule_HighOrderIndexMatch(pDevicePortRAM, pLineEndpoint);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
				
			Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
		}

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_SONET_SDH_HO_Path_C2_GetRX(&(pDeviceHO_PathRAM->Overhead.C2.Status), pRX_C2);
			}
		}
	}

	return Result;
}




/*
 *
 * Section: STATUS FACILITIES SONET/SDH LO PATH
 *
 */

U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pRX_TraceString)
{
	U8																Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(pRX_TraceString);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
			pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
			Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_SONET_SDH_LO_Path_J2_GetRX(&(pDeviceLO_PathRAM->Overhead.J2), pRX_TraceString);
			}
		}
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pRX_V5_SignalLabel)
{
	U8																Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SIGNALLABEL_GETRX]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pRX_V5_SignalLabel);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(1==OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(pLineEndpoint->Bandwidth))
		{
			Result=WPX_UFE_FRAMER_ERROR_FACILITY_BANDWIDTH_MISMATCH;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);

		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
			if(WPX_UFE_FRAMER_OK==Result)
			{
				OMIINO_FRAMER_SONET_SDH_LO_PATH_V5_SIGNAL_LABEL_GetRX(&(pDeviceLO_PathRAM->Overhead.V5_SignalLabel.Status), pRX_V5_SignalLabel);
			}
		}
	}

	return Result;
}





/*
 *
 * Section: STATUS DIAGNOSTICS
 *
 */



U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Ingress_PRBS_GetState(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_State)
{
	U8																Result = WPX_UFE_FRAMER_OK;
	U8																PRBS_Pattern;
    OMIINO_FRAMER_HIERARCHY_TYPE *									pDeviceRAM;
	OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE *			pClientStatus;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *	pClientConfig;

	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_INGRESS_PRBS_GETSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pPRBS_State);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientConfig = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClientConfig);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientConfig = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Ingress_Term_PRBS(&(pClientConfig->Diagnostic),&PRBS_Pattern);
		if(WPX_UFE_FRAMER_PRBS_DISABLED==PRBS_Pattern)
		{
			*pPRBS_State=WPX_UFE_FRAMER_PRBS_STATE_IDLE;
		}
		else
		{
			pClientStatus = OMIINO_FRAMER_GetStatusRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Ingress_PRBS_State(pClientStatus, pPRBS_State);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_Egress_PRBS_GetState(U8 iDevice, U32 iSocketClientPDH, U8 * pPRBS_State)
{
	U8																Result = WPX_UFE_FRAMER_OK;
	U8																PRBS_Pattern;
    OMIINO_FRAMER_HIERARCHY_TYPE *									pDeviceRAM;
	OMIINO_FRAMER_STATUS_SOCKET_CLIENT_PDH_ELEMENT_TYPE *			pClientStatus;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE *	pClientConfig;


	OMIINO_RAM.QC[QC_STATUS_FACILITIES_SOCKET_CLIENT_PDH_DIAGNOSTIC_EGRESS_PRBS_GETSTATE]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(pPRBS_State);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientConfig = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClientConfig);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{ 
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClientConfig = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
		OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Egress_Term_PRBS(&(pClientConfig->Diagnostic),&PRBS_Pattern);
		if(WPX_UFE_FRAMER_PRBS_DISABLED==PRBS_Pattern)
		{
			*pPRBS_State=WPX_UFE_FRAMER_PRBS_STATE_IDLE;
		}
		else
		{
			
			pClientStatus = OMIINO_FRAMER_GetStatusRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
			OMIINO_FRAMER_SOCKET_CLIENT_PDH_Get_Diagnostic_Egress_PRBS_State(pClientStatus, pPRBS_State);
		}
	}

	return Result;
}










U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_GetMonitoringState(U8 iDevice, U8 iLinePort, U8 iPoint, U8 *pState)
{
	U8											Result = WPX_UFE_FRAMER_OK;
	U16											CompressedKey;
	OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_PORT_GETMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_PortPointInRange(iPoint);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		if(!OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GivenNodeKeyGetCompressedKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key, &CompressedKey))
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
		
		*pState=OMIINO_FRAMER_PerformanceMonitoringEngine_GetMonitor_LinePortPoint(iDevice, &OMIINO_RAM.PerformanceMonitoringDeviceTable, &PM_Engine_Key);
    }

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U8 *pState)
{
	U8																Result = WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HIGHORDERPATH_GETMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_HoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		if(0==pDeviceHO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		*pState=pDeviceHO_PathRAM->Configuration.PM_State[iPoint];
	}

	return Result;
}


U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U8 *pState)
{
	U8																Result = WPX_UFE_FRAMER_OK;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;


	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LOWORDERPATH_GETMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_LoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pLineEndpoint);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		if(0==pDeviceLO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		*pState=pDeviceLO_PathRAM->Configuration.PM_State[iPoint];
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_PM_E1_GET_MONITORING_CRC_State(U8 iDevice, U32 iSocketClientPDH, U8 * pState)
{
	U8															Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE								* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;

	OMIINO_RAM.QC[QC_PM_E1_GET_MONITORING_CRC]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
 
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		*pState=pClient->PM.PM_State[WPX_UFE_FRAMER_PDH_E1_T1_CRC];
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_PM_T1_GET_MONITORING_CRC_State(U8 iDevice, U32 iSocketClientPDH, U8 * pState)
{
	U8															Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE								* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;

	OMIINO_RAM.QC[QC_PM_T1_GET_MONITORING_CRC]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{

		*pState=pClient->PM.PM_State[WPX_UFE_FRAMER_PDH_E1_T1_CRC];
	}

    return Result;
}


   
U8 OMIINO_API_INTERNAL_PM_E3T3_GET_MONITORING_State(U8 iDevice, U32 iSocketClientPDH, U8 iPoint, U8 * pState)
{
   U8 Result = WPX_UFE_FRAMER_OK;
   OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;
   OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient=NULL;

   OMIINO_RAM.QC[QC_PM_E3T3_GET_MONITORING]++;
   
   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
      
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E3T3_PM_IsAvailable(pClient, iPoint);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      *pState=pClient->PM.PM_State[iPoint];
   }
   
   return Result;
}










U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint)
{
    U8 Result;
    U8 Length;
	U16 CompressedKey;
	OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_PORT_ENABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_PortPointInRange(iPoint);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		if(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GivenNodeKeyGetCompressedKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key, &CompressedKey))
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_ALREADY_MONITORED;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
		

		OMIINO_FRAMER_PerformanceMonitoringEngine_Monitor_LinePortPoint(iDevice, &OMIINO_RAM.PerformanceMonitoringDeviceTable, &PM_Engine_Key);
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(&OMIINO_RAM.Device[iDevice].M_of_N);

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iLinePort, iPoint, CompressedKey, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
    }

    return Result;
}








U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iLinePort, U8 iPoint)
{
    U8 Result;
    U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;
	U16 CompressedKey;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_PORT_DISABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_PortPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		if(!OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GivenNodeKeyGetCompressedKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key, &CompressedKey))
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		if(OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GivenNodeKeyGetCompressedKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key, &CompressedKey))
		{
			OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_FreeNode(&(pDeviceRAM->PerformanceMonitoringKeyManager), CompressedKey);
		}

		OMIINO_FRAMER_PerformanceMonitoringEngine_UnMonitor_LinePoint(iDevice, &OMIINO_RAM.PerformanceMonitoringDeviceTable, &PM_Engine_Key);
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(&OMIINO_RAM.Device[iDevice].M_of_N);

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iLinePort, iPoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}








U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
    U8 Result;
    U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;
	U16 CompressedKey;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HIGHORDERPATH_ENABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_HoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		if(pDeviceHO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_ALREADY_MONITORED;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_HO;
		OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&PM_Engine_Key.u.HO.LineEndpoint, pLineEndpoint);
		PM_Engine_Key.u.HO.iDataPointIdentifier=iPoint;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
        
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		pDeviceHO_PathRAM->Configuration.PM_State[iPoint]=1;
		pDeviceHO_PathRAM->Configuration.PM_CompressedKey[iPoint]=CompressedKey;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(&OMIINO_RAM.Device[iDevice].M_of_N);

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, pLineEndpoint, iPoint, CompressedKey, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}








U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
    U8 Result;
    U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HIGHORDERPATH_DISABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_HoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
	}

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		if(0==pDeviceHO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_FreeNode(&(pDeviceRAM->PerformanceMonitoringKeyManager), pDeviceHO_PathRAM->Configuration.PM_CompressedKey[iPoint]);
		pDeviceHO_PathRAM->Configuration.PM_State[iPoint]=0;
		pDeviceHO_PathRAM->Configuration.PM_CompressedKey[iPoint]=0xFFFF;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(&OMIINO_RAM.Device[iDevice].M_of_N);

        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, pLineEndpoint, iPoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}







U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
	U8 Result;
    U8 Length;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;
	U16 CompressedKey;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LOWORDERPATH_ENABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_LoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		if(pDeviceLO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_ALREADY_MONITORED;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_LO;
		OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(&PM_Engine_Key.u.LO.LineEndpoint, pLineEndpoint);
		PM_Engine_Key.u.LO.iDataPointIdentifier=iPoint;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
		
		pDeviceLO_PathRAM->Configuration.PM_State[iPoint]=1;
		pDeviceLO_PathRAM->Configuration.PM_CompressedKey[iPoint]=CompressedKey;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(&OMIINO_RAM.Device[iDevice].M_of_N);

        Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, pLineEndpoint, iPoint, CompressedKey, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

    return Result;
}







U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
    U8 Result;
    U8 Length;
	U8 i;
	U8 Count=0;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;


	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LOWORDERPATH_DISABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_LoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pLineEndpoint);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		if(0==pDeviceLO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		pDeviceLO_PathRAM->Configuration.PM_State[iPoint]=0;
		pDeviceLO_PathRAM->Configuration.PM_CompressedKey[iPoint]=0xFFFF;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(&OMIINO_RAM.Device[iDevice].M_of_N);

		for(i=0;i<WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_MAX_DATA_POINTS_PER_LO_PATH;i++)
		{
			Count+=pDeviceLO_PathRAM->Configuration.PM_State[i]; /* Warning - Mixing boolean and int */
		}

		if(0==Count)
		{
			Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, pLineEndpoint, iPoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
			if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
			{
				OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
			}
		}
	}

    return Result;
}




U8 OMIINO_API_INTERNAL_PM_E1_ENABLE_MONITORING_CRC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;
	U16 CompressedKey;
	
	OMIINO_RAM.QC[QC_PM_E1_ENABLE_MONITORING_CRC]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
  
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH;
		PM_Engine_Key.u.PDH.iPDHPort=iSocketClientPDH;
		PM_Engine_Key.u.PDH.iDataPointIdentifier=WPX_UFE_FRAMER_PDH_E1_T1_CRC;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
		
		pClient->PM.PM_State[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=1;
		pClient->PM.PM_CompressedKey[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=CompressedKey;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(&OMIINO_RAM.Device[iDevice].M_of_N);

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E1_ENABLE_MONITORING_CRC(TransactionIdentifier, iSocketClientPDH, CompressedKey, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}



U8 OMIINO_API_INTERNAL_PM_E1_DISABLE_MONITORING_CRC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;

	OMIINO_RAM.QC[QC_PM_E1_DISABLE_MONITORING_CRC]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
 
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pClient->PM.PM_State[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=0;
		pClient->PM.PM_CompressedKey[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=0xFFFF;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(&OMIINO_RAM.Device[iDevice].M_of_N);

		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E1_DISABLE_MONITORING_CRC(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_PM_T1_ENABLE_MONITORING_CRC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) /* TODO */
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;
	U16 CompressedKey;

	OMIINO_RAM.QC[QC_PM_T1_ENABLE_MONITORING_CRC]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{

		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH;
		PM_Engine_Key.u.PDH.iPDHPort=iSocketClientPDH;
		PM_Engine_Key.u.PDH.iDataPointIdentifier=WPX_UFE_FRAMER_PDH_E1_T1_CRC;

		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
		
		pClient->PM.PM_State[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=1;
		pClient->PM.PM_CompressedKey[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=CompressedKey;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(&OMIINO_RAM.Device[iDevice].M_of_N);


		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_T1_ENABLE_MONITORING_CRC(TransactionIdentifier, iSocketClientPDH, CompressedKey, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}




U8 OMIINO_API_INTERNAL_PM_T1_DISABLE_MONITORING_CRC(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH) /* TODO */
{
	U8 Result = WPX_UFE_FRAMER_OK;
    U8 Length;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;

	OMIINO_RAM.QC[QC_PM_T1_DISABLE_MONITORING_CRC]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{

		pClient->PM.PM_State[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=0;
		pClient->PM.PM_CompressedKey[WPX_UFE_FRAMER_PDH_E1_T1_CRC]=0xFFFF;
		OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(&OMIINO_RAM.Device[iDevice].M_of_N);


		Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_T1_DISABLE_MONITORING_CRC(TransactionIdentifier, iSocketClientPDH, OMIINO_RAM.Device[iDevice].LLC.Buffer);
		if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
		{
			OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
		}
	}

	return Result;
}

U8 OMIINO_API_INTERNAL_PM_E3T3_ENABLE_MONITORING(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 iPoint)
{
   U8 Result = WPX_UFE_FRAMER_OK;
   U8 Length;
   OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;
   OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient=NULL;
   OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;
   U16 CompressedKey;
   
   OMIINO_RAM.QC[QC_PM_E3T3_ENABLE_MONITORING]++;
   
   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
      
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E3T3_PM_IsAvailable(pClient, iPoint);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_PDH;
      PM_Engine_Key.u.PDH.iPDHPort=iSocketClientPDH;
      PM_Engine_Key.u.PDH.iDataPointIdentifier=iPoint;
      
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      CompressedKey=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GetNodeKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key);
      
      pClient->PM.PM_State[iPoint]=1;
      pClient->PM.PM_CompressedKey[iPoint]=CompressedKey;
      OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Inc(&OMIINO_RAM.Device[iDevice].M_of_N);
      
      Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E3T3_ENABLE_MONITORING(TransactionIdentifier, iSocketClientPDH, iPoint, CompressedKey, OMIINO_RAM.Device[iDevice].LLC.Buffer);
      if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
      {
         OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
      }
   }
   
   return Result;
}



U8 OMIINO_API_INTERNAL_PM_E3T3_DISABLE_MONITORING(U32 TransactionIdentifier, U8 iDevice, U32 iSocketClientPDH, U8 iPoint)
{
   U8 Result = WPX_UFE_FRAMER_OK;
   U8 Length;
   OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
   OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;
   
   OMIINO_RAM.QC[QC_PM_E3T3_DISABLE_MONITORING]++;
   
   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
      
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E3T3_PM_IsAvailable(pClient, iPoint);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      pClient->PM.PM_State[iPoint]=0;
      pClient->PM.PM_CompressedKey[iPoint]=0xFFFF;
      OMIINO_FRAMER_PerformanceMonitoring_M_of_N_Dec(&OMIINO_RAM.Device[iDevice].M_of_N);
      
      Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PM_E3T3_DISABLE_MONITORING(TransactionIdentifier, iSocketClientPDH, iPoint, OMIINO_RAM.Device[iDevice].LLC.Buffer);
      if(OMIINO_RAM.Device[iDevice].LLC_AccessIsPermitted)
      {
         OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[iDevice].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_CONFIGURATION, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
      }
   }
   
   return Result;
}




U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint)
{
    U8 Result;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_DISCRETEPORT_ENABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_DiscretePortPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	} 

    if(WPX_UFE_FRAMER_OK==Result)
	{
        if (TransactionIdentifier); /* TODO OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_EnableMonitoring */
    }

    return Result;
}







U8 OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iDiscreteClientPort, U8 iPoint)
{
    U8 Result;

	OMIINO_RAM.QC[QC_CONFIGURATION_PERFORMANCE_MONITORING_DISCRETEPORT_DISABLEMONITORING]++;

	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	} 

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_DiscretePortPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
        if (TransactionIdentifier); /* TODO OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_DiscretePort_DisableMonitoring */
    }

    return Result;
}








/*
 *
 * Section: Status PERFORMANCE MONITORING Data Collection
 *
 */

U8 OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_Port_ReadData(U8 iDevice, U8 iTableIndex, U8 iLinePort, U8 iPoint, U32 * pCount)
{
    
	U8 Result;
	U16 CompressedKey;
	OMIINO_FRAMER_HIERARCHY_TYPE				* pDeviceRAM;
    OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE * pDevicePortRAM;
	OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_TYPE PM_Engine_Key;
    
	OMIINO_RAM.QC[QC_STATUS_PERFORMANCE_MONITORING_COLLECT_PORT_DATA]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_PortPointInRange(iPoint);
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(iDevice, iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
	    pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, OMIINO_Port_MapTo_SideA(iDevice, iLinePort));
		Result = OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(&(pDevicePortRAM->Configuration));
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);

		PM_Engine_Key.KeyType=OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_SONET_SDH_PORT;
		PM_Engine_Key.u.LinePort.iPort=iLinePort;
		PM_Engine_Key.u.LinePort.iDataPointIdentifier=iPoint;

		if(!OMIINO_FRAMER_PERFORMANCE_MONITORING_KEY_MANAGER_GivenNodeKeyGetCompressedKey(&(pDeviceRAM->PerformanceMonitoringKeyManager), &PM_Engine_Key, &CompressedKey))
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].MAX_SONET_SDH_PerformanceMonitoringPortTableElements<=iTableIndex)
		{
			Result=WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE;
		}
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].SONET_SDH_PerformanceMonitoringPortTable[iTableIndex].Count[iLinePort][iPoint];
    }

    return Result;
}


U8 OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_HO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint, U8 iPoint, U32 * pCount)
{
	U8 Result;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_TYPE						* pDevicePortRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_TYPE					* pDeviceHO_PathRAM;

	OMIINO_RAM.QC[QC_STATUS_PERFORMANCE_MONITORING_COLLECT_HO_PATH_DATA]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_HoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_HO_FacilityExists(pDeviceHO_PathRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceHO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDevicePortRAM = OMIINO_FRAMER_GetHierarchyNodeForLinePortRAM(pDeviceRAM, pLineEndpoint->iLinePort);
		pDeviceHO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForHO_Path(pDevicePortRAM, pLineEndpoint);

		if(0==pDeviceHO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].MAX_SONET_SDH_PerformanceMonitoringHighOrderPathTableElements<=iTableIndex)
		{
			Result=WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE;
		}
	}


    if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
		switch(DeviceMode)
		{
			case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
					*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iTableIndex].Count.SDH[pLineEndpoint->iLinePort][pLineEndpoint->u.sdh.iJ][pLineEndpoint->u.sdh.iK][iPoint];
					break;

			case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
					*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].SONET_SDH_PerformanceMonitoringHighOrderPathTable[iTableIndex].Count.SONET[pLineEndpoint->iLinePort][pLineEndpoint->u.sonet.iU][iPoint];
					break;

			default:
					Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_OUT_OF_RANGE;
					break;

		}
	}


    return Result;
}


U8 OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_LO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint, U8 iPoint, U32 * pCount)
{
	U8 Result;
	U8 DeviceMode;
	OMIINO_FRAMER_HIERARCHY_TYPE									* pDeviceRAM;
	OMIINO_FRAMER_HIERARCHY_SONET_SDH_LO_PATH_ELEMENT_TYPE			* pDeviceLO_PathRAM;

	OMIINO_RAM.QC[QC_STATUS_PERFORMANCE_MONITORING_COLLECT_LO_PATH_DATA]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_LoPointInRange(iPoint);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilitySupports_SONET_SDH(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result = OMIINO_FRAMER_SONET_SDH_DynamicRule_LO_FacilityExists(pDeviceLO_PathRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		Result = OMIINO_FRAMER_ConnectionsDynamicRule_SONET_SDH_EndpointIsConnected(pDeviceRAM, pLineEndpoint);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);
		Result=OMIINO_FRAMER_SONET_SDH_DynamicRule_ThroughConnectionPreventsConfigurationChange(&(pDeviceLO_PathRAM->Configuration.Connection));
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pDeviceLO_PathRAM = OMIINO_FRAMER_GetConfigurationRAMForLO_Path(pDeviceRAM, pLineEndpoint);

		if(0==pDeviceLO_PathRAM->Configuration.PM_State[iPoint])
		{
			Result=WPX_UFE_FRAMER_ERROR_POINT_IS_NOT_MONITORED;
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].MAX_SONET_SDH_PerformanceMonitoringLowOrderPathTableElements<=iTableIndex)
		{
			Result=WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE;
		}
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		OMIINO_FRAMER_Device_GetMode(pDeviceRAM, &DeviceMode);
		switch(DeviceMode)
		{
			case	WPX_UFE_FRAMER_DEVICE_MODE_SDH:
					*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iTableIndex].Count.SDH[pLineEndpoint->iLinePort][pLineEndpoint->u.sdh.iJ][pLineEndpoint->u.sdh.iK][pLineEndpoint->u.sdh.iL][pLineEndpoint->u.sdh.iM][iPoint];
					break;

			case	WPX_UFE_FRAMER_DEVICE_MODE_SONET:
					*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].SONET_SDH_PerformanceMonitoringLowOrderPathTable[iTableIndex].Count.SONET[pLineEndpoint->iLinePort][pLineEndpoint->u.sonet.iU][pLineEndpoint->u.sonet.iV][pLineEndpoint->u.sonet.iW][iPoint];
					break;

			default:
					Result=WPX_UFE_FRAMER_ERROR_DEVICE_MODE_OUT_OF_RANGE;
					break;

		}
	}

    return Result;
}



U8 OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_DiscreteClientPort_ReadData(U8 iDevice, U8 iTableIndex, U8 iDiscreteClientPort, U8 iPoint, U32 * pCount)
{
    U8 Result;

	OMIINO_RAM.QC[QC_STATUS_PERFORMANCE_MONITORING_COLLECT_DISCRETE_CLIENT_PORT_DATA]++;


	Result = OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO();


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_PerformanceMonitoringStaticRule_DiscretePortPointInRange(iPoint);
	}


	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DISCRETE_CLIENT_PortStaticRule_PortInRange(iDiscreteClientPort);
	} 


	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].MAX_DiscreteClientPerformanceMonitoringTableElements<=iTableIndex)
		{
			Result=WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].DiscreteClientPerformanceMonitoringTable[iTableIndex].Count[iDiscreteClientPort][iPoint];
	}

    return Result;
}




U8 OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SocketClientPort_PDH_E1T1_ReadData(U8 iDevice, U8 iTableIndex, U32 iSocketClientPDH, U8 iPoint, U32 * pCount)
{
	U8 Result = WPX_UFE_FRAMER_OK;
    OMIINO_FRAMER_HIERARCHY_TYPE							* pDeviceRAM;
	OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE	* pClient=NULL;
	
	OMIINO_RAM.QC[QC_STATUS_PERFORMANCE_MONITORING_COLLECT_SOCKET_CLIENT_PORT_E1_DATA]++;

	Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
	}
 
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
	}

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
		pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);

		Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E1T1_CRC_IsAvailable(pClient);
	}

	if(WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT<=iPoint)
	{
		Result=WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE;
	}
	
	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].MAX_PDH_PerformanceMonitoringTableElements<=iTableIndex)
		{
			Result=WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		*pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].PDH_PerformanceMonitoringTable[iTableIndex].E1T1[iSocketClientPDH][iPoint];
	}

    return Result;
}


U8 OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SocketClientPort_PDH_E3T3_ReadData(U8 iDevice, U8 iTableIndex, U32 iSocketClientPDH, U8 iPoint, U32 * pCount)
{
   U8 Result = WPX_UFE_FRAMER_OK;
   OMIINO_FRAMER_HIERARCHY_TYPE	* pDeviceRAM;
   OMIINO_FRAMER_CONFIGURATION_SOCKET_CLIENT_PDH_ELEMENT_TYPE * pClient=NULL;
   
   OMIINO_RAM.QC[QC_STATUS_PERFORMANCE_MONITORING_COLLECT_SOCKET_CLIENT_PORT_T3_DATA]++;
   
   Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceInNormalMode(iDevice);
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_StaticRule_SocketInRange(iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRange(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_DynamicRule_SocketInRangeForConfiguredLinePortBandwidth(iDevice, iSocketClientPDH);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      pDeviceRAM = OMIINO_FRAMER_GetHierarchyRAMForDevice(&OMIINO_RAM, iDevice);
      pClient = OMIINO_FRAMER_GetConfigurationRAMForSocketClientPDH_Element(pDeviceRAM, iSocketClientPDH);
      
      Result = OMIINO_FRAMER_SOCKET_CLIENT_PDH_PortDynamicRule_FacilityExists(pClient);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result=OMIINO_FRAMER_SOCKET_CLIENT_PDH_E3T3_PM_IsAvailable(pClient, iPoint);
   }
   
   if(WPX_UFE_FRAMER_MAX_PDH_MONITORED_POINTS_PER_PDH_ELEMENT<=iPoint)
   {
      Result=WPX_UFE_FRAMER_ERROR_POINT_OUT_OF_RANGE;
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      Result=OMIINO_FRAMER_DeviceDynamicRule_DevicePerformanceMonitoring_M_of_N_OK(iDevice);
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      if(OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].MAX_PDH_PerformanceMonitoringTableElements<=iTableIndex)
      {
         Result=WPX_UFE_FRAMER_ERROR_PM_TABLE_INDEX_OUT_OF_RANGE;
      }
   }
   
   if(WPX_UFE_FRAMER_OK==Result)
   {
      *pCount=OMIINO_RAM.PerformanceMonitoringDeviceTable.Device[iDevice].PDH_PerformanceMonitoringTable[iTableIndex].E1T1[iSocketClientPDH][iPoint];
   }
   
   return Result;
}


/* This API needs to be called for the DEMO BOARD ONLY - after set build personality */ 
void OMIINO_API_INTERNAL_DEMO_ONLY_Configure_GTP_ClockSource(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;
	Result = OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(iDevice);

    if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_DeviceDynamicRule_DeviceCapabilityIsDefined(iDevice);
    }

	if(WPX_UFE_FRAMER_OK==Result)
	{
		OMIINO_LLC_SOUTHBOUND_FORMATTER_DEMO_ONLY_CONFIGURE_GTP_CLOCK_SOURCE_MESSAGE(iDevice);
	}
}








