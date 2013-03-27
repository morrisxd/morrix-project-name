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





void Environment_Screen_Signalling_S1_Report(char iLinePort, char Any_S1)
{
	Environment_ReportLoggerAddItem_U8(&Environment_Screen.Logger[PAGE_SIGNALLING], "iLinePort :", VGA_TXT_COLOUR_WHITE, iLinePort);
	Environment_ReportLoggerAddItem_U8(&Environment_Screen.Logger[PAGE_SIGNALLING], "S1        :", VGA_TXT_COLOUR_WHITE, Any_S1);
}


void Environment_Screen_Signalling_K1K2_Report(char iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2)
{
	Environment_ReportLoggerAddItem_U8(&Environment_Screen.Logger[PAGE_SIGNALLING], "iLinePort :", VGA_TXT_COLOUR_YELLOW, iLinePort);
	Environment_ReportLoggerAddItem_U8(&Environment_Screen.Logger[PAGE_SIGNALLING], "K1        :", VGA_TXT_COLOUR_YELLOW, pRX_K1K2->K1);
	Environment_ReportLoggerAddItem_U8(&Environment_Screen.Logger[PAGE_SIGNALLING], "K2        :", VGA_TXT_COLOUR_YELLOW, pRX_K1K2->K2);
}


void Environment_Screen_Signalling_Initialize(void)
{
	Environment_ReportLoggerInitialize(&Environment_Screen.Logger[PAGE_SIGNALLING], PAGE_SIGNALLING, 1);
}
