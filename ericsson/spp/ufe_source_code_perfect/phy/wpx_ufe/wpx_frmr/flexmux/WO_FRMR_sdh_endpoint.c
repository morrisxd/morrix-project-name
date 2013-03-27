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

void WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_Bandwidth_ToTxt(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * AnyEndpoint, char * pBuffer)
{
	switch(AnyEndpoint->Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_NULL: 
				strcat(pBuffer, "NULL ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
				strcat(pBuffer, "VC4-4C ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
					strcat(pBuffer, "VC4 ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI: 
				strcat(pBuffer, "VC3-HI ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
				strcat(pBuffer, "VC3-LO ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:   
				strcat(pBuffer, "VC12 ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11: 
				strcat(pBuffer, "VC11 ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C: 
				strcat(pBuffer, "STS48C ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C: 
				strcat(pBuffer, "STS12C ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:  
				strcat(pBuffer, "STS3C ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1: 
				strcat(pBuffer, "STS1 ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:  
				strcat(pBuffer, "VT2 ");
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
				strcat(pBuffer, "VT15 ");
				break;

		default:
				strcat(pBuffer, "???");
				break;
	}
}

void WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_Port_ToTxt(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * AnyEndpoint, char * pBuffer)
{
	sprintf(pBuffer, "P-%02d ", 1+AnyEndpoint->iLinePort);
}

void WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_SDH_ToTxt(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * AnyEndpoint, char * pBuffer)
{
	char LocalBuffer[25];

	switch(AnyEndpoint->Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
				sprintf(LocalBuffer, "J-%d ", 1+AnyEndpoint->u.sdh.iJ);
				break;

		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI: 
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
				sprintf(LocalBuffer, "J-%d K-%d ", 1+AnyEndpoint->u.sdh.iJ, 1+AnyEndpoint->u.sdh.iK);
				break;

		default:
				sprintf(LocalBuffer, "J-%d K-%d L-%d M-%d ", 1+AnyEndpoint->u.sdh.iJ, 1+AnyEndpoint->u.sdh.iK, 1+AnyEndpoint->u.sdh.iL, 1+AnyEndpoint->u.sdh.iM);
				break;
	}

	strcat(pBuffer,LocalBuffer);
}

void WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_SONET_ToTxt(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * AnyEndpoint, char * pBuffer)
{
	char LocalBuffer[20];

	switch(AnyEndpoint->Bandwidth)
	{
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C: 
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C: 
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:  
		case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
				sprintf(LocalBuffer, "U-%d ", 1+AnyEndpoint->u.sonet.iU);
				break;

		default:
				sprintf(LocalBuffer, "U-%d V-%d W-%d", 1+AnyEndpoint->u.sonet.iU, 1+AnyEndpoint->u.sonet.iV, 1+AnyEndpoint->u.sonet.iW);
				break;
	}
	strcat(pBuffer,LocalBuffer);
}

void WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_ToTxt(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * AnyEndpoint, char * pBuffer)
{
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_Port_ToTxt(AnyEndpoint, pBuffer);
	WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_Bandwidth_ToTxt(AnyEndpoint, pBuffer);

	if(OMIINO_FRAMER_Is_SDH_Endpoint(AnyEndpoint))
	{
		WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_SDH_ToTxt(AnyEndpoint, pBuffer);
	}
	else
	{
		WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_SONET_ToTxt(AnyEndpoint, pBuffer);
	}

}


U8 OMIINO_FRAMER_SONET_ENDPOINTS_AreIdentical(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
    U8 Result=1;

    if(pFirst->Bandwidth!=pSecond->Bandwidth)
    {
        Result=0;
    }

    if(Result)
    {
        if(pFirst->iLinePort!=pSecond->iLinePort)
        {
            Result=0;
        }
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

U8 OMIINO_FRAMER_SDH_ENDPOINTS_AreIdentical(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
    U8 Result=1;

    if(pFirst->Bandwidth!=pSecond->Bandwidth)
    {
        Result=0;
    }

    if(Result)
    {
        if(pFirst->iLinePort!=pSecond->iLinePort)
        {
            Result=0;
        }
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


U8 OMIINO_FRAMER_SONET_SDH_ENDPOINTS_AreIdentical(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirst, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecond)
{
    U8 Result=1;

    if(OMIINO_FRAMER_Is_SONET_Endpoint(pFirst))
    {
        if(OMIINO_FRAMER_Is_SONET_Endpoint(pSecond))
        {
            Result=OMIINO_FRAMER_SONET_ENDPOINTS_AreIdentical(pFirst,pSecond);
        }
        else
        {
            Result=0;
        }
    }
    else
    {
        if(OMIINO_FRAMER_Is_SDH_Endpoint(pSecond))
        {
            Result=OMIINO_FRAMER_SDH_ENDPOINTS_AreIdentical(pFirst,pSecond);
        }
        else
        {
            Result=0;
        }
    }

    return Result;
}

void OMIINO_FRAMER_SONET_SDH_ENDPOINT_Clone(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pTo, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFrom)
{
    memcpy(pTo, pFrom, sizeof(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE));
}


