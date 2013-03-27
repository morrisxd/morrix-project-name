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


U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_1_OC48_6(U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:

				Result=WPX_UFE_FRAMER_OK;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_2_OC24_6(U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{	
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_4_PLUS_4_OC3:
				Result=WPX_UFE_FRAMER_OK;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_3_OC12_6(U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
				Result=WPX_UFE_FRAMER_OK;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_4_DISCRETE_ONLY_6(U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_64_DISCRETE_ONLY:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_32_DISCRETE_ONLY:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_16_DISCRETE_ONLY:
				Result=WPX_UFE_FRAMER_OK;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_5_OC48_6(U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_OC48:
				Result=WPX_UFE_FRAMER_OK;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;
	}

	return Result;
}

U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_6_MIXED_6(U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	switch(AnyPersonality)
	{
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12_32_DISCRETE:
		case	WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_32_DISCRETE:
				Result=WPX_UFE_FRAMER_OK;
				break;

		default:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;
	}

	return Result;
}



U8 OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule(OMIINO_FRAMER_STATUS_DEVICE_TYPE * pRAM, U8 AnyPersonality)
{
	U8 Result=WPX_UFE_FRAMER_OK;

    switch(pRAM->HardwareVariant)
    {

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_UNDEFINED:
				Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_1_OC48_6:
				Result=OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_1_OC48_6(AnyPersonality);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_2_OC24_6:
				Result=OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_2_OC24_6(AnyPersonality);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_3_OC12_6:
				Result=OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_3_OC12_6(AnyPersonality);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_4_DISCRETE_ONLY_6:
				Result=OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_4_DISCRETE_ONLY_6(AnyPersonality);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_5_OC48_6:
				Result=OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_5_OC48_6(AnyPersonality);
				break;

		case	WPX_UFE_FRAMER_HARDWARE_BUILD_6_MIXED_6:
				Result=OMIINO_FRAMER_HardwareVariantPermitsPersonalityDynamicRule_WPX_UFE_FRAMER_HARDWARE_BUILD_6_MIXED_6(AnyPersonality);
				break;	

        default:
			Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_INCONSISTENT_WITH_BUILD_VARIANT;
            OMIINO_FRAMER_RSE(pRAM->HardwareVariant);
			pRAM->HardwareVariant=WPX_UFE_FRAMER_HARDWARE_BUILD_UNDEFINED;
            break;
    }	

	return Result;
}




