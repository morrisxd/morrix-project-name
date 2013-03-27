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

void OMIINO_FRAMER_Device_SetCardProtectionOperatingMode(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AnyCardProtectionOperatingMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AnyCardProtectionOperatingMode);
	OMIINO_FRAMER_ASSERT(WPX_UFE_MAX_DEVICE_CARD_PROTECTION_OPERATING_MODES>AnyCardProtectionOperatingMode,AnyCardProtectionOperatingMode);

	pHierarchy->Configuration.CardProtectionOperatingMode=AnyCardProtectionOperatingMode;
}


void OMIINO_FRAMER_Device_GetCardProtectionOperatingMode(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 * pAnyCardProtectionOperatingMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pAnyCardProtectionOperatingMode,0);

	*pAnyCardProtectionOperatingMode=pHierarchy->Configuration.CardProtectionOperatingMode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_MAX_DEVICE_CARD_PROTECTION_OPERATING_MODES>*pAnyCardProtectionOperatingMode,*pAnyCardProtectionOperatingMode);
}




void OMIINO_FRAMER_Device_SetProtectionMode(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AnyDeviceProtectionMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AnyDeviceProtectionMode);
	OMIINO_FRAMER_ASSERT(WPX_UFE_MAX_DEVICE_PROTECTION_MODES>AnyDeviceProtectionMode,AnyDeviceProtectionMode);

	pHierarchy->Configuration.ProtectionMode=AnyDeviceProtectionMode;
}


void OMIINO_FRAMER_Device_GetProtectionMode(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 * pAnyDeviceProtectionMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pAnyDeviceProtectionMode,0);

	*pAnyDeviceProtectionMode=pHierarchy->Configuration.ProtectionMode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_MAX_DEVICE_PROTECTION_MODES>*pAnyDeviceProtectionMode,*pAnyDeviceProtectionMode);
}


void OMIINO_FRAMER_Device_SetTestMode(OMIINO_FRAMER_DEVICE_TYPE *pDeviceRAM, U8 TestMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceRAM,TestMode);
	OMIINO_FRAMER_ASSERT(1>=TestMode,TestMode);

	pDeviceRAM->TestMode=TestMode;
}


void OMIINO_FRAMER_Device_SetBuildPersonality(OMIINO_FRAMER_DEVICE_TYPE *pDeviceRAM, U8 BuildPersonality)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceRAM,BuildPersonality);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_PERSONALITY_MAX>BuildPersonality,BuildPersonality);

	pDeviceRAM->BuildPersonality=BuildPersonality;
}



void OMIINO_FRAMER_Device_GetBuildPersonality(OMIINO_FRAMER_DEVICE_TYPE *pDeviceRAM, U8 * pBuildPersonality)
{
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceRAM,0);
	OMIINO_FRAMER_ASSERT(NULL!=pBuildPersonality,0);

	*pBuildPersonality=pDeviceRAM->BuildPersonality;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_BUILD_PERSONALITY_MAX>(*pBuildPersonality),(*pBuildPersonality));
}



void OMIINO_FRAMER_Device_SetMode(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 DeviceMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,DeviceMode);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_DEVICE_MODES>DeviceMode,DeviceMode);

	pHierarchy->Configuration.Mode=DeviceMode;
}


void OMIINO_FRAMER_Device_GetMode(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 * pDeviceMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pDeviceMode,0);

	*pDeviceMode = pHierarchy->Configuration.Mode;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_DEVICE_MODES>(*pDeviceMode),(*pDeviceMode));
}






void OMIINO_FRAMER_Device_Set_AIS_Insertion(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AIS_InsertionMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AIS_InsertionMode);
	OMIINO_FRAMER_ASSERT(2>AIS_InsertionMode,AIS_InsertionMode);

	pHierarchy->Configuration.AIS_InsertionMode=AIS_InsertionMode;
}




void OMIINO_FRAMER_Device_Get_AIS_Insertion(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 * pAIS_InsertionMode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pAIS_InsertionMode,0);

	*pAIS_InsertionMode = pHierarchy->Configuration.AIS_InsertionMode;

	OMIINO_FRAMER_ASSERT(2>(*pAIS_InsertionMode),(*pAIS_InsertionMode));
}






void OMIINO_FRAMER_Device_SONET_SDH_EnableAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AlarmCategory);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	pHierarchy->SONET_SDH_AlarmCategoryReporting[AlarmCategory].IsEnabled=1;
}



void OMIINO_FRAMER_Device_SONET_SDH_DisableAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AlarmCategory);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	pHierarchy->SONET_SDH_AlarmCategoryReporting[AlarmCategory].IsEnabled=0;
}




