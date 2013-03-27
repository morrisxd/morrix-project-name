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



void OMIINO_FRAMER_UTILITIES_Initialize(OMIINO_UTILITY_RAM_TYPE *pUtilityRAM)
{
	OMIINO_FRAMER_FIREWALL_Initialize(&(pUtilityRAM->Firewall));
    OMIINO_FRAMER_API_COUNT_Initialize(pUtilityRAM->API);
}


