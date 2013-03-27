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





void OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pAnySignalLabelConfigurationNode, U8 SignalLabel_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnySignalLabelConfigurationNode,0);
    pAnySignalLabelConfigurationNode->TX=SignalLabel_TX;
}


void OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_GetTX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pAnySignalLabelConfigurationNode, U8 * pSignalLabel_TX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnySignalLabelConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pSignalLabel_TX,0);

    *pSignalLabel_TX=pAnySignalLabelConfigurationNode->TX;
}



void OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_SetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pAnySignalLabelConfigurationNode, U8 SignalLabel_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnySignalLabelConfigurationNode,0);
    pAnySignalLabelConfigurationNode->EX=SignalLabel_EX;
}



void OMIINO_FRAMER_SONET_SDH_SIGNAL_LABEL_LIB_GetEX(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_OVERHEAD_SIGNAL_LABEL_TYPE * pAnySignalLabelConfigurationNode, U8 * pSignalLabel_EX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnySignalLabelConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pSignalLabel_EX,0);

    *pSignalLabel_EX=pAnySignalLabelConfigurationNode->EX;
}




