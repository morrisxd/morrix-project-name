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





void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_SetRX(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_SECTION_J0_TYPE * pPortHierarchy, char * pJ0_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_RX,0);

	memset(&(pPortHierarchy->Status.RX), '\0', WPX_UFE_FRAMER_MAX_CHARS_IN_64_BYTE_PATH_TRACE+1);

	strncpy( pPortHierarchy->Status.RX, pJ0_RX, pPortHierarchy->Configuration.Mode );
}


void OMIINO_FRAMER_SONET_SDH_Port_Section_J0_GetRX(OMIINO_FRAMER_HIERARCHY_SONET_SDH_PORT_SECTION_J0_TYPE * pPortHierarchy, char * pJ0_RX)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pJ0_RX,0);

	memset(pJ0_RX, '\0', pPortHierarchy->Configuration.Mode+1);

	if(WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE==pPortHierarchy->Configuration.Mode)
	{
		strncpy( pJ0_RX, pPortHierarchy->Status.RX, WPX_UFE_FRAMER_PATH_TRACE_MODE_1_BYTE );
	}
	else
	{
		strncpy( pJ0_RX, pPortHierarchy->Status.RX, (pPortHierarchy->Configuration.Mode-1) );
	}
}


