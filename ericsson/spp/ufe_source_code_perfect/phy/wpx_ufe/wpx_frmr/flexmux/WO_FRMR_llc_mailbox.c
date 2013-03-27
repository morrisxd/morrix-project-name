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


#include <stdlib.h>
#include "WO_FRMR_private.h"



void (*OMIINO_LLC_UtilCheckWordsErrorHandler)(char *)=NULL;

void OMIINO_LLC_BindMailboxCopyErrorHandler(void (*AnyHandler)(char *))
{
	OMIINO_LLC_UtilCheckWordsErrorHandler=AnyHandler;
}


void OMIINO_LLC_CallErrorHandler(char * pMessage)
{
	if(NULL!=OMIINO_LLC_UtilCheckWordsErrorHandler)
	{
		OMIINO_LLC_UtilCheckWordsErrorHandler(pMessage);
	}
}


void OMIINO_LLC_Validate_Checksum(unsigned char *pData, unsigned int Length )
{
	unsigned long checksum = 0 ;
	unsigned long packetChecksum = 0 ;
	unsigned int i ;

	packetChecksum = UtilityGetU32(&pData[Length - 4]);

	for( i = 0 ; i < (Length - 4) ; i++ )
	{
		checksum += pData[i] ;
	}
	checksum += packetChecksum ;	


	if ( 0xffffffff != checksum )
	{
		OMIINO_LLC_CallErrorHandler("[OMIINO] Checksum Failure\n");
	}
}






void OMIINO_LLC_UtilCheckWords(volatile int * pDest, volatile int * pSource, U8 LengthBytes)
{
    int i;
    int Fractions=LengthBytes%4;
    int Words=(LengthBytes/4)+((0==Fractions)?0:1);

    for(i=0;i<Words;i++) /* DMCC NB Can't use memcpy as we've only got U32 access */
    {
        if(*(pDest++)!=*(pSource++))
		{
			OMIINO_LLC_CallErrorHandler("[OMIINO] Copy to Mailbox Failure\n");
		}
    }
}


void OMIINO_LLC_UtilCopyWords(volatile int * pDest, volatile int * pSource, U8 LengthBytes)
{
    int i;
    int Fractions=LengthBytes%4;
    int Words=(LengthBytes/4)+((0==Fractions)?0:1);

    for(i=0;i<Words;i++) /* DMCC NB Can't use memcpy as we've only got U32 access */
    {
        *(pDest++)=*(pSource++);
    }
}

void OMIINO_LLC_API_Transmit(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel, U32 * pMessage, U8 LengthBytes)
{
	/******************************************************************************************/
	/* DMCC 20100901 - 16 bit access to CTRL, can't look for 1 as pattern would be 0x00010001 */
	/* DMCC - CR300                                                                           */
	/******************************************************************************************/
    while(0!=(*pMemoryMap->Address[iCommsChannel].CTRL)){;} /* DMCC TODO Consider Timeout mechanism */
    
	
	OMIINO_LLC_UtilCopyWords((volatile int *)pMemoryMap->Address[iCommsChannel].RAM, (volatile int *)pMessage, LengthBytes);
	OMIINO_LLC_UtilCheckWords((volatile int *)pMemoryMap->Address[iCommsChannel].RAM, (volatile int *)pMessage, LengthBytes);
	
	/* OMIINO_LLC_Validate_Checksum((unsigned char *)&(pMemoryMap->Address[iCommsChannel].RAM[4]), (unsigned int)(LengthBytes-16)); */

    OMIINO_LLC_Register_CTRL_ToggleOwnership(pMemoryMap, iCommsChannel);
}


U8   OMIINO_LLC_API_TryReceive(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel, U32 * pMessage, U8 * pLength)
{
    U8 Result = 0;

    *pLength=0;

    if(0==(*pMemoryMap->Address[iCommsChannel].CTRL))
    {
		/* DMCC 20101109 - copy entire mailbox to aid debug. */
        Result=1;
		*pLength=WPX_UFE_FRAMER_MAX_CHARS_IN_MAILBOX;
        OMIINO_LLC_UtilCopyWords((volatile int *)pMessage, (volatile int *)pMemoryMap->Address[iCommsChannel].RAM, *pLength);
    }

	return Result;
}


void OMIINO_LLC_Register_CTRL_ToggleOwnership(volatile WPX_UFE_FRAMER_MAILBOX_MEMORY_MAP_TYPE *pMemoryMap, U8 iCommsChannel)
{
    if(0x00000000==*pMemoryMap->Address[iCommsChannel].CTRL)
    {
#ifndef XALYO_FRAMER_ACCESS
        *pMemoryMap->Address[iCommsChannel].CTRL=0x00000001;
#else
        volatile U32 *ptr = pMemoryMap->Address[iCommsChannel].CTRL;
        WP_MEM_SET(ptr[0], 0x00000001);
#endif
    }
}
