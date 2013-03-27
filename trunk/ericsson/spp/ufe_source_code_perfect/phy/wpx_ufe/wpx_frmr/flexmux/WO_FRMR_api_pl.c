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


U8 WPX_FRMR_PL_PM_SONET_SDH_Port_GetMonitoringState(U8 iDevice, U8 iPresentationLinePort, U8 iPoint, U8 *pState)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_GetMonitoringState(iDevice, iLogicalLinePort, iPoint, pState);
		}
	}

    return Result;
}


U8 WPX_FRMR_PL_PM_SONET_SDH_HO_Path_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U8 *pState)
{
    U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_GetMonitoringState(iDevice, pLineEndpoint, iPoint, pState);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_PM_SONET_SDH_LO_Path_GetMonitoringState(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U8 *pState)
{
    U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_GetMonitoringState(iDevice, pLineEndpoint, iPoint, pState);
		}
	}

	return Result;
}







U8 WPX_FRMR_PL_PM_SONET_SDH_Port_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 iPoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_EnableMonitoring(TransactionIdentifier, iDevice, iLogicalLinePort, iPoint);
		}
	}

    return Result;
}

U8 WPX_FRMR_PL_PM_SONET_SDH_Port_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 iPoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_Port_DisableMonitoring(TransactionIdentifier, iDevice, iLogicalLinePort, iPoint);
		}
	}

    return Result;
}



U8 WPX_FRMR_PL_PM_SONET_SDH_HO_Path_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
    U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, pLineEndpoint, iPoint);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_PM_SONET_SDH_HO_Path_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_HighOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, pLineEndpoint, iPoint);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_PM_SONET_SDH_LO_Path_EnableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_EnableMonitoring(TransactionIdentifier, iDevice, pLineEndpoint, iPoint);
		}
	}

	return Result;

}




U8 WPX_FRMR_PL_PM_SONET_SDH_LO_Path_DisableMonitoring(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_PERFORMANCE_MONITORING_SONET_SDH_LowOrderPath_DisableMonitoring(TransactionIdentifier, iDevice, pLineEndpoint, iPoint);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_DEV_DIAG_DRV_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 iPresentationLinePort)
{
    U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Set(TransactionIdentifier, iDevice, OutputSelector, iLogicalLinePort);
		}
	}

    return Result;
}

U8 WPX_FRMR_PL_DEV_SQUELCH_RecClkSet(U32 TransactionIdentifier, U8 iDevice, U8 OutputSelector, U8 Squelch_Mode)
{
    U8 Result=WPX_UFE_FRAMER_OK;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_SquelchRecoveredClock_Set(TransactionIdentifier, iDevice, OutputSelector, Squelch_Mode);
	}

    return Result;
}


U8 WPX_FRMR_PL_DEV_DIAG_DRV_RecClkGet(U8 iDevice, U8 OutputSelector, U8 * piPresentationLinePort)
{
    U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;
	
	*piPresentationLinePort=0;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_DIAGNOSTIC_DriveRecoveredClock_Get(iDevice, OutputSelector, &iLogicalLinePort);
		if(WPX_UFE_FRAMER_OK==Result)
		{
			Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
			if(OMIINO_FRAMER_PortMap_Logical_iLinePort_To_Presentation_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iLogicalLinePort, piPresentationLinePort))
			{
				Result=WPX_UFE_FRAMER_OK;
			}
		}
	}

	return Result;
}

U8 WPX_FRMR_PL_DEV_SQUELCH_RecClkGet(U8 iDevice, U8 OutputSelector, U8* pSquelch_Mode)
{
    U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;	

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_DEVICE_SquelchRecoveredClock_Get(iDevice, OutputSelector, pSquelch_Mode);
	}

	return Result;

}


U8 WPX_FRMR_PL_SONET_SDH_CreateFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_CreateFacility(TransactionIdentifier, iDevice, pLineEndpoint);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_DeleteFacility(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DeleteFacility(TransactionIdentifier, iDevice, pLineEndpoint);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SFP_SetTxState(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort,U8 Tx_State)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SFP_SetTxState(TransactionIdentifier, iDevice, iLogicalLinePort, Tx_State);
		}
	}

	return Result;
}

U8 WPX_FRMR_PL_SONET_SDH_PORT_SetRate(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 PortRate,U8 Tx_Enable)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_MACRO_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SetRate(TransactionIdentifier, iDevice, iLogicalLinePort, PortRate,Tx_Enable);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_GetRate(U8 iDevice, U8 iPresentationLinePort, U8 * pPortRate)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetRate(iDevice, iLogicalLinePort, pPortRate);
		}
	}
	
	return Result;
}









