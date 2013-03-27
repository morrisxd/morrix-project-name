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



void OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetSignalDegradeThreshold(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * pAnyThresholdConfigurationNode, U8 Threshold)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyThresholdConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_BIP_THRESHOLD>Threshold,Threshold);

	pAnyThresholdConfigurationNode->SignalDegradeThreshold = Threshold;
}




void OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetSignalDegradeThreshold(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * pAnyThresholdConfigurationNode, U8 *pThreshold)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyThresholdConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pThreshold,0);

	*pThreshold = pAnyThresholdConfigurationNode->SignalDegradeThreshold;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_BIP_THRESHOLD>(*pThreshold),(*pThreshold));
}



void OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_SetExcessiveThreshold(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * pAnyThresholdConfigurationNode, U8 Threshold)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyThresholdConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_BIP_THRESHOLD>Threshold,Threshold);

	pAnyThresholdConfigurationNode->ExcessiveThreshold = Threshold;
}




void OMIINO_FRAMER_SONET_SDH_BIP_THRESHOLD_LIB_GetExcessiveThreshold(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_BIP_THRESHOLD_TYPE * pAnyThresholdConfigurationNode, U8 *pThreshold)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyThresholdConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pThreshold,0);

	*pThreshold = pAnyThresholdConfigurationNode->ExcessiveThreshold;

	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_MAX_BIP_THRESHOLD>(*pThreshold),(*pThreshold));
}



