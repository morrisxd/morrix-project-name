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

#include <stdio.h>

#include "WO_FRMR_private.h"
#include "WO_FRMR_MIPS_private.h"

char WPX_UFE_FRAMER_MIPS_CommonEndpointBuffer[128];


U8 WPX_UFE_FRAMER_MIPS_Is_SDH(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	U8 Result=1;

	switch(pCommon_SDH_SONET_LineEndpointType->TransferType)
	{
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_T1:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_E1:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_DS3:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_E3:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12:
		case	WPX_UFE_FRAMER_WUFE_SONET_TYPE_VCAT:
				Result=0;
				break;

		default:
				Result=1;
				break;
	}

   return Result;
}


void WPX_UFE_FRAMER_MIPS_SDH_CommonLineEndpointToTxt(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	sprintf(WPX_UFE_FRAMER_MIPS_CommonEndpointBuffer,"(SDH: stm4=%d, stm1=%d, stm0=%d, tug2=%d, tu=%d)",
															pCommon_SDH_SONET_LineEndpointType->u.SDH.stm4, 
															pCommon_SDH_SONET_LineEndpointType->u.SDH.stm1,
															pCommon_SDH_SONET_LineEndpointType->u.SDH.stm0,
															pCommon_SDH_SONET_LineEndpointType->u.SDH.tug2,
															pCommon_SDH_SONET_LineEndpointType->u.SDH.tu);
}



void WPX_UFE_FRAMER_MIPS_SONET_CommonLineEndpointToTxt(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	sprintf(WPX_UFE_FRAMER_MIPS_CommonEndpointBuffer,"(SONET: sts12=%d, sts3=%d, sts1=%d, vt_group=%d, vt=%d)", 
																pCommon_SDH_SONET_LineEndpointType->u.SONET.sts12,
																pCommon_SDH_SONET_LineEndpointType->u.SONET.sts3,
																pCommon_SDH_SONET_LineEndpointType->u.SONET.sts1,
																pCommon_SDH_SONET_LineEndpointType->u.SONET.vt_group,
																pCommon_SDH_SONET_LineEndpointType->u.SONET.vt);
}



char * WPX_UFE_FRAMER_MIPS_CommonLineEndpointToTxt(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pCommon_SDH_SONET_LineEndpointType)
{
	memset(WPX_UFE_FRAMER_MIPS_CommonEndpointBuffer, '\0', 128);

	if(WPX_UFE_FRAMER_MIPS_Is_SDH(pCommon_SDH_SONET_LineEndpointType))
	{
		WPX_UFE_FRAMER_MIPS_SDH_CommonLineEndpointToTxt(pCommon_SDH_SONET_LineEndpointType);
	}
	else
	{
		WPX_UFE_FRAMER_MIPS_SONET_CommonLineEndpointToTxt(pCommon_SDH_SONET_LineEndpointType);
	}

	return WPX_UFE_FRAMER_MIPS_CommonEndpointBuffer;
}


