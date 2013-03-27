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




void OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsLine(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
    pAnyAISConfigurationNode->TowardsLineIsEnabled=1;
}



void OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsLine(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
    pAnyAISConfigurationNode->TowardsLineIsEnabled=0;
}


void OMIINO_FRAMER_AIS_CONTROL_LIB_GetTowardsLineState(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode, U8 * pLoopbackIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pLoopbackIsEnabled,0);

    *pLoopbackIsEnabled=pAnyAISConfigurationNode->TowardsLineIsEnabled;
}

void OMIINO_FRAMER_RDI_CONTROL_LIB_EnableTowardsLine(OMIINO_FRAMER_CONFIGURATION_RDI_CONTROL_TYPE * pAnyRDIConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyRDIConfigurationNode,0);
    pAnyRDIConfigurationNode->TowardsLineIsEnabled=1;
}



void OMIINO_FRAMER_RDI_CONTROL_LIB_DisableTowardsLine(OMIINO_FRAMER_CONFIGURATION_RDI_CONTROL_TYPE * pAnyRDIConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyRDIConfigurationNode,0);
    pAnyRDIConfigurationNode->TowardsLineIsEnabled=0;
}


void OMIINO_FRAMER_RDI_CONTROL_LIB_GetTowardsLineState(OMIINO_FRAMER_CONFIGURATION_RDI_CONTROL_TYPE * pAnyRDIConfigurationNode, U8 * pLoopbackIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyRDIConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pLoopbackIsEnabled,0);

    *pLoopbackIsEnabled=pAnyRDIConfigurationNode->TowardsLineIsEnabled;
}


void OMIINO_FRAMER_AIS_CONTROL_LIB_EnableTowardsSocket(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
    pAnyAISConfigurationNode->TowardsSocketIsEnabled=1;
}



void OMIINO_FRAMER_AIS_CONTROL_LIB_DisableTowardsSocket(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
    pAnyAISConfigurationNode->TowardsSocketIsEnabled=0;
}


void OMIINO_FRAMER_AIS_CONTROL_LIB_GetTowardsSocketState(OMIINO_FRAMER_CONFIGURATION_AIS_CONTROL_TYPE * pAnyAISConfigurationNode, U8 * pLoopbackIsEnabled)
{
	OMIINO_FRAMER_ASSERT(NULL!=pAnyAISConfigurationNode,0);
	OMIINO_FRAMER_ASSERT(NULL!=pLoopbackIsEnabled,0);

    *pLoopbackIsEnabled=pAnyAISConfigurationNode->TowardsSocketIsEnabled;
}







