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




/*
 *
 * TODO WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C
 *
 */

















void WPX_UFE_FRAMER_PORTING_MAP_BandwidthToInternal(WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pTo, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFrom)
{
   switch(pFrom->TransferType)
   {
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11;
         break;

      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12;
         break;

      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI;
         break;

      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO;
         break;

      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4;
         break;

      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C;
         break;

      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15;
         break;

      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2;
         break;

      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1;
         break;

      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C;
         break;

      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12:
         pTo->Bandwidth=WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C;
         break;

      default:
         OMIINO_FRAMER_RSE(pFrom->TransferType);
         break;
   }
}








U8 WPX_UFE_FRAMER_PORTING_MAP_SDH_EndpointToInternal(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pTo, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFrom)
{
   U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;

   if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
   {
      Result=WPX_UFE_FRAMER_OK;
      switch(OMIINO_RAM.Device[iDevice].BuildPersonality)
      {
         case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
            pTo->iLinePort=pFrom->u.SDH.stm4;
            pTo->u.sdh.iJ=pFrom->u.SDH.stm1;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:
            pTo->iLinePort=4*(pFrom->u.SDH.stm4/2) + pFrom->u.SDH.stm1;
            pTo->u.sdh.iJ=0;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
         case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
            pTo->iLinePort=pFrom->u.SDH.stm1;
            pTo->u.sdh.iJ=0;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
            pTo->iLinePort=0;
            pTo->u.sdh.iJ=pFrom->u.SDH.stm1;
            break;

         default:
            Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
            break;
      }

      pTo->u.sdh.iK=pFrom->u.SDH.stm0;
      pTo->u.sdh.iL=pFrom->u.SDH.tug2;
      pTo->u.sdh.iM=pFrom->u.SDH.tu;
   }

   return Result;
}


U8 WPX_UFE_FRAMER_PORTING_MAP_SONET_EndpointToInternal(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pTo, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFrom)
{
   U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;

   if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
   {
      Result=WPX_UFE_FRAMER_OK;
      switch(OMIINO_RAM.Device[iDevice].BuildPersonality)
      {
         case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
            pTo->iLinePort=pFrom->u.SONET.sts12;
            pTo->u.sonet.iU=(3*pFrom->u.SONET.sts3)+pFrom->u.SONET.sts1;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:
            pTo->iLinePort=(4 * (pFrom->u.SONET.sts12/2)) + pFrom->u.SONET.sts3;
            pTo->u.sonet.iU=pFrom->u.SONET.sts1;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
         case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
            pTo->iLinePort=pFrom->u.SONET.sts3;
            pTo->u.sonet.iU=pFrom->u.SONET.sts1;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
            pTo->iLinePort=0;
            pTo->u.sonet.iU=(3*pFrom->u.SONET.sts3)+pFrom->u.SONET.sts1;
            break;

         default:
            Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
            break;
      }

      pTo->u.sonet.iV=pFrom->u.SONET.vt_group;
      pTo->u.sonet.iW=pFrom->u.SONET.vt;

   }

   return Result;
}





U8 WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_EndpointToPresentation(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pTo, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFrom)
{
   U8 Result=WPX_UFE_FRAMER_OK;

   switch(pFrom->TransferType)
   {
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11:
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12:
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3:
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3:
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4:
      case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C:
         WPX_UFE_FRAMER_PORTING_MAP_BandwidthToInternal(pTo, pFrom);
         Result=WPX_UFE_FRAMER_PORTING_MAP_SDH_EndpointToInternal(iDevice, pTo, pFrom);
         break;

      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5:
      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2:
      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE:
      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3:
      case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12:
         WPX_UFE_FRAMER_PORTING_MAP_BandwidthToInternal(pTo, pFrom);
         Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_EndpointToInternal(iDevice, pTo, pFrom);
         break;

      default:
         Result=WPX_UFE_FRAMER_ERROR_COMMON_INTERFACE;
         break;
   }

   return Result;
}






void WPX_UFE_FRAMER_PORTING_MAP_BandwidthToExternal(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE  *pTo, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFrom)
{
   switch(pFrom->Bandwidth)
   {
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3;
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
         pTo->TransferType=WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12;
         break;

      default:
         OMIINO_FRAMER_RSE(pFrom->Bandwidth);
         break;
   }
}





U8 WPX_UFE_FRAMER_PORTING_MAP_SDH_EndpointToExternal(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE  *pTo, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFrom)
{
   U8 Result=WPX_UFE_FRAMER_OK;

   switch(OMIINO_RAM.Device[iDevice].BuildPersonality)
   {
      case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
         pTo->u.SDH.stm4=pFrom->iLinePort;
         pTo->u.SDH.stm1=pFrom->u.sdh.iJ;
         break;

      case WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:
         pTo->u.SDH.stm4=2 * (pFrom->iLinePort/4);
         pTo->u.SDH.stm1=pFrom->iLinePort%4;
         break;
         
      case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
      case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
         pTo->u.SDH.stm4=0;
         pTo->u.SDH.stm1=pFrom->iLinePort;
         break;

      case WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
         pTo->u.SDH.stm4=0;
         pTo->u.SDH.stm1=pFrom->u.sdh.iJ;
         break;

      default:
         Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
         break;
   }


   pTo->u.SDH.stm0=pFrom->u.sdh.iK;
   pTo->u.SDH.tug2=pFrom->u.sdh.iL;
   pTo->u.SDH.tu=pFrom->u.sdh.iM;

   return Result;
}


