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


U8 OMIINO_LLC_Northbound_Utility_Parse_WPX_UFE_FRAMER_SONET_ENDPOINT_TYPE(U8 * pMessage, U8 BytesRemaining, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  * pLineEndpoint, U8 * pBytesParsed)
{
    U8  Result=1;
    U8  iNextU8=0;

    if(3>BytesRemaining)
    {
        Result=0;
    }
    else
    {
        pLineEndpoint->u.sonet.iU=pMessage[iNextU8++];
        pLineEndpoint->u.sonet.iV=pMessage[iNextU8++];
        pLineEndpoint->u.sonet.iW=pMessage[iNextU8++];
    }
    *pBytesParsed=iNextU8;


	if(1==Result)
	{
		if(WPX_UFE_FRAMER_OK!=OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint))
		{
			Result=0;
		}
	}

    return Result;
}




U8 OMIINO_LLC_Northbound_Utility_Parse_WPX_UFE_FRAMER_SDH_ENDPOINT_TYPE(U8 * pMessage, U8 BytesRemaining, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  * pLineEndpoint, U8 * pBytesParsed)
{
    U8  Result=1;
    U8  iNextU8=0;

    if(4>BytesRemaining)
    {
        Result=0;
    }
    else
    {
        pLineEndpoint->u.sdh.iJ=pMessage[iNextU8++];
        pLineEndpoint->u.sdh.iK=pMessage[iNextU8++];
        pLineEndpoint->u.sdh.iL=pMessage[iNextU8++];
        pLineEndpoint->u.sdh.iM=pMessage[iNextU8++];
    }
    *pBytesParsed=iNextU8;


	if(1==Result)
	{
		if(WPX_UFE_FRAMER_OK!=OMIINO_FRAMER_SONET_SDH_LineEndpointStaticRule_EndpointInRange(pLineEndpoint))
		{
			Result=0;
		}
	}

    return Result;
}


U8 OMIINO_LLC_Northbound_Utility_Encode_SONET_SDH_LineEndpoint(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  * pLineEndpoint, U8 * pEncodedLineEndpoint)
{
    U8  Result=0;
   
	pEncodedLineEndpoint[Result++]=pLineEndpoint->iLinePort;
	pEncodedLineEndpoint[Result++]=pLineEndpoint->Bandwidth;

    if(OMIINO_FRAMER_Is_SONET_Endpoint(pLineEndpoint))
    {
        pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sonet.iU;
        pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sonet.iV;
        pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sonet.iW;
    }
    else
    {
        pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sdh.iJ;
    	pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sdh.iK;
        pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sdh.iL;
		pEncodedLineEndpoint[Result++]=pLineEndpoint->u.sdh.iM;
	}

    return Result;
}

U8 OMIINO_LLC_Northbound_Utility_Parse_SONET_SDH_LineEndpoint(U8 * pMessage, U8 BytesRemaining, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE  * pLineEndpoint, U8 * pBytesParsed)
{
    U8  Result=1;
    U8  iNextU8=0;
    U8  LocalBytesParsed=0;

    if(3>BytesRemaining)
    {
        Result=0;
    }

    if(Result)
    {
        pLineEndpoint->iLinePort=pMessage[iNextU8++];
        pLineEndpoint->Bandwidth=pMessage[iNextU8++];
    }

    if(Result)
    {
		switch(pLineEndpoint->Bandwidth)
		{    
			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  /* Drop through */ 
			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
			case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
					Result=OMIINO_LLC_Northbound_Utility_Parse_WPX_UFE_FRAMER_SDH_ENDPOINT_TYPE(&pMessage[iNextU8], (BytesRemaining-iNextU8), pLineEndpoint, &LocalBytesParsed);
					break;

			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:	/* Drop through */ 
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
			case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
					Result=OMIINO_LLC_Northbound_Utility_Parse_WPX_UFE_FRAMER_SONET_ENDPOINT_TYPE(&pMessage[iNextU8], (BytesRemaining-iNextU8), pLineEndpoint, &LocalBytesParsed);
					break;

			default:
					Result=0;
					OMIINO_FRAMER_RSE(pLineEndpoint->Bandwidth);
					break;

		}
           
        iNextU8+=LocalBytesParsed;
    }
    *pBytesParsed=iNextU8;

    return Result;
}


/* Warning: Assume Path Trace Occupies all bytes remaining. */

U8 OMIINO_LLC_Northbound_Utility_Parse_PathTrace(U8 * pMessage, U8 BytesRemaining, char * pTo, U8 * pBytesParsed)
{
    U8  Result=1;

    if(0==BytesRemaining)
    {
        Result=1;
        *pBytesParsed=0;
    }

    if(Result)
    {
        if(64<BytesRemaining)
        {
            Result=0;
        }
        else
        {
            memcpy(pTo, pMessage, BytesRemaining); 
            *pBytesParsed=BytesRemaining;
        }
    }

    return Result;
}

