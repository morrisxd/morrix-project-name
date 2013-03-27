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

#ifndef _UFE4_VGA_
#define _UFE4_VGA_



#ifdef __cplusplus
extern "C"
{
#endif

#include "WO_FRMR_private.h"

/*
 *
 *------------------------------------------
 *
 *  VGA Driver 
 *
 *------------------------------------------
 *
 */






typedef struct COORDINATE_TYPE
{
    unsigned char iCol;
    unsigned char iRow;

} COORDINATE_TYPE;


#define REPORT_FIRST_ROW				(4)
#define REPORT_COL_A_STR				(2)
#define REPORT_COL_A_VAL				(18)
#define REPORT_COL_B_STR				(27)
#define REPORT_COL_B_VAL				(43)
#define REPORT_COL_C_STR				(53)
#define REPORT_COL_C_VAL				(69)


#define GRAPHIC_TOP_LEFT_CHAR			(0x10)
#define GRAPHIC_TOP_RIGHT_CHAR			(0x11)
#define GRAPHIC_BOTTOM_RIGHT_CHAR		(0x13)
#define GRAPHIC_BOTTOM_LEFT_CHAR		(0x12)
#define GRAPHIC_T_TOP_CHAR				(0x18)
#define GRAPHIC_T_RIGHT_CHAR			(0x19)
#define GRAPHIC_T_BOTTOM_CHAR			(0x1A)
#define GRAPHIC_T_LEFT_CHAR				(0x1B)
#define GRAPHIC_CROSS_HAIR_CHAR			(0x1C)
#define GRAPHIC_VERTICAL_LINE_CHAR		(0x1D)
#define GRAPHIC_HORIZONTAL_LINE_CHAR	(0x1E)


#define VGA_TXT_COLOUR_WHITE	        (0x0000)
#define VGA_TXT_COLOUR_RED		        (0x0080)
#define VGA_TXT_COLOUR_YELLOW	        (0x0100)
#define VGA_TXT_COLOUR_BLUE		        (0x0180)

#define VCG_MAX_COL				        (80)
#define VCG_MAX_ROW				        (25)
#define VCG_SCREEN_CHARS                (VCG_MAX_COL*VCG_MAX_ROW)


void VGA_ClearScreen(void);
void VGA_ScreenTemplate(void);
void VGA_MakeCoordinate(COORDINATE_TYPE * pAnyCoordinate, unsigned char iCol, unsigned char iRow);
void VGA_CopyCoordinate(COORDINATE_TYPE * pDestinationCoordinate, COORDINATE_TYPE * pSourceCoordinate);
void VGA_DrawInt_02X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt);
void VGA_DrawInt_03X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt);
void VGA_DrawInt_04X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt);
void VGA_DrawInt_08X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt);
void VGA_DrawGraphicChar(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, char GraphicChar);
void VGA_DrawGraphicStr(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, char *pStr);
void VGA_DrawHorizontalLine(COORDINATE_TYPE * pLeftCoordinate, COORDINATE_TYPE * pRightCoordinate, unsigned short AnyColour, char GraphicChar);
void VGA_DrawVerticalLine(COORDINATE_TYPE * pTopCoordinate, COORDINATE_TYPE * pBottomCoordinate, unsigned short AnyColour, char GraphicChar);
void VGA_DrawBox(COORDINATE_TYPE * pTopLeftCoordinate, COORDINATE_TYPE * pBottomRightCoordinate, unsigned short AnyColour);
void VGA_DrawVariableTrace(char *pStr, unsigned int AnyValue);

void VGA_FORMATTER_VIRTUAL_VGA_BEGIN_ATOMIC_SEQUENCE_MESSAGE(void);
void VGA_FORMATTER_VIRTUAL_VGA_END_ATOMIC_SEQUENCE_MESSAGE(void);


#define MAX_REPORT_ITEMS				(54)
#define MAX_CHARS_IN_LOGGER_ITEM_TEXT	(15)

typedef struct ENVIRONMENT_REPORT_LOGGER_ITEM_TYPE
{
	char								IsValid;
	char								U8_Item;
	unsigned short						Colour;
	int									U32_Item;
	char								Text[1+MAX_CHARS_IN_LOGGER_ITEM_TEXT];

} ENVIRONMENT_REPORT_LOGGER_ITEM_TYPE;

