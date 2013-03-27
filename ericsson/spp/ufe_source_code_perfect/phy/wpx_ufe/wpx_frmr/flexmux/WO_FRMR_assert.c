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


void OMIINO_FRAMER_ASSERT_Failure(U32 ReasonCode, U32 TheLine, char * TheFile, char * AssertionText)
{
    sprintf(OMIINO_RAM.Utility.AssertBuffer, "Omiino: Assertion Failure %08d (0x%x), %s at Line %d in File %s\n", ReasonCode, ReasonCode, AssertionText, TheLine, TheFile );
	OMIINO_FRAMER_ENVIRONMENT_WRAPPER_FatalErrorHandler(OMIINO_RAM.Utility.AssertBuffer);
}


