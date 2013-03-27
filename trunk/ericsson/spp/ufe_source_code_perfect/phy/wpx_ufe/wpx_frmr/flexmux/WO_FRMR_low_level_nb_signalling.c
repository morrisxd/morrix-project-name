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


void OMIINO_LLC_Northbound_SignallingMessage(U8 iDevice,  OMIINO_FRAMER_RAM_TYPE * pFramerRAM)
{
    U32 MessageIdentifier=0;
    U8 * pMessage;
    U8  Length;
    U32 U32_Length;

    pMessage=(U8 *)pFramerRAM->Device[iDevice].LLC_NorthboundBuffer.Buffer;
    U32_Length=16+UtilityGetU32(&pMessage[8]);

    if(WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX>=U32_Length)
    {
        Length=(U8)U32_Length;

        if(4<Length)
        {
            MessageIdentifier=UtilityGetU32(&pMessage[0]);

            if(OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_MAX_MESSAGES>MessageIdentifier)
            {
                (*pFramerRAM->LLC_Table.SignallingTable.Parser[MessageIdentifier])(MessageIdentifier, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length);
            }
            else
            {
                if((unsigned int)OMIINO_MESSAGE_CATALOGUE_NULL_MESSAGE!=MessageIdentifier)
                {
                    OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length, __LINE__, __FILE__);
                }
            }
        }
        else
        {
            OMIINO_LLC_Northbound_Signalling_ErrorHandler(0x0000DEAD, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, Length, __LINE__, __FILE__);
        }
    }
    else
    {
        OMIINO_LLC_Northbound_Signalling_ErrorHandler(0x0000DEAD, iDevice, &pFramerRAM->Device[iDevice].Hierarchy, pMessage, 0xFF, __LINE__, __FILE__);
    }
}



   
int OMIINO_K1K2_Differ(WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pFirst_K1K2, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pSecond_K1K2)
{
	int Result=1;

	if(pFirst_K1K2->K1==pSecond_K1K2->K1)
	{
		if(pFirst_K1K2->K2==pSecond_K1K2->K2)
		{
			Result=0;
		}
	}

	return Result;
}

void OMIINO_LLC_Northbound_Signalling_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_K1K2_CHANGE(U8 iDevice, U8 iLinePort, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE * pK1K2)
{
	if(OMIINO_K1K2_Differ(&(pHierarchyRAM->SONET_SDH_Port[iLinePort].Line.K1K2.Status.RX), pK1K2))
	{
		OMIINO_SIGNALLING_WRAPPER_SONET_SDH_SECTION_K1K2_ANNOUNCE_TYPE(iDevice, iLinePort, pK1K2);
		OMIINO_FRAMER_SONET_SDH_Port_Line_K1K2_SetRX(&(pHierarchyRAM->SONET_SDH_Port[iLinePort].Line.K1K2.Status), pK1K2);
	}
}



void OMIINO_LLC_Northbound_Signalling_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_K1K2_CHANGE(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8											Result=1;
    U8											iNextU8=16;
    U8                                          iLinePort;
    WPX_UFE_FRAMER_SONET_SDH_PORT_LINE_K1K2_TYPE K1K2;
    


    if(19!=Length)  /* No Transaction Identifier with Autonomous Signalling Messages */
    {
        Result=0;
    }

    if(Result)
    {
        iLinePort=pMessage[iNextU8++];
        K1K2.K1=pMessage[iNextU8++];
        K1K2.K2=pMessage[iNextU8++];

        OMIINO_LLC_Northbound_Signalling_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_K1K2_CHANGE(iDevice, iLinePort, pHierarchyRAM, &K1K2);
    }
    else
    {
        OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}




  
 
void OMIINO_LLC_Northbound_Signalling_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_S1_CHANGE(U8 iDevice, U8 iLinePort, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 S1)
{
	if(S1!=pHierarchyRAM->SONET_SDH_Port[iLinePort].Line.S1.Status.RX)
	{
		OMIINO_SIGNALLING_WRAPPER_SONET_SDH_SECTION_S1_ANNOUNCE_TYPE(iDevice, iLinePort, S1);
		OMIINO_FRAMER_SONET_SDH_Port_Line_S1_SetRX(&(pHierarchyRAM->SONET_SDH_Port[iLinePort].Line.S1.Status), S1);
	}
}


void OMIINO_LLC_Northbound_Signalling_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_S1_CHANGE(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    U8  Result=1;
    U8  iNextU8=16;
    U8	iLinePort;
    U8	S1;
    


    if(18!=Length)  /* No Transaction Identifier with Autonomous Signalling Messages */
    {
        Result=0;
    }

    if(Result)
    {
        iLinePort=pMessage[iNextU8++];
        S1=pMessage[iNextU8++];

        OMIINO_LLC_Northbound_Signalling_Handle_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_S1_CHANGE(iDevice, iLinePort, pHierarchyRAM, S1);
    }
    else
    {
        OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
    }
}



void OMIINO_LLC_Northbound_Signalling_NULL(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    OMIINO_LLC_Northbound_Signalling_ErrorHandler(MessageIdentifier, iDevice, pHierarchyRAM, pMessage, Length, __LINE__, __FILE__);
}



void OMIINO_LLC_Northbound_Signalling_IGNORE(U32 MessageIdentifier, U8 iDevice, OMIINO_FRAMER_HIERARCHY_TYPE * pHierarchyRAM, U8 * pMessage, U8 Length)
{
    OMIINO_REMOVE_COMPILER_WARNING(MessageIdentifier);
    OMIINO_REMOVE_COMPILER_WARNING(iDevice);
    OMIINO_REMOVE_COMPILER_WARNING(pHierarchyRAM);
    OMIINO_REMOVE_COMPILER_WARNING(pMessage);
    OMIINO_REMOVE_COMPILER_WARNING(Length);
}





void OMIINO_LLC_Northbound_SignallingTableInitialize(OMNISPY_SIGNALLING_TABLE_TYPE * pSignallingTable)
{
    int i;

    for(i=0;i<OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_MAX_MESSAGES;i++)
    {
        pSignallingTable->Parser[i]=OMIINO_LLC_Northbound_Signalling_NULL;
    }

    pSignallingTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_DEFAULT_RAM_IGNORE]=OMIINO_LLC_Northbound_Signalling_IGNORE;
    pSignallingTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_K1K2_CHANGE]=OMIINO_LLC_Northbound_Signalling_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_K1K2_CHANGE;
    pSignallingTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_S1_CHANGE]=OMIINO_LLC_Northbound_Signalling_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_S1_CHANGE;
    pSignallingTable->Parser[OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_SIGNALLING_PORT_ALARM_CHANGE]=OMIINO_LLC_Northbound_Status_OMIINO_NORTHBOUND_MESSAGE_CATALOGUE_STATUS_ALARM;
}
