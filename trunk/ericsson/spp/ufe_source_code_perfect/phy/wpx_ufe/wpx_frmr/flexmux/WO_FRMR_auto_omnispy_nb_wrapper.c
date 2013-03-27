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




void OMIINO_OMNISPY_WRAPPER_NorthboundAnnounce(U8 iDevice, U32 * pMessage, U32 Length)
{
    if(OMIINO_DEVICE_PERSONALITY_IsDefined(iDevice))
    {
        if(OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.OmniSpyNorthBoundAnnounce)
        {
            (*OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.OmniSpyNorthBoundAnnounce)(iDevice, pMessage, Length);
        }
    }
}





void OMIINO_OMNISPY_BindCallbacks(WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE *pTo, WPX_UFE_FRAMER_OMNISPY_TABLE_TYPE *pFrom)
{
	pTo->PreProductInitialization.Integrity=pFrom->PreProductInitialization.Integrity;
    pTo->OmniSpyNorthBoundAnnounce=pFrom->OmniSpyNorthBoundAnnounce;
	pTo->VirtualScreen.NextPage=NULL;
	pTo->VirtualScreen.PrevPage=NULL;
	pTo->VirtualScreen.Refresh=NULL;
}


