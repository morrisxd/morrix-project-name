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



void OMIINO_FRAMER_HOUSEKEEPING_Entry(U32 iAPI)
{
    OMIINO_FRAMER_API_COUNT_IncIn(OMIINO_RAM.Utility.API,iAPI);
	/* TODO Make HOST Only OMIINO_FRAMER_FIREWALL_Verify(&(OMIINO_RAM.Utility.Firewall)); */
}





void OMIINO_FRAMER_HOUSEKEEPING_Exit(U32 iAPI)
{
    OMIINO_FRAMER_API_COUNT_IncOut(OMIINO_RAM.Utility.API,iAPI);
	/* TODO Make HOST Only OMIINO_FRAMER_FIREWALL_Verify(&(OMIINO_RAM.Utility.Firewall)); */
}