typedef struct ENVIRONMENT_REPORT_LOGGER_TYPE
{
	char									iHandle;
	char									Use_U8;
	int										iWrite;
	int										Magic4;
	ENVIRONMENT_REPORT_LOGGER_ITEM_TYPE		Item[MAX_REPORT_ITEMS];

} ENVIRONMENT_REPORT_LOGGER_TYPE;



typedef struct SCREEN_COORDINATE_TYPE
{
    COORDINATE_TYPE							BoxTopLeft;	
    COORDINATE_TYPE							BoxBottomRight; 
    COORDINATE_TYPE							HorizontalLineLeft; 
    COORDINATE_TYPE							HorizontalLineRight;	
    COORDINATE_TYPE							TBarLeft;	
    COORDINATE_TYPE							TBarRight;

	COORDINATE_TYPE							LowHorizontalLineLeft; 
    COORDINATE_TYPE							LowHorizontalLineRight;	
    COORDINATE_TYPE							LowTBarLeft;	
    COORDINATE_TYPE							LowTBarRight;

	COORDINATE_TYPE							VariableTraceName;
	COORDINATE_TYPE							VariableTraceValue;

	COORDINATE_TYPE							Title;

} SCREEN_COORDINATE_TYPE;


typedef struct SCREEN_STRINGS_TYPE                
{
	char *									TitleSignallingStr;
	char *									TitleAlarmsStr;				
	char *									TitlePerformanceMonitoringStr;
} SCREEN_STRINGS_TYPE;

#define PAGE_SIGNALLING						(0)
#define PAGE_ALARMS							(1)
#define PAGE_PERFORMANCE_MONITORING			(2)
#define PAGE_MAX							(3)

typedef struct SCREEN_DATA_TYPE                
{
	int										Magic1;
	SCREEN_STRINGS_TYPE						Strings;
    int										Page;
	int										Magic2;
	SCREEN_COORDINATE_TYPE					Coordinate;
	int										Magic3;
	ENVIRONMENT_REPORT_LOGGER_TYPE			Logger[PAGE_MAX];

} SCREEN_DATA_TYPE;


void Environment_Screen_ReportItem_MakeVoid(int iItem);
void Environment_Screen_ReportItem_U8(int iItem, unsigned short AnyColour, char * pStr, char Value);
void Environment_Screen_ReportItem_U32(int iItem, unsigned short AnyColour, char * pStr, int Value);
void Environment_Screen_Initialize(void);
void Environment_Screen_Initialize_Bindings(WPX_UFE_FRAMER_OMNISPY_NORTHBOUND_VIRTUAL_SCREEN_TABLE_TYPE * pVirtualScreenTable);



void Environment_ReportLoggerInitialize(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger, char AnyHandle, char Use_U8);
void Environment_ReportLoggerAddItem_U8(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger, char *pMessage, unsigned short AnyColour, U8 AnyValue);
void Environment_ReportLoggerAddItem_U32(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger, char *pMessage, unsigned short AnyColour, U32 AnyValue);
void Environment_ReportLoggerRefresh(ENVIRONMENT_REPORT_LOGGER_TYPE * pAnyReportLogger);


U8 OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader(U32 * pFormattedMessage, U32 Field_SouthboundMessageID, U32 Field_TransactionIdentifier, U32 Field_PayloadLength, U32 Field_Attributes);
void OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(U32 *pHeader, U8 OverallLength);


void Environment_Screen_Signalling_S1_Report(char iLinePort, char Any_S1);
void Environment_Screen_Signalling_K1K2_Report(char iLinePort, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE *pRX_K1K2);


void Environment_Screen_Signalling_Initialize(void);
void Environment_Screen_Alarms_Initialize(void);
void Environment_Screen_PM_Initialize(void);
void Environment_Screen_PM_Port_Report(char iLinePort, char iPoint, U32 AnyValue);
void Environment_Screen_PM_Port_Header(char * pAnyMessage, U32 AnyValue);


void Environment_Screen_Alarms_PortAlarmAnnounce(U8 iDevice, U8 iLinePort, U8 AlarmCategory, U8 IsAsserted);
void Environment_Screen_Alarms_PathAlarmAnnounce(U8 iDevice, WPX_UFE_FRAMER_COMMON_SDH_SONET_ENDPOINT_TYPE * pLineEndpoint, U8 AlarmCategory, U8 IsAsserted);

extern SCREEN_DATA_TYPE Environment_Screen;

#ifdef __cplusplus
}
#endif

#endif /* _UFE4_VGA_ Include this file only once */

