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





U8 OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointIsLowOrder(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pAnyEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	if(!OMIINO_FRAMER_SONET_SDH_Endpoint_BandwidthIsLowOrder(pAnyEndpoint->Bandwidth))
	{
		Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
	}

	return Result;
}





U8 OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreDifferent(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecondLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	if(OMIINO_FRAMER_SONET_SDH_ENDPOINTS_AreIdentical(pFirstLineEndpoint, pSecondLineEndpoint))
	{
		Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
	}

	return Result;
}


U8 OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreSameType(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecondLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_OK;

	if(pFirstLineEndpoint->Bandwidth!=pSecondLineEndpoint->Bandwidth)
	{
		Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
	}

	return Result;
}




U8 OMIINO_FRAMER_SONET_ENDPOINTS_AreSame_UVW(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
    U8 Result=1;

    if(pFirst->Bandwidth!=pSecond->Bandwidth)
    {
        Result=0;
    }


    if(Result)
    {
        if(pFirst->u.sonet.iU!=pSecond->u.sonet.iU)
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(pFirst->u.sonet.iV!=pSecond->u.sonet.iV)
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(pFirst->u.sonet.iW!=pSecond->u.sonet.iW)
        {
            Result=0;
        }
    }

    return Result;
}



U8 OMIINO_FRAMER_SDH_ENDPOINTS_AreSame_JKLM(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
    U8 Result=1;

    if(pFirst->Bandwidth!=pSecond->Bandwidth)
    {
        Result=0;
    }

    if(Result)
    {
        if(pFirst->u.sdh.iJ!=pSecond->u.sdh.iJ)
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(pFirst->u.sdh.iK!=pSecond->u.sdh.iK)
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(pFirst->u.sdh.iL!=pSecond->u.sdh.iL)
        {
            Result=0;
        }
    }

    if(Result)
    {
        if(pFirst->u.sdh.iM!=pSecond->u.sdh.iM)
        {
            Result=0;
        }
    }

    return Result;
}







U8 OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsAreSame_JKLM_UVW(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
	U8 Result=WPX_UFE_FRAMER_OK;

    if(OMIINO_FRAMER_Is_SONET_Endpoint(pFirst))
    {
        if(OMIINO_FRAMER_Is_SONET_Endpoint(pSecond))
        {
            if(!OMIINO_FRAMER_SONET_ENDPOINTS_AreSame_UVW(pFirst,pSecond))
			{
	            Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
			}
		}
        else
        {
            Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
        }
    }
    else
    {
        if(OMIINO_FRAMER_Is_SDH_Endpoint(pSecond))
        {
            if(!OMIINO_FRAMER_SDH_ENDPOINTS_AreSame_JKLM(pFirst,pSecond))
			{
	            Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
			}
        }
        else
        {
            Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
        }
    }

	return Result;
}



/*
 * DMCC Through Connection Rules 2010-06-09:
 *
 * Through Pairs are separater by 2:-
 *
 * Ports 1 & 3
 * Ports 2 & 4
 * Ports 5 & 7
 * Ports 6 & 8
 *
 */

U8 OMIINO_FRAMER_ConnectionsStaticRule_SONET_SDH_EndpointsArePortPairs(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
	U8 Result=WPX_UFE_FRAMER_OK;

    if(pFirst->iLinePort!=pSecond->iLinePort)
    {
        if(pFirst->iLinePort>pSecond->iLinePort)
		{
			if(2!=((pFirst->iLinePort)-(pSecond->iLinePort)))
			{
	            Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
			}
		}
		else
		{
			if(2!=((pSecond->iLinePort)-(pFirst->iLinePort))) 
			{
	            Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
			}
		}
    }
	else
	{
        Result=WPX_UFE_FRAMER_ERROR_CONNECTION_FACILITY_MISMATCH;
	}


	return Result;
}




