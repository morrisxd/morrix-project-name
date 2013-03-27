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



void OMIINO_FRAMER_SONET_SDH_Port_Set_DCC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 DCC_Mode, U8 DCC_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_DCC_MAX>DCC_Mode,DCC_Mode);
	OMIINO_FRAMER_ASSERT(2>DCC_State,DCC_State);

	pPortHierarchy->DCC[DCC_Mode].State = DCC_State;
}




void OMIINO_FRAMER_SONET_SDH_Port_Get_DCC(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 DCC_Mode, U8 * pDCC_State)
{
	OMIINO_FRAMER_ASSERT(NULL!=pPortHierarchy,0);
	OMIINO_FRAMER_ASSERT(NULL!=pDCC_State,0);
	OMIINO_FRAMER_ASSERT(WPX_UFE_FRAMER_DCC_MAX>DCC_Mode,DCC_Mode);

	*pDCC_State = pPortHierarchy->DCC[DCC_Mode].State;

	OMIINO_FRAMER_ASSERT(2>(*pDCC_State),(*pDCC_State));
}



