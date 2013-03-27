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


void OMIINO_FRAMER_API_COUNT_IncIn(OMIINO_API_ELEMENT_TYPE * pAPI_CountRAM, U32 iAPI)
{
    OMIINO_FRAMER_ASSERT(MAX_APIS>iAPI,iAPI);
    pAPI_CountRAM[iAPI].In++;
}

void OMIINO_FRAMER_API_COUNT_IncOut(OMIINO_API_ELEMENT_TYPE * pAPI_CountRAM, U32 iAPI)
{
    OMIINO_FRAMER_ASSERT(MAX_APIS>iAPI,iAPI);
    pAPI_CountRAM[iAPI].Out++;
}

void OMIINO_FRAMER_API_COUNT_Initialize(OMIINO_API_ELEMENT_TYPE * pAPI_CountRAM)
{
    int i;

    for(i=0;i<MAX_APIS;i++)
    {
        pAPI_CountRAM[i].In=0;
        pAPI_CountRAM[i].Out=0;
    }
}