U8 WPX_FRMR_PL_SONET_SDH_PORT_DCC_Enable(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 DCC_Mode)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Enable(TransactionIdentifier, iDevice, iLogicalLinePort, DCC_Mode);
		}
	}

	return Result;
}






U8 WPX_FRMR_PL_SONET_SDH_PORT_DCC_Disable(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 DCC_Mode)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Disable(TransactionIdentifier, iDevice, iLogicalLinePort, DCC_Mode);
		}
	}

	return Result;
}






U8 WPX_FRMR_PL_SONET_SDH_PORT_DCC_Get(U8 iDevice, U8 iPresentationLinePort, U8 DCC_Mode, U8 * pDCC_State)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DCC_Get(iDevice, iLogicalLinePort, DCC_Mode, pDCC_State);
		}
	}	

	return Result;
}















U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_SetMode(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 Mode)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetMode(TransactionIdentifier, iDevice, iLogicalLinePort, Mode);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_GetMode(U8 iDevice, U8 iPresentationLinePort, U8 * pMode)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetMode(iDevice, iLogicalLinePort, pMode);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_SetTX(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, char * pTX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetTX(TransactionIdentifier, iDevice, iLogicalLinePort, pTX_TraceString);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_SetEX(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, char * pEX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_SetEX(TransactionIdentifier, iDevice, iLogicalLinePort, pEX_TraceString);
		}
	}
	
	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_GetTX(U8 iDevice, U8 iPresentationLinePort, char * pTX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetTX(iDevice, iLogicalLinePort, pTX_TraceString);
		}
	}
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_J0_GetEX(U8 iDevice, U8 iPresentationLinePort, char * pEX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetEX(iDevice, iLogicalLinePort, pEX_TraceString);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetExcessiveThreshold(TransactionIdentifier, iDevice, iLogicalLinePort, Threshold);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_SetSignalDegradeTh(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, iLogicalLinePort, Threshold);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_GetExcessiveTh(U8 iDevice, U8 iPresentationLinePort, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GetExcessive(iDevice, iLogicalLinePort, pThreshold);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_SEC_B1_GetSigDegradeTh(U8 iDevice, U8 iPresentationLinePort, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_SECTION_B1_GetSignalDegradeThreshold(iDevice, iLogicalLinePort, pThreshold);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_K1K2_SetTx(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_SetTX(TransactionIdentifier, iDevice, iLogicalLinePort, pTX_K1K2);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_K1K2_GetTx(U8 iDevice, U8 iPresentationLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pTX_K1K2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GetTX(iDevice, iLogicalLinePort, pTX_K1K2);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_S1_SetTx(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 TX_S1)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_SetTX(TransactionIdentifier, iDevice, iLogicalLinePort, TX_S1);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_S1_GetTx(U8 iDevice, U8 iPresentationLinePort, U8 * pTX_S1)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_S1_GetTX(iDevice, iLogicalLinePort, pTX_S1);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetExcessiveThreshold(TransactionIdentifier, iDevice, iLogicalLinePort, Threshold);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, iLogicalLinePort, Threshold);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_GetExcessiveTh(U8 iDevice, U8 iPresentationLinePort, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GetExcessive(iDevice, iLogicalLinePort, pThreshold);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_LINE_B2_GetSigDegradeTh(U8 iDevice, U8 iPresentationLinePort, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_LINE_B2_GetSignalDegradeThreshold(iDevice, iLogicalLinePort, pThreshold);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_SetMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Mode)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetMode(TransactionIdentifier, iDevice, pLineEndpoint, Mode);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_GetMode(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pMode)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetMode(iDevice, pLineEndpoint, pMode);
		}
	}

	return Result;

}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, pTX_TraceString);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, pEX_TraceString);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetTX(iDevice, pLineEndpoint, pTX_TraceString);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_J1_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_J1_GetEX(iDevice, pLineEndpoint, pEX_TraceString);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_SetRdiMode(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 ERDI_MODE)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SetRdiMode(TransactionIdentifier, iDevice, pLineEndpoint, ERDI_MODE);
		}
	}

	return Result;
}

U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_SS_BITS_SET_TX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Value)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_SET_TX(TransactionIdentifier, iDevice, pLineEndpoint, Value);
		}
	}

	return Result;
}

U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_SS_BITS_GET_RX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_SS_BITS_GET_RX(TransactionIdentifier, iDevice, pLineEndpoint);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_C2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, TX_C2);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 EX_C2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, EX_C2);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pTX_C2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GetTX(iDevice, pLineEndpoint, pTX_C2);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_C2_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pEX_C2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_C2_GetEX(iDevice, pLineEndpoint, pEX_C2);
		}
	}

	return Result;
}






