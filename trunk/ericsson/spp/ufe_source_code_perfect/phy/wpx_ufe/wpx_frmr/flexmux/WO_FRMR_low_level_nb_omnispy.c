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



void OMIINO_LLC_Northbound_OmniSpyMessage(U8 iDevice,  OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    U32 MessageIdentifier=0;
    U8 * pMessage;

	pMessage=(U8 *)pFramerRAM->Device[iDevice].LLC_NorthboundBuffer.Buffer;
    
    MessageIdentifier=UtilityGetU32(&pMessage[0]);
    if(OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_MAX_MESSAGES>MessageIdentifier)
    {
        (*pFramerRAM->LLC_Table.OmnispyTable.Parser[MessageIdentifier])(iDevice);
    }
}




void OMIINO_LLC_Northbound_Omnispy_NULL(U8 iDevice)
{
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
}


void OMIINO_LLC_Northbound_Omnispy_IGNORE(U8 iDevice)
{
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
}


void OMIINO_LLC_Northbound_Omnispy_ToApplication(U8 iDevice)
{
    OMIINO_OMNISPY_WRAPPER_NorthboundAnnounce(iDevice, OMIINO_RAM.Device[iDevice].LLC_NorthboundBuffer.Buffer, WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX);
}

void OMIINO_LLC_Northbound_Omnispy_VirtualScreenNextPage(U8 iDevice)
{
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
	if(OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.VirtualScreen.NextPage)
	{
		(*OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.VirtualScreen.NextPage)();
	}
}


void OMIINO_LLC_Northbound_Omnispy_VirtualScreenPrevPage(U8 iDevice)
{
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
		if(OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.VirtualScreen.PrevPage)
	{
		(*OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.VirtualScreen.PrevPage)();
	}
}


void OMIINO_LLC_Northbound_Omnispy_VirtualScreenRefresh(U8 iDevice)
{
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
		if(OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.VirtualScreen.Refresh)
	{
		(*OMIINO_RAM.OmniSpy_RAM.AutonomousOutputTable.VirtualScreen.Refresh)();
	}
}


void OMIINO_LLC_Northbound_Omnispy_PeekResponse(U8 iDevice)
{
    U8 Result=1;
    U8 * pMessage;
    U32 Address=0;
    U32 Value=0;
    U32 U32_Length;

	pMessage=(U8 *)OMIINO_RAM.Device[iDevice].LLC_NorthboundBuffer.Buffer;
    U32_Length=UtilityGetU32(&pMessage[8]);    
	
    if(8!=U32_Length)  
    {
        Result=0;
    }

	if(Result)
	{
		Address=UtilityGetU32(&pMessage[16]);
		Value=UtilityGetU32(&pMessage[20]);
		OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_PEEK_RESPONSE(iDevice, Address, Value);
	}
}

void OMIINO_LLC_Northbound_Omnispy_BlockPokeVerifyResponse(U8 iDevice)
{
    U8 Result=1;
    U8 * pMessage;
    U32 VerifyResult=0;
    U32 U32_Length;

	pMessage=(U8 *)OMIINO_RAM.Device[iDevice].LLC_NorthboundBuffer.Buffer;
    U32_Length=UtilityGetU32(&pMessage[8]);    

	if(Result)
	{
		VerifyResult=UtilityGetU32(&pMessage[16]);
		OMIINO_DIAGNOSTIC_WRAPPER_ANNOUNCE_BLOCK_POKE_VERIFY_RESPONSE(iDevice, VerifyResult);
	}
}



void OMIINO_LLC_Northbound_OmnispyTableInitialize(OMNISPY_TABLE_TYPE * pOmnispyTable)
{
    int i;

    for(i=0;i<OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_MAX_MESSAGES;i++)
    {
        pOmnispyTable->Parser[i]=OMIINO_LLC_Northbound_Omnispy_NULL;
    }

    pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_DEFAULT_RAM_IGNORE]=OMIINO_LLC_Northbound_Omnispy_IGNORE;
    pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_TO_APPLICATION]=OMIINO_LLC_Northbound_Omnispy_ToApplication;
    pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_VIRTUAL_SCREEN_NEXT_PAGE]=OMIINO_LLC_Northbound_Omnispy_VirtualScreenNextPage;
	pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_VIRTUAL_SCREEN_PREV_PAGE]=OMIINO_LLC_Northbound_Omnispy_VirtualScreenPrevPage;
	pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_VIRTUAL_SCREEN_REFRESH]=OMIINO_LLC_Northbound_Omnispy_VirtualScreenRefresh;
	pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_PEEK_RESPONSE]=OMIINO_LLC_Northbound_Omnispy_PeekResponse;
	pOmnispyTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_OMNISPY_BLOCK_POKE_VERIFY_RESPONSE]=OMIINO_LLC_Northbound_Omnispy_BlockPokeVerifyResponse;
}


		
			


			
