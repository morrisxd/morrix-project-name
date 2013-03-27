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




U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC4_4C_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}



	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iJ)
		{
			Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iK)
		{
			Result = WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iL)
		{
			Result = WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iM)
		{
			Result = WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC4_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}



	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT<=pLineEndpoint->u.sdh.iJ)
		{
			Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iK)
		{
			Result = WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iL)
		{
			Result = WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iM)
		{
			Result = WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC3_HI_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT<=pLineEndpoint->u.sdh.iJ)
		{
			Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_K<=pLineEndpoint->u.sdh.iK)
		{
			Result = WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iL)
		{
			Result = WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iM)
		{
			Result = WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC3_LO_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT<=pLineEndpoint->u.sdh.iJ)
		{
			Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_K<=pLineEndpoint->u.sdh.iK)
		{
			Result = WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iL)
		{
			Result = WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sdh.iM)
		{
			Result = WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC12_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT<=pLineEndpoint->u.sdh.iJ)
		{
			Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_K<=pLineEndpoint->u.sdh.iK)
		{
			Result = WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_L<=pLineEndpoint->u.sdh.iL)
		{
			Result = WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(3<=pLineEndpoint->u.sdh.iM)
		{
			Result = WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE;
		}
	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC11_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_J_PER_PORT<=pLineEndpoint->u.sdh.iJ)
		{
			Result = WPX_UFE_FRAMER_ERROR_iJ_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_K<=pLineEndpoint->u.sdh.iK)
		{
			Result = WPX_UFE_FRAMER_ERROR_iK_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_L<=pLineEndpoint->u.sdh.iL)
		{
			Result = WPX_UFE_FRAMER_ERROR_iL_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_M<=pLineEndpoint->u.sdh.iM)
		{
			Result = WPX_UFE_FRAMER_ERROR_iM_OUT_OF_RANGE;
		}
	}

	return Result;
}






U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS48C_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT<=pLineEndpoint->u.sonet.iU)
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=((pLineEndpoint->u.sonet.iU)/48))
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iV)
		{
			Result = WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iW)
		{
			Result = WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE;
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS12C_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT<=pLineEndpoint->u.sonet.iU)
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=((pLineEndpoint->u.sonet.iU)/12))
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iV)
		{
			Result = WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iW)
		{
			Result = WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE;
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS3C_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT<=pLineEndpoint->u.sonet.iU)
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=((pLineEndpoint->u.sonet.iU)%3))
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iV)
		{
			Result = WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iW)
		{
			Result = WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE;
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS1_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT<=pLineEndpoint->u.sonet.iU)
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iV)
		{
			Result = WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(0!=pLineEndpoint->u.sonet.iW)
		{
			Result = WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE;
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_VT15_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT<=pLineEndpoint->u.sonet.iU)
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_V<=pLineEndpoint->u.sonet.iV)
		{
			Result = WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(3<=pLineEndpoint->u.sonet.iW)
		{
			Result = WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE;
		}
	}

	return Result;
}

U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_VT2_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_OK==Result)
	{
		Result = OMIINO_FRAMER_SONET_SDH_Port_StaticRule_PortInRange(pLineEndpoint->iLinePort);
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_U_PER_PORT<=pLineEndpoint->u.sonet.iU)
		{
			Result = WPX_UFE_FRAMER_ERROR_iU_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_V<=pLineEndpoint->u.sonet.iV)
		{
			Result = WPX_UFE_FRAMER_ERROR_iV_OUT_OF_RANGE;
		}
	}

	if(WPX_UFE_FRAMER_OK==Result)
	{
		if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_W<=pLineEndpoint->u.sonet.iW)
		{
			Result = WPX_UFE_FRAMER_ERROR_iW_OUT_OF_RANGE;
		}
	}

	return Result;
}



U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_BandwidthInRange(pLineEndpoint->Bandwidth);

	if(WPX_UFE_FRAMER_OK==Result)
	{
		switch(pLineEndpoint->Bandwidth)
		{    
			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC4_4C_EndpointInRange(pLineEndpoint);
					break;

			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC4_EndpointInRange(pLineEndpoint);
					break;

			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC3_HI_EndpointInRange(pLineEndpoint);
					break;

			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC3_LO_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC12_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SDH_VC11_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS48C_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS12C_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS3C_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_STS1_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_VT15_EndpointInRange(pLineEndpoint);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
					Result = OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_SONET_VT2_EndpointInRange(pLineEndpoint);
					break;

			default:
					Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
					break;

		}
	}

	return Result;
}



U8 OMIINO_FRAMER_Is_SDH_Endpoint(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = 0;

	switch(pLineEndpoint->Bandwidth)
	{    
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  /* Drop through */ 
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				Result = 1;
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:	/* Drop through */ 
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
				/* Nothing to do */
				break;

		default:
				OMIINO_FRAMER_RSE(pLineEndpoint->Bandwidth);
				break;

	}

	return Result;
}





U8 OMIINO_FRAMER_Is_SONET_Endpoint(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pLineEndpoint)
{
	U8 Result = 0;

	switch(pLineEndpoint->Bandwidth)
	{    
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  /* Drop through */ 
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
				/* Nothing to do */
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:	/* Drop through */ 
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
				Result = 1;
				break;

		default:
				OMIINO_FRAMER_RSE(pLineEndpoint->Bandwidth);
				break;

	}

	return Result;
}


U8 OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_BandwidthInRange(U8 Bandwidth)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(Bandwidth)
    {    
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		/* Drop through */ 
                Result = WPX_UFE_FRAMER_OK;
                break;

        default:
                Result=WPX_UFE_FRAMER_ERROR_BANDWIDTH_OUT_OF_RANGE;
                break;

    }

	return Result;
}




U8 OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsHighOrder(U8 Bandwidth)
{
	U8 Result = 0;

	switch(Bandwidth)
    {    
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:		/* Drop through */ 
                Result = 1;
                break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		/* Drop through */ 
                Result = 0;
                break;

        default:
                OMIINO_FRAMER_RSE(Bandwidth);
                break;
    }

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsLowOrder(U8 Bandwidth)
{
	U8 Result = 0;

	switch(Bandwidth)
    {    
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:		/* Drop through */ 
                Result = 0;
                break;

		case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		/* Drop through */ 
                Result = 1;
                break;

        default:
                OMIINO_FRAMER_RSE(Bandwidth);
                break;

    }

	return Result;
}





