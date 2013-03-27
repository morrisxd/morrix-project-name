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




void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_SetRX(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPortHierarchy, char * pJ1_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_RX,0);

    memset(pPortHierarchy->Status.RX, '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1);

	strncpy( pPortHierarchy->Status.RX, pJ1_RX, pPortHierarchy->Configuration.Mode );
}


void OMIINO_FRAMER_SONET_SDH_HO_Path_J1_GetRX(OMIINO_FRAMER_HIERARCHY_SONET_SDH_HO_PATH_OVERHEAD_J1_TYPE * pPortHierarchy, char * pJ1_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ1_RX,0);

	memset(pJ1_RX, '\0', pPortHierarchy->Configuration.Mode+1);

	strncpy( pJ1_RX, pPortHierarchy->Status.RX, (pPortHierarchy->Configuration.Mode-1) );
}