U8 WPX_UFE_FRAMER_PORTING_MAP_SONET_EndpointToExternal(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE  *pTo, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFrom)
{
   U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;

   if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
   {
      Result=WPX_UFE_FRAMER_OK;
      switch(OMIINO_RAM.Device[iDevice].BuildPersonality)
      {
         case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3_OC12:
            pTo->u.SONET.sts12=pFrom->iLinePort;
            pTo->u.SONET.sts3=((pFrom->u.sonet.iU)/3);
            pTo->u.SONET.sts1=((pFrom->u.sonet.iU)%3);
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_8_PLUS_8_OC3:
            pTo->u.SONET.sts12=2 * (pFrom->iLinePort/4);
            pTo->u.SONET.sts3=(pFrom->iLinePort)%4;
            pTo->u.SONET.sts1=pFrom->u.sonet.iU;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_2_PLUS_2_OC3:
         case WPX_UFE_FRAMER_BUILD_PERSONALITY_4_PLUS_4_OC3:
	    pTo->u.SONET.sts12=0;
	    pTo->u.SONET.sts3=pFrom->iLinePort;
            pTo->u.SONET.sts1=pFrom->u.sonet.iU;
            break;

         case WPX_UFE_FRAMER_BUILD_PERSONALITY_1_PLUS_1_OC3_OC12:
            pTo->u.SONET.sts12=0;
            pTo->u.SONET.sts3=((pFrom->u.sonet.iU)/3);
            pTo->u.SONET.sts1=((pFrom->u.sonet.iU)%3);
            break;

         default:
            Result=WPX_UFE_FRAMER_ERROR_BUILD_PERSONALITY_NOT_RECOGNISED;
            break;
      }

      pTo->u.SONET.vt_group=pFrom->u.sonet.iV;
      pTo->u.SONET.vt=pFrom->u.sonet.iW;

   }

   return Result;
}





U8 WPX_UFE_FRAMER_PORTING_MAP_SONET_SDH_PresentationEndpointToExternal(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE  *pTo, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE *pFrom)
{
   U8 Result=WPX_UFE_FRAMER_OK;

   switch(pFrom->Bandwidth)
   {
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
         WPX_UFE_FRAMER_PORTING_MAP_BandwidthToExternal(pTo, pFrom);
         Result=WPX_UFE_FRAMER_PORTING_MAP_SDH_EndpointToExternal(iDevice, pTo, pFrom);
         break;

      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
      case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:
         WPX_UFE_FRAMER_PORTING_MAP_BandwidthToExternal(pTo, pFrom);
         Result=WPX_UFE_FRAMER_PORTING_MAP_SONET_EndpointToExternal(iDevice, pTo, pFrom);
         break;

      default:
         Result=WPX_UFE_FRAMER_ERROR_COMMON_INTERFACE;
         break;
   }

   return Result;
}



U8 WPX_UFE_FRAMER_PORTING_SDH_CommonEndpointsAreIdentical(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFirstCommonEndpoint, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pSecondCommonEndpoint)
{
   U8 Result=1;

   if(pFirstCommonEndpoint->u.SDH.stm0!=pSecondCommonEndpoint->u.SDH.stm0)
   {
      Result=0;
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SDH.stm1!=pSecondCommonEndpoint->u.SDH.stm1)
      {
         Result=0;
      }
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SDH.stm4!=pSecondCommonEndpoint->u.SDH.stm4)
      {
         Result=0;
      }
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SDH.tu!=pSecondCommonEndpoint->u.SDH.tu)
      {
         Result=0;
      }
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SDH.tug2!=pSecondCommonEndpoint->u.SDH.tug2)
      {
         Result=0;
      }
   }



   return Result;
}


U8 WPX_UFE_FRAMER_PORTING_SONET_CommonEndpointsAreIdentical(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFirstCommonEndpoint, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pSecondCommonEndpoint)
{
   U8 Result=1;

   if(pFirstCommonEndpoint->u.SONET.sts1!=pSecondCommonEndpoint->u.SONET.sts1)
   {
      Result=0;
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SONET.sts12!=pSecondCommonEndpoint->u.SONET.sts12)
      {
         Result=0;
      }
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SONET.sts3!=pSecondCommonEndpoint->u.SONET.sts3)
      {
         Result=0;
      }
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SONET.vt!=pSecondCommonEndpoint->u.SONET.vt)
      {
         Result=0;
      }
   }

   if(1==Result)
   {
      if(pFirstCommonEndpoint->u.SONET.vt_group!=pSecondCommonEndpoint->u.SONET.vt_group)
      {
         Result=0;
      }
   }

   return Result;
}


U8 WPX_UFE_FRAMER_PORTING_CommonEndpointsAreIdentical(WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pFirstCommonEndpoint, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE *pSecondCommonEndpoint)
{
   U8 Result=1;

   if(pFirstCommonEndpoint->TransferType!=pSecondCommonEndpoint->TransferType)
   {
      Result=0;
   }

   if(Result)
   {
      switch(pFirstCommonEndpoint->TransferType)
      {
         case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC11:
         case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC12:
         case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_TU3:
         case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC3:
         case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4:
         case    WPX_UFE_FRAMER_WUFE_SDH_TYPE_VC4_4C:
            Result=WPX_UFE_FRAMER_PORTING_SDH_CommonEndpointsAreIdentical(pFirstCommonEndpoint, pSecondCommonEndpoint);
            break;

         case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT1_5:
         case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_VT2:
         case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS1_SPE:
         case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS3:
         case    WPX_UFE_FRAMER_WUFE_SONET_TYPE_STS12:
            Result=WPX_UFE_FRAMER_PORTING_SONET_CommonEndpointsAreIdentical(pFirstCommonEndpoint, pSecondCommonEndpoint);
            break;

         default:
            Result=0;
            break;
      }

   }

   return Result;
}