U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetExcessiveThreshold(TransactionIdentifier, iDevice, pLineEndpoint, Threshold);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, pLineEndpoint, Threshold);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_GetExcessiveTh(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GetExcessiveThreshold(iDevice, pLineEndpoint, pThreshold);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_HO_PATH_B3_GetSigDegradeTh(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_HO_PATH_B3_GetSignalDegradeThreshold(iDevice, pLineEndpoint, pThreshold);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, pTX_TraceString);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, pEX_TraceString);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pTX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GetTX(iDevice, pLineEndpoint, pTX_TraceString);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_J2_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pEX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_J2_GetEX(iDevice, pLineEndpoint, pEX_TraceString);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_SetTX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 TX_V5_SignalLabel)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetTX(TransactionIdentifier, iDevice, pLineEndpoint, TX_V5_SignalLabel);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_SetEX(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 EX_V5_SignalLabel)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_SetEX(TransactionIdentifier, iDevice, pLineEndpoint, EX_V5_SignalLabel);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_GetTX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pTX_V5_SignalLabel)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetTX(iDevice, pLineEndpoint, pTX_V5_SignalLabel);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_SigLabel_GetEX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pEX_V5_SignalLabel)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetEX(iDevice, pLineEndpoint, pEX_V5_SignalLabel);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetExcessiveThreshold(TransactionIdentifier, iDevice, pLineEndpoint,Threshold);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_SetSigDegradeTh(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 Threshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_SetSignalDegradeThreshold(TransactionIdentifier, iDevice, pLineEndpoint, Threshold);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveTh(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GetExcessiveThreshold(iDevice, pLineEndpoint, pThreshold);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_LO_PATH_V5_BIP2_GetSigDegradeTh(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pThreshold)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_LO_PATH_V5_BIP2_GetSignalDegradeThreshold(iDevice, pLineEndpoint, pThreshold);
		}
	}

	return Result;
}





U8 WPX_FRMR_PL_CONNECTIONS_ThroughCreate(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecondLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pFirstLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pFirstLineEndpoint->iLinePort=iLogicalLinePort;
			if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pSecondLineEndpoint->iLinePort, &iLogicalLinePort))
			{
				pSecondLineEndpoint->iLinePort=iLogicalLinePort;
				Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_CreateConnection(TransactionIdentifier, iDevice, pFirstLineEndpoint, pSecondLineEndpoint);
			}
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_CONNECTIONS_ThroughDelete(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pFirstLineEndpoint, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pSecondLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pFirstLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pFirstLineEndpoint->iLinePort=iLogicalLinePort;
			if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pSecondLineEndpoint->iLinePort, &iLogicalLinePort))
			{
				pSecondLineEndpoint->iLinePort=iLogicalLinePort;
				Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_THROUGH_DeleteConnection(TransactionIdentifier, iDevice, pFirstLineEndpoint, pSecondLineEndpoint);
			}
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_CONNECTIONS_AddDropLineToSocketCreate(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_CreateConnection(TransactionIdentifier, iDevice, pLineEndpoint, iSocketClientPDH);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_CONNECTIONS_AddDropLineToSocketDelete(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U32 iSocketClientPDH)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_CONNECTIONS_ADD_DROP_LINE_TO_CLIENT_SOCKET_DeleteConnection(TransactionIdentifier, iDevice, pLineEndpoint, iSocketClientPDH);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_PORT_ForceA(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_A(TransactionIdentifier, iDevice, iLogicalLinePort);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_ForceB(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_Force_B(TransactionIdentifier, iDevice, iLogicalLinePort);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_GetForceState(U8 iDevice, U8 iPresentationLinePort, U8 * pForceSide)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_GetForceState(iDevice, iLogicalLinePort, pForceSide);
		}
	}
	
	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkEnableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableShallowLine(TransactionIdentifier, iDevice, iLogicalLinePort);
		}
	}
	
	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkDisableShallowLine(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableShallowLine(TransactionIdentifier, iDevice, iLogicalLinePort);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkGetShallowLineState(U8 iDevice, U8 iPresentationLinePort, U8 * pLoopbackIsEnabled)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GetShallowLineState(iDevice, iLogicalLinePort, pLoopbackIsEnabled);
		}
	}
	
	return Result;
}





U8 WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkEnableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_EnableDeepSystem(TransactionIdentifier, iDevice, iLogicalLinePort);
		}
	}
	
	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkDisableDeepSystem(U32 TransactionIdentifier, U8 iDevice, U8 iPresentationLinePort)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_DisableDeepSystem(TransactionIdentifier, iDevice, iLogicalLinePort);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_PORT_DIAG_LpbkGetDeepSystemState(U8 iDevice, U8 iPresentationLinePort, U8 * pLoopbackIsEnabled)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_PORT_DIAGNOSTIC_LOOPBACK_GetDeepSystemState(iDevice, iLogicalLinePort, pLoopbackIsEnabled);
		}
	}
	
	return Result;
}




