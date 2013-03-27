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

#include "WO_FRMR_vga_driver_public.h"




void Environment_Screen_PM_Port_Header(char * pAnyMessage, U32 AnyValue)
{
	Environment_ReportLoggerAddItem_U32(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING],  pAnyMessage, VGA_TXT_COLOUR_WHITE, AnyValue);
}

void Environment_Screen_PM_Port_Report(char iLinePort, char iPoint, U32 AnyValue)
{
	switch(iPoint)
	{
		case	WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B1:
				Environment_ReportLoggerAddItem_U32(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING],  "B1 (port) :", VGA_TXT_COLOUR_WHITE, iLinePort);
				break;

		case	WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_B2:
				Environment_ReportLoggerAddItem_U32(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING],  "B2 (port) :", VGA_TXT_COLOUR_WHITE, iLinePort);
				break;

		case	WPX_UFE_FRAMER_SONET_SDH_PERFORMANCE_MONITORING_DATA_POINT_M1:
				Environment_ReportLoggerAddItem_U32(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING],  "M1 (port) :", VGA_TXT_COLOUR_WHITE, iLinePort);
				break;

		default:
				Environment_ReportLoggerAddItem_U32(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING],  "?? (port) :", VGA_TXT_COLOUR_WHITE, iLinePort);
				break;
	}
	Environment_ReportLoggerAddItem_U32(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING],  "Count     :", VGA_TXT_COLOUR_WHITE, AnyValue);

	
}

void Environment_Screen_PM_Initialize(void)
{
	Environment_ReportLoggerInitialize(&Environment_Screen.Logger[PAGE_PERFORMANCE_MONITORING], PAGE_PERFORMANCE_MONITORING, 0);
}
