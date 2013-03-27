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



void OMIINO_LLC_Northbound_PerformanceMonitoring_ErrorHandler(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length, U32 AnyLine, char * AnyFile)
{
	char LocalBuffer[256];

    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	sprintf(LocalBuffer, "PM Error Handler Report at Line %08d in File %s for MessageIdentifier %0xd", AnyLine, AnyFile, MessageIdentifier);
	OMIINO_FRAMER_TransmitMessageTo_OMNISHELL(LocalBuffer);
	OMIINO_FRAMER_Binary_TransmitMessageTo_OMNISHELL((char *)pMessage, Length);
}

void OMIINO_LLC_Northbound_Status_ErrorHandler(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length, U32 AnyLine, char * AnyFile)
{
	char LocalBuffer[256];

    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	sprintf(LocalBuffer, "Status Error Handler Report at Line %08d in File %s for MessageIdentifier %0xd", AnyLine, AnyFile, MessageIdentifier);
	OMIINO_FRAMER_TransmitMessageTo_OMNISHELL(LocalBuffer);
	OMIINO_FRAMER_Binary_TransmitMessageTo_OMNISHELL((char *)pMessage, Length);
}


void OMIINO_LLC_Northbound_Signalling_ErrorHandler(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length, U32 AnyLine, char * AnyFile)
{
	char LocalBuffer[256];

    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	sprintf(LocalBuffer, "Signalling Error Handler Report at Line %08d in File %s for MessageIdentifier %0xd", AnyLine, AnyFile, MessageIdentifier);
	OMIINO_FRAMER_TransmitMessageTo_OMNISHELL(LocalBuffer);
	OMIINO_FRAMER_Binary_TransmitMessageTo_OMNISHELL((char *)pMessage, Length);
}


void OMIINO_LLC_Northbound_Auxiliary_ErrorHandler(U32 MessageIdentifier, U8 iDevice, U8 * pMessage, U8 Length, U32 AnyLine, char * AnyFile)
{
	char LocalBuffer[256];

    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);

	sprintf(LocalBuffer, "Auxiliary Error Handler Report at Line %08d in File %s for MessageIdentifier %0xd", AnyLine, AnyFile, MessageIdentifier);
	OMIINO_FRAMER_TransmitMessageTo_OMNISHELL(LocalBuffer);
	OMIINO_FRAMER_Binary_TransmitMessageTo_OMNISHELL((char *)pMessage, Length);
}