void OMIINO_FRAMER_Device_SONET_SDH_GetAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory, U8 * pIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pIsEnabled,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	*pIsEnabled = pHierarchy->SONET_SDH_AlarmCategoryReporting[AlarmCategory].IsEnabled;

	OMIINO_FRAMER_ASSERT(1>=(*pIsEnabled),(*pIsEnabled));
}




void OMIINO_FRAMER_Device_SOCKET_CLIENT_EnableAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AlarmCategory);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	pHierarchy->SOCKET_CLIENT_AlarmCategoryReporting[AlarmCategory].IsEnabled=1;
}



void OMIINO_FRAMER_Device_SOCKET_CLIENT_DisableAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AlarmCategory);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	pHierarchy->SOCKET_CLIENT_AlarmCategoryReporting[AlarmCategory].IsEnabled=0;
}




void OMIINO_FRAMER_Device_SOCKET_CLIENT_GetAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory, U8 * pIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pIsEnabled,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	*pIsEnabled = pHierarchy->SOCKET_CLIENT_AlarmCategoryReporting[AlarmCategory].IsEnabled;

	OMIINO_FRAMER_ASSERT(1>=(*pIsEnabled),(*pIsEnabled));
}



void OMIINO_FRAMER_Device_DISCRETE_CLIENT_EnableAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AlarmCategory);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	pHierarchy->DISCRETE_CLIENT_AlarmCategoryReporting[AlarmCategory].IsEnabled=1;
}



void OMIINO_FRAMER_Device_DISCRETE_CLIENT_DisableAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,AlarmCategory);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	pHierarchy->DISCRETE_CLIENT_AlarmCategoryReporting[AlarmCategory].IsEnabled=0;
}




void OMIINO_FRAMER_Device_DISCRETE_CLIENT_GetAlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 AlarmCategory, U8 * pIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pIsEnabled,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES>AlarmCategory,AlarmCategory);

	*pIsEnabled = pHierarchy->DISCRETE_CLIENT_AlarmCategoryReporting[AlarmCategory].IsEnabled;

	OMIINO_FRAMER_ASSERT(1>=(*pIsEnabled),(*pIsEnabled));
}




void OMIINO_FRAMER_InitializeDevice_DISCRETE_CLIENT_AlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy)
{
	U8 i;

	for(i=0; i<WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES; i++)
	{
		OMIINO_FRAMER_Device_DISCRETE_CLIENT_DisableAlarmCategoryReporting(pHierarchy, i);
	}
}




void OMIINO_FRAMER_InitializeDevice_SOCKET_CLIENT_AlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy)
{
	U8 i;

	for(i=0; i<WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES; i++)
	{
		OMIINO_FRAMER_Device_SOCKET_CLIENT_DisableAlarmCategoryReporting(pHierarchy, i);
	}
}





void OMIINO_FRAMER_InitializeDevice_SONET_SDH_AlarmCategoryReporting(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy)
{
	U8 i;

	for(i=0; i<WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES; i++)
	{
		OMIINO_FRAMER_Device_SONET_SDH_DisableAlarmCategoryReporting(pHierarchy, i);
	}
}



void OMIINO_FRAMER_InitializeDeviceParameters(OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchy, U8 DeviceMode, U8 DeviceProtectionMode, U8 DeviceCardProtectionOperatingMode, U8 AIS_InsertionMode)
{
	U8 OutputSelector;

	OMIINO_FRAMER_Device_SetMode(pHierarchy, DeviceMode);
	OMIINO_FRAMER_Device_SetProtectionMode(pHierarchy, DeviceProtectionMode);
	OMIINO_FRAMER_Device_SetCardProtectionOperatingMode(pHierarchy, DeviceCardProtectionOperatingMode);
	OMIINO_FRAMER_Device_Set_AIS_Insertion(pHierarchy, AIS_InsertionMode);
	
	OMIINO_FRAMER_InitializeDevice_SONET_SDH_AlarmCategoryReporting(pHierarchy);

	for(OutputSelector=0;OutputSelector<MAX_SONET_SDH_RECOVERED_CLOCKS;OutputSelector++)
	{
		OMIINO_FRAMER_DEVICE_Diagnostic_SetDriveRecoveredClock(&(pHierarchy->Configuration.Diagnostic), 0, OutputSelector);
	}

	OMIINO_FRAMER_InitializeDevice_SOCKET_CLIENT_AlarmCategoryReporting(pHierarchy);
	OMIINO_FRAMER_InitializeDevice_DISCRETE_CLIENT_AlarmCategoryReporting(pHierarchy);
}