U8 WPX_FRMR_PL_SONET_SDH_DIAG_LpbkEnable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Enable(TransactionIdentifier, iDevice, pLineEndpoint);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_SONET_SDH_DIAG_LpbkDisable(U32 TransactionIdentifier, U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_Disable(TransactionIdentifier, iDevice, pLineEndpoint);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_SONET_SDH_DIAG_LpbkGetState(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pLoopbackIsEnabled)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_CONFIGURATION_FACILITIES_SONET_SDH_DIAGNOSTIC_LOOPBACK_GetState(iDevice, pLineEndpoint, pLoopbackIsEnabled);
		}
	}

	return Result;

}


U8 WPX_FRMR_PL_STATUS_SONET_SDH_PORT_SEC_J0_GetRX(U8 iDevice, U8 iPresentationLinePort, char * pRX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_SECTION_J0_GetRX(iDevice, iLogicalLinePort, pRX_TraceString);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_STATUS_SONET_SDH_PORT_LINE_S1_GetRX(U8 iDevice, U8 iPresentationLinePort, U8 * pRX_S1)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_LINE_S1_GetRX(iDevice, iLogicalLinePort, pRX_S1);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_STATUS_SONET_SDH_PORT_LINE_K1K2_GetRX(U8 iDevice, U8 iPresentationLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_PORT_LINE_K1K2_GetRX(iDevice, iLogicalLinePort, pRX_K1K2);
		}
	}
	
	return Result;
}




U8 WPX_FRMR_PL_STATUS_SONET_SDH_HO_PATH_J1_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pRX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_HO_PATH_J1_GetRX(iDevice, pLineEndpoint, pRX_TraceString);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_STATUS_SONET_SDH_HO_PATH_C2_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pRX_C2)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_HO_PATH_C2_GetRX(iDevice, pLineEndpoint, pRX_C2);
		}
	}

	return Result;
}



U8 WPX_FRMR_PL_STATUS_SONET_SDH_LO_PATH_J2_GetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, char * pRX_TraceString)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_LO_PATH_J2_GetRX(iDevice, pLineEndpoint, pRX_TraceString);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_STATUS_SONET_SDH_LO_PATH_V5_SigLabelGetRX(U8 iDevice, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 * pRX_V5_SignalLabel)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_STATUS_FACILITIES_SONET_SDH_LO_PATH_V5_SignalLabel_GetRX(iDevice, pLineEndpoint, pRX_V5_SignalLabel);
		}
	}

	return Result;
}




U8 WPX_FRMR_PL_STATUS_PM_SONET_SDH_Port_ReadData(U8 iDevice, U8 iTableIndex, U8 iPresentationLinePort, U8 iPoint, U32 * pCount)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), iPresentationLinePort, &iLogicalLinePort))
		{
			Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_Port_ReadData(iDevice, iTableIndex, iLogicalLinePort, iPoint, pCount);
		}
	}
	
	return Result;
}


U8 WPX_FRMR_PL_STATUS_PM_SONET_SDH_HO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U32 * pCount)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_HO_PATH_ReadData(iDevice, iTableIndex, pLineEndpoint, iPoint, pCount);
		}
	}

	return Result;
}


U8 WPX_FRMR_PL_STATUS_PM_SONET_SDH_LO_PATH_ReadData(U8 iDevice, U8 iTableIndex, WPX_UFE_FRAMER_SONET_SDH_ENDPOINT_TYPE * pLineEndpoint, U8 iPoint, U32 * pCount)
{
	U8 Result=WPX_UFE_FRAMER_ERROR_UNABLE_TO_MAP_LINE_PORT;
	U8 iLogicalLinePort;

	if(WPX_UFE_FRAMER_BUILD_OPTION_MAX_DEVICES>iDevice)
	{
		if(OMIINO_FRAMER_PortMap_Presentation_iLinePort_To_Logical_iLinePort(&(OMIINO_RAM.Device[iDevice].PortMap), pLineEndpoint->iLinePort, &iLogicalLinePort))
		{
			pLineEndpoint->iLinePort=iLogicalLinePort;
			Result=OMIINO_API_INTERNAL_STATUS_PERFORMANCE_MONITORING_SONET_SDH_LO_PATH_ReadData(iDevice, iTableIndex, pLineEndpoint, iPoint, pCount);
		}
	}

	return Result;
}

