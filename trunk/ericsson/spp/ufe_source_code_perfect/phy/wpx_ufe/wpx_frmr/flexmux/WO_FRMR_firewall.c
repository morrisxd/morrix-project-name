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




void OMIINO_FRAMER_FIREWALL_AddItemToObservationList(OMIINO_FRAMER_FIREWALL_DATA_TYPE *pFirewallRAM, U32 AnyAddress, U32 AnySignature)
{
	if(MAX_FIREWALL_ELEMENTS>(pFirewallRAM->NextFree))
	{ 
        pFirewallRAM->Element[pFirewallRAM->NextFree].Address=AnyAddress;
        pFirewallRAM->Element[pFirewallRAM->NextFree].Signature=AnySignature;
        pFirewallRAM->NextFree++;
        *(U32 *)AnyAddress=OMIINO_FIREWALL_PATTERN;
	}
	else
	{
		OMIINO_FRAMER_RSE(AnyAddress);
	}
}



void OMIINO_FRAMER_FIREWALL_Verify(OMIINO_FRAMER_FIREWALL_DATA_TYPE *pFirewallRAM)
{        
    U32 i;

	for(i=0; i<(pFirewallRAM->NextFree); i++)
	{
        OMIINO_FRAMER_ASSERT(OMIINO_FIREWALL_PATTERN==*(U32 *)pFirewallRAM->Element[i].Address,pFirewallRAM->Element[i].Signature);
	}
}



void OMIINO_FRAMER_FIREWALL_Initialize(OMIINO_FRAMER_FIREWALL_DATA_TYPE *pFirewallRAM)
{
	U32 i;

	for(i=0; i<MAX_FIREWALL_ELEMENTS; i++)
	{
        pFirewallRAM->Element[i].Address=0x00000000;
        pFirewallRAM->Element[i].Signature=0x00000000;
	}
    pFirewallRAM->NextFree=0;
}


