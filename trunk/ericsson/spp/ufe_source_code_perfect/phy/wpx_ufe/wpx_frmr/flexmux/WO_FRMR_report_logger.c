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



void Environment_ReportLoggerInitializeItem(ENVIRONMENT_REPORT_LOGGER_ITEM_TYPE * pAnyReportLoggerItem)
{
	pAnyReportLoggerItem->U32_Item=0;
	pAnyReportLoggerItem->U8_Item=0;
	pAnyReportLoggerItem->Colour=VGA_TXT_COLOUR_WHITE;
	pAnyReportLoggerItem->IsValid=0;

	memset(pAnyReportLoggerItem->Text,'\0', 1+MAX_CHARS_IN_LOGGER_ITEM_TEXT);
}

void Environment_ReportLoggerInitialize(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger, char AnyHandle, char Use_U8)
{
	int i;

	pAnyReportLogger->iHandle=AnyHandle;
	pAnyReportLogger->Use_U8=Use_U8;
	pAnyReportLogger->iWrite=0;
	pAnyReportLogger->Magic4=0xAAAAAAA4;

	for(i=0;i<MAX_REPORT_ITEMS; i++)
	{
		Environment_ReportLoggerInitializeItem(&(pAnyReportLogger->Item[i]));
	}
}

void Environment_ReportLoggerAddItem_U8(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger, char *pMessage, unsigned short AnyColour, U8 AnyValue)
{
	int iPrevWrite;

	if(0==pAnyReportLogger->iWrite)
	{
		iPrevWrite=MAX_REPORT_ITEMS-1;	
	}
	else
	{
		iPrevWrite=(pAnyReportLogger->iWrite)-1;
	}

	strncpy(pAnyReportLogger->Item[pAnyReportLogger->iWrite].Text,pMessage,MAX_CHARS_IN_LOGGER_ITEM_TEXT);
	pAnyReportLogger->Item[pAnyReportLogger->iWrite].U8_Item=AnyValue;
	pAnyReportLogger->Item[pAnyReportLogger->iWrite].Colour=AnyColour;
	pAnyReportLogger->Item[pAnyReportLogger->iWrite].IsValid=1;

	if(pAnyReportLogger->iHandle==Environment_Screen.Page)
	{
		if(pAnyReportLogger->Item[iPrevWrite].IsValid)
		{
			Environment_Screen_ReportItem_U8(iPrevWrite, pAnyReportLogger->Item[iPrevWrite].Colour, pAnyReportLogger->Item[iPrevWrite].Text, pAnyReportLogger->Item[iPrevWrite].U8_Item);
		}
		Environment_Screen_ReportItem_U8(pAnyReportLogger->iWrite, VGA_TXT_COLOUR_BLUE, pAnyReportLogger->Item[pAnyReportLogger->iWrite].Text, pAnyReportLogger->Item[pAnyReportLogger->iWrite].U8_Item);
	}

	pAnyReportLogger->iWrite++;
	if(MAX_REPORT_ITEMS<=pAnyReportLogger->iWrite)
	{
		pAnyReportLogger->iWrite=0;
	}
}



void Environment_ReportLoggerAddItem_U32(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger, char *pMessage, unsigned short AnyColour, U32 AnyValue)
{
	int iPrevWrite;

	if(0==pAnyReportLogger->iWrite)
	{
		iPrevWrite=MAX_REPORT_ITEMS-1;	
	}
	else
	{
		iPrevWrite=(pAnyReportLogger->iWrite)-1;
	}

	strncpy(pAnyReportLogger->Item[pAnyReportLogger->iWrite].Text,pMessage,MAX_CHARS_IN_LOGGER_ITEM_TEXT);
	pAnyReportLogger->Item[pAnyReportLogger->iWrite].U32_Item=AnyValue;
	pAnyReportLogger->Item[pAnyReportLogger->iWrite].Colour=AnyColour;
	pAnyReportLogger->Item[pAnyReportLogger->iWrite].IsValid=1;

	if(pAnyReportLogger->iHandle==Environment_Screen.Page)
	{
		if(pAnyReportLogger->Item[iPrevWrite].IsValid)
		{
			Environment_Screen_ReportItem_U32(iPrevWrite, pAnyReportLogger->Item[iPrevWrite].Colour, pAnyReportLogger->Item[iPrevWrite].Text, pAnyReportLogger->Item[iPrevWrite].U32_Item);
		}
		Environment_Screen_ReportItem_U32(pAnyReportLogger->iWrite, VGA_TXT_COLOUR_BLUE, pAnyReportLogger->Item[pAnyReportLogger->iWrite].Text, pAnyReportLogger->Item[pAnyReportLogger->iWrite].U32_Item);
	}

	pAnyReportLogger->iWrite++;
	if(MAX_REPORT_ITEMS<=pAnyReportLogger->iWrite)
	{
		pAnyReportLogger->iWrite=0;
	}
}

void Environment_ReportLoggerRefresh(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger)
{
	int i;
	int iPrevWrite;
	unsigned short AnyColour;

	if(pAnyReportLogger->iHandle==Environment_Screen.Page)
	{
		if(0==pAnyReportLogger->iWrite)
		{
			iPrevWrite=MAX_REPORT_ITEMS-1;	
		}
		else
		{
			iPrevWrite=(pAnyReportLogger->iWrite)-1;
		}

		for(i=0;i<MAX_REPORT_ITEMS; i++)
		{
			if(pAnyReportLogger->Item[i].IsValid)
			{
				if(i==iPrevWrite)
				{
					AnyColour=VGA_TXT_COLOUR_BLUE;
				}
				else
				{
					AnyColour=pAnyReportLogger->Item[i].Colour;
				}

				if(pAnyReportLogger->Use_U8)
				{
					Environment_Screen_ReportItem_U8(i, AnyColour, pAnyReportLogger->Item[i].Text, pAnyReportLogger->Item[i].U8_Item);
				}
				else
				{
					Environment_Screen_ReportItem_U32(i, AnyColour, pAnyReportLogger->Item[i].Text, pAnyReportLogger->Item[i].U32_Item);
				}
			}
			else
			{
				Environment_Screen_ReportItem_MakeVoid(i);
			}
		}
	}
}
