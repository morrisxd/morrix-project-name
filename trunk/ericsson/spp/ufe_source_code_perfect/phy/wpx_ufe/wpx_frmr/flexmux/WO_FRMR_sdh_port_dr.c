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







U8 OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsBandwidthType(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 Bandwidth)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(pPortHierarchy->PortMode)
    {
    
		case    WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				switch(Bandwidth)
				{    
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  /* Drop through */ 
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_BANDWIDTH_NOT_SUPPORTED_IN_DEVICE_MODE;
							break;

				}
				break;

        case    WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				switch(Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS48C:	/* Drop through */ 
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_BANDWIDTH_NOT_SUPPORTED_IN_DEVICE_MODE;
							break;
				}               
				break;

        default: 
				OMIINO_FRAMER_RSE(pPortHierarchy->PortMode);
                break;
    }

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_SupportsRequiredBandwidth(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 Bandwidth)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(pPortHierarchy->Rate)
    {    
		case    WPX_UFE_FRAMER_LINE_PORT_RATE_OFF:
				Result = WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED;
				break;

		case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM4:
				switch(Bandwidth)
				{    
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4_4C:  /* Drop through */ 
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_BANDWIDTH_EXCEEDS_PORT_RATE;
							break;

				}
				break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM1: /* Drop through */ 
				switch(Bandwidth)
				{    
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC4:     /* Drop through */
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_HI :
					case    WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC3_LO:  
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC12:    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SDH_VC11:
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_BANDWIDTH_EXCEEDS_PORT_RATE;
							break;

				}
				break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC12:
				switch(Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS12C: /* Drop through */ 
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_BANDWIDTH_EXCEEDS_PORT_RATE;
							break;
				}  
				break;

        case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC3:	
				switch(Bandwidth)
				{    
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS3C:	/* Drop through */ 
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_STS1:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT2:
					case	WPX_UFE_FRAMER_LINE_BANDWIDTH_SONET_VT15:		
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_BANDWIDTH_EXCEEDS_PORT_RATE;
							break;
				}  
				break;

        default:
                Result=WPX_UFE_FRAMER_ERROR_PORT_RATE_OUT_OF_RANGE;
                break;

    }

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_ModeSupportsRate(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy, U8 PortRate)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	switch(pPortHierarchy->PortMode)
    {
    
		case    WPX_UFE_FRAMER_DEVICE_MODE_SDH:
				switch(PortRate)
				{    
					case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM1:	/* Drop through */ 
					case    WPX_UFE_FRAMER_LINE_PORT_RATE_STM4:
					case    WPX_UFE_FRAMER_LINE_PORT_RATE_OFF:	  
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_RATE_NOT_SUPPORTED_IN_DEVICE_MODE;
							break;
				}
				break;

        case    WPX_UFE_FRAMER_DEVICE_MODE_SONET:
				switch(PortRate)
				{    
					case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC3:	/* Drop through */ 
					case    WPX_UFE_FRAMER_LINE_PORT_RATE_OC12:
					case    WPX_UFE_FRAMER_LINE_PORT_RATE_OFF:	  
							Result = WPX_UFE_FRAMER_OK;
							break;

					default:
							Result = WPX_UFE_FRAMER_ERROR_RATE_NOT_SUPPORTED_IN_DEVICE_MODE;
							break;
				}               
				break;

        default: 
				OMIINO_FRAMER_RSE(pPortHierarchy->PortMode);
                break;
    }

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_RateIsConfigured(OMIINO_FRAMER_CONFIGURATION_SONET_SDH_PORT_TYPE * pPortHierarchy)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if (pPortHierarchy->Rate==WPX_UFE_FRAMER_LINE_PORT_RATE_OFF)
	{
		Result = WPX_UFE_FRAMER_ERROR_PORT_RATE_NOT_CONFIGURED;
	}

	return Result;
}





U8 OMIINO_FRAMER_SONET_SDH_Port_DynamicRule_PortInRange(U8 iDevice, U8 iLinePort)
{
	U8 Result = WPX_UFE_FRAMER_OK;

	if(OMIINO_DEVICE_PERSONALITY_NumberOfLinePorts(iDevice)<=iLinePort)
	{
		Result = WPX_UFE_FRAMER_ERROR_LINE_PORT_OUT_OF_RANGE;
	}

	return Result;
}




