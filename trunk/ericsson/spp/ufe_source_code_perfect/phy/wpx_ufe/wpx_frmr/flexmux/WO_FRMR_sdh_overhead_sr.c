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





U8 OMIINO_FRAMER_SONET_SDH_Overhead_StaticRule_TraceStringValid(char * pTraceString)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(NULL==pTraceString)
	{
		Result = WPX_UFE_FRAMER_ERROR_PARAMETER_OUT_OF_RANGE;
	}

	return Result;
}



