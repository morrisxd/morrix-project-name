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


U8 OMIINO_FRAMER_DeviceStaticRule_DeviceCardProtectionOperatingModeInRange(U8 CardProtectionOperatingMode)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	if(WPX_UFE_MAX_DEVICE_CARD_PROTECTION_OPERATING_MODES<=CardProtectionOperatingMode)
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_CARD_PROTECTION_OPERATING_MODE_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceStaticRule_DeviceProtectionModeInRange(U8 AnyDeviceProtectionMode)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	if(WPX_UFE_MAX_DEVICE_PROTECTION_MODES<=AnyDeviceProtectionMode)
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_PROTECTION_MODE_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceStaticRule_FunctionalityNotImplemented_TODO(void)
{
	return WPX_UFE_FRAMER_ERROR_NOT_IMPLEMENTED;
}


U8 OMIINO_FRAMER_DeviceStaticRule_DeviceInRange(U8 iDevice)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES<=iDevice)
	{
		Result = WPX_UFE_FRAMER_ERROR_DEVICE_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceStaticRule_BuildPersonalitySupported(U8 AnyPersonality)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:	
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:	
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:	
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:
				Result = WPX_UFE_FRAMER_OK;
				break;


		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12_4_PLUS_4_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_16_OC3:		
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_4_PLUS_4_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_64_DISCRETE_ONLY:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_32_DISCRETE_ONLY:				
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_16_DISCRETE_ONLY:				
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_OC48:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_32_DISCRETE:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_32_DISCRETE:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_UNDEFINED:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_UNDEFINED;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
				break;

	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceStaticRule_BuildPersonalityInRange(U8 AnyPersonality)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:				
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:

		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:	
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:				
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:				
				Result = WPX_UFE_FRAMER_OK;
				break;

		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_UNDEFINED:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_UNDEFINED;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
				break;

	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceStaticRule_DeviceModeInRange(U8 DeviceMode)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(DeviceMode)
    {
    
		case    WPX_UFE_FRAMER_DEVICE_MODE_SDH:	  /* Drop through */ 
        case    WPX_UFE_FRAMER_DEVICE_MODE_SONET:
                Result = WPX_UFE_FRAMER_OK;
                break;

        default: 
                Result = WPX_UFE_FRAMER_ERROR_DEVICE_MODE_OUT_OF_RANGE;
                break;

    }

	return Result;
}



U8 OMIINO_FRAMER_DeviceStaticRule_ParamterInRange(U8 * pAnyParameter)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(NULL==pAnyParameter)
	{
		Result = WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE;
	}

	return Result;
}



U8 OMIINO_FRAMER_DeviceStaticRule_SONET_SDH_AlarmCategoryInRange(U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_SONET_SDH_MAX_ALARM_CATEGORIES<=AlarmCategory)
	{
		Result = WPX_UFE_FRAMER_ERROR_ALARM_CATEGORY_OUT_OF_RANGE;
	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceStaticRule_DISCRETE_CLIENT_AlarmCategoryInRange(U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_DISCRETE_CLIENT_MAX_ALARM_CATEGORIES<=AlarmCategory)
	{
		Result = WPX_UFE_FRAMER_ERROR_ALARM_CATEGORY_OUT_OF_RANGE;
	}

	return Result;
}


U8 OMIINO_FRAMER_DeviceStaticRule_SOCKET_CLIENT_AlarmCategoryInRange(U8 AlarmCategory)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_SOCKET_CLIENT_MAX_ALARM_CATEGORIES<=AlarmCategory)
	{
		Result = WPX_UFE_FRAMER_ERROR_ALARM_CATEGORY_OUT_OF_RANGE;
	}

	return Result;
}




