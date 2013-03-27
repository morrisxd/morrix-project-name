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








void VGA_FORMATTER_VIRTUAL_VGA_BEGIN_ATOMIC_SEQUENCE_MESSAGE(void)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{	
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_BEGIN_ATOMIC_SEQUENCE_MESSAGE, 0, 0, 0);
		piAddAt+=16;
		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_2X_MESSAGE(COORDINATE_TYPE * pAnyCoordinate, int TheColour, int TheInt)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		piAddAt+=4;
		Length=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_2X_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(pAnyCoordinate->iCol, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(pAnyCoordinate->iRow, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheColour, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheInt, piAddAt);
		piAddAt+=4;
		Length+=4;

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_3X_MESSAGE(COORDINATE_TYPE * pAnyCoordinate, int TheColour, int TheInt)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_3X_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(pAnyCoordinate->iCol, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(pAnyCoordinate->iRow, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheColour, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheInt, piAddAt);
		piAddAt+=4;
		Length+=4;

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_4X_MESSAGE(COORDINATE_TYPE * pAnyCoordinate, int TheColour, int TheInt)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_4X_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(pAnyCoordinate->iCol, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(pAnyCoordinate->iRow, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheColour, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheInt, piAddAt);
		piAddAt+=4;
		Length+=4;

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}

void VGA_FORMATTER_VIRTUAL_VGA_8X_MESSAGE(COORDINATE_TYPE * pAnyCoordinate, int TheColour, int TheInt)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_8X_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(pAnyCoordinate->iCol, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(pAnyCoordinate->iRow, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheColour, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheInt, piAddAt);
		piAddAt+=4;
		Length+=4;

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_GRAPHIC_CHAR_MESSAGE(COORDINATE_TYPE * pAnyCoordinate, int TheColour, char TheChar)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_GRAPHIC_CHAR_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(pAnyCoordinate->iCol, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(pAnyCoordinate->iRow, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheColour, piAddAt);
		piAddAt+=4;
		Length+=4;

		*piAddAt=TheChar;
		piAddAt+=1;
		Length+=1;

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_GRAPHIC_STR_MESSAGE(COORDINATE_TYPE * pAnyCoordinate, int TheColour, char TheStrLength, char * pMessage)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;
	int				i;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_GRAPHIC_STR_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(pAnyCoordinate->iCol, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(pAnyCoordinate->iRow, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheColour, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheStrLength, piAddAt);
		piAddAt+=4;
		Length+=4;

		for(i=0;i<TheStrLength;i++)
		{
			*piAddAt=pMessage[i];
			piAddAt++;
			Length++;
		}

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_VAR_TRACE_MESSAGE(int TheInt, char TheStrLength, char * pMessage)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;
	int				i;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_VAR_TRACE_MESSAGE, 0, 0, 0);
		piAddAt+=16;

		UtilityPutU32(TheInt, piAddAt);
		piAddAt+=4;
		Length+=4;

		UtilityPutU32(TheStrLength, piAddAt);
		piAddAt+=4;
		Length+=4;

		for(i=0;i<TheStrLength;i++) /* memcpy doesn't work with mailboxes */
		{
			*piAddAt=pMessage[i];
			piAddAt++;
			Length++;
		}

		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_CLEAR_SCREEN_MESSAGE(void)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_CLEAR_SCREEN_MESSAGE, 0, 0, 0);
		piAddAt+=16;
		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_SCREEN_TEMPLATE_MESSAGE(void)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_SCREEN_TEMPLATE_MESSAGE, 0, 0, 0);
		piAddAt+=16;
		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}


void VGA_FORMATTER_VIRTUAL_VGA_END_ATOMIC_SEQUENCE_MESSAGE(void)
{
	U32				Buffer[1+(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX/4)]; /* Warning, Large Local */
    U8 *			piAddAt=(U8 *)Buffer;
	U8				Length=0;

	if(OMIINO_RAM.Device[0].LLC_AccessIsPermitted) /* Support for first device only */
	{
		UtilityPutU32(0, piAddAt); /* Padding */
		Length=4;
		piAddAt+=4;
		Length+=OMIINO_LLC_SOUTHBOUND_FORMATTER_EncodeHeader((unsigned int *)piAddAt, MESSAGE_ID_OMNISPY_SOUTHBOUND_VIRTUAL_VGA_END_ATOMIC_SEQUENCE_MESSAGE, 0, 0, 0);
		piAddAt+=16;
		OMIINO_LLC_SOUTHBOUND_FORMATTER_GivenOverallLengthSetHeaderLengthField(1+Buffer, Length-4);
		OMIINO_LLC_API_Transmit(&OMIINO_RAM.Device[0].MemoryMap, WPX_UFE_FRAMER_UNI_DIRECTIONAL_COMMS_CHANNEL_SOUTHBOUND_OMNISPY, Buffer, Length);
	}
}







void VGA_MakeCoordinate(COORDINATE_TYPE * pAnyCoordinate, unsigned char iCol, unsigned char iRow)
{
    pAnyCoordinate->iCol=iCol;
    pAnyCoordinate->iRow=iRow;
}


void VGA_CopyCoordinate(COORDINATE_TYPE * pDestinationCoordinate, COORDINATE_TYPE * pSourceCoordinate)
{
    pDestinationCoordinate->iCol=pSourceCoordinate->iCol;
    pDestinationCoordinate->iRow=pSourceCoordinate->iRow;
}






void VGA_ClearScreen(void)
{
	VGA_FORMATTER_VIRTUAL_VGA_CLEAR_SCREEN_MESSAGE();
}


void VGA_ScreenTemplate(void)
{
	VGA_FORMATTER_VIRTUAL_VGA_SCREEN_TEMPLATE_MESSAGE();
}

void VGA_DrawInt_02X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt)
{
	VGA_FORMATTER_VIRTUAL_VGA_2X_MESSAGE(pCoordinate, AnyColour, TheInt);
}


void VGA_DrawInt_03X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt)
{
    VGA_FORMATTER_VIRTUAL_VGA_3X_MESSAGE(pCoordinate, AnyColour, TheInt);
}



void VGA_DrawInt_04X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt)
{
   VGA_FORMATTER_VIRTUAL_VGA_4X_MESSAGE(pCoordinate, AnyColour, TheInt);
}


void VGA_DrawInt_08X(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, unsigned int TheInt)
{
    VGA_FORMATTER_VIRTUAL_VGA_8X_MESSAGE(pCoordinate, AnyColour, TheInt);
}



void VGA_DrawGraphicChar(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, char GraphicChar)
{
   VGA_FORMATTER_VIRTUAL_VGA_GRAPHIC_CHAR_MESSAGE(pCoordinate, AnyColour, GraphicChar);
}


void VGA_DrawGraphicStr(COORDINATE_TYPE * pCoordinate, unsigned short AnyColour, char *pStr)
{
	VGA_FORMATTER_VIRTUAL_VGA_GRAPHIC_STR_MESSAGE(pCoordinate, AnyColour, (char)strlen(pStr), pStr);
}


void VGA_DrawVariableTrace(char *pStr, unsigned int AnyValue)
{
	VGA_FORMATTER_VIRTUAL_VGA_VAR_TRACE_MESSAGE(AnyValue, (char)strlen(pStr), pStr);
}












