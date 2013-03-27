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


U8 OMIINO_FRAMER_SONET_HIERARCHY_Given_STS1_Get_STS3c(U8 iSTS1)
{
    return(iSTS1/3);
}


U8 OMIINO_FRAMER_SONET_HIERARCHY_Given_STS3c_Get_STS12c(U8 iSTS3c)
{
    return(iSTS3c/12);
}


U8 OMIINO_FRAMER_SONET_HIERARCHY_Given_STS12c_Get_STS48c(U8 iSTS12c)
{
    return (iSTS12c/48);
}
