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



SCREEN_DATA_TYPE Environment_Screen;



void Environment_Screen_Calculate_Str_Coordinate(int iItem, COORDINATE_TYPE * pCoordinate)
{
	if(35<iItem)
	{
		pCoordinate->iCol=REPORT_COL_C_STR;
		pCoordinate->iRow=(unsigned char)(REPORT_FIRST_ROW+(iItem-36));
	}
	else
	{
		if(17<iItem)
		{
			pCoordinate->iCol=REPORT_COL_B_STR;
			pCoordinate->iRow=(unsigned char)(REPORT_FIRST_ROW+(iItem-18));
		}
		else
		{
			pCoordinate->iCol=REPORT_COL_A_STR;
			pCoordinate->iRow=(unsigned char)(REPORT_FIRST_ROW+iItem);
		}
	}
}



void Environment_Screen_Calculate_Val_Coordinate(int iItem, COORDINATE_TYPE * pCoordinate)
{
		if(35<iItem)
	{
		pCoordinate->iCol=REPORT_COL_C_VAL;
		pCoordinate->iRow=(unsigned char)(REPORT_FIRST_ROW+(iItem-36));
	}
	else
	{
		if(17<iItem)
		{
			pCoordinate->iCol=REPORT_COL_B_VAL;
			pCoordinate->iRow=(unsigned char)(REPORT_FIRST_ROW+(iItem-18));
		}
		else
		{
			pCoordinate->iCol=REPORT_COL_A_VAL;
			pCoordinate->iRow=(unsigned char)(REPORT_FIRST_ROW+iItem);
		}
	}
}


void Environment_Screen_ReportItem_MakeVoid(int iItem)
{
	COORDINATE_TYPE Any_Str_Coordinate;
	COORDINATE_TYPE Any_Val_Coordinate;

	Environment_Screen_Calculate_Str_Coordinate(iItem, &Any_Str_Coordinate);
	Environment_Screen_Calculate_Val_Coordinate(iItem, &Any_Val_Coordinate);

	VGA_DrawGraphicStr(&Any_Str_Coordinate, VGA_TXT_COLOUR_WHITE, "               ");
	VGA_DrawGraphicStr(&Any_Val_Coordinate, VGA_TXT_COLOUR_WHITE, "        ");
}


void Environment_Screen_ReportItem_U8(int iItem, unsigned short AnyColour, char * pStr, char Value)
{
	COORDINATE_TYPE Any_Str_Coordinate;
	COORDINATE_TYPE Any_Val_Coordinate;

	Environment_Screen_Calculate_Str_Coordinate(iItem, &Any_Str_Coordinate);
	Environment_Screen_Calculate_Val_Coordinate(iItem, &Any_Val_Coordinate);

	VGA_DrawGraphicStr(&Any_Str_Coordinate, AnyColour, pStr);
	VGA_DrawInt_02X(&Any_Val_Coordinate, AnyColour, (unsigned int)Value);
}

void Environment_Screen_ReportItem_U32(int iItem, unsigned short AnyColour, char * pStr, int Value)
{
	COORDINATE_TYPE Any_Str_Coordinate;
	COORDINATE_TYPE Any_Val_Coordinate;

	Environment_Screen_Calculate_Str_Coordinate(iItem, &Any_Str_Coordinate);
	Environment_Screen_Calculate_Val_Coordinate(iItem, &Any_Val_Coordinate);

	VGA_DrawGraphicStr(&Any_Str_Coordinate, AnyColour, pStr);
	VGA_DrawInt_08X(&Any_Val_Coordinate, AnyColour, Value);
}



void Environment_Screen_LoadTitle(void)
{

	switch(Environment_Screen.Page)
	{
		case	PAGE_SIGNALLING:
				VGA_DrawGraphicStr(&Environment_Screen.Coordinate.Title, VGA_TXT_COLOUR_RED, Environment_Screen.Strings.TitleSignallingStr);
				break;

		case	PAGE_ALARMS:
				VGA_DrawGraphicStr(&Environment_Screen.Coordinate.Title, VGA_TXT_COLOUR_RED, Environment_Screen.Strings.TitleAlarmsStr);
				break;

		case	PAGE_PERFORMANCE_MONITORING:
				VGA_DrawGraphicStr(&Environment_Screen.Coordinate.Title, VGA_TXT_COLOUR_RED, Environment_Screen.Strings.TitlePerformanceMonitoringStr);
				break;

		default:
				VGA_DrawGraphicStr(&Environment_Screen.Coordinate.Title, VGA_TXT_COLOUR_RED, Environment_Screen.Strings.TitleSignallingStr);
				break;
	}
}




