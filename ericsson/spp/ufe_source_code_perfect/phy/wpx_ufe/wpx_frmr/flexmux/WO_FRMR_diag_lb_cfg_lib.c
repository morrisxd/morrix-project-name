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




void OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableShallow(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
    pAnyLoopbackConfigurationNode->ShallowIsEnabled=1;
}



void OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableShallow(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
    pAnyLoopbackConfigurationNode->ShallowIsEnabled=0;
}


void OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetShallowState(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode, U8 * pLoopbackIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pLoopbackIsEnabled,0);

    *pLoopbackIsEnabled=pAnyLoopbackConfigurationNode->ShallowIsEnabled;
}




void OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_EnableDeep(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
    pAnyLoopbackConfigurationNode->DeepIsEnabled=1;
}



void OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_DisableDeep(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
    pAnyLoopbackConfigurationNode->DeepIsEnabled=0;
}


void OMIINO_FRAMER_DIAGNOSTIC_LOOPBACK_LIB_GetDeepState(OMIINO_FRAMER_CONFIGURATION_DIAGNOSTIC_LOOPBACK_TYPE * pAnyLoopbackConfigurationNode, U8 * pLoopbackIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyLoopbackConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pLoopbackIsEnabled,0);

    *pLoopbackIsEnabled=pAnyLoopbackConfigurationNode->DeepIsEnabled;
}