void MakeScreen(void)
{

	VGA_FORMATTER_VIRTUAL_VGA_BEGIN_ATOMIC_SEQUENCE_MESSAGE();
	VGA_ClearScreen();
	VGA_ScreenTemplate();
	Environment_Screen_LoadTitle();
	Environment_ReportLoggerRefresh(&Environment_Screen.Logger[Environment_Screen.Page]);
	VGA_FORMATTER_VIRTUAL_VGA_END_ATOMIC_SEQUENCE_MESSAGE();
}



void Environment_Screen_NextPage(void)
{
	switch(Environment_Screen.Page)
	{
		case	PAGE_SIGNALLING:
				Environment_Screen.Page=PAGE_ALARMS;
				break;

		case	PAGE_ALARMS:
				Environment_Screen.Page=PAGE_PERFORMANCE_MONITORING;
				break;

		case	PAGE_PERFORMANCE_MONITORING:
				Environment_Screen.Page=PAGE_SIGNALLING;
				break;

		default:
				Environment_Screen.Page=PAGE_SIGNALLING;
				break;
	}

	MakeScreen();
}

void Environment_Screen_PrevPage(void)
{
	switch(Environment_Screen.Page)
	{
		case	PAGE_SIGNALLING:
				Environment_Screen.Page=PAGE_PERFORMANCE_MONITORING;
				break;

		case	PAGE_ALARMS:
				Environment_Screen.Page=PAGE_SIGNALLING;
				break;

		case	PAGE_PERFORMANCE_MONITORING:
				Environment_Screen.Page=PAGE_ALARMS;
				break;

		default:
				Environment_Screen.Page=PAGE_SIGNALLING;
				break;
	}

	MakeScreen();
}


void Environment_Screen_Refresh(void)
{
	MakeScreen();
}


void Environment_Screen_InitializeStrings(void)
{
	Environment_Screen.Strings.TitleSignallingStr					=	"Device Driver (Signalling)";
	Environment_Screen.Strings.TitleAlarmsStr						=	"  Device Driver (Alarms)  ";
	Environment_Screen.Strings.TitlePerformanceMonitoringStr		=	"    Device Driver (PM)    ";
}

void Environment_Screen_Initialize(void)
{
	Environment_Screen.Magic1=0xAAAAAAA1;
	Environment_Screen.Magic2=0xAAAAAAA2;
	Environment_Screen.Magic3=0xAAAAAAA3;

	Environment_Screen_InitializeStrings();

	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.BoxTopLeft),				0, 0);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.BoxBottomRight),			79, 24);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.HorizontalLineLeft),		1, 2);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.HorizontalLineRight),	78, 2);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.TBarLeft),				0, 2);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.TBarRight),				79, 2);

	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.LowHorizontalLineLeft),		1, 22);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.LowHorizontalLineRight),		78, 22);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.LowTBarLeft),				0, 22);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.LowTBarRight),				79, 22);

	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.VariableTraceName),		 2, 23);
	VGA_MakeCoordinate(&(Environment_Screen.Coordinate.VariableTraceValue),		20, 23);

	VGA_MakeCoordinate(&Environment_Screen.Coordinate.Title, 24, 1);

	Environment_Screen.Page=PAGE_SIGNALLING;

	Environment_Screen_Alarms_Initialize();
	Environment_Screen_PM_Initialize();
	Environment_Screen_Signalling_Initialize();

}

void Environment_Screen_Initialize_Bindings(WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_VIRTUAL_SCREEN_TABLE_TYPE * pVirtualScreenTable)
{
	pVirtualScreenTable->NextPage=Environment_Screen_NextPage;
	pVirtualScreenTable->PrevPage=Environment_Screen_PrevPage;
	pVirtualScreenTable->Refresh=Environment_Screen_Refresh;
}


